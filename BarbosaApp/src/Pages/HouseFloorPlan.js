import React, {useState} from 'react'
import { View, Image, StyleSheet, Dimensions, Button, TouchableOpacity, ImageBackground} from 'react-native'

import Icon from 'react-native-vector-icons/MaterialCommunityIcons';

const screenWidth = Dimensions.get('window').width;
const screenHeight = Dimensions.get('window').height;

const {height, width} = Image.resolveAssetSource(require('../assets/Images/PlantaCasaBSBCarros.jpg'))
const imageRatio = height/width


export default () =>{
    const [lightState, setLightState] = useState('lightbulb-outline')
    const [buttonSize, setLightSize] = useState(screenWidth*0.07)
    const [paddingState, setPaddingState] = useState(0)

    const [gustavoRoom, setgustavoRoom] = useState({state: 'lightbulb-outline', paddingState: 0, buttonSize: screenWidth*0.07})
    const [gate, setgate] = useState({state: 'gate', paddingState: 0, buttonSize: screenWidth*0.07})
    const [garage, setgarage] = useState({state: 'lightbulb-outline', paddingState: 0, buttonSize: screenWidth*0.07})
    const [livingRoom, setlivingRoom] = useState({state: 'lightbulb-outline', paddingState: 0, buttonSize: screenWidth*0.07})
    const [diningRoom, setdiningRoom] = useState({state: 'lightbulb-outline', paddingState: 0, buttonSize: screenWidth*0.07})
    const [parentsRoom, setparentsRoom] = useState({state: 'lightbulb-outline', paddingState: 0, buttonSize: screenWidth*0.07})
    const [parentsBathroom, setparentsBathroom] = useState({state: 'lightbulb-outline', paddingState: 0, buttonSize: screenWidth*0.07})
    const [commonBathroom, setcommonBathroom] = useState({state: 'lightbulb-outline', paddingState: 0, buttonSize: screenWidth*0.07})
    const [roomHall, setroomHall] = useState({state: 'lightbulb-outline', paddingState: 0, buttonSize: screenWidth*0.07})
    const [guestsRoom, setguestsRoom] = useState({state: 'lightbulb-outline', paddingState: 0, buttonSize: screenWidth*0.07})
    const [kitchenHall, setkitchenHall] = useState({state: 'lightbulb-outline', paddingState: 0, buttonSize: screenWidth*0.07})
    const [guilhermeRoom, setguilhermeRoom] = useState({state: 'lightbulb-outline', paddingState: 0, buttonSize: screenWidth*0.07})
    const [kitchen, setkitchen] = useState({state: 'lightbulb-outline', paddingState: 0, buttonSize: screenWidth*0.07})
  
    const changeLampState = (room) => {
        if(eval(room +'.state') == 'lightbulb-on-outline'){
            eval('set'+ room + `({ 
                state:'lightbulb-outline', 
                buttonSize:screenWidth*0.07, 
                paddingState:0})`)
        }else if(eval(room +'.state') == 'lightbulb-outline'){
            eval('set'+ room + `({
                state:'lightbulb-on-outline',
                buttonSize:screenWidth*0.08,
                paddingState:screenWidth*0.012
            })`)
        } else if(eval(room +'.state') == 'gate'){
            eval('set'+ room + `({
                ...${room},
                state:'gate-open',
            })`)
        }else if(eval(room +'.state') == 'gate-open'){
            eval('set'+ room + `({
                ...${room},
                state:'gate',
            })`)
        }
    }

    const handleLamp = (room) => {
        changeLampState(room)     
    } 
   
    return(
           
        <View style={styles.imageContainer}  >
            <ImageBackground
            style = {styles.imageStyle}
            source = {require('../assets/Images/PlantaCasaBSBCarros.jpg')} 
            >
                <TouchableOpacity style = {[styles.lightMainStyle,{paddingBottom:gustavoRoom.paddingState}, styles.gustavoRoom]} onPress = {_ => handleLamp('gustavoRoom')} >
                    <Icon name={gustavoRoom.state} size={gustavoRoom.buttonSize} color='#0009' />
                </TouchableOpacity>

                <TouchableOpacity style = {[styles.lightMainStyle,{paddingBottom:gate.paddingState}, styles.gate]} onPress = {_ => handleLamp('gate')} >
                    <Icon name={gate.state} size={gate.buttonSize} color='#0009' />
                </TouchableOpacity>

                <TouchableOpacity style = {[styles.lightMainStyle,{paddingBottom:garage.paddingState}, styles.garage]} onPress = {_=>handleLamp('garage')} >
                    <Icon name={garage.state} size={garage.buttonSize} color='#0009' />
                </TouchableOpacity>
                
                <TouchableOpacity style = {[styles.lightMainStyle,{paddingBottom:livingRoom.paddingState}, styles.livingRoom]} onPress = {_=>handleLamp('livingRoom')} >
                    <Icon name={livingRoom.state} size={livingRoom.buttonSize} color='#0009' />
                </TouchableOpacity>

                <TouchableOpacity style = {[styles.lightMainStyle,{paddingBottom:diningRoom.paddingState}, styles.diningRoom]} onPress = {_=>handleLamp('diningRoom')} >
                    <Icon name={diningRoom.state} size={diningRoom.buttonSize} color='#0009' />
                </TouchableOpacity>
                                    
                <TouchableOpacity style = {[styles.lightMainStyle,{paddingBottom:parentsRoom.paddingState}, styles.parentsRoom]} onPress = {_=>handleLamp('parentsRoom')} >
                    <Icon name={parentsRoom.state} size={parentsRoom.buttonSize} color='#0009' />
                </TouchableOpacity>
                                    

                <TouchableOpacity style = {[styles.lightMainStyle,{paddingBottom:parentsBathroom.paddingState}, styles.parentsBathroom]} onPress = {_=>handleLamp('parentsBathroom')} >
                    <Icon name={parentsBathroom.state} size={parentsBathroom.buttonSize} color='#0009' />
                </TouchableOpacity>
                                    

                <TouchableOpacity style = {[styles.lightMainStyle,{paddingBottom:commonBathroom.paddingState}, styles.commonBathroom]} onPress = {_=>handleLamp('commonBathroom')} >
                    <Icon name={commonBathroom.state} size={commonBathroom.buttonSize} color='#0009' />
                </TouchableOpacity>
                                    

                <TouchableOpacity style = {[styles.lightMainStyle,{paddingBottom:roomHall.paddingState}, styles.roomHall]} onPress = {_=>handleLamp('roomHall')} >
                    <Icon name={roomHall.state} size={roomHall.buttonSize} color='#0009' />
                </TouchableOpacity>
                                    

                <TouchableOpacity style = {[styles.lightMainStyle,{paddingBottom:guestsRoom.paddingState}, styles.guestsRoom]} onPress = {_=>handleLamp('guestsRoom')} >
                    <Icon name={guestsRoom.state} size={guestsRoom.buttonSize} color='#0009' />
                </TouchableOpacity>
                                    

                <TouchableOpacity style = {[styles.lightMainStyle,{paddingBottom:kitchenHall.paddingState}, styles.kitchenHall]} onPress = {_=>handleLamp('kitchenHall')} >
                    <Icon name={kitchenHall.state} size={kitchenHall.buttonSize} color='#0009' />
                </TouchableOpacity>

                <TouchableOpacity style = {[styles.lightMainStyle,{paddingBottom:guilhermeRoom.paddingState}, styles.guilhermeRoom]} onPress = {_=>handleLamp('guilhermeRoom')} >
                    <Icon name={guilhermeRoom.state} size={guilhermeRoom.buttonSize} color='#0009' />
                </TouchableOpacity>

                <TouchableOpacity style = {[styles.lightMainStyle,{paddingBottom:kitchen.paddingState}, styles.kitchen]} onPress = {_=>handleLamp('kitchen')} >
                    <Icon name={kitchen.state} size={kitchen.buttonSize} color='#0009' />
                </TouchableOpacity>
                                    
            </ImageBackground>
        
        </View>            
    )
}

const styles = StyleSheet.create({
    imageContainer:{
        flex:1,
        paddingTop:20,
        alignItems:'center', 
        justifyContent:'flex-start',
    },
    imageStyle:{
        width: screenWidth*0.9,
        height:screenWidth/imageRatio*0.9,
    },
    lightMainStyle:{
        alignItems:'center',
        justifyContent:'center',
        height:screenWidth*0.15,
        width: screenWidth*0.15,
        borderRadius:1000,
        backgroundColor:'#AAAA',
    },
    gustavoRoom:{
        position:'absolute',
        bottom:screenWidth/imageRatio*0.9*0.625,
        right:screenWidth*0.9*0.1,
    },
    gate:{
        position:'absolute',
        bottom:screenWidth/imageRatio*0.9*(-0.01),
        right:screenWidth*0.9*0.62,
    },
    garage:{
        position:'absolute',
        bottom:screenWidth/imageRatio*0.9*0.09,
        right:screenWidth*0.9*0.50-25,
    },
    livingRoom: {
        position:'absolute',
        bottom:screenWidth/imageRatio*0.9*0.315,
        right:screenWidth*0.9*0.21,
    },
    diningRoom:{
        position:'absolute',
        bottom:screenWidth/imageRatio*0.9*0.5,
        right:screenWidth*0.9*0.14,
    },
    parentsRoom:{
        position:'absolute',
        bottom:screenWidth/imageRatio*0.9*0.26,
        right:screenWidth*0.9*0.62,
    },
    parentsBathroom:{
        position:'absolute',
        bottom:screenWidth/imageRatio*0.9*0.385,
        right:screenWidth*0.9*0.7,
    },
    commonBathroom:{
        position:'absolute',
        bottom:screenWidth/imageRatio*0.9*0.465,
        right:screenWidth*0.9*0.7,
    },
    roomHall:{
        position:'absolute',
        bottom:screenWidth/imageRatio*0.9*0.38,
        right:screenWidth*0.9*0.51,
    },
    guestsRoom:{
        position:'absolute',
        bottom:screenWidth/imageRatio*0.9*0.56,
        right:screenWidth*0.9*0.65,
    },
    kitchenHall:{
        position:'absolute',
        bottom:screenWidth/imageRatio*0.9*0.64,
        right:screenWidth*0.9*0.39,
    },
    guilhermeRoom:{
        position:'absolute',
        bottom:screenWidth/imageRatio*0.9*0.66,
        right:screenWidth*0.9*0.65,
    },
    kitchen:{
        position:'absolute',
        bottom:screenWidth/imageRatio*0.9*0.82,
        right:screenWidth*0.9*0.61,
    }
})