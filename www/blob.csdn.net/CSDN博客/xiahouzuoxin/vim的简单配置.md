# vim的简单配置 - xiahouzuoxin - CSDN博客





2012年09月07日 14:25:47[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2061标签：[vim																[cmd](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=vim&t=blog)
个人分类：[“磨刀不误砍材工”](https://blog.csdn.net/xiahouzuoxin/article/category/1510855)








注：这里的vim使用技巧既可用于Linux下的vim，也能用在Windows下的gvim中，




1、  打开_vimrc文件（默认在gvim的安装目录下），在文件后面添加

syntax enable

syntax on

set nu

colo torte

2、  从[http://sourceforge.net/projects/ctags/files/ctags/5.8/ctags58.zip/download下载ctags.zip](http://sourceforge.net/projects/ctags/files/ctags/5.8/ctags58.zip/download%E4%B8%8B%E8%BD%BDctags.zip)。完成后解压，在解压后的文件下有一个ctags.exe文件，将其拷贝到gvim.exe相同目录下。若不拷贝也可，但需要为ctags.exe添加环境变量。

使用方法，

在工程目录下使用命令ctags –R

在源码需要跳转的地方Ctrl+]

在源码需要跳回的地方Ctrl+T

3、  从[http://www.vim.org/scripts/script.php?script_id=273](http://www.vim.org/scripts/script.php?script_id=273)下载taglist.zip。解压，

将/doc/taglist.txt拷贝到vim安装目录下/vimfiles/doc/下

将/plugin/taglist.vim拷贝到vim安装目录下/vimfiles/pulgin/下

运行cmd或cgwin，进入/vimfiles/doc/目录下，打开gvim.exe，在命令行模式下运行

:helptags.                 注：注意后面的点别忘了                                                                  

   到此taglist安装完成。

   使用方法：在使用vim打开的源文件中，运行命令Tlist即可

                        要在Tlist和Content区域切换，连按Ctrl+w,Ctrl+w两次

4、  自动补全功能实现

到[http://www.vim.org/scripts/script.php?script_id=1520](http://www.vim.org/scripts/script.php?script_id=1520)下载omnicppcomplete-0.41.zip。

解压后有3个文件夹，原样将3个文件夹拷贝到gvim的安装路径的vimfiles文件夹下，

在.vimrc中添加


- set nocp    
- filetype plugin on
  



到此算安装完成了。

但是，要使用自动提示功能，需要在使用ctags -R时添加选项，如下，一个选项都不能少：

ctags -R --c++-kinds=+p --fields=+iaS --extra=+q [project root]


[project root]指工程根目录，如果是当前目录则可以省略






5、  c.vim增强注释功能

到[http://www.vim.org/scripts/script.php?script_id=213](http://www.vim.org/scripts/script.php?script_id=213)下载cvim安装插件，

解压后拷贝到/vimfiles目录下即可。如果没有在.vimrc中添加filetype plugin on则添加。




（1）c.vim功能描述如下

- Adding file header
- Adding function/frame comment
- Including default code snippet
- Performing syntax check
- Reading documentation about a function
- Converting a code block to comment, and vice versa  


（2）c.vim使用——在normal模式下执行，不是在命令行中

打开新文件时自动添加文件头描述信息

\if     : 自动添加函数

\im   : 自动添加主函数(main)

\cfu  : 自动添加函数头注释信息

\cfr   : 添加注释框架

\ct    : 添加日期(Date)和时间(Time)

\cd   : 添加日期(Date)

\cc   : 注释

\co   :取消注释

\nr     : 添加自定义代码或框架

---------------------附:详细的c.vim hotkey-------------------------

-- Help ---------------------------------------------------------------

  \hm       show manual for word under the cursor (n,i)

  \hp       show plugin help                      (n,i)

  -- Comments -----------------------------------------------------------

  \cl       end-of-line comment                 (n,v,i)

  \cj       adjust end-of-line comment(s)       (n,v,i)

  \cs       set end-of-line comment column      (n)

  \c* comment /">       code -> comment /* */               (n,v)

  \cc       code -> comment //                  (n,v)

  \co       comment -> code                     (n,v)

  \cfr      frame comment                       (n,i)

  \cfu      function comment                    (n,i)

  \cme      method description                  (n,i)

  \ccl      class description                   (n,i)

  \cd       date                                (n,v,i)

  \ct       date \& time                        (n,v,i)

  -- Statements ---------------------------------------------------------

  \sd       do { } while                        (n,v,i)

  \sf       for                                 (n,i)

  \sfo      for { }                             (n,v,i)

  \si       if                                  (n,i)

  \sif      if { }                              (n,v,i)

  \sie      if else                             (n,v,i)

  \sife     if { } else { }                     (n,v,i)

  \sw       while                               (n,i)

  \swh      while { }                           (n,v,i)

  \ss       switch                              (n,v,i)

  \sc       case                                (n,i)

  \s{       { }                                 (n,v,i)

  -- Preprocessor -------------------------------------------------------

  \p                         (n,i)

  \p"       #include ""                         (n,i)

  \pd       #define                             (n,i)

  \pu       #undef                              (n,i)

  \pie      #if  #else #endif                   (n,v,i)

  \pid      #ifdef #else #endif                 (n,v,i)

  \pin      #ifndef #else #endif                (n,v,i)

  \pind     #ifndef #def #endif                 (n,v,i)

  \pi0      #if 0 #endif                        (n,v,i)

  \pr0      remove #if 0 #endif                 (n,i)

  \pe       #error                              (n,i)

  \pl       #line                               (n,i)

  \pp       #pragma                             (n,i)

  -- Idioms -------------------------------------------------------------

  \if       function                            (n,v,i)

  \isf      static function                     (n,v,i)

  \im       main()                              (n,v,i)

  \i0       for( x=0; x=0; x-=1 )            (n,v,i)

  \ie       enum   + typedef                    (n,i)

  \is       struct + typedef                    (n,i)

  \iu       union  + typedef                    (n,i)

  \ip       printf()                            (n,i)

  \isc      scanf()                             (n,i)

  \ica      p=calloc()                          (n,i)

  \ima      p=malloc()                          (n,i)

  \isi      sizeof()                            (n,v,i)

  \ias      assert()                            (n,v)

  \ii       open input file                     (n,i)

  \io       open output file                    (n,i)

  -- Snippets -----------------------------------------------------------

  \nr       read code snippet                   (n,i)

  \nw       write code snippet                  (n,v,i)

  \ne       edit code snippet                   (n,i)

  \np       pick up prototype                   (n,v,i)

  \ni       insert prototype(s)                 (n,i)

  \nc       clear  prototype(s)                 (n,i)

  \ns       show   prototype(s)                 (n,i)

  \ntl      edit local templates                (n,i)

  \ntg      edit global templates               (n,i)

  \ntr      rebuild templates                   (n,i)





6、VIM操作补充

（1）进入视图（Visual）模式：V，该模式下可以通过hjkl实现多行选中

（2）多行注释：进入多行选中模式选中多行

                           \cc:注释         \co:取消注释

（3）多行Tab缩进：进入多行选中模式选中多行

                           >:右缩进一个Tab        <:左移一个Tab

（4）局部变量跳转：gd


（6）查看OpenCV源代码（V2.3.1）智能提示：

到[OpenCV源代码目录]/modules/目录下，执行ctags -R --c++-kinds+=p --fields=+iaS --extra=+q生成tags文件

打开/etc/vimrc文件，添加set tags+=[OpenCV源代码目录]/modules/tags即可












