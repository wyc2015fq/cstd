
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

import htm2md

def down_csdn_one(url):
    print(url)
    data = getdata(url)
    #save_txt('test1.html', data)
    root, t, d = htm2md.htm2md(data)
    # 'data-za-detail-view-element_name'
    save_txt_td(root, t, d)

def down_csdn_list_one(url, aa):
    ll=getlist(url, aa)
    for li in ll:
        down_csdn_one(li)
    return ll

def getlist_zhihu(url):
    data = getdata1(url)
    #save_txt('test1.html', data)
    tt = data.replace('[', '\n').replace(']', '\n').split('\n')
    c = 0
    t = ''
    for i in (tt):
        cc = i.count('null,null')+i.count('false,false')
        if cc>c:
            c = cc
            t = i
    # t = t.replace('null', '0')
    li = t.split(',')
    ll=[]
    for i in li:
        if 'null'!=i:
            ll.append('https://zhuanlan.zhihu.com/p/'+i)
    return ll

def getlist_sciencenet(url):
    data = getdata(url)
    tt = data.replace('"', '\n').split('\n')
    ll=[]
    for i in tt:
        if len(i)>15 and 'blog-'==i[0:5] and '.html'==i[-5:]:
            ll.append('http://blog.sciencenet.cn/'+i)
    return ll

def getlist_sina(url):
    data = getdata(url)
    tt = data.replace('"', '\n').split('\n')
    ll=[]
    for i in tt:
        if i.find('/s/blog_')>0:
            ll.append(i)
    return ll

def getlist_skywind(url):
    data = getdata(url)
    aa='//a[@rel="bookmark"]/@href'
    html = etree.HTML(data)
    tt = html.xpath(aa)
    ll=[]
    for i in tt:
        if i.find('www.skywind.me/blog/archives/')>0:
            ll.append(i)
    return ll

def getlist_51cto(url):
    data = getdata(url)
    aa='//a[@class="tit"]/@href'
    html = etree.HTML(data)
    tt = html.xpath(aa)
    ll=[]
    for i in tt:
        if i.find('//blog.51cto.com')>0:
            ll.append(i)
    return ll

def getlist_jobbole(url):
    data = getdata(url)
    aa='//div[@class="post-thumb"]/a/@href'
    html = etree.HTML(data)
    tt = html.xpath(aa)
    ll=[]
    for i in tt:
        if i.find('//blog.jobbole.com')>0:
            ll.append(i)
    return ll

def getlist_cnblogs(url):
    data = getdata(url)
    if 0:
        tt = data.replace('"', '\n').split('\n')
    else:
        aa='//div[@class]/a/@href'
        html = etree.HTML(data)
        tt = html.xpath(aa)

    ll=[]
    for i in tt:
        if i.find('/articles/')>0 or i.find('/p/')>0 or i.find('/archive/')>0:
            ll.append(i)
    return ll



def down_list_one(url, getlist, ll1):
    ll0=getlist(url)
    ll0=set(ll0+ll1)
    ll = list(ll0-set(ll1))
    for li in ll:
        down_csdn_one(li)
    return ll, list(ll0)

def down_list(urllist, getlist):
    tt = 0
    ll1=[]
    for url in urllist:
        print('------', url)
        ll, ll1 = down_list_one(url, getlist, ll1)
        if len(ll)<5:
            tt+=1
        else:
            tt=0
        if tt>3:
            break
    return 0

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
            url = '/'.join(url.split('/')[0:4])
            url += '/article/list'

            if url.find('/list')>0:
                aa='//li/a/@href'
                aa='//div[@data-articleid]/h4/a/@href'
                down_csdn_list(url, aa, temp)
            else:
                down_csdn_one(url)
    elif url.find('www.cnblogs.com')>0:
        temp='%s=%d'
        if url.find('/articles/')>0 or url.find('/archive/')>0:
            down_csdn_one(url)
        else:
            url = '/'.join(url.split('/')[0:4])
            url += '/default.html?page='
            urllist = map(lambda i:url+str(i), range(1,1000))
            urllist = list(urllist)
            down_list(urllist, getlist_cnblogs)

    elif url.find('www.jianshu.com/u/')>0:
        pass
    elif url.find('baike.baidu.com')>0:
        down_csdn_one(url)
    elif url.find('blog.sciencenet.cn')>0:
        if url.find('&do=blog&')>0:
            url = '&'.join(url.split('&')[0:5])
            url += '&page='
            urllist = map(lambda i:url+str(i), range(1,1000))
            urllist = list(urllist)
            down_list(urllist, getlist_sciencenet)
        else:
            down_csdn_one(url)
    elif url.find('blog.sina.com.cn')>0:
        if url.find('/s/articlelist_')>0:
            url = '_'.join(url.split('_')[0:3])
            url += '_'
            urllist = map(lambda i:url+str(i)+'.html', range(1,1000))
            urllist = list(urllist)
            down_list(urllist, getlist_sina)
        else:
            down_csdn_one(url)
    elif url.find('blog.jobbole.com')>0:
        if url.find('/category')>0:
            url = '/'.join(url.split('/')[0:5])
            url += '/page/'
            urllist = map(lambda i:url+str(i)+'/', range(1,1000))
            urllist = list(urllist)
            down_list(urllist, getlist_jobbole)
        elif url.find('/all-posts')>0:
            url = '/'.join(url.split('/')[0:4])
            url += '/page/'
            urllist = map(lambda i:url+str(i)+'/', range(1,1000))
            urllist = list(urllist)
            down_list(urllist, getlist_jobbole)
        else:
            down_csdn_one(url)
    elif url.find('www.skywind.me')>0:
        if url.find('/blog/page/')>0:
            url = '/'.join(url.split('/')[0:4])
            url += '/page/'
            urllist = map(lambda i:url+str(i), range(1,1000))
            urllist = list(urllist)
            down_list(urllist, getlist_skywind)
        else:
            down_csdn_one(url)
    elif url.find('blog.51cto.com')>0:
        tt = url.split('/')
        if len(tt)<5:
            url = '/'.join(tt[0:4])
            url += '/p'
            urllist = map(lambda i:url+str(i), range(1,1000))
            urllist = list(urllist)
            down_list(urllist, getlist_51cto)
        else:
            down_csdn_one(url)
    elif url.find('zhihu.com')>0:
        if url.find('/people/')>0 or url.find('/org/')>0:
            url = '/'.join(url.split('/')[0:5])
            url += '/posts?page='
            urllist = list(map(lambda i:url+str(i), range(1,1000)))
            down_list(urllist, getlist_zhihu)
        down_csdn_one(url)
    else:
        down_csdn_one(url)
    ##print(data)
