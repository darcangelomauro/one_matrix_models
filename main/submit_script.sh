#!/bin/bash

# Check that level 0 seed has been passed as argument
if [ -z "$1" ]
then
	echo "Need to pass level 0 seed"
	return
fi

# Check that work directory has been passed as argument
if [ -z "$2" ]
then
	echo "Need to pass directory name"
	return
fi

# Check that the work directory actually exists
if [ ! -d $2 ]
then
	echo "Directory doesn't exist"
	return
fi

# Define some variables
SEED_LVL0=$1
HERE=$PWD
WORK_PATH=$2
INIT_FILE=$WORK_PATH/init.txt
G2_FILE=$WORK_PATH/g2.txt

# Check that init file actually exists
if [ ! -f $INIT_FILE ]
then
	echo "Init file doesn't exist"
	return
fi

# Check that g2 file actually exists
if [ ! -f $G2_FILE ]
then
	echo "g2 file doesn't exist"
	return
fi


# Cycle on g2 values
COUNTER=0
while read -r G2_VAL
do
	# Create g2 directory
	G2_STRING=$(echo "$G2_VAL" | tr - n | tr . d)
	mkdir $WORK_PATH/$G2_STRING
	
	LOG_FILE=$WORK_PATH/$G2_STRING.log

	# Compute Level 1 seed
	SEED_LVL1=$((SEED_LVL0+COUNTER))
	
	# Submit job
	nohup ./simulation $SEED_LVL1 $G2_VAL $WORK_PATH > $LOG_FILE &

	# Increment counter
	COUNTER=$((COUNTER+1))
done < $G2_FILE