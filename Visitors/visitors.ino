
//Include the library files
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "DTs_Ryodc1htFTUv1npNRLl1F1RRosX3";//Enter your Auth token
char ssid[] = "Chetan's Mob";//Enter your WIFI name
char pass[] = "00222883";//Enter your WIFI password
 
#define inSensor D5
#define outSensor D6
#define relay1 D7
int inStatus;
int outStatus;
 
int countin = 0;
int countout = 0;
 
int in;
int out;
int now;

BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, HIGH);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  lcd.setCursor(0, 0);
  lcd.print("Home Automation");
  lcd.setCursor(4, 1);
  lcd.print("System");
  delay(4000);
  lcd.clear();

//Call the functions
  timer.setInterval(100L, lmsensor);
}

//Get the DHT11 sensor values
void lmsensor() {
  inStatus =  digitalRead(inSensor);
  outStatus = digitalRead(outSensor);
  if (inStatus == 0)
  {
    in = countin++;
  }
 
  if (outStatus == 0)
  {
    out = countout++;
  }
 
  now = in - out;
 
  Serial.println(now);
  if (now <= 0)
  {
    digitalWrite(relay1, HIGH);
  }
  else
  {
    digitalWrite(relay1, LOW); 
  }
  Blynk.virtualWrite(V1, in); // Visitors In
  Blynk.virtualWrite(V2, out); // Visitors Out
  Blynk.virtualWrite(V3, now); // Current Visitors
}


void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}
