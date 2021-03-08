#!/bin/bash

# Lab4: LOSS=0, CORRUPT=0
SPEED=1
DELAY=100
LOSS=0
CORRUPT=50
#BDP - to be modified when implementing sliding window
BDP=$((SPEED * DELAY))

killall link 2> /dev/null
killall recv 2> /dev/null
killall send 2> /dev/null

#echo "Removing input and output."
#rm input output
#echo "Creating input:"
#dd if=/dev/random of=input count=1400 bs=1 
#echo "Input created."
#echo ""

./link_emulator/link speed=$SPEED delay=$DELAY loss=$LOSS corrupt=$CORRUPT &> /dev/null &
sleep 1
./recv &
sleep 1

./send $BDP #input

#echo ""
#echo "Diff betwen input and output:"
#diff input output