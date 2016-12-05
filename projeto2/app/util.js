import { walk } from 'esprima-walk';
import React from 'react';

// Check if given parameter is a number, using regexp
// http://stackoverflow.com/questions/1303646/check-whether-variable-is-number-or-string-in-javascript
function isNumber(n) { return /^-?[\d.]+(?:e-?\d+)?$/.test(n); }

// Function responsible for translating special React Native styles to
// a css equivalent.
function translateStyle(attr) {
  switch(attr) {
    case "border-width":
      return "border-top-style: solid; " +
      "border-bottom-style: solid; " +
      "border-left-style: solid; " +
      "border-right-style: solid; " +
      "border-width"
    default:
      return attr;
  }
}

// Parse a list of properties that came from a component style attribute
// to a CSS equivalent
function parseStyle(properties) {
  let styleString = "";
  // Iterate over properties
  for(var i = 0; i < properties.length; i++) {
    const property = properties[i];
    // Start a buffer for property name
    var name = "";
    // Get property value
    var value = property.value.value;

    // Iterate over property name and changes upper case characters
    // to lower case with a '-' before it.
    //
    // This is needed because React Native uses camelCase name convetion
    // to define style properties, while CSS uses score-separated names
    for(var j = 0; j < property.key.name.length; j++) {
      const char = property.key.name[j];
      if(char == char.toUpperCase()) {
        name += "-" + char.toLowerCase();
      } else {
        name += char;
      }
    }

    // If value is a number, it needs a scale associated.
    // By default, React Native uses only numbers as numeric values,
    // while CSS uses 'px' for pixels
    if(isNumber(value)) {
      value = value + "px";
    }

    // Concatenate the "name: value; " to the final string buffer
    styleString += translateStyle(name) + ": " + value + "; ";
  }

  return styleString;
}

// Map a React Native component to a HTML equivalent;
function map(component, isRoot) {
  const name = component.openingElement.name.name;
  var element = null;
  var content = "";
  var attributes = [];

  switch(name) {
    // The closest element in HTML to View is div.
    // In React Native, is a good practice that every root component
    // should be a View, so if this component is a Root, it will receive
    // a 'container' class to format properly on screen.
    case "View":
      element = document.createElement('div');
      if(isRoot) {
        element.setAttribute('class', 'container rn-view');
      } else {
        element.setAttribute('class', 'rn-view');
      }
      break;

    // There is a lot of elements that could map to Text,
    // but for simplicity we've adopted the span
    case "Text":
      element = document.createElement('span');
      element.setAttribute('class', 'rn-text');
      for(var i = 0; i < component.children.length; i++) {
        content += component.children[i].value;
      }
      element.innerHTML = content;
      break;

    // React Native Button to HTML Button
    // Pretty straightforward
    case "Button":
      attributes = component.openingElement.attributes;
      element = document.createElement('button');
      for(var i = 0; i < attributes.length; i++) {
        const attribute = attributes[i];
        if(attribute.name.name == 'title') {
          content = attribute.value.value;
          break;
        } else if (attribute.name.name == 'disabled') {
          element.setAttribute('disabled', 'true');
        }
      }
      // The default android button color is grey
      element.setAttribute('class', 'rn-button btn grey lighten-1');
      element.innerHTML = content;
      break;

    // TextInput is equivalent to a HTML input
    case "TextInput":
      element = document.createElement('input');
      element.setAttribute('class', 'rn-input validate');
      element.setAttribute('type', 'text');
      attributes = component.openingElement.attributes;
      for(var i = 0; i < attributes.length; i++) {
        const attribute = attributes[i];
        if(attribute.name.name == 'secureTextEntry') {
          element.setAttribute('type', 'password');
        } else if(attribute.name.name == 'placeholder') {
          element.setAttribute('placeholder', attribute.value.value);
        } else if(attribute.name.name == 'editable') {
          element.setAttribute('disabled', !attribute.value.value);
        }
      }
      break;

    // React Native Image to HTML img
    case "Image":
      element = document.createElement('img');
      element.setAttribute('class', 'rn-image');
      // Since we don't handle the image itself,
      // we've chosen to put a high quality placeholder
      // to support stretching.
      element.setAttribute('src', 'images/picture.svg');
      break;
  }

  // Style parsing
  if(element != null) {
    attributes = component.openingElement.attributes;
    for(var i = 0; i < attributes.length; i++) {
      const attribute = attributes[i];
      if(attribute.name.name == 'style') {
        const properties = attribute.value.expression.properties;
        element.setAttribute('style', parseStyle(properties));
        break;
      }
    }
  }

  return element;
}

// Gets a JSXElement and start mapping him and his children to HTML
function buildHTMLTree(root, isRoot) {
  var html = null;

  if(root) {
    const rootElement = map(root, isRoot);

    for(var i = 0; i < root.children.length; i++) {
      const child = root.children[i];
      if(child.type == 'JSXElement') {
        rootElement.appendChild(buildHTMLTree(child, false));
      }
    }

    html = rootElement;
  }

  return html;
}

// Receives the parsed AST from user interface and search for the root node
// to start the HTML building process
export function translate(parsed) {
  var root = null;

  // Search for the root component in AST
  walk(parsed, (node) => {
    if(node.type == 'JSXElement') {
      if(!root) {
        root = node;
      }
    }
  });

  // Rebuild tree
  return buildHTMLTree(root, true);
}

// Remove every element from a root element
export function clearPreview(element) {
  var firstChild = element.firstChild;

  while(firstChild) {
    element.removeChild(firstChild);
    firstChild = element.firstChild;
  }
}

// Basic semantical analysis
// It checks for key concepts that every React Native component must have.
//
// It checks if the used components was previously declared,
// if what is inside editor is a class (Every component must be a class)
// and if it extends 'React.Component', because that is a must do to any
// React component (React Native is based on React).
// It also checks if a 'render' method is declared, because without a
// 'render' method, the component is useless and, for the scope of this
// project, if the returned value of the render method is a JSX component.
export function checkSemantic(parsed) {
  var isReactComponent = false;
  var hasRenderMethod = false;
  var isJSX = false;
  var declaredImports = [];
  var missingImports = [];
  var errors = [];

  // Interface method to walk through the AST
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


// Receive a list of tokens and break them into 'span'
// HTML elements with classes that will colorize the text.
// It's our representation of Lexical Analysis
export function colorize(tokens, jsxcode){

  var codeList = [];
  var column = 0;
  var line = 1;
  var pd = 0;

  for(var i = 0; i < tokens.length; i++){
    const token = tokens[i];
    var className = "no-token";

    if(token.loc.start.line > line){
      for(var y = 0; y < token.loc.start.line-line; y++){
          codeList.push(<br />);
        }
      line = token.loc.end.line;
      column = 0;
      pd = 0;
    }

    if(column === 0){
      pd = token.loc.start.column*5;
    }

    if(token.loc.start.column > column){
        for(var x = 0; x < token.loc.start.column-column; x++){
          codeList.push(" ");
        }
      }

    column = token.loc.end.column;

    switch (token.type){
      case "Boolean":
        className="boolean";
        break;
      case "Identifier":
        className="identifier";
        break;
      case "Keyword":
        className="keyword";
        break;
      case "Null":
        className="null";
        break;
      case "Numeric":
        className="numeric";
        break;
      case "Punctuator":
        className="punctuator";
        break;
      case "String":
        if(token.value == "'__JSX_CODE__'"){
          token.value = jsxcode;
          className="jsx";
        }else{
        className="string";
        }
        break;
      case "RegularExpression":
        className="regularExpression";
        break;
      default:
        className="no-token";
        break;
    }

    codeList.push(<span id={"token" + i} style={{padding: pd + "px"}}
    className={className}>{token.value}</span>);
    pd = 0;

  }
  return codeList;
}
