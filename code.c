#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define BLYNK_TEMPLATE_ID "TMPL3x5ykZQg4"
#define BLYNK_TEMPLATE_NAME "water quality"
#define BLYNK_AUTH_TOKEN "ZiCzeRYSLUdFK1NoBTi0ssVzJFrWy1fF"
int phdata[100],sum=0;
float phavg;
int i;
char auth[] = "ZiCzeRYSLUdFK1NoBTi0ssVzJFrWy1fF";       // You should get Auth Token in the Blynk App.
char ssid[] = "realme 7";                    // Your Wi-Fi Credentials
char pass[] = "9526807738";
#include <Wire.h>

const int turbidityPin = 32; // Change this to the appropriate pin
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

void setup() {
   Blynk.begin(auth,ssid,pass,"blynk.cloud",80);
    sensors.begin();  
  Serial.begin(9600);
pinMode(33,INPUT);
  // Initialize the sensor
  // You may need to add specific initialization steps based on your sensor's datasheet
}

void loop() {
  Blynk.run();
  // Read the analog value from the turbidity sensor
  int turbidityValue = analogRead(turbidityPin);

  // Convert the analog value to turbidity level
  float turbidityLevel = map(turbidityValue, 0, 4095, 0, 100); // Adjust the mapping based on your sensor's characteristics

  // Print the turbidity level
  Serial.print("Turbidity: ");
  Serial.println(turbidityLevel);
  Blynk.virtualWrite(V0,turbidityLevel);
  delay(1000);
  sensors.requestTemperatures(); 

  //print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print((char)176);//shows degrees character
  Serial.print("C  |  ");
  
  //print the temperature in Fahrenheit
  Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  Serial.print((char)176);//shows degrees character
  Serial.println("F");
   Blynk.virtualWrite(V1,sensors.getTempCByIndex(0));
  delay(500);
  for(i=0;i<10;i++)
  {
    phdata[i]=analogRead(33);
    sum+=phdata[i];
  }
  
  phavg=sum/10;
  Serial.println(phavg);
  sum=0;
  delay(500);
  float phacid = map(phavg, 1750, 1770, 1, 5);
     float phbase = map(phavg, 1800, 1890, 8, 14);
     float phneutr = map(phavg, 2019, 2087, 6, 7);
     if(phacid>=1&&phacid<=5)
      {
        Serial.println("ACIDIC");
        Serial.println(phacid);
        Blynk.virtualWrite(V2,"ACIDIC");
      }
      else if(phbase>=8&&phbase<=14)
      {
        Serial.println("base");
        Serial.println(phbase);
         Blynk.virtualWrite(V2,"BASE");
      }
      else
      {
        Serial.println("neutral");
        Serial.println(phneutr);
          Blynk.virtualWrite(V2,"NEUTRAL");
      }
      Serial.println();
