
# python通过Cookie跳过登录验证码 - anlcy - 博客园






# [python通过Cookie跳过登录验证码](https://www.cnblogs.com/camilla/p/7160847.html)
1.通过浏览器获取登录cookie，找到重要的两个参数“BAIDUID”和“BDUSS”；
2.编写Selenium自动化测试脚本，跳过登录。
from selenium import webdriver
driver = webdriver.Chrome()
driver.get("https://www.baidu.com")
\#添加Cookie
driver.add_cookie({'name':'BAIDUID','value':'26dd76sdd36DDD7DDBFDF4:FG=1'})
driver.add_cookie({'name':'BDUSS','value':'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQ'})
\# 刷新页面
driver.refresh()
\# 获取登录用户并打印
username = driver.find_element_by_class_name("user-name").text
print(username)
driver.implicitly_wait(30)
\#关闭浏览器
driver.quit()
注意：当前登录不能退出。





