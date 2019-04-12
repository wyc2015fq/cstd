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
