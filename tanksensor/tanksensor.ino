/*  Arduino + Maxbotix MB7389 sensor to detect water tank levels
 *  Amdrew Elwell <Andrew.Elwell@gmail.com> - 2016-08-01
 *  Licenced under GPL 2+
 */


const int THEIGHT = 2000 ;   // Height im MM to sensor zero
const int TRADIUS = 1050 ;    // Radius of water tank (mm)
const int NTANKS = 1 ;       // if there are multiple IDENTICAL tanks plumbed together

const int SENS_PIN=3;
const int RANGE_PIN=13;
long mm;

void setup() {
  Serial.begin(9600);
  pinMode(RANGE_PIN,OUTPUT);
  pinMode(SENS_PIN,INPUT);
  digitalWrite(RANGE_PIN,LOW);
}

void read_sensor(){
  digitalWrite(RANGE_PIN,HIGH);
  for (int i=0 ; i < 5 ; i++) {
      mm = pulseIn(SENS_PIN, HIGH);
      Serial.print(mm);
      Serial.print(" ");
  }
  Serial.println();
  digitalWrite(RANGE_PIN,LOW);
}

void printvol(){
  int level = THEIGHT - mm ;
  Serial.println(level);
  float volume = (3.14 * TRADIUS * TRADIUS * level)/100000 ; // should be remaining capacity in l
  Serial.println(volume);
}


void loop() {
  read_sensor();
  Serial.println(mm);
  printvol();
  delay(3000); //make this match the refresh rate of the sensor
}
