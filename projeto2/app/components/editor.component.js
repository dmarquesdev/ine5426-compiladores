import React from 'react';

import esprima from 'esprima';

class Editor extends React.Component {
  render() {
    return (
    	<div>
      		<textarea id="editor-textarea" value={this.props.code}
      			onChange={this.props.handleChange}
      			onKeyPress={this.props.handleKey}
      			style={editorStyle} />
      </div>
    );
  }
}

const editorStyle = {
  'width': '100%',
  'height': '100%',
  'border': 'none',
  'background-color': '#333',
  'color': '#EEE'
}

export default Editor;
