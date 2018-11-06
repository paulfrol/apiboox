#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
#include <IRremote.h>

#include "StringSplitter.h"

int RECV_PIN = 4;                                                                   // Пин с IR-приемником
IRrecv irrecv(RECV_PIN);
decode_results results;

Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  Serial.begin(9600);
  //  Настройки дисплея
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);                                        // initialize with the I2C addr 0x3C (for the 0.96" 128X64 OLED LCD Display)I2C АДРЕС.
  display.setTextColor(WHITE);                                                      // Цвет текста.                                                             
  pinMode(2, OUTPUT);                                                               // устанавливаем 2 ногу как выход 
  irrecv.enableIRIn();                                                              // Start the receiver
  pinMode(3, OUTPUT);
  digitalWrite(3,LOW);
  delay(1000);
  display.clearDisplay();
}

char rx_byte = 0;

void loop() {
  if (Serial.available() > 0) {                                                     // is a character available?
        String rx_string = Serial.readString();                                     // get the character

        StringSplitter *splitter = new StringSplitter(rx_string, ':', 5);           // new StringSplitter(string_to_split, delimiter, limit)
        String display_text = splitter->getItemAtIndex(0);
        String display_x = splitter->getItemAtIndex(1);
        String display_y = splitter->getItemAtIndex(2);
        String display_size = splitter->getItemAtIndex(3);
        String display_scroll = splitter->getItemAtIndex(4);
        delete StringSplitter;

        if(display_scroll == "true"){
          display.startscrollleft(0x00, 0x0F);          
        }
        if(display_scroll == "false"){
          display.stopscroll();        
        }

        display.setTextSize(display_size.toInt());                                  // Размер текста (1).
        display.setCursor(display_x.toInt(),display_y.toInt());                     // Устанавливаем курсор в колонку 0, строку 0. на самом деле это строка №1, т.к. нумерация начинается с 0.
                   
        if(display_text == "light"){
          digitalWrite(2, HIGH);        
        } else if(display_text == "unlight"){
          digitalWrite(2, LOW);      
        } else if(display_text == "blink"){
            blink_(display_x.toInt(),display_y.toInt());
        } else if(display_text == "agly_dsp"){
          display.clearDisplay();
          display.write(1);
        } else if(display_text == "smile_dsp"){
          display.clearDisplay();
          display.write(2);
        } else if(display_text == "clean_dsp"){
          display.clearDisplay();
        } else if(display_text == "api_dsp"){
          display.clearDisplay();
          Serial.println("text:x_point:y_point:size:scroll(true/false) | example: test:0:0:3:true");
          Serial.println("text commands: agly_dsp, smile_dsp, clead_dsp, api_dsp");
        } else {
          display.clearDisplay();
          display.println(display_text);
        }

        display.display();
        delay(200);

  }

  //  Управляем через Ir receiver
  if (irrecv.decode(&results)) {

    //  Дйствия при нажатии кнопок
    switch (results.value) {
//      case 95190340:
//        delay(100);
//        digitalWrite(2, HIGH);
//        break;

      default:
        Serial.println(results.value);
        Serial.println(results.value, HEX);
        break;
    }

    delay(200);
    irrecv.enableIRIn();
    irrecv.resume();                                                                // Receive the next value

  }
}

void blink_(int count_, int delay_)
{
  int count = 0;
  while ( count < count_ - 1)
  {
    digitalWrite(2, HIGH);
    delay(delay_);
    digitalWrite(2, LOW); 
    delay(delay_);
    digitalWrite(2, HIGH);
    delay(delay_);
    count++;
  }
  digitalWrite(2, LOW);
}
