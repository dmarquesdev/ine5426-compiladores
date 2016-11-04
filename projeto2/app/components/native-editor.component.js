import React from 'react';

import { Editor } from 'draft-js';

class NativeEditor extends React.Component {
  render() {
    return (
      <div style={editorStyle}>
    	 <Editor editorState={this.props.editorState}
          onChange={this.props.onChange} />
      </div>
    );
  }
}

const editorStyle = {
  'width': '100vw',
  'height': '50vh',
  'border': 'none',
  'backgroundColor': '#333',
  'color': '#EEE',
  'resize': 'none',
  'position': 'fixed',
  'left': '0',
  'top': '0'
}

export default Editor;
