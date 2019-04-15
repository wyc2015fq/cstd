import urllib.request as request
from lxml import etree

def getdata(url):
    try:
        file = request.urlopen(url)
        data = file.read().decode('utf8')
    except:
        data = "<html></html>"
        pass
    return data

def gethtml(url):
    html = ""
    data = getdata(url)
    if len(data)<20:
        data = getdata(url)

    html = etree.HTML(data)
    return html


import os
def mkdir(path):
    if not os.path.exists(path):
        print(path)
        os.mkdir(path)


def save_txt(fn, s):
    f = open(fn, 'wt',encoding='utf8')
    try:
        f.write(s)
    except:
        pass
    f.close()


def save_txt_td(t, d):
    if len(t)<2:
        return
    if len(d)<2:
        return
    t = t.replace('/', ' ').replace('\\', ' ').replace('*', ' ').replace('?', ' ')
    t = t.replace('.', ' ').replace('|', ' ')
    t = t.replace(':', ' ')
    t = t.replace('~', ' ')
    t = t.replace(' - ', '/')
    cc = t.split('/')
    ttt=cc[-1].strip()
    name=cc[-2].strip()
    t = '-'.join(cc[0:-2]).strip()
    mkdir(ttt)
    mkdir(ttt+'/'+name)
    print(t)
    print(len(d))
    if len(t)>1:
        save_txt(ttt+'/'+name + '/'+t+'.md', d)

import win32clipboard as w
import win32con

def gettext():
    w.OpenClipboard()
    t = w.GetClipboardData(win32con.CF_TEXT)
    w.CloseClipboard()
    return t
