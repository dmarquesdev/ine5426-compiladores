import React from 'react';
import ReactDOM from 'react-dom';
import esprima from 'esprima';

import { Editor } from './components';

class AppContainer extends React.Component {
  constructor(props) {
  	super(props);
  	this.handleCodeChange = this.handleCodeChange.bind(this);
  	this.handleKeyStroke = this.handleKeyStroke.bind(this);
  	this.state = {code: '', parsed: {}, tokens: {}};
  }

  render() {
    return (
      <div>
        <Editor handleChange={this.handleCodeChange}
        	handleKey={this.handleKeyStroke}
        	code={this.state.code} />
        <div>
          {JSON.stringify(this.state.parsed)}
        </div>
        <div>
          {JSON.stringify(this.state.tokens)}
        </div>
      </div>
    );
  }

  handleCodeChange(e) {
  	this.setState({code: e.target.value});
  }

  handleKeyStroke(e) {
    this.setState({
      tokens: esprima.tokenize(this.state.code)
    });

  	if(e.key == 'Enter') {
  		this.setState({
        parsed: esprima.parse(this.state.code, { sourceType: 'module' })
      });
  	}
  }
}

ReactDOM.render(<AppContainer />, document.getElementById('root'));
