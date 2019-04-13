
# 使用python实现生成用户信息 - jiahaowanhao的博客 - CSDN博客


2018年02月27日 20:49:54[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：187


[使用python实现生成用户信息](http://cda.pinggu.org/view/24843.html)
今天练习的时候要展示一个从用户信息列表,就想把他做成信息和修改在一起的一个网页,方便用户修改内容
考虑到要把信息和值分开放,那么肯定是字典了,因为需要保证位置不变,使用有序字典
考虑到需要解析方便和好看点,所以让models.py中返回的就直接是”k1 v1 k2 v2”格式
第一步:
修改models.py文件,将之前的分割符全部去掉,使用空格分割
为了省事和可读的折中吧,没有使用格式化字符串的时候没有用关键字
def __str__(self):
return "email {0} idcard {1} adress {2} phonenumber {3}".format(
self.email, self.idcard, self.adress, self.phonenumber
)

第二步:
修改views.py,把字符串整合成有序字典
from collections import OrderedDict as ordic
@login_required
def msg(request):
msg = UserMsg.objects.filter(whoami=request.user)
for item in msg:
msglist = str(item).split(" ")
msgkey = msglist[::2]
msgvalue = msglist[1::2]
msgs = ordic(zip(msgkey,msgvalue))
context = {'msg':msgs}
return render(request, 'usermsg/msg.html', context)
获取列表的时候也可以用列表生成器, 像这样就行
>>>[str(i).split() for i in msg][0]
>>>['email', 'xxxxxx@163.com', 'idcard', '12', 'adress', '13', 'phonenumber', '14']
最后展示在页面就行了,简单的放到表格里了,没有继续处理
<table border="0">
{% for key,value in msg.items %}
<br>
<tr>
<td> {{key}} </td>
<td> :{{ value }} </td>
<td> <a href="\#" rel="external nofollow" value="change{{ key }}"> 修改{{ key }}
</a> </td>
</tr>
{% endfor %}
</table>
就是这样了,嗯,好丑

