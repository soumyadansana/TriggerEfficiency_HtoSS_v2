#!/bin/bash
echo "========================================="
ptr=$1
n1=$2
n2=$(expr $n1 + $3)
echo "First arg: $ptr"
echo "Second arg: $n1"
echo "Third arg: $n2"
echo "========================================="
