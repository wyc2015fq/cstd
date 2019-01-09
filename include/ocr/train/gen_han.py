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

f = open("汉字书写大赛参考----6763字频表(基数10亿).md", "rb")

lines = f.readlines()             # 调用文件的 readline()方法
#lines = lines[2:10]
ss = ''
for line in lines:
	strs = line.split(b'|', 8)
	s = str(strs[2], encoding='utf-8')
	ss = ss + s.strip()
	#print(s) # 在 Python 3中使用

print(ss)
f.close()
