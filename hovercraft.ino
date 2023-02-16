#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "ESP32_Servo.h"

// first motor
int ENA_pin_1 = 14;
int IN1 = 27;
int IN2 = 26;
String slider_value_elevation = "0";

// second motor
int ENA_pin_2 = 18;
int IN3 = 25;
int IN4 = 33;
String slider_value_throttle = "0";

const int frequency = 500;
const int pwm_channel_1 = 0;
const int pwm_channel_2 = 1;
const int resolution = 8;

// servo
Servo servo;
int SERVO_PIN = 13;
String slider_value_steering = "0";

const char *input_parameter = "value";

const char *ssid = "ESP32 Access-Point";
const char *password = "password";

AsyncWebServer server(80);

void setup()
{
  // First motor
  pinMode(ENA_pin_1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  ledcSetup(pwm_channel_1, frequency, resolution);
  ledcAttachPin(ENA_pin_1, pwm_channel_1);
  ledcWrite(pwm_channel_1, slider_value_elevation.toInt());
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  // Second motor
  pinMode(ENA_pin_2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  ledcSetup(pwm_channel_2, frequency, resolution);
  ledcAttachPin(ENA_pin_2, pwm_channel_2);
  ledcWrite(pwm_channel_2, slider_value_throttle.toInt());
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  // Servo
  pinMode(SERVO_PIN, OUTPUT);
  servo.attach(SERVO_PIN);

  // Access-Point creation
  Serial.begin(115200);
  Serial.println("\n[*] Creating AP");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("[+] AP Created with IP Gateway ");
  Serial.println(WiFi.softAPIP());

  // Route to load index.html file
  server.on("/html", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  // Route to load index.js file
  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.js", "text/javascript"); });

  // first motor
  server.on("/slider_elevation", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String message;
    if (request->hasParam(input_parameter)) {
        message = request->getParam(input_parameter)->value();
        slider_value_elevation = message;
        ledcWrite(pwm_channel_1, slider_value_elevation.toInt());
        if (slider_value_elevation.toInt() == 0)
        {
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
        } else{
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
        }
    } else {
      message = "No message sent";
    }
    Serial.println(message);
    request->send(200, "text/plain", "OK"); });

  // second motor
  server.on("/slider_throttle", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String message;
    if (request->hasParam(input_parameter)) {
        message = request->getParam(input_parameter)->value();
        slider_value_throttle = message;
        ledcWrite(pwm_channel_2, slider_value_throttle.toInt());
        if (slider_value_throttle.toInt() == 0)
        {
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
        } else{
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
        }
    } else {
        message = "No message sent";
    }

    Serial.println(message);
    request->send(200, "text/plain", "OK"); });

  // servo
  server.on("/slider_steering", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String message;
    if (request->hasParam(input_parameter)) {
        message = request->getParam(input_parameter)->value();
        slider_value_steering = message;
        servo.write(slider_value_steering.toInt());
    } else {
        message = "No message sent";
    }

    Serial.println(message);
    request->send(200, "text/plain", "OK"); });

  // start the server
  server.begin();
}

void loop() {}
