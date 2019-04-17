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

csdn_cookies= {
    'SESSION':'d40451fc-c7f5-4390-a03b-321ef4e73675',
    'UserToken':'757a1b7c70f8414aa194fb0788859850',
    'UserName':'u014528142'
}

def getdata(url):
    try:
        if url.find('csdn.net')>0 or url.find('blog.sciencenet.cn')>0:
            r = requests.get(url,cookies=csdn_cookies)
            data = r.text
        else:
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


def save_txt_td(root, t, d):
    if len(t)<2:
        return
    if len(d)<2:
        return

    if t.find('随笔列表第')>=0:
        return

    if t.find('随笔档案')>=0:
        return

    name=''
    ttt = root
    delimt=' - '
    if t.find('百度百科')>0:
        delimt='_'
    cc = t.count(delimt)
    t = t.replace('/', ' ').replace('\\', ' ').replace('*', ' ').replace('?', ' ')
    t = t.replace('.', ' ').replace('|', ' ').replace('"', ' ')
    t = t.replace(':', ' ').replace('>', ' ').replace('<', ' ')
    t = t.replace('~', ' ').replace('"', ' ').replace('\t', ' ')
    t = t.replace('  ', ' ')
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


if 0:
    text = """
    1 <script type="text/javascript">
    2   //JavaScript声明数组的四种方式
    3     var arr1 = new Array();//创建一个空数组
    4     arr1[0]="xdp";
    5     arr1[1]="gacl";
    6     var arr2 = new Array(2);//创建一个数组并指定长度为2
    7     arr2["name0"]="xdp";//arr2第一个元素
    8     arr2["name1"]="gacl";//arr2第二个元素
    9     arr2["name2"]="xtxd";//arr2第三个元素，arr2虽然在声明时指明了长度为2，但是还是可以添加超过其指明长度的元素
    10     var arr3 = new Array("孤傲苍狼","白虎神皇","灭世魔尊");//创建一个数组并初始化数组中的元素
    11     var arr4 = [1,true,"String"];//Array的简化声明
    12
    13     document.write("遍历arr1中的元素：<br/>");
    14     for(var i in arr1) {
    15         document.write(arr1[i]+"<br/>");
    16     }
    """

    text = text.strip()
    ltext = text.split('\n')
    for i in ltext:
        t = i.strip().split(' ')

    def remove_line_number(text):
        pass


if __name__ == '__main__':
    if 0:
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
