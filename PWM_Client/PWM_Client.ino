/*
 *  Author: Vincent Gijsen
 *  Date: 21-dec-2014
 */

#include <VirtualWire.h>

//pro-mini
#define led 13
#define receive_pin 11


//We define addresses, so every device is uniquely identifieable (as char!);

#define MyAddress '1'

uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN;



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

    for (i = 0; i < buflen; i++)
    {
      Serial.print(buf[i], HEX);
      Serial.print(' ');
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

