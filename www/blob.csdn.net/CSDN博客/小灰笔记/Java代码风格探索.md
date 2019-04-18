# Java代码风格探索 - 小灰笔记 - CSDN博客





2017年01月12日 23:21:41[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：397








刚刚开始学习Java，对这门语言还没有什么深入的了解。跟着教程学习了几天，还处于hell world的阶段。不过也没什么，我本身就是喜欢这种按部就班、稳扎稳打的方式，何况我计划学习Java学习的深入一些。

看了不同的教程，包括纸质的以及慕课网网站上的视频，看到的代码风格基本上都是像下面这种：

  1 publicclass HelloWorld{

  2 publicstaticvoid main(String[] args){

  3                 String i_love_mooc =
"我爱慕课网";

  4                 String mooc_web    =
"www.imooc.com";

  5                 System.out.println(i_love_mooc);

  6                 System.out.println(mooc_web);

  7         }

  8 }




我知道这种代码风格有助于写出紧凑形式的代码，这种风格最初接触到是在Perl，但是后来还是没有坚持使用这种方式。因为总觉得对于我来说这种方式阅读代码的时候有一点不习惯。后来浏览了一下Go语言，发现不仅是这种风格的代码，而且对这种风格进行了强制。我有学习了解Go语言的想法，但是直到现在我还是不是很想切换这种风格。

上面这段代码我是编译执行过的，对代码做了如下修改以满足我自己喜欢的口味：

  1 publicclass HelloWorld

  2 {       

  3 publicstaticvoid main(String[] args)

  4         {       

  5                 String i_love_mooc =
"我爱慕课网";

  6                 String mooc_web    =
"www.imooc.com";

  7                 System.out.println(i_love_mooc);

  8                 System.out.println(mooc_web);

  9         }

 10 }




当然，这并没有什么革新式的修改，只是能够让我感觉代码的层次结构更为清晰。简单进行编译运行，全都没有问题：

GreydeMac-mini:Java greyzhang$ vim HelloWorld.java 

GreydeMac-mini:Java greyzhang$ javac HelloWorld.java 

GreydeMac-mini:Java greyzhang$ java HelloWorld

我爱慕课网

www.imooc.com

GreydeMac-mini:Java greyzhang$ 




既然如此，还是遵循我自己的习惯吧！至于变量名的定义，我也不想做什么解释了。尽管匈牙利或者驼峰等变量名命名方式有一定的优势，能够把变量名字缩短，让代码屏显不至于过宽灯，但是我觉得可读性还是应该占在第一位的。况且，出于手指敲击键盘的灵活性考虑，我也不是很想频繁使用组合键。由此，还是使用unix-like的方式吧！只要是代码读起来顺畅，什么都好说。同样的逻辑，最终成为可执行文件或者字节码的时候大概是没有什么差异了吧！既然如此，这方面何必纠结！



