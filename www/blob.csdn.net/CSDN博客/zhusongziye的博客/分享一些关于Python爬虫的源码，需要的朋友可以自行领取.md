# 分享一些关于Python爬虫的源码，需要的朋友可以自行领取 - zhusongziye的博客 - CSDN博客





2018年11月24日 14:10:54[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：67








**利用Python批量下载百度图片**

```
# !/usr/bin/env python
# -*- coding:utf-8 -*-

# 导入URLLIB库的编码方法
from urllib.parse import urlencode
# 请求库
import requests
# 用于处理Json格式的文件
import json


# 函数作用,拼接完整的URL
def page_url_cont():
    # 可以传入2个变量,一个用于搜索关键字,一个用于翻页
    # queryWord   word    对应关键字
    # pn 对应翻页
    data = {'tn': 'resultjson_com',
            'ipn': 'rj',
            'ct': '201326592',
            'is': '',
            'fp': 'result',
            'queryWord': '美女',
            'cl': 2,
            'lm': -1,
            'ie': 'utf-8',
            'oe': 'utf-8',
            'adpicid':'',
            'st': '-1',
            'z': '',
            'ic': 0,
            'word': '美女',
            's': '',
            'se': '',
            'tab': '',
            'width': '',
            'height': '',
            'face': 0,
            'istype': 2,
            'qc': '',
            'nc': 1,
            'fr': '',
            'pn': 0,
            'rn': 30,
            'gsm': 96
            }
    # 拼接url
    url = 'http://image.baidu.com/search/index?' + urlencode(data)

    # 返回拼接好的url
    return url


# 获取图片链接返回一个列表
def get_images_url(response):
    # 用jsondata保存 用json.loads的方法处理返回回来的类似字典的格式
    jsondata = json.loads(response.text)
    # 创建一个空的列表,用于处理提取出来的Url
    image_url = []
    # 检查字典里是否包含data这个关键字
    if 'data' in jsondata.keys():
        # 从字典里面提取出data这个键的值
        for items in jsondata.get('data'):
            # 从items这个自动中提取thumbURL的值,返回一个url
            url = items.get('thumbURL')
            # 空列表添加提取出来的url
            image_url.append(url)
    # 添加完成后统一返回
    return image_url

# 下载用的函数
def downimage(image_url):
    # 从我们传入的url列表重依次取出url
    for url in image_url:
        # 异常处理
        try:
            # 请求我们图片地址得到返回
            response = requests.get(url)
            # 判断响应码
            if response.status_code == 200:
                # 用切片的方法给图片命名
                name = url.split(',')[-1].split('&')[0]
                # 以字节的方式读取读取数据
                image = response.content
                # 创建一个空白的文件,以二进制的方式写入数据
                # 给创建出来的这个文件取个别名叫f
                with open('./images/%s.jpg' % name, 'wb') as f:
                    # 写入我们的2进制数据
                    f.write(image)
        except:
            print('当前请求出错')


def main():
    url = page_url_cont()
    # 获得的响应 被赋值  GEI请求赋值
    response = requests.get(url)
    # 用json格式读取返回的文本数据
    # print(jsondata.keys())
    image_url = get_images_url(response)
    # print(image_url)
    # 传入我们的下载器
    downimage(image_url)


if __name__ == '__main__':
    main()
```



**利用Python批量下载斗图网表情包**

```
# encoding: utf-8
# 第一步：导入第三方库
import requests  # 请求网页
from lxml import etree  # 解析网页
from urllib import request  # 下载内容
import os  # 系统包



# 第二步：抓取目标网页
def parse_page(url):
    # 2.1简单的反爬虫机制
    HEADERS = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36'}
    # 2.2获取服务器响应
        # 获取请求响应的状态码
    response = requests.get(url, headers=HEADERS)
    # 2.3 从响应里提取出网页
        # 获取整个网页
    text = response.text
        # 将抓取的网页作为参数返回
    return text
# 第三步：解析网页获取数据
def page_list(text):
    # 3.1 将网页保存在HTML对象里
    html = etree.HTML(text)  # 把网页保存在HTML对象里，便于对数据进行提取
    # 3.2 从网页对象根据一定的规则提取数据
    imgs = html.xpath("//div[@class='page-content text-center']//a//img")
    # 获取所有的表情图片以对象的形式保存在列表里
    return imgs


def main():
    # 根据URL的规则对URL进行循环，获取多页URL，并传给parse_page()进行抓取数据
    for x in range(1, 11):
        url = "http://www.doutula.com/photo/list/?page=%d" % x
        # 调用parse_page()函数并把url给进去
        text=parse_page(url)
        # 接收parse_page()返回的网页，并传给page_list()函数进行解析
        imgs = page_list(text)
        # 3.3 对列表里的img对象进行提取
        for img in imgs:
            # try.... except    对程序进行异常处理，避免因为其他原因报错
            try:
                # 3.4获取所有表情的URL并保存在列表里
                imgurl = img.xpath(".//@data-original")
                # 3.5从列表里提取出表情的URL，至于为啥不取零，列表中有空值，取零报错
                for img_url in imgurl:
                    # print(img_url)
                    # 分割后缀名：.jpg .png
                    # 3.6对表情的URL进行处理，提取出表情图片的格式，用于组成表情的名字
                    suffix = os.path.splitext(img_url)[1]
                    suffix = suffix.split("!")[0]

                    # 3.7获取表情的名字
                    alt = img.xpath(".//@alt")[0]
                    # alt = re.sub(r'[，。？?,/\\·]','',alt)  #利用正则表达式对表情名字中存在的特殊字符进行处理
                    # 3.8用 alt+suffix组成表情的新名字
                    img_name = alt + suffix

# 第四步：计算机代替下载
                    # 使用request.urlretrieve()对表情进行下载并保存在images文件里
                    request.urlretrieve(img_url, 'images/' + img_name)
                    # 打印出那些表情已经下载
                    print(img_name + '下载完毕！')

            except:
                print("表情报错")

    # 执行函数
if __name__ == '__main__':
    main()
```



**模拟登陆京东**

```
# -*- coding:utf-8 -*-

import time
import requests
from bs4 import BeautifulSoup


class JD_crawl:
    def __init__(self, username, password):
        self.headers = {
                        'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36'
                                      ' (KHTML, like Gecko) Chrome/62.0.3202.89 Safari/537.36',
                        'Referer': 'https://www.jd.com/',
                        }
        self.login_url = "https://passport.jd.com/new/login.aspx"
        self.post_url = "https://passport.jd.com/uc/loginService"
        self.auth_url = "https://passport.jd.com/uc/showAuthCode"
        self.session = requests.session()
        self.username = username
        self.password = password

    def get_login_info(self):
        html = self.session.get(self.login_url, headers=self.headers).content
        soup = BeautifulSoup(html, 'lxml')

        uuid = soup.select('#uuid')[0].get('value')
        eid = soup.select('#eid')[0].get('value')
        fp = soup.select('input[name="fp"]')[0].get('value')  # session id
        _t = soup.select('input[name="_t"]')[0].get('value')  # token
        login_type = soup.select('input[name="loginType"]')[0].get('value')
        pub_key = soup.select('input[name="pubKey"]')[0].get('value')
        sa_token = soup.select('input[name="sa_token"]')[0].get('value')

        auth_page = self.session.post(self.auth_url,
                                      data={'loginName': self.username, 'nloginpwd': self.password}).text
        if 'true' in auth_page:
            auth_code_url = soup.select('#JD_Verification1')[0].get('src2')
            auth_code = str(self.get_auth_img(auth_code_url))
        else:
            auth_code = ''

        data = {
            'uuid': uuid,
            'eid': eid,
            'fp': fp,
            '_t': _t,
            'loginType': login_type,
            'loginname': self.username,
            'nloginpwd': self.password,
            'chkRememberMe': True,
            'pubKey': pub_key,
            'sa_token': sa_token,
            'authcode': auth_code
            }
        return data

    def get_auth_img(self, url):
        auth_code_url = 'http:{}&yys={}'.format(url, str(int(time.time()*1000)))
        auth_img = self.session.get(auth_code_url, headers=self.headers)
        with open('authcode.jpg', 'wb') as f:
            f.write(auth_img.content)
        code_typein = input('请根据下载图片输入验证码：')
        return code_typein

    def login(self):
        data = self.get_login_info()
        headers = {
                    'Referer': self.post_url,
                    'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36'
                                  ' (KHTML, like Gecko) Chrome/62.0.3202.89 Safari/537.36',
                    'X-Requested-With': 'XMLHttpRequest'
                  }
        try:
            login_page = self.session.post(self.post_url, data=data, headers=headers)
            print(login_page.text)
        except Exception as e:
            print(e)

        # self.session.cookies.clear()

    def shopping(self):
        login = self.session.post('https://cart.jd.com/cart.action', headers=self.headers)
        print(login.text)


if __name__ == '__main__':
    un = input('请输入京东账号：')
    pwd = input('请输入京东密码：')
    jd = JD_crawl(un, pwd)
    jd.login()
    jd.shopping()
```



**利用Python爬取喜马拉雅音频文件**



```
import re
import requests
from lxml import etree
from onexima import Xima


def get_id():
    """获取排行榜每一本书的信息"""
    main_url = "https://www.ximalaya.com/shangye/top/"
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36"
    }
    r = requests.get(main_url, headers=headers)
    # 获取到当前页面的xml数据
    html = etree.HTML(r.content.decode())
    # 得到每一本书的位置的信息
    div_list = html.xpath("//div[contains(@class,'e-2997888007 rrc-album-item')]")
    all_lsit = []  # 待会把每一本书的音频以字典形式放进列表当中
    for div in div_list:
        author = {}  # 创建一个列表, 我们要获取书的id和书的名字, 并且一一对应
        r = div.xpath("./a/@href")[0]  # 获取到当前书的id所在信息, 数据为: /renwen/4859823/
        print(r)
        # 所以得通过正则把正确的id取出来, id是为了传入正确的id, 得到正确的json数据
        author['id'] = re.search(r'\/.*?\/(.*)\/', r).group(1)
        author['book_name'] = div.xpath("./a/div[3]/div[1]/span/text()")[0]
        # 向列表中传入每一个音频的信息
        all_lsit.append(author)
    print(all_lsit)
    return all_lsit


# 调用函数得到所有每一本书的信息, 是一个列表类型
all_lsit = get_id()
for i in all_lsit:
    # 遍历列表, 把每本书对应的id和对应的书名传到类里面去
    x = Xima(i['id'], i['book_name'])
    x.run()
```



**利用Python爬取妹子图**

```
import requests
from bs4 import BeautifulSoup
import os
import re

Hostreferer = {
    'User-Agent':'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)',
    'Referer':'http://www.mzitu.com'
}
Picreferer = {
    'User-Agent':'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)',
    'Referer':'http://i.meizitu.net'
}

def get_page_name(url):#获得图集最大页数和名称
    html = get_html(url)
    soup = BeautifulSoup(html, 'lxml')
    span = soup.findAll('span')
    title = soup.find('h2', class_="main-title")
    return span[10].text, title.text

def get_html(url):#获得页面html代码
    req = requests.get(url, headers=Hostreferer)
    html = req.text
    return html

def get_img_url(url, name):
    html = get_html(url)
    soup = BeautifulSoup(html, 'lxml')
    img_url = soup.find('img', alt= name)
    return img_url['src']

def save_img(img_url, count, name):
    req = requests.get(img_url, headers=Picreferer)
    new_name = rename(name)
    with open(new_name+'/'+str(count)+'.jpg', 'wb') as f:
        f.write(req.content)

def rename(name):
    rstr = r'[\/\\\:\*\?\<\>\|]'
    new_name = re.sub(rstr, "", name)
    return new_name

def save_one_atlas(old_url):
    page, name = get_page_name(old_url)
    new_name = rename(name)
    os.mkdir(new_name)

    print("图集--" + name + "--开始保存")
    for i in range(1, int(page)+1):
        url = old_url + "/" + str(i)
        img_url = get_img_url(url, name)
        # print(img_url)
        save_img(img_url, i, name)
        print('正在保存第' + str(i) + '张图片')
    print("图集--" + name + "保存成功")


def get_atlas_list(url):
    req = requests.get(url, headers=Hostreferer)
    soup = BeautifulSoup(req.text, 'lxml')
    atlas = soup.find_all(attrs={'class':'lazy'})
    atlas_list = []
    for atla in atlas:
        atlas_list.append(atla.parent['href'])
    return atlas_list

def save_one_page(start_url):
    atlas_url = get_atlas_list(start_url)
    for url in atlas_url:
        save_one_atlas(url)


if __name__ == '__main__':
    start_url = "http://www.mzitu.com/"
    for count in range(1, 3):
        url = start_url + "page/" + str(count) +"/"
        save_one_page(url)
    print("爬取完成")
```



**Python制作微信朋友圈九宫图**



```
from PIL import Image  
import sys  
#先将 input image 填充为正方形  
def fill_image(image):  
    width, height = image.size      
    #选取长和宽中较大值作为新图片的  
    new_image_length = width if width > height else height      
    #生成新图片[白底]  
    new_image = Image.new(image.mode, (new_image_length, new_image_length), color='white')   #注意这个函数！  
    #将之前的图粘贴在新图上，居中   
    if width > height:#原图宽大于高，则填充图片的竖直维度  #(x,y)二元组表示粘贴上图相对下图的起始位置,是个坐标点。  
        new_image.paste(image, (0, int((new_image_length - height) / 2)))  
    else:  
        new_image.paste(image, (int((new_image_length - width) / 2),0))      
    return new_image  
def cut_image(image):
    width, height = image.size
    item_width = int(width / 3)  #因为朋友圈一行放3张图。
    box_list = []
    # (left, upper, right, lower)
    for i in range(0,3):
        for j in range(0,3):
            #print((i*item_width,j*item_width,(i+1)*item_width,(j+1)*item_width))
            box = (j*item_width,i*item_width,(j+1)*item_width,(i+1)*item_width)
            box_list.append(box)
    image_list = [image.crop(box) for box in box_list]
    return image_list
#保存  
def save_images(image_list):  
    index = 1   
    for image in image_list:  
        image.save(str(index) + '.png', 'PNG')  
        index += 1  
if __name__ == '__main__':  
    file_path = "4.jpg"  
    image = Image.open(file_path)     
    #image.show()  
    image = fill_image(image)  
    image_list = cut_image(image)  
    save_images(image_list)
```



**利用Python爬取LOL官网**

```
# -*- coding:utf-8 -*-

import requests
import re
import json
#获取JS源代码  获取英雄的ID
#拼接URL地址
#获取下载图片的地址
#下载图片

#驼峰命名法
#获取英雄图片
def getLOLImages():
    header = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.104 Safari/537.36'}
    url_js = 'http://lol.qq.com/biz/hero/champion.js'
    #获取JS源代码 str bytrs字节
    res_js = requests.get(url_js).content
    #转码
    html_js = res_js.decode()
    #正则表达
    req = '"keys":(.*?),"data"'
    list_js = re.findall(req,html_js)
    #print(list_js[0])

    # str → dict
    dict_js = json.loads(list_js[0])
    #print(dict_js)
    #定义图片列表
    pic_list = []
    for key in dict_js:
        #print(key)
        for i in range(20):
            num = str(i)
            if len(num) == 1:
                hreo_num = "00"+num
            elif len(num) == 2:
                hreo_num = "0"+num
            numstr = key+hreo_num
            url = "http://ossweb-img.qq.com/images/lol/web201310/skin/big"+numstr+".jpg"
            #print(url)
            pic_list.append(url)

            list_filepath = []
            path = "E:\\文章\\LOL官网\LOLpic\\"
            #print(dict_js.values())
            for name in dict_js.values():
                for i in range(20):
                    file_path = path + name + str(i) + '.jpg'
                    list_filepath.append(file_path)
                    #print(list_filepath)
    n = 0               
    for picurl in pic_list:
        res = requests.get(picurl)
        n+=1

        if res.status_code ==200:

            print("正在下载%s"%list_filepath[n])
            #time.sleep(1)
            with open(list_filepath[n],'wb') as f:
                f.write(res.content)
getLOLImages()
```



