// Reed Switch to MQTT publisher for home
// Andrew Elwell, May 2015

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#include <dht.h>
#define DHTPIN 8     // what pin we're connected to
dht DHT;
char message_buff[100];

unsigned long startTime = millis ();
unsigned long interval = 5000;

byte mac[]    = { 0x00, 0x04, 0xA3, 0x2C, 0x1D, 0x01 };  // This rev of the etherten doesn't have SPI mac address chip
byte server[] = { 10, 1, 1, 251 };

// 'open' and 'closed' defined as bytestrings
uint8_t op[] = "open";
uint8_t cl[] = "closed";

EthernetClient ethClient;
PubSubClient client(server, 1883, 0, ethClient);

int gate= 2; // Blue -> Green pair
int gateState = 0;
int gatelast = 0;

int east = 5;
int eastState = 0;
int eastlast = 0;

int west = 3;
int westState = 0;
int westlast = 0;

int door = 6;
int doorState = 0;
int doorlast = 0;

int rear = 7;
int rearState = 0;
int rearlast = 0;


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ardushield1","status/ardushield1",0,true,"OFFLINE")) {
      client.publish("status/ardushield1", "ONLINE");
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() { 
 Serial.begin(115200);
 Serial.println("Arduino Shield 1- Home sensors");
 Serial.print("DHT LIBRARY VERSION: ");
 Serial.println(DHT_LIB_VERSION);
  
 delay( 50 );   // allow some time (50 ms) after powerup and sketch start, for the Wiznet W5100 Reset IC to release and come out of reset.
 
if (Ethernet.begin(mac) == 0)  {
//      Serial.println("Failed to configure Ethernet using DHCP");
      return;
  }
 
// pinMode(led, OUTPUT);
 pinMode(gate, INPUT_PULLUP);           // set pin to input
 pinMode(east, INPUT_PULLUP);
 pinMode(west, INPUT_PULLUP);
 pinMode(door, INPUT_PULLUP);
 pinMode(rear, INPUT_PULLUP);
 
 pinMode(DHTPIN, INPUT_PULLUP);

}

// the loop routine runs over and over again forever:
void loop() {
  if (!client.connected()) {
    reconnect();
  }


 gateState = digitalRead(gate);
  if (gateState != gatelast) {
    if (gateState == HIGH) {
      client.publish("sensors/perimeter/gate", op, 4, 1);
    }  else {
      client.publish("sensors/perimeter/gate", cl, 6, 1);
    }
  }
  gatelast = gateState ;
 
  eastState = digitalRead(east);
  if (eastState != eastlast) {
    if (eastState == HIGH) {     
      client.publish("sensors/perimeter/east_roller", op, 4, 1);
    } else {
      client.publish("sensors/perimeter/east_roller", cl, 6, 1);
    }
  }
  eastlast = eastState ;

  westState = digitalRead(west);
  if (westState != westlast) {
    if (westState == HIGH) {     
      client.publish("sensors/perimeter/west_roller", op, 4, 1);
    }  else {
      client.publish("sensors/perimeter/west_roller", cl, 6, 1);
    }
  }
  westlast = westState ;

  doorState = digitalRead(door);
  if (doorState != doorlast) {
    if (doorState == HIGH) {     
      client.publish("sensors/perimeter/garage_door", op, 4, 1);
    }  else {
      client.publish("sensors/perimeter/garage_door", cl, 6, 1);
    }
  }
  doorlast = doorState ;

  rearState = digitalRead(rear);
  if (rearState != rearlast) {
    if (rearState == HIGH) {     
      client.publish("sensors/perimeter/rear_roller", op, 4, 1);
    }  else {
      client.publish("sensors/perimeter/rear_roller", cl, 6, 1);
    }
  }
  rearlast = rearState ;


if ((unsigned long)(millis() - startTime) >= interval) {
  startTime = millis();
  int chk = DHT.read22(DHTPIN);
  switch (chk)
  {
    case DHTLIB_OK:  
    Serial.print("OK,\t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error,\t"); 
    break;
    default: 
    Serial.print("Unknown error,\t"); 
    break;
  }
  // DISPLAY DATA
  String json = "{ \"temp\": " + String(DHT.temperature) + ", \"humidity\": " + String(DHT.humidity) + " }";
  json.toCharArray(message_buff, json.length()+1);
  client.publish("sensors/garage/json", message_buff);
  }  
  
  client.loop();
}
