dear diary: 

:P

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
na | custom pcb 100x100mm | 1.8 | `r0.2 ordered`
na | [smaller screen (1.8"?)] | 3.5 | `bulk arrived`
na | 10-12 [push buttons + caps from china] | 1  | `bulk arrived`
na | 4 10k pots for analog input | 3 | `bulk arrived`
na | 4 [thonkicon jack] inputs | 1.5 | `bulk arrived`
na | [a2d (mcp3008) from china] | 2 | `bulk arrived`
na | few resistors + diodes (8 bat85 + 4 1kohm) | 2 | `bulk arrived`
na | few pin headers | 0.5 | `bulk arrived`
na | [composite video jack] | 1 | `bulk arrived`
na | [sd card 8gb] | 2 | `bulk arrived`
na | [power-adapter] |  4 |  `bulk arrived`
na | (optional) [raspi camera clone] | 7 | `bulk arrived` 
na | 5-way nav switch | 1.5 | `bulk arrived`

### 5 way push button

~~it might also be worth considering a [5-way push button] like this - saves on space , may make navigation more intuitave - prob will need a few more pins but that is ok `also bulk ordered`~~

### experiments to try:

some tech things to confirm:

- try getting the [1.8" display] running -> python first - maybe think about directly in of if time permits?
- reading button presses, direct and from matrix -> working with [pad4pi] package -> will then extend this to of

### problem with sourcing the pi0s cheap

~~i still need to order the pi0s~~, ~~it is not possible to bulk order pi0s~~ - ~~people will have to bring their own..~~ ~~waiting on the power supply to test how it works will the pi~~ i ordered some _too cheap to be true_ pi0s on ebay - lets see if they arrive / work ? the backup plan is to buy enough pi0s at the bulk price : around 12euros, and give people at the workshop the choice out of a 12euro pi0 now from me or a 5euro pi0 later they can buy themself ... 

things still left to order :
 - ~~pots and resisitors i will get from mouser i think : waiting till i test some other parts before ordering these incase the cheaper chips (for example) dont work~~
 - ~~will get other set of thonkiconn jacks~~ : ordered from modular-addict
 - ~~decide and order a cheap - ish power supply that will arrive on time ... (the one from ali i think will be too slow)~~ ordered 10 5euro powersupplys from ebay - should arrive by beginning of next week - i hope they work !!


# final pcb order deadline.

i have some stress about getting pcbs fabbed and delivered in time for this workshop. flight from berlin is oct 4th.

with fast shipping would still need to allow 14 days to get pcbs fabbed and arrive, which means the final cutoff for a second revision of the board is 18th september. since first revision hasnt been shipped yet, and is with slow post , it would need to get here at most in 3 weeks (this would give me a few days to do the soldering and make any needed revisions)
i think this is possible but also not so likely. also will need to aleast have enough parts to do this test build before mid september. 

what other options are there ? possibly to use the revision1 boards if possible (but then there is only 10 in total - one of which i will need to use as a test board but maybe can give away at the workshop ... )

## first r0.1_board problem:

currently r0.1 of the board has the _display_ on pins for __SPI0__ and the _a2d_ on pins for __SPI1__. however it seems the a2d code can only read from __SPI0__ , whereas the _display_ can read from both. a easy hardware fix is to swap these pins around on the scematic/layout however as mentioned above there may not be much time for a second revision of the board to arrive. if everything else on r0.1 boards are ok another backup plan is to manually cut and rewire traces on the r0.1 boards for the workshop.

## r0.1 boards arrived !

this took less time than i expected. i soldered the board together in 70mins. i think the workshop has two 3hour slots so hopefully this will be enough time for everyone. everything worked together suprizingly well. the only real problem is the spi1/spi0 problem i spotted earlier. otherwise the display, push buttons, a2d etc all are working ! woop (need to test the composite video output)

i will create a new revision of the board, some changes include:
- switching the spi ports around
- slightly resizing the button footprint
- labelling buttons with the silk-screen
- moving button placement around a little
- maybe moving resistor/diode placements around a little
- ordering more than 10 this time with fast postage

i also will order the mouser parts to make sure we have everything and enough spare parts.

some testing for example, the display, the a2d, the other parts i used in the build all worked...

## some performance problems

we have yet to test all the parts together on a pi0 but i am antisipating some performance optimizations needed.
some things to try:
- updating screen less / only when something changes
- reading from the a2d less
- not running the ui desktop at all - launch everything from console
- experimenting with different gpu/cpu splits

## update 20/09

besides the pi0s themself - which havnt arrived - will order some more this weekend - everything else should be here now - only the circuitboards which should arrive soon. there is still lots to do including finsihing the code, testing on a pi0, and making the build booklet



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
