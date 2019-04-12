
from down import *

url='https://yuhui.blog.csdn.net/article/list/2?'

if 0:
    data = getdata(url)
    html = etree.HTML(data)
    aa = '//div/@data-articleid'
    li = html.xpath(aa)
    print((li))
    print(len(li))
    print(len(data))
    #save_txt('aaa.txt', data)
    ##print(data)

def getlist(url):
    ll=[]
    i=1
    while(1):
        data = getdata('%s/article/list/%d?' % (url, i))
        html = etree.HTML(data)
        i=i+1
        aa = '//div/@data-articleid'
        li = html.xpath(aa)
        if len(li)==0:
            return ll
        ll += li
    return ll

#li = ['82762601', '83549202', '83410050', '83383119', '83302424', '83104764', '83056420', '82868658', '82865579', '82839713', '82783265', '82684203', '82660979', '82492354', '81506977', '81224151', '81218840', '81171545', '80820511', '80654007', '80207646']
#li = ['82762601']

#li=getlist()

import htm2md
import win32clipboard as w
import win32con

def gettext():
    w.OpenClipboard()
    t = w.GetClipboardData(win32con.CF_TEXT)
    w.CloseClipboard()
    return t

if 1:
    url='https://yuhui.blog.csdn.net/article/details/80106526'
    url='https://www.cnblogs.com/chaihy/p/10615117.html'
    url = gettext().decode('gbk')
    print(url)
    data = getdata(url)
    save_txt('test1.html', data)
    t, d = htm2md.htm2md(data)

    cc = t.split('-')
    ttt=cc[-1].strip()
    name=cc[-2].strip()
    t = '-'.join(cc[0:-2]).strip()
    mkdir(ttt)
    mkdir(ttt+'/'+name)
    print(t)
    print(len(d))
    save_txt(ttt+'/'+name + '/'+t+'.md', d)
    ##print(data)
