// Spedometer coverter

#include <ESP8266WiFi.h> //Библиотека для работы с WIFI 
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h> // Библиотека для OTA-прошивки
#include <Tachometer.h>
Tachometer tacho;
const byte        interruptPin = 12; // Or other pins that support an interrupt
volatile boolean  interrupt_occurred = false;
boolean  ledstate = false;
uint32_t tmr;
uint16_t SPD , SPDtemp, dif;
#define           Output_pin 14 
#define           LED_pin1 2 // buildin Led

const char* ssid = "GARAGE WIFI"; //Name AP WIFI
const char* password = "23217232"; //Password AP WIFI

void setup() {

  pinMode(LED_pin1, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() == WL_CONNECTED)  digitalWrite(LED_pin1, ledstate);

  //************************
  ArduinoOTA.setHostname("ESP8266-00001"); // Задаем имя сетевого порта
  //ArduinoOTA.setPassword((const char *)"0000"); // Задаем пароль доступа для удаленной прошивки
  ArduinoOTA.begin(); // Инициализируем OTA
  //***********************

  pinMode(Output_pin, OUTPUT);
  digitalWrite(Output_pin, LOW);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ISR, FALLING); //Respond to falling edges on the pin
  
  tacho.setWindow(10);    // установка количества тиков для счёта времени (по умолч 10)20
  tacho.setTimeout(1000); // таймаут прерываний (мс), после которого считается что вращение прекратилось

}

void loop() {
  ArduinoOTA.handle(); // Всегда готов к прошивке

  if (millis() - tmr > 200)  //300
  {
    tmr = millis();
    digitalWrite(LED_pin1, ledstate);
    ledstate = !ledstate;
    SPDtemp = tacho.getHz() / 11.6; //11.8/12
    //if (SPDtemp <= 5) SPDtemp = 0;  //
    if (SPDtemp >= 130) SPDtemp = 130;
    //dif = SPDtemp - SPD;
    //if (abs(dif) > 20  )   SPDtemp = SPD;
    //SPD = SPDtemp;
    tone(Output_pin, SPDtemp );
  }
}

ICACHE_RAM_ATTR void ISR ()
{
  tacho.tick();
}
