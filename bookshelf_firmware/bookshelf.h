

#ifndef __bookshelf_h__
#define __bookshelf_h__

// WS2812 Config 
#define pixel_pin_out D4
#define pixel_pin_return D2
#define pixel_count 41

// this is the DNS-SD name that the bookshelf will broadcast,
// it will be contactable at bookshelf.local
#define DNS_NAME "bookshelf"


// TCP socket server port
const int socket_port = 5000;

// enable or disable serial debugging
#define DEBUG
#include "DEBUG.h"


// include the fauxmo Library for Alexa
#include <fauxmoESP.h>
extern unsigned int BOOKSHELF_DEVICE_ID;


// Create a client read buffer to hold any data received over TCP
// while it's being processed.
#define CLIENT_BUFFER_SIZE 256
extern uint8_t client_buffer[];



#endif
