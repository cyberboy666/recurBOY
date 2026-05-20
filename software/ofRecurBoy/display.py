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
import json



def create_display():

    with open('/home/pi/openframeworks10.1/apps/myApps/ofRecurBoy/bin/data/settings.json', 'r') as f:
        settings = json.load(f)

    pixel_offset = settings.get("PIXEL_OFFSET", False)
    print("PIXEL OFFSET: ", pixel_offset)
    width = 130 if pixel_offset else 128 
    height = 161 if pixel_offset else 160
    SPEED_HZ = 4000000

    # Raspberry Pi configuration.
    DC = 27
    RST = 26
    SPI_PORT = 1
    SPI_DEVICE = 2

    disp = TFT.ST7735(
        DC,
        rst=RST,
        spi=SPI.SpiDev(
            SPI_PORT,
            SPI_DEVICE,
            max_speed_hz=SPEED_HZ),
        width=width,
        height=height)
    disp.begin()  # Initialize the display
    return disp


disp = create_display()
# Initialize display.
disp.begin()

# Get a PIL Draw object to start drawing on the display buffer.
draw = disp.draw()

# Load default font.
font = ImageFont.load_default()
font_name = "LiberationMono-Regular.ttf"
font = ImageFont.truetype(font_name, 10)
font_title = ImageFont.truetype(font_name, 16)
font_fx = ImageFont.truetype(font_name, 14)

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

class Page(object):
    def __init__(self, name):
        self.name = name
        self.menu_list = []
        self.selected_row = 0
        self.list_offset = 0
        self.playing_row =-1
        self.playing = False
        self.title = ""
        self.title_color = (255,0,255)
        self.set_page_based_on_name()

    def set_page_based_on_name(self):
        if(self.name == "VIDEO"):
            self.title = "_VIDEO_"
            self.title_color = (255,0,255) # magenta
        elif(self.name == "PATTERN"):
            self.title = "_PATTERN_"
            self.title_color = (255,255,0) # cyan
        elif(self.name == "EFFECT"):
            self.title = "_EFFECT_"
            self.title_color = (0, 255, 255) # yellow
        elif(self.name == "EXTERNAL"):
            self.title = "_EXTERNAL_"
            self.title_color = (255, 0, 0) # blue
        elif(self.name == "SETTING"):
            self.title = "_SETTING_"
            self.title_color = (255,255,255) # white
        elif(self.name == "TEXT"):
            self.title = "_TEXT_"
            self.title_color = (0,0,255) # red
        elif(self.name == "FONT"):
            self.title = "_FONT_"
            self.title_color = (0,255,0) # green
        elif(self.name == "MESSAGE"):
            self.title = "_MESSAGE_"
            self.title_color = (0,0,255) # red
    
    def get_view_list(self):
        return self.menu_list[self.list_offset:self.list_offset + 6] 


class Display(object):
    def __init__(self):
        self.osc_server = self.setup_osc_server()
        self.update_count = 0
        self.pages = {"VIDEO": Page("VIDEO"), "PATTERN": Page("PATTERN"), "EFFECT": Page("EFFECT"), "EXTERNAL": Page("EXTERNAL"), "TEXT": Page("TEXT"), "FONT": Page("FONT"), "SETTING": Page("SETTING"), "MESSAGE": Page("MESSAGE")}
        self.current_page_name = 'VIDEO'


    @staticmethod
    def get_state_symbol(state):
        if state:
            return '>'  #'▶ 
        else:
            return '■' #'■'        

    def create_this_screen(self):
        # print title
        thisPage = self.pages[self.current_page_name]
        title = thisPage.title + "_" + self.get_state_symbol(thisPage.playing)
        draw_rotated_text(disp.buffer, title, (110, 10),270, font_title, fill=(0,0,0), background=thisPage.title_color)
        if self.pages["EFFECT"].playing and self.pages["TEXT"].playing:
            draw_rotated_text(disp.buffer, '[ft]', (110, 128),270, font_fx, fill=(0,0,0), background=(255,255,255))
        elif self.pages["EFFECT"].playing:
            draw_rotated_text(disp.buffer, '[fx]', (110, 128),270, font_fx, fill=(0,0,0), background=(0,255,255))
        elif self.pages["TEXT"].playing:
            draw_rotated_text(disp.buffer, '[xt]', (110, 128),270, font_fx, fill=(0,0,0), background=(255,0,0))
        # print content
        for i, value in enumerate(thisPage.get_view_list()):
            if i == thisPage.selected_row - thisPage.list_offset:
                draw_rotated_text(disp.buffer, value, (110 - 15 - i*15, 10) ,270, font, fill=(0,0,0), background=(255,255,255))        
            elif i == thisPage.playing_row - thisPage.list_offset:
                draw_rotated_text(disp.buffer, value, (110 - 15 - i*15, 10) ,270, font, fill=(0,0,0), background=thisPage.title_color)   
            else:
                draw_rotated_text(disp.buffer, value, (110 - 15 - i*15, 10) ,270, font, fill=(255,255,255), )

    
    def setup_osc_server(self):
        server_parser = argparse.ArgumentParser()
        server_parser.add_argument("--ip", default="127.0.0.1", help="the ip")
        server_parser.add_argument("--port", type=int, default=9000, help="the port")
        
        server_args = server_parser.parse_args()

        this_dispatcher = dispatcher.Dispatcher()

        this_dispatcher.map("/LIST/*", self.set_list)
        this_dispatcher.map("/CLEAR/LIST/*", self.clear_list)
        this_dispatcher.map("/SELECTED_ROW/*", self.set_selected_row)
        this_dispatcher.map("/PLAYING_ROW/*", self.set_playing_row)
        this_dispatcher.map("/PLAYING/*", self.set_playing)
        this_dispatcher.map("/CURRENT_PAGE", self.set_current_page)
        this_dispatcher.map("/RESTART_DISPLAY", self.restart_display)

        server = osc_server.ThreadingOSCUDPServer((server_args.ip, server_args.port), this_dispatcher)
        server_thread = threading.Thread(target=server.serve_forever)
        server_thread.start()
        return server

    def set_list(self, unused_addr, *args):
        page_name = unused_addr.rsplit('/', 1)[-1].upper()
        self.pages[page_name].menu_list.extend([i.split("/")[-1] for i in list(args)])
        self.update_display_count()

    def clear_list(self, unused_addr, bool):
        page_name = unused_addr.rsplit('/', 1)[-1].upper()
        self.pages[page_name].menu_list = []    

    def set_selected_row(self, unused_addr, row):
        page_name = unused_addr.rsplit('/', 1)[-1].upper()
        thisPage = self.pages[page_name]
        if abs(thisPage.selected_row - row) > 1 and row == 0:
            # if difference is greater than 1 and row 0 must be a wrap over    
            thisPage.list_offset = 0 
        elif abs(thisPage.selected_row - row) > 1:
            # otherwise if difference is greater than 1 must be wrap under    
            thisPage.list_offset = max(row - 5, 0)
        elif row == thisPage.list_offset - 1:
            # otherwise if one less than offset moving page down    
            thisPage.list_offset = thisPage.list_offset - 1
        elif row == thisPage.list_offset + 5 + 1:
            # otherwise if one over offset page moving page up    
            thisPage.list_offset = thisPage.list_offset + 1


        thisPage.selected_row = row
        self.update_display_count()

         
    def set_playing_row(self, unused_addr, playing_row):
        page_name = unused_addr.rsplit('/', 1)[-1].upper()
        self.pages[page_name].playing_row = playing_row
        self.update_display_count()

    def set_playing(self, unused_addr, is_playing):
        page_name = unused_addr.rsplit('/', 1)[-1].upper()
        self.pages[page_name].playing = is_playing
        self.update_display_count()

    def set_current_page(self, unused_addr, page_name):
        self.current_page_name = page_name
        self.update_display_count()

    def restart_display(self, unused_addr, bool):
        global disp, draw
        disp = create_display()
        disp.begin()
        draw = disp.draw()

    def loop_over_display_update(self):
        while True:
            if self.update_count > 0:
                self.update_display()
                time.sleep(0.1)
                self.update_count = self.update_count - 1

    def update_display(self):
        disp.clear()
        self.create_this_screen()  
        disp.draw()
        disp.display()
        time.sleep(0.1)

    def update_display_count(self):
        self.update_count = self.update_count + 1        

def setup_gpio():
    import RPi.GPIO as GPIO
    GPIO.setmode(GPIO.BCM)

    pins = [4, 5, 6, 7, 12, 13, 17, 18, 19, 22, 23 ]
    for pin in pins:
        print('pin is ', pin)
        GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

    # Set GPIO25 as output, initially HIGH
    GPIO.setup(25, GPIO.OUT, initial=GPIO.HIGH)

setup_gpio()
display = Display()
display.loop_over_display_update()


