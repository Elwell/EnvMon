
/* Outdoor arduino for House water tanks (maxbotix MB7389) and a DHT22 in the studio
 * Publishing to MQTT broker.
 * 
 * Datasheet for MB7389 - https://www.maxbotix.com/documents/HRXL-MaxSonar-WR_Datasheet.pdf
 * Andrew ELwell, Oct 2018 
 */

// Can't use LED_BUILTIN (13) as thats also used by Ethernet SPI (SCK)
// Pin 2 on the ultrasonic rangefinder is PW out
// Pin 4 pulled low suppresses the ranging
const int RangeStart = 5;
const int TankPW = 6;

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

/* #include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN    8
#define DHTTYPE   DHT22

DHT_Unified dht(DHTPIN, DHTTYPE);
*/

// Make sure mac address and IP don't clash on the lan
byte mac[] = { 0xAE, 0x00, 0xFF, 0x00, 0x0A, 0xC9 }; 
IPAddress ip(192, 168, 10, 201);
IPAddress broker(192, 168, 10, 250);
EthernetClient ethClient;
PubSubClient client(broker, 1883, ethClient);

long TankDepth;
const int interval = 10000; // Timing cycle (ms)
unsigned long previousMillis=0;


const int TankHeight = 1850;  // Height of tank (mm) - from Sensor to pipe outlet
const int TankRadius = 1520;  // Radius of tank (mm)
const int TankCount  = 2;     // How many identical tanks are interconnected 
int TankVolRemain = 0;
char strTankDepth[5] ;
char strTankVolRemain[6];
// Volume = pi * r^2 * h
//float t = 0; // temp and humidity from DHHT22
//float h = 0; 
//char strT[5];
//char strH[5];

void setup () {
  delay( 50 ) ; // Allow the ethernet to reset if using etherten
  Serial.begin(115200);
  Serial.println("tank_water_depth (House Tanks)- AE Oct 2018");
  pinMode(TankPW, INPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, LOW);
  pinMode(RangeStart, OUTPUT);
  digitalWrite(RangeStart, LOW);
  Ethernet.begin(mac, ip);
  if (client.connect("TankSensor1","status/housetanksensor",0,1,"offline")) {
    client.publish("status/housetanksensor","online",1);
  }
  //dht.begin();
  //sensor_t sensor;


}

void read_tank(){
  digitalWrite(RangeStart, HIGH);
  TankDepth = pulseIn(TankPW, HIGH);
  digitalWrite(RangeStart, LOW);
  TankVolRemain = int(3.14 * TankRadius * TankRadius * (TankHeight - TankDepth) * TankCount /1000000);
  if (TankDepth <= 250) { // H/W problem - won't range under 30cm!
    Serial.println("WARNING - Suspect tank gauge sensor measurement (Too Small)");
  }
  if (TankDepth >= TankHeight) { // This shouldn't happen either
    Serial.println("WARNING - Suspect tank gauge sensor measurement (> TankHeight)");
  }
}

void printall(){
  Serial.print("Depth Below Sensor: ");
  Serial.print(TankDepth); 
  Serial.println(" mm"); 
  Serial.print("Tank Volume Remaining: ");
  Serial.print(TankVolRemain);
  Serial.println(" l");
}

void puball(){
  dtostrf(TankDepth,0,0,strTankDepth); // pesky pubsubclient doesn't send int
  dtostrf(TankVolRemain,0,0,strTankVolRemain);
  client.publish("sensors/housetanks/depthbelowsensor",strTankDepth);
  client.publish("sensors/housetanks/litres",strTankVolRemain);
  //dtostrf(t,0,2,strT);
  //dtostrf(h,0,2,strH);
  //client.publish("sensors/studio/temperature",strT);
  //client.publish("sensors/studio/hhumidity",strH);
}

void loop () {
  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillis) >= interval) {
      // digitalWrite(LED_BUILTIN, HIGH);
      read_tank();

      /* sensors_event_t event;  
      dht.humidity().getEvent(&event);
      dht.temperature().getEvent(&event);
      */ 
      printall();
      puball();
      previousMillis = currentMillis;
      // digitalWrite(LED_BUILTIN, LOW);
  }
  client.loop();
}
