#!/bin/bash
for i in $(seq 1 10); do 
  seq $i > "archivo$i.cpp"
done;
