#!/bin/sh

gcc -Wall -o /tmp/output "$1" 2> errors_warnings.txt

num_warnings=$(grep -c warning errors_warnings.txt)
num_errors=$(grep -c error errors_warnings.txt)

if [ "$num_errors" -eq 0 ] && [ "$num_warnings" -eq 0 ]; then
    score=10
elif [ "$num_errors" -ge 1 ]; then
    score=1
elif [ "$num_warnings" -gt 10 ]; then
    score=2
else
    score=$(echo "2 + 8*(10 - $num_warnings)/10" | bc)
fi

echo "$1: $score"

rm -f output errors_warnings.txt