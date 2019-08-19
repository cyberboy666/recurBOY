# recurBOY
a pi0 video-instrument designed for diy soldering workshops

## brainstorming repo

the finished repo may not be here - this is just a placeholder for brainstorming

## goal

to design a 'lite' version of r_e_c_u_r (raspberry pi based video-sampler) that can be built from a kit in a group workshop for under 30euro per unit

## hardware

currently i am thinking of using :

- [raspberry pi zero] - 5euro
- custom pcb 100x100mm - 1.8euro
- [smaller screen (1.8"?)] - 3.5euro
- 10-12 [push buttons + caps from china] - ~ 1euro

- 4 pots for analog input - 3euro from tayda (less if from china)
- 4 [thonkicon jack] inputs - 1.5euro 
- [a2d (mcp3008) from china] - 2euro

- few resistors + diodes (8 bat85 + 4 1kohm) - 2euro
- few pin headers ~ 0.5euro
- [composite video jack] (from mouser) - 1euro
- [sd card 8gb] (from china) - 2euro
- [power-adapter] ~ 4euro (if the ones from china work ?)

- (maybe) [raspi camera clone] - 7euro

__estimate total : ~ 27.5 + (optional 7 camera)__
(this includes postage on most things / assumes free for mouser etc ...)

## software

my idea is for the code to run completely in openframeworks. video-playing / shader-effects / capture input all are already running in my _ofxVideoArtTools_ repo. things still to do / check :
- reading from gpio pins in OF ; for mcp3008 and button matrix...
- writing to the gpio-screen from openframeworks ; it might need to use python as workaround here , im not sure ...
- video-looping / data-storing / ui navigating still to be written i guess

## layout idea

### initial brainstorm

![image](https://user-images.githubusercontent.com/12017938/60989363-1d87fb80-a346-11e9-8037-842e402947fb.png)

### revision0.1

![image](https://user-images.githubusercontent.com/12017938/63300041-8b292f00-c2d7-11e9-9079-4668646ccf98.png)

...


[raspberry pi zero]: https://www.berrybase.de/raspberry-pi-zero-v1.3
[smaller screen (1.8"?)]: https://www.aliexpress.com/item/32996979276.html
[a2d (mcp3008) from china]: https://www.aliexpress.com/item/32735896933.html
[push buttons + caps from china]: https://www.aliexpress.com/item/32826994795.html
[thonkicon jack]: https://modularaddict.com/pj301m12-jacks
[sd card 8gb]: https://www.aliexpress.com/item/33040093922.html
[composite video jack]: https://www.mouser.de/ProductDetail/CUI/RCJ-024?qs=%2Fha2pyFadujC6XIlhTY7nF4RUCR%2FYibjfCLz8sPuiKglF9KHFnEXMg%3D%3D
[power-adapter]: https://www.aliexpress.com/item/32898334338.html
[raspi camera clone]: https://www.aliexpress.com/item/32825264717.html
