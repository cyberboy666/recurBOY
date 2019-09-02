
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
na | [raspberry pi zero] | 5 |  `bulk ordered`
na | custom pcb 100x100mm | 1.8 | `r0.1 ordered`
na | [smaller screen (1.8"?)] | 3.5 | `bulk ordered`
na | 10-12 [push buttons + caps from china] | 1  | `bulk arrived`
na | 4 pots for analog input | 3 | `mouser order standby`
na | 4 [thonkicon jack] inputs | 1.5 | `bulk ordered`
na | [a2d (mcp3008) from china] | 2 | `bulk arrived`
na | few resistors + diodes (8 bat85 + 4 1kohm) | 2 | `bulk diodes arrived awaiting resistors mouser standby`
na | few pin headers | 0.5 | `bulk arrived`
na | [composite video jack] | 1 | `bulk arrived`
na | [sd card 8gb] | 2 | `bulk arrived`
na | [power-adapter] |  4 |  `bulk ordered`
na | (optional) [raspi camera clone] | 7 | `bulk ordered` 
na | 5-way nav switch | 1.5 | `bulk arrived`

### 5 way push button

it might also be worth considering a [5-way push button] like this - saves on space , may make navigation more intuitave - prob will need a few more pins but that is ok `also bulk ordered`

### experiments to try:

some tech things to confirm:

- try getting the [1.8" display] running -> python first - maybe think about directly in of if time permits?
- reading button presses, direct and from matrix -> working with [pad4pi] package -> will then extend this to of

### things still to order

~~i still need to order the pi0s~~, it is not possible to bulk order pi0s - people will have to bring their own.. waiting on the power supply to test how it works will the pi 

things still left to order :
 - pots and resisitors i will get from mouser i think
 - will get other set of thonkiconn jacks (either from modular or otherwise..)
 - decide and order a cheap - ish power supply that will arrive on time ... (the one from ali i think will be too slow)

## features

video-sampler and synthesizer : 

### 3 input modes:

- `sampler` : play videos from sd or usb - seamless ? how much control over player tbc - ie seeking, setting start/end points , seamless or not ? havnt quite decided on this yet
- `shader` : running frag-shaders from sd or usb - param inputs from 4 knobs and 4 cv in
- `capture` : live input from piCamera - can preview and record

### -> effects

any of the 3 inputs can be piped through an _effect_ shader.

# final pcb order deadline.

i have some stress about getting pcbs fabbed and delivered in time for this workshop. flight from berlin is oct 4th.

with fast shipping would still need to allow 14 days to get pcbs fabbed and arrive, which means the final cutoff for a second revision of the board is 18th september. since first revision hasnt been shipped yet, and is with slow post , it would need to get here at most in 3 weeks (this would give me a few days to do the soldering and make any needed revisions)
i think this is possible but also not so likely. also will need to aleast have enough parts to do this test build before mid september. 

what other options are there ? possibly to use the revision1 boards if possible (but then there is only 10 in total - one of which i will need to use as a test board but maybe can give away at the workshop ... )

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
