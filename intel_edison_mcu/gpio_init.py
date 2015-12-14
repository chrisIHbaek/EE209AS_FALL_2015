#!/usr/bin/env python

# Author: Xiangrui Liu
# This python script is used to call init_DIG.sh to setup Edison board 
# to use DIG5 to DIG8 as GPIOs in our project 

import os

os.system("./init_DIG.sh -o 5 -d input")
os.system("./init_DIG.sh -o 6 -d input")
os.system("./init_DIG.sh -o 7 -d input")
os.system("./init_DIG.sh -o 8 -d input")