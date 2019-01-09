import urllib.request
from lxml import etree

# 历届诺贝尔文学奖作品
# http://vdisk.weibo.com/s/dorkVHqEL8IHs

def getdata(url):
    try:
        file = urllib.request.urlopen(url)
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

# //*[@id="share_table"]/tbody/tr[1]/td[1]/div/div[2]/div/a

def getpages(html):
    aa = '/html/body/div[2]/div[2]/div[3]/div/div[1]/div[3]/div/a'
    tr = html.xpath(aa)
    if len(tr)==0:
        return 1
    tr = html.xpath(aa + '[%d]/text()'% (len(tr)-1))[0]
    ret = int(tr)
    return ret


def printhtml(htm):
    print(etree.tostring(htm,method='html'))

# 1406763582

def getpages2(html):
    aa = '/html/body/div[3]/div[3]/div[2]/div[1]/div[1]/div/div[3]/div/a'
    aa = './/div[@class="vd_page"]/a'
    tr = html.xpath(aa)
    #printhtml(tr[0])
    #print(tr)
    #print(len(tr))
    if len(tr)==0:
        return 1
    tr = html.xpath(aa + '[%d]/text()'% (len(tr)-1))[0]
    ret = int(tr)
    return ret

def getu1(html):
    ret = []
    tr = html.xpath('//*[@id="share_table"]/tbody/tr')
    for itr in tr:
        aa = itr.xpath('td[1]/div/div[2]/div/a/@title')[0]
        hh = itr.xpath('td[1]/div/div[2]/div/a/@href')[0]
        cc = itr.xpath('td[1]/div/div[1]/a/@class')
        if len(cc)==0:
            cc = itr.xpath('td[1]/div/div[1]/div/a/@class')
        if len(cc)==0:
            cc = 'xxxx'
        else:
            cc = cc[0]
        # print(hh, cc, aa)
        ret.append({"url":hh, "class":cc, "name":aa})

    return ret

def getname(html):
    aa = html.xpath('/html/body/div[2]/div[2]/div[1]/div/div[2]/p/text()')[0]
    aa = aa.strip('\t ')
    return aa;

if 0:
    url = 'http://vdisk.weibo.com/u/1943957265'
    html = gethtml(url)
    name = getname(html)
    print(name)

def getu(url):
    html = gethtml(url)
    name = getname(html)
    n = getpages(html)
    ret = getu1(html)
    print('pages=%d'% n)
    for i in range(2, n+1):
        print('pages=%d'% i)
        ret = ret + getu1(gethtml(url + '?page=%d'%(i)))

    return ret, name

def gets1(html):
    ret = []
    # //*[@id="file_share_list_0"]/td[2]/div/div[2]/div/a
    # //*[@id="file_share_list_0"]
    #
    tr = html.xpath('//*[@id="fileListBody"]/tr')
    for itr in tr:
        aa = itr.xpath('td[2]/div/div[2]/div/a/@title')[0]
        hh = itr.xpath('td[2]/div/div[2]/div/a/@href')[0]
        cc = itr.xpath('td[2]/div/div[1]/a/@class')
        if len(cc)==0:
            cc = itr.xpath('td[1]/div/div[1]/div/a/@class')
        if len(cc)==0:
            cc = 'xxxx'
        else:
            cc = cc[0]
        ret.append({"url":hh, "class":cc, "name":aa})

    return ret

def gets(url):
    ret = []
    # //*[@id="file_share_list_0"]/td[2]/div/div[2]/div/a
    # //*[@id="file_share_list_0"]
    #
    html = gethtml(url)
    n = getpages2(html)
    ret = gets1(html)
    #n=1000
    print('pns=%d'% n)
    for i in range(2, n+1):
        print('pn=%d'% i)
        ret1 = gets1(gethtml(url + '?pn=%d'%(i)))
        if len(ret1)==0:
            return ret;
        ret = ret + ret1

    return ret

def mkdir(path):
    print(path)
    if not os.path.exists(path):
        os.mkdir(path)


def save_txt(t, fn):
    f = open(fn, 'w')
    for i in t:
        try:
            f.write("%s#%s#%s\n" % (i['url'], i['class'], i['name']))
        except:
            pass
    f.close()

import os

def str2name(str):
    str = str.strip()
    str = str.replace('*','')
    str = str.replace('?','？')
    return str


def runalls(url, path):
    ret = gets(url)
    path = str2name(path)
    mkdir(path)
    save_txt(ret, path + '/list.txt')
    for s in ret:
        if s['class'] == 'vd_icon32_v2 vd_folder':
            runalls(s['url'], path + '/' + s['name'])


def runallu(url, path):
    ret,name = getu(url)
    path = str2name(path)
    name = str2name(name)
    path = path + name
    mkdir(path)
    save_txt(ret, path + '/list.txt')
    for s in ret:
        path1 = path + '/' + s['name']
        if not os.path.exists(path1):
            if s['class'] == 'vd_icon32_v2 vd_folder':
                runalls(s['url'], path1)

import clip
# 1400818390
if 1:
    str = clip.gettext().decode('gbk')
    if str.isdigit():
        url = 'http://vdisk.weibo.com/u/'
        url = url + str
        runallu(url, 'E:/微盘/')
    else:
        url = 'http://vdisk.weibo.com/s/'
        url = url + str
        runalls(url, 'E:/微盘/_aaa/')

if 0:
    str = clip.gettext().decode('gbk')
    url = 'http://vdisk.weibo.com/s/'
    url = url + str
    runalls(url, 'E:/微盘/_aaa/')
# print(data)


if 0:
    aa = html.xpath('//*[@id="share_table"]/tbody/tr[1]/td[1]/div/div[2]/div/a/@title')
    hh = html.xpath('//*[@id="share_table"]/tbody/tr[1]/td[1]/div/div[2]/div/a/@href')
    cc = html.xpath('//*[@id="share_table"]/tbody/tr[1]/td[1]/div/div[1]/a/@class')
    print(aa)
    print(cc)
    print(hh)

import re
import json

if 0:
    url = 'http://vdisk.weibo.com/s/dorkVHqEL8IHs'
    url = 'http://vdisk.weibo.com/s/dorkVHqEbr3YO'
    ret = gets(url)

if 0:
    url = 'http://vdisk.weibo.com/s/dorkVHqEbLwwl?category_id=0&parents_ref=dorkVHqEbr3YO'
    url = 'http://vdisk.weibo.com/s/dorkVHqEbr3Yn?category_id=0&parents_ref=dorkVHqEbr3YO'
    data = getdata(url)
    # print(data)
    a = re.findall(r"fileDown.init\((.+?)\);",data)
    j = json.loads(a[0])
    print(json.dumps(j, indent=2))
    parsed_tuple = urllib.parse.urlparse(j['url'])
    k = urllib.parse.parse_qs(parsed_tuple.query)
    print(k)


