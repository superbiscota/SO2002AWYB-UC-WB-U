/*

   ( ˘ω˘)ｽﾔｧ

*/

#include <Wire.h>
#include "SO2002A_I2C.h"
#define SDA 32
#define SCL 26

SO2002A_I2C oled(0x3c);
int chars_code_buf[255] = {}; // 0 でおわる

void push (int num) {
     for (int itr = 0 ; itr < 256; itr++) {
      if (chars_code_buf[itr] == 0 ) {
        chars_code_buf[itr] = num;
        break;
        }
     }
}

void cls_buffer () {
     for (int itr = 0 ; itr < 256; itr++) {
        chars_code_buf[itr] = 0;
        }
}

void dump () {
     for (int itr = 0 ; itr < 256; itr++) {
      if (chars_code_buf[itr] == 0 ) {
        break;
       } else {
       Serial.println(chars_code_buf[itr]);
     }
    }
}

void convert_to_code(char *str) {
   for (int itr = 0 ; itr < 256; itr++) {
     if (char ((str[itr]>>7)&1 )  == 0 )  {     // 1bit 目が0なら1バイト文字
      push (int (str[itr]));
      itr = itr + 0;
     }
     else if (char ((str[itr]>>5)&1 )  == 0 )  { // 3bit 目が0なら2バイト文字
      push (int(str[itr]) * 256 + int(str[itr+1]));
      itr = itr + 1;
     }
     else if (char ((str[itr]>>4)&1 )  == 0 )  { // 4bit 目が0なら3バイト文字
      push (int(str[itr]) * 256 * 256 + int(str[itr+1]) * 256 + int(str[itr+2]));
      itr = itr + 2;
     }
     else if (char ((str[itr]>>3)&1 )  == 0 )  { // 5bit 目が0なら4バイト文字
      push (int(str[itr]) *256*256*256 + int(str[itr+1]) * 256 *256 + int(str[itr+2]) * 256 + int(str[itr+3]));
      itr = itr + 3;
     }
    }
}

void pp () {
     char caret = 0;
     char line  = 0;
     oled.selectCharacterRom(SO2002A_I2C_CHARACTER_ROM_B);
     for (int itr = 0 ; itr < 256; itr++) {
      if (chars_code_buf[itr] == 0 ) {
        break;
       } else {
        if (chars_code_buf[itr] >= 256) {
        // utf8 charctor   
         char rus_char = chars_code_buf[itr] - 53392 ; // А　
         oled.setCursor(caret,line);
         oled.write(rus_char + 0x80);
         //Serial.println(rus_char,HEX);
        } else {
        // ascii charctor   
         char ascii_char = chars_code_buf[itr];
         oled.write(ascii_char);
        }
        caret++;
         if (caret > 19) {
          line = 1 ;
          caret = 0; 
         }
     }
    }
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Wire.begin(SDA,SCL);
  //Wire.setClock(400000);
  oled.clear();
  oled.begin(20, 2);
  //dump();
}

void loop() {

  // put your main code here, to run repeatedly:
  char data1[] = "I2C OLED YELLOW 20x2";
  char data2[] = "HELLO WORLD !!";

  cls_buffer();
  char str[256] = "УМ ";

  //oled.setCursor(0,7);
  convert_to_code(str);
  pp();
  delay(1000);  
}
