import { walkAddParent } from 'esprima-walk';
import React from 'react';

const path = {
  mapView: 'assets/images/mapview.jpg'
}

const ComponentMap = [
  {
    rn: 'Button',
    html: <button className="button" />
  },

  {
    rn: 'DrawerLayoutAndroid',
    html: <div className="drawer-android" />
  },

  {
    rn: 'Image',
    html: <img className="image" />
  },

  {
    rn: 'ListView',
    html: <ul className="list-view" />
  },

  {
    rn: 'MapView',
    html: <img src={path.mapView} />
  },

  {
    rn: 'Modal',
    html: <div className="modal" />
  },

  {
    rn: 'ScrollView',
    html: <div className="scroll-view" />
  },

  {
    rn: 'Slider',
    html: <input type="range" className="slider" />
  },

  {
    rn: 'Switch',
    html: <span className="switch" />
  },

  {
    rn: 'TabBar',
    html: <div className="tab-bar" />
  },

  {
    rn: 'Text',
    html: <span className="text" />
  },

  {
    rn: 'TextInput',
    html: <input className="text-input" />
  },

  {
    rn: 'View',
    html: <div className="view"></div>
  },

  {
    rn: 'Alert',
    html: <div className="alert"></div>
  }
];

function getChildrenComponent(root) {
  // Get children
}

export function translate(parsed) {
  var Translated = null;

  //Search for root JSX component
  var rootComponent = null;
  walkAddParent(parsed, (node) => {
    if(!rootComponent && node.type === 'JSXElement') {
      rootComponent = node;
    }
  });

  if(rootComponent) {
    Translated = ComponentMap[rootComponent.name];
    return (
      <Translated>
        {getChildrenComponent(rootComponent)}
      </Translated>
    );
  }
}
