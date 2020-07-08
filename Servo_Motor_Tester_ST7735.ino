/*
Servo Motor Tester
By DrakerDG (c)

Based on course example (In Spanish) Week 4: Electrones en Acción: Electrónica y Arduinos para tus propios Inventos.

Reference: https://www.coursera.org/learn/electrones-en-accion

https://www.youtube.com/user/DrakerDG
*/

#include <SPI.h>
#include <TFT_ST7735.h>
#include <Servo.h>

#define BLUE    0xF800
#define RED     0x001F
#define GREEN   0x07E0
#define CYAN    0xFFE0
#define MAGENTA 0xF81F
#define YELLOW  0x07FF

#define DC 9
#define RS 10
#define CS 11
#define pinS 3
#define pinA A0
#define pinB A1
#define LED_R 2
#define LED_S LED_BUILTIN

int valA;
int valB;
int valD = 90;
bool Stop = true;
Servo servo1;

TFT_ST7735 tft = TFT_ST7735(CS, DC, RS);

void Servo_Ctrl(void);
void Draw_Table(void);
void Print_Data(void);

void setup(){
  tft.begin();
  tft.setRotation(1);
  tft.clearScreen();
  tft.setTextWrap(true);
  tft.setTextColor(YELLOW, BLACK);
  tft.setCursor(0, 0);
  pinMode(pinS, OUTPUT);
  pinMode(pinB, INPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_S, OUTPUT);
  digitalWrite(LED_S, Stop);
  servo1.attach(pinS);
  Draw_Table();
}

void loop() {
  Servo_Ctrl();
  Print_Data();
  delay(15);
}

void Servo_Ctrl(){
  valB = digitalRead(pinB);
  if(valB==LOW){
    Stop = !Stop;
    digitalWrite(LED_R, !Stop);
    digitalWrite(LED_S, Stop);
    valD = 90;
    delay(250);
  }
  tft.setCursor(60, 100);
  if (Stop) tft.print("|-Stopped-|");
  else{
    tft.print("<-Running->");
    valA = analogRead(pinA);
    valD = map(valA, 0, 1023, 0, 180);
  }
  servo1.write(valD);
}

void Draw_Table(){
  tft.drawFastVLine(22, 0, 128, WHITE);
  for ( int i=0; i<11; i+=1 ){
    tft.drawFastHLine( 20, 5+i*12, 4, WHITE);
    if (!(i&1)){
      tft.setCursor( 0, i*12 + 2);
      tft.print((10-i)*180/10, 1);
    }
  }
  tft.drawFastHLine(20, 125, 128, WHITE);
  tft.setCursor(60, 5);
  tft.print("Servo Motor");
  tft.setCursor(60, 25);
  tft.print("Tester V1.1");
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(116, 43);
  tft.setTextSize(2);
  tft.print("o");
  tft.setTextSize(1);
  tft.setCursor(105, 80);
  tft.print("ms");
}

void Print_Data(){
  char strV[5];
  char strT[7];
  dtostrf(valD, 3, 0, strV);
  tft.setCursor(60, 45);
  tft.setTextSize(3);
  tft.print(strV);
  tft.setTextSize(1);
  float ms = 1.00 + valD*1.00/180;
  dtostrf(ms, 3, 2, strT);
  tft.setCursor(75, 80);
  tft.print(strT);
  int valG = map(valD, 0, 180, 0, 120);
  tft.fillRect(30, 5, 20, 120-valG, BLACK);
  tft.fillRect(30, 125-valG, 20, valG, GREEN);
}