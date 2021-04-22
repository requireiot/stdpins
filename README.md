stdpins.h
=========
Macros to define AVR port pins as input or output, and set/clear (polarity aware).

Introduction
------------

In embedded projects with ATmega or ATtiny microcontrollers (and maybe others), assignment of port pins to functions may change multiple times, e.g. when you start working on your PCB layout and learn that it would really be easier if the LED was connected to PC2 instead of PB5 ...

This simple header file allows you to map a logical *pin* to a port and bit number, and define polarity (active high or active low), in *one* place, and then refer to that definition throughout your code.
	
Say we have an LED connected to PB5, and the light is on if PB5=Low
``` C
#define led B,5,ACTIVE_LOW 
```
Somewhere else in your code, you configure the pin as output
``` C
AS_OUTPUT(led);
```
turn the LED on for a while
``` C
ASSERT(led); 
_delay(100); 
NEGATE(led); 
```
If the schematics change and LED is now connected to PC2, and turns on if PC2=Hi, just change the definition to
``` C
#define led C,2,ACTIVE_HIGH
```
Everything else remains the same.
	
Inspired by http://www.starlino.com/port_macro.html

Defining a pin
--------------
The general format is 
<pre>#define <i>name</i> <i>port</i>,<i>pin</i>,<i>polarity</i></pre>
where *port* is one of `A`,`B`,`C`,etc., *pin* is in the range 0-7, and *polarity* is `ACTIVE_LOW` or `ACTIVE_HIGH`

Configuring a pin
-----------------
Use these macros to configure a pin as output or input (with or without pull-up)

| Macro               | Direction | pull-up   |
| ------------------- | --------- | --------- |
| AS_INPUT(pin)       | in        | undefined |
| AS_INPUT_PU(pin)    | in        | enabled   |
| AS_INPUT_FLOAT(pin) | in        | disabled  |
| AS_OUTPUT(pin)      | out       | n/a       |

For example, this would configure pin PC2 as input and enable the pull-up resistor:
```
#define BUTTON C,2,ACTIVE_LOW
AS_INPUT_PU(BUTTON);
```

Pre-defined pins
----------------
Several pins used by AVR peripherals are "pre-defined", depending on the microcontroller model. So, for an ATmega328, the symbol `_I2C_SDA` is defined as
```C
#define _I2C_SDA	C,4,ACTIVE_HIGH
```
for an ATtiny2313, it is defined as
```C
#define _I2C_SDA	B,5,ACTIVE_HIGH
```
They can be used in your code as if you had defined them
```C
// pulse low the I²C clock line
SET_LOW(_I2C_SCL);
_delay(100);
SET_HIGH(_I2C_SCL);
```

When defining your logical pins, you can refer to the controller-epcific pin assignment, and just specify a polarity:
```C
#define led _OC0A(ACTIVE_LOW)
#define button _INT0(ACTIVE_LOW)
AS_OUTPUT(led);
AS_INPUT_PU(button);
```

See header for more details.

Testing input pins
------------------
Use these macros to test an input pin:
* `ÌS_TRUE(pin)` returns `true` if the pin is *active*, i.e. high for an `ACTIVE_HIGH` pin or low for an `ACTIVE_LOW` pin
* `ÌS_FALSE(pin)` returns `true` if the pin is *not active*, i.e. low for an `ACTIVE_HIGH` pin or high for an `ACTIVE_LOW` pin
* `ÌS_HIGH(pin)` returns `true` if the pin is *high*, irrespective of polarity
* `ÌS_LOW(pin)` returns `true` if the pin is *low*, irrespective of polarity

For example, if BUTTON is defined as
``` C
#define BUTTON C,2,ACTIVE_LOW
```
and the voltage at pin PC2 is currently 0 Volt, then
* `IS_TRUE(BUTTON)` returns `true`
* `IS_FALSE(BUTTON)` returns `false`
* `IS_HIGH(BUTTON)` returns `false`
* `IS_LOW(BUTTON)` returns `true`

Setting output pins
-------------------
Use these macros to set the state of an output pin:
* `ASSERT(pin)` sets the output to *active*, i.e. high for an `ACTIVE_HIGH` pin or low for an `ACTIVE_LOW` pin
* `NEGATE(pin)` sets the output to *not active*, i.e. low for an `ACTIVE_HIGH` pin or high for an `ACTIVE_LOW` pin
* `SET_HIGH(pin)` sets the output to *high*, irrespective of polarity
* `SET_LOW(pin)` sets the output to *low*, irrespective of polarity
* `TOGGLE(pin)` flips the output
* `SET_PA(pin,value)` sets the output based on expression `value`, i.e. sets it to *active* if `value!=0` or to *not active* if `value==0`

Direct access to pin definition
-------------------------------
Sometimes, you will need to convert your pin definition to a "classic" pin name like PC4, oryou need direct access to the port register or data direction register. Use these macros to get that access:

| Macro          | Description                  | Example   |
| -------------- | ---------------------------- | ------- |
| PORT           | Name of port output register | PORTC   |
| PIN            | Name of port input register  | PINC    |
| DDR            | Name of data direction reg.  | DDRC    |
| BV             | Bit mask for pin             | 0x08    |
| portNAME       | Letter of port               | C       |
| portBIT        | Bit number                   | 3       |

The example is what you get if the pin has been defined as 
```C
#define mypin C,3,ACTIVE_HIGH
```

Pin change interrrupts
----------------------
These pin definitions can also be used in conjunction with pin change interrupts (in controllers that support this feature).

Say you have connected a button to input PC2 of your ATmega328:
```C
#define button C,2,ACTIVE_LOW
AS_INPUT_PU(button);
```
To get pin change interrupts from this button, you need to enable the bank of pin change interrupts for the port, and then enable the individual pin change interrupt
```C
PCI_ENABLE(button)
PCIEx_ENABLE(button)
```
since `button` is currently defined as PC2, this translates to
```C
PCICR |= 1 << PCIE1
PCIMSK1 |= 1 << 2
```
If, later in your project, you decide to re-assign the button to PD7
```C
#define button D,7,ACTIVE_LOW
```
The `PCI_ENABLE`and `PCIEx_ENABLE` macros above are now automatically expanded to
```C
PCICR |= 1 << PCIE2
PCIMSK1 |= 1 << 7
```
Your interrupt service routine could look like this
```C
PCI_ISR(button) {
    // ... do something
}
```

There are also matching `PCI_DISABLE(pin)`and `PCIEx_DISABLE(pin)` macros.

All of this works well if you use only *one* pin change interrupt per port. If you want to use more than one pin change interrupt on the same port, the refer to the ATmega datasheet to understand how these interrupts are enabled and disabled in groups.