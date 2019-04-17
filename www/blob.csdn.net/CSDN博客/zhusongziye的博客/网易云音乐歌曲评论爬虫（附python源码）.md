# 网易云音乐歌曲评论爬虫（附python源码） - zhusongziye的博客 - CSDN博客





2018年10月20日 15:37:36[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：186








♚转载：

**作者**：志颖 ，一个狂热的python爬虫热爱者

GitHub：github.com/zyingzhou

用过网易云音乐听歌的朋友都知道，网易云音乐每首歌曲后面都有很多评论，热门歌曲的评论更是接近百万或者是超过百万条．现在我就来分享一下如何爬取网易云音乐歌曲的全部评论，由于网易云音乐的评论都做了混淆加密处理，因此我们需要深入了解它的加密过程之后才能爬取到网易云音乐歌曲的全部评论．

**一、首先分析数据的请求方式**

网易云音乐歌曲页面的URL形式为`https://music.163.com/#/song?id=歌曲id号`,这里我用Delacey的Dream it possible 为例进行讲解,它的URL为`https://music.163.com/#/song?id=38592976`.接下来开始分析数据的请求方式.

由于网易云音乐的评论是通过Ajax传输，我们打开浏览器的开发者工具(检查元素)，选中控制面板中的Network,再点击XHR(捕获ajax数据)，然后点击左上角的重新加载，会看到下面图片中的数据请求列表

![](https://img-blog.csdn.net/20181020153526578?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点击`R_SO_4_38592976?csrf_token=cdee144903c5a32e6752f50180329fc9`这一行,再点击Preview

![](https://img-blog.csdn.net/20181020153534628?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

发现我们所需要的数据就在这json格式的数据中,其中comments中是第一页的全部评论,一共20条,hotcomments是精彩评论一共有15条,每首歌曲只有第一页评论才有精彩评论.接着看一下它的请求头,点击Headers

![](https://img-blog.csdn.net/20181020153542878?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们发现的它是个post请求,向下滑你会发现这个post请求还带有数据

![](https://img-blog.csdn.net/2018102015355080?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这些数据都是经过加密处理的,因此我们需要分析它的加密过程来生成相应的参数,然后把加密后的参数加到post请求中才能获取到我们需要的评论数据.

**二、分析加密过程**

通过断点调试发现params和encSecKey是由js脚本中的`window.asrsea()`函数生成的.

![](https://img-blog.csdn.net/20181020153559643?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们发现`window.asrsea()`函数有4个参数,在浏览器的js控制台分别对这四个参数进行调试:

![](https://img-blog.csdn.net/20181020153606933?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

后面三个参数是定值,只有第一个参数是控制评论页面偏移量的参数,它是一个变量.笔者经过分析发现第一个参数的形式是:

```
{"rid":"R_SO_4_38592976","offset":"0","total":"True","limit":"20","csrf_token":""}
```

下面我来详细讲解这个变量的发现过程:

首先找到`core_dfe56728795d119e4d476fd09ea2dc51.js`这个js脚本,然后将断点打在第12973行,点击第一页评论,页面加载到断点处便停止了

![](https://img-blog.csdn.net/20181020153626765?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后按下电脑的Esc键打开js控制台,输入i1x,查看第一个变量:

![](https://img-blog.csdn.net/20181020153634147?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这是第一页的i1x的值,接下来看第二页的(需要点击第2页,然后输入i1x的值):

![](https://img-blog.csdn.net/20181020153642210?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

再看第3页:

![](https://img-blog.csdn.net/20181020153647948?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

再看第4页:

![](https://img-blog.csdn.net/20181020153656316?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过这几页的分析,我们可以得到i1x值的变化规律,且可以得到它的一般形式:

```
{"rid":"R_SO_4_38592976","offset":"0","total":"True","limit":"20","csrf_token":""}
```

offset和limit是必选参数,其他参数是可选的,其他参数不影响data数据的生成,offset (页面偏移量) = (页数-1) * 20, 注意limit最大值为100,当设为100时,获取第二页时,默认前一页是20个评论,也就是说第二页最新评论有80个,有20个是第一页显示的.因此我们可以构造第一个参数为:

```
# 偏移量,page是页数
offset = (page-1) * 20
msg = '{"offset":' + str(offset) + ',"total":"True","limit":"20","csrf_token":""}'
```

接下来,我们来看一下window.asrsea()函数的整个加密过程:

```
!function() {
    // 函数a生成长度为16的随机字符串
    function a(a) {
        var d, e, b = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", c = "";
        for (d = 0; a > d; d += 1)
            e = Math.random() * b.length,
            e = Math.floor(e),
            c += b.charAt(e);
        return c
    }
    // 函数b实现AES加密
    function b(a, b) {
        var c = CryptoJS.enc.Utf8.parse(b)
          , d = CryptoJS.enc.Utf8.parse("0102030405060708")
          , e = CryptoJS.enc.Utf8.parse(a)
          , f = CryptoJS.AES.encrypt(e, c, {
            iv: d,
            mode: CryptoJS.mode.CBC
        });
        return f.toString()
    }
    // 函数c实现RSA加密
    function c(a, b, c) {
        var d, e;
        return setMaxDigits(131),
        d = new RSAKeyPair(b,"",c),
        e = encryptedString(d, a)
    }
    function d(d, e, f, g) {
        var h = {}
          , i = a(16);
        return h.encText = b(d, g),
        h.encText = b(h.encText, i),
        h.encSecKey = c(i, e, f),
        h
    }
    function e(a, b, d, e) {
        var f = {};
        return f.encText = c(a + e, b, d),
        f
    }
    window.asrsea = d,
    window.ecnonasr = e
}();
```

`window.asrsea()`函数就是上面的d函数,现在我们来看函数d:

```
function d(d, e, f, g) {
        var h = {}
          , i = a(16);
        return h.encText = b(d, g),   // 第一次AES加密
        h.encText = b(h.encText, i),    // 第二次AES加密
        h.encSecKey = c(i, e, f),    // RSA加密
        h
    }
```

参数`h.encText`是经过两次AES加密得到的,h.encSecKey是经过一次RSA加密得到的,其中i是随机生成的长度为16的随机字符串.

**三、生成加密参数**

首先我们需要生成长度为16的随机字符串,这里我们仿照上面的javascript的实现,用Python生成16位长的随机字符串:

```
# 生成随机字符串
def generate_random_strs(length):
    string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    # 控制次数参数i
    i = 0
    # 初始化随机字符串
    random_strs  = ""
    while i < length:
        e = random.random() * len(string)
        # 向下取整
        e = math.floor(e)
        random_strs = random_strs + list(string)[e]
        i = i + 1
    return random_strs
```

接着用Python实现AES加密,这里要用到pycrypto库,先安装好这个库:

```
pip   install   pycrypto
```

然后导入加密模块:

```
from Crypto.Cipher import AES
```

由于AES加密的明文长度必须是16的倍数,因此我们需要对明文进行必要的填充,以满足它的长度是16的倍数:

```
# msg是需要加密的明文,如果不是16的倍数则进行填充(paddiing)
padding = 16 - len(msg) % 16
# 这里使用padding对应的单字符进行填充
msg = msg + padding * chr(padding)
```

AES加密的模式是AES.MODE_CBC,初始化向量`iv=’0102030405060708′`,具体的AES加密:

```
# AES加密
def AESencrypt(msg, key):
    # 如果不是16的倍数则进行填充(paddiing)
    padding = 16 - len(msg) % 16
    # 这里使用padding对应的单字符进行填充
    msg = msg + padding * chr(padding)
    # 用来加密或者解密的初始向量(必须是16位)
    iv = '0102030405060708'

    cipher = AES.new(key, AES.MODE_CBC, iv)
    # 加密后得到的是bytes类型的数据
    encryptedbytes = cipher.encrypt(msg)
    # 使用Base64进行编码,返回byte字符串
    encodestrs = base64.b64encode(encryptedbytes)
    # 对byte字符串按utf-8进行解码
    enctext = encodestrs.decode('utf-8')

    return enctext
```

然后是RSA加密.首先我简单介绍一下RSA的加密过程.在RSA中,明文,密钥和密文都是数字.RSA的加密过程可以用下列的公式来表达,这个公式非常的重要,你只有理解了这个公式,才能用Python实现RSA加密.

```
密文    =    明文E mod  N           (RSA加密)
```

RSA的密文是对代表明文的数字的E次方求mod N 的结果, 通俗的讲就是将明文和自己做E次乘法,然后将其结果除以N 求余数,这个余数就是密文.

下面来看具体的RSA加密代码实现:

```
# RSA加密
def RSAencrypt(randomstrs, key, f):
    # 随机字符串逆序排列
    string = randomstrs[::-1]
    # 将随机字符串转换成byte类型数据
    text = bytes(string, 'utf-8')
    seckey = int(codecs.encode(text, encoding='hex'), 16)**int(key, 16) % int(f, 16)
    # 返回整数的小写十六进制形式
    return format(seckey, 'x').zfill(256)
```

RSA加密后得到的字符串长为256,如果不够长则进行填充(不足部分在左侧添0).

最后就是获取那两个加密参数:

```
# 获取参数
def get_params(page):
    # msg也可以写成msg = {"offset":"页面偏移量=(页数-1) *　20", "limit":"20"},offset和limit这两个参数必须有(js)
    # limit最大值为100,当设为100时,获取第二页时,默认前一页是20个评论,也就是说第二页最新评论有80个,有20个是第一页显示的
    # 偏移量
    offset = (page-1) * 20
    # offset和limit是必选参数,其他参数是可选的,其他参数不影响data数据的生成,最好还是保留
    msg = '{"offset":' + str(offset) + ',"total":"True","limit":"20","csrf_token":""}'
    key = '0CoJUm6Qyw8W8jud'
    f = '00e0b509f6259df8642dbc35662901477df22677ec152b5ff68ace615bb7b725152b3ab17a876aea8a5aa76d2e417629ec4ee341f56135fccf695280104e0312ecbda92557c93870114af6c9d05c4f7f0c3685b7a46bee255932575cce10b424d813cfe4875d3e82047b97ddef52741d546b8e289dc6935b3ece0462db0a22b8e7'
    e = '010001'
    enctext = AESencrypt(msg, key)
    # 生成长度为16的随机字符串
    i = generate_random_strs(16)

    # 两次AES加密之后得到params的值
    encText = AESencrypt(enctext, i)
    # RSA加密之后得到encSecKey的值
    encSecKey = RSAencrypt(i, e, f)
    return encText, encSecKey
```

**四、获取全部评论**

上面我们获取到了两个参数encText和encSecKey,利用这两个参数来构造post表单数据(Form Data),即data的值:

```
params, encSecKey = get_params(page)
data = {'params': params, 'encSecKey': encSecKey}
```

歌曲评论的URL为:

```
url = 'https://music.163.com/weapi/v1/resource/comments/R_SO_4_' + str(songid) + '?csrf_token='
```

然后把data加到post的参数中去就能获取到json格式的评论数据.

```
html = requests.post(url, headers=headers, data=data)
```

至此,获取网易云音乐全部评论的Python爬虫实现原理分析全部完成！

获取**本文完整代码:**

**链接：https://pan.baidu.com/s/1YiNjYVQXkKEvnxDhXOXL6Q 提取码：72oh**



