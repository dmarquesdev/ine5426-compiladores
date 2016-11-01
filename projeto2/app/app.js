import React from 'react';
import ReactDOM from 'react-dom';
import esprima from 'esprima';

import { Editor } from './components';

class AppContainer extends React.Component {
  constructor(props) {
  	super(props);
  	this.handleCodeChange = this.handleCodeChange.bind(this);
  	this.handleKeyStroke = this.handleKeyStroke.bind(this);
  	this.state = {code: '', parsed: {}};
  }

  render() {
    return (
      <div>
        <Editor handleChange={this.handleCodeChange}
        	handleKey={this.handleKeyStroke}
        	code={this.state.code} />
      </div>
    );
  }

  handleCodeChange(e) {
  	this.setState({code: e.target.value});
  }

  handleKeyStroke(e) {
  	if(e.key == 'Enter') {
      alert("code: "+ this.state.code);
  		this.setState({parsed: esprima.parse(this.state.code)});
  	}
  }
}

ReactDOM.render(<AppContainer />, document.getElementById('root'));
