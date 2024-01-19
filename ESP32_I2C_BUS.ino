//Import All reqired libaries for I2C LCD , BME280 Sensor, MPU6050 sensor
#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

//uncomment the following lines if you're using SPI
/*#include <SPI.h>
  #define BME_SCK 18
  #define BME_MISO 19
  #define BME_MOSI 23
  #define BME_CS 5*/

#include <LiquidCrystal_I2C.h>
// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
float temperature = 0;
float humidity = 0;

const int MPU6050_addr = 0x68; // MPU6050 sensor configured on address 0x68 for I2C
int16_t AccX, AccY, AccZ, Temp, GyroX, GyroY, GyroZ;


void setup() {
  Serial.begin(115200);
  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();
  if (!bme.begin(0x76)) { // BME280 sensor configured on address 0x76 for I2C
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  // drawing commands to make them visible on screen!
  Wire.begin();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);


}
void loop() {
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("MICRODIGISOFT");
  // set cursor to first column, second row
  lcd.setCursor(0, 1);
  // Temperature in Celsius
  temperature = bme.readTemperature();
  // Uncomment the next line to set temperature in Fahrenheit
  // (and comment the previous temperature line)
  //temperature = 1.8 * bme.readTemperature() + 32; // Temperature in Fahrenheit
  // Convert the value to a char array
  char tempString[8];
  dtostrf(temperature, 1, 2, tempString);
  Serial.print("Temperature: ");
  Serial.println(tempString);
  lcd.print("BME-TEMP=");
  lcd.println(tempString);
  humidity = bme.readHumidity();
  delay(1000);
  // clears the display to print new message
  lcd.clear();
  lcd.setCursor(0, 0);
  // Convert the value to a char array
  char humString[8];
  dtostrf(humidity, 1, 2, humString);
  Serial.print("Humidity: ");
  Serial.println(humString);
  lcd.print("BME-HUM=");
  lcd.println(humString);
  delay(2000);
  lcd.clear();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr, 14, true);
  AccX = Wire.read() << 8 | Wire.read();
  AccY = Wire.read() << 8 | Wire.read();
  AccZ = Wire.read() << 8 | Wire.read();
  Temp = Wire.read() << 8 | Wire.read();
  GyroX = Wire.read() << 8 | Wire.read();
  GyroY = Wire.read() << 8 | Wire.read();
  GyroZ = Wire.read() << 8 | Wire.read();
  lcd.setCursor(0, 0);
  lcd.println("MPU WITH OLED");
  lcd.setCursor(0, 1);
  lcd.print("Acce-meter-m/s^2");
  lcd.print(AccX);
  lcd.print(", ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(AccY);
  lcd.print(", ");
  lcd.print(AccZ);
  lcd.print("");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Gyroscope-rps");
  lcd.setCursor(0, 1);
  lcd.print(GyroX, 1);
  lcd.print(", ");
  lcd.print(GyroY);
  lcd.print(", ");
  lcd.print(GyroZ);
  lcd.println("");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MPU Temp=");
  lcd.print(Temp / 340.00 + 36.53);
  delay(2000);
  // clears the display to print new message
  lcd.clear();
}
