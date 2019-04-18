# python 读取配置文件与报错可能 NoSectionError: No section - wsp_1138886114的博客 - CSDN博客





2019年03月08日 17:04:54[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：29











### 文章目录
- [Python读取配置文件](#Python_1)
- [python执行报错 NoSectionError: No](#python_NoSectionError_No_32)
- [run_all.py代码如下：](#run_allpy_99)




### Python读取配置文件

配置文件: driver_data.ini

```bash
[db] 
url=www.baidu.com 

[bowers] 
bowers_ff=firefox 
bowers_cr=chrome 
bowers_ie=ie
```

```python
import os,sys 
import ConfigParser 

# class ReadConf():
cf = ConfigParser.ConfigParser() 
path = “../conf/driver_data.ini” 

fileopen = open(path) 
cf.readfp(fileopen)


secs = cf.sections()      # read all section 
print (‘sections:’, secs) 
print (cf.get(‘db’,’url’))  # read db section url value 

kvs = cf.items(‘bowers’)  # read bowers all item&key 
print (‘bowers:’, kvs)
```

### python执行报错 NoSectionError: No

sectionconfigparser.NoSectionError: No section: ‘section_1’

博客原文：[https://www.cnblogs.com/kakaln/p/8193292.html](https://www.cnblogs.com/kakaln/p/8193292.html)

场景:请求获取验证码模块regVC.py读取配置文件config.ini时，regVC.py模块单独执行正常，但通过run_all.py模块批量执行时报错，找不到section

解决办法：配置文件路径需写绝对路径

config.ini文件如下：

```bash
headers = {"Content-Type":"application/json"}
url = http://test.invoice.html
data_path = D:\data\test_data\data_name.xls
```

regVC.py模块代码如下：

```python
import requests
import configparser
import unittest
from Case.readexcel import ExcelData
import json

class registerVerifyCode(unittest.TestCase):
    def setUp(self):
        self.Purl = "/api/register/getVerifyCode"
        #取配置文件内数据
        self.config = configparser.ConfigParser()
        self.text = self.config.read("F:\\Case\\config.ini")      #这里要写配置文件的绝对路径                                  
        self.section = self.config.sections()
        self.option = self.config.options("section_1")
        self.item = self.config.items("section_1")
        self.url = self.config.items("section_1")[1][1]+self.Purl
        self.headers = self.config.items("section_1")[0][1]
        #self.headers由str类型转化为字典类型
        self.header = eval(self.headers)
        self.data_path = self.config.items("section_1")[2][1]
        self.sheetname = "注册验证码获取"
        self.data = ExcelData(self.data_path,self.sheetname).readExcel()
        print(self.url)
        print(self.data)

    def test_reVC(self):
        for a in self.data:
            for b in a:
                print(a)
                print(b)
                par = {"data":{
                    b:a[b]
                }
                }
                print(par)
                par_json = json.dumps(par)
                res = requests.post(self.url,par_json,headers=self.header)
                print(res.text)
                if "手机号码已注册" in res.text:
                    print("该手机号码已注册")
                if "请求注册验证码成功" in res.text:
                    print("请求注册验证码成功")

if __name__ == '__main__':
   unittest.main()
```

##### run_all.py代码如下：

```python
import unittest

def all_case():
    case_dir = "F:\\KEJINSUO_interface\\Case\\"
    testCase = unittest.TestSuite()
    discover = unittest.defaultTestLoader.discover(case_dir, pattern = "reg*.py", top_level_dir = None)
    testCase.addTest(discover)
    return testCase

if __name__ == '__main__':
    runner = unittest.TextTestRunner()
    runner.run(all_case())
```



