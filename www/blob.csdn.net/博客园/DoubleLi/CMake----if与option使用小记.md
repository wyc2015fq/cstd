# CMake----if与option使用小记 - DoubleLi - 博客园






在CMake中if语法比较简单，if后面括号中的参数随着CMake版本的推进，在else和endif中也可以不用写了。

```
if(address)
else()
endif()
```

    对于if语法，比较常用的就是字符串比较了，这里个人简单用到过两种，一种是这个变量有没有定义过。有两种写法可以验证

```
if(DEFINED address)
else()
endif()
```

```
if(address)
else()
endif()
```

两种都可以验证这个变量有没有定义过，注意：是定义过，比如你在CMake命令行中随便写了-Daddress=xxx,就表示定义过了，对里面的值没有任何要求。



    第二中常用的用法就是字符串比较，比如

```
if(${address} STREQUAL "ON")
else()
endif()
```

当然去掉大括号也是可以比较的

```
if(address STREQUAL "ON")
else()
endif()
```



这种就是比较字符串的值了，注意：在这个时候，你的脚本已经假设你对于address已经有了**已被定义的默认要求**，也就是说此时如果你没有定义过address变量，在正常处理cmake ..的时候会报错，会提示无法识别的参数“STREQUAL”和"ON",这个需要特别注意。还有就是如果真的要加上大括号来取值，适当加上双引号可以防止出现一些不必要的麻烦。

    有的同学会问了，如果我就想有个默认值呢？如果我就想，当我自己不去设这个，我希望它存在，被定义过，并且有个自己的默认值，有没有办法？

    答案当然是有的，强大的CMake为我们准备了--option这个命令，给我们作为默认初始值并且作为定义值的候选。

```
option(address "This is a option for address" ON)
```

此时表示，如果用户没有定义过address,那我address的默认值就是ON,如果用户在命令行显示改变过address的值比如为OFF,那么在脚本中address的值就是OFF。

有一个需要注意，就是对于那些依赖于address有没有定义过的时候，如果你用了option你要特别注意，就是在脚本处理过程中，如果你没有在命令行中显示定义过address，那么在option之前的语句对于address都是没有定义过，直到到达了option,此时才真正定义了address.说起来不好理解，给个例子

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

if(address)
    message("defined address!!!!!!!!!!")
else()
    message("NOT defined address!!!!!!!!!")
endif()

option(address "hello world" ON)
message("option is ${address}")

if(address)
    message("defined address!!!!!!!!!!")
else()
    message("NOT defined address!!!!!!!!!")
endif()

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

输出结果为

```
NOT defined address!!!!!!!!!!
option is a
defined address!!!!!!!!!!
```

这样就比较好理解了。

    配合option和if一起使用，可以灵活多变的针对编译环境做各种定制化处理，包括项目中宏的各种替换和使用等等。这里我给大家开了个简单的头，希望这篇文章能够给你带来哪怕只有一点点的帮助就够了  : )









