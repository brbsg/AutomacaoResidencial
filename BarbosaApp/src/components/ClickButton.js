import React,{useState} from 'react'
import {StyleSheet, View, Text, TouchableWithoutFeedback, Dimensions} from 'react-native'

export default props => {
    

    const onChangeClick = () => {
        setChangeClick(!changeClick)
    }

    return(
        <TouchableWithoutFeedback  >
            <View>
                {props.changeClick ? <View style={styles.onButton} ></View>
                : <View style={styles.offButton} ></View>}
            </View>
        </TouchableWithoutFeedback>
    )
}

const width = Dimensions.get('window').width/7
const height = Dimensions.get('window').width/7

const styles = StyleSheet.create({
    onButton: {
        width: width+1,
        height: height+1,
        borderRadius: 100,
        backgroundColor: '#AAA',
    },
    offButton:{
        width: width,
        height: height,
        borderRadius: 100,
        backgroundColor: '#CCC'
    }
  })

  
  