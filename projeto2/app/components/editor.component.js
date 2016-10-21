import React from 'react';

class Editor extends React.Component {
  render() {
    return (
      <textarea id="editor-textarea" style={{'width': '100%', 'height': '100%'}} />
    );
  }
}

export default Editor;
