import React,{useState, useEffect} from 'react';
import { StyleSheet, SafeAreaView, StatusBar,View, Text} from 'react-native';

import Routes from './Routes'

export default function App() {
 
  
  return(
    <SafeAreaView style={styles.container}>
      <StatusBar backgroundColor='#0000' translucent={true} hidden={false}/>
      <Routes />
    </SafeAreaView>
  )
}

const styles = StyleSheet.create({
  container: {
    flex:1,
    backgroundColor: '#000420'
  },
})
 