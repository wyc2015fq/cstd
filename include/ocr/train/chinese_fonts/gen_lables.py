
import glob  # glob 文件通配符模块
import os  # os 处理文件和目录的模块
import time
import random

root = 'E:/OCR_Line/chars/'
path = root+'fz-v4.0/'
cate = [path+x for x in os.listdir(path) if os.path.isdir(path+x)]
labels = []
for idx, folder in enumerate(cate):
    # glob.glob(s+'*.py') 从目录通配符搜索中生成文件列表
    for im in glob.glob(folder+'/*.jpg'):
        s = '%s;%d' % (im, idx)
        labels.append(s)

random.shuffle(labels)


outtxt = root+'list.txt'
f = open(outtxt, 'w')
for s in labels:
    print(s)
    f.write(s+'\n')

f.close()




