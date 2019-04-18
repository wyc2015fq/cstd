# HTML5基础知识汇总_(2)自己定义属性及表单新特性 - weixin_33985507的博客 - CSDN博客
2017年08月05日 11:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
# 自己定义属性data-*
说起这个属性,事实上如今非经常见了;怎么说呢,由于在一些框架都能看到他的身影!!! 
比方Jquery mobile,里面非常频繁的使用了这个属性;
这个属性是哪里来的….当然是尾随最新的H5一起出来的….. 
兼容性在PC端仅仅能呢说一般般(眼下.比較老式浏览器居多),,手机端支持还是比較OK的;
虽说是自己定义属性,可是还是有一定的规格的,,比方前缀必须是**data-[自己定义属性]**;
比方
`    <div data-id="id"></div>`
这货有什么用呢?用来操作数据的居多,比方给一款游戏弄一些额外的信息!!(data-level,data-score); 
而操作数据一般有两种方式(原生JS和JQ): 
1. 利用 getAttribute、setAttribute 存取 data[原生JS操作] 
2. 利用 dataset API 存取 dataset [这样的能够直接忽略data-,相似对象直接訪问属性,也是JS操作的] 
3. jQuery使用attr 和 data 依次等同于上面两种
**在网上发现了一篇解说比較具体的操作数据,能够瞄瞄**
传送门: [http://blog.netsh.org/posts/html-data-jquery_1812.netsh.html](http://blog.netsh.org/posts/html-data-jquery_1812.netsh.html)
# 表单的进化!
## input添加的特性!
### email
`邮箱: <input type="email" name="user_email" >`- 若是用submit提交会验证Email,检查是否缺少@,@后面是否为空….挺齐全的
### url
`首页: <input type="url" name="user_url" >`- 同上,会检查内容格式,眼下仅仅支持绝对路径!!!!
### number
`页数: <input type="number" name="pagin" min="1" max="30" />`- 有四个属性: 
- min : 最小值 , 小于会报错提醒
- max : 最大值, 大于会报错提醒
- step : 默觉得1, 能够看需求设置
- value : 设置默认值,这个跟传统的一样 
### range
`范围:<input type="range" name="range" min="5" max="25" />`- 这个和上面那个非常相似,连属性值都一致了..就是表现形式不同,,显示为滚动栏
- 值得一提的是,宽高度的不一致决定了滚动栏的方向是水平还是垂直;**h>w**.垂直!!
### Date pickers
` <input type="[attribute]" name="user_week" />`
[attribute]换成以下的属性就能看到各种效果了,,就是兼容性不怎么样,chrome下正常; 
等到各大浏览器都支持良好了,就能代替JS日期选择器了…. 一条代码搞定一个日期选择;
- date - 选取日、月、年
- month - 选取月、年
- week - 选取周和年
- time - 选取时间（小时和分钟）
- datetime - 选取时间、日、月、年（UTC 时间）
- datetime-local - 选取时间、日、月、年（本地时间）
### search
`<input type="search" name="user_search" > `- 跟text相比,多了一个可关闭的X;另一些样式上的差别;其它基本一致
### tel
`联系方式： <input type="tel" name="user_tel" > `- 和text大同小异,支持不论什么字符输入
### color
`最喜欢的颜色： <input type="color" name="color" > `- 这个属性兼容性不是非常好,可是能够支持的浏览器体验非常好,就是一个**color picker**!!!
### required
`邮箱: <input type="email" name="user_email" required="required">`- 防止域为空时提交表单,返回值是boolean, 设置了必须通过验证才干提交
### placeholder
`邮箱: <input type="email" name="user_email" required="required" placeholder="请输入您的邮箱!!">`- 这个属性见怪不怪了,,有点相似占位符,显示一行浅灰色的字体,可是又是能够编辑的,也能够通过JS获取当输入为空的时候,用placeholder的值替换
### pattern
```
电话号码: <input type="text" name="phone"
pattern="[1-9]{11}" title="联系人号码" />
```
- 眼下非常少看到, 可是正则却非经常见,,待慢慢普及吧
### novalidate
```
<form action="#" method="post" novalidate="true">
邮箱: <input type="email" name="user_email" required="required" placeholder="请输入您的邮箱!!" novalidate="true">
<input type="submit" />
</form>
```
- H5特性之中的一个,验证表单值;
- novalidate的作用就是取消验证,可作用于表单和input元素;
- 默认不设置则为验证!
### multiple
```
<form action="#" method="post" novalidate="true">
邮箱: <input type="email" name="user_email" required="required" placeholder="请输入您的邮箱!!" novalidate="true" multiple="multiple">
<input type="submit" />
</form>
```
- 支持上传(file)和邮箱控件(email)输入多个值,值之间用逗号隔开(半角)
### autofocus
`    <input autofocus="autofocus" type="text">`- 自己主动获取焦点
### autocomplete
`    <input autofocus="autofocus" type="text" autocomplete="no">`- h5下该属性添加了开关功能(on/off),来决定是否自己主动完毕
## 表单新元素
### datalist
```
搜索引擎: <input type="url" list="url_list" name="link" />
        <datalist id="url_list">
            <option label="Bing" value="http://www.bing.com" />
            <option label="Baidu" value="http://www.baidu.com" />
            <option label="Microsoft" value="http://www.microsoft.com" />
        </datalist>
```
- 如需把 datalist 绑定到输入域，请用输入域的 list 属性引用 datalist 的 id
- 下拉默认值设置在option的value中
### keygen
```
<form action="#" method="post">
username: <input type="text" name="usr_name" >
加密方式: <keygen name="security" >
<input type="submit" >
</form>
```
- 非常多浏览器对这属性支持还不大完好,,,,
- 加密方式两种,原理还是通过公钥和密钥的方式(相似SSH)
### progress
```
<progress value="5" max="70">
</progress>
```
- 进度条制作利器!!,用来推断file这些的载入挺不错的….
- 必须设置max值才干显示相应的进度条,,不支持百分比!!!
### meter
`<div>您的额度：<meter value="50001" min="0" max="10000" low="1000" high="9000" optimum="6000">尽情耍!!</meter> </div>`- 用的不多,,眼下;也是进度条效果
- min/max是最小值和最大值;
- value是当前值
- low和high是低于和高于最优值(optimum)
# 总结
表单来了一次大跃进,可是吧,,眼下还是没法顺心的用,,一些浏览器没跟上,另一些用户使用老版本号浏览器; 
所以这些新特性的使用,,哪怕再人性化,也仅仅能慢慢来;过渡总须要时间的~~~
