
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

def getlist(url, aa):
    ll=[]
    data = getdata(url)
    html = etree.HTML(data)
    print(len(data))
    ll = html.xpath(aa)
    return ll

#li = ['82762601', '83549202', '83410050', '83383119', '83302424', '83104764', '83056420', '82868658', '82865579', '82839713', '82783265', '82684203', '82660979', '82492354', '81506977', '81224151', '81218840', '81171545', '80820511', '80654007', '80207646']
#li = ['82762601']

#li=getlist()

import htmltree

def down_csdn_one(url):
    print(url)
    data = getdata(url)
    #save_txt('test1.html', data)
    root, t, d = htmltree.htm2md(data)
    save_txt_td(root, t, d)

def down_csdn_list_one(url, aa):
    ll=getlist(url, aa)
    for li in ll:
        down_csdn_one(li)
    return ll

import sys

def down_csdn_list(url, aa, temp):
    if len(sys.argv)>1:
        tt = 0
        # n = int(sys.argv[1])
        for i in range(1, 1000):
            url1=temp%(url, i)
            print('---'+url1)
            ll = down_csdn_list_one(url1, aa)
            if len(ll)<10:
                tt+=1
            else:
                tt=0
            if tt>3:
                break
    else:
        ll = down_csdn_list_one(url, aa)
    return 0

if __name__ == '__main__':
    url='https://yuhui.blog.csdn.net/article/details/80106526'
    url='https://www.cnblogs.com/chaihy/p/10615117.html'
    url='https://blog.csdn.net/sinat_26917383/article/category/6093543'
    url='https://blog.csdn.net/sinat_26917383/article/details/82880021'
    url='https://www.cnblogs.com/pinard/p/6645766.html'
    url='https://www.cnblogs.com/pinard/category/894690.html'
    url = gettext().decode('gbk')
    if url.find('csdn.net')>0:
        temp='%s/%d?'
        if url.find('/category/')>0:
            aa='//div[@data-articleid]/h4/a/@href'
            down_csdn_list(url, aa, temp)
        else:
            if len(url.split('/'))==4:
                url += '/article/list'

            if url.find('/list')>0:
                aa='//li/a/@href'
                aa='//div[@data-articleid]/h4/a/@href'
                down_csdn_list(url, aa, temp)
            else:
                down_csdn_one(url)
    elif url.find('www.cnblogs.com')>0:
        temp='%s=%d'
        if url.find('/category/')>0:
            aa='//div[@class]/a/@href'
            down_csdn_list(url, aa, temp)
        else:
            url = url.strip('/')
            if len(url.split('/'))==4:
                url += '/default.html?page'

            if url.find('/default.html?page')>0:
                aa='//div[@class]/a/@href'
                down_csdn_list(url, aa, temp)
            else:
                down_csdn_one(url)
    elif url.find('www.jianshu.com/u/')>0:
        pass
    elif url.find('baike.baidu.com')>0:
        down_csdn_one(url)
    else:
        down_csdn_one(url)
    ##print(data)
