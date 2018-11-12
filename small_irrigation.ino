/* Dye Garden (front hexagon) irrigation controller
 * Andrew ELwell, Oct 2018 
 */

// Can't use LED_BUILTIN (13) as thats also used by Ethernet SPI (SCK)

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
/* Ethernet shield uses pins:
 *  4 - SS for SD card
 * 10 - SS for ethernet
 * 11 - MOSI
 * 12 - MISO
 * 13 - SCK
 */

 // Testing using 4 channel board, prod is 8
const int Chan1 = 2;
const int Chan2 = 3;
const int Chan3 = 5;
const int Chan4 = 6;
const int Chan5 = 7;
const int Chan6 = 8;
const int Chan7 = 9;
// const int Chan8 = 

// Make sure mac address and IP don't clash on the lan
byte mac[] = { 0xAE, 0x00, 0x01, 0x00, 0x00, 0x03 }; 
IPAddress ip(192, 168, 1, 202);
IPAddress broker(192, 168, 1, 250);
EthernetClient ethClient;
PubSubClient client(broker, 1883, ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
   if (client.connect("HexIrrigation1","status/hexIrrigation",0,1,"offline")) {
       Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("status/hexIrrigation","online",1);

      // ... and resubscribe
      client.subscribe("control/hexIrrigation/+");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  String Payload = "";
  for (int i=0;i<length;i++) {
    Payload += (char)payload[i];
  }
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print(Payload);
  Serial.println();
  String Topic = String(topic).substring(22);
  // OK so we can't use case, so one messy list of if statements coming up..
  if ( Topic == "valve1") {
      if ( Payload == "1" ) {
          digitalWrite(Chan1, LOW);
          client.publish("status/hexIrrigation/valve1", "On");
      } else {
          digitalWrite(Chan1, HIGH);
          client.publish("status/hexIrrigation/valve1", "Off"); 
      }
  }  
   if ( Topic == "valve2") {
      if ( Payload == "1" ) {
          digitalWrite(Chan2, LOW);
          client.publish("status/hexIrrigation/valve2", "On");
      } else {
          digitalWrite(Chan2, HIGH);
          client.publish("status/hexIrrigation/valve2", "Off"); 
      }
   }
 if ( Topic == "valve3") {
      if ( Payload == "1" ) {
          digitalWrite(Chan3, LOW);
          client.publish("status/hexIrrigation/valve3", "On");
      } else {
          digitalWrite(Chan3, HIGH);
          client.publish("status/hexIrrigation/valve3", "Off"); 
      }
 }
 if ( Topic == "valve4") {
      if ( Payload == "1" ) {
          digitalWrite(Chan4, LOW);
          client.publish("status/hexIrrigation/valve4", "On");
      } else {
          digitalWrite(Chan4, HIGH);
          client.publish("status/hexIrrigation/valve4", "Off"); 
      }
 }
 if ( Topic == "valve5") {
      if ( Payload == "1" ) {
          digitalWrite(Chan5, LOW);
          client.publish("status/hexIrrigation/valve5", "On");
      } else {
          digitalWrite(Chan5, HIGH);
          client.publish("status/hexIrrigation/valve5", "Off"); 
      }
 }
 if ( Topic == "valve6") {
      if ( Payload == "1" ) {
          digitalWrite(Chan6, LOW);
          client.publish("status/hexIrrigation/valve6", "On");
      } else {
          digitalWrite(Chan6, HIGH);
          client.publish("status/hexIrrigation/valve6", "Off"); 
      }
 }
 if ( Topic == "valve7") {
      if ( Payload == "1" ) {
          digitalWrite(Chan7, LOW);
          client.publish("status/hexIrrigation/valve7", "On");
      } else {
          digitalWrite(Chan7, HIGH);
          client.publish("status/hexIrrigation/valve7", "Off"); 
      }
 }
 /* if ( Topic == "valve8") {
      if ( Payload == "1" ) {
          digitalWrite(Chan8, LOW);
          client.publish("status/hexIrrigation/valve8", "On");
      } else {
          digitalWrite(Chan8, HIGH);
          client.publish("status/hexIrrigation/valve8", "Off"); 
      }
  } */
}

void setup () {
  // Set up the Relay IO pins
  pinMode(Chan1, OUTPUT);
  pinMode(Chan2, OUTPUT);
  pinMode(Chan3, OUTPUT);
  pinMode(Chan4, OUTPUT);
  pinMode(Chan5, OUTPUT);
  pinMode(Chan6, OUTPUT);
  pinMode(Chan7, OUTPUT);
  //pinMode(Chan8, OUTPUT);
  // Relays come ON when LOW (well, the LED does) so bring them high on init to stay off
  digitalWrite(Chan1, HIGH);
  digitalWrite(Chan2, HIGH);
  digitalWrite(Chan3, HIGH);
  digitalWrite(Chan4, HIGH);
  digitalWrite(Chan5, HIGH);
  digitalWrite(Chan6, HIGH);
  digitalWrite(Chan7, HIGH);
  //digitalWrite(Chan8, HIGH);

  
  delay( 50 ) ; // Allow the ethernet to reset if using etherten
  Serial.begin(115200);
  Serial.println("Hexagon Irrigation controller- AE Oct 2018");
  //pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, LOW);
  Ethernet.begin(mac, ip);
  client.setCallback(callback);
}

void loop () {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
