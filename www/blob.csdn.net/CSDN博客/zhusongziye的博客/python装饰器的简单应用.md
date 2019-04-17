# python装饰器的简单应用 - zhusongziye的博客 - CSDN博客





2018年10月30日 20:41:58[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：49








 在Python的函数中，函数的参数我们成为形式参数，想比较而言，默认参数在实际的应用中更加丰富，还有一种情况就是函数的参数是函数，特别是在接口自动化测试中经常应用到这一思想。函数的返回值在一定程度上来说就是为了给另外一个函数提供函数执行的参数而已，这是它的意义，不管在函数中是否编写return函数它都是有返回值的。函数的参数是函数，这句话可以拿接口测试的一个案例作为说明，比如登录成功后，会返回token，那么另外一个请求的需要和登录成功后的token一致，否则就返回登录无效，说的简单点，就是另外一个请求需要使用到登录成功后的token的值，那么登录中可以返回，并且当参数一样给另外一个函数执行，来看一个简单的实例代码：

```python
def login(username,passwd):
   if username=='wuya' and passwd=='admin':
      return 'ma5QmSCrnZnzqCJ98fd1540130490175'

def order(token):
   if token=='ma5QmSCrnZnzqCJ98fd1540130490175':
      return '订单的数据'
   else:
      return '登录无效'

if __name__ == '__main__':
    print(order(login('wuya','admin')))
```

执行后的结果如下图所示：

![](https://img-blog.csdnimg.cn/20181030203952739.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

      针对如上的一个案例，通过装饰器对代码进行改造下，见构造后的源码：

```python
def login(func):
   def getToken(*args,**kwargs):
      r=func(*args,**kwargs)
      if r:
         return r
   return  getToken

@login
def order(username,password):
   if username=='wuya' and password=='admin':
      return '订单的数据'
   else:
      return '登录无效'

if __name__ == '__main__':
   print(order('wuya', 'admin'))
```

再次执行如上的代码，见执行后返回的结果如下图所示：

![](https://img-blog.csdnimg.cn/20181030204042745.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

这里就是一个装饰器的简单应用，查看订单的数据必须要进行登录系统成功后才可以查看，否则就返回登录无效（访问页面直接跳转到登录的页面）。这是产品的设计方式，执行order()函数后，函数执行的顺序为：
- 
执行函数login并把order当作函数login的参数来使用

- 
login函数的返回值重新赋值给order函数

- 
调用函数order其实执行的是getToken函数的，原来的order函数被覆盖

- 
函数一旦使用装饰器后，被装饰的函数重新赋值成装饰器的内部函数(也就是order函数被getToken函数被赋值)


见函数执行的顺序截图：

![](https://img-blog.csdnimg.cn/20181030204057495.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

见login函数的返回值重新赋值给了order函数截图：

![](https://img-blog.csdnimg.cn/20181030204104605.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

见调用order函数实际赋值给装饰器内部函数getToken函数截图：

![](https://img-blog.csdnimg.cn/20181030204114987.png)

![](https://img-blog.csdnimg.cn/20181030204124195.png)



