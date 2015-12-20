# RadioHead
Packet Radio library for embedded microprocessors

This variation has been sincronized to one of the last distro but added SPI Transactions (completely absent before) and more compatibility with all Teensy's (3,3.1,LC).<br>
RadioHead library it's great but it's a <b>mess</b> to put hands on, takes hours digging code so don't complain!<br>
I have cloned because Paul Stoffregen's was seamlessy work but not totally SPI Transaction compatible and I got problems with other devices, also the original library grows and was updated but Paul's version not (I know Paul you have tons of better stuff to do!).<br>
In brief, the purpose of this variation it's better compatibility with Teensy 3.x when other SPI devices are used in the same time!<br>
Current testing and results with my TFT_ILI9163C display connected (examples in example folder):<br><br>

<b>This library:</b>____________ Teensy 3.0/3.2/LC_________RF23BP________Works!<br>
<b>Last official radiohead:</b>____Teensy 3.0/3.2/LC_________RF23BP________not working!<br>
