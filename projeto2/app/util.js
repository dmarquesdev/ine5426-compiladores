import { walk } from 'esprima-walk';
import React from 'react';

function map(component) {
  const name = component.openingElement.name.name;
  var element = null;
  var content = "";
  var attributes = [];

  switch(name) {
    case "View":
      element = document.createElement('div');
      element.setAttribute('class', 'rn-view');
      break;

    case "Text":
      element = document.createElement('span');
      element.setAttribute('class', 'rn-text');
      for(var i = 0; i < component.children.length; i++) {
        content += component.children[i].value;
      }
      element.innerHTML = content;
      break;

    case "Button":
      attributes = component.openingElement.attributes;
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
      element = document.createElement('input');
      element.setAttribute('class', 'rn-input');
      element.setAttribute('type', 'text');
      attributes = component.openingElement.attributes;
      for(var i = 0; i < attributes.length; i++) {
        const attribute = attributes[i];
        if(attribute.name.name == 'secureTextEntry') {
          element.setAttribute('type', 'password');
        }
      }
      break;

    case "Image":
      element = document.createElement('img');
      element.setAttribute('class', 'rn-image');
      element.setAttribute('src', 'assets/images/placeholder.png');
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

export function clearPreview(element) {
  var firstChild = element.firstChild;

  while(firstChild) {
    element.removeChild(firstChild);
    firstChild = element.firstChild;
  }
}

export function checkSemantic(parsed) {
  var isReactComponent = false;
  var hasRenderMethod = false;
  var isJSX = false;
  var declaredImports = [];
  var missingImports = [];
  var errors = [];

  walk(parsed, (node) => {
    if(node.type == 'ImportDefaultSpecifier' ||
    node.type == 'ImportSpecifier') {
      const name = node.local.name;
      const index = missingImports.indexOf(name);

      declaredImports.push(name);
      if(index >= 0) {
        missingImports.splice(index, 1);
        if(name == 'React') {
          isReactComponent = true;
        }
      }
    } else if(node.type == 'ClassDeclaration' &&
    node.superClass.object.name == 'React' &&
    node.superClass.property.name == 'Component'){
      if(declaredImports.indexOf(node.superClass.object.name) >= 0) {
        isReactComponent = true;
      } else {
        missingImports.push('React');
      }

    } else if(node.type == 'MethodDefinition' && node.key.name == 'render') {
      hasRenderMethod = true;

    } else if(node.type == 'ReturnStatement' &&
    node.argument.type == 'JSXElement') {
      isJSX = true;
    } else if(node.type == 'JSXElement' &&
    declaredImports.indexOf(node.openingElement.name.name) < 0) {
      missingImports.push(node.openingElement.name.name);
    }
  });

  if(!isReactComponent) {
    errors.push("This is not a React Component! \
    Your class must extend React.Component");
  }

  if(!hasRenderMethod) {
    errors.push("You must declare a render method within your class!");
  }

  if(missingImports.length) {
    errors.push("Your program is missing some imports! "
    + missingImports.join(', '));
  }

  if(!isJSX) {
    errors.push("Your render method doesn't return a valid JSX component!");
  }

  if(!isReactComponent || !hasRenderMethod || missingImports.length || !isJSX) {
    throw errors;
  }

  return true;
}

export function colorize(tokens){
  var codeList = [];
  for(var i = 0; i < tokens.length; i++){
    const token = tokens[i];
    var className = "no-token";
    switch (token.type){
      case "Boolean":
        className="boolean";
        break;
      case "Identifier":
        break;
      case "Keyword":
        break;
      case "Null":
        break;
      case "Numeric":
        break;
      case "Punctuator":
        break;
      case "String":
        break;
      case "RegularExpression":
        break;
      default:
        className="no-token";
        break;
    }

    codeList.push(<span className={className}>{token.value}</span>);
  }
  return codeList;
}
