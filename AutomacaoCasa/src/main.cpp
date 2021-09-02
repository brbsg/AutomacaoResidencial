#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
// #include <SD.h>
// #include <SPI.h>
// #include <Wire.h>
//#include <IRremoteESP8266.h>


#define MAXCLIENTS 8
#define CHANNEL 1
#define BUILTIN_LED 2
#define FLASH_BUTTON 0 

#define SSID "Toni"
#define PASSWORD "oliveira" 
#define PORT 80



// // //=========================================================



void availableClient();
void availableMessage();
void handleClient();
void setupWiFi();

void setupESPNOW();
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void onDataReceive(const uint8_t *macAddr, const uint8_t *incomingData, int len);

void getDateTime();

void sendMessage();

void refreshConnections();



IPAddress local_IP(192, 168,4, 117);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   
IPAddress secondaryDNS(8, 8, 4, 4);


WiFiClient clients[MAXCLIENTS];
WiFiServer server(PORT);




// // //========================================================



//File myFile;


// // //hw_timer_t *timer = NULL;



String msg;



// // // //========================================================



uint8_t macPeers[][6] = {
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, //MAC de Broadcast
  //{0x3C, 0x61, 0x05, 0x12, 0xA1, 0x20}, // MAC ESP32 Master
  {0xE8, 0x68, 0xE7, 0x44, 0x60, 0x0B}, // MAC Sonoff Portão

  };


// // // seco = 766
// // // molhado = 380

char lastRandom[30];

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

  // pinMode(BUILTIN_LED, OUTPUT);
  // digitalWrite(BUILTIN_LED, LOW);

  setupWiFi();  
  
  setupESPNOW();
  
  int slavesCount = sizeof(macPeers)/6/sizeof(uint8_t);

  for(int i=0; i<slavesCount; i++){
    esp_now_peer_info_t slave;

    slave.channel = CHANNEL;
    slave.encrypt = 0;
    memcpy(slave.peer_addr,macPeers[i], sizeof(macPeers[i]));

    esp_now_add_peer(&slave);

  }

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


  availableClient();
  availableMessage();

  //getDateTime();

  // if(hora >= 20 ){
  //   digitalWrite(D0, LOW);
  // // }else if(hora == 20 && minuto > 0 && minuto < 30){
  // //   digitalWrite(D0, LOW);
  // }else{
  //   digitalWrite(D0, HIGH);
  // } 
  
  

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




void refreshConnections(){
  // Flag que indica se pelo menos um client ser desconectou
 // bool flag = false;
  
  // Objeto que receberá apenas os clients conectados
 // std::vector<WiFiClient> newVector;

  // Percorremos o vector
  // for(int i=0; i<clients.size(); i++){
  //   // Verificamos se o client está desconectado
  //   if(!clients[i].connected()){
  //     // Exibimos na serial que um cliente se desconectou e a posição em que ele está no vector (debug)
  //     Serial.println("Client disconnected! ["+String(i)+"]");
  //     // Desconectamos o client
  //     clients[i].stop();
  //     // Setamos a flag como true indicando que o vector foi alterado
  //     flag = true;          
  //   }
  //   else{
  //     newVector.push_back(clients[i]); // Se o client está conectado, adicionamos no newVector
  //   }
  // }
  // // Se pelo menos um client se desconectou, atribuimos ao vector "clients" os clients de "newVector"
  // if(flag) clients = newVector;
}



void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){

  if(status == 0){
    Serial.println("Entregado com sucesso");
  }else{
    Serial.println("Falha ao entregar");
  }
} 



void onDataReceive(const uint8_t *macAddr, const uint8_t *incomingData, int len){
  Serial.println("Mensagem Recebida: ");
  
  char incomingMessage[30];
  char currentRandom[20];
  char msg[20];
  char tempMsg[30];

  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

  // Serial.println(incomingMessage);
  

  char* token = strtok(incomingMessage, "*");

  strcpy(currentRandom, token);
  strcpy(tempMsg, token);
  token = strtok(NULL,"*");
  strcat(msg, token);
  strcat(tempMsg, "*");
  strcat(tempMsg, token);

  Serial.println(tempMsg);

  if(strcmp(currentRandom, lastRandom) == 0){
    return;
  }

  esp_now_send(macPeers[0], (uint8_t *) & tempMsg, sizeof(tempMsg) );
  
  strcpy(lastRandom, currentRandom);
  
//   if(incomingMessage == ""){
//     digitalWrite(2,LOW);
//   }else{
//     digitalWrite(2,HIGH);
//   } 
}


void setupESPNOW(){
  Serial.print("MAC Adress: ");
  Serial.println(WiFi.macAddress());


  if(esp_now_init() == ESP_OK){
    Serial.println("ESP Now inicializado com sucesso");
    //digitalWrite(2, LOW);
  } else {
    Serial.println("ESP Now falhou");
    ESP.restart();
  }

  esp_now_register_recv_cb(onDataReceive);
  esp_now_register_send_cb(onDataSent);

}



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



void availableClient(){
  if(server.hasClient()){
    for(int i=0;i < MAXCLIENTS; i++){
     if(!clients[i] || !clients[i].connected()){
        if(clients[i]){
          clients[i].stop();
        }
        if(clients[i] = server.available()){
          Serial.println("Novo Cliente: " + String(i+1));
        }
        continue;
      }
    }
    //Serial.println(String(clients[1]));
    WiFiClient clients = server.available();
    clients.stop();
  }
}



void availableMessage(){
  for(int i=0;i < MAXCLIENTS; i++){
    if(clients[i] && clients[i].connected() && clients[i].available() ){
      while(clients[i].available()){
        msg = clients[i].readStringUntil('*');
        clients[i].flush();
        Serial.println("Cliente " + String(i+1) + ": " + msg );
        handleClient();
      }
    }
  }
}

void handleClient(){ 
  char buffer[30];
  int randomNum = random(1,51257);
  itoa(randomNum, buffer, 10);
  strcat(buffer, "*");
  strcat(buffer, msg.c_str());

  if(strcmp(msg.c_str(), "GATE")==0){
    strcat(buffer, "on");

  }

  esp_now_send(macPeers[0], (uint8_t *) & buffer, sizeof(buffer) );
  

  if(msg[0]== 'i'){
    if(msg.lastIndexOf("on") > -1){
    
      Serial.print("Send Status: ");
      

    }
    if(msg.lastIndexOf("off") > -1){
     // digitalWrite(D0, HIGH);
    }
    
  }
}

void setupWiFi(){
  
  //WiFi.config(local_IP, gateway, subnet); 
  
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  //digitalWrite(2, LOW)


  server.begin();

  Serial.println("Servidor Conectado");
  Serial.print("IP para se conectar ao NodeMCU: ");
  Serial.println(WiFi.localIP());
  //Serial.println(WiFi.softAPIP());

}


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



    


