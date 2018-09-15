#!/bin/bash
trap "pkill -P $$ && exit" SIGTERM

for i in {1..5}
do
    aplay -q $1 &>/dev/null &
    wait
done
