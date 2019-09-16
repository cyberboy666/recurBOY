from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

import ST7735 as TFT
import Adafruit_GPIO as GPIO
import Adafruit_GPIO.SPI as SPI

from pythonosc import osc_server
from pythonosc import osc_message_builder
from pythonosc import dispatcher
import threading
import argparse
import time

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

class Display(object):
    def __init__(self):
        self.osc_server = self.setup_osc_server()
        self.inputs = ['SAMPLER', 'SHADERS', 'CAMERA']
        self.sample_list = []
        self.sampler_state = False
        self.shaders_state = False
        self.shader_list = ['colours.frag', 'spinning.frag', 'flips.frag', 'flowing.frag', 'blobs1.frag', 'blobs2.frag', 'bounce.frag', 'colours3.frag', 'colours4.frag']
        self.fx_list = ['hsv.frag', 'kalia.frag', 'spinning.frag', 'blacknwhite.frag', 'colourizer.frag', 'wobble.frag', 'wobble2.frag', 'wobble3.frag', 'too_much_wobbble.frag']
        self.current_mode = 'SAMPLER'
        self.fx_screen_visible = False
        self.is_camera_on = False  
        self.switch_input_mode()
        self.current_list_offset = 0
        self.view_list = self.get_view_list()
        self.selected_row = 0

    def get_view_list(self):
        return self.current_list[self.current_list_offset:self.current_list_offset + 6] 

    @staticmethod
    def get_state_symbol(state):
        if state:
            return '>'  #'▶'
        else:
            return '[]' #'■'        

    def switch_input_mode(self):

        if self.fx_screen_visible:
            self.title = '__FX___'
            self.current_list = self.fx_list
            self.current_title_colour = (0,255,255)
        elif self.current_mode == 'SAMPLER':
            self.title = '__SAMPLER_{}_'.format(self.get_state_symbol(self.sampler_state))
            self.current_title_colour = (255,0,255)
            self.current_list = self.sample_list
        elif self.current_mode == 'SHADERS':
            self.title = '__SHADERS_{}_'.format(self.get_state_symbol(self.shaders_state))
            self.current_list = self.shader_list
            self.current_title_colour = (255,255,0)
        elif self.current_mode == 'CAMERA':
            self.title = '__CAMERA___'
            if self.is_camera_on:
                self.current_list = ['record']
            else:
                self.current_list = ['preview']
            self.current_title_colour = (0,255,255)
        
        self.current_list_offset = 0
        self.selected_row = 0        

    def create_this_screen(self):
        self.create_sample_screen()

    def create_sample_screen(self):
        # print title
        draw_rotated_text(disp.buffer, self.title, (110, 10),270, font_title, fill=(0,0,0), background=self.current_title_colour)
        # print content
        for i, value in enumerate(self.get_view_list()):
            if i == self.selected_row - self.current_list_offset:
                draw_rotated_text(disp.buffer, value, (110 - 15 - i*15, 10) ,270, font, fill=(0,0,0), background=(255,255,255))        
            else:
                draw_rotated_text(disp.buffer, value, (110 - 15 - i*15, 10) ,270, font, fill=(255,255,255), )


    
    def setup_osc_server(self):
        server_parser = argparse.ArgumentParser()
        server_parser.add_argument("--ip", default="127.0.0.1", help="the ip")
        server_parser.add_argument("--port", type=int, default=9000, help="the port")
        
        server_args = server_parser.parse_args()

        this_dispatcher = dispatcher.Dispatcher()

        this_dispatcher.map("/sampleList", self.set_sample_list)
        this_dispatcher.map("/shaderList", self.set_shader_list)
        this_dispatcher.map("/fxList", self.set_fx_list)
        this_dispatcher.map("/selectedRow", self.set_selected_row)
        this_dispatcher.map("/inputMode", self.set_input_mode)
        this_dispatcher.map("/fxScreenVisible", self.set_fx_screen_visible)
        this_dispatcher.map("/isCameraOn", self.set_is_camera_on)


        server = osc_server.ThreadingOSCUDPServer((server_args.ip, server_args.port), this_dispatcher)
        server_thread = threading.Thread(target=server.serve_forever)
        server_thread.start()
        return server

    def set_sample_list(self, unused_addr, *args):
        self.sample_list = [i.split("/")[-1] for i in list(args)]
        self.switch_input_mode()         


    def set_shader_list(self, unused_addr, *args):
        self.shader_list = [i.split("/")[-1] for i in list(args)]
        self.switch_input_mode()

    def set_fx_list(self, unused_addr, *args):
        self.fx_list = [i.split("/")[-1] for i in list(args)]
        self.switch_input_mode()

    def set_is_camera_on(self, unused_addr, is_camera_on):
        self.is_camera_on = bool(is_camera_on)
        self.switch_input_mode()

         
    def set_selected_row(self, unused_addr, row):
        self.selected_row = row
        if self.selected_row == self.current_list_offset - 1:
            self.current_list_offset = self.current_list_offset - 1
        elif self.selected_row == self.current_list_offset + 5 + 1:
            self.current_list_offset = self.current_list_offset + 1

    def set_input_mode(self, unused_addr, mode):
        self.current_mode = mode
        self.switch_input_mode()

    def set_fx_screen_visible(self, unused_addr, is_visible):
        self.fx_screen_visible = bool(is_visible)
        self.switch_input_mode()

    def loop_over_display_update(self):
        while True:

            disp.clear()
            self.create_this_screen()  
            disp.draw()
            disp.display()
            time.sleep(0.1)

display = Display()
display.loop_over_display_update()

