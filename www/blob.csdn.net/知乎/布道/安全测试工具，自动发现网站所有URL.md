# 安全测试工具，自动发现网站所有URL - 知乎
# 



作为一个安全测试人员来说，首先要拿到网站所有url，然后根据拿到的url进行渗透测试进行漏洞挖掘。本文给大家介绍的是如何拿到一个网站所有的url。
![](https://pic3.zhimg.com/v2-3eb83da2d8fc1aa009a638219bedc97a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1272' height='680'></svg>)深度爬取层级控制![](https://pic4.zhimg.com/v2-881ea5faf57bcc4169477d61e5341767_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1007' height='681'></svg>)整体架构图
相信大家对深度控制和架构已经有基本了解，剩下的代码大家直接来看。非常易懂，不妨来试试。（下一次来给大家说一下代理池的创建，让你的Spider不被封）、


```
from lxml import html
import requests
import re
import pymysql

from search_ip import search_ip
###############代理###########

def get_proxy():
    return requests.get("http://127.0.0.1:5010/get/").content

def delete_proxy(proxy):
    requests.get("http://127.0.0.1:5010/delete/?proxy={}".format(proxy))

def html_prase(url):
    r = requests.get(url).content
    return html.fromstring(r)



def url_is_correct():
    '''
    使用requests.get方法判断url是否正确,并返回url
    :return:
    '''
    url = "https://www.wuage.com" #网站入口
    requests.get(url)
    return url

url = url_is_correct()   #将验证为正确的url地址赋值给url

def url_protocol(url):
    '''
    获取输入的url地址的协议，是http、https等
    '''
    print('该站使用的协议是：' + re.findall(r'.*(?=://)',url)[0])
    return re.findall(r'.*(?=://)',url)[0]

urlprotocol = url_protocol(url)

def same_url(url):
    '''
    处理输入的url，判断是否为同一站点做准备，爬取的时候不能爬到其它站
    '''
    #将完整的url中的http://删除
    url = url.replace(urlprotocol + '://','')

    if re.findall(r'^www',url) == []:
        sameurl = 'www.' + url
        if sameurl.find('/') != -1:
            sameurl = re.findall(r'(?<=www.).*?(?=/)', sameurl)[0]
        else:
            sameurl = sameurl + '/'
            sameurl = re.findall(r'(?<=www.).*?(?=/)', sameurl)[0]
    else:
        if url.find('/') != -1:
            sameurl = re.findall(r'(?<=www.).*?(?=/)', url)[0]
        else:
            sameurl = url + '/'
            sameurl = re.findall(r'(?<=www.).*?(?=/)', sameurl)[0]
    #print('同站域名地址：' + sameurl)
    return sameurl

domain_url = url



'''
处理url的类，对已访问过的和未访问过的进行记录，待后续使用
'''
class linkQuence:
    def __init__(self):
        self.visited = []    #已访问过的url初始化列表
        self.unvisited = []  #未访问过的url初始化列表

    def getVisitedUrl(self):  #获取已访问过的url
        return self.visited
    def getUnvisitedUrl(self):  #获取未访问过的url
        return self.unvisited
    def addVisitedUrl(self,url):  #添加已访问过的url
        return self.visited.append(url)
    def addUnvisitedUrl(self,url):   #添加未访问过的url
        if url != '' and url not in self.visited and url not in self.unvisited:
            return self.unvisited.insert(0,url)

    def removeVisited(self,url):
        return self.visited.remove(url)
    def popUnvisitedUrl(self):    #从未访问过的url中取出一个url
        try:                      #pop动作会报错终止操作，所以需要使用try进行异常处理
            return self.unvisited.pop()
        except:
            return None
    def unvisitedUrlEmpty(self):   #判断未访问过列表是不是为空
        return len(self.unvisited)


class Spider():
    '''
    爬取程序
    '''
    def __init__(self,url):
        self.linkQuence = linkQuence()   #引入linkQuence类
        self.linkQuence.addUnvisitedUrl(url)   #并将需要爬取的url添加进linkQuence对列中
        self.current_deepth = 1    #设置爬取的深度

    def getPageLinks(self,url):
        '''
        获取页面中的所有链接
        '''
        sel = html_prase(url)
        pageLinks = sel.xpath('//a/@href')
        return pageLinks
    def processUrl(self,url):
        '''
        处理相对路径为正确的完整url
        '''
        true_url = []
        for l in self.getPageLinks(url):
            if re.findall(r'//',l):
                if re.findall('https://',l) or re.findall('http://',l):
                    true_url.append(l)
                elif not re.findall('@',l):
                    #true_url.append(urlprotocol + '://' + domain_url + l)
                    true_url.append(urlprotocol+':' + l)
        # for l in true_url:
        #     print(l)
        return true_url

    def sameTargetUrl(self,url):
        '''
        判断是否为同一站点链接，防止爬出站外。
        '''
        same_target_url = []
        for l in self.processUrl(url):
            if re.findall(domain_url,l):
                same_target_url.append(l)
        #print(self.same_target_url)
        return same_target_url

    def unrepectUrl(self,url):
        '''
        删除重复url，排除指定域名
        '''
        unrepect_url = []
        expect_domain = ['s.wuage.com','static.wuage.com',
                         'shop.wuage.com','img.wuage.com','medici.wuage.com',
                         'buyer.wuage.com','item.wuage.com']
        for l in self.sameTargetUrl(url):
            if l not in unrepect_url and l.split('//')[1].split('/')[0] not in expect_domain:
                unrepect_url.append(l)

        return unrepect_url

    def crawler(self,crawl_deepth=1):
        '''
        依据深度进行爬取层级控制
        '''
        #while self.current_deepth <= crawl_deepth:
        while self.current_deepth <= crawl_deepth:
            visitedUrl = self.linkQuence.popUnvisitedUrl()
            if visitedUrl is None or visitedUrl == '':
                continue
            self.getPageLinks(visitedUrl)
            links = self.unrepectUrl(visitedUrl)
            self.linkQuence.addVisitedUrl(visitedUrl)
            for link in links:
                sel_link = html_prase(link)
                print(link)
                self.linkQuence.addUnvisitedUrl(link)
            self.current_deepth += 1

        return self.linkQuence.visited


if __name__ == '__main__':
    spider = Spider(url)
    spider.crawler(3)
```


**作者：刘晓明，互联网公司运维技术负责人，拥有10年的互联网开发和运维经验。一直致力于运维工具的开发和运维专家服务的推进，赋能开发，提高效能。****广告时间：最后给自己代个盐~~欢迎大家有空时翻下我牌子(知乎号：布道 )，看看“开发运维”专栏的文章，希望多些关注和点赞是给作者最好的鼓励 ！**




