//import all required libraries
#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#endif

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
const char* ssid = "xxxxxxxxxx";
const char* password = "xxxxxxxx";
Adafruit_BME280 bme;  // I2C
//Adafruit_BME280 bme(BME_CS);  // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);  // software SPI
String SensorName = "BME280";
String sensorLocation = "ESP32";
#define RETRY_LIMIT  20
int rtl = RETRY_LIMIT;
void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address");
  Serial.println(WiFi.localIP());
  // (you can also pass in a Wire library object like &Wire2)
  bool status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring or change I2C address!");
    while (1);
  }
}
void loop()
{
  float h = bme.readHumidity();
  float t = bme.readTemperature();
  float p = (bme.readPressure() / 100.0F);
  delay(500);
  while (isnan(t) || isnan(h)) {
    Serial.println("Check sensor again - " + rtl);
    h = bme.readHumidity();
    t = bme.readTemperature();
    delay(500);
    if (--rtl < 1) {
      ESP.restart(); // At times the BME280 just kept returning NAN. A Restart fixed this.
    }
  }
  //Open a connection to the server
  HTTPClient http;
  http.begin("http://192.168.29.183/ESP32/writedata.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  //format your POST request.
  //SensorName,location,Tempraure,Humidity,Pressure
  int httpResponseCode = http.POST("&SensorName=" + SensorName
                                   + "&location=" + sensorLocation + "&Temperature=" + String(t)
                                   + "&Humidity=" + String(h) + "&Pressure=" + String(p) + "");

  if (httpResponseCode > 0) {
    //check for a return code - This is more for debugging.
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  }
  else {
    Serial.print("Error on sending post");
    Serial.println(httpResponseCode);
  }
  //closde the HTTP request.
  http.end();
  //Monitor values in console for debugging.
  Serial.println("Temp = " + String(t));
  Serial.println("humidity = " + String(h));
  Serial.println("Pressure = " + String(p));
  //wait 1 minutes for next reading
  delay(10000);
}
