# 浅谈 Cookie 与 Session的关系 - zhuzhuxia183的博客 - CSDN博客





2018年06月29日 11:30:15[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：194








前段时间，有和朋友一起讨论起在爬虫中， Cookie和Session的一些关系，可是总感觉二者之间的关系似乎总是十分的模糊，所以之前找了一个时间，好好地捋了下它们的一些关系

## 首先我们来了解一下背景知识

首先http是一个无状态的连接，连接一次便会断开，我们可以想象，一个服务器，很多台电脑使用浏览器通过URL去访问它，而且因为会有多次点击，每次基本上会产生一次连接，而且有些连接其实是需要经过身份验证才可以进行业务操作，所以问题来了，怎么解决呢？

## 分别看看 Cookie 和 Session

### (1) Cookie:

wiki百科上面的原话是说 ：

An HTTP cookie (also called web cookie, Internet cookie, browser cookie, or simply cookie) is a small piece of data sent from a website and stored on the user’s computer by the user’s web browser while the user is browsing. Cookies were designed to be a reliable mechanism for websites to remember stateful information (such as items added in the shopping cart in an online store) or to record the user’s browsing activity (including clicking particular buttons, logging in, or recording which pages were visited in the past). They can also be used to remember arbitrary pieces of information that the user previously entered into form fields such as names, addresses, passwords, and credit card numbers.

*对于上面进行简单理解可知，关于cookie的定义，我们可知 *

1、cookie一般可以认为是浏览网站时候被用户浏览器存储在用户电脑上面 

的一小片数据（后面可知是键值对的形式存在） 

2、同时cookie被设计成一种可靠的机制，让浏览器来记住一些一些重要的信息或者用户的浏览活动，包括像账号密码之类的 
![](https://img-blog.csdn.net/20180629112208863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXpodXhpYTE4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
PS:这里就是浏览器存储的Cookie，分成两种：放到本地存储的cookie,和内存中的Cookie

### (2) Session的理解:

wiki百科上面的原话是说 ：

In computer science, in particular networking, a session is a semi-permanent interactive information interchange between two or more communicating devices, or between a computer and user (see login session). A session is established at a certain point in time, and then torn down at some later point. An established communication session may involve more than one message in each direction. A session is typically stateful, meaning that at least one of the communicating parts needs to save information about the session history in order to be able to communicate, as opposed to stateless communication, where the communication consists of independent requests with responses.

*对于上面进行简单理解可知，关于 session的定义，我们可知 *

session中文含义会话，一个会话建立在某个时间点，然后在稍后的时间点被拆掉。非无状态通信，即Http，其中通信由具有响应的独立请求组成。

*网站中的Session概念，是在保存在服务器的内存中*

PS:嘻嘻嘻，这里没有图

## 来看看解决方案：

### 使用Session+Cookie:

设想一下，我们第一次登陆网站，是没有任何记录的，需要输入账号密码，然后经过验证后，后台可以确认你的身份的，然后会自动生成一个Session，同时产生一个标识本次会话的SessionID，在处理请求之后的response中会返给浏览器，浏览器会把这个SessionID保存在本地Cookie中，之后每次访问这个网站，都会带上这个SessionID的信息，服务器之后每次收到请求中，如果Cookie中带有SessionID信息，首先会判断此SessionID标识的会话是否存在，如果存在把存在会话查找出来（PS:会话有很多，不需要我们进行管理，一般的网站服务器会自动管理），不存在就创建一个新的。 
![这里直接进入主页，就不要登陆直接可以进入](https://img-blog.csdn.net/20180629111822680?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXpodXhpYTE4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*这里需要注意的是，这里的Session是有一定时间限制的，长时间的话，服务器会删除此处会话信息，具体的规则可以去google或者百度下 ，而且这里储存SessionID的Cookie，是内存Cookie,关掉浏览器之后便会删除。通常来说，我们一般关掉浏览器之前的网站，Session在服务器中并不会立即删掉，因为服务器并不知道我们关闭了浏览器，服务器是不会有任何立即相应的，重新打开主页如果登不上了，跳到登陆界面，可能是本地存储SessionID的Cookie被删了*
### 只使用Cookie能够干嘛：

*存储账号密码：比如你之前输入的账号密码，不保存，下次登陆就不会自动填满的，这个和服务器没有关系，只是你的浏览器用了Cookie存储了而已，别怕，都已经加密了。*

### 只使用Cookie可不可以实现Session+Cookie的效果呢？

*答案我觉得是可以的*
*我们每次发送Request时在Cookie那里填入标示账号的一个标识值，然后后台进行验证，当然这个标示规则得前后台统一一致，并且做到标示值不可逆且不能重复*

*PS:一开始的时候是没有Session这个概念的,Cookie比Session先出现。其实我们可以认为Session里面的SessionID，也是一种标示某个账号的标示值，并且它做到了不可逆且不易重复，不过它是服务器产生发送给浏览器存储的*





