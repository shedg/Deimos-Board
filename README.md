# Deimos-Board
A small and easy to use PID heater 

## OVERVIEW
The mod itself consist of a small circuit board with just a couple of components that is designed to accept an arduino nano board to control a powerfull mosfet, the arduino itself works as a PID controller and can be easily configured to use any passive thermistor (usually a glass bead PTC resistor used in FDM printers) and heater up to 150W, an LCD is used for a simple GUI and three push buttons are used to set the desired temperature.

### I/Os

-Input voltage is 12V typical, but can work at 24V (might need a little heatsink on one of the components).

-Output is a PWM signal with the value of the Input voltage, the board itself should be able to pilot loads up to 150W DC and still have some safety margin, typical value is 100W and that is plenty to heat the enclosure of the printer even 15 °C above ambient temperature with no problems.

-Optional always on terminal is straight up connected to the input voltage, ideally it's to use with a small fan to circulate the hot air in the enclosure.

-The thermistor Terminal is coupled with a resistor to ensure correct reading of the temperature, you can use any passive thermistor as long as you couple it with the correct value resistor, typically used is a PTC 100k glass bead thermistor coupled with a 100k ohm resistor, cheaply available as it's the standard FDM thermistor.




### Display

On the board these is a predisposition for standard character LCD, the firmware itself is prepared to use a 1602 LCD to write the temperature data, to use a backlit display you just need to connect the two terminals to the optional header in the back or for a cleaner solution to jump the on-board resistor and tap into the 5V display wire directly when making the display cable.

### User input and temperature set

The interface is simple: current temperature on the left and set temperature on the right of the display, pressing SET button will enter set-mode (a small arrow will flash near the set temperature) and you can use the UP and DOWN buttons to set the desired temperature in the enclosure, pressing SET again will exit set mode.
When booting up the set temperature is at a safe low value to ensure the heater won't be immediately turned on, that value can be set in the firmware to a different value if you wish to just turn the entire board on and off when needed with always the same setpoint.

###3D printed parts

A small enclosure for the board and the interfacing components need to be printed or made in some way to ensure the board itself is not exposed and to protect from unwanted shorts or damage, the small footprint of the board itself should make it an easy print job for any FDM printer or even slightly bigger resin printers and shouldn't take much space near the resin printer that you want to add this upgrade to. Additional prints or modification are required to securely hold the chosen heater inside the printer itself.


###Customization

The board itself is very modular, you can use a lot of different options for heaters, thermistors,buttons and display with some minor limitation aspect you have to consider for it to work. The core structure of the firmware makes it easy to change key parameters such as the thermistor values and you have the option of power-limiting the heater to a certain percentage or change the PID parameters for a slower or quicker response (only suggest that if you are sure of what you are doing).

###The kit itself

The board is populated by just a bunch of throughhole components, some screw terminals and for the display and arduino it actually uses pin-headers so nothing too difficult to manually solder and i’ll make a nice little schematic to ensure anyone can assemble one correctly. i can also prepare populated boards ready to go for a little extra or you can buy the PCB only and just get your components locally while checking the component list. For the heater and PSU you’ll have to find one yourself but they are easily available on amazon so it’s not a problem.

	Useful links
  [The component checklist](https://docs.google.com/spreadsheets/d/1y2S4blwJZZ-803kOBqK99VzKAkgP2FpXWgCPjR_RUzY/edit#gid=1386834576) 

  [Heater mount i use](https://www.thingiverse.com/thing:4305321)
