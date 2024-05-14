#define BLYNK_TEMPLATE_ID "TMPL69z9nXewZ"
#define BLYNK_TEMPLATE_NAME "doan"
#define BLYNK_AUTH_TOKEN "0kaIFXHgkLRZPK6f32MR6fFCUf9S-OLv"
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
//Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 4);
DHT dht(D2, DHT11); //(sensor pin,sensor type)

char auth[] = "0kaIFXHgkLRZPK6f32MR6fFCUf9S-OLv";//Enter your Auth token
char ssid[] = "PHP";//Enter your WIFI name
char pass[] = "22222222";//Enter your WIFI password

BlynkTimer timer;
bool Relay = 0;
bool Fan = 0;
//Define component pins
#define sensor A0
#define waterPump D3
#define fan D1

void setup() {
  Serial.begin(9600);
  Wire.begin(12,14);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, LOW);
  pinMode(fan, OUTPUT);
  digitalWrite(fan, LOW);
  lcd.init();
  lcd.backlight();

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  lcd.clear();

  //Call the function
  timer.setInterval(100L, soilMoistureSensor);
  timer.setInterval(100L, sendSensor);
}

//Get the button value
BLYNK_WRITE(V1) {
  Relay = param.asInt();

}
BLYNK_WRITE(V4) {
  Fan = param.asInt();

}

//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V0, value);
  lcd.setCursor(0, 0);
  lcd.print("Moisture :");
  lcd.print(value);
  lcd.print(" ");
  if(value < 30)
  {
    Blynk.logEvent("soil_moisture_alert");
  }
}

///humity and temprature
void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  lcd.setCursor(0, 1);
  lcd.print("Temp : ");
  lcd.print(t);
  lcd.setCursor(-4, 2);
  lcd.print("Humi : ");
  lcd.print(h);

  Blynk.virtualWrite(V2, t);
  Blynk.virtualWrite(V3, h);
  if (t < 18 || t > 39)
  {
      Blynk.logEvent("temperature_alert");
  }

}

void loop() {

  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
    if (Fan == 0) {
    digitalWrite(fan, LOW);
    lcd.setCursor(5, 3);
    lcd.print("Fan OFF ");
  } else {
    digitalWrite(fan, HIGH);
    lcd.setCursor(5, 3);
    lcd.print("Fan ONN");
  }
    if (Relay == 0) {
    digitalWrite(waterPump, LOW);
    lcd.setCursor(-4, 3);
    lcd.print("Pump OFF ");
  } else {
    digitalWrite(waterPump, HIGH);
    lcd.setCursor(-4, 3);
    lcd.print("Pump ONN");
  }
}
