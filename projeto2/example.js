import React from 'react';

import { View, Text, Button, TextInput } from 'react-native';

class Test extends React.Component {
  render() {
    return (
      <View>
        <Text>Hello World</Text>
        <TextInput />
        <Button title="Press Me!" />
      </View>
    );
  }
}


import React from 'react';

import { View, Text, Button, TextInput, Image } from 'react-native';

class Login extends React.Component {
  render() {
    return (
      <View>
        <Text>Login</Text>
        <View>
          <Image style={{width: 50, height: 50, borderWidth: 5}} />
        </View>
        <TextInput placeholder="Username" />
        <TextInput placeholder="Password" secureTextEntry={true} />
        <Button title="Login" />
      </View>
    );
  }
}
