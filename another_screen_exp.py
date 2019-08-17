from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

import ST7735 as TFT
import Adafruit_GPIO as GPIO
import Adafruit_GPIO.SPI as SPI

#from pythonosc import dispatcher
import keyboard

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
        self.text = 'Hello World! this is \n the place where we would \n draw the infomation \n about recurBOY'
        self.inputs = ['SAMPLER', 'SHADERS', 'CAMERA']
        self.sample_list = ['long_spinning.mp4', 'mushroom_dreams.mp4', 'motorway1.mp4', 'motorway2.mp4', 'motorway3.mp4', 'simple_loop.mp4', 'tv_screen.mp4', 'tv_screen2.mp4', 'tv_screen3.mp4']
        self.sampler_state = False
        self.shaders_state = False
        self.shader_list = ['colours.frag', 'spinning.frag', 'flips.frag', 'flowing.frag', 'blobs1.frag', 'blobs2.frag', 'bounce.frag', 'colours3.frag', 'colours4.frag']
        self.fx_list = ['hsv.frag', 'kalia.frag', 'spinning.frag', 'blacknwhite.frag', 'colourizer.frag', 'wobble.frag', 'wobble2.frag', 'wobble3.frag', 'too_much_wobbble.frag']
        self.current_mode = 'SAMPLER'
        self.fx_screen_visible = False
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
            self.current_list = ['preview', 'recording']
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
            if i == self.selected_row:
                draw_rotated_text(disp.buffer, value, (110 - 15 - i*15, 10) ,270, font, fill=(0,0,0), background=(255,255,255))        
            else:
                draw_rotated_text(disp.buffer, value, (110 - 15 - i*15, 10) ,270, font, fill=(255,255,255), )


    def check_for_key_press(self):

        if keyboard.is_pressed('up'):
            if self.selected_row == 0 and self.current_list_offset == 0 :
                pass
            elif self.selected_row == 0 and self.current_list_offset != 0 :
                self.current_list_offset = self.current_list_offset - 1
            else:  
                self.selected_row = self.selected_row - 1
        elif keyboard.is_pressed('down'):

            if self.selected_row == 5 and self.current_list_offset == len(self.sample_list) - 6 or self.selected_row == len(self.current_list) - 1 :
                pass
            elif self.selected_row == 5 and self.current_list_offset != len(self.sample_list) - 6:
                self.current_list_offset = self.current_list_offset + 1
            else:  
                self.selected_row = self.selected_row + 1
        elif keyboard.is_pressed('c'):
            self.inputs = self.inputs[1:] + self.inputs[:-1]
            self.current_mode = self.inputs[0]
            self.switch_input_mode()
        elif keyboard.is_pressed('right'):
            self.fx_screen_visible = True
            self.switch_input_mode()
        elif keyboard.is_pressed('left'):
            if self.fx_screen_visible:
                self.fx_screen_visible = False
                self.switch_input_mode()
        elif keyboard.is_pressed('enter'):
            if self.fx_screen_visible:
                pass
            elif self.current_mode == 'SAMPLER':
                self.sampler_state = not self.sampler_state
                self.title = '__SAMPLER_{}_'.format(self.get_state_symbol(self.sampler_state))
            elif self.current_mode == 'SHADERS':
                self.shaders_state = not self.shaders_state 
                self.title = '__SHADERS_{}_'.format(self.get_state_symbol(self.shaders_state))


    def loop_over_display_update(self):
        while True:

            disp.clear()
            self.create_this_screen()
            self.check_for_key_press()    
            disp.draw()
            disp.display()


display = Display()
display.loop_over_display_update()

def setup_osc_server(self):
    server_parser = argparse.ArgumentParser()
    server_parser.add_argument("--ip", default="127.0.0.1", help="the ip")
    server_parser.add_argument("--port", type=int, default=9000, help="the port")

    server_args = server_parser.parse_args()

    this_dispatcher = dispatcher.Dispatcher()
    this_dispatcher.map("/player/c/status", self.video_driver.receive_status, "c.c")
    this_dispatcher.map("/detour/detour_info", self.receive_detour_info)
    this_dispatcher.map("/capture/recording_finished", self.capture.receive_recording_finished)
    this_dispatcher.map("/shutdown", self.exit_osc_server)

    server = osc_server.ThreadingOSCUDPServer((server_args.ip, server_args.port), this_dispatcher)
    server_thread = threading.Thread(target=server.serve_forever)
    server_thread.start()
    return server



