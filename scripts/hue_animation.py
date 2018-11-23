
import time
import colorsys

import numpy as np

from bookshelf import Bookshelf


def hsv2rgb(h,s,v):
    return tuple(round(i * 255) for i in colorsys.hsv_to_rgb(h,s,v))



# note, this function is kinda jerky with the delays in wifi packets
def hue_animation(pixel_count, fps, cycle_length):
	'''Animate a cycle through the hue space, updating at 
	`fps` frames per second, with each full cycle through 
	the hue space lasting `cycle_length` seconds.'''

	frame_duration = 1.0 / fps
	frame_count = int(cycle_length / frame_duration)
	
	# initialise these for the first frame
	frame_start = time.time()
	wait_time = 0.0 


	while True:
		for f in range(frame_count):
			# build the pixel data array 
			colour = hsv2rgb(float(f) / frame_count, 1, 1)
			pix = pixel_array(led_count, colour)

			if wait_time > 0:
				time.sleep(wait_time) # wait until the next frame
			else:
				print('Error, delayed frame by {:.3f} secs.'.format(abs(wait_time)))
			
			frame_start = time.time()
			send_to_bookshelf(pix)

			# wait_time = frame duration - how long the update took
			wait_time = frame_duration - (time.time() - frame_start)



def stream_hue_animation(pixel_count, cycle_length):
	'''Animate a cycle through the hue space, updating continuously, 
	with each full cycle through the hue space lasting `cycle_length` seconds.'''
	
	b = Bookshelf()
	cycle_start = time.time()

	while True:
		hue_space_fraction = (time.time() - cycle_start) / cycle_length

		if hue_space_fraction > 1:
			cycle_start = time.time()
			continue

		# build the pixel data array 
		colour = hsv2rgb(hue_space_fraction, 1, 1)
		b.set_all(colour)

		start = time.time()
		b.send()

		timings.append(time.time() - start)




timings = []

try:
	stream_hue_animation(41, 20)
except KeyboardInterrupt:
	print('Mean frame duration:', np.mean(timings))
	print('')
	print('Frame times:')
	print(np.round(timings, 5))

