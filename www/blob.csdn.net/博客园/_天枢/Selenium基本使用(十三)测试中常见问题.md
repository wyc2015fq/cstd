# Selenium基本使用(十三)测试中常见问题 - _天枢 - 博客园
## [Selenium基本使用(十三)测试中常见问题](https://www.cnblogs.com/yhleng/p/8074267.html)
2017-12-20 16:03 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8074267)
**我们在使用selenium测试过程中,经常会遇到这样的问题:**
**1.frame或iframe**
```
<iframe id='frame1' name='frame1'>
    
<input type='btuuton' name = 'btn1' value=''/>
   
    <iframe id='frame2' name='frame2'>
        <input type='btuuton' name = 'btn2' value=''/>
    </iframe>
</iframe>
```
**frame层层切换,  定位button并单击.**
```
driver.switch_to.frame(frame1)
    driver.switch_to.frame(frame2)
    driver.find_element_by_name('btn2').click()
```
切换frame后,如果还想回到外层去操作其它元素,  记得要切回去
```
driver.switch_to.default_content()
```
**切换frame  四种方法:**
```
driver.switch_to.frame(id) #通过frame id
    driver.switch_to.frame(name) #通过frame name
    driver.switch_to.frame(index) #通过frame 索引  从0开始
    driver.switch_to.frame(webelement) #定位frame元素
```
**动态frame定位,切换**
**处理动态的,也就是说没有id没有name或者有也是动态变化的.   这个我们一般是采用webelement定位再切换**
**这里就要用到xpath的内建方法contains(包含--某属性包含字符串)**
```
iframe = driver.find_element_by_xpath("//iframe[contains(@name,'fra')]")
    driver.switch_to.frame(iframe)
```
**2.获取页面中table中行数据**
![](https://images2017.cnblogs.com/blog/1149221/201712/1149221-20171220161257209-1514542957.png)
思路:
1>先定位到table元素
2>定位table**元素下**的tr元素
3>定位tr元素下的td元素
```
table = driver.findElement(By.id("table"))
    rows = table.findElements(By.tagName("tr"))
    for row in rows: #控制遍历所有行
       cols= row.findElements(By.tagName("td"))
       for col in cols: #控制遍历当前迭代行下所有列
          col.getText()
```
