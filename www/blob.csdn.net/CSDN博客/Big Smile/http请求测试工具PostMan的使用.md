# http请求测试工具PostMan的使用 - Big Smile - CSDN博客
2017年06月25日 23:36:09[王啸tr1912](https://me.csdn.net/tr1912)阅读数：875
        在测试restful风格的接口的时候，经常会用的到一些测试http请求的工具，由于这类接口形式的请求一般分为5种形式的请求，而我们如果只是在浏览器中输入请求地址来访问的话一般是get类型的请求，所以我们一般需要一些工具来是实现这类接口的测试。
# 一、安装
        其实安装没有多大难度，从网上可以很轻松的找到这个程序的安装包，但是需要说明的是，这个测试程序原生支持的是chrome浏览器，所以我们最好还是需要安装一个chorme浏览器，然后从它的商店里面下载（需要蓝灯一类的工具）。下图为从应用商店处见到的搜索结果。这里需要注意从chrome处安装也有两个版本的postman可以安装，一种是在chrome里面也就是作为浏览器插件使用的一种。还有一种是作为一个电脑应用程序出现的电脑程序，个人推荐安装电脑版的，并且最好注册一个postman的账号，这样可以随时记录你的测试网站的信息。
![](https://img-blog.csdn.net/20170630074105252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 二、使用
        让我们打开postman，我们会发现这个工具的布局其实很是简单，并且我们可以很简单的发现url的输入口：
![](https://img-blog.csdn.net/20170630075405393?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        然后我们可以看到在地址输入的上方有很多的标签可以建立和删除，我们可以同时测试多个地址，这个和浏览器接班没有什么区别，方便我们进行多个地址的测试。
        同时我们还可以看到在左侧有一个测试记录的地方，我们也可以随时测试已经测试过的地址，而且可以发现如果我们登陆了postman的账号，这些地址是可以在不同地方同步的，非常方便。
        接下来就是看请求的选择了，我们在输入框的左侧可以看到有多达15中请求类型可以供我们选择极大的方便了我们的测试需要。
![](https://img-blog.csdn.net/20170630081303601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        我们还可以发现在输入框右侧有一个send按钮和一个save按钮，通过这两个按钮，我们可以发送请求和把此请求保存到我们云端，这两个按钮也是很常用的。我们在发送请求的时候甚至可以选择是发送还是下载也就是根据测试类型不同，我们的操作也可以不同。在进行save的时候也是一样的，我们可以选择进行保存，也可以选择进行加标签进行存储。
![](https://img-blog.csdn.net/20170630081237374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 三、扩展
        除了进行常规的请求测试，这个工具还可以进行一些高级的带参或者带权限的请求测试。
比如我们需要在请求的时候带一些我们后台要接收的参数，那么我们就需要点击一下输入框的右边，有个叫params的按钮，这个按钮按下就会出现一个key值的输入框：
![](https://img-blog.csdn.net/20170630075846414?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
         通过这个输入框我们可以输入一些我们需要传递的值，并且可以直接输入我们需要的参数和值，他会自动添加到请求url的后面。
在输入框的下方有一个叫做Authorization的选项，我们可以通过这里输入我们的请求所需要的用户名和密码，默认是没有请求限制的，我们可以认为加上一个权限限制：
![](https://img-blog.csdn.net/20170630080757814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        如图我们加上了一个基础的权限请求，用户名是sam，密码是1234，这样就可以带权限的方式进行请求的发送了，可以避免一些因权限限制而不能进行的请求。这个是一般浏览器所没有的功能。
        我们还可以看到里面还有很多可以在发请求的时候附带的东西，比如头值，预执行脚本，还有测试用文本等等，等待着我们去发掘。
# 四、总结
        其实这个工具我也是刚刚接触，但是发现他的功能真的是异常强大，如果早点知道这个工具的话能够为我省去不少事情，同时也了解到了我们输入的url其实都相当于是请求，并且都是有请求类型存在的，只是我们不知道罢了，现在知道了却更想要了解一下以前编写的程序都是走的什么请求了，对此很好奇，不知道读者们对于自己写的程序所用的请求有没有一丝好奇呢？
