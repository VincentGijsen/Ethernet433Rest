/*
 *  Author: Vincent Gijsen
 *  Date: 21-dec-2014
 */

#include <VirtualWire.h>

//pro-mini
#define led 13
#define receive_pin 11


//We define addresses, so every device is uniquely identifieable (as char!);

#define MYADDRESS '1'

uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN;




byte _ledTable[256] = {
  0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
  1,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   4,   4,
  4,   4,   4,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,
  8,   8,   9,   9,   9,  10,  10,  10,  11,  11,  12,  12,  12,  13,  13,  14,
14,  15,  15,  15,  16,  16,  17,  17,  18,  18,  19,  19,  20,  20,  21,  22,
22,  23,  23,  24,  25,  25,  26,  26,  27,  28,  28,  29,  30,  30,  31,  32,
33,  33,  34,  35,  36,  36,  37,  38,  39,  40,  40,  41,  42,  43,  44,  45,
46,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,
61,  62,  63,  64,  65,  67,  68,  69,  70,  71,  72,  73,  75,  76,  77,  78,
80,  81,  82,  83,  85,  86,  87,  89,  90,  91,  93,  94,  95,  97,  98,  99,
101, 102, 104, 105, 107, 108, 110, 111, 113, 114, 116, 117, 119, 121, 122, 124,
125, 127, 129, 130, 132, 134, 135, 137, 139, 141, 142, 144, 146, 148, 150, 151,
153, 155, 157, 159, 161, 163, 165, 166, 168, 170, 172, 174, 176, 178, 180, 182,
184, 186, 189, 191, 193, 195, 197, 199, 201, 204, 206, 208, 210, 212, 215, 217,
219, 221, 224, 226, 228, 231, 233, 235, 238, 240, 243, 245, 248, 250, 253, 255 };


void setup(void){
  Serial.begin(115200);

  pinMode(led, OUTPUT);

  vw_set_rx_pin(receive_pin);

  vw_setup(2000);	 // Bits per sec
  vw_rx_start();       // Start the receiver PLL running

  Serial.println("PWM_client Started");
  blink();



}



void loop(){

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    int i;

    // Message with a good checksum received, print it.
    Serial.print("Got: ");

  //package should be formated as <address 1 byte><payload>
   if(MYADDRESS != buf[i]){
        //package not for me,
       return; 
      }
     
     
    for (i = 1; i < buflen; i++)
    {
      //check if first byte equals my address
      //Serial.print(buf[i], HEX);
      //Serial.print(' ');
    }
    Serial.println();
    //digitalWrite(led_pin, LOW);
    blink();
  }


}

void blink(){
  for (int x=0;x<10;x++){
    digitalWrite(led, HIGH);
    delay(50);
    digitalWrite(led, LOW);
    delay(50);

  } 
}

