import React from 'react';
import ReactDOM from 'react-dom';
import esprima from 'esprima';

import { EditorState, CompositeDecorator } from 'draft-js';

import { NativeEditor } from './components';

class AppContainer extends React.Component {
  constructor(props) {
  	super(props);

    const decorator = new CompositeDecorator([
      {
        strategy: handleToken,
        component: TokenSpan
      }
    ]);

  	this.onChange = (editorState) => this.setState({editorState});
    this.handleReturn = this.handleReturn.bind(this);
  	this.state = {
      parsed: {},
      tokens: {},
      editorState: EditorState.createEmpty(decorator)
    };
  }

  render() {
    const {editorState} = this.state;

    return (
      <div>
        <NativeEditor editorState={editorState}
          handleReturn={this.handleReturn}
          onChange={this.onChange} />
        <div>
          {JSON.stringify(this.state.parsed)}
        </div>
        <div>
          {JSON.stringify(this.state.tokens)}
        </div>
      </div>
    );
  }

  handleReturn(e) {
    const {editorState} = this.state;
    const content = editorState.getCurrentContent();
    this.setState({parsed: esprima.parse(content.getSelection().get)});
  }
}

function handleToken(contentState, contentBlock, callback) {
  // INPUT TOKEN PARSE AND STRATEGY FOR HIGHLIGHT HERE
}

const TokenSpan = (props) => {
  // THIS IS THE COMPONENT RENDERED WHEN A TOKEN IS DETECTED
  return (
    <span style="color: #F00;">{props.children}</span>
  );
}

ReactDOM.render(<AppContainer />, document.getElementById('root'));
