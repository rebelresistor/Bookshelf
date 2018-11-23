#!/usr/bin/env python3

'''A module to interface with my bookshelf'''


import socket
import argparse
import webcolors


__author__ = "Kevin O'Connell"
__license__ = "GNU GPL"
__version__ = "0.1"
__maintainer__ = "Kevin O'Connell"
__email__ = "kevinoc@apple.com"
__status__ = "Prototype"


bookshelf = ('bookshelf.local', 5000)
led_count = 41


# note, this segment map becomes attributes of the Bookshelf()
# class, so the names you choose for your keys in this dict 
# should be valid attribute names, or you won't be able to 
# use this functionality
SEGMENT_MAP = {
	'L1': (24, 31),
	'L2': (16, 23),
	'L3': (0, 7),
	'R1': (33, 40),
	'R2': (8, 15)
}


def name2rgb(colour_name):
	c = COLOURS.get(args.colour)
	if c is None:
		c = webcolors.name_to_rgb(args.colour)
	return c


class Bookshelf(object):
	def __init__(self, pixel_count = led_count, segment_map = SEGMENT_MAP):

		# this is required so the overridden __setattr__ function doesn't 
		# crap out by not having _segment_map defined... :-/
		object.__setattr__(self, '_segment_map', segment_map)

		self.pixel_count = pixel_count
		self.pixels = bytearray([0] * (pixel_count * 3))

	def __setattr__(self, name, value):
		if name in self._segment_map and len(value) == 3:
			self.set_segment(name, value)
		else:
			# otherwise, do the normal python assignment
			self.__dict__[name] = value


	def set_pixel(self, pixel, colour):
		if len(colour) != 3:
			raise ValueError('Colour should be a tuple of RGB values')

		self.pixels[pixel * 3]     = colour[0]
		self.pixels[pixel * 3 + 1] = colour[1]
		self.pixels[pixel * 3 + 2] = colour[2]


	def set_pixel_range(self, start_pixel, stop_pixel, colour):
		for p in range(start_pixel, stop_pixel + 1):
			self.set_pixel(p, colour)


	def set_all(self, colour):
		for p in range(self.pixel_count):
			self.set_pixel(p, colour)
			
	def set_segment(self, segment_name, colour):
		if segment_name not in self._segment_map and len(colour) != 3:
			raise ValueError('Invalid segment name')
		
		start, stop = self._segment_map.get(segment_name)
		self.set_pixel_range(start, stop, colour)


	@property
	def segment_names(self):
		return list(self._segment_map.keys())


	def clear(self):
		self.set_all((0,0,0))
			

	def show(self):
		self.send()
		

	def send(self, pixel_byte_array = None):
		'''Send the pixel byte array to the bookshelf'''

		data = pixel_byte_array or self.pixels

		# Create a TCP/IP socket
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		sock.settimeout(5)
		sock.connect(bookshelf)

		bytes_sent = sock.send(data)

		return bytes_sent == len(data)




COLOURS = {
	"Candle":                      (255, 147, 41 ),
	"40W Tungsten":                (255, 197, 143),
	"100W Tungsten":               (255, 214, 170),
	"Halogen":                     (255, 241, 224),
	"Carbon Arc":                  (255, 250, 244),
	"High Noon Sun":               (255, 255, 251),
	"Direct Sunlight":             (255, 255, 255),
	"Overcast Sky":                (201, 226, 255),
	"Clear Blue Sky":              (64,  156, 255),

	"Warm Fluorescent":            (255, 244, 229),
	"Standard Fluorescent":        (244, 255, 250),
	"Cool White Fluorescent":      (212, 235, 255),
	"Full Spectrum Fluorescent":   (255, 244, 242),
	"Grow Light Fluorescent":      (255, 239, 247),
	"Black Light Fluorescent":     (167, 0,   255),
	"Mercury Vapor":               (216, 247, 255),
	"Sodium Vapor":                (255, 209, 178),
	"Metal Halide":                (242, 252, 255),
	"High Pressure Sodium":        (255, 183, 76),
}

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description=__doc__)
	parser.add_argument('--on', default = False, action='store_true', help='Turn on the Bookshelf, all white')
	parser.add_argument('--off', default = False, action='store_true', help='Turn off the Bookshelf')
	parser.add_argument('--colour', type=str, help='Set a specific colour on the bookshelf')

	args = parser.parse_args()

	b = Bookshelf()

	if args.on:
		b.set_all((255,255,255))


	if args.off:
		b.set_all((0,0,0))


	if args.colour:
		b.set_all(name2rgb(args.colour))


	b.send()
