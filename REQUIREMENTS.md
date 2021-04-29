# Requirements for stdpins.h

## Objectives

1. Define port and pin used for a function, easily changed later in the development process
2. Define polarity of a function, i. e. active high or active low, easily changed later
3. use port/pin/polarity definition for regular input/output
4. use port/pin/polarity for pin change interrupt applications
5. also be able to use 3rd party libraries such as VUSB or IRMP, which require specifying a port and pin as separate #defines, 
6. Refer to pins with alternate functions like RXD0 or OC1B in a manner that is independent of the controller type (e. g. on ATtiny85, OC0B is PB1, but on ATmega328, OC0B is PD5)

## Requirements

### Environment

- [x] `R001` usable in C programs
- [x] `R002` usable in C++ programs
- [x] `R003` usable in "classic" AVR projects (AtmelStudio)
- [x] `R004` usable in Arduino projects
- [x] `R005` usable for projects with ATmega µCs
- [x] `R006` usable for projects with ATtiny µCs

### Basic I/O

- [x] `R011` Assign information about port/pin/polarity to *one* symbol (preprocessor #define or variable)

- [x] `R012` Set output using defined port/pin/polarity
  - [x] `R012.1` set output to true or false or a variable (polarity aware)
  - [x] `R012.2` set output to high or low (not polarity aware)
- [x] `R013` Test input using defined port/pin/polarity
  - [x] `R013.1` test if input is true or false (polarity aware)
  - [x] `R013.2` test if input is high or low (not polarity aware)
- [x] `R014` Define data direction using defined port/pin, as one of input (pull-up undefined), input with pull-up, input without pull-up, output

### Alternate function names

- [x] `R021` define alternate function names that can be used like normal pin/port/polarity definitions, independent of controller type
  - [x] `R021.1` define names for UART pins
  - [x] `R021.2` define names for I2C pins
  - [x] `R021.3` define names for external interrupt pins (polarity aware)
  - [x] `R021.4` define names for timer PWM pins (polarity aware)

### Compatibility with other libraries

- [x]  `R031` extract port name like 'A', 'B', 'C' from port&pin definition
- [x]  `R032` extract bit number 0..7 from port&pin definition

### Support for pin change interrupts

- [x] `R041` calculate name of pin change interrupt mask register `PCMSKn` from pin/port/polarity definition
- [x] `R042` calculate name of pin change interrupt enable register `PCIEn` from pin/port/polarity definition
- [x] `R043` calculate name of pin change interrupt vector `PCINTx_vect` from pin/port/polarity definition
