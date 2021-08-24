#include <ESP8266WiFi.h> 
#include <espnow.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include <painlessMesh.h>
#include <Schedule.h>
#include <TaskScheduler.h>

//#include <IRremoteESP8266.h>

#define ID_NODE "ID:1-"

#define MAXCLIENTS 6
#define CHANNEL 8
#define CS_PIN 15

#define   MESH_PREFIX     "RedeMesh"
#define   MESH_PASSWORD   "Mesh1234@"
#define   MESH_PORT       5555

//ESP - Local IP Address: 192.168.4.1
char ssid[] = "Toni";
char password[] = "oliveira";


//================= =======================================



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




//========================================================


File myFile;
RTC_DS3231 rtc;

painlessMesh mesh;
Scheduler userScheduler;
Task taskSendMessage(TASK_SECOND * 1 , TASK_FOREVER, &sendMessage);


//hw_timer_t *timer = NULL;

WiFiServer server(80);
WiFiClient client[MAXCLIENTS];

IPAddress ip(192, 168, 0, 117);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

String msg;



//========================================================


// seco = 766
// molhado = 380
// NodeMCU-1 = 2C:F4:32:49:D0:C7
// NodeMCU-2 = D8:BF:C0:10:C1:11
// ESP01-1 = DC:4F:22:7E:4D:33
// ESP01-2 = EC:FA:BC:62:B4:F0
// ESP01-3= EC:FA:BC:62:B4:10

uint8_t macPeers[5][6] = {
  {0xEC, 0xFA, 0xBC, 0x62, 0xB4, 0x10},
  {0xD8, 0xBF, 0xC0, 0x10, 0xC1, 0x11},
  {0xDC, 0x4F, 0x22, 0x7E, 0x4D, 0x33},
  {0xEC, 0xFA, 0xBC, 0x62, 0xB4, 0xF0},

  {0x2C, 0xF4, 0x32, 0x49, 0xD0, 0xC7}
  };

uint8_t macBroadcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

char weekDays[7][8] = {"Domingo","Segunda","Terca", "Quarta", "Quinta", "Sexta", "Sabado"};
int segundo = 0;
int minuto = 51;
int hora = 18;
int dia = 24;
int mes = 4;
int ano = 2021;
String singleDay = "";

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 1000;

int diaBuffer = 0;

//========================================================



void setup(){
  startMillis = millis();
  pinMode(2, OUTPUT);
  pinMode(D0,OUTPUT);
  digitalWrite(2, HIGH);
  digitalWrite(D0, HIGH);
  
  Serial.begin(115200);
  Serial.println("");
  
  setupWiFi();
  
  //setupESPNOW();
  
  Wire.begin(D1, D2);
  Wire.status();
  if(rtc.begin()){
    //rtc.adjust(DateTime(ano, mes, dia, hora, minuto, segundo)); //Descomentar a linha se quiser atualizar o horário do RTC
    Serial.println("RTC Inicializado com sucesso!");
  }else{
    Serial.println("Erro ao iniciar RTC DS3231");
    rtc.begin();
    return;
  }

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  // if(SD.begin(CS_PIN)){
  //   Serial.println("Cartão inicializado");
  // }else{
  //   Serial.println("Falha, verifique se o cartão está plugado");
  //   return;
  // }

}



//========================================================



void loop(){
  currentMillis = millis();

  mesh.update();

  //getDateTime();

  if(hora >= 20 ){
    digitalWrite(D0, LOW);
  // }else if(hora == 20 && minuto > 0 && minuto < 30){
  //   digitalWrite(D0, LOW);
  }else{
    digitalWrite(D0, HIGH);
  } 
  
  availableClient();
  availableMessage();

  if(startMillis-currentMillis == 1000){
    sendMessage();
    startMillis = currentMillis;
  }
  
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



void receivedCallback( uint32_t from, String &msg ) {
  
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void sendMessage() {
  String msg = ID_NODE;
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

bool setIrrigationTime(int horaInicial, int minutoInicial, int horaFinal, int minutoFinal, int diasIrrigando){

  bool irrigaBool = false;
  bool horaBool = false;
  bool minutoBool;
 
  if(diaBuffer == 0){
    diaBuffer = 0;
  }

  if(diasIrrigando == 0){

    if(hora == 165){// quando o RTC buga tem que chamar esse método. Ele fornece o dado assim:(Terca) 165/165/2165 165:165:85
      Wire.status();
    }

    if(horaInicial < horaFinal){
      if(hora >= horaInicial && hora < horaFinal) horaBool = true;
    }

    if(horaInicial > horaFinal){
      if(hora >= horaInicial || hora < horaFinal) horaBool = true;
    }

    if(horaInicial == horaFinal){
      if(hora == horaInicial) horaBool = true;
    }

    if(hora == horaInicial){
      if(minuto >= minutoInicial) minutoBool = true;
    }

    if(horaBool == true){
      if(hora == horaInicial){
        if(minuto >= minutoInicial){
          minutoBool = true;
        }else minutoBool = false;
      }else if(hora == horaFinal){
        if(minuto <= minutoInicial){
          minutoBool = true;
        }else minutoBool = false;
      }else minutoBool = true;
    }

    if(horaBool == true && minutoBool == true) irrigaBool = true;

  }// if(diasIrrigando)

  if(diasIrrigando >= 1){
    

  }

  
  return irrigaBool;
}// setIrrigationTime
 
void getDateTime(){
  DateTime now = rtc.now();

  segundo = now.second();
  minuto = now.minute();
  hora = now.hour();
  dia = now.day();
  mes = now.month();
  ano = now.year();
  singleDay = weekDays[now.dayOfTheWeek()];

}


void onDataSent(uint8_t *macAddr, uint8_t sendStatus){
  Serial.print("Status dos últimos dados: ");
  if(sendStatus == 0){
    Serial.println("Entregado com sucesso");
  }else{
    Serial.println("Falha ao entregar");
  }
} 


void onDataReceive(uint8_t *macAddr, uint8_t *incomingData, uint8_t len){
  Serial.print("Mensagem Recebida: ");
  
  bool incomingMessage;

  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

  Serial.println(incomingMessage);
  
  if(incomingMessage == 1){
    digitalWrite(2,LOW);
  }else{
    digitalWrite(2,HIGH);
  } 
}


void setupESPNOW(){
  Serial.print("MAC Adress: ");
  Serial.println(WiFi.macAddress());

  if(esp_now_init() == 0){
    Serial.println("ESP Now inicializado com sucesso");
    //digitalWrite(2, LOW);
   
  } else {
    Serial.println("ESP Now falhou");
    ESP.restart();
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  
  int slavesCount = sizeof(macPeers)/6/sizeof(uint8_t);

  for(int i=0; i<slavesCount; i++){
    esp_now_add_peer(macPeers[i],ESP_NOW_ROLE_COMBO,CHANNEL,NULL, 0);
  }

  esp_now_register_recv_cb(onDataReceive);
  esp_now_register_send_cb(onDataSent);
}


void availableClient(){
  if(server.hasClient()){
    for(int i=0;i < MAXCLIENTS; i++){
     if(!client[i] || !client[i].connected()){
        if(client[i]){
          client[i].stop();
        }
        if(client[i] = server.available()){
          Serial.println("Novo Cliente: " + String(i+1));
        }
        continue;
      }
    }
    //Serial.println(String(client[1]));
    WiFiClient client = server.available();
    client.stop();
  }
}



void availableMessage(){
  for(int i=0;i < MAXCLIENTS; i++){
    if(client[i] && client[i].connected() && client[i].available() ){
      while(client[i].available()){
        msg = client[i].readStringUntil('*');
        client[i].flush();
        Serial.println("Cliente " + String(i+1) + ": " + msg );
        handleClient();
      }
    }
  }
}

void handleClient(){ 
  int handleClient = msg[0] - '0';
  bool myMessage ;

  if(msg[0] == '0'){
    myMessage = true;
    esp_now_send(macPeers[handleClient], (uint8_t *) & myMessage, sizeof(myMessage) ); 
  }
  if(msg[0]== 'i'){
    if(msg.lastIndexOf("on") > -1){
      digitalWrite(D0, LOW);
    }
    if(msg.lastIndexOf("off") > -1){
      digitalWrite(D0, HIGH);
    }
    
  }
}


void setupWiFi(){
  
  WiFi.begin(ssid, password);
  
  //WiFi.disconnect();

  WiFi.config(ip, gateway, subnet);

  // while (WiFi.status() != WL_CONNECTED){
  //   delay(500);
  //   Serial.print(".");
  // }

  //digitalWrite(2, LOW);

  Serial.println("");
  Serial.println("ESP32 está funcionando como AP. ");
  
  server.begin();

  // Serial.println("Servidor Conectado");
  // Serial.print("IP para se conectar ao NodeMCU: ");
  // Serial.println(WiFi.localIP());

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
    


