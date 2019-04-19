# intellij idea配置网络代理 - 左直拳的马桶_日用桶 - CSDN博客
2018年07月13日 20:14:04[左直拳](https://me.csdn.net/leftfist)阅读数：6911
背景：intellij idea所在的这台电脑本身上不了网，要通过代理上网。
使用代理上网，以往都是在IE的工具-Internet选项-连接里进行设置就OK了，谷歌浏览器啥的就可以上网了。但intellij idea不行，gradle不行，提示什么
`Unknown host 'services.gradle.org'. You may need to adjust the proxy settings in Gradle.`
；然后想在Intellij idea里安装个FindBugs插件，报错说下载不了，这才知道是网络问题。
那么intellij idea怎么设置代理上网呢？
入口在插件这里：File-settings-plugins 
![这里写图片描述](https://img-blog.csdn.net/20180713201308354?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018071320132772?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
根据实际情况填写就可以了。
直接在setting里设置都可以了 
![这里写图片描述](https://img-blog.csdn.net/20180717110418888?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
