import urllib.request as request
from lxml import etree
import requests

def getdata1(url):
    try:
        file = request.urlopen(url)
        data = file.read().decode('utf8')
    except:
        data = "<html></html>"
        pass
    return data

cookies= {
    'SESSION':'d40451fc-c7f5-4390-a03b-321ef4e73675',
    'UserToken':'757a1b7c70f8414aa194fb0788859850',
    'UserName':'u014528142'
}

def getdata(url):
    try:
        r = requests.get(url,cookies=cookies)
        data = r.text
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


def save_txt_td(root, t, d):
    if len(t)<2:
        return
    if len(d)<2:
        return
    name=''
    ttt = root
    delimt=' - '
    if t.find('百度百科')>0:
        delimt='_'
    cc = t.count(delimt)
    t = t.replace('/', ' ').replace('\\', ' ').replace('*', ' ').replace('?', ' ')
    t = t.replace('.', ' ').replace('|', ' ')
    t = t.replace(':', ' ')
    t = t.replace('~', ' ')
    if cc>=2:
        t = t.replace(delimt, '/')
        cc = t.split('/')
        ttt=cc[-1].strip()
        name=cc[-2].strip()
        t = '-'.join(cc[0:-2]).strip()

    if 1==cc:
        t = t.replace(delimt, '/')
        cc = t.split('/')
        ttt=cc[-1].strip()
        t = '-'.join(cc[0:-1]).strip()

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



if __name__ == '__main__':
    url = 'https://blog.csdn.net/hpulfc/article/details/80084398'
    cookies={
        'SESSION':'d40451fc-c7f5-4390-a03b-321ef4e73675',
        'UserToken':'757a1b7c70f8414aa194fb0788859850',
        'UserName':'u014528142'
        }
    r = requests.get(url,cookies=cookies)
    if r.text.find('CSDN登录注册')>0:
        print('fail')
    print(r.text)
