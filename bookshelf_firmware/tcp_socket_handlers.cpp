
#include <Arduino.h>
#include <stdint.h>
#include <WiFiClient.h>
#include <Adafruit_NeoPixel.h>


#include "bookshelf.h"


// link to the main pixels object
extern Adafruit_NeoPixel pixels;

// create the client buffer
uint8_t client_buffer[CLIENT_BUFFER_SIZE];


// definition of the client handler
void handle_client(WiFiClient client) {

  DEBUG_PRINTLN("Client connected!");

  // read the data from the client
  int i = 0;
  while (client.connected()) {
    while (client.available()>0) {
      char c = client.read();
      client_buffer[i++] = c;
    }
    delay(10);
  }
 
  client.stop();
  DEBUG_PRINT("Client disconnected.");

  // set the pixel data
  for (i = 0; i < pixel_count; i++) {
    int buf_offset = i * 3;
    
    pixels.setPixelColor(i,
      client_buffer[buf_offset],
      client_buffer[buf_offset + 1],
      client_buffer[buf_offset + 2]
    ); 
  }

  // send the data out to the pixels
  pixels.show();


  // zero out the client buffer
  // if a new connection comes in with a shorter data message
  // the end of the last message will still be in memory
  memset(client_buffer, 0, CLIENT_BUFFER_SIZE);
}


