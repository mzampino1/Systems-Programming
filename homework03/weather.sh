#!/bin/sh

# Globals

URL="https://forecast.weather.gov/zipcity.php"
ZIPCODE=46556
FORECAST=0
CELSIUS=0

# Functions

usage() {
    cat 1>&2 <<EOF
Usage: $(basename $0) [zipcode]

-c    Use Celsius degrees instead of Fahrenheit for temperature
-f    Display forecast text after the temperature

If zipcode is not provided, then it defaults to $ZIPCODE.
EOF
    exit $1
}

weather_information() {
    # Fetch weather information from URL based on ZIPCODE
	curl -sL $URL?inputstring=$ZIPCODE
}

temperature() {
    # Extract temperature information from weather source
	if [ $CELSIUS -eq 0 ]; then
    	weather_information | grep -Eo '"myforecast-current-lrg">((-[0-9]+)|([0-9]+))&deg;F' | grep -Eo '(-[0-9]+)|([0-9]+)'
	else
		weather_information | grep -Eo '"myforecast-current-sm">((-[0-9]+)|([0-9]+))&deg;C' | grep -Eo '(-[0-9]+)|([0-9]+)'
	fi
}

forecast() {
    # Extract forecast information from weather source
    weather_information | grep -Eo '"myforecast-current">[[:alpha:][:space:]]*</p>' | grep -Eo '>[[:alpha:][:space:]]*<' | tr -d '><' | sed -e 's/^ *//' -e 's/ *$//'
}

# Parse Command Line Options

while [ $# -gt 0 ]; do
    case $1 in
		-c) CELSIUS=1;;
		-f) FORECAST=1;;
        -h) usage 0;;
         [0-9][0-9][0-9][0-9][0-9]) ZIPCODE=$1;;
		 *) usage 1;;
    esac
    shift
done

# Display Information
echo "Temperature: $(temperature) degrees"
if [ $FORECAST -eq 1 ]; then
	echo "Forecast:    $(forecast)"
fi
