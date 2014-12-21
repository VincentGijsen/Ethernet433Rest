
#define LIGHTWEIGHT 1

/* 
 This a simple example of the aREST Library for Arduino (Uno/Mega/Due/Teensy)
 using the Ethernet library (for example to be used with the Ethernet shield). 
 See the README file for more details.
 
 Written in 2014 by Marco Schwartz under a GPL license. 
 */

// Libraries
#include <SPI.h>
#include <Ethernet.h>
#include <aREST.h>
#include <avr/wdt.h>


//#include <RemoteReceiver.h>
#include <RemoteTransmitter.h>
#include <VirtualWire.h>


//Transmitter stuff
ElroTransmitter elroTransmitter(A0);
ActionTransmitter actionTransmitter(A0);
KaKuTransmitter kaKuTransmitter(A0);
BlokkerTransmitter blokkerTransmitter(A0);


// Enter a MAC address for your controller below.
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };

// IP address in case DHCP fails
IPAddress ip(192,168,1,2);

// Ethernet server
EthernetServer server(80);

// Create aREST instance
aREST rest = aREST();

// Variables to be exposed to the API
int temperature;
int humidity;

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);


  //config VirtualWire
  // Initialise the IO and ISR
  //IMPORTANT: PUT IT BEFORE SERVER.begin or it WILL FAIL
  vw_set_tx_pin(A0);
  // vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);	 // Bits per sec



  // Init variables and expose them to REST API
  temperature = 24;
  humidity = 40;
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);

  // Function to be exposed
  rest.function("led",ledControl);

  rest.function("elro", elroControl);
  //rest.function("kaku", kakuControl);
  rest.function("blokker", blokkerControl);
  rest.function("action", actionControl);
  rest.function("virtual", virtualWireControl);

  // Give name and ID to device
  rest.set_id("008");
  rest.set_name("viniesRemote");

  // Start the Ethernet connection and the server
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());


  Serial.println("watchdog started");
  // Start watchdog
  wdt_enable(WDTO_4S);

}

void loop() {  

  // listen for incoming clients
  EthernetClient client = server.available();
  rest.handle(client);
  wdt_reset();

}

int virtualWireControl(String package){
  //first param is the address of the receiver
  // sencond param is value/operation/device specific
  //eg: 32,255 (for a simple dimmer app );
  char buff[package.length()+1];
  package.toCharArray(buff, package.length());
  vw_send((uint8_t *) buff, package.length()+1);
}

int elroControl(String cmd){  
  int code = getValue(cmd, ',', 0).toInt();
  char device = getValue(cmd, ',', 1)[0];
  int state = getValue(cmd, ',', 2).toInt();    
  //elro needs systemcode (numerical), device (upper char), and boolean
  elroTransmitter.sendSignal(code, device, state);      
}

//
int blokkerControl(String cmd){  
  //int code = getValue(cmd, ',', 0).toInt();
  char device = getValue(cmd, ',', 0)[0];
  int state = getValue(cmd, ',', 1).toInt();    
  //elro needs systemcode (numerical), device (upper char), and boolean
  blokkerTransmitter.sendSignal(device, state);      
}


int actionControl(String cmd){  
  int code = getValue(cmd, ',', 0).toInt();
  char device = getValue(cmd, ',', 1)[0];
  int state = getValue(cmd, ',', 2).toInt();    
  //action needs systemcode (numerical), device (upper char), and boolean
  actionTransmitter.sendSignal( device, code, state);
}
// Custom function accessible by the API
int ledControl(String command) {

  // Get state from command
  int state = command.toInt();

  digitalWrite(6,state);
  return 1;
}


//
// Helper stuff
//
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {
    0, -1  };
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

