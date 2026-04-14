/*=========================== SDA = A4  SCL = A5  Buzzer = D8  RED_led = D10  White_led = D9 ==========================*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define buzzer  8
#define while_led  9
#define red_led  10

int sensor_min = 0;
int sensor_max = 1023;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===== For display  =====
void showText(String msg) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30, 25);

  display.print(msg);
  display.display();
}

void setup() {

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
}

void loop() {

  int val = analogRead(A0);
  val = map(val, sensor_min, sensor_max, 0, 100);

  Serial.println(val);

  showText((String)val + " %");
  int temp = map(val, 0, 100, 0, 255);
  analogWrite(while_led, temp);

  if (val > 50) {
    digitalWrite(red_led, HIGH);
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(red_led, LOW);
    digitalWrite(buzzer, LOW);
  }
}
