
#include <ESP8266WiFi.h>
#include <espnow.h>

#define RELAY 12
#define CHANNEL 6

void setupWiFi();

void initESPNow();
void setupESPNOW();
void onDataSent(uint8_t *macAddr, uint8_t sendStatus);
void onDataReceive();

uint8_t macPeers[5][6] = {
  {0xEC, 0xFA, 0xBC, 0x62, 0xB4, 0x10},
  {0xD8, 0xBF, 0xC0, 0x10, 0xC1, 0x11},
  {0xDC, 0x4F, 0x22, 0x7E, 0x4D, 0x33},
  {0xEC, 0xFA, 0xBC, 0x62, 0xB4, 0xF0},

  {0x2C, 0xF4, 0x32, 0x49, 0xD0, 0xC7}
  };

uint8_t macBroadcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void setup() {
  Serial.begin(115200);

  setupESPNOW();
  setupWiFi();
}



void loop() {
 
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

void setupWiFi(){
  WiFi.mode(WIFI_AP_STA);

  Serial.println(WiFi.macAddress());
  //WiFi.disconnect();


  // while (WiFi.status() != WL_CONNECTED){
  //   delay(500);
  //   Serial.print(".");
  // }

  //digitalWrite(2, LOW);

  Serial.println("");
  Serial.println("ESP32 está funcionando como AP. ");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  

  // Serial.println("Servidor Conectado");
  // Serial.print("IP para se conectar ao NodeMCU: ");
  // Serial.println(WiFi.localIP());

}