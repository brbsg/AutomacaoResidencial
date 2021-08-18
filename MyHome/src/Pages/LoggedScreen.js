import React, {useState} from 'react'
import {Image, Text, View, Dimensions, StyleSheet,SafeAreaView, ScrollView } from 'react-native'

import {FabButton} from '../components/FabButton'
import Screens from './Screens'

const screenWidth = Dimensions.get('window').width
const screenHeight = Dimensions.get('window').height

export default (props) => {

    const [screen, setScreen] = useState('home')
  
    const [openFab, setOpenFab] = useState('false')
  
    const [umid, setUmid] = useState("--")
    
    
  
    const onTractorPress = () => {
      setScreen('tractor')
      
      setOpenFab(!openFab)
    }
    const onHomePress = () => {
      setScreen('home')
      setOpenFab(!openFab)
    }
    const onGearPress = () => {
      setScreen('gear')
      setOpenFab(!openFab)
    }

    
    return(

        <SafeAreaView style={{flex:1}}>
          <ScrollView style={{ borderTopLeftRadius:60}}>
            <View style={styles.header}>

              <Image 
                style={{ width:screenWidth*0.14, height:screenWidth*0.14}}
                source={require('../assets/Images/BarbosaLogo.png')}
              />

              <View style={{paddingRight: screenWidth*0.1}}>
                <Text style={styles.mainFont}>Bem Vindo, Gustavo!</Text>
              </View>

            </View>

            <View style={styles.mainBody}>

              <Screens screen={screen} />

            </View>

            
          </ScrollView>
          <FabButton 
                onTractorPress={onTractorPress} 
                onHomePress={onHomePress} 
                onGearPress={onGearPress} 
                openFab={openFab} 
              />

        </SafeAreaView>
        
    )
}

const styles = StyleSheet.create({
  
  mainBody:{
    flex:1,
    backgroundColor: '#FFF',
    borderTopLeftRadius: 60,
    
  },  

  header: {
    height: screenHeight*0.20,
    flexDirection:'row',
    justifyContent:'space-around',
    backgroundColor: '#000420',
    alignItems: 'center',
  },

  mainFont:{
    fontSize: 20,
    color: '#FFF',
    paddingTop:'4%',
    
  },
})