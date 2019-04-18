# SVN 看这个就够了 - weixin_33985507的博客 - CSDN博客
2016年03月07日 22:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
0、自己搭建个人服务器。当然，在公司，会有人专门给你配置账号和密码的，账号和密码是加过密的。除非，公司确实没啥人。
![1197386-e7af1eb92d0977ec.png](https://upload-images.jianshu.io/upload_images/1197386-e7af1eb92d0977ec.png)
桌面.png
1、给你刚搭建的SNV 配置一些属性，例如：权限、额外账号等。
![1197386-0322937a817adaa7.png](https://upload-images.jianshu.io/upload_images/1197386-0322937a817adaa7.png)
打开显示.png
2、目的是修改后和服务器有关联。
![1197386-8dbd8efefc44fee8.png](https://upload-images.jianshu.io/upload_images/1197386-8dbd8efefc44fee8.png)
导出并修改名字.png
3、2个文件是一样的，testLLL是你要上传到SVN的，SNV-testLLL是从SVN上下载下来的。然后，正式开发时，把原先的testLLL删除，只使用SNV-testLLL.为啥这样搞啊？
- 
这样做的好处是:1、这样可以检测你的SVN是可以用的，团队的成员也可以下载，一起团队开发。2、SVN-testLLL改变时，会同步到SVN。
![1197386-c345ddc9149ed012.png](https://upload-images.jianshu.io/upload_images/1197386-c345ddc9149ed012.png)
删除最初的.png
4、其它情况要提交，就是增加和删除，和下图有区别的都要提交！！！！
![1197386-f7183080fb3ab1e2.png](https://upload-images.jianshu.io/upload_images/1197386-f7183080fb3ab1e2.png)
不鸟它，就是忽略的意思嘛，或者直接删掉它，然后右击ignore忽略它.png
### 稍微在详细介绍一下：
![1197386-1236156620a950ad.png](https://upload-images.jianshu.io/upload_images/1197386-1236156620a950ad.png)
选择性的提交.png
- 
UserInterface.xcuserstate 很明显啊！看后缀名字就知道，它是Xcode 的一个记录用户状态的信息而已，例如：你打开xib 或 storyboard 等操作时，就记录你的转态。其实这是没必要提交的吧！
- 
所以，为了解决这个烦人的东西，我们可以把它直接删除丢到垃圾桶，让后command+Q关掉工程。
- 
打开工程时：出现如下如：
![1197386-0e2084617a7fcdb1.png](https://upload-images.jianshu.io/upload_images/1197386-0e2084617a7fcdb1.png)
忽略.png
- 可以点击submit 直接提交，然后就不会出现了。
- 或者操作就是，先忽略，然后选择提交，然后，你发现可能是 问号，再次提交，忽略。
# 让我们---- 回到过去！！！
- SVN 理论上是没有后悔药吃的，提交了得话，就那样吧！
- 但是，还是可以回到过去的。例如：朋友在一家大公司工作，他之前写了某些功能，并提交了，但是，后来某一天，突然发现，该做法是不行的，想回到过去！怎么办？
- 其实很简单：先 check out ，然后，出现如下的提示：
![1197386-3f3cf145460c4471.png](https://upload-images.jianshu.io/upload_images/1197386-3f3cf145460c4471.png)
回到过去.png
- 选择1 箭头指向的 Additional Options
- 选择2 箭头指向的 Revision 然后展开。选择你要回到的时间点，然后，Check out下来，之前的代码有了，爱干嘛就干嘛咯！
### 项目中少不了使用第三方库、SDK等，那么我们的.a 文件默认是不会被上传到SVN的，那么？就该这样去到【偏好设置】里找到如下图解决。
![1197386-1a424ac5b2484cbe.png](https://upload-images.jianshu.io/upload_images/1197386-1a424ac5b2484cbe.png)
含第三方库.png
- 在Versions中手动添加文件
选择Versions的菜单View-->Show Ignored Items，这样就会显示出ignored的文件，找到你要上传的.a文件，右键“Add”就可以了。
#### 扩展：
- 上图的东西好烦啊！没错！你每次提交时，都会出现那几个东西，事实上它们是不需提交的。怎么一次性搞定它呢？？？
- 
解决很简单，只需右击选择删除项即可。
![1197386-e195eeb01336543e.png](https://upload-images.jianshu.io/upload_images/1197386-e195eeb01336543e.png)
不需提交，右击删除最后忽略.png
#### 如果你的SVN出现红色D怎么解决呢？
- 
###### 答案：*直接点击Resolve 就OK了，嗯，是不是很简单啊！*
![1197386-1b9d2b6654a81289.png](https://upload-images.jianshu.io/upload_images/1197386-1b9d2b6654a81289.png)
当出现红色C时，点击resoverj解决冲突.png
5、这个就要提交了：
![1197386-cc436e2f6c719654.png](https://upload-images.jianshu.io/upload_images/1197386-cc436e2f6c719654.png)
这个就要提交了.png
#### 感叹号，怎么解决啊！大神！
- 
### 答：直接删掉咯！！！（右击---》选择delete就可以啦，然后你会看到变红色了，不急，请下一张图）
![1197386-5e9434803ae80c24.png](https://upload-images.jianshu.io/upload_images/1197386-5e9434803ae80c24.png)
感叹号就要删除掉.png
#### 弱弱问一下，下面【黄色】的问号怎么处理呢？
- 
#### 答：就是自己手动添加。
![1197386-5e435c1817a643a7.png](https://upload-images.jianshu.io/upload_images/1197386-5e435c1817a643a7.png)
问号就是要手动添加.png
#### 如果提交时碰到像下面这样的警告，先add一下，解决方法是：1、（先点击取消“cancel”按钮）右击-》add ；2、也可以点击下面的Add文件夹进行处理。
- 
- 
像下面的 提示："XXXX" contains unversersioned files，表示，还没有添加的文件。
- 
使用场景：（修改 或 删除的文件及文件夹太多时，使用这种方法就达到高效率了）
- 
1、创建工程时，有时【路径修改太多了】，就选中全部的修改文件，然后，选择 commit -----》弹出下面的提示框"XXXX" contains unversersioned files 然后，理所当然的选 “Add 头Working Copy”，把文件或 删除后的红色文件处理掉。
- 
2、【添加新文件夹和文件太多了】，例如：添加一个第三方库，难道你要手动一个一个的添加？，肯定是和 1 中得操作一样啦。
- 
3、删除的东西太多了，一大片红色啊！和 1中得操作一样。
![1197386-0fa8ecc5682c1059.png](https://upload-images.jianshu.io/upload_images/1197386-0fa8ecc5682c1059.png)
警告.png
### add一下。
![1197386-369c8c9f3fd03084.png](https://upload-images.jianshu.io/upload_images/1197386-369c8c9f3fd03084.png)
add.png
- 看完了，给个赞啊！
