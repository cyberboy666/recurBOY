from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

import ST7735 as TFT
import Adafruit_GPIO as GPIO
import Adafruit_GPIO.SPI as SPI


WIDTH = 128
HEIGHT = 160
SPEED_HZ = 4000000


# Raspberry Pi configuration.
DC = 24
RST = 25
SPI_PORT = 0
SPI_DEVICE = 0

# Create TFT LCD display class.
disp = TFT.ST7735(
    DC,
    rst=RST,
    spi=SPI.SpiDev(
        SPI_PORT,
        SPI_DEVICE,
        max_speed_hz=SPEED_HZ),
	width=WIDTH,
	height=HEIGHT)

# Initialize display.
disp.begin()

# Get a PIL Draw object to start drawing on the display buffer.
draw = disp.draw()

# Load default font.
font = ImageFont.load_default()
font_name = "LiberationMono-Regular.ttf"
font = ImageFont.truetype(font_name, 10)
font_title = ImageFont.truetype(font_name, 16)

# Define a function to create rotated text.  Unfortunately PIL doesn't have good
# native support for rotated fonts, but this function can be used to make a
# text image and rotate it so it's easy to paste in the buffer.
def draw_rotated_text(image, text, position, angle, font, fill=(255,255,255),background=(0,0,0) ):
    # Get rendered font width and height.
    draw = ImageDraw.Draw(image)
    width, height = draw.textsize(text, font=font)
    # Create a new image with transparent background to store the text.
    textimage = Image.new('RGBA', (width, height), background + (255,))
    # Render the text.
    textdraw = ImageDraw.Draw(textimage)
    textdraw.text((0,0), text, font=font, fill=fill)
    # Rotate the text image.
    rotated = textimage.rotate(angle, expand=1)
    # Paste the text into the image, using it as a mask for transparency.
    image.paste(rotated, position, rotated)

text = 'Hello World! this is \n the place where we would \n draw the infomation \n about recurBOY'
title = '__SAMPLER_____'
sample_list = ['long_spinning.mp4', 'mushroom_dreams.mp4', 'motorway1.mp4', 'motorway2.mp4', 'motorway3.mp4', 'simple_loop.mp4', 'tv_screen.mp4']
sample_list_offset = 0
view_list = sample_list[sample_list_offset:sample_list_offset + 6] 
selected_row = 5


def create_this_screen():
    create_sample_screen()

def create_sample_screen():
    # print title
    draw_rotated_text(disp.buffer, title, (110, 10),270, font_title, fill=(0,0,0), background=(255,0,255))
    # print content
    for i, value in enumerate(view_list):
        if i == selected_row:
            draw_rotated_text(disp.buffer, value, (110 - 15 - i*15, 10) ,270, font, fill=(0,0,0), background=(255,255,255))        
        else:
            draw_rotated_text(disp.buffer, value, (110 - 15 - i*15, 10) ,270, font, fill=(255,255,255), )


while True:
    disp.clear()
    create_this_screen()
    
    disp.draw()
    disp.display()


