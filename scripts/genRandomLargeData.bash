#!/bin/bash

BIT_32_DATA="./1m-32-bit-data.data"
BIT_64_DATA="./1m-64-bit-data.data"
MAX_LOOP=1000000

if [ -f "$BIT_32_DATA" ]; then
    echo "Old copy of $BIT_32_DATA detected, deleting."
    rm "$BIT_32_DATA"
fi
if [ -f "$BIT_64_DATA" ]; then
    echo "Old copy of $BIT_64_DATA detected, deleting."
    rm "$BIT_64_DATA"
fi

echo "Generating 1m 32 bit signed numbers"
# Generate random numbers and append to the file
for i in $(seq 1 $MAX_LOOP); do
    # Calculate percentage
    percent=$((i * 100 / MAX_LOOP))

    # Print progress bar
    bar=""
    for ((j = 0; j < percent / 2; j++)); do
        bar="${bar}#"
    done
    # Ensure the progress bar is exactly 50 characters wide
    printf "\r[%s%s] %d%%" "$bar" $(printf "%-50s" "") "$percent"
    RAND_NUM_STR=$(od -A n -t dL -N 4 /dev/urandom | tr -d ' ')

    # Bash natively handles 64-bit integers on 64-bit systems.
    # We explicitly specify the number is a long decimal for safety in the next step.
    # This variable now holds the raw 64-bit unsigned integer value.
    UNSIGNED_32BIT=$RAND_NUM_STR

    # To treat it as signed, we need to manually implement two's complement logic
    # for the most significant bit.
    # The maximum positive signed 64-bit integer is 2^63 - 1.
    MAX_SIGNED=$(( 2**31 - 1 ))

    # Check if the generated number is larger than the max positive signed number.
    if [[ $UNSIGNED_32BIT -gt $MAX_SIGNED ]]; then
        # If it is, subtract 2^64 to get the negative representation.
        # 2^64 is too large for bash, so we do subtraction in two parts:
        # 2^64 = 2 * 2^63
        SIGNED_32BIT=$(( UNSIGNED_32BIT - 2 * 2**31 ))
    else
        SIGNED_32BIT=$UNSIGNED_32BIT
    fi

    echo "$SIGNED_32BIT" >> $BIT_32_DATA
done

echo "\nGenerating 500k 64 bit signed numbers"
# Generate random numbers and append to the file
for i in $(seq 1 $MAX_LOOP); do
    # Calculate percentage
    percent=$((i * 100 / MAX_LOOP))

    # Print progress bar
    bar=""
    for ((j = 0; j < percent / 2; j++)); do
        bar="${bar}#"
    done
    # Ensure the progress bar is exactly 50 characters wide
    printf "\r[%s%s] %d%%" "$bar" $(printf "%-50s" "") "$percent"
    RAND_NUM_STR=$(od -A n -t dL -N 8 /dev/urandom | tr -d ' ')

    # Bash natively handles 64-bit integers on 64-bit systems.
    # We explicitly specify the number is a long decimal for safety in the next step.
    # This variable now holds the raw 64-bit unsigned integer value.
    UNSIGNED_64BIT=$RAND_NUM_STR

    # To treat it as signed, we need to manually implement two's complement logic
    # for the most significant bit.
    # The maximum positive signed 64-bit integer is 2^63 - 1.
    MAX_SIGNED=$(( 2**63 - 1 ))

    # Check if the generated number is larger than the max positive signed number.
    if [[ $UNSIGNED_64BIT -gt $MAX_SIGNED ]]; then
        # If it is, subtract 2^64 to get the negative representation.
        # 2^64 is too large for bash, so we do subtraction in two parts:
        # 2^64 = 2 * 2^63
        SIGNED_64BIT=$(( UNSIGNED_64BIT - 2 * 2**63 ))
    else
        SIGNED_64BIT=$UNSIGNED_64BIT
    fi

    echo "$SIGNED_64BIT" >> $BIT_64_DATA
done

echo "\nGenerated 1m 64 bit signed numbers"
