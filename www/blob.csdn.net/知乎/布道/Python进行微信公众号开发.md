# Python进行微信公众号开发 - 知乎
# 



## 一、微信公众号的准备：

## 1. 注册

访问地址：[https://mp.weixin.qq.com/](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/)
按照提示注册即可 
注意：本文样例使用个人公众号，由于个人公众号没有接口权限，自定义菜单无法进行开发，同学们不要注册错！ 

![](https://pic3.zhimg.com/v2-744930e2871262858f12c4cc3cb21cfa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1171' height='534'></svg>)
申请公众号信息如下，也可扫码关注

此微信公众号致力于AI、开发、安全、数据、运维方向技术推广和技术传播，期待传播技术和知识推广的您加入。点击扫描二维码：
[https://img.wuage.com/1.jpg​img.wuage.com](https://link.zhihu.com/?target=https%3A//img.wuage.com/1.jpg)
2. 配置

需要准备好自己的服务器地址，可以使用腾讯云阿里云京东云等等皆可，没有域名用IP也可以。 
在公众平台官网的开发-基本设置页面，勾选协议成为开发者，点击“修改配置”按钮，填写服务器地址（URL）、Token和EncodingAESKey，其中URL是开发者用来接收微信消息和事件的接口URL。Token可由开发者可以任意填写，用作生成签名（该Token会和接口URL中包含的Token进行比对，从而验证安全性）。EncodingAESKey由开发者手动填写或随机生成，将用作消息体加解密密钥。 
[微信公众平台​mp.weixin.qq.com](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/wiki%3Ft%3Dresource/res_main%26id%3Dmp1421135319)
按照要求对开发基本信息配置，如下：
![](https://pic1.zhimg.com/v2-e8e82054390ac62a11d38bc2c7b14a4c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='985' height='621'></svg>)
## 3. 连接

服务器配置提交后，微信服务器发送GET请求到填写的服务器地址URL上，GET请求携带参数包括signature、timestamp、nonce、echostr，服务端程序通过检验signature对请求进行校验（下面有校验方式）。若确认此次GET请求来自微信服务器，请原样返回echostr参数内容给微信服务器确认后接入生效。加密/校验流程如下，下文中有代码具体实现： 
1）将token、timestamp、nonce三个参数进行字典序排序 
2）将三个参数字符串拼接成一个字符串进行sha1加密 
3）开发者获得加密后的字符串可与signature对比，标识该请求来源于微信

## 二、基于Django开发微信公众号后台的步骤：

## 1.准备工作
- 新建 django project，项目名称：weixin

`django-admin.py startproject weixin`
- 新建 app 
进入项目目录下，cd weixin然后执行下面的命令

`python manage.py startapp weixin`

或

`django-admin.py startapp weixin`
- 修改settings.py加入刚才创建的weixin应用


```
INSTALLED_APPS = (
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'weixin',
)
```

- 修改项目同名app内的urls.py,添加新的weixin的路由解析跳转


```
urlpatterns = [
    url(r'^admin/', include(admin.site.urls)),
    url(r'^weixin/', include('weixin.urls',namespace="weixin")),
   ]
```


## 2.编写weixin应用的urls.py

修改app weixin内的urls.py,添加针对视图函数的解析，默认访问指向视图函数的weixin_index方法。


```
app_name = 'weixin'

urlpatterns = [
    url(r'^admin/', include(admin.site.urls)),
    url(r'^$', views.index, name='weixin_index'),
    ]
```


## 3.编写weinxin应用的views.py

get方法做校验,post方法传xml数据,解析和拼凑xml数据实现自动回复功能


```
from django.http.response import HttpResponse, HttpResponseBadRequest
from django.views.decorators.csrf import csrf_exempt
from wechat_sdk import WechatBasic
from wechat_sdk.exceptions import ParseError
from wechat_sdk.messages import TextMessage
# Create your views here.

wechat_instance = WechatBasic(
    token='您设置的token',
    appid='您的公众号id',
    appsecret='您的公众号的秘钥'
)

@csrf_exempt
def index(request):
    if request.method == 'GET':
        # 检验合法性
        # 从 request 中提取基本信息 (signature, timestamp, nonce, xml)
        signature = request.GET.get('signature')
        timestamp = request.GET.get('timestamp')
        nonce = request.GET.get('nonce')

        if not wechat_instance.check_signature(
                signature=signature, timestamp=timestamp, nonce=nonce):
            return HttpResponseBadRequest('Verify Failed')

        return HttpResponse(
            request.GET.get('echostr', ''), content_type="text/plain")

    # 解析本次请求的 XML 数据
    else:
        try:
            print(request.body)
            wechat_instance.parse_data(data=request.body)
        except ParseError:
            return HttpResponseBadRequest('Invalid XML Data')

        # 获取解析好的微信请求信息
        message = wechat_instance.get_message()

        # 关注事件以及不匹配时的默认回复
        response = wechat_instance.response_text(
            content=(
                '感谢您的关注！\n回复【功能】两个字查看支持的功能，还可以回复任意内容开始聊天'
            ))
        if isinstance(message, TextMessage):
            # 当前会话内容
            content = message.content.strip()
            if content == '功能':
                reply_text = (
                    '目前支持的功能：\n1. 关键词后面加上【教程】两个字可以搜索教程，'
                    '比如回复 "Django 后台教程"\n'
                    '2.回复任意词语，查天气，陪聊天，讲故事，无所不能！\n'
                    '还有更多功能正在开发中哦 ^_^\n'
                )
            response = wechat_instance.response_text(content=reply_text)
        print(response)
        return HttpResponse(response, content_type="application/xml")
```


## 3.配置公网域名映射后，运行程序，效果展示

`python manage.py runserver 0.0.0.0:80`
![](https://pic1.zhimg.com/v2-a8fe10bbfb7e11f492b185262c9bdc2c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='750' height='1334'></svg>)
发现不能正常使用，提示“该公众号提供的服务出现故障，请稍后再试”。

于是百度里各种找资料，又是查看微信公众号开发文档，各种百度和google，都没有解决。微信公众平台关闭启用，重置密钥。仔细核对填写，都没有问题。没有填写错误。

我开启了开发-运维中心-接口报警-告警规则全部调到“1次/5分钟”，偶尔会收到这样的报错：302网页重定向出错。
![](https://pic3.zhimg.com/v2-beb907199df0ca3951d1170b79e5e786_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='355' height='603'></svg>)
**最后，我就查到微信公众平台-开发-基本配置-服务器配置-服务器地址（url），这里的http://头改成https://。**
![](https://pic3.zhimg.com/v2-6f0dbe43f60562f9990bd0fdc416b342_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1008' height='297'></svg>)
再试自动回复真的可以了。
![](https://pic4.zhimg.com/v2-1a7bf17feb3eb505a006e67df09d760b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='750' height='1334'></svg>)
**如果对您有帮助，不如给个鼓励，点个赞。欢迎大家有空时翻下我牌子([知乎号：布道](https://www.zhihu.com/people/dockersky/columns) )，看看“[开发运维](https://zhuanlan.zhihu.com/idevops)”专栏的文章！**


