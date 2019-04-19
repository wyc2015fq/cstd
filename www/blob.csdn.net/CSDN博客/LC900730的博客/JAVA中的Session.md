# JAVA中的Session - LC900730的博客 - CSDN博客
2017年07月25日 18:45:01[lc900730](https://me.csdn.net/LC900730)阅读数：134
## Session技术
Cookie局限性： 
- 1.Cookie只能存字符串类型，不能保存对象；
- 2.非中文；
- 3.一个cookie中容量不超过4KB
- 4.可以被浏览器清除
Session特点： 
    会话数据保存在服务器端(内存中)；
HttpSession类：用于保存会话数据 
- 1.创建或者得到session对象
```
HttpSession getSession();
HttpSession getSession(boolean create);
```
服务器能够识别不同的浏览者。
浏览器1： 
1.创建session对象，保存对话数据 
浏览器1的窗口：可以取出； 
新的浏览器1：不可以 
浏览器2：不可以
但是代码相同，有的可以拿到session对象，有的不能拿到session； 
背后就是sessionID
session s1，分配一个唯一的标记，如s001，下一次用新的窗口访问
HttpSseeion session=request.getSession();
1.第一次访问创建session对象，给session对象分配一个唯一的ID，叫做JSESSIONID 
2.把JSESSIONID作为Cookie的值发送给浏览器保存； 
3.第二次访问 ，浏览器带着JSESSIONID的cookie访问服务器； 
4.服务器得到JSESSIONID，在服务器内存中搜索是否存放着对应编号的session对象 
5.如果找到则返回 
6.找不到则创建新的session对象，继续走1流程
### Session细节
|session.getId();|得到session编号(找不到则返回null)|
|----|----|
|setMaxinactiveInterval(int interval)|设置session有效时间，默认30分钟自动回收。此方法可以修改时间，单位是秒|
#### 如何避免手动关闭浏览器JSESSION的cookie随着浏览器关闭而丢失的问题
手动发送一个硬盘级别的保存 cookie给浏览器 
Cookie c=new Cookie(“JSESSIONID”,session.getId()); 
c.setMaxAge(60*60); 
response.addCookie(c);
