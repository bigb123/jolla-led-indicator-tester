#!/usr/bin/env bash

#
# Script to blink Jolla's led. Looks like breathing
#
# Parameters:
# 1 - path to 'leds_modificator' program

SLEEP_DURATION=0

usage() {
  echo
  echo "Usage:"
  echo "$0 path_to_leds_modificator"
  echo
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
  if [ "$1" == 'brighten' ]; then
    range='{0..255}'
  else
    range='{255..0}'
  fi

  for i in $(eval echo "$range"); do
    # 'leds_modificator' execution: path_to_program color brightness_value
    "$2" "$3" "$i"
    sleep "$SLEEP_DURATION"
  done
}

main() {
  if [ $# -eq 0 ]; then
    usage
    exit
  fi

  while getopts ':h' opt ; do
    case $opt in
      h|\?)
        usage
        exit
        ;;
      esac
  done

  leds_modificator_path="$1"

  change_brightness "brighten" "$leds_modificator_path" "-r"
  change_brightness "darken" "$leds_modificator_path" "-r"
  change_brightness "brighten" "$leds_modificator_path" "-g"
  change_brightness "darken" "$leds_modificator_path" "-g"
  change_brightness "brighten" "$leds_modificator_path" "-b"
  change_brightness "darken" "$leds_modificator_path" "-b"
}

main "$@"
