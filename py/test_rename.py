import os
import re
import sys

path = 'D:/Youku Files/transcode'
old_names = os.listdir(path)

for old_name in old_names:
    rn = old_name.replace('[饭拍] ', '').replace('[饭拍]', '')
    print(rn)

# print(l)



