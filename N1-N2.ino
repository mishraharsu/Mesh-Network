#include "painlessMesh.h"

#define   MESH_PREFIX     "Harsh"
#define   MESH_PASSWORD   "GauravMishra"
#define   MESH_PORT       5555

Scheduler userScheduler; 
painlessMesh  mesh;

const uint32_t node1ID = 3202988893;
const uint32_t node2ID = 3203009205;
const uint32_t node3ID = 692800616;

void sendMessage();

Task taskSendMessage( TASK_SECOND * 2 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg;
  uint32_t myNodeId = mesh.getNodeId();
  
  
  if (myNodeId == node1ID) {
    msg = "Data from Node 1";
    mesh.sendSingle(node3ID, msg); // Send to Node 3
  } else if (myNodeId == node2ID) {
    msg = "Data from Node 2";
    mesh.sendSingle(node1ID, msg); // Send to Node 1
  } else if (myNodeId == node3ID) {
    msg = "Data from Node 3";
    mesh.sendSingle(node2ID, msg); // Send to Node 2
  }

  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void setup() {
  Serial.begin(115200);

  mesh.setDebugMsgTypes( ERROR | STARTUP );

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}
   
void loop() {
  mesh.update();
}
