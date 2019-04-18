# python 爬虫—selenium（切换frame以及滚动条操作） - wsp_1138886114的博客 - CSDN博客





2018年06月19日 11:14:50[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：681
所属专栏：[python基础及相关](https://blog.csdn.net/column/details/23328.html)









## python 爬虫——selenium

```
使用常用的爬虫工具无法获取网页页面标签或元素时无法找到有以下两种情况：
    该网页为动态网页（可以使用selenium，操作网页下拉加载网页再进行爬取）
    该网页有多个frame标签导致无法爬取（可以使用selenium 切换frame）
```

### 一、selenium–操作滚动条

```
from selenium import webdriver  
from selenium.webdriver.common.keys import Keys  
from selenium.webdriver.common.action_chains import ActionChains  
import time  

driver = webdriver.Firefox()  
driver.get('https://www.baidu.com')  
driver.maximize_window()  
driver.find_element_by_id('kw').send_keys('python')  
driver.find_element_by_id('su').click()  
time.sleep(5)  
# first method  
# js = "window.scrollTo(0,document.body.scrollHeight)"  
# driver.execute_script(js)  
# time.sleep(3)  
# js = "window.scrollTo(0,0)"  
# driver.execute_script(js)  

# second  
# js = "var q=document.documentElement.scrollTop=10000"  
# driver.execute_script(js)  
# time.sleep(3)  
# js = "var q=document.documentElement.scrollTop=0"  
# driver.execute_script(js)  

# 滚动条拉到指定位置（具体元素）  
target = driver.find_element_by_id("rs")  
driver.execute_script("arguments[0].scrollIntoView();", target)  

#将网页滚动到网页底部
driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")

# 向下滚动200个像素，鼠标位置也跟着变了
driver.execute_script("window.scrollBy(0,200)", "") 

# 类似于按键盘上的PgDn按键，可以换成Keys.DOWN(下箭头，不过这个下拉进度比较慢)  
for i in range(6):  
    ActionChains(driver).send_keys(Keys.PAGE_DOWN).perform()  
    time.sleep(1)  

time.sleep(3)  
driver.close()
```

```
from selenium import webdriver
import time      #用来使程序运行歇息一会儿

#模拟登录百度
url = 'https://www.baidu.com'
driver = webdriver.Chrome()
driver.maximize_window()
driver.get(url)

# 找到登录按钮并点击
# login = driver.find_element_by_id('u1').find_elements_by_class_name("lb")[0]
# login.click()

login = driver.find_elements_by_css_selector("div[id=u1] a[class=lb]")[0]
login.click()
time.sleep(3)

#找到用户名登录按钮
user_name_login = driver.find_elements_by_css_selector('p.tang-pass-footerBarULogin')[0]
user_name_login.click()
time.sleep(3)

#输入账号与密码
user_name = driver.find_element_by_id("TANGRAM__PSP_10__userName")
user_name.send_keys("your_user_name")
time.sleep(3)
password = driver.find_element_by_id("TANGRAM__PSP_10__password")
password.send_keys("your_password")

#找到并点击登录按钮
denglu=driver.find_element_by_id('TANGRAM__PSP_10__submit')
denglu.click()
```

### 二、使用  selenium 方法切换 frame

```
driver.switch_to.frame(reference)    # 切换到子 frame
driver.switch_to.parent_frame()      # 切回到父 frame      
driver.switch_to.default_content()   # 切回主文档
```

> 
```
frame标签有（frameset、frame、iframe）三种，其中frameset为普通标签，不会影响到正常的定位。
而frame与iframe对selenium定位而言是一样的，selenium有一组方法对frame进行操作。
```

##### 2.1 `switch_to.frame(reference)`

**reference是传入的参数，用来定位frame，可以传入id、name、index以及selenium的WebElement对象**

```
▶ 示例：
<html lang="en">
<head>
    <title>FrameTest</title>
</head>
<body>
<iframe src="a.html" id="frame1" name="myframe"></iframe>
</body>
</html>
```

```
▶ 定位其中的iframe并切进去（对于无 id和name 这两项属性，则可以用index和WebElement来定位）
from selenium import webdriver
driver = webdriver.Firefox()
driver.switch_to.frame(0)             # 1.index 定位，从0开始
driver.switch_to.frame("frame1")      # 2.用id 定位
driver.switch_to.frame("myframe")     # 3.用name 定位

driver.switch_to.frame(               # 4.用WebElement对象定位(方法：tag_name,xpath)
      driver.find_element_by_tag_name("iframe")) 
driver.switch_to.frame(
    driver.find_element_by_xpath("//iframe[contains(@src,'myframe')]"))
```

##### 2.2  `switch_to.default_content()`从 frame 中切回主文档

切到frame中之后，我们若像继续操作主文档的元素，则需切回主文档
`driver.switch_to.default_content()`
##### 2.3`switch_to.parent_frame()` 嵌套frame的操作

```
示例：
<html>
<iframe id="frame1">
    <iframe id="frame2" / >
</iframe>
```



```
● 从主文档切到frame2，一层层切进去 
driver.switch_to.frame("frame1")
driver.switch_to.frame("frame2") 

● 从frame2再切回frame1（子frame切回到父frame）
driver.switch_to.parent_frame()  # 如果当前已是主文档，则无效果
```




