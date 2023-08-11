
//Include the library files
// #include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
 
WidgetLCD lcd(V8);

// LiquidCrystal_I2C lcd(0x27, 16, 2);
char auth[] = "Cu1O4xYs5fQth2cjYwTW8CfBpQ6mTWOE";//Enter your Auth token
char ssid[] = "Chetan's Mob";//Enter your WIFI name
char pass[] = "00222883";//Enter your WIFI password
 

BlynkTimer timer;


#define Buzzer D0
#define LED D6
#define MQ2 A0
#define trig D4
#define echo D5
#define relay1 D7
#define relay2 D8

void setup() {
  Serial.begin(9600);
 // lcd.init();
 // lcd.backlight();
  pinMode(Buzzer, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(Buzzer, LOW);
  digitalWrite(LED, LOW);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  // lcd.setCursor(0, 0);
  lcd.print(0,0,"Home Automation");
  //lcd.setCursor(4, 1);
  lcd.print(0,1,"System");
  delay(4000);
  lcd.clear();

//Call the functions
  timer.setInterval(100L, gassensor);
  timer.setInterval(100L, ultrasonic);
}

//Get the MQ2 sensor values
void gassensor() {
  int value = analogRead(MQ2);
  Serial.println(value);
  value = map(value, 0, 1024, 0, 100);
  if (value <= 55) {
    digitalWrite(Buzzer, LOW);
    lcd.print(0,0,"Safe Zone ⛑");
  } else if (value > 55) {
    lcd.print(0,0,"warning, Danger🧯");
    Blynk.notify("Warning! Gas leak detected");
    digitalWrite(Buzzer, HIGH);
  }
  Blynk.virtualWrite(V1, value);
  // lcd.setCursor(0, 0);
  // lcd.print("G:");
  // lcd.print(" ");
  // lcd.print(value);

}

//Get the ultrasonic sensor values
void ultrasonic() {
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH);
  long cm = t / 29 / 2;
  Blynk.virtualWrite(V4, cm);
  if(cm<40){
    lcd.print(0,1,"Motion: ");
    lcd.print(10,1,"YES 🚷");
  }
  else{
    lcd.print(0,1,"Motion: ");
    lcd.print(10,1,"NO ⛔️");
  }
  

  // lcd.setCursor(8, 1);
  // lcd.print("W:");
  // lcd.print(cm);
  // lcd.print("  ");
}

BLYNK_WRITE(V0) {
  bool pirbutton = param.asInt();
  if (pirbutton == 1) {
    digitalWrite(Buzzer, HIGH);
  } else {
    digitalWrite(Buzzer, LOW);
  }
}

BLYNK_WRITE(V3) {
  bool LEDbutton = param.asInt();
  if (LEDbutton == 1) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}

//Get buttons values
BLYNK_WRITE(V5) {
 bool RelayOne = param.asInt();
  if (RelayOne == 1) {
    digitalWrite(relay1, HIGH);
  } else {
    digitalWrite(relay1, LOW);
  }
}

//Get buttons values
BLYNK_WRITE(V6) {
 bool RelayTwo = param.asInt();
  if (RelayTwo == 1) {
    digitalWrite(relay2, HIGH);
  } else {
    digitalWrite(relay2, LOW);
  }
}

void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}
