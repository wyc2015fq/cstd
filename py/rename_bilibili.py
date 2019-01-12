# https://www.bilibili.com/video/av19511740?from=search&seid=71078845766095419
# //*[@id="v_multipage"]/ul/a[3]

import re
import os
import Levenshtein
#  pip install python-Levenshtein

def initindexs(char, string):
    index = []
    length = len(string)
    for i in range(length):
        if char == string[i]:
            index.append(i + 1)  # 保存相同字符坐标+1的位置
    return index


def Substring(str1, str2):
    str1_len = len(str1)
    str2_len = len(str2)
    length = 0
    longest = 0
    startposition = 0
    start = 0
    for i in range(str1_len):
        start = i
        index = initindexs(str1[i], str2)
        index_len = len(index)
        for j in range(index_len):
            end = i + 1
            while end < str1_len and index[j] < str2_len and str1[end] == str2[index[j]]:  # 保证下标不会超出列表范围
                end += 1
                index[j] += 1
            length = end - start
            if length > longest:
                longest = length
                startposition = start

    return startposition, longest


def ttt_Substring():
    str1 = "pmcdcdfe"
    str2 = 'aoccddcdfe'
    Substring(str1, str2)
    (start, longest) = Substring(str1, str2)
    print(start, longest)
    for i in range(longest):
        print(str1[start + i], end=' ')


def getName1(l1, l2):
    l1copy = l1.copy()
    out = []
    print([len(l1), len(l2)])
    for i2, str2 in enumerate(l2):
        k = -1
        maxd = -100000
        ret = ''
        for i1, str1 in enumerate(l1copy):
            # (start, longest) = Substring(str1, str2)
            # longest = 10000 / (1 + Levenshtein.distance(str1, str2))
            longest = Levenshtein.ratio(str1, str2)
            d = longest
            if d > maxd:
                maxd = d
                ret = str1
                k = i1
                if len(str1) == 0:
                    print('asdf')

        if k >= 0:
            out.append(ret)
            del (l1copy[k])
            # out.append(ret + "( %d )" % maxd)
        else:
            out.append('')

    return out


def getMaxDistPair(l1, l2):
    maxd = -100000
    a1 = -1
    a2 = -1
    for i2, str2 in enumerate(l2):
        for i1, str1 in enumerate(l1):
            longest = Levenshtein.ratio(str1, str2)
            d = longest
            if d > maxd:
                maxd = d
                a1 = i1
                a2 = i2
    return a1, a2


def loadlist_chrome(fn, encoding):
    fp = open(fn, 'r', encoding=encoding)
    content = fp.read()
    fp.close()
    # content = u''
    l11 = re.sub(r'(\d+)、', r'\n\1、', content).split(u'\n')
    t = []
    l1 = []
    pos = 0
    t.append(content.find('1、'))
    for i in range(1, len(l11)):
        # print(str(i+1)+'、')
        t.append(content.find(str(i + 1) + '、', t[-1]))
        if t[-1] > 0:
            l1.append(content[t[-2]:t[-1]])
        else:
            l1.append(content[t[-2]:len(content)])
            break

    # print(t)
    return l1

def loadlist_firefox(fn, encoding):
    fp = open(fn, 'r', encoding=encoding)
    content = fp.read()
    fp.close()
    # content = u''
    l11 = content.split(u'\n')
    if len(l11)<3:
        return loadlist_chrome(fn, encoding)
    # print(t)
    l11 = [i.strip(' ') for i in l11]
    if '收起' in l11:
        l11.remove('收起')
    return l11

if 0:
    with open((os.path.join('rename_bilibili.txt')), 'rb') as f:
        data = f.read()
    flvpath = data.decode('utf-8')
    l1 = loadlist_firefox(flvpath + '/新建文本文档.txt', encoding='gbk')
    print(flvpath)
    print(l1)

def dirlist(flvpath):
    l2 = []
    for filename in os.listdir(flvpath):
        # print(filename)
        if filename.find('.flv')>0:
            l2.append(filename)
    return l2

def getName(l1, l2):
    l1copy = l1.copy()
    l2copy = l2.copy()
    out = []
    n = min(len(l1), len(l2))
    for i2 in range(len(l2)):
        out.append('')

    for i2 in range(n):
        a1, a2 = getMaxDistPair(l1copy, l2copy)
        if a1 >= 0:
            i = l2.index(l2copy[a2])
            out[i] = (l1copy[a1])
            del (l1copy[a1])

        del (l2copy[a2])

    return out

from prettytable import PrettyTable

def get_prettytable(l1, l2):
    x = PrettyTable()
    x.add_column("1", l1)
    x.add_column("2", l2)
    x.align["1"] = "l"
    x.align["2"] = "l"
    #xx = x.get_string(sortby="2", reversesort=False)
    xx = x.get_string()
    return xx

def renalllist(flvpath, l2, ss):
    (filepath, filename) = os.path.split(flvpath)
    for i2, str2 in enumerate(l2):
        if len(str2)>1:
            root, ext = os.path.splitext(str2)
            fn = ss[i2] + ext
            fn = filename + ' ' + ss[i2] + ext
            #print(flvpath+'/'+fn)
            fn = fn.replace(':', '-')
            fn = fn.replace('?', ' ')
            os.rename(flvpath+'/'+str2, flvpath+'/'+fn)


import clip
#print(clip.gettext().decode('gbk'))
import sys
if 1:
    flvpath = 'E:/_new/_ok1/考研计算机网络'
    flvpath = 'F:/kk/_unname/期权、期货及其他衍生产品'
    flvpath = '//169.254.233.118/kk/_ok/北京大学-操作系统原理'
    #with open((os.path.join('rename_bilibili.txt')), 'rb') as f:
    #    data = f.read()
    #flvpath = data.decode('utf-8')
    if len(sys.argv)>1:
        flvpath = sys.argv[1]
    else:
        flvpath = clip.gettext().decode('gbk').trip('\r\n')
    l1 = loadlist_firefox(flvpath + '/新建文本文档.txt', encoding='gbk')
    l2 = dirlist(flvpath)
    print(l1)
    print(l2)
    ss = getName(l2, l1)
    print(get_prettytable(l1, ss))
    renalllist(flvpath, ss, l1)
    print([len(l1), len(ss)])
    # exit(0)


# print(ss)

