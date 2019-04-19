# python拼接multipart/form-data类型post请求格式 - _天枢 - 博客园
## [python拼接multipart/form-data类型post请求格式](https://www.cnblogs.com/yhleng/p/10696922.html)
2019-04-12 16:36 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=10696922)
# 最近要做form-data类型接口，大多数这种格式用来文件上传，但是我们公司就是用这种格式传输请求数据。
# 百度了一些基本都是files方式的，可是我们需要data=方式的。下面自己来拼接,代码很简单。
```
# multipart/form-data
class MultipartFormData(object):
    """multipart/form-data格式转化"""
    @staticmethod
    def format(data, boundary="----WebKitFormBoundary7MA4YWxkTrZu0gW", headers={}):
        """
        form data
        :param: data:  {"req":{"cno":"18990876","flag":"Y"},"ts":1,"sig":1,"v": 2.0}
        :param: boundary: "----WebKitFormBoundary7MA4YWxkTrZu0gW"
        :param: headers: 包含boundary的头信息；如果boundary与headers同时存在以headers为准
        :return: str
        :rtype: str
        """
        #从headers中提取boundary信息
        if "content-type" in headers:
            fd_val = str(headers["content-type"])
            if "boundary" in fd_val:
                fd_val = fd_val.split(";")[1].strip()
                boundary = fd_val.split("=")[1].strip()
            else:
                raise "multipart/form-data头信息错误，请检查content-type key是否包含boundary"
        #form-data格式定式
        jion_str = '--{}\r\nContent-Disposition: form-data; name="{}"\r\n\r\n{}\r\n'
        end_str = "--{}--".format(boundary)
        args_str = ""
        if not isinstance(data, dict):
            raise "multipart/form-data参数错误，data参数应为dict类型"
        for key, value in data.items():
            args_str = args_str + jion_str.format(boundary, key, value)
        
        args_str = args_str + end_str.format(boundary)
        args_str = args_str.replace("\'", "\"")
        return args_str
```
![](https://img2018.cnblogs.com/blog/1149221/201904/1149221-20190416101841749-2130272904.png)
调用方式：示例：data即我们要传的接口参数，需要转化成form-data格式
需要注意的是boundary参数，这个参数要跟header头中定义的一样，否则会出问题。否则会出问题，否则会出问题。重要事情说三遍。
```
headers = {
        'content-type': "multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW",
        'cache-control': "no-cache",
    }
```
```
data = {
    "req":{"cno":"1213058673616305"},
    "appid":"dp3wY4YtycajNEz23zZpb5Jl",
    "ts":1,
    "sig":1,
    "v": 2.0
    }
# mh = MultipartFormData.format(data=data, boundary="----WebKitFormBoundary7MA4YWxkTrZu0gW")
mh = MultipartFormData.format(data=data, headers=headers)
print(mh)
```
转化后的data  form-data字符串为：
```
------WebKitFormBoundary7MA4YWxkTrZu0gW
Content-Disposition: form-data; name="v"
2.0
------WebKitFormBoundary7MA4YWxkTrZu0gW
Content-Disposition: form-data; name="req"
{"cno": "1213058673616305"}
------WebKitFormBoundary7MA4YWxkTrZu0gW
Content-Disposition: form-data; name="sig"
1
------WebKitFormBoundary7MA4YWxkTrZu0gW
Content-Disposition: form-data; name="ts"
1
------WebKitFormBoundary7MA4YWxkTrZu0gW
Content-Disposition: form-data; name="appid"
dp3wY4YtycajNEz23zZpb5Jl
------WebKitFormBoundary7MA4YWxkTrZu0gW--
View Code
```
![](https://img2018.cnblogs.com/blog/1149221/201904/1149221-20190412163311791-863465655.png)
python调用时将转form-data的字符串给data参数即可：
```
res = requests.request("POST",self.url,data=**mh**, headers= headers)
```

