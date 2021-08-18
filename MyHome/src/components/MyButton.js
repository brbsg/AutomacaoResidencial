import React,{ useState} from 'react'
import {View, Text, StyleSheet,TouchableOpacity, Animated, Dimensions} from 'react-native'

const screenWidth = Dimensions.get('window').width

export default props => {
    if(props.buttonStyle == 'big'){
         var size = styles.bigButton
    }else if(props.buttonStyle == 'small'){
        var size = styles.smallButton
    }

    if(props.buttonStyle == 'big' || props.buttonStyle == 'small'){
        return(
            <View style={{paddingBottom:screenWidth*0.02}}>
                <TouchableOpacity onPress={() => props.onClick()} style={[size, styles.commonSpecs, props.style]}>
                    <Text style={styles.textColor} >{props.text1}</Text>
                    <Text style={styles.smallText} >{props.text2}</Text>
                </TouchableOpacity>
            </View>
            
        )
    }
    if(props.buttonStyle == 'double'){
        return(
            <View style = {styles.double}>
                <TouchableOpacity onPress={() => props.onClick()} style={[styles.smallButton, styles.commonSpecs]}>
                    <Text style={styles.textColor} >{props.text[0]}</Text>
                </TouchableOpacity> 
                
                <TouchableOpacity onPress={() => props.onClick()} style={[styles.smallButton, styles.commonSpecs]}>
                    <Text style={styles.smallText} >{props.text[1]}</Text>
                </TouchableOpacity>
            </View>
)
    }
    
}

const styles = StyleSheet.create({
    commonSpecs:{
        alignItems:'center',
        justifyContent:'center',
        borderRadius:15,
        backgroundColor: '#000420'
    },
    smallButton:{
        height: screenWidth*0.25,
        width: screenWidth*0.38,
    },
    bigButton:{
        height: screenWidth*0.30,
        width: screenWidth*0.80,
    },

    textColor:{
        color:'#FFF',
        fontWeight:'bold',
        fontSize:30
    },
    smallText:{
        color:'#FFF',
        fontSize:12
    },
    double:{
        flexDirection: 'row',
        width: screenWidth*0.80,
        justifyContent: 'space-between',
        paddingTop:screenWidth*0.02,
        paddingBottom:screenWidth*0.02,
    }
})
