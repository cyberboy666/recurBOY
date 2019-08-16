
i just want to start some notes to keep track and plan for the first recurBOY workshop in OCTOBER. this is a little over 8 weeks from now. this is plently of time to get code working but not much room for wiggle if sourcing parts and pcbs cheaply from china.

for aliexpress parts it would be best to allow 5-6weeks max - although most get here faster you never know. for ciruit-boards, cheap from seeed should take max 4 weeks - this is the biggest risk as i dont know how many revisions i might need. prioty circuits arrive within a week but cost MUCH MUCH more maybe 4x the price or more.

for ali-express some parts i can order now, some need to be tested first - i should try to do these asap
given the ali-parts are confirmed the next step is to get started on the circuit board to allow time for revisions - even if the software is not finished need to start with hardware - i want to aim to have first revision send away in less than 2 weeks from now - maybe try to do it this weekend

ofcourse if needed i can pay more to get things faster closer to the deadline - i would like to have decided a cutoff date early so i know when to act fast.

a table of parts ordered, tested, recieved etc to come here soon plus more dates and deadlines tbc

## parts and their status :

the status flow might look something like this: `test ordered` -> `test arrived` -> `test checked` -> `bulk ordered` -> `bulk arrived` -> `bulk checked`


REF | NAME | COST | STATUS
--- | --- | --- | ---
na | [raspberry pi zero] | 5 |  `test checked`
na | custom pcb 100x100mm | 1.8 | `test wip`
na | [smaller screen (1.8"?)] | 3.5 | `bulk ordered`
na | 10-12 [push buttons + caps from china] | 1  | `bulk arrived`
na | 4 pots for analog input | 3 | `test checked`
na | 4 [thonkicon jack] inputs | 1.5 | `some arrived need to count`
na | [a2d (mcp3008) from china] | 2 | `bulk ordered`
na | few resistors + diodes (8 bat85 + 4 1kohm) | 2 | `bulk diodes arrived awaiting resistors`
na | few pin headers | 0.5 | `bulk male arrived,bulk female ordered`
na | [composite video jack] | 1 | `bulk arrived`
na | [sd card 8gb] | 2 | `bulk ordered`
na | [power-adapter] |  4 |  `test ordered`
na | (optional) [raspi camera clone] | 7 | `bulk ordered` 
na | 5-way nav switch | 1.5 | `bulk ordered`

### 5 way push button

it might also be worth considering a [5-way push button] like this - saves on space , may make navigation more intuitave - prob will need a few more pins but that is ok `also bulk ordered`

### experiments to try:

some tech things to confirm:

- try getting the [1.8" display] running -> python first - maybe think about directly in of if time permits?
- reading button presses, direct and from matrix -> working with [pad4pi] package -> will then extend this to of

### things still to order

i still need to order the pi0s, they are close tho. waiting on the power supply to test how it works will the pi - then can order a bunch ...
also need to get resisters, pots, maybe a few thonkiconn jacks and pcbs ofcourse - hopefully things dont get lost !

## features

video-sampler and synthesizer : 

### 3 input modes:

- `sampler` : play videos from sd or usb - seamless ? how much control over player tbc - ie seeking, setting start/end points , seamless or not ? havnt quite decided on this yet
- `shader` : running frag-shaders from sd or usb - param inputs from 4 knobs and 4 cv in
- `capture` : live input from piCamera - can preview and record

### -> effects

any of the 3 inputs can be piped through an _effect_ shader.

[raspberry pi zero]: https://www.berrybase.de/raspberry-pi-zero-v1.3
[smaller screen (1.8"?)]: https://www.aliexpress.com/item/32996979276.html
[a2d (mcp3008) from china]: https://www.aliexpress.com/item/32735896933.html
[push buttons + caps from china]: https://www.aliexpress.com/item/32826994795.html
[thonkicon jack]: https://modularaddict.com/pj301m12-jacks
[sd card 8gb]: https://www.aliexpress.com/item/33040093922.html
[composite video jack]: https://www.mouser.de/ProductDetail/CUI/RCJ-024?qs=%2Fha2pyFadujC6XIlhTY7nF4RUCR%2FYibjfCLz8sPuiKglF9KHFnEXMg%3D%3D
[power-adapter]: https://www.aliexpress.com/item/32898334338.html
[raspi camera clone]: https://www.aliexpress.com/item/32825264717.html
[5-way push button]: https://www.aliexpress.com/item/32845147449.html
[1.8" display]: https://jakew.me/2018/01/19/st7735-pi/
[pad4pi]: https://github.com/brettmclean/pad4pi/
