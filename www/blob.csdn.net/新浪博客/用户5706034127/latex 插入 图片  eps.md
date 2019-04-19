# latex 插入 图片  eps_用户5706034127_新浪博客
|||
原载于[http://blog.csdn.net/yanxiangtianji](http://blog.csdn.net/yanxiangtianji)
error：  unknown graphics extension .eps
这个时候，用
![latex <wbr>插入 <wbr>图片 <wbr> <wbr>eps](http://s8.sinaimg.cn/mw690/006e9W8vzy7bZYgdsSX97&690)
latex编译器，然后用div to pdf查看。
不要用pdf latex编译器即可。
问题表现为：
在Windows下使用pdflatex编译时，所有使用.eps文件的地方都会得到一条错误信息：!LaTex Error:Unknown
graphics extension:.eps。
不管你用的是\epsfig、\includegraphics还是其他命令，都会遇到这个问题。
原因：
pdflatex只能支持pdf、jpg、jpeg、png共4中格式的图片。
解决：
方法一（换编译链）：
使用latex编译不存在这个问题。
就是麻烦和耗时一点，latex直接编译完是ps，需要再用其他命令继续编译得到pdf。但是如果你的IDE可以配置自动的编译链就没有问题了。
方法二（依然使用pdflatex）：
可以使用epstopdf包将eps图片转换成pdf图片，需要注意epstopdf包必须需要配合graphicx包使用。
\usepackage[pdftex]{graphicx}
\DeclareGraphicsExtensions{.pdf,.jpeg,.png}
%    \usepackage[dvips]{graphicx}%    \DeclareGraphicsExtensions{.eps}\usepackage{epstopdf}
如果这样做之后出现“Pacakge pdftex.df Error: file'xxx-eps-converted-to.pdf'
not found”错误，那么需要在pdflatex命令后面加一个参数“-shell-escape”。
这样做比较快，因为过程简单嘛。
需要注意的是第一次编译的时候会对每一个eps图片生成一个对应pdf文件，这个过程回略耗时，但是只会发生一次（如果图片更改也会重新生成）。
PS：生成的pdf图片默认保存在eps的同一级目录下，假设eps图片名为“abc.eps”则对应的pdf文件名默认为“abc-eps-converted-to.pdf”。


