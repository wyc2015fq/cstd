# HTML Meta信息的优先级 - =朝晖= - 博客园
# [HTML Meta信息的优先级](https://www.cnblogs.com/dhcn/p/7116216.html)
    一般来讲meta的信息都是不同维度的不会有冲突，不过下面两个有一定冲突：
```
<meta name="renderer" content="webkit">  
<meta http-equiv="X-UA-Compatible" content="IE=edge">
```
    第一行头信息是使360这类双核浏览器使用chrome内核，第二行是使IE浏览器使用比较新版的内核。如果两行倒过来，就会使360浏览器使用IE内核。可见Meta头信息的优先级是从前往后的。

