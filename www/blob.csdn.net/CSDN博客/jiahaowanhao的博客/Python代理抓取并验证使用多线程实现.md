
# Python代理抓取并验证使用多线程实现 - jiahaowanhao的博客 - CSDN博客


2018年02月02日 19:28:51[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：140


[Python代理抓取并验证使用多线程实现](http://cda.pinggu.org/view/24634.html)
这里没有使用队列只是采用多线程分发对代理量不大的网页还行但是几百几千性能就很差了，感兴趣的朋友可以了解下，希望对你有所帮助
没有使用队列，也没有线程池还在学习只是多线程
代码如下:
\#coding:utf8
import urllib2,sys,re
import threading,os
import time,datetime
'''''
这里没有使用队列 只是采用多线程分发对代理量不大的网页还行但是几百几千性能就很差了
'''
def get_proxy_page(url):
'''''解析代理页面 获取所有代理地址'''
proxy_list = []
p = re.compile(r'''''<div>(.+?)<span class="Apple-tab-span" style="white-space:pre">.*?</span>(.+?)<span class="Apple-tab-span" style="white-space:pre">.+?</span>(.+?)(<span.+?)?</div>''')
try:
res = urllib2.urlopen(url)
except urllib2.URLError:
print 'url Error'
sys.exit(1)
pageinfo = res.read()
res = p.findall(pageinfo) \#取出所有的
\#组合成所有代理服务器列表成一个符合规则的list
for i in res:
ip = i[0]
port = i[1]
addr = i[2]
l = (ip,port,addr)
proxy_list.append(l)
return proxy_list
\#同步锁装饰器
lock = threading.Lock()
def synchronous(f):
def call(*args, **kw):
lock.acquire()
try:
return f(*args, **kw)
finally:
lock.release()
return call
\#时间计算器
def sumtime(f):
def call(*args, **kw):
t1 = time.time()
try:
return f(*args, **kw)
finally:
print u'总共用时 %s' % (time.time() - t1)
return call
proxylist = []
reslist = []
\#获取单个代理并处理
@synchronous
def getoneproxy():
global proxylist
if len(proxylist)>0:
return proxylist.pop()
else:
return ''
\#添加验证成功的代理
@synchronous
def getreslist(proxy):
global reslist
if not (proxy in reslist):
reslist.append(proxy)
def handle():
timeout = 10
test_url = r'http://www.baidu.com'
test_str = '030173'
while 1:
proxy = getoneproxy()
\#最后一个返回是空
if not proxy:
return
print u"正在验证 : %s" %proxy[0]
\#第一步启用 cookie
cookies = urllib2.HTTPCookieProcessor()
proxy_server = r'http://%s:%s' %(proxy[0],proxy[1])
\#第二步 装载代理
proxy_hander = urllib2.ProxyHandler({"http":proxy_server})
\#第三步 组合request
try:
opener = urllib2.build_opener(cookies, proxy_hander)
pass
except urllib2.URLError:
print u'url设置错误'
continue
\#配置request
opener.addheaders = [('User-Agent','Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/21.0.1180.89 Safari/537.1')]
\#发送请求
urllib2.install_opener(opener)
t1 = time.time()
try:
req = urllib2.urlopen(test_url,timeout=timeout)
result = req.read()
pos = result.find(test_str)
timeused = time.time() - t1
if pos>1:
\#保存到列表中
getreslist((proxy[0],proxy[1],proxy[2],timeused))
print u'成功采集',proxy[0],timeused
else:
continue
except Exception,e:
print u'采集失败 %s ：timeout' %proxy[0]
continue
def save(reslist):
path = os.getcwd()
filename = path + '/Proxy-'+datetime.datetime.now().strftime(r'%Y%m%d%H%M%S')+'.txt'
f = open(filename,'w+')
for proxy in reslist:
f.write('%s %s %s %s \r\n'%(proxy[0],proxy[1],proxy[2],proxy[3]))
f.close()
@sumtime
def main():
url = r'http://www.free998.net/daili/httpdaili/8949.html'
global proxylist,reslist
\#获取所有线程
proxylist = get_proxy_page(url)
print u'一共获取 %s 个代理' %len(proxylist)
\#print proxylist
print '*'*80
\#线程创建和分发任务
print u'开始创建线程处理.....'
threads = []
proxy_num = len(proxylist)
for i in range(proxy_num):
th = threading.Thread(target=handle, args=())
threads.append(th)
for thread in threads:
thread.start()
for thread in threads:
threading.Thread.join(thread)
print u'获取有效代理 %s 个,现在开始排序和保存 '%len(reslist)
reslist = sorted(reslist,cmp=lambda x,y:cmp(x[3],y[3]))
save(reslist)
if __name__=='__main__':
main()

输出：
一共获取 31 个代理
********************************************************************************
开始创建线程处理.....
正在验证 : 122.10.48.13
正在验证 : 122.72.76.121
正在验证 : 122.72.11.129
正在验证 : 222.89.159.131
正在验证 : 218.5.74.174
正在验证 : 218.203.107.165
正在验证 : 219.224.101.81
正在验证 : 221.176.169.14
正在验证 : 112.5.254.85
正在验证 : 113.106.73.210
正在验证 : 114.247.21.212
正在验证 : 122.72.76.122
正在验证 : 219.239.26.23
正在验证 : 222.89.154.14
正在验证 : 58.67.147.197
正在验证 : 222.188.88.26
正在验证 : 103.247.16.241
正在验证 : 183.221.250.141
正在验证 : 183.221.250.137
正在验证 : 122.72.80.108
正在验证 : 122.72.76.125
正在验证 : 122.72.11.131
正在验证 : 122.72.80.101
正在验证 : 122.72.120.41
正在验证 : 122.72.120.38
正在验证 : 122.72.120.35
正在验证 : 218.203.105.26
正在验证 : 221.130.18.211
正在验证 : 110.77.236.48
正在验证 : 218.91.206.146
正在验证 : 211.162.16.210
成功采集 114.247.21.212 0.300999879837
成功采集 218.203.105.26 0.306999921799
成功采集 221.176.169.14 0.417000055313
成功采集 122.72.120.35 0.369999885559
采集失败 218.5.74.174 ：timeout
成功采集 122.72.120.38 0.40900015831
成功采集 183.221.250.137 0.608999967575
成功采集 122.72.11.131 0.679999828339
成功采集 183.221.250.141 0.791000127792
成功采集 113.106.73.210 0.891000032425
成功采集 122.72.76.121 1.40299987793
成功采集 122.72.80.108 1.4470000267
成功采集 211.162.16.210 1.625
成功采集 122.72.76.125 1.6819999218
成功采集 112.5.254.85 1.74399995804
成功采集 122.72.80.101 1.79799985886
成功采集 122.72.11.129 2.00900006294
成功采集 122.72.120.41 1.99099993706
采集失败 222.188.88.26 ：timeout
成功采集 122.72.76.122 3.49100017548
成功采集 218.91.206.146 3.66000008583
成功采集 122.10.48.13 3.91799998283
成功采集 222.89.154.14 3.93499994278
成功采集 222.89.159.131 3.99699997902
成功采集 221.130.18.211 3.99500012398
采集失败 219.224.101.81 ：timeout采集失败 218.203.107.165 ：timeout
采集失败 58.67.147.197 ：timeout
采集失败 103.247.16.241 ：timeout
采集失败 110.77.236.48 ：timeout
成功采集 219.239.26.23 12.2809998989
获取有效代理 24 个,现在开始排序和保存
总共用时 13.2810001373

