import React from 'react';
import ReactDOM from 'react-dom';
import esprima from 'esprima';

import { translate, clearPreview, checkSemantic, colorize } from './util';

class AppContainer extends React.Component {

	

  constructor(props) {
  	super(props);

  	this.state = {
      parsed: {},
      tokens: [],
      code: '',
      errorDetected: false,
      errors: [],
      html: '',
      jsxcode: ''
    };

    this.handleChange = this.handleChange.bind(this);
    this.handleKeyPress = this.handleKeyPress.bind(this);
  }



  render() {
    var errors = [];
    var count = 0;

    if(this.state.errorDetected && this.state.errors.length) {
      this.state.errors.forEach((elem, i) => {
        errors.push(<p key={"error"+(++count)}>{elem}</p>);
      });
    }

    return (
      <div>
        <div className="messages">
          {errors}
        </div>

        <div className="content-block">
          <h5 className="content-block-header">Editor</h5>
          <textarea value={this.state.code} onChange={this.handleChange}
            onKeyPress={this.handleKeyPress} id="editor" />
        </div>

        <div className="content-block debug-block">
          <h5 className="content-block-header">Tokenization</h5>
          <div>{colorize(this.state.tokens, this.state.jsxcode)}</div>
        </div>

        <div className="content-block" id="preview-block">
          <h5 className="content-block-header">Preview</h5>
          <div className="mobile">
            <div id="preview" />
          </div>
        </div>

        <div className="content-block debug-block">
          <h5 className="content-block-header">AST</h5>
          <pre id="debug-ast">
            {JSON.stringify(this.state.parsed, null, 2)}
            </pre>
        </div>

        <div className="content-block debug-block">
          <h5 className="content-block-header">Tokens</h5>
          <pre id="debug-tokens">
            {JSON.stringify(this.state.tokens, null, '\t')}
          </pre>
        </div>

        <div className="content-block debug-block">
          <h5 className="content-block-header">HTML</h5>
          <pre id="debug-html">
            {this.state.html}
          </pre>
        </div>
      </div>
    );
  }

  handleChange(e) {
    this.setState({
      code: e.target.value
    });
  }

  handleKeyPress(e) {
    clearPreview(document.getElementById("preview"));

    var jsxpattern = /<[a-zA-Z]+>(.*|\s*)*<\/[a-zA-Z]+>/g;
    var codewithoutjsx = this.state.code.replace(jsxpattern, "'__JSX_CODE__'");

    try {
      this.setState({
        parsed: esprima.parse(this.state.code,
          {jsx: true, sourceType: 'module'}),
        tokens: esprima.tokenize(codewithoutjsx,
        {loc: true, range: true, tolerant: true}),
        jsxcode: jsxpattern.exec(this.state.code),
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
      document
        .getElementById("preview")
        .appendChild(translate(this.state.parsed));

      this.setState({
        html: document
        .getElementById('preview')
        .firstChild.innerHTML
      });

    } catch(err) {
      this.setState({
        errorDetected: true
      });

      if(err != 'Semantic Error!') {
        this.setState({
          errors: ["Error: Line " + err.lineNumber + ": " + err.description]
        })
      }
    }
  }
}

ReactDOM.render(<AppContainer />, document.getElementById('root'));
