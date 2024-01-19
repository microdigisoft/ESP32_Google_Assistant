#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>

#define FIREBASE_Host "https://esp32-control-gpio-default-rtdb.firebaseio.com/"                   // replace with your Firebase Host
#define FIREBASE_authorization_key "HMAY6JnncExxxxxxxxxxxxxxxxxnGOqsrLzDJdU" // replace with your secret key
#define Your_SSID "xxxxxxxxx"// replace with your SSID
#define Your_PASSWORD "xxxxxxxxx"//replace with your Password
String led_state = "";// LED State
int led_gpio = 2;
void setup() {

  Serial.begin(115200);
  delay(1000);
  pinMode(2, OUTPUT);
  WiFi.begin(Your_SSID, Your_PASSWORD);
  Serial.print("Connecting to WIFI");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected to WIFI!");
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());//print local IP address
  Firebase.begin(FIREBASE_Host, FIREBASE_authorization_key);// connect to firebase
  Firebase.setString("ESP32-GPIO2: ", "OFF");//send initial string of led status

}

void loop() {

  led_state = Firebase.getString("ESP32-GPIO2: ");
  if (led_state == "ON") {

    Serial.println("ESP32-GPIO2 is ON");
    digitalWrite(led_gpio, HIGH);

  }

  else if (led_state == "OFF") {

    Serial.println("ESP32-GPIO2 is OFF");
    digitalWrite(led_gpio, LOW);

  }

  else {
    Serial.println("Wrong value entered! Only ON or OFF accepted");

  }

}
