# 4个Shell小技巧，帮你提高机器学习生产效率 - 知乎
# 



> 晓查 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

在机器学习的实践过程中，用好Shell能帮你很多节省时间。

最近，有位来自ETHZ的学生分享了一些Shell小技巧。对程序员来说，这些技巧更重要的是让你的思维从琐碎小事中解脱出来，大大提高了工作效率。
![](https://pic3.zhimg.com/v2-d055e1d5f1aa555191969dfb942ba52a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='663'></svg>)
下面就是他分享的4个tips。由于本文中涉及到的shell脚本过多，你可以去文末地址中查看所有脚本的源代码。

## **抓取远程文件**

当你在远程服务器上监视程序运行时，如果想把一个远程文件抓取到本地查看，通常你会怎么做？

记下文件路径，打开终端，用rsync之类的工具同步到本地，再到文件浏览器中查看。

实际上不必这么麻烦。只需要事先写好几个shell脚本，就可以避免重复的机械劳动。

在这里强烈推荐**iTerm 2**，它比Mac自带的终端要强大得多，可以指定某个触发关键词执行某个相应的命令。
![](https://pic3.zhimg.com/v2-8dc608600cd0cb412ce1158a22a063de_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='421'></svg>)
iTerm 2下载地址：
[iTerm2 - macOS Terminal Replacement​www.iterm2.com](https://link.zhihu.com/?target=https%3A//www.iterm2.com/)
先在远程服务器上设置一个脚本**t**。当我们输入r awesome_video.mp4时，它会搜索awesome_video.mp4文件所在路径，并连同主机名以字符串rtransfer <host> <path>的形式打印出来。
![](https://pic1.zhimg.com/v2-b97d181e48242fb52d2b70383dd13314_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)
rtransfer作为iTerm2的触发关键词，解析出主机和路径，然后调用另一个本地脚本**t2**。脚本t2将这个视频文件传输到临时目录，然后在该目录中打开Finder。
![](https://pic4.zhimg.com/v2-8a082f0a03cbfe47d4e6b6bd95105ccf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='715'></svg>)
使用iTerm 2触发关键词功能调用脚本，可以大大提升效率，而你要做的只是在前期花费时间定制脚本。

## **远程访问TensorBoard**

除了抓取远程文件，另一个让人痛苦的是访问远程服务器上的TensorBoard实例。你可以设置SSH端口转发，但是哪个端口对应哪个服务器？它们都在Docker容器中。

**ngrok**可以让你把访问本地端口变成访问URL，比如输入ngrok http 6006后，它会为你生成一个URL网址[http://683acac3.ngrok.io](https://link.zhihu.com/?target=http%3A//683acac3.ngrok.io)，你可以在这个地址中查看TensorBoard实例。

结合脚本**n**，以更快的速度启动ngrok，然后用脚本**tb**打开Web浏览器，启动TensorBoard，在十秒内就能从运行目录转到显示图形。
![](https://pic1.zhimg.com/v2-ee3827e0535b46f2567908d19a3a3478_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='892' height='560'></svg>)
ngrok有个缺点是它一次只能允许一个会话，因此在使用前需要杀死上一个ngork进程。如果你忘记在另外一台服务器上杀死ngrok，可能会比较麻烦。

ngrok下载地址：
[secure introspectable tunnels to localhost​ngrok.com![图标](https://pic2.zhimg.com/v2-e9750d78f3b321a6f601fba6004b3959_180x120.jpg)](https://link.zhihu.com/?target=https%3A//ngrok.com/)



## **用tbplot代替TensorBoard绘图**

对于运行大量metrics的情形，我们需要等待较长时间让TensorBoard加载所有图形。如果使用**tbplot**脚本，速度会快很多，并且能直接生成图像文件。



![](https://pic1.zhimg.com/v2-dc6d547a02bd9ec4adcdff943d5dcbd8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='674'></svg>)
tbplot实际上调用的是Matplotlib，缺点是目前只能生成标量图。

tbplot下载地址：
[mrahtz/tbplot​github.com![图标](https://pic2.zhimg.com/v2-b1cfbebd551226db78a936f748c72259_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/mrahtz/tbplot)
## **自动化崩溃监测**

运行代码时，最耗费精力的事情就是是担心遇到了错误并崩溃，因此需要不断检查他们的运行情况。

当程序出现问题时，通过警报自动监控可以缓解这种担忧。一般可以把警报发送到邮箱中，为了避免这么麻烦，可以使用**sqs-alerts**，它依靠AWS AWS Simple Queue服务存储和接收消息。

在每台远程机器上，使用cron运行一个脚本，监控日志并在检测到运行中断时向队列发送消息。然后在本地机器上运行一个服务来监控队列，并在收到消息时弹出警报。
![](https://pic2.zhimg.com/v2-baa573b35090e8c605471d072d0b3c69_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='590' height='420'></svg>)
sqs-alerts下载地址：
[mrahtz/sqs-alerts​github.com![图标](https://pic2.zhimg.com/v2-b1cfbebd551226db78a936f748c72259_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/mrahtz/sqs-alerts)



最后，本文使用的所有shell脚本都可以在以下地址中找到：

[mrahtz/dotfiles​github.com![图标](https://pic2.zhimg.com/v2-b1cfbebd551226db78a936f748c72259_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/mrahtz/dotfiles/tree/master/.local/bin)
— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


