
# python 百度翻译破解版，亲证可行 - 追求卓越,做到专业 - CSDN博客


2019年03月07日 13:08:52[Waldenz](https://me.csdn.net/enter89)阅读数：47


不需要注册百度翻译平台，利用获取界面js，得知百度翻译中sign的加密规则
直接上源码
```python
import execjs
import requests
import re
JS_CODE = """
function a(r, o) {
    for (var t = 0; t < o.length - 2; t += 3) {
        var a = o.charAt(t + 2);
        a = a >= "a" ? a.charCodeAt(0) - 87 : Number(a),
        a = "+" === o.charAt(t + 1) ? r >>> a: r << a,
        r = "+" === o.charAt(t) ? r + a & 4294967295 : r ^ a
    }
    return r
}
var C = null;
var token = function(r, _gtk) {
    var o = r.length;
    o > 30 && (r = "" + r.substr(0, 10) + r.substr(Math.floor(o / 2) - 5, 10) + r.substring(r.length, r.length - 10));
    var t = void 0,
    t = null !== C ? C: (C = _gtk || "") || "";
    for (var e = t.split("."), h = Number(e[0]) || 0, i = Number(e[1]) || 0, d = [], f = 0, g = 0; g < r.length; g++) {
        var m = r.charCodeAt(g);
        128 > m ? d[f++] = m: (2048 > m ? d[f++] = m >> 6 | 192 : (55296 === (64512 & m) && g + 1 < r.length && 56320 === (64512 & r.charCodeAt(g + 1)) ? (m = 65536 + ((1023 & m) << 10) + (1023 & r.charCodeAt(++g)), d[f++] = m >> 18 | 240, d[f++] = m >> 12 & 63 | 128) : d[f++] = m >> 12 | 224, d[f++] = m >> 6 & 63 | 128), d[f++] = 63 & m | 128)
    }
    for (var S = h,
    u = "+-a^+6",
    l = "+-3^+b+-f",
    s = 0; s < d.length; s++) S += d[s],
    S = a(S, u);
    return S = a(S, l),
    S ^= i,
    0 > S && (S = (2147483647 & S) + 2147483648),
    S %= 1e6,
    S.toString() + "." + (S ^ h)
}
"""
class Dict:
    def __init__(self):
        self.sess = requests.Session()
        self.headers = {
            'User-Agent':
            'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36'
        }
        self.token = None
        self.gtk = None
        
        # 获得token和gtk
        # 必须要加载两次保证token是最新的，否则会出现998的错误
        self.loadMainPage()
        self.loadMainPage()
    def loadMainPage(self):
        """
            load main page : https://fanyi.baidu.com/
            and get token, gtk
        """
        url = 'https://fanyi.baidu.com'
        try:
            r = self.sess.get(url, headers=self.headers)
            self.token = re.findall(r"token: '(.*?)',", r.text)[0]
            self.gtk = re.findall(r"window.gtk = '(.*?)';", r.text)[0]
        except Exception as e:
            raise e
            # print(e)
    def langdetect(self, query):
        """
            post query to https://fanyi.baidu.com/langdetect
            return json
            {"error":0,"msg":"success","lan":"en"}
        """
        url = 'https://fanyi.baidu.com/langdetect'
        data = {'query' : query}
        try:
            r = self.sess.post(url=url, data=data)
        except Exception as e:
            raise e
            # print(e)
        json = r.json()
        if 'msg' in json and json['msg'] == 'success':
            return json['lan']
        return None
    def dictionary(self, query):
        """
            max query count = 2
            get translate result from https://fanyi.baidu.com/v2transapi
        """
        url = 'https://fanyi.baidu.com/v2transapi'
        sign = execjs.compile(JS_CODE).call('token', query, self.gtk)
        lang = self.langdetect(query)
        data = {
            'from': 'en' if lang == 'en' else 'zh',
            'to': 'zh' if lang == 'en' else 'en',
            'query': query,
            'simple_means_flag': 3,
            'sign': sign,
            'token': self.token,
        }
        try:
            r = self.sess.post(url=url, data=data)
        except Exception as e:
            raise e
        
        if r.status_code == 200:
            json = r.json()
            if 'error' in json:
                raise Exception('baidu sdk error: {}'.format(json['error']))
                # 998错误则意味需要重新加载主页获取新的token
            return json
        return None
    def dictionary_by_lang(self, query,fromlang,tolang):
        """
            max query count = 2
            get translate result from https://fanyi.baidu.com/v2transapi
        """
        url = 'https://fanyi.baidu.com/v2transapi'
        sign = execjs.compile(JS_CODE).call('token', query, self.gtk)
        lang = self.langdetect(query)
        data = {
            'from': fromlang,
            'to': tolang,
            'query': query,
            'simple_means_flag': 3,
            'sign': sign,
            'token': self.token,
        }
        try:
            r = self.sess.post(url=url, data=data)
        except Exception as e:
            raise e
        
        if r.status_code == 200:
            json = r.json()
            if 'error' in json:
                raise Exception('baidu sdk error: {}'.format(json['error']))
                # 998错误则意味需要重新加载主页获取新的token
            print(json)
            return json["trans_result"]["data"][0]['dst']
        return None
    def trans_baidu_en1(self,text):
        the_ret = self.dictionary_by_lang(text,"zh","en")
        ret1 = self.dictionary_by_lang(the_ret,"en","zh")
        return ret1
```
如果你有官方注册的appid，可以利用官方的方法。 百度翻译开放平台收费说明，总体说，其实还是挺有良心的，没有200万字内免费。
![](https://img-blog.csdnimg.cn/20190307104649660.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
官方提供的是python2.7的程序，本人已经转换为python3.
直接上连接：[https://blog.csdn.net/enter89/article/details/88287510](https://blog.csdn.net/enter89/article/details/88287510)

