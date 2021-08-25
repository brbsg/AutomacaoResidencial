#include <ESP8266WiFi.h> 
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include <painlessMesh.h>
#include <vector>
//#include <IRremoteESP8266.h>

#define ID_NODE "ID:1-"

#define MAXCLIENTS 6


#define   MESH_SSID "RedeMesh"
#define   MESH_PASSWORD "Mesh1234@"
#define   MESH_PORT 5555

#define SSID "Toni"
#define PASSWORD "oliveira" 
#define PORT 80


// // //================= =======================================



void availableClient();
void availableMessage();
void handleClient();
void setupWiFi();

void initESPNow();
void setupESPNOW();
void onDataSent();
void onDataReceive();

void getDateTime();

void sendMessage();

void receivedCallback( uint32_t from, String &msg );
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);

void taskPrintMeshTopology();
void taskNewClients();
void taskHandleClients();
void taskCheckIP();

void refreshConnections();
void handleCommand(String cmd);
void sendToClients(String msg);
void receivedCallback(uint32_t from, String & cmd);
bool verifyAndSetRelayState(String state);

void setupMesh();




// // //========================================================


File myFile;
RTC_DS3231 rtc;

painlessMesh mesh;
Scheduler scheduler;


//Tarefa que mostra no monitor serial a topologia da rede mesh de 1 em 1 segundo
Task t1(1000, TASK_FOREVER, &taskPrintMeshTopology, &scheduler, true);
//Tarefa para verificar uma nova conexão feita pelo aplicativo está sendo feita
Task t2(1, TASK_FOREVER, &taskNewClients, &scheduler, true);
//Tarefa para verificar se há novas mensagens do aplicativo
Task t3(1, TASK_FOREVER, &taskHandleClients, &scheduler, true);
//Tarefa para verificar e guardar mudanças no IP
Task t4(1000, TASK_FOREVER, &taskCheckIP, &scheduler, true);


// // //hw_timer_t *timer = NULL;

WiFiServer server(PORT);
std::vector<WiFiClient> clients;

IPAddress myIP;

IPAddress ip(192, 168, 0, 117);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

String msg;
String ID = "LIGHT1";
String ID_ON = ID + " ON";
String ID_OFF = ID + " OFF";

String currentState = ID_OFF;



// // //========================================================


// // // seco = 766
// // // molhado = 380

// char weekDays[7][8] = {"Domingo","Segunda","Terca", "Quarta", "Quinta", "Sexta", "Sabado"};
// int segundo = 0;
// int minuto = 51;
// int hora = 18;
// int dia = 24;
// int mes = 4;
// int ano = 2021;
// String singleDay = "";

// int diaBuffer = 0;


// // unsigned long startMillis;
// // unsigned long currentMillis;
// // const unsigned long period = 1000;


// //========================================================



void setup(){
  Serial.begin(115200);

  // startMillis = millis();

  // // pinMode(2, OUTPUT);
  // // pinMode(D0,OUTPUT);
  // // digitalWrite(2, HIGH);
  // // digitalWrite(D0, HIGH);

  ID_ON.toUpperCase();
  ID_OFF.toUpperCase();

  server.begin(PORT);

  setupMesh();

  scheduler.startNow();
  

  
  // Wire.begin(D1, D2);
  // Wire.status();
  // if(rtc.begin()){
  //   //rtc.adjust(DateTime(ano, mes, dia, hora, minuto, segundo)); //Descomentar a linha se quiser atualizar o horário do RTC
  //   Serial.println("RTC Inicializado com sucesso!");
  // }else{
  //   Serial.println("Erro ao iniciar RTC DS3231");
  //   rtc.begin();
  //   return;
  // }

  // if(SD.begin(CS_PIN)){
  //   Serial.println("Cartão inicializado");
  // }else{
  //   Serial.println("Falha, verifique se o cartão está plugado");
  //   return;
  // }

}



//========================================================



void loop(){


  mesh.update();

  scheduler.execute();

  //getDateTime();

  // if(hora >= 20 ){
  //   digitalWrite(D0, LOW);
  // // }else if(hora == 20 && minuto > 0 && minuto < 30){
  // //   digitalWrite(D0, LOW);
  // }else{
  //   digitalWrite(D0, HIGH);
  // } 
  
  // availableClient();
  // availableMessage();

  // if(startMillis-currentMillis == 1000){
  //   sendMessage();
  //   startMillis = currentMillis;
  // }
  
  //myFile = SD.open("LOG.txt", FILE_WRITE);

  // if(myFile){

  //   myFile.print(" (");
  //   myFile.print(singleDay);
  //   myFile.print(") ");
  //   myFile.print(dia);
  //   myFile.print("/");
  //   myFile.print(mes);
  //   myFile.print("/");
  //   myFile.print(ano);
  //   myFile.print(" ");
  //   myFile.print(hora);
  //   myFile.print(":");
  //   myFile.print(minuto);
  //   myFile.print(":");
  //   myFile.println(segundo);
    
  // if(currentMillis-startMillis >= period){
  //   Serial.print(" (");
  //   Serial.print(singleDay);
  //   Serial.print(") ");
  //   Serial.print(dia);
  //   Serial.print("/");
  //   Serial.print(mes);
  //   Serial.print("/");
  //   Serial.print(ano);
  //   Serial.print(" ");
  //   Serial.print(hora);
  //   Serial.print(":");
  //   Serial.print(minuto);
  //   Serial.print(":");
  //   Serial.println(segundo);
    
  //    startMillis = currentMillis;
  // }
  //   Serial.println("Arquivo Escrito");

  //   myFile.close();
    
  // }else{
  //   Serial.println("Falha ao abrir o arquivo");
  //   SD.begin(CS_PIN);
  //   return;
  //}

} 



//========================================================



void taskPrintMeshTopology() {
  Serial.println(mesh.subConnectionJson());
}

void taskNewClients(){
  // Se existir um novo client atribuimos para a variável
  WiFiClient newClient = server.available(); 
  
  // Se o client for diferente de nulo
  if(newClient) {      
    // Inserimos no vector
    clients.push_back(newClient);
    // Exibimos na serial indicando novo client e a quantidade atual de clients
    Serial.println("New client! size:"+String(clients.size()));
  }
}  

// Função que verifica se o app enviou um comando
void taskHandleClients()
{
  // String que receberá o comando vindo do aplicativo
  String cmd;

  // Atualizamos o vector deixando somente os clientes conectados
  refreshConnections();

  // Percorremos o vector
  for(int i=0; i<clients.size(); i++){      
    // Se existir dados a serem lidos
    if(clients[i].available()){
      // Recebemos a String até o '\n'
      cmd = clients[i].readStringUntil('*');
      // Verificamos o comando, enviando por parametro a String cmd
      handleCommand(cmd);        
    }          
  }
}

void handleCommand(String cmd)
{
  // Se a String estiver vazia nao precisamos fazer nada
  if (cmd.equals(""))
    return;

  //Coloca todos os caracters em maiúsculo
  cmd.toUpperCase();

  // Exibimos o comando recebido na serial
  Serial.println("Received from app: " + cmd);
  //Verifica se a mensagem é para este nó e modifica o estado do relê de acordo com o que foi enviado
  if(verifyAndSetRelayState(cmd)) {
    //Envia mensagem de confirmaçao de volta para o app
    String confirmationMessage = cmd + " OK";
    sendToClients(confirmationMessage);
    Serial.println("Changed Relay status for this node: " + confirmationMessage);
  }
  //A mensagem não era para este nó então envia para todos os nós da rede mesh
  else {
    Serial.println("Sending command through mesh network");
    //Envia o comando para todos os nós da rede mesh
    mesh.sendBroadcast(cmd);
  }
}

void refreshConnections(){
  // Flag que indica se pelo menos um client ser desconectou
  bool flag = false;
  
  // Objeto que receberá apenas os clients conectados
  std::vector<WiFiClient> newVector;

  // Percorremos o vector
  for(int i=0; i<clients.size(); i++){
    // Verificamos se o client está desconectado
    if(!clients[i].connected()){
      // Exibimos na serial que um cliente se desconectou e a posição em que ele está no vector (debug)
      Serial.println("Client disconnected! ["+String(i)+"]");
      // Desconectamos o client
      clients[i].stop();
      // Setamos a flag como true indicando que o vector foi alterado
      flag = true;          
    }
    else{
      newVector.push_back(clients[i]); // Se o client está conectado, adicionamos no newVector
    }
  }
  // Se pelo menos um client se desconectou, atribuimos ao vector "clients" os clients de "newVector"
  if(flag) clients = newVector;
}

// //Função que envia mensagem para todos os apps conectados
void sendToClients(String msg) {
  for(int i=0; i<clients.size(); i++){
    clients[i].print(msg);
  }
}

bool verifyAndSetRelayState(String state) {
  //Se for para ligar o relê deste nó
  if(state == ID_ON) {
    //Colocamos o pino em LOW por causa do nosso relê que libera a passagem de corrente quando está em LOW
    digitalWrite(2, LOW);
    //Guarda o estado atual
    currentState = ID_ON;
    //Exibe informações do display (se USE_DISPLAY estiver habilitado)

    return true;
  }

  //Se for para ligar o relê deste nó
  if (state == ID_OFF) {
    //Colocamos o pino em HIGH por causa do nosso relê que corta a passagem de corrente quando está em HIGH
    digitalWrite(2, HIGH);
     //Guarda o estado atual
    currentState = ID_OFF;
    //Exibe informações do display (se USE_DISPLAY estiver habilitado)

    return true;
  }

  return false;
}

void taskCheckIP() {
  //Guarda o ip que o roteador deu para o esp
  IPAddress ip = IPAddress(mesh.getStationIP());

  //Se o ip mudou
  if(myIP != ip){
    //Guardamos o ip
    myIP = ip;
    //Mostramos o ip no monitor serial
    Serial.println("IP: " + myIP.toString());
    //Exibe informações do display (se USE_DISPLAY estiver habilitado)
  }
}

void setupMesh() {
  //As mensagens de debug da rede nesh apareceram no monitor serial
  mesh.setDebugMsgTypes(ERROR | DEBUG| STARTUP | CONNECTION);

  // O canal (nesse caso 6) deve ser o mesmo da rede do seu roteador. Verifique qual o canal se roteador usa e modifica de 6 para o número correto
  mesh.init( MESH_SSID, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, 6);

  //Conecta ao roteador
  mesh.stationManual(SSID, PASSWORD);

  //Informamos qual função irá ser executada quando recebemos uma mensagem
  mesh.onReceive(&receivedCallback);
}

//Função que definimos que irá receber as mensagens da rede mesh
void receivedCallback(uint32_t from, String & cmd) {
  //Coloca todos os caracteres do comando em letra maiúscula
  cmd.toUpperCase(); 

  //Mostra o comando recebido
  Serial.println("Received from mesh: " + cmd);
   //Verifica se o comando é para este nó e modifica o estado do relê de acordo com o que foi enviado
  if(verifyAndSetRelayState(cmd)) {
    //Envia mensagem de confirmação para o app
    String confirmationMessage = cmd + " OK";
    sendToClients(confirmationMessage);
    Serial.println("Changed Relay status for this node: " + confirmationMessage);
  }
  //Algum nó enviou de volta uma confirmação que devemos enviar para o app
  else {
    sendToClients(cmd);
  }
}


// void newConnectionCallback(uint32_t nodeId) {
//     Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
// }

// void changedConnectionCallback() {
//   Serial.printf("Changed connections\n");
// }

// void nodeTimeAdjustedCallback(int32_t offset) {
//     Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
// }








// bool setIrrigationTime(int horaInicial, int minutoInicial, int horaFinal, int minutoFinal, int diasIrrigando){

//   bool irrigaBool = false;
//   bool horaBool = false;
//   bool minutoBool;
 
//   if(diaBuffer == 0){
//     diaBuffer = 0;
//   }

//   if(diasIrrigando == 0){

//     if(hora == 165){// quando o RTC buga tem que chamar esse método. Ele fornece o dado assim:(Terca) 165/165/2165 165:165:85
//       Wire.status();
//     }

//     if(horaInicial < horaFinal){
//       if(hora >= horaInicial && hora < horaFinal) horaBool = true;
//     }

//     if(horaInicial > horaFinal){
//       if(hora >= horaInicial || hora < horaFinal) horaBool = true;
//     }

//     if(horaInicial == horaFinal){
//       if(hora == horaInicial) horaBool = true;
//     }

//     if(hora == horaInicial){
//       if(minuto >= minutoInicial) minutoBool = true;
//     }

//     if(horaBool == true){
//       if(hora == horaInicial){
//         if(minuto >= minutoInicial){
//           minutoBool = true;
//         }else minutoBool = false;
//       }else if(hora == horaFinal){
//         if(minuto <= minutoInicial){
//           minutoBool = true;
//         }else minutoBool = false;
//       }else minutoBool = true;
//     }

//     if(horaBool == true && minutoBool == true) irrigaBool = true;

//   }// if(diasIrrigando)

//   if(diasIrrigando >= 1){
    

//   }

  
//   return irrigaBool;
// }// setIrrigationTime
 
// void getDateTime(){
//   DateTime now = rtc.now();

//   segundo = now.second();
//   minuto = now.minute();
//   hora = now.hour();
//   dia = now.day();
//   mes = now.month();
//   ano = now.year();
//   singleDay = weekDays[now.dayOfTheWeek()];

// }



// void availableClient(){
//   if(server.hasClient()){
//     for(int i=0;i < MAXCLIENTS; i++){
//      if(!clients[i] || !clients[i].connected()){
//         if(clients[i]){
//           clients[i].stop();
//         }
//         if(clients[i] = server.available()){
//           Serial.println("Novo Cliente: " + String(i+1));
//         }
//         continue;
//       }
//     }
//     //Serial.println(String(clients[1]));
//     WiFiClient clients = server.available();
//     clients.stop();
//   }
// }



// void availableMessage(){
//   for(int i=0;i < MAXCLIENTS; i++){
//     if(clients[i] && clients[i].connected() && clients[i].available() ){
//       while(clients[i].available()){
//         msg = clients[i].readStringUntil('*');
//         clients[i].flush();
//         Serial.println("Cliente " + String(i+1) + ": " + msg );
//         handleClient();
//       }
//     }
//   }
// }

// void handleClient(){ 
//   int handleClient = msg[0] - '0';
//   bool myMessage ;

//   if(msg[0] == '0'){
//     myMessage = true; 
//   }
//   if(msg[0]== 'i'){
//     if(msg.lastIndexOf("on") > -1){
//       digitalWrite(D0, LOW);
//     }
//     if(msg.lastIndexOf("off") > -1){
//       digitalWrite(D0, HIGH);
//     }
    
//   }
// }

// void setupWiFi(){
  
//   WiFi.begin(SSID, PASSWORD);
  
//   //WiFi.disconnect();

//   WiFi.config(ip, gateway, subnet);

//   // while (WiFi.status() != WL_CONNECTED){
//   //   delay(500);
//   //   Serial.print(".");
//   // }

//   //digitalWrite(2, LOW);

//   Serial.println("");
//   Serial.println("ESP32 está funcionando como AP. ");
  
//   server.begin();

//   // Serial.println("Servidor Conectado");
//   // Serial.print("IP para se conectar ao NodeMCU: ");
//   // Serial.println(WiFi.localIP());

// }

  // if(msg.lastIndexOf("on") > -1){
  //   myMessage = true;
  //   esp_now_send(macPeers[handleClient], (uint8_t *) & myMessage, sizeof(myMessage) );
  //   Serial.println(myMessage);
  // }
  // if(msg.lastIndexOf("off") > -1){
  //   myMessage = false;
  //   esp_now_send(macPeers[handleClient], (uint8_t *) & myMessage, sizeof(myMessage) );
  //   Serial.println(myMessage);
  // }
  // if(msg.lastIndexOf("client01-close") > -1){
    
  // }
    


