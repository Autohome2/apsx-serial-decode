# apsx-serial-decode
APSX lambda d1 board serial decoder

This project runs on an arduino mega2560

It connects to the APSX D1 digital output via pin 12

if you have a APSX G1 LED connect this as normal to the APSX D1.

Connecting via Serial0 you will see messages of the data information that the D1 is sending to the LED.

This allows you to program the D1 if you dont have a LED or other APSX Display.

The code is also setup to drive a 4x20 i2c lcd (with backpack expander) but could essily be changed for  different display.
The display shows the current AFR and messages.
