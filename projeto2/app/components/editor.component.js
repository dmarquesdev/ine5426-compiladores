import React from 'react';

import esprima from 'esprima';

class Editor extends React.Component {
  render() {
    return (
    	<div>
      		<textarea id="editor-textarea" value={this.props.code} 
      			onChange={this.props.handleChange} 
      			onKeyDown={this.props.handleKey}
      			style={{'width': '100%', 'height': '100%'}} />
      </div>
    );
  }
}

export default Editor;
