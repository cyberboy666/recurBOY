# recurBOY

### a raspberrry pi zero video instrument

[3d rendered circuit image goes here]

- this circuit is distributed by __UNDERSCORES__ - _an open video hardware label_ : it is available to purchase - as a pcb, kit or assembled unit - at [underscores.shop](https://underscores.shop/product/recurboy/)
- the schematic for the circuit can be found [here](/hardware/schematic.pdf)
- the pcb gerber files for the lastest version can be found [here](/hardware/gerber_latest.zip)
- interactive BOM is [here](https://htmlpreview.github.io/?https://github.com/cyberboy666/recurBOY/blob/main/hardware/bom/ibom.html)
- consider [donating](https://opencollective.com/underscores) to the underscores project to help us continue creating for the commons

## description

_recurBOY_ is a stand-alone digital video synthesizer and sampler. it can trigger clips and run shaders to create and manipulate sd video. 

- outputs sd composite video
- 2 source modes to generate video : sampler and shaders
- process any source with additional FX
- control shader/fx parameters directly with 4x knobs or externally with 4x cv inputs
- process (glitchy) external video through easycap capture card


## demo video

[coming soon]

## background

<details><summary>background</summary>

recurBOY was originally designed in 2019 by tim & guergana to be built together with others in group workshop soldering sessions - the design is a spinoff from [r_e_c_u_r](https://github.com/cyberboy666/r_e_c_u_r) an existing project tim created and maintains. r_e_c_u_r is simple to assemble but more complex to operate due to its scope and customizablity. recurBOY distills the best parts, aiming to be simpler and more beginner friendly. it uses cheaper parts and runs on a raspberry pi zero which can be a fraction the price of the pi3 used in r_e_c_u_r.


</details>

# documentation

this project is fully _open-source hardware_ - all the files required to build it are included in this repo for free. if you have the time and/or skill you can contribute back by collaborating on / testing new designs, improving these docs, making demo videos/other creative content etc. you can also support the project financially by [donating directing](https://opencollective.com/underscores), or purchasing through the [web shop](https://underscores.shop).

depending on whether you are going fully diy or buying an assembled and tested unit, some of the following guides will be relavent to you. the flow would be:

## ordering parts

<details><summary><b>parts sourcing guide (w/ notes on pcb fabracation )</b> - start here if you are building fully from scatch or have purchased a pcb</summary>

i try to source all the parts i can from either:
- [tayda](https://www.taydaelectronics.com/) ; cheaper for common parts like resistors etc, also good for mechanical parts like switches and buttons
- [mouser](https://www.mouser.de/) ; has lots more options, speciality video ic's, can sometimes cost more (free shipping on orders over 50euros)
- other ; ocationally there will be parts which will need to be sourced elsewhere - usaully either aliexpress, ebay or amazon etc...

take a look at the [full_bom](/hardware/bom/full_bom.csv) for this project to see where i am sourcing each part from
  

## import into tayda

- go to the [tayda quick order](https://www.taydaelectronics.com/quick-order/) and in bottom corner choose _add from file_
- select the file [tayda_bom.csv](../hardware/bom/tayda_bom.csv) in the BOM folder (you will have to download it first or clone this repo)
- after importing select _add to cart_
- __NOTE:__ the minimum value for resistors is 10, so you may need to modify these values to add to cart (or if they are already modified here you will need to see the  full_bom for actual part QTY) 

- OPTIONAL: it is a good idea to add some dip-ic sockets and 2.54pin headers/sockets to your tayda order if you dont have them around already
  
## import into mouser

- go to [mouser bom tool](https://nz.mouser.com/Bom/) and click _upload spreadsheet_
- select the file [mouser_bom.csv](../hardware/bom/mouser_bom.csv) in this folder (you will have to download it first or clone this repo), then _upload my spreadsheet_ and _next_
- ensure that __Mouser Part Number__ is selected in the dropdown above the first row, then _next_, _process_
- if everything looks correct can now put _add to basket_

# ordering pcbs

you can support this project by buying individual pcbs from the [shop](https://underscores.shop). if you would rather have pcbs fabricated from gerbers directly the file you need is [here](/hardware/gerber_latest.zip) 

- i get my pcbs fabricated from [jlcpcb](https://cart.jlcpcb.com/quote) - 5 is the minumum order per design
- upload the zip file with the `add gerber file` button
- the default settings are mostly fine - set the __PCB Qty__ and __PCB Color__ settings (you can check that the file looks correct with pcb veiwer)
- it may be best to combine orders with other pcbs you want to have fab'd since the shipping can cost more than the items - also orginising group buys is a good way to distribute the extra pcbs /costs 
  
i often use jlcpcb because they are reliable, cheap and give you an option of colours. remember though that the cheapest Chinese fab houses are not always the most ethical or environmently friendly - if you can afford it consider supporting local companies. 

</details>

## assembly guide

<details><summary><b>assembly guide</b> - start here if you have purchased a diy kit</summary>
  

## interactive BOM for build guiding

follow this link to view the [interactive BOM](https://htmlpreview.github.io/?https://github.com/cyberboy666/recurBOY/blob/main/hardware/bom/ibom.html)
  


## general solder advices

- remember to heat pad first (2-3seconds), then add solder, then continue to heat (1-2seconds)

- Checkout the web-comic [soldering is easy](https://mightyohm.com/files/soldercomic/FullSolderComic_EN.pdf) for more soldering advice
  
## specific assembly advice

### Step 1: R1 - R4, D1 - D8
start by placing the resistors and diodes. it is important that the diodes are placed in the correct direction ! for resistors it does not matter.

### Step 2: J8, J6
there are two parts that need to be placed from the bottom. this is so the raspberry pi can be attached underneath. turn the board over and find j8 - the 2x20 pin socket will connect to the pi0 gpio pins. it is very important that these are soldered on straight. next to this is j6 – a double pin socket that will connect to the pi0 tv out.

### Step 3: U1, SW1-5, J9
flip back to the front. now you can solder the ic and buttons. use the ic socket. also try to make sure the 5-way button is straight before soldering all the pins

### Step 4: J5
for screen make sure the included 8x1 pin header is soldered to it first - with short pin end to the screen. then you can solder the screen - with long header pin ends through the recurBOY pcb.

### Step 5: J1-4, J7
next solder the top row of jacks and the RCA connector

### Step 6: RV1 - RV5
now you can solder the potentiometers.

you may need to also solder the 2x20 gpio-header to the raspberry pi and a 1x2 pin-header for the tv-out - then your raspberry pi should slot into the bottom of the pcb.

### flashing image to sd card

you can flash the recurBOY image to a sd card if you didnt get one from me, or to update its firmware.  download [the image here](https://s3.eu-central-1.wasabisys.com/underscores-storage/recurBoyFeb2020.img.gz) and install an image-flashing program like [etcher](https://www.balena.io/etcher/) - select the downloaded zip file and sdcard in the program and let it flash
  


</details>
  
## operating guide
  
<details><summary><b>operating guide</b> - start here if you have purchased an assembled unit</summary>

![image](https://user-images.githubusercontent.com/12017938/192133313-97dffbea-41c0-44d0-b04d-b71c0b3cf532.png)

### NAVIGATION

recurBOY has 2 source modes : `sampler` & `shaders` . pressing the __MODE__
button will cycle through these modes. you can tell which mode is selected by looking at the title and colour of the display.

### CONTENT SELECTION

the _nav_button_ can be pressed UP , DOWN , LEFT, RIGHT and IN. we will not use the IN button for now. pressing UP and DOWN lets you scroll through the list of content - either samples or shaders depending on the MODE. pressing SELECT on a row will start playing it. the playing content will be highlighted on the display and the ▶️ play symbol will display next to the MODE.

pressing the play/pause button will toggle this state. when the content is stopped the ⏹ will be displayed.

to safely turn off recurBOY hold down the play/pause button for 5 seconds. it is not recommended to remove your usb-drive while recurBOY is operating.

### Videos

the content list while in sampler MODE comes from the `~/Videos` folder on the pi's SD card and the `/Videos` folder on top level of an attached usb-drive. any .mp4, .mkv, .avi or .mov file will be shown although it is not guaranteed it will work with the player - we find sd h264 mp4 to be most reliable.

### Shaders

the content list while in the shaders MODE comes from the `~/Shaders` folder on the pi and in the `/Shaders` folder on top level of an attached device.

while the display is in SHADER mode you can use the 4 knobs or CV inputs to manipulate the shader parameters. each shader has 3 parameters mapped to input 0, 1 and 2. input 3 always controls the speed.

![image](https://user-images.githubusercontent.com/12017938/192133386-87cfa086-68dd-4a37-a210-a4cd647a51b1.png)

### Fx

from any source mode (SAMPLER or SHADERS) you can press the RIGHT nav_button to enter FX mode. This mode applies effects or filters to the media that is currently playing. Once you enter FX mode, you can navigate the same way as in SAMPLER or SHADERS with UP, DOWN and start the effect with SELECT. pressing LEFT will return to the previous screen.

these shaders need to be stored at `~/Fx` folder on the pi and `/Fx` folder on top level of attached usb drive. pressing the FX button will toggle the selected effect on and off. this effect will process whichever of the sources is selected.

the 4 knobs / cv inputs will control parameters of the effect when source SHADERS mode is not selected.

</details>

### more info

<details><summary><b>switching between pal and ntsc</b></summary>

- to switch the video format that your recurBOY uses, power down the device and put the sd card into a computer.
- now inside the `boot` partition look for a file called `config.txt`
- open it with any text editor and search for `sdtv_mode`
- here you can switch between `0` for _normal ntsc_ and `2` for _normal pal_
- save the file, safely eject the sd and put it back into the pi
- see [docs](https://www.raspberrypi.com/documentation/computers/config_txt.html#sdtv_mode) here for more settings

</details>

<details><summary><b>what are shaders anyway?</b></summary>
  
shaders are small text files of code that tell a graphics card what to draw. they use a language called glsl to communicate what colour a pixel should be and where. you don't have to understand every line to begin playing around with them.

in recurBOY we use shaders in two places - the SHADERS mode is used for launching shaders that generate video. these shaders take no video inputs. we also use shaders in the FX mode. here we are selecting shaders that process video. these take one video source as input and pass it through the fx shader.

the type of shader recurBOY can play is called GLES. this is the shader language used for embedded systems, including raspberry pi's and mobile phones.

![image](https://user-images.githubusercontent.com/12017938/192133557-cf8bfb17-41c3-4cf0-95d6-7501d25f494f.png)

thanks to [Erogenous Tones](http://erogenous-tones.com/) - a modular synth company who have a
mature and very powerful shader-playing video instrument called STRUCTURE - we now also have a web-based environment for browsing, modifying and creating shaders to perform with.

go to [glsl.erogenous-tones.com](https://glsl.erogenous-tones.com) - if you select any example you will see the code used to create the patch. try changing some of the numbers - modifying the input parameters is especially interesting.

when you are happy with the results select the save as .glsl file. Now copy this file onto your USB in the correct folder - /Shaders and connect to recurBOY

for more information on understanding and writing shaders yourself check out [The Book of Shaders](https://thebookofshaders.com/) by Gonzalez Vivo.
                                                                                                                             
</details>

<details><summary><b>how the circuit works</b></summary>
  
### reading button presses

we can use some of the GPIO pins on the raspberry pi to know when a button is pressed. one side of the button is connected to the pin and the other is connected to ground. on the raspberry pi we tell these pins to pull up. this means they are HIGH by default. when the button is pressed however the circuit connecting the pin to ground is completed and the pin becomes LOW. in the code we ask the state of a GPIO pin and if LOW we know the button is pressed.

![image](https://user-images.githubusercontent.com/12017938/192133647-47b9d445-207e-4996-8ebd-5e2926eca164.png)

### reading continuous inputs

digital pins - HIGH/LOW are good for discrete inputs like button presses which can only be OFF/ON. however we also want to have continuous inputs for example from knobs which can be set to any amount between LOW and HIGH. this kind of input is called analog - the reading is analogous to the voltage on the pin. since raspberry pi has no analog GPIO pins we need to introduce a new part - the MCP3008. this type of ic is called an analog to digital converter or adc for short. it has 8 analog pins which read the voltage applied to them and converts it to digital information. this information can be understood by the raspberry pi's digital pins. in this case using a digital protocol called SPI.

### potentiometers

four of the MCP3008 channels are connected to potentiometers which in this circuit act as voltage dividers. one side of the pot is connected to
+5V and the other to GND. The output is always some voltage between these. after passing through the adc this voltage value between GND and +5V is converted to a number between 0 and 1024

![image](https://user-images.githubusercontent.com/12017938/192133689-cf1db5b2-33bf-43de-b94f-3e4d7d4bf429.png)

### cv inputs

the other four MCP3008 channels are connected to 3.5mm jacks. this allows the voltage to be set by external devices - this kind of interaction between instruments is called Control Voltage or CV each of the CV inputs also use a resistor and two diodes - these are to protect the IC from incoming voltages above +5V or below GND

![image](https://user-images.githubusercontent.com/12017938/192133706-bb532af6-e862-46cb-bf3e-730c6b154864.png)

### display

the raspberry pi also connects to the display screen with GPIO pins - this time the pins are used as outputs, telling the screen which pixels to colour - again the protocol used here is SPI - but we dont need to worry about how exactly this works - there is a python library that is used to describe what the screen should show
                                                                                                                             
</details>

<details><summary><b>contributing guide</b></summary>
  
if you would like to contribute back to these projects in some way but dont know how the best thing (for now) would be to reach out to me directly ( tim@cyberboy666.com or @cyberboy666 on scanlines forum) - i will be happy to help
  
</details>


## credits & more info


This circuit is distributed through UNDERSCORES – open video hardware label – visit [underscores.shop](https://underscores.shop) for more info

The pcb was designed using KICAD

Everything from gerbers, cad files, panels and documentation is freely available online and distributed under CC-BY-SA / open-source licenses – help us contribute to the commons !

Ask any questions or start discussions related to this project on the [scanlines.xyz](https://scanlines.xyz) forum – an online community space dedicated to diy av / electronic media art

You can contact me directly at tim (at) cyberboy666 (dot) com 
Please get in touch if you are interested in hosting a workshop !

![image](https://user-images.githubusercontent.com/12017938/192133285-0cabdc30-9759-4a75-b5f9-2cd4231c50cf.png)


Thanks to Guergana Tzatchkova for all the love put into creating this project, and the design inspiration used across my work. to Bastien Lavaud for circuit advice, always. To Ben Caldwell for project advice. To everyone who has or will contribute ♥♥♥

