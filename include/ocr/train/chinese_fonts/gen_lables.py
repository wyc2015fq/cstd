
import glob  # glob 文件通配符模块
import os  # os 处理文件和目录的模块
import time
import random

root = 'E:/OCR_Line/chars/'
path = root+'images/'
cate = [path+x for x in os.listdir(path) if os.path.isdir(path+x)]
labels = []
for idx, folder in enumerate(cate):
    # glob.glob(s+'*.py') 从目录通配符搜索中生成文件列表
    for im in glob.glob(folder+'/*.jpg'):
        s = '%s;%d' % (im, idx)
        labels.append(s)

random.shuffle(labels)


def savelines(labels, outtxt):
    f = open(outtxt, 'w')
    for s in labels:
        #print(s)
        f.write(s+'\n')
    f.close()


count = len(labels)
count_train = int(count*0.8)
count_test = count - count_train

outtxt = root+'list.txt'
savelines(labels[:count_train], root+'train.txt')
savelines(labels[count_train:], root+'test.txt')
