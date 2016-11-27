import React from 'react';
import ReactDOM from 'react-dom';
import esprima from 'esprima';

import { NativeEditor } from './components';

import { translate } from './util';

class AppContainer extends React.Component {
  constructor(props) {
  	super(props);

  	this.state = {
      parsed: {},
      tokens: {},
      code: ''
    };

    this.handleChange = this.handleChange.bind(this);
    this.handleKeyPress = this.handleKeyPress.bind(this);
  }

  render() {

    return (
      <div>
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
    if(e.key == 'Enter') {
      this.setState({
        parsed: esprima.parse(this.state.code, {jsx: true, sourceType: 'module'})
      });

      console.log(JSON.stringify(this.state.parsed, null, '\t'));
      document
        .getElementById("preview")
        .appendChild(translate(this.state.parsed));
    }
  }
}

ReactDOM.render(<AppContainer />, document.getElementById('root'));
