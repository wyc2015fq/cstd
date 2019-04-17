# Python模拟京东登录（附完整代码） - 知乎
# 



来源：IMyxuan    

链接：[https://segmentfault.com/a/1190000011978866](https://link.zhihu.com/?target=https%3A//segmentfault.com/a/1190000011978866)




**分析登录的网络请求**

　　打开[https://passport.jd.com/new/login.aspx](https://link.zhihu.com/?target=https%3A//passport.jd.com/new/login.aspx)

打开浏览器的调试面板，然后，输入账号密码 =>> 登录，然后看调试面板上面的Network捕捉到的网络请求，如下图：
![](https://pic4.zhimg.com/v2-1a28cd0a3129595b5a77f0c4bcad4aef_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='235'></svg>)![](https://pic2.zhimg.com/v2-554800ffb5bf3d3b8cacec71fde485f1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='245'></svg>)![](https://pic3.zhimg.com/v2-d9d7e91a11476c6ce656ed7e9088ac72_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='363'></svg>)
根据上面的图片，我们可以知道，在我们点击完登录按钮之后，浏览器发送了一个POST请求到[https://passport.jd.com/uc/loginService](https://link.zhihu.com/?target=https%3A//passport.jd.com/uc/loginService)，然后在请求头上面带上了一些基本的参数，最重要的有三个，一个是cookie，一个是Referer，还有一个是User-Agent。




然后，再看看请求所需要的参数，这里面有两部分，一个是Query Params：


```
uuid:6a9ddda4-1819-4819-a888-b59301f83a59
ReturnUrl：https://item.jd.com/5089239.html
r: 0.2520828231526894
version: 2015
```


另一个是Form Data：


```
uuid:6a9ddda4-1819-4819-a888-b59301f83a59
eid:F37T4YRBOPZZ6JSEGDA7WH2VCIETQY…J23JGCTD3IM4TMK44Y5ATOTO6G4SM
fp:87783198d64a9aa25b3f0b28b152b094
_t:_t
loginType:f
loginname:test@qq.com
nloginpwd:P+TbkVGbhhhC4DFlZ7y4az8j/Y1QYw…h0qs8+bKp7UbHeGWF1KEMrQUG5po=
chkRememberMe:    
authcode:
pubKey: MIGfMA0GCSqGSIb3DQEBAQUAA4GNAD…woanQq+CA6agNkqly2H4j6wIDAQAB
sa_token: B68C442BE645754F33277E70120805…8EF504D2CD67D7341B8BFBA47FFA5
```


上面的参数里面，基本可以在页面上面可以找到
![](https://pic3.zhimg.com/v2-997c7df9852afb3a73933979d35c1372_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='165'></svg>)
其中：ReturnUrl是登录成功后返回的页面，authcode是图片验证码，本例子上面不需要输入验证码。




**Python实现**

根据上面的分析我们只需要通过代码发送一个post请求，并且带上相对应的参数就可以实现登录了。所有的参数，我们都可以轻而易举地获取，唯一的困难是cookie，但是，Python的requests库给我们提供了会话机制，也就是requests.Session(),因此，具体的实现过程可以如代码所示。




**完整代码**


```
# -*- coding: UTF-8 -*-
import json
import requests
import sys

from bs4 import BeautifulSoup

s = requests.Session()

class JD:
    def __init__(self, username, password):
        self.username = username
        self.password = password
        self.headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:56.0) Gecko/20100101 Firefox/56.0',
            'Referer': 'https://www.jd.com/'
        }

    def get_login_data(self):
        url = 'https://passport.jd.com/new/login.aspx'
        html = s.get(url, headers=self.headers).content
        soup = BeautifulSoup(html, 'lxml')
        display = soup.select('#o-authcode')[0].get('style')
        auth_code = ''
        if not display:
            print('需要验证码。。。')
            auth_code_url = soup.select('#JD_Verification1')[0].get('src2')
            auth_code = self.get_auth_img(auth_code_url)
        uuid = soup.select('#uuid')[0].get('value')
        eid = soup.select('#eid')[0].get('value')
        fp = soup.select('input[name="fp"]')[0].get('value')  # session id
        _t = soup.select('input[name="_t"]')[0].get('value')  # token
        login_type = soup.select('input[name="loginType"]')[0].get('value')
        pub_key = soup.select('input[name="pubKey"]')[0].get('value')
        sa_token = soup.select('input[name="sa_token"]')[0].get('value')

        data = {
            'uuid': uuid,
            'eid': eid,
            'fp': fp,
            '_t': _t,
            'loginType': login_type,
            'loginname': self.username,
            'nloginpwd': self.password,
            'chkRememberMe': True,
            'authcode': '',
            'pubKey': pub_key,
            'sa_token': sa_token,
            'authCode': auth_code
        }
        return data

    def get_auth_img(self, url):
        auth_code_url = 'http:' + url
        auth_img = s.get(auth_code_url, headers=self.headers)
        with open(sys.path[0] + '/auth.jpg', 'wb') as f:
            f.write(auth_img.content)
        code = input('请输入验证码：')
        return code

    def login(self):
        """
        登录
        :return:
        """
        url = 'https://passport.jd.com/uc/loginService'
        data = self.get_login_data()
        headers = {
            'Referer': 'https://passport.jd.com/uc/login?ltype=logout',
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:56.0) Gecko/20100101 Firefox/56.0',
            'X-Requested-With': 'XMLHttpRequest'
        }
        content = s.post(url, data=data, headers=headers).text
        result = json.loads(content[1: -1])
        return result

    def rush(self):
        print('功能正在赶来的路上，敬请期待。。。')
        pass

def handle():
    print("*************** 菜单列表 **************")
    print('1、抢购')
    print('2、加入购物车')
    num = input('请输入功能编号：')
    if num == '1':
        print('抢购功能正在赶来的路上，敬请期待。。。')
    else:
        print('加入购物车功能正在赶来的路上，敬请期待。。。')
        # print('加入购物车成功！！！')
    pass

username = input('请输入京东账号：')
password = input('请输入京东密码：')
jd = JD(username, password)
result = jd.login()
if result.get('success'):
    print('登录成功')
    handle()
else:
    print('登录失败')
```





 今晚8点公开课 |【2节课从零实现区块链】第一节课将直播

还有几个名额，感兴趣的小伙伴抓紧微信扫海报二维码预约
![](https://pic1.zhimg.com/v2-e8a5a70fb5e0cca34d4e73a55831436c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='1940'></svg>)



