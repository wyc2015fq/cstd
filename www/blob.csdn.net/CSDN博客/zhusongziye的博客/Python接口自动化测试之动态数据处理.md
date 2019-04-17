# Python接口自动化测试之动态数据处理 - zhusongziye的博客 - CSDN博客





2018年09月01日 12:48:24[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：948








   在前面的知识基础上介绍了在接口自动化测试中，如何把数据分离出来，并且找到它的共同点，然后依据这个共同点来找到解决复杂问题的思想。我一直认为，程序是人设计的，它得符合人性，那么自动化测试的，就是需要在复杂世界的背后，找到一个共同的点，然后把复杂的程序进行简单化。再次看如下的截图：

![](https://img-blog.csdn.net/20180901124730119?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       在这个图片中，可以看到这是不同的接口请求的数据，那么我们在自动化测试中就得定位到具体的行和列，才可以找到这条数据，那么就涉及到一个问题，这个问题就是在一个excel中有太多的数据，如果精确到行和列，某些时候会出现混乱，按照行和列的方式并不是解决问题的最佳思想。在接口测试中，客户端发送请求到服务端的过程中，除了请求方法和请求头外，必须要带的是请求地址和请求参数，那么也就是说在接口自动化测试中，应该更多的去关心请求地址和请求参数这部分，在如上截图中，可以看到共同点是虽然有很多的数据，但是请求地址永远是在第二列，请求数据永远是在第三列，这个数据不管它是在那一行，这个是不变的，那么读取数据的思想方式是：

1、首先读取excel中的数据，读取XX行的数据并且返回

2、读取XX行中请求地址数据

3、读取XX行中请求参数的数据

4、对post请求进行二次封装，让调用的时候只传请求地址和请求参数，至于请求头，超时处理，以及cookie这些信息，在二次封装的时候直接处理好

先看对excel数据中对数据的处理代码：

#!/usr/bin/env python
#-*-coding:utf-8-*-

#author:wuya


import  os
import  xlrd
import  json


class Helper(object):
   '''公共方法'''

   def base_dir(self,filePath,folder='data'):
      '''
      返回公共路径
      :parameter folder:文件夹
      :parameter filePath:文件名称
      '''
      return os.path.join(
         os.path.dirname(
            os.path.dirname(__file__)),
         folder,filePath)

   def readExcel(self,rowx,filePath='data.xlsx'):
      '''
      读取excel中数据并且返回
      :parameter filePath:xlsx文件名称
      :parameter rowx:在excel中的行数
      '''
      book=xlrd.open_workbook(self.base_dir(filePath))
      sheet=book.sheet_by_index(0)
      return sheet.row_values(rowx)

   def getUrl(self,rowx):
      '''
      获取请求地址
      :parameter rowx:在excel中的行数
      '''
      return self.readExcel(rowx)[1]

   def getData(self,rowx):
      '''
      获取数据并且返回
      :parameter rowx:在excel中的行数
      '''
      return  json.loads(self.readExcel(rowx)[2])



接着看post请求的二次封装代码：

import  requests

def getHeaders():
   '''返回请求头'''
   headers= {'Content-Type':'application/json; charset=UTF-8',
           'Parkingwang-Client-Source':'ParkingWangAPIClientWeb'}
   return headers

def post(url,data):
   '''
   对post请求方法进行二次封装
   :parameter url:请求地址
   :parameter data:请求参数
   '''
   r=requests.post(
      url=url,
      json=data,
      headers=getHeaders(),
      timeout=6)
   return r

      解决了如上问题后，接着来思考另外一个问题，基于业务的来思考， 比如添加一个用户，然后在删除该用户，那么删除该用户的时候必须要带的参数是用户的ID，用户的ID获取的方式是在添加用户之后，获取用户的ID，步骤具体为：

1、添加用户

2、查询用户获取用户的ID并且返回这个用户的ID

3、删除用户，把这个返回的用户ID当作参数传进去

见删除用户的数据，见截图：

![](https://img-blog.csdn.net/20180901124749215?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

虽然在数据中有用户ID的参数，但是这个用户ID的参数是写死的，实际接口测试中用户的ID参数是动态的，因为每次添加用户之后用户的ID是不同的，那么如何可以做到删除用户的时候这个用户的ID就是创建用户之后拿到的最新的用户的ID了，处理的思路是：

1、读取删除用户的请求数据

2、把读取的数据是字符串类型，通过反序列化的形式转为字典的数据类型

3、成为字典的类型后，对字典中的参数用户ID进行赋值，值为添加用户之后获取的用户ID

4、对字典赋值后，然后返回字典

5、调用删除用户接口的时候，请求参数直接调用对用户ID赋值后的方法

6、这样就可以保证用户ID与创建后的用户ID数据是一致的

通过如上的思路，可以很好的解决了动态数据的处理，以及数据关联性的处理，在Jmeter测试工具中，使用的方案是后置处理器中的正则表达式提取器，见python实现如上的代码：

#!/usr/bin/env python 
#-*-coding:utf-8-*-

#author:wuya

import  unittest
import  requests
import  time as t
import  os
import  json

from page.user import *
from utils.helper import *

class TestUserApi(unittest.TestCase,Helper):
   @classmethod
   def setUpClass(cls):
      t.sleep(1)

   @classmethod
   def tearDownClass(cls):
      pass

   def statusCode(self,r):
      '''对HTTP状态码和业务状态码校验'''
      self.assertEqual(r.status_code,200)
      self.assertEqual(r.json()['status'],0)

   def test_user_api_001(self):
      '''登录业务:登录系统'''
      r=post(self.getUrl(1),self.getData(1))
      self.statusCode(r)
      self.assertEqual(r.json()['data']['name'],'6666666666')
      with open(self.base_dir('token'),'w') as f:
         f.write(r.json()['data']['token'])

   @property
   def getToken(self):
      '''获取登录成功后的token'''
      with open(self.base_dir('token'),'r') as f:
         return f.read()

   def setToken(self,rowx):
      '''
      对excel中的请求参数token重新赋值
      :parameter rowx:在excel中的行数
      '''
      dict1=self.getData(rowx)
      dict1['token']=self.getToken
      return dict1

   def test_user_api_002(self):
      '''登录业务:查看登录成功后的用户信息'''
      r=post(self.getUrl(2),self.setToken(2))
      self.statusCode(r)
      self.assertEqual(r.json()['data']['username'],'6666666666')

   def test_user_api_003(self):
      '''用户管理业务:添加用户'''
      r=post(self.getUrl(3),self.setToken(3))
      self.statusCode(r)

   def test_user_api_004(self):
      '''用户管理业务:用户查询'''
      r=post(self.getUrl(4),self.setToken(4))
      self.statusCode(r)
      self.assertEqual(r.json()['data']['records'][0]['name'],'666666')

   @property
   def getUserID(self):
      '''获取用户的ID'''
      r = post(self.getUrl(4), self.setToken(4))
      return r.json()['data']['records'][0]['id']

   def setTokenUserID(self,rowx):
      '''
      对excel中的请求参数token,用户ID重新赋值
      :parameter rowx:在excel中的行数
      '''
      dict1=self.getData(rowx)
      dict1['token']=self.getToken
      dict1['id']=self.getUserID
      return dict1
      
   def test_user_api_009(self):
      '''用户管理业务:删除用户'''
      r = post(self.getUrl(7), self.setTokenUserID(7))
      self.statusCode(r)

if __name__ == '__main__':
   unittest.main(verbosity=2)

     OK，在接口自动化测试中，动态参数的处理思路介绍完毕。



