
#include <Arduino.h>
#include <stdint.h>
#include <WiFiClient.h>
#include <Adafruit_NeoPixel.h>

extern float global_brightness;
extern uint8_t operating_mode;
extern bool bookshelf_state;

#include "render.h"
#include "bookshelf.h"

// main function that gets called when Alexa changes the state
void alexa_callback(unsigned char device_id, const char * device_name, bool state, unsigned char value) {
	#ifdef DEBUG
		Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
	#endif

	// set the current state, on or off
	if (state != bookshelf_state) {
    execute_brightness_animation(state ? (value * 0.003921568627) : 0.0);
    bookshelf_state = state;
    return;
	}

	// based on the brightness value, do something clever
	switch(value) {

		case SET_SERVER_OPERATING_MODE: 
			operating_mode = SERVER_MODE;
			break;

		case SET_NORMAL_OPERATING_MODE: 
			operating_mode = NORMAL_MODE;
			break;

		default:
      // divide by 255 to make it in the range 0 to 1
			if (state)
			  execute_brightness_animation(value * 0.003921568627);
			break;
	}
}
