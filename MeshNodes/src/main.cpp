
#include <painlessMesh.h>

#define RELAY 12

//Identificação da rede mesh. Deve ser igual a do outro arquivo
#define MESH_SSID "RedeMesh"
#define MESH_PASSWORD "Mesh1234@"
#define MESH_PORT 5555

void taskPrintMeshTopology();
void setupMesh();
bool verifyAndSetRelayState(String state);
void receivedCallback(uint32_t from, String & cmd);

//Id e funções deste nó (altere para cada nó)
String ID = "LIGHT2";
String ID_ON = ID + " ON";
String ID_OFF = ID + " OFF";
//Variável para guardar o valor do estado atual do relê 
String currentState = ID_OFF;

//Objeto responsável por conectar à rede mesh
painlessMesh  mesh;

//Objeto responsável por agendar as chamdas das tarefas (tasks)
Scheduler scheduler;
//Avisa o compilador que haverá uma função existe e será implementada depois
void taskPrintMeshTopology();
//Tarefa que chama a função taskPrintMeshTopology a cada 1 segundo, para sempre.
Task t1(1000, TASK_FOREVER, &taskPrintMeshTopology, &scheduler, true);

void setup() {
  Serial.begin(115200);

  //Coloca tudo em maiúsculo
  ID_ON.toUpperCase();
  ID_OFF.toUpperCase();
  
  
  //Coloca o pino onde o relê está como saída
  pinMode(RELAY, OUTPUT);

  //Inicializa a rede mesh
  setupMesh();

  //Verifica se o estado é válido e modifica o estado do relê (desligado neste caso)
  verifyAndSetRelayState(ID_OFF);

  //Inicializa o agendador de tarefas
  scheduler.startNow();
}


//Task que exibe a topologia da rede mesh no monitor serial
void taskPrintMeshTopology() {
  Serial.println(mesh.subConnectionJson());
}

void setupMesh() {
  //As mensagens de debug da rede nesh apareceram no monitor serial
  mesh.setDebugMsgTypes(ERROR | DEBUG| STARTUP | CONNECTION);

  //Conecta à rede mesh
  mesh.init(MESH_SSID, MESH_PASSWORD, MESH_PORT);

  //Informamos qual função irá ser executada quando recebemos uma mensagem
  mesh.onReceive(&receivedCallback);
}


//Função que verifica se o estado é valido para este nó e modifica o relê de acordo
bool verifyAndSetRelayState(String state) {
  //Se for para ligar o relê deste nó
  if(state == ID_ON) {
    //Colocamos o pino em LOW por causa do nosso relê que libera a passagem de corrente quando está em LOW
    digitalWrite(RELAY, LOW);
    //Guarda o estado atual
    currentState = ID_ON;
    //Exibe informações do display (se USE_DISPLAY estiver habilitado)
  }
  //Se for para ligar o relê deste nó
  if (state == ID_OFF) {
    //Colocamos o pino em HIGH por causa do nosso relê que corta a passagem de corrente quando está em HIGH
    digitalWrite(RELAY, HIGH);
    //Guarda o estado atual
    currentState = ID_OFF;
    //Exibe informações do display (se USE_DISPLAY estiver habilitado)
    return true; //retorna true para avisar que a mensagem era válida para este nó
  }
  //Se chegou aqui então a mensagem não era para este nó
  return false;
}


//Função que definimos que irá receber as mensagens
void receivedCallback(uint32_t from, String & cmd) {
  //Coloca todos os caracteres do comando em letra maiúscula
  cmd.toUpperCase(); 

  //Mostra o comando recebido
  Serial.println("Received from mesh: " + cmd);

  //Verifica se a mensagem é para este nó e modifica o estado do relê de acordo com o que foi enviado
  if(verifyAndSetRelayState(cmd)) {
    //Mensagem de confirmação que irá ser enviada de volta até o app
    String confirmationMessage = cmd + " OK";
    //Manda mensagem de volta para o nó que enviou a mensagem para que este informe ao app que tudo ocorreu corretamente
    mesh.sendSingle(from, confirmationMessage);
    Serial.println("Changed Relay status for this node: " + confirmationMessage);
  }
}

void loop() {
  //O update é responsável pelas verificações de mensagens recebidas dentre outras tarefas necessárias da rede mesh
  mesh.update();

  //Executa as tarefas que foram adicionadas ao scheduler
  scheduler.execute();
}