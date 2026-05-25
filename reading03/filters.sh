#!/bin/bash

q1_answer() {
    echo "Read the fine manual" | tr a-z A-Z
}

q2_answer() {
    echo "BTW, I use Arch" | sed 's/Arch/Ubuntu/'
}

q3_answer() {
	echo "     Yes, do as I say" | sed 's/^[[:space:]]*//'
}

q4_answer() {
	curl -sL https://yld.me/raw/yWh | grep ^root | cut -d ':' -f 7
}

q5_answer() {
	curl -sL https://yld.me/raw/yWh | grep 4[0-9]*7
}

q6_answer() {
	curl -sL https://yld.me/raw/yWh | sed -E 's/\/bin\/(sh|bash|csh|tcsh)/\/bin\/zsh/' | grep zsh
}
