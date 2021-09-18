#!/bin/bash

rm db
bash data-compile.sh && ./db &
rm main 
bash main-compile.sh && ./main
