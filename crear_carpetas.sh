#!/bin/bash 
for i in $(seq 1 2); do 
  mkdir "carpeta$i"
  mv "archivo$i.cpp" "carpeta$i/" 
done;

