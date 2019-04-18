# 21天打造分布式爬虫-requests库（二） - weixin_33985507的博客 - CSDN博客
2018年07月28日 20:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
### **2.1.get请求**
简单使用
```
import requests
response = requests.get("https://www.baidu.com/")
#text返回的是unicode的字符串，可能会出现乱码情况
# print(response.text)
#content返回的是字节，需要解码
print(response.content.decode('utf-8'))
# print(response.url)             #https://www.baidu.com/
# print(response.status_code)     #200
# print(response.encoding)        #ISO-8859-1
```
添加headers和params
```
import requests
params = {
    'wd':'python'
}
headers = {
    'User-Agent':'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36'
}
response = requests.get("https://www.baidu.com/s",params=params,headers=headers)
#content返回的是字节，需要解码
with open('baidu.html','w',encoding='utf-8') as f:
    f.write(response.content.decode('utf-8'))
```
### **2.2.POST请求**
 爬去拉钩网职位信息
```
import requests
url = "https://www.lagou.com/jobs/positionAjax.json?city=%E5%8C%97%E4%BA%AC&needAddtionalResult=false"
data = {
    'first':'true',
    'pn':1,
    'kd':'python'
}
headers = {
    "User-Agent":"Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36",
    "Referer":"https://www.lagou.com/jobs/list_python?city=%E5%8C%97%E4%BA%AC&cl=false&fromSearch=true&labelWords=&suginput="
}
response = requests.post(url,data=data,headers=headers)
# print(response.text)
print(type(response.text))       #<class 'str'>
print(type(response.json()))     #<class 'dict'>
print(response.json())           #获取为字典的形式
```
### **2.3.使用代理**
```
import requests
proxy = {'http':'115.210.31.236.55:9000'}
response = requests.get("https://www.baidu.com/",proxies=proxy)
print(response.content.decode('utf-8'))
```
### **2.4.session登录**
```
# _*_ coding:utf-8 _*_
import requests
# 1. 创建session对象，可以保存Cookie值
ssion = requests.session()
# 2. 处理 headers
headers = {'User-Agent':'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.101 Safari/537.36'}
# 3. 需要登录的用户名和密码
data = {"email":"158xxxxxxxx", "password":"pythonxxxxxxx"}
# 4. 发送附带用户名和密码的请求，并获取登录后的Cookie值，保存在ssion里
ssion.post("http://www.renren.com/PLogin.do", data = data)
# 5. ssion包含用户登录后的Cookie值，可以直接访问那些登录后才可以访问的页面
response = ssion.get("http://zhibo.renren.com/news/108")
# 6. 打印响应内容
print(response.text)
```
