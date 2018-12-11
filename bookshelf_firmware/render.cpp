

#include <Arduino.h>
#include <stdint.h>
#include <WiFiClient.h>
#include <Adafruit_NeoPixel.h>

#include "bookshelf.h"

extern float global_brightness;
extern bool bookshelf_state;
extern uint8_t operating_mode;
extern uint8_t bookshelf_colour[]; 

extern Adafruit_NeoPixel pixels;

// set to true any time there's an active brightness
// animation in progress.
bool animation_in_progress = false;

float brightness_begin = 0;
float brightness_end = 0;

unsigned long brightness_animation_start = 0L;


// begins a brightness animation
void execute_brightness_animation(float target_brightness) {
	brightness_begin = global_brightness;
	brightness_end = target_brightness;

	animation_in_progress = true;
	brightness_animation_start = millis();

}


void update_brightness() {
	unsigned int animation_elapsed = (int)(millis() - brightness_animation_start);

	if (animation_elapsed > BRIGHTNESS_ANIMATION_DURATION) {
		// finish up the animation
		global_brightness = brightness_end;
		animation_in_progress = 0;

	} else {
		float brightness_diff = (brightness_end - brightness_begin);
		// linear brightness change 
		//    TODO: look at log scaling 
		global_brightness = brightness_begin + (brightness_diff * animation_elapsed / BRIGHTNESS_ANIMATION_DURATION);
	}
}


void scale_pixel_brightness_in_place(uint8_t* px, float b) {
	for (int i = 0; i < 3; i++) px[i] = px[i] * b;		
}


void render_normal_mode() {
	if (animation_in_progress) {

		update_brightness();

    // calculate new scaled values given brightness
    static uint8_t scaled_colour[3];
    for (int i = 0; i<3; i++) scaled_colour[i] = floor(bookshelf_colour[i] * global_brightness);

//    #ifdef DEBUG
//      Serial.printf("Brightness: %d -- pixel colour [%d, %d, %d]\n", (int)(100 * global_brightness), 
//        scaled_colour[0], scaled_colour[1], scaled_colour[2]);
//    #endif

		// set the pixel data
		for (int i = 0; i < pixel_count; i++) {
			pixels.setPixelColor(i,
				scaled_colour[0],
				scaled_colour[1],
				scaled_colour[2]
			); 
		}

		// send the data out to the pixels
		pixels.show();
 	}
}


void render_server_mode() {
	return;
}


void render() {
	if (operating_mode == NORMAL_MODE) 
		render_normal_mode();

	else if (operating_mode == SERVER_MODE) 
		render_server_mode();

}
