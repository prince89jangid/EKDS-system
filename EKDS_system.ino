/*=========================== SDA = A4  SCL = A5  Buzzer = D8  RED_led = D10  White_led = D9 ==========================*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define buzzer 8
#define while_led 9
#define red_led 10

/* =========================================================================================================================*/

int sensor_min = 0;
int sensor_max = 1023;
int delta = 10;  // diffence bw two pressue
int Dt = 100;     // 20ms
int detection_delay = 1000;
int threshold = 50;
/* =========================================================================================================================*/


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

void alarm1() {
  for (int i = 0; i < 3; i++) {

    if (cal_values() <= threshold) {
      return;
    }
    digitalWrite(buzzer, HIGH);
    digitalWrite(red_led,HIGH);
    delay(100);
    digitalWrite(buzzer , LOW); 
    digitalWrite(red_led , LOW); 
    delay(100);
  }
  return;
}

void alarm2() {
  for (int i = 0; i < 25; i++) {
    Serial.println(cal_values());

    digitalWrite(buzzer, HIGH);
    digitalWrite(red_led, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    digitalWrite(red_led, LOW);
    delay(50);
  }
}

int cal_values() {
  int val = analogRead(A0);
  val = map(val, sensor_min, sensor_max, 0, 100);
  return val;
}

bool isKick(int pc, int po) {
  int Dp = pc - po;
  if (Dp >= delta)

  {
    return true;
  }
  return false;
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



  int curr_pres = cal_values();

  showText((String)curr_pres + " %");

  int temp = map(curr_pres, 0, 100, 0, 255);
  analogWrite(while_led, temp);

  Serial.println(curr_pres);

  if (curr_pres > threshold) {
    showText("High      Pressure");
    alarm1();
  }

  curr_pres = cal_values();
  Serial.println(curr_pres);

  delay(Dt);

  Serial.println(cal_values());

  int old_pres = cal_values();

  if (isKick(curr_pres, old_pres) == true) {
    digitalWrite(red_led, HIGH);
    showText("KICK       FOUND");
    alarm2();
    delay(detection_delay);

  } else {
    {
      digitalWrite(red_led, LOW);
    }
  }
}
