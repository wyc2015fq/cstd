# python提取浏览器Cookie - TaigaComplex求职中 - 博客园







# [python提取浏览器Cookie](https://www.cnblogs.com/TaigaCon/p/9721697.html)





在用浏览器进行网页访问时，会向网页所在的服务器发送http协议的GET或者POST等请求，在请求中除了指定所请求的方法以及URI之外，后面还跟随着一段Request Header。Request Header的内容主要用于描述本地信息，如所用的浏览器、所用的系统、语言、所能接受的返回数据的编码格式等，其中有一个非常重要的Header项就是Cookie，Cookie可以说是网站的自定义数据集。由于服务器端无法无法控制本地（浏览器）的内存数据，但服务器又有必要搜集与自己所提供的服务相关的本地状态信息，而Cookie就承载了这一功能，目的是记录用户在网站的状态信息。

在用python对网页进行访问的时候，如果希望得到与在网页端相同的结果，用该网页在浏览器中所保留的Cookie作为python的请求Cookie是一个比较值得推荐的做法。

本文主要讨论如何提取浏览器保存在本地的Cookie，所用的浏览器为Chrome。





# Cookie文件

Chrome用sqlite来维护Cookie，Cookie中的信息被保存在sqlite数据库当中，如果系统为Windows，那么数据库文件所在的位置为[[1]](#ref1)：
C:\Users\{UserName}\AppData\Local\Google\Chrome\User Data\Default\Cookies
其中该路径的{UserName}是当前系统的用户名。
username = os.environ.get('USERNAME')
cookie_file = 'C:\Users\{UserName}\AppData\Local\Google\Chrome\User Data\Default\Cookies'.format(UserName=username)




# Cookie表单

通过Cookie文件路径，我们可以建立数据库连接，然后提取出数据库的信息。
con = sqlite3.connect(cookie_file)
cursor = con.cursor()
数据库中的cookies表就是用于保存浏览器Cookie的。提取表中各列的名称
cursor.execute('SELECT * FROM cookies')
for description in cursor.description:
    print(description[0])
各字段的描述如下
- creation_utc：Cookie产生的utc时间
- host_key：Cookie所在的网页(domain)
- name：Cookie名称
- value：不加密的Cookie值，由于Chrome几乎都会对Cookie值加密后再存储，因此这个字段基本都是空的
- path：如果服务器需要设置Cookies，那么服务器在响应浏览器请求的时候会返回Set-Cookie的响应，并且附带所要设置的Cookies，这里的path的默认值就是返回Set-Cookie的那个页面。path以'/'为开头。
- expires_utc：Cookie的有效期限
- [is_secure](https://tools.ietf.org/html/rfc6265#section-4.1.2.5)：指示在浏览器与服务器之间传输该Cookie时需要采用加密通道，即https
- [is_httponly](https://tools.ietf.org/html/rfc6265#section-4.1.2.6)：当设置了该值为1时，在浏览器上运行的JS不能读取到该Cookie，该Cookie只能由http请求读取。这个标记主要目的是提高Cookie的安全性，防止无关的JS脚本窃取Cookie中的重要信息
- last_access_utc：上一次访问到该Cookie的时间
- has_expires：Cookie的期限是否有效
- [is_persistent](https://tools.ietf.org/html/rfc6265#section-5.3)：如果expires_utc不为0，那么这个值为1
- [priority](https://tools.ietf.org/html/rfc6265#page-24)：Cookie的删除优先级，Cookie也有存储上限的，当超出上限则需要删除，此时会有特定的删除策略来删除不同priority的Cookie
- encrypted_value：加密后的Cookie值
- [firstpartyonly](https://tools.ietf.org/html/draft-west-first-party-cookies-01#page-5)：first-party以及third-party是HTTP Request的一种分类，first-party指的是当前所发送的HTTP请求的URL跟浏览器地址栏上的URL一致；否则就是third-party。如我们平常看到的很多网页上的图片或者广告，其实都是third-party request。无论first-party或者third-party，都是HTTP请求，在往服务器发送请求的时候会带上host为该URL的Cookies，不过如果一个Cookie指定了firstpartyonly，那么如果请求为thrid-party，在发送请求的时候不会附带该Cookie。以上面所说的网页中的图片为例子，如果该图片URL的Cookie设定为firstpartyonly，在浏览网站时，通过third-party访问了该图片所在的URL，就不会发送该Cookie。



上述字段中有些是只有浏览器才会用到的，这里没有必要用上，因此我们仅需要提取一些必要字段
cursor.execute('SELECT host_key, name, value, path, expires_utc, is_secure, encrypted_value '
                        'FROM cookies WHERE host_key like "%{}%";'.format(domain_name))





# 解密Cookie

我们前面说过，Chrome在对Cookie的值保存之前会进行加密处理，并保存在数据库的encrypt_value字段中。在Windows系统中，Cookie加密采用的是系统提供的函数[CryptProtectData](https://docs.microsoft.com/en-us/windows/desktop/api/dpapi/nf-dpapi-cryptprotectdata)，我们在解密的时候也需要调用系统提供的函数[CryptUnprotectData](https://docs.microsoft.com/zh-cn/windows/desktop/api/dpapi/nf-dpapi-cryptunprotectdata)[[1]](#ref1)。解密的Windows用户必须与加密的用户一致才能成功解密。

不过系统提供的是C函数，python通过[ctypes](https://docs.python.org/2.7/library/ctypes.html)库来实现对C函数的调用。

CryptUnprotectData需要7个参数：
- pDataIn：一个指向DATA_BLOB结构体的指针，该DATA_BLOB内需存放被解密的数据。DATA_BLOB结构体内含两个成员：cbData，数据的所占用的字节数；pbData，指向数据所在内存的指针。
- ppszDataDescr：描述该加密数据的信息，如果在进行加密操作的时候添加了描述，那么在解密的时候也能得到该描述信息。获得该描述后需要调用系统提供的LocalFree释放ppszDataDescr指向的内存。如果不需要，设为NULL即可。
- pOptionalEntropy：一个指向含有密钥DATA_BLOB的指针，不过在进行Cookie加密时通常不会用到。
- pvReserved：保留参数，设为NULL即可。
- pPromptStruct：解密是一个有安全风险的操作，可能需要弹出风险提升，如果不需要弹出提示设置为NULL即可。
- dwFlags：安全相关的标志，设置为0即可。
- pDataOut：一个指向解密后的数据的DATA_BLOB，获得解密数据后需要调用系统提供的LocalFree函数释放pbData指向的内存。
class DATA_BLOB(ctypes.Structure):
    _fields_ = [("cbData", ctypes.wintypes.DWORD),
                ("pbData", ctypes.POINTER(ctypes.c_char))]
    def __init__(self, data):
        string = str(data)
        self.cbData = len(string)
        self.pbData = ctypes.create_string_buffer(string)

def descrypt(cipher):
    #parameters
    DataIn = DATA_BLOB(cipher)
    Descr = ctypes.c_wchar_p()
    DataEntropy = DATA_BLOB('')
    Reserved = None
    PromptStruct = None
    CRYPTPROTECT_UI_FORBIDDEN = 0x00
    DataOut = DATA_BLOB('')
    #win call
    ret = ctypes.windll.crypt32.CryptUnprotectData(ctypes.byref(DataIn),
                                                    Descr, 
                                                    ctypes.byref(DataEntropy), 
                                                    Reserved, 
                                                    PromptStruct, 
                                                    CRYPTPROTECT_UI_FORBIDDEN, 
                                                    ctypes.byref(DataOut)
                                                    )
    if not ret:
        raise RuntimeError("failed to descrypt")

    buf = ctypes.create_string_buffer(int(DataOut.cbData))
    ctypes.memmove(buf, DataOut.pbData, DataOut.cbData)
    ctypes.windll.kernel32.LocalFree(Descr)    
    ctypes.windll.kernel32.LocalFree(DataOut.pbData)
    return buf.value




# 创建Cookie并填充CookieJar

python并不推荐我们去自行创建Cookie，因为作为用户通常不需要去改动，甚至没有必要知道Cookie的内容，不过此处出于特殊的需求，需要通过在Cookie数据库中获取的数据来创建Cookie。

首先我们来简单了解一下Set-Cookie。我们知道Cookie都是服务器为浏览器设置的，设置Cookie是通过服务器返回的Response Header，如果header中包含有Set-Cookiie相关字段，则能进行Cookie的设置。

http.cookiejar.Cookie的初始化需要提供18个参数[[2]](#ref2)：
- version：比较老版本的Cookie（如[rfc2109](https://www.ietf.org/rfc/rfc2109.txt)，已淘汰）要求Cookie必须有Version字段，不过较新的版本（如[rfc6265](https://tools.ietf.org/html/rfc6265)）的Cookie去掉了这一字段，因此这里填0就行。
- name：Cookie名称
- value：Cookie值
- port：http端口，指定了该字段的Cookie只能发送到服务器的指定端口，只有[rfc2965](https://tools.ietf.org/html/rfc2965.html)中的Set-Cookie2才用到该字段，rfc2695是一个已淘汰的版本，这里填None就行。
- port_specified：是否有指定端口，同样是已淘汰的参数，填False。
- domain：服务器域名
- domain_specified：是否指定了服务器域名
- domain_initial_dot：服务器域名是否以"."作为开头
- path：服务器路径
- path_specified：是否指定了服务器路径
- secure：是否采用安全通道
- expires：Cookie期限
- discard：是否为一次性Cookie，同上方的is_persistent，填False
- comment：Cookie注释，填None
- comment_url：Cookie注释所在的URL，填None
- rest：存储该Cookie的一些非标准的属性，填{}
- rfc2109：是否为rfc2109标准，默认值为False


    cj = http.cookiejar.CookieJar()
    for row in cursor.fetchall():
        host, name, value, path, expires, secure, encrypted_value = row[:]
        data = descrypt(encrypted_value)
        c = http.cookiejar.Cookie(0, name, data, None, False, host, host.startswith('.'), host.startswith('.'),
                                    path, True, secure, expires, False, None, None, {})
        cj.set_cookie(c)


其中比较重要的domain相关参数请查看：rfc2109的Interpreting Set-Cookie、Rejecting Cookies以及rfc6265的The Domain Attribute、Storage Model



**Reference**：
- [Client Side HTTP Cookie Security](https://www.defcon.org/images/defcon-22/dc-22-presentations/Wyde/DEFCON-22-David-Wyde-Client-Side-HTTP-Cookie-Security.pdf)
- [cookiejar.Cookie](https://github.com/python/cpython/blob/e42b705188271da108de42b55d9344642170aa2b/Lib/http/cookiejar.py#L729)












