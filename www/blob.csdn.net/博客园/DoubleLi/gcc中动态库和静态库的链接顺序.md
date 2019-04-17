# gcc中动态库和静态库的链接顺序 - DoubleLi - 博客园






so文件：动态库
a文件： 静态库
exe文件：可执行程序（linux下以文件属性来标示是否是可执行文件，与后缀名无关）

经过自己写的一些测试程序，大致了解了下gcc中链接顺序问题，总结出以下几点：
1，动态库中可以包含另一个静态库，通过参数 -lxxx 把静态库libxxx.a加入so文件中，这样so文件中
   就包含了libxxx.a的所有实现。当然，如果不包含libxxx.a也没有问题，这样生成的so会小一点。
   如果不包含libxxx.a，最终使用这个so的可执行文件，在其生成时必须加入 -lxxx。
2, 2个so文件可以包含同一个静态库libxxx.a，最终生成exe文件时，不会产生冲突。更广泛的说，生成
   exe文件时候，可以链接多个so文件和a文件，如果其中的a文件有多份实现，最终只会有一份生效，其他
   都会被忽略。不用担心冲突。
3，当生成exe时候，当a文件有多份实现时，最左边指定的a文件才生效。

具体例子来说：
     libstatic.a ：   一个静态库文件
     libdynamic1.so:需要使用libstatic.a中的函数，但是没有包含libstatic.a
     libdynamic2.so:需要使用libstatic.a中的函数，包含libstatic.a
     libdynamic3.so:需要使用libstatic.a中的函数，也包含libstatic.a
     test.exe:最终的生成的可执行文件（linux对后缀没有要求，为了说明文件，姑且用exe后缀来表示可执行文件）

     gcc -o test.exe -ldynamic1 :错误，libdynamic1.so中没有包含libstatic.a，找不到libstatic.a的实现。

     gcc -o test.exe -ldynamic1 -lstatic:正确，so中没有，但是指定了libstatic.a，可以编译过

     gcc -o test.exe -ldynamic2 :正确，libdynamic2.so中有libstatic.a的实现。

     gcc -o test.exe -ldynamic1 -ldynamic2:正确，libdynamic1.so中没有libstatic.a，但是libdynamic2.so中有。

     gcc -o test.exe -ldynamic2 -ldynamic3:正确，虽然libdynamic2.so和libdynamic3.so都含有静态库，但是不会冲突，
                                           最终只会有一份存在，并且是libdynamic2.so中的静态库有效。

     gcc -o test.exe -ldynamic2 -ldynamic3 -lstatic:同样正确，最终只会有一份存在，并且是libdynamic2.so中的静态库有效。

     gcc中库的链接顺序是从右往左进行，所以要把最基础实现的库放在最后，这样左边的lib就可以调用右边的lib中的代码。同时，当一个函数的实现代码在多个lib都存在时，最左边的lib代码最后link，所以也将最终保存下来。









