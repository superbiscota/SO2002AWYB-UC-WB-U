/*
  ASCII table

  Prints out byte values in all possible formats:
  - as raw binary values
  - as ASCII-encoded decimal, hex, octal, and binary values

  For more on ASCII, see http://www.asciitable.com and http://en.wikipedia.org/wiki/ASCII

  The circuit: No external hardware needed.

  created 2006
  by Nicholas Zambetti <http://www.zambetti.com>
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/ASCIITable
*/

#include <Wire.h>
#include "SO2002A_I2C.h"
#define SDA 19
#define SCL 33

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

void dump () {
     for (int itr = 0 ; itr < 256; itr++) {
      if (chars_code_buf[itr] == 0 ) {
        break;
       } else {
       Serial.println(chars_code_buf[itr]);
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
         Serial.println(rus_char,HEX);
  
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

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Wire.begin(SDA,SCL);
  Wire.setClock(400000);
      
  
  oled.clear();
  oled.begin(20, 2);

  char str[256] = "ОТ СУМЫ ДА ТЮРЬМЫ НЕ ЗАРЕКАЙСЯ";
  convert_to_code(str);
  //dump();
  pp();
}




void loop() {

}
