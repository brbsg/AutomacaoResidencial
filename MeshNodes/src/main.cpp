
#include <ESP8266WiFi.h>
#include <espnow.h>

#define NODE_ID "gustavoRoom"

#define RELAY 12
#define LIGHT_SWITCH 4
#define CHANNEL 1

void setupWiFi();
void onDataSent(uint8_t *macAddr, uint8_t sendStatus);
void onDataReceive(uint8_t *macAddr, uint8_t *incomingData, uint8_t len);
void setupESPNOW();

uint8_t macPeers[6][6] = {
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, //MAC de Broadcast
  //{0x3C, 0x61, 0x05, 0x12, 0xA1, 0x20}, // MAC ESP32 Master
  {0xE8, 0x68, 0xE7, 0x44, 0x60, 0x0B}, // MAC Sonoff Portão
  {0xD8, 0xBF, 0xC0, 0x10, 0xC1, 0x11},
  {0xDC, 0x4F, 0x22, 0x7E, 0x4D, 0x33},
  {0xEC, 0xFA, 0xBC, 0x62, 0xB4, 0xF0},
  {0x2C, 0xF4, 0x32, 0x49, 0xD0, 0xC7},
  };



char lastRandom[20];
bool openGate = false;

int buttonPushCounter = 0;   
int buttonState = 0;         
int lastButtonState = 1; 

  
void setup() {
  delay(2000);
  Serial.begin(115200);

  pinMode(RELAY,OUTPUT);
  pinMode(LIGHT_SWITCH, INPUT);


  WiFi.mode(WIFI_STA);

  setupESPNOW();

  
}



void loop() {

 // read the pushbutton input pin:
  buttonState = digitalRead(LIGHT_SWITCH);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    digitalWrite(RELAY, !digitalRead(RELAY));
    delay(50); // Delay a little bit to avoid bouncing
  }
  
  lastButtonState = buttonState; // save the current state as the last state, for next time through the loop


  if(openGate){
    digitalWrite(12, HIGH);
    delay(250);
    digitalWrite(12, LOW);
    openGate = false;
  }


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
  Serial.print("Mensagem Recebida:");
  
  char incomingMessage[30];
  char msg[30];
  char tempOn[30] = NODE_ID;
  //char tempOff[30] = NODE_ID;
  char tempMsg[30];
  char currentRandom[30];

//  strcat(tempOn, "on");  
//  strcat(tempOff, "off");

  strcpy(msg, "");//Tem que inicializar o msg com uma string vazia senão dá ruim. Por que? Não tenho a mínima ideia.

  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

  char* token = strtok(incomingMessage, "*");

  strcpy(currentRandom, token);
  strcpy(tempMsg, token);
  token = strtok(NULL,"*");
  strcat(msg, token);
  strcat(tempMsg, "*");
  strcat(tempMsg, token);

  if(strcmp(currentRandom, lastRandom) == 0){
    return;
  }


  strcpy(lastRandom, currentRandom);

  Serial.println(msg);

  if(strcmp(NODE_ID, "GATE")==0 ){
    if(strcmp(tempOn, msg) == 0){
      openGate = true;
    }   
  }else{
     if(strcmp(tempOn, msg) == 0){

      digitalWrite(RELAY, !digitalRead(RELAY));
     // lastButtonState = digitalRead(RELAY);
      
//    }else if(strcmp(tempOff, msg) == 0){
//      //digitalWrite(12, HIGH);
    }
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