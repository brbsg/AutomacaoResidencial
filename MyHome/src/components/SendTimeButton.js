import React,{ useState} from 'react'
import {View, Text, TextInput, StyleSheet,TouchableOpacity, Dimensions} from 'react-native'

const screenWidth = Dimensions.get('window').width
const screenHeight = Dimensions.get('window').height

export default props => {

  const [hourTimer1, setHourTimer1] = useState('')
  const [hourTimer2, setHourTimer2] = useState('')
  const [minuteTimer1, setMinuteTimer1] = useState('')
  const [minuteTimer2, setMinuteTimer2] = useState('')
  const [daysIrrigating, setDaysIrrigating] = useState('');

  const valueInput = () =>{
    return "00"
  }
  return(
    
    <View style={styles.container}>

      <View style = {styles.timeArea}>

        <Text style = {{flex:1, fontSize:20}} >Hora Inicial:</Text>

        <TextInput 
          maxLength = {2} 
          keyboardType = "numeric"
          style={styles.input} 
          autoCapitalize = "none"
          autoCorrect = {false}
          placeholder={"00"}
          value = {`${hourTimer1}`}
          onChangeText = {v =>{
            if(v <= 24){
              setHourTimer1(v)
            }
          }}
        />

        <Text>:</Text>

        <TextInput 
          maxLength = {2}
          keyboardType = "numeric"
          style={styles.input} 
          autoCapitalize = "none"
          autoCorrect = {false}
          placeholder={"00"}
          value = {`${minuteTimer1}`}
          onChangeText = {v => {
            if(v <= 60){
              setMinuteTimer1(v)
            }
          }}
        />
      </View>
      
      <View style = {styles.timeArea} >

        <Text style = {{flex:1, fontSize:20}}>Hora Final:</Text>

        <TextInput 
        
          maxLength = {2}
          keyboardType = "numeric"
          style={styles.input} 
          autoCapitalize = "none"
          autoCorrect = {false}
          placeholder={"00"}
          value={`${hourTimer2}`}
          onChangeText = {v =>{ 
            if(v <= 24){
              setHourTimer2(v)
            }
          }}
        />

        <Text>:</Text>  

        <TextInput 
          maxLength = {2}
          keyboardType = "numeric"
          style={styles.input} 
          autoCapitalize = "none"
          autoCorrect = {false}
          placeholder={"00"}
          value = {`${minuteTimer2}`}
          onChangeText = {v =>{
            if(v <= 60){
              setMinuteTimer2(v)
            }
          }}
        />
        

      </View>

      <View style = {styles.timeArea}>

        <Text style = {{flex:1, fontSize:20}}>Dias Irrigando:</Text>

        <TextInput 
            maxLength = {2}
            keyboardType = "numeric"
            style={styles.input} 
            autoCapitalize = "none"
            autoCorrect = {false}
            placeholder={"00"}
            value = {`${daysIrrigating}`}
            onChangeText = {v =>{
              if(v <= 7){
                setDaysIrrigating(v)
              }
            }}
          />
      </View>
      
      <View style = {{alignItems:'center'}}>
        <TouchableOpacity onPress={() => props.onClick(hourTimer1,minuteTimer1,hourTimer2, minuteTimer2, daysIrrigating)} 
        style={styles.buttonStyle} 
        >
          <Text style={{color:'white'}} >ENVIAR</Text>
        </TouchableOpacity>
      </View>
      
      <View style={styles.line}></View>       
     
    </View>

  )
}

const styles = StyleSheet.create({
  
  container: {
    height: screenHeight*0.35,
    width: screenWidth*0.9,
    borderRadius: 10,
    justifyContent: 'center',

  },

  timeArea:{
    flexDirection: 'row',
    alignItems:'center',
    justifyContent: 'center',
  },
  input: {
    height: screenHeight*0.05,
    width: screenHeight*0.06,
    fontSize: 20,
    backgroundColor: '#DDD',
    borderRadius: 5,
    color:"#000420",
    margin: 10,
    padding:0,
    textAlign:'center',
  },
  line: {
    height:0.7,
    width: screenWidth*0.9,
    backgroundColor: '#aaa'
  },
  buttonStyle: {
    backgroundColor:"#000420", 
    width:screenWidth*0.4,
    height:screenWidth*0.08,
    alignItems:'center',
    justifyContent:'center',
    borderRadius: 5,
    marginBottom:10
  },
})
