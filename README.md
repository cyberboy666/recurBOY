# recurBOY

__recurBOY__ is a raspberry pi zero based diy video-instrument for live performance.

![alt text](img/splash.gif "Splash Screen")

### motivation

it was designed to be built together with others in group workshop sessions.

for many people there is a barrier to enter the world of hardware video-art making instruments - both due to the general higher cost of video gear compared with audio equipment and in some cases its obsolescence and thus rarity.

this project aims to remove these financial barriers while enabling anyone interested to build their own art-making tools. by creating together we can learn from each other and help foster community in a physical space.

### features

- outputs composite video
- 2 source modes : _sampler_ and _shaders_
- process any source with additional _FX_
- control shader/fx parameters directly with 4x knobs or externally with 4x cv inputs

## BOM

REF | NAME | NUMBER | APROX COST 
--- | --- | --- | ---
na | [raspberry pi zero] | 1 | 5
na | custom pcb 100x100mm | 1 | 5
J5 | [1.8" screen] | 1 | 4
SW1-5 | [push buttons + caps from china] | 5 | 1 
RV1-4 | 10k linear pots | 4 | 3
J1-4 | [thonkicon jack] inputs | 4 | 2
U1 | [mcp3008 a2d dip] | 1 | 2
R1-4 | 1k resistors | 4 | 0.5
D1-8 | bat85 diodes| 8 | 2
J8 | 2x20 pin header | 1 in 1 out | 1
J7 | [composite video jack] | 1 | 1
na | [sd card 8gb] | 1 | 2 |
na | power-adapter | 1 |  5
J9 | [5-way push button] | 1 | 1.5

aprox total cost : 30euro plus the raspi0 - can get for 5euro from some distributors

additional parts:

- 1-pin header and socket for tv-out
- DIP8 ic socket
- usb-micro to USB-A socket to attach usb
- USB drive
- a rca cable
- a composite tv, display or mixer
- some M2 screws, nuts and spacers

[raspberry pi zero]: https://www.berrybase.de/raspberry-pi-zero-v1.3
[1.8" screen]: https://www.aliexpress.com/item/32996979276.html
[mcp3008 a2d dip]: https://www.aliexpress.com/item/32735896933.html
[push buttons + caps from china]: https://www.aliexpress.com/item/32826994795.html
[thonkicon jack]: https://modularaddict.com/pj301m12-jacks
[sd card 8gb]: https://www.aliexpress.com/item/33040093922.html
[composite video jack]: https://www.mouser.de/ProductDetail/CUI/RCJ-024
[5-way push button]: https://www.aliexpress.com/item/32845147449.html

