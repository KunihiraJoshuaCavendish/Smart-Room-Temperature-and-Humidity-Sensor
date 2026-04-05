#include <LiquidCrystal.h> //tell arduino u are using a 16x2 lcd in parallel mode

// LCD pins RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define TEMP_PIN A1
#define HUMIDITY_PIN A0
#define FAN_PIN 9

bool fanState = false; //remeber whether the fan is currently on or off so it dont flicker

void setup() {
  lcd.begin(16, 2); //lcd dimensions rxc
  pinMode(FAN_PIN, OUTPUT); //fan pin as output pin
  
  //welcome message
  lcd.setCursor(0, 0);
  lcd.print("Smart Room T&H");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(3000);

  lcd.clear();
  delay(1000);
}

void loop() {

  
  //TEMPERATURE (TMP36) converts analog voltage from sensor into a temp in celcius
  int tempRaw = analogRead(TEMP_PIN); //reads voltage as 0-1023
  float voltage = tempRaw * (5.0 / 1023.0);//converts adc value t volts
  float tempC = (voltage - 0.5) * 100; //TMP36 formula: 0.5V = 0°C, every 0.01V = 1°C

    // HUMIDITY (Potentiometer)
  int humRaw = analogRead(HUMIDITY_PIN); //reads voltage from potentiometer
  int humidity = map(humRaw, 0, 1023, 0, 100); //converts 0-1023 into 0-100%

 // FAN speeds
  int tempSpeed = 0;
  int humSpeed = 0;

  // Only start after 30°C
  if (tempC >= 30) {
    tempSpeed = map(tempC, 30, 45, 100, 255);
  }

  // Only start after 70% humidity
  if (humidity >= 70) {
    humSpeed = map(humidity, 70, 100, 100, 255);
  }

  // Take strongest effect
  int fanSpeed = max(tempSpeed, humSpeed);

  // Safety limits
  fanSpeed = constrain(fanSpeed, 0, 255);

  // Apply to motor
  analogWrite(FAN_PIN, fanSpeed);

  // LCD DISPLAY 
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(tempC, 1);
  lcd.print((char)223);
  lcd.print("C   ");

  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humidity);
  lcd.print("% ");

  lcd.print("F:");
  lcd.print(fanSpeed);
  lcd.print("   ");

  delay(500);
}