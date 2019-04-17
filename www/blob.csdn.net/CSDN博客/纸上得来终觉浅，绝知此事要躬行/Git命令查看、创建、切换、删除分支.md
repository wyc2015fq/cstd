# Git命令查看、创建、切换、删除分支 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年12月07日 22:26:44[boonya](https://me.csdn.net/boonya)阅读数：11301标签：[git																[branch																[ide](https://so.csdn.net/so/search/s.do?q=ide&t=blog)
个人分类：[Git/GitHub/SVN/TFS](https://blog.csdn.net/boonya/article/category/6068199)





对于有些IDE检出代码很困难，所以不得已的情况下使用git命令是最高效快捷的。熟悉了解Git基本的命令操作是很有必要的，这篇文章主要讲对代码分支的操作。


## 1. 查看远程分支



```
$ git branch -a  
      
    　　* br-2.1.2.2  
      
    　　master  
      
    　　remotes/origin/HEAD -> origin/master  
      
    　　remotes/origin/br-2.1.2.1  
      
    　　remotes/origin/br-2.1.2.2  
      
    　　remotes/origin/br-2.1.3  
      
    　　remotes/origin/master
```

## 2. 查看本地分支



```
$ git branch  
      
    　　* br-2.1.2.2  
      
    　　master
```


## 3. 创建分支



```
$ git branch test  
  
$ git branch  
  
* br-2.1.2.2  
  
master  
  
test
```


## 4. 切换分支

切换分支到test



```
$ git branch  
  
* br-2.1.2.2  
  
master  
  
test  
  
shuohailhl@SHUOHAILHL-PC /f/ggg/jingwei (br-2.1.2.2)  
  
$ git checkout test  
  
M jingwei-server/src/main/java/com/taobao/jingwei/server/service/cmd/GetCustomerTarCmd.java  
  
M jingwei-server/src/main/java/com/taobao/jingwei/server/util/ServerUtil.java  
  
Switched to branch 'test'  
  
shuohailhl@SHUOHAILHL-PC /f/ggg/jingwei (test)  
  
$ git branch  
  
br-2.1.2.2  
  
master  
  
* test
```


## 5. 删除本地分支



```
$ git checkout br-2.1.2.2  
      
    M jingwei-server/src/main/java/com/taobao/jingwei/server/service/cmd/GetCustomerTarCmd.java  
      
    M jingwei-server/src/main/java/com/taobao/jingwei/server/util/ServerUtil.java  
      
    Switched to branch 'br-2.1.2.2'  
      
    shuohailhl@SHUOHAILHL-PC /f/ggg/jingwei (br-2.1.2.2)  
      
    $ git br  
      
    * br-2.1.2.2  
      
    master  
      
    test  
      
    shuohailhl@SHUOHAILHL-PC /f/ggg/jingwei (br-2.1.2.2)  
      
    $ git br -d test  
      
    Deleted branch test (was 17d28d9).  
      
    shuohailhl@SHUOHAILHL-PC /f/ggg/jingwei (br-2.1.2.2)  
      
    $ git br  
      
    * br-2.1.2.2  
      
    master
```


## 6. 查看本地和远程分支  -a

前面带*号的代表你当前工作目录所处的分支



```
$ git branch -a  
  
br-2.1.2.2  
  
master  
  
* test  
  
remotes/origin/HEAD -> origin/master  
  
remotes/origin/br-2.1.2.1  
  
remotes/origin/br-2.1.2.2  
  
remotes/origin/br-2.1.3  
  
remotes/origin/master
```](https://so.csdn.net/so/search/s.do?q=branch&t=blog)](https://so.csdn.net/so/search/s.do?q=git&t=blog)




