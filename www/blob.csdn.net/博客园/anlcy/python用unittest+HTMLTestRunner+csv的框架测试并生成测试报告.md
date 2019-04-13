
# python用unittest+HTMLTestRunner+csv的框架测试并生成测试报告 - anlcy - 博客园






# [python用unittest+HTMLTestRunner+csv的框架测试并生成测试报告](https://www.cnblogs.com/camilla/p/7272343.html)
直接贴代码：
import csv  \# 导入scv库，可以读取csv文件
from selenium import webdriver
import unittest
from time import sleep
import time
import os
import HTMLTestRunner
import codecs
import sys
dr = webdriver.Chrome()

class testLo(unittest.TestCase):
def setUp(self):
pass
def test_login(self):
'''登陆测试'''
path = 'F:\\Python_test\\'
\# 要读取的scv文件路径
my_file = 'F:\\pythonproject\\interfaceTest\\testFile\\ss.csv'
\# csv.reader()读取csv文件，
\# Python3.X用open，Python2.X用file，'r'为读取
\# open(file,'r')中'r'为读取权限，w为写入，还有rb，wd等涉及到编码的读写属性
\#data = csv.reader(codecs.open(my_file, 'r', encoding='UTF-8',errors= 'ignore'))
with codecs.open(my_file, 'r', encoding='UTF-8',errors= 'ignore') as f:
data=csv.reader((line.replace('\x00','') for line in f))
\# for循环将读取到的csv文件的内容一行行循环，这里定义了user变量(可自定义)
\# user[0]表示csv文件的第一列，user[1]表示第二列，user[N]表示第N列
\# for循环有个缺点，就是一旦遇到错误，循环就停止，所以用try，except保证循环执行完
print(my_file)
for user in data:
print(user)
dr.get('https://passport.cnblogs.com/user/signin')
\# dr.find_element_by_id('input1').clear()
dr.find_element_by_id('input1').send_keys(user[0])
\# dr.find_element_by_id('input2').clear()
dr.find_element_by_id('input2').send_keys(user[1])
dr.find_element_by_id('signin').click()
sleep(1)
print('\n' + '测试项：' + user[2])
dr.get_screenshot_as_file(path + user[3] + ".jpg")
try:
assert dr.find_element_by_id(user[4]).text
try:
error_message = dr.find_element_by_id(user[4]).text
self.assertEqual(error_message, user[5])
print('提示信息正确！预期值与实际值一致:')
print('预期值：' + user[5])
print('实际值:' + error_message)
except:
print('提示信息错误！预期值与实际值不符：')
print('预期值：' + user[5])
print('实际值:' + error_message)
except:
print('提示信息类型错误,请确认元素名称是否正确！')

def tearDown(self):
dr.refresh()
\# 关闭浏览器
dr.quit()

if __name__ == "__main__":
\# 定义脚本标题，加u为了防止中文乱码
report_title = u'登陆模块测试报告'
\# 定义脚本内容，加u为了防止中文乱码
desc = u'登陆模块测试报告详情：'
\# 定义date为日期，time为时间
date = time.strftime("%Y%m%d")
time = time.strftime("%Y%m%d%H%M%S")
\# 定义path为文件路径，目录级别，可根据实际情况自定义修改
path = 'F:\\Python_test\\' + date + "\\login\\" + time + "\\"
\# 定义报告文件路径和名字，路径为前面定义的path，名字为report（可自定义），格式为.html
report_path = path + "report.html"
\# 判断是否定义的路径目录存在，不能存在则创建
if not os.path.exists(path):
os.makedirs(path)
else:
pass
\# 定义一个测试容器
testsuite = unittest.TestSuite()
\# 将测试用例添加到容器
testsuite.addTest(testLo("test_login"))
\# 将运行结果保存到report，名字为定义的路径和文件名，运行脚本
report = open(report_path, 'wb')
\#with open(report_path, 'wb') as report:
runner = HTMLTestRunner.HTMLTestRunner(stream=report, title=report_title, description=desc)
runner.run(testsuite)
\# 关闭report，脚本结束
report.close()

csv文件格式：
![](https://images2017.cnblogs.com/blog/74815/201708/74815-20170802100121490-513807415.png)
备注：
使用python处理中文csv文件，并让execl正确显示中文（避免乱码）设施编码格式为：utf_8_sig，示例：
'''''
将结果导出到result.csv中,以UTF_8 with BOM编码（微软产品能正确识别UTF_8 with BOM存储的中文文件）存储
'''
\#data.to_csv('result_utf8_no_bom.csv',encoding='utf_8')\#导出的结果不能别excel正确识别
data.to_csv('result_utf8_with_bom.csv',encoding='utf_8_sig')





