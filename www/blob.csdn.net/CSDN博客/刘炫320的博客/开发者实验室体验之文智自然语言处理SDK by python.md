# 开发者实验室体验之文智自然语言处理SDK by python - 刘炫320的博客 - CSDN博客
2017年08月02日 22:50:33[刘炫320](https://me.csdn.net/qq_35082030)阅读数：2089
# 1. 前言
今天我们将对于腾讯的文智自然语言处理SDK进行一次体验。
# 2. 实验
## 2.1 实验准备
新手可以去[文智管理平台](https://console.qcloud.com/nlp/manage_app.cgi)领取5万次新手礼包，并[创建一个新的API](https://console.qcloud.com/capi)的公钥和私钥。（果然是腾讯，连这个都成为新手礼包……游戏玩多了吧。）
## 2.2 创建工作目录
在根目录下创建 data 目录，之后操作中相关的代码均放置在此目录下(注：若目录已存在则直接跳过本步骤)。
`mkdir /data`
## 2.3 安装 Git 工具和 Python 环境
`yum install -y git python-pip`
## 2.4安装 qcloudapi-sdk-python
安装依赖包 requests
`pip install requests`
安装 qcloudapi-sdk-python
```php
cd /data && git clone https://github.com/QcloudApi/qcloudapi-sdk-python
```
# 3. 正式实验
## 3.1 情感分析代码
在 /data/qcloudapi-sdk-python 下创建 wenzhi.py 文件，Ctrl + S 
 保存，内容如下(注：将 SecretId 和 SecretKey 字段修改为对应取值):
```
cd qcloudapi-sdk-python
touch wenzhi.py
vim wenzhi.py
```
wenzhi.py内容如下：
```python
#!/usr/bin/python
# -*- coding: utf-8 -*-
# 引入云API入口模块
from src.QcloudApi.qcloudapi import QcloudApi
module = 'wenzhi'
'''
action 对应接口的接口名，请参考wiki文档上对应接口的接口名
'''
action = 'TextSentiment'
config = {
    'secretId': '之前取得的 secretId', 
    'secretKey': '之前取得的 secretKey', 
    'Region': 'gz',
    'method': 'POST'
}
'''
params 请求参数，请参考wiki文档上对应接口的说明
'''
params = {"content": "李亚鹏挺王菲：加油！孩他娘。"}
try:
    service = QcloudApi(module, config)
    # 生成请求的URL，不发起请求
    print service.generateUrl(action, params)
    # 调用接口，发起请求
    print service.call(action, params)
except Exception, e:
    print 'exception:', e
```
## 3.2 执行代码
执行以下命令，就可以得到对 “李亚鹏挺王菲：加油！孩儿他娘。” 这句话的情感分析结果。
`cd /data/qcloudapi-sdk-python && python wenzhi.py`
得到类似如下的结果， 证明调用成功。
```
https://wenzhi.api.qcloud.com/v2/index.php
{"code":0,"message":"","codeDesc":"Success","positive":0.99481022357941,"negative":0.0051898001693189}
```
各字段的含义如下： 
positive 正面情感概率 
negative 负面情感概率 
code 0表示成功，非0表示失败 
message 失败时候的错误信息，成功则无该字段
## 3.3 命名实体识别代码
只需修改wenzhi.py里这几行代码即可：
```
'''
action 对应接口的接口名，请参考wiki文档上对应接口的接口名
'''
action = 'LexicalAnalysis'
params = {"text": "苏州大学位于江苏省苏州市干将东路333号，是一个美丽的大学。",
"code":0x00200000,
"type":1
}
```
执行成功后为：
```
https://wenzhi.api.qcloud.com/v2/index.php
{"code":0,"message":"","codeDesc":"Success","tokens":[{"pos":0,"wlen":"8","word":"\u82cf\u5dde\u5927\u5b66","wtype":"\u540d\u8bcd","wtype_pos":16},{"pos":8,"wlen":"4","word":"\u4f4d\u4e8e","wtype":"\u52a8\u8bcd","wtype_pos":31},{"pos":12,"wlen":"6","word":"\u6c5f\u82cf\u7701","wtype":"\u540d\u8bcd","wtype_pos":16},{"pos":18,"wlen":"6","word":"\u82cf\u5dde\u5e02","wtype":"\u540d\u8bcd","wtype_pos":16},{"pos":24,"wlen":"8","word":"\u5e72\u5c06\u4e1c\u8def","wtype":"\u540d\u8bcd","wtype_pos":16},{"pos":32,"wlen":"3","word":"333","wtype":"\u6570\u8bcd","wtype_pos":15},{"pos":35,"wlen":"2","word":"\u53f7","wtype":"\u91cf\u8bcd","wtype_pos":26},{"pos":37,"wlen":"2","word":"\uff0c","wtype":"\u6807\u70b9\u7b26\u53f7","wtype_pos":34},{"pos":39,"wlen":"2","word":"\u662f","wtype":"\u52a8\u8bcd","wtype_pos":31},{"pos":41,"wlen":"2","word":"\u4e00","wtype":"\u6570\u8bcd","wtype_pos":15},{"pos":43,"wlen":"2","word":"\u4e2a","wtype":"\u91cf\u8bcd","wtype_pos":26},{"pos":45,"wlen":"4","word":"\u7f8e\u4e3d","wtype":"\u5f62\u5bb9\u8bcd","wtype_pos":1},{"pos":49,"wlen":"2","word":"\u7684","wtype":"\u52a9\u8bcd","wtype_pos":30},{"pos":51,"wlen":"4","word":"\u5927\u5b66","wtype":"\u540d\u8bcd","wtype_pos":16},{"pos":55,"wlen":"2","word":"\u3002","wtype":"\u6807\u70b9\u7b26\u53f7","wtype_pos":34}],"combtokens":[{"cls":"\u77ed\u8bed","pos":0,"wlen":"8","word":"\u82cf\u5dde\u5927\u5b66"},{"cls":"\u673a\u6784\u540d","pos":0,"wlen":"8","word":"\u82cf\u5dde\u5927\u5b66"},{"cls":"\u77ed\u8bed","pos":18,"wlen":"6","word":"\u6c5f\u82cf\u7701"},{"cls":"\u5730\u540d","pos":18,"wlen":"6","word":"\u6c5f\u82cf\u7701"},{"cls":"\u77ed\u8bed","pos":32,"wlen":"6","word":"\u82cf\u5dde\u5e02"},{"cls":"\u5730\u540d","pos":32,"wlen":"6","word":"\u82cf\u5dde\u5e02"},{"cls":"\u77ed\u8bed","pos":37,"wlen":"8","word":"\u5e72\u5c06\u4e1c\u8def"},{"cls":"\u5730\u540d","pos":37,"wlen":"8","word":"\u5e72\u5c06\u4e1c\u8def"}]}
```
还有更多的API接口，大家可以参阅《[文智自然语言处理API文档](https://mc.qcloudimg.com/static/qc_doc/2122fec15ff4183a35a99bc87a58cb71/7231.pdf)》体验更多接口，包括文本分类、关键词提取、句法分析、同义词、纠错、转码和敏感信息识别及下载抽取等。
