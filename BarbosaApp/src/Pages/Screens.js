import React, {useState, useEffect} from 'react'
import { StyleSheet, View, Text,TextInput,Button, FlatList, Dimensions, ScrollView } from 'react-native'
import TcpSocket from 'react-native-tcp-socket'

import MyButton from '../components/MyButton.js'
import SendTimeButton from '../components/SendTimeButton.js'
import HouseFloorPlan from './HouseFloorPlan.js'

const screenWidth = Dimensions.get('window').width
const screenHeight = Dimensions.get('window').height

export default props => {
  const [hasConnection,setHasConnection] = useState(false)
  const [host, setHost] = useState("192.168.10.102")
  const [port, setPort] = useState('80')
  const [con, setCon] = useState(null)

  const [onOffButton, setOnOffButton] = useState(false)
  const [button, setButton] = useState(false);

  
  const connectButton = () => {
      if(host != ''){
        let client = TcpSocket.createConnection({host, port})
        
        client.on('close', () => {
          setHasConnection(false)
        })
        
        client.on('error', (e) =>{  
          alert(e)
        })
        client.on('data', (d) => {

          alert(d);
        //   let response = Buffer.from(d).toString()
          
        //   response = response.split('!')
          
        //   for( i in response){
        //     if(umidFind = response[i].indexOf){
        //       let str = response[i].split('t=')[1]
        //       setUmid(str)
        //     }
        //   }
  
        })

        setHasConnection(true)
        setCon(client)
      }
    }

  const disconnectButton = () => {
      if(hasConnection){
        con.write("stop*")
      }
  }

  const handleLamp = ( e, id) => {
  
    // onOffButton ? (
    //   con.write(id + "on*"),
    //   setOnOffButton(!onOffButton)
    // ) : (
    //   con.write(id +"off*"),
    //   setOnOffButton(!onOffButton)
    // )
  }

  const handleClick = (room) => {
    con.write(room + '*')

    console.log(room)
    // if(room == 'GATE'){
    //   //con.write("GATEon*")
    // }else{
    //   onOffButton ? (
    //   //con.write(room + "on*"),
    //   setOnOffButton(!onOffButton)
    // ) : (
    //   //con.write(room +"off*"),
    //   setOnOffButton(!onOffButton)
    // )
    // }

  }

  const sendTimerData = (hourTimer1,minuteTimer1,hourTimer2, minuteTimer2, daysIrrigating) => {
    let timerConcateneted = "hr"
    console.log(hourTimer1,minuteTimer1,hourTimer2, minuteTimer2, daysIrrigating)
    timerConcateneted = timerConcateneted + "-"
    timerConcateneted = timerConcateneted + hourTimer1
    timerConcateneted = timerConcateneted + "-"
    timerConcateneted = timerConcateneted + minuteTimer1
    timerConcateneted = timerConcateneted + "-"
    timerConcateneted = timerConcateneted + hourTimer2
    timerConcateneted = timerConcateneted + "-"
    timerConcateneted = timerConcateneted + minuteTimer2
    timerConcateneted = timerConcateneted + "-"
    timerConcateneted = timerConcateneted + daysIrrigating
    timerConcateneted = timerConcateneted + "*"

    con.write(timerConcateneted)
  }

  useEffect(()=>{
    connectButton()
  },[])

  const data = [
    {
      id: '1',
      title: 'Portão',
      buttonSize: 'big',
      callback: handleClick
    },
    {
      id: '2',
      title: ['Luz', 'Luz'],
      buttonSize: 'double',
      callback: handleClick
    },
   
  ]

  
  if (props.screen == 'tractor'){
    return(
        <View style = {styles.container}>

          <Text style = {{flex:1, fontSize:40}} >Chácara</Text>

          <View style={{height:0.7, width: screenWidth*0.9, backgroundColor: '#aaa'}}></View>
        
          <SendTimeButton onClick={sendTimerData} />

          <MyButton onClick={e => handleLampz} buttonStyle={'small'} text1={'Irrigar'} text2={'(24 horas)'} />

          <View style={{backgroundColor:'white', height:screenHeight*0.5}} ></View>
        </View>    
    )
  }
  if (props.screen == 'home'){
    return(
      <View style = {styles.container}>
          <Text style = {{flex:1, fontSize:40}} >Casa</Text>

          <View style={{height:0.7, width: screenWidth*0.9, backgroundColor: '#aaa'}}></View>

          {/* <Button title={'On/Off'} onPress={e => handleLamp(e,'0')} /> 
          <Button title={'On/Off'} onPress={e => handleLamp(e,'1')} /> 
          <TouchableHighlight title={'On/Off'} disabled={button} onPress={handleGate} />  */}
        <HouseFloorPlan onClick={handleClick} />

        <View style={{backgroundColor:'white', height:screenHeight*0.1}} ></View>
       
      </View>
    )
  }

  if (props.screen == 'gear'){
    return(

        <View style = {styles.container}>
          
          <View style={styles.ipArea}>
              <TextInput 
                style = {[styles.input, styles.inputIp]}
                keyboardType = "numeric"
                style={styles.input} 
                autoCapitalize = "none"
                autoCorrect = {false}
                value = {host}
                onChangeText = {v => setHost(v)}
              />
              <TextInput 
                style = {[styles.input, styles.inputPort]}
                keyboardType = "numeric"
                style={styles.input} 
                autoCapitalize = "none"
                autoCorrect = {false}
                value = {port}
                onChangeText = {v => setPort(v)}
              />
          </View>

          <Button title={"Conectar"} onPress={connectButton} />

          <View style={{backgroundColor:'white', height:screenHeight*0.8}} ></View>

        </View>
    )
  }
}

const styles = StyleSheet.create({
    
    container:{
      flex:1,
      justifyContent: 'center',
      alignItems: 'center',
      padding:10,
      backgroundColor:"transparent"
    },

    buttonStyle: {
      width: '50%',
      backgroundColor: '#FFF',
      borderRadius: 25,
      padding: 10,
      justifyContent: 'center',
      alignItems: 'center',
    },

    biometryText: {
      color: '#FFF',
      fontSize: 17,
      fontWeight: 'bold',
      marginTop: 30,
    },

    mainView: {
      alignItems: 'center',  
      marginTop: -100,
      backgroundColor: '#333'
    },

    mainFont:{
      fontSize: 40,
      color: '#EEE',
      marginTop:-50
    },

    input: {
      fontSize: 22,
      padding: 15,
      backgroundColor: '#00213B',
      borderRadius: 5,
      color: '#EEE',
      margin: 10
    },
    
    ipArea: {
      flexDirection: 'row',
      justifyContent: 'center',
      margin: 20
    },

    inputIp: {
      width: 150
    },

    inputPort: {
      width: 70
    },

    inputTimer: {
      width: 70
    },

    button: {
      backgroundColor: "#d6f34f",
      width:50,
      height: 50,
      borderRadius: 100,
      alignItems:'center',
      justifyContent:'center',
    }
  })
  

