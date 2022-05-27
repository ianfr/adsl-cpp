#!/bin/bash

cd ../datasets

unzip stocks_5_9-90.zip

find stocks_5_9-90/ -name '*.csv' | xargs rpl -q '$' ''
find stocks_5_9-90/ -name '*.csv' | xargs rpl -q 'N/A' '-1'
