# Python爬取并闪存微信群里的百度云资源 - zhusongziye的博客 - CSDN博客





2018年11月24日 15:56:54[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：65标签：[Python爬取并闪存微信群里的百度云资源](https://so.csdn.net/so/search/s.do?q=Python爬取并闪存微信群里的百度云资源&t=blog)
个人分类：[Python爬虫](https://blog.csdn.net/zhusongziye/article/category/7611456)









**需求背景：**

最近误入一个免费（daoban）资源的分享群（正经脸），群里每天都在刷资源链接。但是大家都知道，百度云的分享链接是很容易被河蟹的，群里除了分享链接外，就是各种抱怨 “怎么又失效了”，“又河蟹了...”。本着学习技术的初心，于是我就开始研究怎样自动爬取微信群的消息并自动转存到自己的云盘。

![](https://img-blog.csdnimg.cn/20181124155703715.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**需求：**

1、爬取微信群里的百度云分享链接

2、将资源转存到自己的网盘

**涉及：**

1、正则表达式

2、如何分析cookie和api

3、selenium（webdriver）

**本篇文章目录：**

1.、爬取微信群聊信息里的网盘资源

2、寻找并分析百度云的转存api

3、爬取shareid、from、filelist，发送请求转存到网盘

4、完整代码

5、参考

# 爬取微信群聊信息里的网盘资源

爬取微信群聊信息可以用微信网页版的api，这里推荐一个高度封装，使用简单的工具：wxpy: 用 Python 玩微信

![](https://img-blog.csdnimg.cn/20181124155727178.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



这是个不错的工具，可以实现网页版微信的所有功能，之前博主利用它还实现了机器人聊天功能，自己的个人账号华丽转变 **微软小冰** 。虽然微软还没有公开小冰的 api ，不过我们完全可以利用这个工具的转发功能来实现，思路很简单，微信上领养一个小冰，把别人说的话转发给小冰，再把小冰说的话转发回去。

![](https://img-blog.csdnimg.cn/201811241557416.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

扯远了，总之利用这个工具，就可以对微信群聊里的信息进行监听，接着就是利用正则表达式把网盘链接抓取出来。具体的代码我就不贴了，使用起来很简单。

# 寻找并分析百度云的转存api

这个部分才是我们的重点，首先你得有一个百度云盘的账号，然后登录，用浏览器（这里用火狐浏览器做示范）打开一个分享链接。F12打开控制台进行抓包。手动进行转存操作：全选文件->保存到网盘->选择路径->确定。点击【确定】前建议先清空一下抓包记录，这样可以精确定位到转存的api，这就是我们中学时学到的【控制变量法】2333。

![](https://img-blog.csdnimg.cn/20181124155756485.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

可以看到上图中抓到了一个带有 “transfer” 单词的 post 请求，这就是我们要找的转存（transfer）api 。接下来很关键，就是分析它的请求头和请求参数，以便用代码模拟。
`https://pan.baidu.com/share/transfer?shareid=3927175953&from=140959320&ondup=newcopy&async=1&bdstoken=xxx&channel=chunlei&clienttype=0&web=1&app_id=250528&logid=xxx`
点击它，再点击右边的【Cookies】就可以看到请求头里的 cookie 情况。

![](https://img-blog.csdnimg.cn/20181124155805320.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**cookie分析：**

因为转存是登录后的操作，所以需要模拟登录状态，将与登录有关的 cookie 设置在请求头里。我们继续使用【控制变量法】，先将浏览器里关于百度的 cookie 全部删除（在右上角的设置里面，点击【隐私】，移除cookies。具体做法自己百度吧。）

然后登录，右上角进入浏览器设置->隐私->移除cookie，搜索 "bai" 观察 cookie 。这是所有跟百度相关的 cookie ，一个个删除，删一个刷新一次百度的页面，直到删除了**BDUSS** ，刷新后登录退出了，所以得出结论，它就是与登录状态有关的 cookie 。

同理，删除掉 **STOKEN** 后，进行转存操作会提示重新登录。所以，这两个就是转存操作所必须带上的 cookie 。

弄清楚了 cookie 的情况，可以像下面这样构造请求头。

```
def __init__(self,bduss,stoken,bdstoken):
        self.bdstoken = bdstoken
        self.headers = {
            'Accept': '*/*',
            'Accept-Encoding': 'gzip, deflate, br',
            'Accept-Language': 'zh-CN,zh;q=0.8',
            'Connection': 'keep-alive',
            'Content-Length': '161',
            'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8',
            'Cookie': 'BDUSS=%s;STOKEN=%s;' % (bduss, stoken),
            'Host': 'pan.baidu.com',
            'Origin': 'https://pan.baidu.com',
            'Referer': 'https://pan.baidu.com/s/1dFKSuRn?errno=0&errmsg=Auth%20Login%20Sucess&&bduss=&ssnerror=0',           
            'User-Agent': self.pro.get_user_agent(),# 作为应对反爬虫机制的策略，这是博主写的一个随机抽取user_agent的方法，你也可以自己到网上去搜集一些，写一个随机方法。
            'X-Requested-With': 'XMLHttpRequest',
        }
```

除了上面说到的两个 cookie ，其他的请求头参数可以参照手动转存时抓包的请求头。这两个 cookie 预留出来做参数的原因是 cookie 都是有生存周期的，过期了需要更新，不同的账号登录也有不同的 cookie 。

**参数分析：**

接下来分析参数，点击【Cookies】右边的【Params】查看参数情况。如下：

![](https://img-blog.csdnimg.cn/20181124155820872.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

上面的query string（也就是？后跟的参数）里，除了框起来的shareid、from、bdstoken需要我们填写以外，其他的都可以不变，模拟请求的时候直接抄下来。

前两个与分享的资源有关，bdstoken与登录的账号有关。下面的form data里的两个参数分别是资源在分享用户的网盘的所在目录和刚刚我们点击保存指定的目录。

所以，需要我们另外填写的参数为：**shareid、from、bdstoken、filelist 和 path**，bdstoken 可以手动转存抓包找到，path 根据你的需要自己定义，前提是你的网盘里有这个路径。其他三个需要从分享链接里爬取，这个将在后面的**【爬取shareid、from、filelist，发送请求转存到网盘】**部分中进行讲解。

搞清楚了参数的问题，可以像下面这样构造转存请求的 url 。

```
def transfer(self,share_id,uk,filelist_str,path_t_save):# 需要填写的参数，分别对应上图的shareid、from、filelist 和 path
        # 通用参数
        ondup = "newcopy"
        async = "1"
        channel = "chunlei"
        clienttype = "0"
        web = "1"
        app_id = "250528"
        logid = "你的logid"

        url_trans = "https://pan.baidu.com/share/transfer?shareid=%s" \
                    "&from=%s" \
                    "&ondup=%s" \
                    "&async=%s" \
                    "&bdstoken=%s" \
                    "&channel=%s" \
                    "&clienttype=%s" \
                    "&web=%s" \
                    "&app_id=%s" \
                    "&logid=%s" % (share_id, uk, ondup, async, self.bdstoken, channel, clienttype, web, app_id, logid)

        form_data = {
            'filelist': filelist_str,
            'path': path_t_save,
        }

        proxies = {'http': self.pro.get_ip(0, 30, u'国内')}# 为了应对反爬虫机制，这里用了代理，稍后做解释

        response = requests.post(url_trans, data=form_data, proxies = proxies,headers=self.headers)
        print response.content

        jsob = json.loads(response.content)

        if "errno" in jsob:
            return jsob["errno"]
        else:
            return None
```

# 爬取shareid、from、filelist，发送请求转存到网盘

```
https://pan.baidu.com/s/1jImSOXg
```



以上面这个资源链接为例（随时可能被河蟹，但是没关系，其他链接的结构也是一样的），我们先用浏览器手动访问，F12 打开控制台先分析一下源码，看看我们要的资源信息在什么地方。控制台有搜索功能，直接搜 “shareid”。

![](https://img-blog.csdnimg.cn/20181124155839331.png)

定位到4个shareid，前三个与该资源无关，是其他分享资源，最后一个定位到该 html 文件的最后一个<script></script>标签块里。双击后可以看到格式化后的 js 代码，可以发现我们要的信息全都在里边。如下节选：

```
yunData.SHAREPAGETYPE = "multi_file";

        yunData.MYUK = "";
        yunData.SHARE_USER_NAME = "wang*****2121";
        yunData.SHARE_ID = "3927175953";
        yunData.SIGN = "7f166e9b5cf54486074ccce2fc0548e8aa50bdfb";
        yunData.sign = "7f166e9b5cf54486074ccce2fc0548e8aa50bdfb";
        yunData.TIMESTAMP = "1502175170";

        yunData.SHARE_UK = "140959320";
        yunData.SHARE_PUBLIC = 0;
        yunData.SHARE_TIME = "1502150259";
        yunData.SHARE_DESCRIPTION = "";
        yunData.MYSELF = +false;
        yunData.MYAVATAR = "";

                    yunData.NOVELID = "";
                yunData.FS_ID = "3097042711872";
        yunData.FILENAME = "归来的福丹芝（58）";
        yunData.PATH = "\/我的资源\/归来的福丹芝（58）";
        yunData.PATH_MD5 = "4215521821664681584";
        yunData.CTIME = "1502150259";
        yunData.CATEGORY = "6";
                    yunData.DOCPREVIEW = "";
                            yunData.IS_BAIDUSPIDER = "";
                            yunData.ARTISTNAME = "";
                            yunData.ALBUMTITLE = "";
                            yunData.TRACKTITLE = "";
                            yunData.FILEINFO = [{"fs_id":3097042711872,"app_id":"250528","parent_path":"%2F%E6%88%91%E7%9A%84%E8%B5%84%E6%BA%90","server_filename":"归来的福丹芝（58）","size":0,"server_mtime":1502150215,"server_ctime":1494903797,"local_mtime":1494903797,"local_ctime":1494903797,"isdir":1,"isdelete":"0","status":"0","category":6,"share":"0","path_md5":"4215521821664681584","delete_fs_id":"0","extent_int3":"0","extent_tinyint1":"0","extent_tinyint2":"0","extent_tinyint3":"0","extent_tinyint4":"0","path":"\/我的资源\/归来的福丹芝（58）","root_ns":1653016720,"md5":"","file_key":""},{"fs_id":1057404338934434,"app_id":"250528","parent_path":"%2F%E6%88%91%E7%9A%84%E8%B5%84%E6%BA%90","server_filename":"多样的儿媳（46）","size":0,"server_mtime":1502150223,"server_ctime":1496696760,"local_mtime":1496696760,"local_ctime":1496696760,"isdir":1,"isdelete":"0","status":"0","category":6,"share":"0","path_md5":"5972282562760833248","delete_fs_id":"0","extent_int3":"0","extent_tinyint1":"0","extent_tinyint2":"0","extent_tinyint3":"0","extent_tinyint4":"0","path":"\/我的资源\/多样的儿媳（46）","root_ns":1653016720,"md5":"","file_key":""}];
```

可以看到这两行

```
yunData.SHARE_ID = "3927175953";
yunData.SHARE_UK = "140959320"; // 经过对比，这就是我们要的 "from"
```

yunData.PATH 只指向了一个路径信息，完整的 filelist 可以从 yunData.FILEINFO 里提取，它是一个 json ，list 里的信息是Unicode编码的，所以在控制台看不到中文，用Python代码访问并获取输出一下就可以了。

直接用request请求会收获 404 错误，可能是需要构造请求头参数，不能直接请求，这里博主为了节省时间，直接用selenium的webdriver来get了两次，就收到了返回信息。第一次get没有任何 cookie ，但是baidu 会给你返回一个BAIDUID ，在第二次 get 就可以正常访问了。

yunData.FILEINFO 结构如下，你可以将它复制粘贴到json.cn里，可以看得更清晰。



```
[{"fs_id":3097042711872,"app_id":"250528","parent_path":"%2F%E6%88%91%E7%9A%84%E8%B5%84%E6%BA%90","server_filename":"归来的福丹芝（58）","size":0,"server_mtime":1502150215,"server_ctime":1494903797,"local_mtime":1494903797,"local_ctime":1494903797,"isdir":1,"isdelete":"0","status":"0","category":6,"share":"0","path_md5":"4215521821664681584","delete_fs_id":"0","extent_int3":"0","extent_tinyint1":"0","extent_tinyint2":"0","extent_tinyint3":"0","extent_tinyint4":"0","path":"\/我的资源\/归来的福丹芝（58）","root_ns":1653016720,"md5":"","file_key":""},{"fs_id":1057404338934434,"app_id":"250528","parent_path":"%2F%E6%88%91%E7%9A%84%E8%B5%84%E6%BA%90","server_filename":"多样的儿媳（46）","size":0,"server_mtime":1502150223,"server_ctime":1496696760,"local_mtime":1496696760,"local_ctime":1496696760,"isdir":1,"isdelete":"0","status":"0","category":6,"share":"0","path_md5":"5972282562760833248","delete_fs_id":"0","extent_int3":"0","extent_tinyint1":"0","extent_tinyint2":"0","extent_tinyint3":"0","extent_tinyint4":"0","path":"\/我的资源\/多样的儿媳（46）","root_ns":1653016720,"md5":"","file_key":""}]
```

![](https://img-blog.csdnimg.cn/20181124155924187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

清楚了这三个参数的位置，我们就可以用正则表达式进行提取了。代码如下：

```
from wechat_robot.business import proxy_mine # 这是我自己的代理类，测试时可以先用本机ip

pro = proxy_mine.Proxy()
url = "https://pan.baidu.com/s/1jImSOXg"
driver = webdriver.Chrome()
print u"初始化代理..."
driver = pro.give_proxy_driver(driver)

def get_file_info(url):
    driver.get(url)
    time.sleep(1)
    driver.get(url)

    script_list = driver.find_elements_by_xpath("//body/script")
    innerHTML = script_list[-1].get_attribute("innerHTML")# 获取最后一个script的innerHTML

    pattern = 'yunData.SHARE_ID = "(.*?)"[\s\S]*yunData.SHARE_UK = "(.*?)"[\s\S]*yunData.FILEINFO = (.*?);[\s\S]*'  # [\s\S]*可以匹配包括换行的所有字符,\s表示空格，\S表示非空格字符
    srch_ob = re.search(pattern, innerHTML)

    share_id = srch_ob.group(1)
    share_uk = srch_ob.group(2)

    file_info_jsls = json.loads(srch_ob.group(3))# 解析json
    path_list = []
    for file_info in file_info_jsls:
        path_list.append(file_info['path'])

    return share_id,share_uk,path_list

try:
    print u"发送连接请求..."
    share_id,share_uk,path_list = get_file_info(url)
except:
    print u"链接失效了，没有获取到fileinfo..."
else:
    print share_id
    print share_uk
    print path_list
```

爬取到了这三个参数，就可以调用之前的 transfer 方法进行转存了。

# 完整代码

```
# -*- coding:utf-8 -*-
import requests
import json
import time
import re
from selenium import webdriver
from wechat_robot.business import proxy_mine

class BaiduYunTransfer:

    headers = None
    bdstoken = None
    pro = proxy_mine.Proxy()

    def __init__(self,bduss,stoken,bdstoken):
        self.bdstoken = bdstoken
        self.headers = {
            'Accept': '*/*',
            'Accept-Encoding': 'gzip, deflate, br',
            'Accept-Language': 'zh-CN,zh;q=0.8',
            'Connection': 'keep-alive',
            'Content-Length': '161',
            'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8',
            'Cookie': 'BDUSS=%s;STOKEN=%s;' % (bduss, stoken),
            'Host': 'pan.baidu.com',
            'Origin': 'https://pan.baidu.com',
            'Referer': 'https://pan.baidu.com/s/1dFKSuRn?errno=0&errmsg=Auth%20Login%20Sucess&&bduss=&ssnerror=0',
            'User-Agent': self.pro.get_user_agent(),
            'X-Requested-With': 'XMLHttpRequest',
        }

    def transfer(self,share_id,uk,filelist_str,path_t_save):
        # 通用参数
        ondup = "newcopy"
        async = "1"
        channel = "chunlei"
        clienttype = "0"
        web = "1"
        app_id = "250528"
        logid = "你的logid"

        url_trans = "https://pan.baidu.com/share/transfer?shareid=%s" \
                    "&from=%s" \
                    "&ondup=%s" \
                    "&async=%s" \
                    "&bdstoken=%s" \
                    "&channel=%s" \
                    "&clienttype=%s" \
                    "&web=%s" \
                    "&app_id=%s" \
                    "&logid=%s" % (share_id, uk, ondup, async, self.bdstoken, channel, clienttype, web, app_id, logid)

        form_data = {
            'filelist': filelist_str,
            'path': path_t_save,
        }

        proxies = {'http': self.pro.get_ip(0, 30, u'国内')}

        response = requests.post(url_trans, data=form_data, proxies = proxies,headers=self.headers)
        print response.content

        jsob = json.loads(response.content)

        if "errno" in jsob:
            return jsob["errno"]
        else:
            return None

    def get_file_info(self,url):
        driver = webdriver.Chrome()
        print u"初始化代理..."
        driver = self.pro.give_proxy_driver(driver)

        print u"尝试打开"
        driver.get(url)
        time.sleep(1)
        print u"正式打开链接"
        driver.get(url)
        print u"成功获取并加载页面"
        script_list = driver.find_elements_by_xpath("//body/script")
        innerHTML = script_list[-1].get_attribute("innerHTML")

        pattern = 'yunData.SHARE_ID = "(.*?)"[\s\S]*yunData.SHARE_UK = "(.*?)"[\s\S]*yunData.FILEINFO = (.*?);[\s\S]*'  # [\s\S]*可以匹配包括换行的所有字符,\s表示空格，\S表示非空格字符
        srch_ob = re.search(pattern, innerHTML)

        share_id = srch_ob.group(1)
        share_uk = srch_ob.group(2)

        file_info_jsls = json.loads(srch_ob.group(3))
        path_list_str = u'['
        for file_info in file_info_jsls:
            path_list_str += u'"' + file_info['path'] + u'",'

        path_list_str = path_list_str[:-1]
        path_list_str += u']'

        return share_id, share_uk, path_list_str

    def transfer_url(self,url_bdy,path_t_save):
        try:
            print u"发送连接请求..."
            share_id, share_uk, path_list = self.get_file_info(url_bdy)
        except:
            print u"链接失效了，没有获取到fileinfo..."
        else:
            error_code = self.transfer(share_id, share_uk, path_list, path_t_save)
            if error_code == 0:
                print u"转存成功！"
            else:
                print u"转存失败了，错误代码：" + str(error_code)

bduss = '你的BDUSS' 
stoken = '你的STOKEN'
bdstoken = "你的bdstoken"
bdy_trans = BaiduYunTransfer(bduss,stoken,bdstoken)

url_src = "https://pan.baidu.com/s/1jImSOXg"
path = u"/电影"

bdy_trans.transfer_url(url_src,path)
```



```
作者：王雨城
源自：https://www.jianshu.com/p/56f0c07e0ecb
```



