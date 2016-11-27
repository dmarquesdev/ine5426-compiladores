import { walk } from 'esprima-walk';
import React from 'react';

function map(component) {
  const name = component.openingElement.name.name;
  var element = null;
  var content = "";

  switch(name) {
    case "View":
      element = document.createElement('div');
      element.setAttribute('class', 'view');
      break;

    case "Text":
      element = document.createElement('span');
      for(var i = 0; i < component.children.length; i++) {
        content += component.children[i].value;
      }
      element.innerHTML = content;
      break;

    case "Button":
      const attributes = component.openingElement.attributes;
      for(var i = 0; i < attributes.length; i++) {
        const attribute = attributes[i];
        if(attribute.name.name == 'title') {
          content = attribute.value.value;
          break;
        }
      }
      element = document.createElement('button');
      element.setAttribute('class', 'rn-button');
      element.innerHTML = content;
      break;

    case "TextInput":
      break;
  }

  return element;
}

function buildHTMLTree(root) {
  var html = null;

  if(root) {
    const rootElement = map(root);

    for(var i = 0; i < root.children.length; i++) {
      const child = root.children[i];
      if(child.type == 'JSXElement') {
        rootElement.appendChild(buildHTMLTree(child));
      }
    }

    html = rootElement;
  }

  return html;
}

export function translate(parsed) {
  var root = null;

  // Gather all components from JSX
  walk(parsed, (node) => {
    if(node.type == 'JSXElement') {
      if(!root) {
        root = node;
      }
    }
  });

  // Rebuild tree
  return buildHTMLTree(root);
}
