import React, {useState, useEffect} from 'react'
import {  Animated, Keyboard, Image, Dimensions, View, Text, KeyboardAvoidingView, TextInput, TouchableOpacity, StyleSheet} from 'react-native'
import FingerprintScanner from 'react-native-fingerprint-scanner'
import AsyncStorage from '@react-native-community/async-storage'
//import { Header } from 'react-native/Libraries/NewAppScreen'

const screenWidth = Dimensions.get('window').width
const screenHeight = Dimensions.get('window').height

export default ({navigation}) => {
  const [biometryType,setBiometryType] = useState(null)

  const [name, setName] = useState("")
  const [email, setEmail] = useState("")
  const [password, setPassword] = useState("")
  const [confirmPassword, setConfirmPassword] = useState("")
  const [firstLogin, setFirstLogin] = useState("")
 
  const [offset] = useState(new Animated.ValueXY({x:0, y:200}))
  const [logo] = useState(new Animated.ValueXY({x:screenWidth/2, y:screenWidth/2}))
  const [rotAnimation] = useState(new Animated.Value(0))

  async function verifyLogin(){
    let response = await AsyncStorage.getItem('useData')
    let json = await JSON.parse(response)
    console.log(json)
  }

  
  const showAuthenticationDialog = () => {
    
    if(biometryType!==null && biometryType!==undefined ){
      FingerprintScanner.authenticate({
        description:'Verifique a Sua Identidade Para Abrir o Portão',
        cancelButton: 'Cancelar',

      })
      .then(() => {
        navigation.navigate("LoggedScreen")
      })
      .catch((error) => {
        console.log('Authentication error is => ', error);
      });
    }else{
      if(login == 'gustavo' && password == '1234'){
        navigation.navigate("LoggedScreen")
      }
      console.log('biometric authentication is not available');
    }
  }

  const spinAnimation = () => {
    Animated.timing(rotAnimation,{
      useNativeDriver: true,
      toValue: 360,
      duration: 1000
    }).start()
  }

  const rotateLogo = () =>{
    Animated.parallel([
      Animated.spring(offset.y,{
        useNativeDriver:true,
        toValue:0,
        speed:4,
      }).start(),
      spinAnimation()
    ])
  }

  useEffect(()=>{(

    FingerprintScanner.isSensorAvailable()
    .then((biometryType) => {
      setBiometryType(biometryType)
    })
    .catch((error) => console.log('isSensorAvailable error => ', error)),

    verifyLogin(),

    rotateLogo()
  
  )},[])

  const rotInterpolate = rotAnimation.interpolate({
    inputRange: [0, 360],
    outputRange: ["0deg", "360deg"]
  })
  
  
  return(
    <KeyboardAvoidingView 
    
    style={styles.container}
    >

      <View style={{flex:1,alignItems:"center", justifyContent:'space-around'}}>

        {/* <TouchableOpacity onPress={()=>spinAnimation()}>
          <View style={styles.touchable}></View>
        </TouchableOpacity> */}
        <Animated.View 
        style={[styles.imageStyle,{width:logo.x, height:logo.y}]} >
          <Image 
          style={{ position:'absolute', top:'33%',left:'35%', width:screenWidth/5.5, height: screenWidth/5.5}}
          source={require('../assets/Images/BarbosaLogoInterno.png')}
          />
          <Animated.Image 
          style={[{transform:[{rotate:rotInterpolate}]} ,{position:'absolute',width:screenWidth/2, height: screenWidth/2}]}
          source={require('../assets/Images/BarbosaLogoExterno.png')}
          />
        </Animated.View>  

        <Animated.View 
        style={[{ alignItems:'center'},
        {
          transform: [
            {translateY: offset.y}
          ]
        }
        ]} 
        >

          <TextInput 
          style={styles.inputStyle}
          placeholder="Email"
          autoCorrect={false}
          onChangeText={(v)=>setEmail(v)}
          autoCapitalize='none'
          />

          <TextInput 
          style={styles.inputStyle}
          placeholder="Senha"
          autoCorrect={false}
          onChangeText={(v)=>setPassword(v)}
          keyboardType= 'numeric'
          secureTextEntry ={true}
          />
          <View >
            <TouchableOpacity onPress={showAuthenticationDialog} style={styles.buttonStyle} >
              <Text style={{color:'white'}} >ENTRAR</Text>
            </TouchableOpacity>

            <TouchableOpacity style={styles.buttonStyle}  >
              <Text style={{color:'white'}} >PRIMEIRO CADASTRO</Text>
            </TouchableOpacity>
          </View> 

        </Animated.View>

    </View>

    <View style={styles.version} ><Text style={{color:'white'}} >Version: 1.0.3</Text></View> 

    </KeyboardAvoidingView>
  )
}

const styles = StyleSheet.create({
  container: {
    flex:1,
    alignItems:'center', 
    justifyContent:'center',
    backgroundColor:"#000420"
  },
  inputStyle: {
    backgroundColor: 'white',
    width:screenWidth*0.8,
    marginBottom: screenWidth*0.05,
    borderRadius:10,
    padding: 10,
  },
  buttonStyle: {
    backgroundColor:"#555", 
    width:screenWidth*0.4,
    height:screenWidth*0.08,
    alignItems:'center',
    justifyContent:'center',
    borderRadius: 5,
    marginBottom:10
  },
  imageStyle: {
    alignItems:'center',
    justifyContent:'center',
    backgroundColor:'transparent'
  },
  touchable :{
   
    width:screenWidth/2,
    height:screenWidth/2,
    backgroundColor:'#555'
  },
  version:{
    alignItems:'center',
    justifyContent:'center',
    backgroundColor: 'transparent',
  }
})