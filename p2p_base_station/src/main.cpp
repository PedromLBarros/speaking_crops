#include <Arduino.h>

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP  3600

#define BAND 866E6  //868 MHz

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST); // Display Object

//Functions Declaration
void die (void);

void setup() {
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    die(); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Speaking Crops Sender");
  display.display();

  Serial.begin(115200);   //initialize Serial Monitor
  Serial.println("Speaking Crops Sender");

  SPI.begin(SCK, MISO, MOSI, SS);                 //set SPI pins
  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);    //set LoRa pins
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    die();
  }

  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void die(void){
  noInterrupts();
  Serial.flush();
  esp_deep_sleep_start();

  while (true){
    esp_deep_sleep_start();
  }
}
