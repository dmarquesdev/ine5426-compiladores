import React from 'react';
import ReactDOM from 'react-dom';

import { Editor } from './components';

class AppContainer extends React.Component {
  render() {
    return (
      <div>
        <Editor />
      </div>
    );
  }
}

ReactDOM.render(<AppContainer />, document.getElementById('root'));
