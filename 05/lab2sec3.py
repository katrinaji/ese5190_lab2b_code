import board
import busio
import adafruit_apds9960.apds9960
import time
import neopixel

i2c = busio.I2C(board.SCL1, board.SDA1)
apds = adafruit_apds9960.apds9960.APDS9960(i2c)

#Coloring
apds.enable_color = True
#Change the sampling time
apds.color_integration_time = 50
#Set LED
pixels = neopixel.NeoPixel(board.NEOPIXEL, 1)
while True:
    r, g, b, c = apds.color_data
    print('Red: {0}, Green: {1}, Blue: {2}, Clear: {3}'.format(r, g, b, c))
    pixels.fill((c, c, c))