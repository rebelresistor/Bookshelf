
from bookshelf import Bookshelf


red = (255, 0, 0)
white = (255, 255, 255)
off = (0, 0, 0)

b = Bookshelf()

for x in range(10):
	for x in range(b.pixel_count):
		b.set_all(off)
		b.set_pixel(x, red)
		b.send()

	for x in reversed(range(b.pixel_count)):
		b.set_all(off)
		b.set_pixel(x, red)
		b.send()

b.set_all(off)
b.send()