# 利用 Linux 系统生成随机密码的 10 种方法 - 文章 - 伯乐在线
原文出处： [wangzhen102](http://very1024.blog.51cto.com/3588520/1533369)
通常情况下大家生成密码都好困惑,一来复杂程度不够会不安全,复杂程度够了又不能手动随便敲击键盘打出一同字符(但通常情况下这些字符是有规律的),使用1password 或者 keepass 这种软件生成也可以,不过貌似1password 要收费,既然这样我们就玩一下好玩的用 linux 来生成随机密码玩玩吧;
Linux操作系统的一大优点是对于同样一件事情，你可以使用高达数百种方法来实现它。例如，你可以通过数十种方法来生成随机密码。本文将介绍生成随机密码的十种方法。
1. 使用SHA算法来加密日期，并输出结果的前32个字符：


```
date +%s |sha256sum |base64 |head -c 32 ;echo
生成结果如下:
ZTNiMGM0NDI5OGZjMWMxNDlhZmJmNGM4
```
2. 使用内嵌的/dev/urandom，并过滤掉那些日常不怎么使用的字符。这里也只输出结果的前32个字符：
3. 使用openssl的随机函数


```
openssl rand -base64 32
生成结果如下:
rYJWqJlTLAYeX3j7nCbir20h1k/0CnqLNEuNyTScfKo=
```
4. 这种方法类似于之前的urandom，但它是反向工作的


```
tr -cd '[:alnum:]' < /dev/urandom | fold -w32 | head -n1;echo
 
生成结果如下:
tpgudzF7sqtU4yyW2LVhmUQOZIQi87
```
5. 使用string命令，它从一个文件中输出可打印的字符串


```
strings /dev/urandom | grep -o '[[:alnum:]]' | head -n 32 | tr -d 'n'; echo
生成结果如下:
W4v1iQtkmQ8sIDd9jxDQNpg8HPMOZ8
```
6. 这是使用urandom的一个更简单的版本
7. 使用非常有用的dd命令


```
dd if=/dev/urandom bs=1 count=32 2>/dev/null | base64 -w 0 | rev | cut -b 2- | rev
生成结果如下:
9+0RUd4U3HmSdMlgD7j0sf/r09MZFDVBS28W+pO2WcA
```
8. 你甚至可以生成一个只用左手便可以输入的密码


```
/urandom  tr -dc '12345!@#$%qwertQWERTasdfgASDFGzxcvbZXCVB' | head -c32; echo
生成结果如下:
VTG3#TR4sAgXg3Z%##WZG4ZQ@GZ$wdqF
```
9. 如果每次都使用上述某种方法，那更好的办法是将它保存为函数。如果这样做了，那么在首次运行命令之后，你便可以在任何时间只使用randpw就可以生成随机密码。或许你可以把它保存到你的~/.bashrc文件里面


```
randpw(){ < /dev/urandom tr -dc _A-Z-a-z-0-9 | head -c${1:-16};echo;}
 
生成结果如下:
vgBX8cNo950RiykZRpPYa4BvbAvZbY_x
```
10. 最后这种生成随机密码的方法是最简单的。它同样也可以在安装了Cygwin的Windows下面运行。在Mac OS X下也可以运行。我敢肯定会有人抱怨这种方法生成的密码没有其它方法来的随机。但实际上如果你使用它生成的全部字符串作为密码，那这个密码就足够随机了


```
date | md5sum
生成结果如下:
e0d057b46a9a78346cbd94b25e574e79  -
```


```
date | base64
生成结果如下:
MjAxNOW5tCAwN+aciCAzMeaXpSDmmJ/mnJ/lm5sgMTc6MDA6MzYgQ1NUCg==
```


```
ifconfig | md5sum
生成结果如下:
7c4243742aa515d45c12deca31428a95  -
```


```
甚至你想生成一个核弹发射密码都可以,下面是一个生成长密码的例子;
ifconfig | base64
生成结果如下:
ZW0xICAgICAgIExpbmsgZW5jYXA6RXRoZXJuZXQgIEhXYWRkciA3ODoyQjpDQjoyQjpCMDo5NCAg
CiAgICAgICAgICBpbmV0IGFkZHI6MTkyLjE2OC4zLjUgIEJjYXN0OjE5Mi4xNjguMy4yNTUgIE1h
c2s6MjU1LjI1NS4yNTUuMAogICAgICAgICAgaW5ldDYgYWRkcjogZmU4MDo6N2EyYjpjYmZmOmZl
MmI6YjA5NC82NCBTY29wZTpMaW5rCiAgICAgICAgICBVUCBCUk9BRENBU1QgUlVOTklORyBNVUxU
SUNBU1QgIE1UVToxNTAwICBNZXRyaWM6MQogICAgICAgICAgUlggcGFja2V0czoyMDY3NTY0IGVy
cm9yczowIGRyb3BwZWQ6MCBvdmVycnVuczowIGZyYW1lOjAKICAgICAgICAgIFRYIHBhY2tldHM6
ODg2NDUgZXJyb3JzOjAgZHJvcHBlZDowIG92ZXJydW5zOjAgY2FycmllcjowCiAgICAgICAgICBj
b2xsaXNpb25zOjAgdHhxdWV1ZWxlbjoxMDAwIAogICAgICAgICAgUlggYnl0ZXM6MjAzNDkzNTEx
ICgxOTQuMCBNaUIpICBUWCBieXRlczozMjUyNzUxNiAoMzEuMCBNaUIpCgpsbyAgICAgICAgTGlu
ayBlbmNhcDpMb2NhbCBMb29wYmFjayAgCiAgICAgICAgICBpbmV0IGFkZHI6MTI3LjAuMC4xICBN
YXNrOjI1NS4wLjAuMAogICAgICAgICAgaW5ldDYgYWRkcjogOjoxLzEyOCBTY29wZTpIb3N0CiAg
ICAgICAgICBVUCBMT09QQkFDSyBSVU5OSU5HICBNVFU6MTY0MzYgIE1ldHJpYzoxCiAgICAgICAg
ICBSWCBwYWNrZXRzOjU2OTkzMSBlcnJvcnM6MCBkcm9wcGVkOjAgb3ZlcnJ1bnM6MCBmcmFtZTow
CiAgICAgICAgICBUWCBwYWNrZXRzOjU2OTkzMSBlcnJvcnM6MCBkcm9wcGVkOjAgb3ZlcnJ1bnM6
MCBjYXJyaWVyOjAKICAgICAgICAgIGNvbGxpc2lvbnM6MCB0eHF1ZXVlbGVuOjAgCiAgICAgICAg
ICBSWCBieXRlczozMzEzMDcxOSAoMzEuNSBNaUIpICBUWCBieXRlczozMzEzMDcxOSAoMzEuNSBN
aUIpCgo=
```
