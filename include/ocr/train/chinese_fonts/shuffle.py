
from gen_utils import *
from allcharset import *

if 1:
    #random.shuffle(fontfiles)
    #fontfiles = fontfiles[:200] + ['huawenxihei.ttf', 'fz-v4.0.ttf']
    outpath = 'D:/OCR_Line/lines/han200w'


def savelines(labels, outtxt):
    f = open(outtxt, 'w')
    for s in labels:
        # print(s)
        f.write(s+'\n')
    f.close()



def loadlines(txtfn):
    f = open(txtfn, 'r')
    labels = f.readlines()
    f.close()
    for i in range(0, len(labels)):
        aa = labels[i]
        bb = aa.split(" ")[0:-1]
        cc = " ".join(bb)
        labels[i] = cc
    return labels

imgoutpath = outpath+'/img'
root = outpath + '/'

labels = loadlines(root + '/infos.txt')
if 1:
	count = len(labels)
	count_train = int(count*0.8)

	root = outpath + '/'
	outtxt = root+'list.txt'
	savelines(labels[:count_train], root+'train.txt')
	savelines(labels[count_train:], root+'test.txt')

	labels = ['blank']

