import React from 'react';
import ReactDOM from 'react-dom';
import esprima from 'esprima';

import { NativeEditor } from './components';

import { translate, clearPreview, checkSemantic } from './util';

class AppContainer extends React.Component {
  constructor(props) {
  	super(props);

  	this.state = {
      parsed: {},
      tokens: {},
      code: '',
      errorDetected: false,
      errors: []
    };

    this.handleChange = this.handleChange.bind(this);
    this.handleKeyPress = this.handleKeyPress.bind(this);
  }

  render() {
    var errors = [];
    var count = 0;

    if(this.state.errorDetected) {
      this.state.errors.forEach((elem, i) => {
        errors.push(<p key={"error"+(++count)}>{elem}</p>);
      });
    }

    return (
      <div>
        {errors}
        <textarea value={this.state.code} onChange={this.handleChange}
          onKeyPress={this.handleKeyPress} />
        <div id="preview" />
      </div>
    );
  }

  handleChange(e) {
    this.setState({
      code: e.target.value
    });
  }

  handleKeyPress(e) {
    try {
      this.setState({
        parsed: esprima.parse(this.state.code,
          {jsx: true, sourceType: 'module'}),
        errorDetected: false
      });

      try {
        checkSemantic(this.state.parsed);
      } catch(err) {
        this.setState({
          errors: err
        });

        throw "Semantic Error!";
      }

      console.log(JSON.stringify(this.state.parsed, null, '\t'));
      document
        .getElementById("preview")
        .appendChild(translate(this.state.parsed));
    } catch(err) {
      this.setState({
        errorDetected: true
      });

      if(err != 'Semantic Error!') {
        this.setState({
          errors: ["Error: Line " + err.lineNumber + ": " + err.description]
        })
      }

      clearPreview(document.getElementById("preview"));
    }
  }
}

ReactDOM.render(<AppContainer />, document.getElementById('root'));
