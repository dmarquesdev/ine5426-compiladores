import { walk } from 'esprima-walk';
import React from 'react';

const path = {
  mapView: 'assets/images/mapview.jpg'
}

const ComponentMap = {};
ComponentMap['Button'] = (<button className="button" />);
ComponentMap['DrawerLayoutAndroid'] = (<div className="drawer-android" />);
ComponentMap['Image'] = (<img className="image" />);
ComponentMap['ListView'] = (<ul className="list-view" />);
ComponentMap['MapView'] = (<img src={path.mapView} />);
ComponentMap['Modal'] = (<div className="modal" />);
ComponentMap['ScrollView'] = (<div className="scroll-view" />);
ComponentMap['Slider'] = (<input type="range" className="slider" />);
ComponentMap['Switch'] = (<span className="switch" />);
ComponentMap['TabBar'] = (<div className="tab-bar" />);
ComponentMap['Text'] = (<span className="text" />);
ComponentMap['TextInput'] = (<input className="text-input" />);
ComponentMap['View'] = (<div className="view" />);
ComponentMap['Alert'] = (<div className="alert" />);

function rebuildTree(dict, parent) {
  if(!parent) {
    return null;
  }

  var children = [];
  for(var i = 0; i < parent.children.length; i++) {
    var child = parent.children[i];
    if(child.type != 'JSXElement') {
      continue;
    }

    children.push(
      <child>
        {rebuildTree(dict, child)}
      </child>
    );
  }

  return (
    <parent>
      {children}
    </parent>
  )
}

export function translate(parsed) {
  var components = [];
  var root = null;

  // Gather all components from JSX
  walk(parsed, (node) => {
    if(node.type == 'JSXElement') {
      if(!root) {
        root = node;
      }
      components.push(node);
    }
  });

  var translateDict = {};

  // Translate all React Native components to equivalent in pure JSX
  for(var i = 0; i < components.length; i++) {
    var component = components[i];
    const name = component.openingElement.name.name;
    translateDict[component] = ComponentMap[name];
  }

  // Rebuild tree
  return rebuildTree(translateDict, root);
}
