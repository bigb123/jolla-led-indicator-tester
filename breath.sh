#!/usr/bin/env bash

#
# Script to blink Jolla's led. Looks like breathing
#
# Parameters:
# 1 - path to 'leds_modificator' program

usage() {
  echo "Usage:"
  echo "$0 [-h] [path_to_leds_modificator]"
  echo
  echo "Where:"
  echo "  -h display this help"
  echo "  path_to_leds_modificator - path to 'leds_modificator' program. Not"
  echo "                             necessary if executing from the same path"
  echo "                             where the 'leds_modificator' is located"
}

# Change brightness of choosen led
# Parameters:
# 1 - mode:
#   - 'brighten'
#   - 'darken'
# 2 - path to 'leds_modificator' program
# 3 - led color:
#   '-r' - red
#   '-g' - green
#   '-b' - blue
change_brightness() {
  # Check what mode will be used
  if [ "$1" == 'brighten' ]; then
    range='{0..255}'
  else
    range='{255..0}'
  fi

  # Need to use 'eval' command to evaluate string rather than use this string
  for i in $(eval echo "$range"); do
    # 'leds_modificator' execution: path_to_program color brightness_value
    "$2" "$3" "$i"
  done
}

main() {
  if [ $# -eq 0 ]; then
    # If no parameters given make assumption that it will be executable
    # in the local environment
    leds_modificator_path="./leds_modificator"
  else
    leds_modificator_path="$1"
  fi

  # Parse help option
  while getopts ':h' opt ; do
    case $opt in
      h|\?)
        usage
        exit
        ;;
      esac
  done

  # Check if file exist
  if [ ! -x "$leds_modificator_path" ]; then
    echo "Trying to find program to change leds brigntness. Can't find file \
$leds_modificator_path or this file is not executable"
    usage
  fi

  # Execute brightness changing witt infinity loop
  while true; do
    # Reset brightness
    "$leds_modificator_path" --reset

    change_brightness "brighten" "$leds_modificator_path" "-r"
    change_brightness "darken" "$leds_modificator_path" "-r"
    change_brightness "brighten" "$leds_modificator_path" "-g"
    change_brightness "darken" "$leds_modificator_path" "-g"
    change_brightness "brighten" "$leds_modificator_path" "-b"
    change_brightness "darken" "$leds_modificator_path" "-b"
  done
}

main "$@"
