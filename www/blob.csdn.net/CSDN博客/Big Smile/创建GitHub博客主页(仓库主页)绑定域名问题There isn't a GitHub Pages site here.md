# 创建GitHub博客主页(仓库主页)绑定域名问题There isn't a GitHub Pages site here - Big Smile - CSDN博客
2018年06月17日 20:39:13[王啸tr1912](https://me.csdn.net/tr1912)阅读数：2655
![](https://img-blog.csdn.net/20180613081922448)
出现这个404 的错误，一般表示你的域名已经能够解析到了XXX.github.io，但是你的github还需要进一步配置。
首先，我们需要先进入到你的项目中去：
![](https://img-blog.csdn.net/20180617201519674?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击settings进行我们的仓库配置，之后找到GitHubPages进行页面的配置：
![](https://img-blog.csdn.net/20180617201658648?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
上图中的 domain就是我们需要设置域名的地方，我们输入完成我们的域名之后，进行绑定（点击Save）就可以了。
这个时候我们会在我们的仓库目录下面看到一个名字为CNAME的文件：
![](https://img-blog.csdn.net/20180617203505873?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    里面的内容就是你的域名的全称。
之后我们还需要再确认一下我们的域名配置（阿里云域名）。
**解析**
我们需要建立两个域名解析条例
![](https://img-blog.csdn.net/20180617203033955?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
        A记录是记录IP的，我们先要ping一下我们自己的主页的地址，我的就是ping tr1912.github.io ，然后可以得到一个IP地址：
![](https://img-blog.csdn.net/20180617203659230?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这个就是我们要绑定在A记录上的地址（ping域名的时候地址可能会变，因为github动态的分配了3个ip进去，哪个都可以访问的）。
      CNAME地址，我们在记录里面写的是www，然后指向了我们的github域名（cname类型的解析为域名解析到域名）。这样我们的解析就算是配置完成了，直接访问我们的www的域名就可以访问到了。
