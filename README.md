# RadioHead
Packet Radio library for embedded microprocessors

This variation has been sincronized to one of the last distro but added SPI Transactions (completely absent before) and more compatibility with all Teensy's (3,3.1,LC).<br>
RadioHead library it's great but it's a <b>mess</b> to put hands on, takes hours digging code so don't complain!<br>
I have cloned because Paul Stoffregen's was seamlessy work but not totally SPI Transaction compatible and I got problems with other devices, also the original library grows and was updated but Paul's version not (I know Paul you have tons of better stuff to do!).<br>
Apart SPI Transaction changes, RadioHead suffers of many internal small bugs, typically operations between INT or Words on uint8_t (that causes sometime wrong results), un-inited vars, etc.<br>
Why not propose patches to the author?<br>
First, he's not on github, this don't allow me to easily propose patches.<br>
Second, he's currently uses massive preprocessors stantements inside code (a LOT) to use his library with many MCU's, this is the main cause of problems, I had to add MCU's statements as well because my main purpose was maximum Teensy 3 compatibility but later I decided for SPI Transaction MCU's max compatibility (including Teensy's, AVR, DUE, ESP8266), I canno easily veryfy other systems like Raspi or different ARM's.<br>
In brief, the purpose of this variation it's better compatibility with Teensy 3.x when other SPI devices are used in the same time!<br>
Current testing and results with my TFT_ILI9163C display connected (examples in example folder):<br><br>

<b>This library:</b>____________ Teensy 3.0/3.2/LC_________RF23BP________Works!<br>
<b>This library:</b>____________ ESP8266_________RF23BP________Compile (not tested)<br>
<b>This library:</b>____________ Teensy 3.0/3.2/LC_________RNF24________Works<br>
<b>This library:</b>____________ Teensy 3.0/3.2/LC_________RF96________Works first time, then stops (library init?)<br>
<b>Last official radiohead:</b>____Teensy 3.0/3.2/LC_________RF23BP________not working!<br>
<b>Last official radiohead:</b>____ESP8266_________RF23BP________not working!<br>
<b>Last official radiohead:</b>____________ Teensy 3.0/3.2/LC_________RNF24________Compile but not works<br>
<b>Last official radiohead:</b>____________ Teensy 3.0/3.2/LC_________RF96________Not working at all...<br>
Here's my test setup, I have 2 host board with Teensy and a couple of piggyboard for each rf so I can easily exchange modules by using the same pins. Each RF module piggyboard has onboard power supply and filters<br>

