# Hessian跨语言调用实例 - wsp_1138886114的博客 - CSDN博客





2019年04月01日 16:55:08[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：36








# Hessian跨语言调用实例

Hessian是跨语言基于HTTP的RPC框架，传输的数据格式是二机制。使用Java编写PRC服务端，使用Java和Python作为客户端调用RPC服务。关于源码请查看：[http://hessian.caucho.com/download/hessianlib.py](http://hessian.caucho.com/download/hessianlib.py)

#### 安装与使用

```python
#!python
# encoding: utf-8

'''
使用Python语言实现远程调用
使用Python客户端来进行远程调用，我们可以从https://github.com/bgilmore/mustaine下载，然后安装Hessian的代理客户端Python实现库：
git clone https://github.com/bgilmore/mustaine.git
cd mustaine
sudo python setup.py install

http://san-yun.iteye.com/blog/1628405
http://pydoc.net/Python/mustaine/0.1.7/mustaine.parser/

https://github.com/Kingson4Wu/Frieza-revenge/blob/master/RPC-framework%2Fsrc%2Fmain%2Fjava%2Fapp%2Fdemo%2FBasicService.java

'''


from mustaine.client import HessianProxy
test = HessianProxy("http://localhost:8080/EasyHessian/hello")
print test._headers
print test.hello()
print test.getUser()
print test.getUser().userName
user = test.getUser()
print user.password
test.setGreeting("Hessian Python")
print test.hello()
```

关于详情：[https://blog.csdn.net/sinat_41898105/article/details/83818446](https://blog.csdn.net/sinat_41898105/article/details/83818446)

#### 调用示例

```python
import json
from pyhessian.client import HessianProxy
 
 
def run():
    url = "http://192.168.130.129:8000/apollo-erp-remote/lmWorkerTaskService"
    params = {
        'barcode': 'A502170661032-5-1',
        'lmTaskJson': json.dumps({
                "barcode": "A502170661032-5-1",
                "unitcode": "YB",
                "userCode": "211573",
                "operateTime": "2018-09-28 11:12:39:667",
                "type": "in"
            })
        }
    service = HessianProxy(url)
    response = service.__call__(method='insertLmWorkerTask', args=(params['barcode'], params['lmTaskJson']))
    return response
 
if __name__ == '__main__':
    print(run())
```



