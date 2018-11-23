/*
 * WS8212 TCPserver
 * 
 */

#define DEBUG_FAUXMO_VERBOSE true


// Neopixel
#include <Adafruit_NeoPixel.h>

// ESP8266 libraries
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

// My wifi credentials, not committed to GitHub :)
#include "wifi_credentials.h"

// Wifi Credentials
// ================
//
// The above header file just contains two lines:
//   const char *ssid = "WIFI-NAME";
//   const char *password = "WIFI-PASSWORD";


// useful function to allow aborting TCP connections
#include "tcp_abort_connections.h"

// bookshelf settings
#include "bookshelf.h"

// create the socket server
WiFiServer socket_server(socket_port);


// LED Pin on the ESP8266 (debug LED on the PCB)
const int led_pin = 13;


// create the neopixel object
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(pixel_count, pixel_pin_out, 
                                             NEO_GRB + NEO_KHZ800);

// TODO: the Data Out pin of the very last LED on the line has also 
//       been brought back to the ESP8266. In the future, I'll
//       simultaneously shift out data on D4 and shift in data on D2
//       which will give a way to automatically count the number of
//       LEDs and know when the data path is broken.


// the handlers that will handle the TCP requests
#include "tcp_socket_handlers.h"

const char *dns_name = DNS_NAME;

fauxmoESP fauxmo;


void set_all_pixels(uint8_t colours[3]) {
  // set the pixel data
  for (int i = 0; i < pixel_count; i++) {    
    pixels.setPixelColor(i,
      colours[0],
      colours[1],
      colours[2]
    );
  }
  pixels.show();
}

void AlexaCallback(unsigned char device_id, const char * device_name, bool state, unsigned char value) {
  Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
  
  uint8_t colour[] = {0, 0, 0};
  if (state) memset(colour, 255, 3);
 
  set_all_pixels(colour); 
}


void setup ( void ) {
  // turn off the LED on the board
  pinMode ( led_pin, OUTPUT );
  digitalWrite ( led_pin, 0 );

  // zero out the client buffer
  memset(client_buffer, 0, CLIENT_BUFFER_SIZE);

  #ifdef DEBUG
    Serial.begin ( 115200 );
  #endif

  // Allocate memory and initialise the pixels to 0
  pixels.begin();
  pixels.show();
  DEBUG_PRINTLN(F("NeoPixel started"));
  
  // Connect to wifi
  WiFi.begin( ssid, password );
  
  // Wait for connection
  DEBUG_PRINT("Connecting to Wifi ");
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 250 );
    DEBUG_PRINT( "." );
  }

  DEBUG_PRINTLN ("" );
  DEBUG_PRINT   ("Connected to ");
  DEBUG_PRINTLN (ssid );
  DEBUG_PRINT   ("IP address: ");
  DEBUG_PRINTLN (WiFi.localIP());

  if (MDNS.begin(dns_name)) {
    DEBUG_PRINT ("MDNS responder started @ ");
    DEBUG_PRINT (dns_name);
    DEBUG_PRINTLN (".local");
  }

  // kick off the socket server
  DEBUG_PRINTLN ("Starting socket server.");
  socket_server.begin();

  // add the device for Alexa;
  DEBUG_PRINTLN ("Adding Alexa support for Bookshelf");
  fauxmo.addDevice("Bookshelf");
  fauxmo.onSetState(AlexaCallback);
  fauxmo.enable(true);
  fauxmo.enable(false);
  fauxmo.enable(true);
  
  // turn on the LED so we know the board has exited startup
  digitalWrite ( led_pin, 1 );

  DEBUG_PRINTLN ("Done with setup!");

}

void loop ( void ) {
  WiFiClient client = socket_server.available();
 
  if (client) {
    handle_client(client); 
  }

  fauxmo.handle();

  //tcpCleanup(); // remove any old TCP connections in the WAIT_STATE
}


