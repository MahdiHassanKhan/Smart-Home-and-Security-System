
#include "CTBot.h"

CTBot myBot;
const int irPin = D2;
const int buzzerPin = D3;
const int pirPin = D4;
const int smokePin = D5;

int pirValue = 0;

int calibrationTime = 30;
long unsigned int lowIn;
long unsigned int pause = 5000;

boolean lockLow = true;
boolean takeLowTime;


String ssid = "BIG BANG...........";     
String pass = "@l!m@l!m!!"; 
String token = "2011068568:AAHR60CQqW3Y2jn3WB-pIsK0HFq-EU6ekts";  
uint8_t greenLed = D0;           
uint8_t redLed = D1;                        
// (replace 2 with BUILTIN_LED)


float smokeValue;
void setup() {
  // initialize the Serial
  Serial.begin(115200);
  Serial.println("Starting TelegramBot...");

  // connect the ESP8266 to the desired access point
  myBot.wifiConnect(ssid, pass);

  // set the telegram bot token
  myBot.setTelegramToken(token);

  // check if all things are ok
  if (myBot.testConnection())
    Serial.println("\ntestConnection OK");
  else
    Serial.println("\ntestConnection NOK");

  // set the pin connected to the LED to act as output pin
  pinMode(redLed, OUTPUT);

  pinMode(greenLed, OUTPUT);
  digitalWrite(redLed, LOW); // turn off the led (inverted logic!)
  digitalWrite(greenLed, LOW);

  pinMode(irPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);
  pinMode(smokePin, INPUT);
}

void loop() {
  // a variable to store telegram message data
  TBMessage msg;
  int irValue = digitalRead(irPin);
  smokeValue = analogRead(smokePin);
  pirValue = digitalRead(pirPin);

  Serial.println("irValue: " + String(irValue));
  Serial.println("PirValue: " + String(pirValue));

  if (CTBotMessageText == myBot.getNewMessage(msg)) {

    if (msg.text.equalsIgnoreCase("RED ON")) {             
      digitalWrite(redLed, HIGH);                              
      myBot.sendMessage(msg.sender.id, "RED Light is now ON"); 
    }
    else if (msg.text.equalsIgnoreCase("RED OFF")) {      
      digitalWrite(redLed, LOW);                              
      myBot.sendMessage(msg.sender.id, "RED Light is now OFF"); 
    } else if (msg.text.equalsIgnoreCase("GREEN ON")) {      
      digitalWrite(greenLed, HIGH);                              
      myBot.sendMessage(msg.sender.id, "GREEN Light is now ON");
    } else if (msg.text.equalsIgnoreCase("GREEN OFF")) {      
      digitalWrite(greenLed, LOW);                          
      myBot.sendMessage(msg.sender.id, "GREEN Light is now OFF"); 
    }
    else {                                                 
      // generate the message for the sender
      String reply;
      reply = (String)"Welcome " + msg.sender.username + (String)". Try LIGHT ON or LIGHT OFF.";
      myBot.sendMessage(msg.sender.id, reply);             
    }
  }

  if (irValue == 0)
  {
    digitalWrite(buzzerPin, HIGH);
    myBot.sendMessage(msg.sender.id, "Someone break into your house BY IR."); 

  } else
  {
    digitalWrite(buzzerPin, LOW);
  }



  if ( pirValue == HIGH) {
    tone(buzzerPin, 500);
    if (lockLow) {

      lockLow = false;
      Serial.println("---");
      Serial.print("motion detected at ");
      Serial.print(millis() / 1000);
      Serial.println(" sec");
      myBot.sendMessage(msg.sender.id, "Someone break into your house BY PIR."); // notify the sender
    }
    takeLowTime = true;
  }

  if (digitalRead(pirPin) == LOW) {
    noTone(buzzerPin);
    if (takeLowTime) {
      lowIn = millis();        
      takeLowTime = false;      
    }

    if (!lockLow && millis() - lowIn > pause) {
      lockLow = true;
      Serial.print("motion ended at ");      //output
      Serial.print((millis() - pause) / 1000);
      Serial.println(" sec");

    }
  }
  delay(1000);
}
