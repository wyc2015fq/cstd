
import itsdangerous
import time
from itsdangerous import SignatureExpired

# https://www.cnblogs.com/haifeima/p/9963103.html
salt='sdaf'#加盐，指定一个盐值，别让别人知道哦，否则就可以解密出来了
t=itsdangerous.TimedJSONWebSignatureSerializer(salt,expires_in=1)#过期时间600秒

# ==============如何加密==================
res=t.dumps({'username':'yangfan','user_id':1})# 在t中加入传输的数据
token=res.decode()#指定编码格式
print(token)
# 得到的数据如下，就是包含数据和盐值的token了，只有在知道盐值的时候才能被解密出来
#
# ======================加密的数据如何解析=================
res = ""
ss = token

time.sleep(2)

try:
    res=t.loads(ss)
except SignatureExpired as e:
    # 激活链接已过期
    print('激活链接已过期') 
print(res)
# 返回的数据如下：
# {'username': 'yangfan', 'user_id': 1}
# 我们试一下将解析的数据改一个字母，或者超过了过期时间