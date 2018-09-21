#!/usr/bin/env python

import lmdb
import json
import os, sys
import urllib.request
from lxml import etree


def getdata(url):
    try:
        file = urllib.request.urlopen(url)
    except e:
        return ''
    data = file.read().decode('utf8')
    return data

def getdatasave(url, fn):
    file = urllib.request.urlopen(url)
    data = file.read()
    fhandle =open(fn,"wb")
    fhandle.write(data)
    fhandle.close()
    return data

def gethtml(data):
    html = ""
    html = etree.HTML(data)
    return html


aa = '//ul[2]/li[2]/a/div[2]/div[1]/text()' # title
aa = '//ul[2]/li[2]/a/div[2]/div[2]/text()' # time
aa = '//ul'
#print(uls)

#imgurl = 'http://p1.img.cctvpic.com/fmspic/2018/09/03/5766741ceafe4f938945ae87d2305ae0-180.jpg?p=2&h=120'
#getdatasave(imgurl, '111.jpg')

def get_text(url):
    data = getdata(url)
    html = gethtml(data)
    #print(data)
    #i = data.find('7e76462293b441b6a404833655896918', 3698+5)
    #print(i, data[i:-1])
    #return "";
    time = html.xpath('//*[@id="page_body"]/div[2]/div[1]/p[2]/text()[2]')[0]
    time = time.strip('\r\n\t ')
    #text = html.xpath('//*[@id="about_txt"]/div[2]/div/p')
    guid = html.xpath('//*[@id="about_txt"]/div[4]/text()')[-2]
    ps = html.xpath('//*[@id="about_txt"]/div[2]/div/p')
    #text = html.xpath('//*[@id="about_txt"]/div[2]/div/p[4]/text()')[0]
    text = []
    text.append(time)
    #print(ps)
    for p in ps:
        t = p.xpath('text()')
        if len(t)>0:
            text.append(t[0])
    #print(text)
    return text,guid

def get_vedio(url):
    data = getdata(url)
    text = json.loads(data)
    #jsontext = json.dumps(text,indent=4)
    #print(jsontext)
    #print(text['title'])
    return text

def get_day(url):
    data = getdata(url)
    html = gethtml(data)
    out = []
    #print(data)
    uls = html.xpath('//ul')
    for ul in uls:
        ulli = ul.xpath('li')
        for li in ulli:
            title = li.xpath('a/div[2]/div[1]/text()')[0]
            print(title)
            href = li.xpath('a/@href')[0]
            imgurl = li.xpath('a/div[1]/img/@src')[0]
            #guid = imgurl.split('/')[-1].split('-')[0]
            vediourl = 'http://vdn.apps.cntv.cn/api/getHttpVideoInfo.do?pid='
            text,guid = get_text(href)
            jsonobj = get_vedio(vediourl+guid)
            jsonobj['text'] = text
            jsonobj['guid'] = guid
            jsonobj['title2'] = title
            out.append(jsonobj)
            #if guid=='6ce25a40e0e54a538df191b83e8011c3':
    return {'url': url, 'data':out}


# //*[@id="content"]/ul[1]/li[2]/a/div[2]/div[1]

if 0:
    url = 'http://vdn.apps.cntv.cn/api/getHttpVideoInfo.do?pid=c710cfce6492471682d77e24a31527e4'
    out = get_vedio(url)

if 0:
    url = 'http://tv.cctv.com/2018/09/03/VIDEcJ4axAzOul6GWNg6kFDX180903.shtml'
    url = 'http://tv.cctv.com/2018/09/03/VIDE8PB4e5v8ircNSaotQK51180903.shtml'
    url = 'http://tv.cctv.com/2018/09/02/VIDEo0ge1TWB4SaTtKnZ29Mr180902.shtml'
    get_text(url)

if 1:
    url = 'http://tv.cctv.com/lm/xwlb/day/20100901.shtml'
    try:
        out = get_day(url)
    except Exception as e:
        pass
    else:
        out['time'] = '20180903'
        jsontext = json.dumps(out,indent=4)
        print(jsontext)
    finally:
        pass




import lmdb
import os, sys

def str2byte(x):
    # print(type(x))
    if isinstance(x, int):
        return str(x).encode('utf-8')

    if isinstance(x, str):
        return x.encode('utf-8')

    return "?";

def initialize(name):
    env = lmdb.open(name);
    return env;

def insert(env, sid, name):
    txn = env.begin(write = True);
    txn.put(str2byte(sid), str2byte(name));
    txn.commit();

def delete(env, sid):
    txn = env.begin(write = True);
    txn.delete(str2byte(sid));
    txn.commit();

def update(env, sid, name):
    txn = env.begin(write = True);
    txn.put(str2byte(sid), str2byte(name));
    txn.commit();

def search(env, sid):
    txn = env.begin();
    name = txn.get(str2byte(sid));
    return name;

def display(env):
    txn = env.begin();
    cur = txn.cursor();
    for key, value in cur:
        print(key, value);


import datetime
 
if 0:
    start='2016-06-01'
    end='2017-01-01'
    datestart=datetime.datetime.strptime(start,'%Y-%m-%d')
    dateend=datetime.datetime.strptime(end,'%Y-%m-%d')
     
    while datestart<dateend:
        datestart+=datetime.timedelta(days=1)
        print(datestart.strftime('%Y%m%d'))


def get_datetime_list(start, end, ifmt, ofmt):
    datestart=datetime.datetime.strptime(start,ifmt)
    dateend=datetime.datetime.strptime(end,ifmt)
    out = []
    while datestart<dateend:
        datestart+=datetime.timedelta(days=1)
        out.append(datestart.strftime(ofmt))
    return out

lmdb_fn = 'E:/data/cctv/cctv_video_info'

if 0:
    env = initialize(lmdb_fn)
    datelist = get_datetime_list('20010801', '20180903', '%Y%m%d', '%Y%m%d')
    print(datelist)

if 0:
    for t in datelist:
        if None==search(env, t):
            url = 'http://tv.cctv.com/lm/xwlb/day/' + t + '.shtml'
            try:
                out = get_day(url)
            except Exception as e:
                pass
            else:
                out['time'] = '20180903'
                #jsontext = json.dumps(out,indent=4)
                jsontext = json.dumps(out)
                #print(jsontext)
                insert(env, t, jsontext)
                print(t, 'OK!')
            finally:
                pass
    env.close();


if 0:
    env = initialize(lmdb_fn);
    txn = env.begin();
    cur = txn.cursor();
    fhandle =open('E:/data/cctv/mp4list.txt',"wb")
    for key, value in cur:
        jsonobj = json.loads(value)
        for aa in jsonobj['data']:
            data = aa['video']['lowChapters'][0]['url']
            print(data)
            data = data+'\n'
            fhandle.write(data.encode('utf-8'))
    fhandle.close()
    env.close();

