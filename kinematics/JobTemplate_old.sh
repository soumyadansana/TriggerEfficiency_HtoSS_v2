#!/bin/bash
echo "========================================="
ptr=$1
n1=$2
n2=$(expr $n1 + $3 - 1)
echo "First arg: $ptr"
echo "Second arg: $n1"
echo "Third arg: $n2"
root -l -q all_in_one_v4_singlemuon.C\($ptr,$n1,$n2\)
echo "========================================="
