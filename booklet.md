# recurBOY

__recurBOY__ is a raspberry pi based diy video-instrument for live performance.

### motivation

it was designed to be built together with others in group soldering workshop sessions.

for many people there is a barrier to enter the world of hardware video-art making instruments - both due to the general higher cost of video gear compared with audio equipment and in some cases its obsolescence and thus rarity.

this project aims to remove these financial barriers while enabling anyone interested to build their own art-making tools. by creating together we can learn from each other and help foster community in a physical space.

### features

- outputs composite video
- 3 source modes : _sampler_ , _shaders_ and _camera_
- process any source with additional _FX_
- control shader/fx parameters directly with 4x knobs or externally with 4x cv inputs

## build guide

familiarize yourself with the basics of soldering and the parts we will be using:

ref | part
--- | ---
R1, R2, R3, R4 | 1k resistors
D1, D2, D3, D4, D5, D6, D7, D8 | bat85 diodes
J8 | Raspberry_Pi / 2x20 pin socket
J6 | Raspberry_Pi_TV_out
U1 | MCP3208 IC
J9 | 5-way push buton
SW1, SW2, SW3, SW4, SW5 | push buttons
J1, J2, J3, J4 | 3.5mm jacks
J7 | rca socket
RV1, RV2, RV3, RV4 | 10k potentiometers
J5 | 1.8" TFT display

__R1-4, D1-8__ : start by placing the resistors and diodes. it is __important__ that the _diodes_ are placed in the correct direction ! for resistors it does not matter.

__J8, J6__ : there are two parts that need to be placed from the bottom for the raspberry pi to attach. we will do these next. it is __very important__ that these are soldered on straight. start by soldering 1 or 2 pins from the header and then check it is perpendicular to the board - once multiple pins are soldered it is very difficult to correct the angle.

__U1, SW1-4, J1-4, J7__ : now you can solder the ic and other flatter components. use the ic socket if you like. also try to make sure the 5-way button is straight before soldering all the pins

__RV1-4, J5__ : finally you can solder the pots and screen. make sure the header is soldered to the screen  with short pin end through screen holes and then the combined part is soldered to the board.

you will need to also solder the gpio-header to the raspberry pi and a single pin for the tv-out - then this should slot into the bottom of the pcb.

## operation guide

check that the raspberry pi is attached to the recurBOY pcb and the flashed micro-sd is inserted.

connect recurBOY to a tv via the rca jack in the top middle. now plug the micro-usb power supply into the raspberry pi power-jack - take care it is the power jack and not the usb.

you should see the display light up and recurBOY splash on the connected tv. wait a few moments for the pi to boot - the display should show a menu when ready.

### source selection

__recurBOY__ has 3 source modes : _sampler_ , _shaders_ and _camera_. pressing the `MODE` button will cycle through these modes. note - you will only see the _camera_ mode if a raspberry pi camera is attached to the pi csi port.

you can tell which mode is selected by looking at the title and colour of the display.

### sampler

the nav_button can be pressed `UP` , `DOWN` , `LEFT`, `RIGHT` and `IN`. we will not use the `IN` button for now.

pressing `UP` and `DOWN` lets you scroll through the list of samples. this list comes from the `~/Videos` folder on the pi's SD card and from the top level of an attached usb-drive. any .mp4, .mkv, .avi or .mov file will be shown although it is not guarenteed it will work with the player - we find h264 mp4 to be most reliable.

pressing `SELECT` on a sample will start playing it. the playing sample will be highlighed on the display

### shaders

in exactly the same way as in _samples_ from this source mode you can navigate through and select a shader file to run. this list is compiled from the `~/Shaders` folder on the pi and any .frag, .shader, .glsl, .glslf,or .fsh file in the top level of an attached device.

while the display is in __SHADER__ mode you can use the 4 knobs or CV inputs to manipulate the shader parameters. each shader has parameters 1-3 mapped to different values. parameter 4 always controls the speed.

### camera

to use live input from a raspberry pi camera first ensure it is attached correctly. if a camera is attached the _camera_ source will be avaliabe by pressing `MODE`. from here pressing `SELECT` will start showing the camera _preview_. when previewing it is possible to press `SELECT` again to start and stop camera _record_. after recording has stopped it will be converted to h264 mp4 and saved in `~/Videos` to be launched in _sampler_ mode.

### FX

from any source mode you can press the `RIGHT` nav_button to enter _fx_ mode. you can see this on the title of the display. same as elsewhere an _fx_ can be selected with `UP`, `DOWN` and `SELECT`. pressing `LEFT` will return to the selected _source mode_ 

pressing the `FX` button will toggle the selected _fx_ on and off. this _fx_ will process whichever of the 3 sources is selected.

the 4 knobs / cv inputs will control parameters of the _fx_ when _source_ shader is not selected.

## extra information

### origin

the _recurBOY_ is a spinoff from an existing project tim has created and maintains - __r_e_c_u_r__ : _an open diy video sampler_ ; r_e_c_u_r is simple to assemble but can be more complex to operate due to its scope and customisablty. recurBOY distills the best parts, aiming to be simplier and more beginner friendly. it uses cheaper parts and runs on a raspberry pi zero which can be around 1/7th the price of the pi3 used in r_e_c_u_r. 

if you like this device you might want to look into also building a r_e_c_u_r one day - checkout the github or video guides online.

### some technical details

the recurBOY application is built in __openframeworks__ - a collection of open-source c++ libraries for creative coding. in particular it uses _ofxVideoArtTools_ - an abstraction of openframeworks libraries and extensions into modules specifically for making video instruments on raspberry pi.

a python3 script is used to create and interface with the 1.8inch tft display used in this project

for more infomation on writing shaders check out _the book of shaders by Gonzalez Vivo_ , also interactive sandbox : glsl.erogenous-tones.com , 

the pcb was designed in KiCad.