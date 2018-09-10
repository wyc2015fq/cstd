#! /usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw
import pickle
import argparse
from argparse import RawTextHelpFormatter
import fnmatch
import os
import cv2
import json
import random
import numpy as np
import shutil
import traceback
import copy


# 注意，chinese_labels里面的映射关系是：（ID：汉字）
def get_label_dict():
    f=open('./chinese_labels','rb')
    label_dict = pickle.load(f)
    f.close()
    return label_dict

label_dict = get_label_dict()
char_list=[]
value_list=[]
f=open('./label.txt','w')
for (value,chars) in label_dict.items():
    chars = chars.strip('\r\n')
    print(value,chars)
    char_list.append(chars)
    value_list.append(value)
    f.write(chars+'\n')

f.close()