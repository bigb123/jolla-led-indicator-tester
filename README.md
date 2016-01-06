# jolla-led-indicator-tester

Program to test led indicator in Jolla phone.

## Dependencies
You need to have developer mode on your phone turned on, because the program is terminal-based and you need to have administrator privileges to set leds' brightness. Also remember to install stuff needed to compile this program. As root run:
```
pkcon install make gcc
```

## Compiling
Execution of command
```
make
```
will produce <b>leds_modificator</b> executable program.

## Execution
Example:<br>
After execution
```
leds_modificator -r 100
```
you will see that your led shines red.

## Usage

```
leds_modificator [[-r 0-255] [-g 0-255] [-b 0-255]] [-t] [-v] [-h]

Program to test RGB led indicator in Jolla phone

Usage: leds_modificator options [value (from 0 to 255)]
where: 
  -h  --help              display this help message
  -r  --red               set red led brightness
  -g  --green             set green led brightness
  -b  --blue              set blue led brightness
  -t  --test              test all leds turning them on and off one by one
  -i  --info-values       inform about actual setted brightness
  -0  --reset             set all led values to 0
```

## Observed issues
In test mode when phone enters in energy-saving mode (the screen goes black) led can hang for a while. Turn on screen with tapping or clicking on/off button to go back to normal mode.