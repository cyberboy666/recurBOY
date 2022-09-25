# recurBOY

### a raspberrry pi zero video instrument

[3d rendered circuit image goes here]

- this circuit is distributed by __UNDERSCORES__ - _an open video hardware label_ : it is available to purchase - as a pcb, kit or assembled unit - at [underscores.shop](https://underscores.shop/_recurboy_/)
- the schematic for the circuit can be found [here](/hardware/schematic.pdf)
- the pcb gerber files for the lastest version can be found [here](/hardware/gerber_latest.zip)
- consider [donating](https://liberapay.com/underscores/) to the underscores project to help us continue creating for the commons

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


<details>

# documentation

this project is fully _open-source hardware_ - all the files required to build it are included in this repo for free. if you have the time and/or skill you can contribute back by collaborating on / testing new designs, improving these docs, making demo videos/other creative content etc. you can also support the project financially by [donating directing](https://liberapay.com/underscores/), or purchasing through the [web shop](https://underscores.shop).

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

follow this link to view the [interactive BOM](https://htmlpreview.github.io/?https://github.com/cyberboy666/recurboy/blob/main/hardware/bom/ibom.html)
  


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

![image](https://user-images.githubusercontent.com/12017938/158474325-491af7d0-d0d1-4446-a689-3c4a773478fd.png)


</details>

### more info

<details><summary><b>how the circuit works</b></summary>
  
[coming soon]
                                                                                                                             
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

![image](https://user-images.githubusercontent.com/12017938/158493552-49a106d9-8a07-45a7-9833-da2faddb7406.png)


Thanks to Guergana Tzatchkova for all the love put into creating this project, and the design inspiration used across my work. to Bastien Lavaud for circuit advice, always. To Ben Caldwell for project advice. To everyone who has or will contribute ♥♥♥

