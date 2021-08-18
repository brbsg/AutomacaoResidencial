import { Platform } from "react-native";

const server = Platform.OS === 'ios'
    ? 'http://localhost:3000' : 'http://10.0.2.2:3000'

function showError(err){
    alert('Ocorreu um problema!', err)
}

function showSucces(msg){
    alert('Sucesso!', msg)
}

export {server, showError, showSucces}