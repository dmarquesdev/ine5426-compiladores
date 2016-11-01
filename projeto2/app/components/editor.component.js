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
  'width': '100vw',
  'height': '100vh',
  'border': 'none',
  'backgroundColor': '#333',
  'color': '#EEE',
  'resize': 'none',
  'position': 'fixed',
  'left': '0',
  'top': '0'
}

export default Editor;
