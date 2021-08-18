import React,{ useRef, useState} from 'react'
import {View, Text, StyleSheet,TouchableWithoutFeedback, Animated, Dimensions} from 'react-native'
import AntDesign from 'react-native-vector-icons/AntDesign'
import Icon from 'react-native-vector-icons/FontAwesome5'
import Octicons from 'react-native-vector-icons/Octicons'

const screenWidth = Dimensions.get('window').width;

const FabButton = props => {
    const [open, setOpen] = useState(false)
    const [mainButton, setMainButton] = useState('up')
    const [screenMod, setScreenMod] = useState(0.16)

    const Animation = useRef(new Animated.Value(0)).current

    const toggleMenu = () => {
        
        const toValue = open ? 0 : 1

        Animated.spring(Animation, {
            toValue,
            friction: 6,
            useNativeDriver: true
        }).start()
        
        setOpen(!open)
 
        open ? (setScreenMod(0.16),setMainButton('up') ) 
        : (setScreenMod(0.59), setMainButton('down'))
    }

    const handleTractor = () => {
        props.onTractorPress()
        toggleMenu()
    }

    const handleHome = () => {
        props.onHomePress()
        toggleMenu()
    }

    const handleGear = () =>{
        props.onGearPress()
        toggleMenu()
    }

    const tractorStyle = {
        transform: [
            {scale: Animation},
            {
                translateY: Animation.interpolate({
                    inputRange: [0, 1],
                    outputRange: [240,-screenWidth*0.03]
                })
            }
        ]
    }

    const homeStyle = {
        transform: [
            {scale: Animation},
            {
                translateY: Animation.interpolate({
                    inputRange: [0, 1],
                    outputRange: [240,-screenWidth*0.02]
                })
            }
        ]
    }

    const gearStyle = {
        transform: [
            {scale: Animation},
            {
                translateY: Animation.interpolate({
                    inputRange: [0, 1],
                    outputRange: [240,-screenWidth*0.01]
                })
            }
        ]
    }

    

    return(
        
        <View style={[styles.container, props.style,{height:screenWidth*screenMod}]} >
            
                <TouchableWithoutFeedback onPress={handleTractor} >
                    <Animated.View style={[styles.button, styles.submenu, tractorStyle]}>
                        <Icon name="tractor" size={20} color="#FFF" />
                    </Animated.View>
                </TouchableWithoutFeedback>

                <TouchableWithoutFeedback onPress={handleHome} >
                    <Animated.View style={[styles.button, styles.submenu, homeStyle]}>
                        <AntDesign name="home" size={20} color="#FFF" />
                    </Animated.View>
                </TouchableWithoutFeedback>

                <TouchableWithoutFeedback onPress={handleGear} >
                    <Animated.View style={[styles.button, styles.submenu, gearStyle]}>
                        <Octicons name="gear" size={20} color="#FFF" />
                    </Animated.View>
                </TouchableWithoutFeedback> 


                <TouchableWithoutFeedback onPress={toggleMenu} >
                    <Animated.View style={[styles.button, styles.menu]}>
                        <AntDesign name={mainButton} size={24} color="#FFF" />
                    </Animated.View>
                </TouchableWithoutFeedback>
            
        </View>
    )
}

const styles = StyleSheet.create({
    container: {
        position:'absolute',
        justifyContent:'flex-end',
        alignItems:'center',
        width:screenWidth*0.16,
        backgroundColor:'transparent',
        borderRadius:500,
        left:screenWidth*0.80,
        bottom:screenWidth*0.04,
     
    },
    button: {
        
        width: screenWidth*0.16,
        height: screenWidth*0.16,
        borderRadius: 500,
        justifyContent: 'center',
        alignItems: 'center',
        shadowRadius: 10,
        shadowColor: '#00213B',
        shadowColor: 0.3,
        shadowOffset: {
            height: 10,
        }
    },
    menu: {
        backgroundColor: '#000420'
    },
    submenu: {
        width: screenWidth*0.13,
        height: screenWidth*0.13,
        borderRadius: 1000,
        backgroundColor: '#000420'
    }

})

export {FabButton}