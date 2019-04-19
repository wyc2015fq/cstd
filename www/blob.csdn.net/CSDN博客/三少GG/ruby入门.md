# ruby入门 - 三少GG - CSDN博客
2017年05月01日 20:03:25[三少GG](https://me.csdn.net/scut1135)阅读数：916
Ubuntu 通过 PPA 安装 Ruby
最佳方法是通过 PPA 安装：
```
**sudo apt-add-repository ppa:brightbox/ruby-ngsudo apt-get updatesudo apt-get install ruby2.2 ruby2.2-devruby -v**
```
PPA 的优点是可以用 apt 统一更新，不用处理 PATH 等问题。
