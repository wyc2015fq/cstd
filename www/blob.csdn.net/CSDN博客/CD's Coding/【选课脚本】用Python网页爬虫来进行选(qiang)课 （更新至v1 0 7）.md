# 【选课脚本】用Python网页爬虫来进行选(qiang)课 （更新至v1.0.7） - CD's Coding - CSDN博客





2017年06月01日 08:53:18[糖果天王](https://me.csdn.net/okcd00)阅读数：9037








## 0x00 前言

每当选课的时候，都如同打仗一般 

都有自己想要的课，但是名额就那么一点 

于是各显神通，有人用js，有人用chrome的console 

人生苦短，我用Python
## 0x01 环境依赖
- Python 2.7.12
- (NEW) Python 3.3 & Python 3.6
- pip freeze > Requirement.txt

**Requirement.txt**

```
beautifulsoup4==4.6.0
bs4==0.0.1
configparser==3.5.0
lxml==3.7.3
requests==2.13.0
tqdm==4.11.2
```

## 0x02 使用方法

### 获取程序

你可以直接`git clone`最新版本的程序

```
$ git clone https://github.com/okcd00/CDSelector.git
$ cd CDSelector
$ vim config 修改登陆信息
$ vim courseid 修改选课信息
$ python CDSelector
```

你也可以去`release`下载当前最新的稳定版Release 
[https://github.com/okcd00/CDSelector/releases](https://github.com/okcd00/CDSelector/releases)

### 修改文件 **config**

```
[info]
username = [你的帐户名邮箱]
password = [你的密码]
runtime  = [每隔多少秒尝试选课一次]

[action]
debug = true [debug模式输出中间变量，为节省资源可设置为false]
enroll = true [轮询模式下无限循环尝试，没想过什么情况下需要设为false]
evaluate = true [验证选课成功与否，建议开启]
select_bat = false [批选课，应用于类似英语B这种不让单独选，必须同时选俩才给提交表单的特殊情况]
```

### 修改文件 **courseid**

类似如下的每行一门课的课程编号即可，

```
091M7014H
091M7021H
```

特别的，如果这门课要选成学位课的话，

```
091M7014H
091M7021H on
```

形似第二行加个空格加个`on`即可

### 然后运行 **CDSelector.py**

```
$ python CDSelector.py
Debug Mode: True
Login success
Enrolling start
> Course Selection is unreachable or not started. <1134> Thu Jun 01 08:43:42 2017
```

如果显示ImportError，就是说缺少了某些python包，在这里提供了`requirement.txt`

可以在当前目录下直接
`$ pip install -r requirement.txt`
一次性安装所有依赖项

## 0x03 Source Code

由于时不时还更新一下，所以这里只是v1.0.0首发版本 

web端访问部分参考了scusjs，功能强化参考了zoecur

(Updated 2017-09-07) 现在更新一波，v1.0.7版本。

```python
#coding=utf8
# =====================================================
#   Copyright (C) 2016 All rights reserved.
#   
#   filename : CDSelector.py
#   author   : okcd00 / okcd00@qq.com
#   refer    : scusjs@foxmail.com
#   date     : 2017-01-06
#   desc     : UCAS Course_Selection Program
# =====================================================

import os
import sys
import time
import requests
from bs4 import BeautifulSoup
from configparser import RawConfigParser

class UCASEvaluate:
    def __init__(self):
        self.__readCoursesId('./courseid')

        cf = RawConfigParser()
        cf.read('config')

        self.username = cf.get('info', 'username')
        self.password = cf.get('info', 'password')
        self.runtime = cf.getint('info', 'runtime')
        self.debug = cf.getboolean('action', 'debug')
        self.enroll = cf.getboolean('action', 'enroll')
        self.evaluate = cf.getboolean('action', 'evaluate')
        self.select_bat = cf.getboolean('action', 'select_bat')

        self.loginPage = 'http://sep.ucas.ac.cn'
        self.loginUrl = self.loginPage + '/slogin'
        self.courseSystem = self.loginPage + '/portal/site/226/821'
        self.courseBase = 'http://jwxk.ucas.ac.cn'
        self.courseIdentify = self.courseBase + '/login?Identity='
        self.courseSelected = self.courseBase + '/courseManage/selectedCourse'
        self.courseSelectionBase = self.courseBase + '/courseManage/main'
        self.courseCategory = self.courseBase + '/courseManage/selectCourse?s='
        self.courseSave = self.courseBase + '/courseManage/saveCourse?s='

        self.studentCourseEvaluateUrl = 'http://jwjz.ucas.ac.cn/Student/DeskTopModules/'
        self.selectCourseUrl = 'http://jwjz.ucas.ac.cn/Student/DesktopModules/Course/SelectCourse.aspx'

        self.enrollCount = {}
        self.headers = {
            'Host': 'sep.ucas.ac.cn',
            'Connection': 'keep-alive',
            'Pragma': 'no-cache',
            'Cache-Control': 'no-cache',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
            'Upgrade-Insecure-Requests': '1',
            'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.80 Safari/537.36',
            'Accept-Encoding': 'gzip, deflate, sdch',
            'Accept-Language': 'zh-CN,zh;q=0.8,en;q=0.6',
        }

        self.s = requests.Session()
        loginPage = self.s.get(self.loginPage, headers=self.headers)
        self.cookies = loginPage.cookies

    def login(self):
        postdata = {
            'userName': self.username,
            'pwd': self.password,
            'sb': 'sb'
        }
        self.s.post(self.loginUrl, data=postdata, headers=self.headers)
        if 'sepuser' in self.s.cookies.get_dict(): return True
        return False

    def getMessage(self, restext):
        css_soup = BeautifulSoup(restext, 'html.parser')
        text = css_soup.select('#main-content > div > div.m-cbox.m-lgray > div.mc-body > div')[0].text
        return "".join(line.strip() for line in text.split('\n'))

    def __readCoursesId(self, filename):
        coursesFile = open(filename, 'r')
        self.coursesId = {}
        for line in coursesFile.readlines():
            line = line.strip().replace(' ', '').split(':')
            courseId = line[0]
            isDegree = False
            if len(line) == 2 and line[1] == 'on':
                isDegree = True
            self.coursesId[courseId] = isDegree

    def enrollCourses(self):
        response = self.s.get(self.courseSystem, headers=self.headers)
        soup = BeautifulSoup(response.text, 'html.parser')
        try:
            identity = str(soup).split('Identity=')[1].split('"'[0])[0]
            coursePage = self.courseIdentify + identity
            response = self.s.get(coursePage)
            response = self.s.get(self.courseSelected)

            idx, lastMsg = 0, ""
            while True:
                msg = ""
                if self.select_bat: 
                    result, msg = self.__enrollCourses(self.coursesId)
                    if result: 
                        self.coursesId.clear()
                else:   
                    for eachCourse in self.coursesId:
                        if eachCourse in response.text:
                            print("Course " + eachCourse + " has been selected.")
                            continue
                        if (eachCourse in self.enrollCount and
                                self.enrollCount[eachCourse] == 0):
                            continue
                        self.enrollCount[eachCourse] = 1
                        result, msg = self.__enrollCourse(eachCourse, self.coursesId[eachCourse])
                        if result:
                            self.enrollCount[eachCourse] = 0

                for enroll in self.enrollCount:
                    if self.enrollCount[enroll] == 0:
                        self.coursesId.pop(enroll)
                self.enrollCount.clear()
                if not self.coursesId: return 'INVALID COURSES_ID'
                idx += 1
                time.sleep(self.runtime)
                showText = "\r> " + "%s <%d> %s" % (
                        msg, idx,
                        time.asctime( time.localtime(time.time()) )
                    )
                lastMsg = msg
                sys.stdout.write(showText)
                sys.stdout.flush()
        except KeyboardInterrupt:
            print("\nKeyboardInterrupt Detected, bye!")
            return "STOP"
        except Exception as exception:
            return "Course_Selection_Port is not open, waiting..."


    def __enrollCourse(self, courseId, isDegree):
        response = self.s.get(self.courseSelectionBase)
        if self.debug:
            with open('./check.html', 'wb+') as f:
                f.write(response.text.encode('utf-8'))

        soup = BeautifulSoup(response.text, 'html.parser')
        categories = dict([(label.contents[0][:2], label['for'][3:])
                          for label in soup.find_all('label')[2:]])
        categoryId = categories[courseId[:2]]
        identity = soup.form['action'].split('=')[1]

        postdata = {
            'deptIds': categoryId,
            'sb': 0
        }
        categoryUrl = self.courseCategory + identity
        response = self.s.post(categoryUrl, data=postdata)
        if self.debug:
            print ("Now Posting, save snapshot in check2.html.")
            with open('./check2.html', 'wb+') as f:
                f.write(response.text.encode('utf-8'))
        soup = BeautifulSoup(response.text, 'html.parser')
        courseTable = soup.body.form.table
        if courseTable:
            courseTable = courseTable.find_all('tr')[1:]
        else: return False, "Course Selection is unreachable or not started."
        courseDict = dict([(c.span.contents[0], c.span['id'].split('_')[1])
                           for c in courseTable])

        if courseId in courseDict:
            postdata = {
                'deptIds': categoryId,
                'sids': courseDict[courseId]
            }

            if isDegree:
                postdata['did_' + courseDict[courseId]] = courseDict[courseId]

            courseSaveUrl = self.courseSave + identity
            response = self.s.post(courseSaveUrl, data=postdata)

            print ("Now Checking, save snapshot in result.html.")
            with open('result.html','wb+') as f:
                f.write(response.text.encode('utf-8'))
            if 'class="error' not in response.text:
                return True, '[Success] ' + courseId
            else: return False, self.getMessage(response.text).strip()
        else:
            return False, "No such course"

    def __enrollCourses(self, courseIds):  # For English
        response = self.s.get(self.courseSelectionBase)
        if self.debug: 
            with open('./check.html', 'wb+') as f:
                f.write(response.text.encode('utf-8'))

        soup = BeautifulSoup(response.text, 'html.parser')
        categories = dict([(label.contents[0][:2], label['for'][3:])
                          for label in soup.find_all('label')[2:]])
        identity = soup.form['action'].split('=')[1]

        categoryIds = []
        for courseId in courseIds:
            categoryIds.append(categories[courseId[:2]])

        postdata = {
            'deptIds': categoryIds,
            'sb': 0
        }

        categoryUrl = self.courseCategory + identity
        response = self.s.post(categoryUrl, data=postdata)

        if self.debug: 
            print ("Now Posting, save snapshot in check2.html.")
            with open('./check2.html', 'wb+') as f:
                f.write(response.text.encode('utf-8'))

        soup = BeautifulSoup(response.text, 'html.parser')
        courseTable = soup.body.form.table
        if courseTable:
            courseTable = courseTable.find_all('tr')[1:]
        else: return False, "Course Selection is unreachable or not started."
        courseDict = dict([(c.span.contents[0], c.span['id'].split('_')[1])
                           for c in courseTable])

        postdata = {
            'deptIds': categoryIds,
            'sids': [courseDict[courseId] for courseId in courseIds]
        }

        courseSaveUrl = self.courseSave + identity
        response = self.s.post(courseSaveUrl, data=postdata)

        print ("Now Checking, save snapshot in result.html.")
        with open('result.html','wb+') as f:
            f.write(response.text.encode('utf-8'))
        if 'class="error' not in response.text:
            return True, '[Success] ' + courseId
        else: return False, self.getMessage(response.text).strip()


if __name__ == "__main__":
    print("starting...")
    os.system('MODE con: COLS=128 LINES=32 & TITLE Welcome to CDSelector')

    from logo import show_logo
    show_logo() # delete this for faster start 23333
    os.system('cls')

    time.sleep(1)
    os.system("color 0A")
    os.system('MODE con: COLS=80 LINES=10 & TITLE CD_Course_Selecting is working')

    while True:
        try:
            ucasEvaluate = UCASEvaluate()
            break
        except Exception as e:
            if e[0]=="Connection aborted.":
                ucasEvaluate = UCASEvaluate()

    if ucasEvaluate.debug:
        print ("Debug Mode: %s" % str(ucasEvaluate.debug) )
        print ("In debug mode, you can check snapshot with html files.")
        print ("By the way, Ctrl+C to stop.")

    if not ucasEvaluate.login():
        print('Login error. Please check your username and password.')
        exit()
    print('Login success: ' + ucasEvaluate.username)

    print('Enrolling starts')
    while ucasEvaluate.enroll:
        status = ucasEvaluate.enrollCourses()
        if status == 'STOP':
            break
        else: 
            status += time.asctime( time.localtime(time.time()) )
            sys.stdout.write("%s\r" % status)
    print('Enrolling finished')
```

## 0xFF 获取途径
- Github： [https://github.com/okcd00/CDSelector](https://github.com/okcd00/CDSelector)
- Release： [https://github.com/okcd00/CDSelector/releases](https://github.com/okcd00/CDSelector/releases)
- 说明文档： [http://blog.csdn.net/okcd00/article/details/72827861](http://blog.csdn.net/okcd00/article/details/72827861)

鸣谢： 

Mailto: zoecur@icloud.com 

Mailto: scusjs@foxmail.com




