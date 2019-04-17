# ana3+opencv+TensorFlow+NVIDIAGPU 安装 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





http://blog.csdn.net/qq_30611601/article/details/79067982

这个博客写的挺完整的

当你发现你的anna下载的贼鸡儿的慢，你就需要使用清华的镜像网站！

在ana prompt 中逐个输入以下三段语句

```
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main/
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --set show_channel_urls yes
```

你会发现，速度飞起~

最后安装opencv

```
下载opencv

conda install -c https://conda.binstar.org/menpo opencv3
```

真的舒服啊。













