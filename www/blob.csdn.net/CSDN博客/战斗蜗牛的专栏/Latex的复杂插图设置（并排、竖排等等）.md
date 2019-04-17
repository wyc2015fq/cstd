# Latex的复杂插图设置（并排、竖排等等） - 战斗蜗牛的专栏 - CSDN博客





2017年07月23日 16:32:58[vbskj](https://me.csdn.net/vbskj)阅读数：4269








http://blog.sciencenet.cn/blog-561346-747243.html







latex图片并排

1.并排摆放，共享标题

\begin{figure}[htbp]

\centering

\includegraphics{left}

\includegraphics{right}

\caption{abcd}

\end{figure}




2.并排摆放，各有标题

\begin{figure}[htbp]

\centering

\begin{minipage}[t]{0.3\textwidth}

\centering

\includegraphics{left}

\caption{ab}

\end{minipage}

\begin{minipage}[t]{0.3\textwidth}

\centering

\includegraphics{right}

\caption{cd}

\end{minipage}

\end{figure}




3.并排摆放，共享标题，各有子标题

如果想要两幅并排的图片共享一个标题，并各有自己的子标题，可以使用subfig宏包提供的\subfloat命令。subfloat命令缺少宽度参数。虽然我们可以用\hspace命令调整子图的距离，子标题却只能和子图本身一样宽，就会出现折行。

为了避免子标题折行，我们可以在\subfloat里再嵌套个minipage，因为后者是有宽度的。

\begin{figure}[htbp]

\centering




\subfloat[ab]{

\label{fig:improved_subfig_a}

\begin{minipage}[t]{0.3\textwidth}

\centering

\includegraphics{left}

\end{minipage}

}




\subfloat[cd]{

\label{fig:improved_subfig_b}

\begin{minipage}[t]{0.3\textwidth}

\centering

\includegraphics{right}

\end{minipage}

}




\caption{abcd}

\end{figure}




使用latex的subfigure竖排图形

注意要使用宏包\usepackage{graphicx}和\usepackage{subfigure}，IEEE给的模板是图形横排，subfigure不支持\\换行，有人用minipage放在subfigure{}中，在minipage{}里换行

\begin{figure}

\centering




\subfigure[the first subfigure]{

\begin{minipage}[b]{0.2\textwidth}

\includegraphics[width=1\textwidth]{fig1.eps} \\

\includegraphics[width=1\textwidth]{fig2.eps}

\end{minipage}

}




\subfigure[the second subfigure]{

\begin{minipage}[b]{0.2\textwidth}

\includegraphics[width=1\textwidth]{fig3.eps} \\

\includegraphics[width=1\textwidth]{fig4.eps}

\end{minipage}

}

\end{figure}




以上显示在单列。如果希望跨列显示，可以用

\begin{figure*}...\end{figure*}\begin{figure*}...\end{figure*}

，并且修改相应的宽度参数。







Latex插入图片相关问题


/usepackage{subfigure}




/begin{figure}[H]

/centering

/subfigure[SubfigureCaption]{

/label{Fig.sub.1}

/includegraphics[width=0.4/textwidth]{figurename.eps}}

/subfigure[SubfigureCaption]{

/label{Fig.sub.2}

/includegraphics[width=0.4/textwidth]{figurename.eps}}

/caption{MainfigureCaption}

/label{Fig.lable}

/end{figure}


Latex子图排放


首先使用宏包\usepackage{graphicx}, \usepackage{subfigure}




（1）竖排图形

subfigure不支持\\换行，可以用minipage放在subfigure{}中，在minipage{}里换




\begin{figure}

\centering




\subfigure[the first subfigure]{

\begin{minipage}[b]{0.2\textwidth}

\includegraphics[width=1\textwidth]{fig1.eps} \\

\includegraphics[width=1\textwidth]{fig2.eps}

\end{minipage}

}




\subfigure[the second subfigure]{

\begin{minipage}[b]{0.2\textwidth}

\includegraphics[width=1\textwidth]{fig3.eps} \\

\includegraphics[width=1\textwidth]{fig4.eps}

\end{minipage}

}

\end{figure}


（2）横排子图

使用latex的subfigure排版并排放置的两张图.

\begin{figure}




\begin{minipage}[t]{0.5\linewidth}

\centering

\includegraphics[width=2.2in]{fig1.eps}

\caption{fig1}

\label{fig:side:a}

\end{minipage}%




\begin{minipage}[t]{0.5\linewidth}

\centering

\includegraphics[width=2.2in]{fig2.eps}

\caption{fig2}

\label{fig:side:b}

\end{minipage}

\end{figure}




还有一种方法: 在某些情况下，有时会希望将并列的图形组成一组，而其中的每一幅图都保持其独立性。

\begin{figure}

\centering




\subfigure[Small Box with a Long Caption]{

\label{fig:subfig:a} %% label for first subfigure

\includegraphics[width=1.0in]{graphic.eps}}

\hspace{1in}




\subfigure[Big Box]{

\label{fig:subfig:b} %% label for second subfigure

\includegraphics[width=1.5in]{graphic.eps}}

\caption{Two Subfigures}

\label{fig:subfig} %% label for entire figure

\end{figure}


关于Visio转eps

很多人习惯用Visio画图（包括me），但是Visio却不能导出EPS格式，所以只好借助于第三方软件。通过Visio另存为*.emf格式然后用openoffice的Draw打开，可以导出为EPS格式。导出时需要选中打开的图片，这样导出EPS之后就只包含*.emf图片的内容，不会有大片的空白。但是用这个方法导出的EPS图片会走样，比如直线经常会断断续续的，我在Visio中加入的“计算机”形状还会出现莫名其妙的阴影。也可能是我没有设置好吧。

还有另外一种更好的解决方案，导出的EPS文件十分完美。也就是通过acrobat打印机将Visio图形打印成pdf，再经过gsview（GSview4.8）裁减。具体过程如下：

1 编辑图片文件，保存为pdf文件

安装acrobat,，在Visio和Wod下把编辑好的图片打印为pdf文件即可

2 转换为eps文件

在acobat下打开pdf文件，选择另存为eps文件即可

3 对eps文件裁减

用GSView打开eps文件，选择File-->PS to EPS，选择自动裁减，存为另一个*.eps文件即可

minipage的使用




\begin{figure}

%\begin{tabular}{cc}




\begin{minipage}{0.48\linewidth}

\centerline{\includegraphics[width=100px]{image1.jpg}}

\centerline{(a) Result 1}

\end{minipage}

\hfill




\begin{minipage}{.48\linewidth}

\centerline{\includegraphics[width=100px]{image2.jpg}}

\centerline{(b) Results 2}

\end{minipage}




\vfill

\begin{minipage}{0.48\linewidth}

\centerline{\includegraphics[width=100px]{image3.jpg}}

\centerline{(c) Result 3}

\end{minipage}

\hfill




\begin{minipage}{0.48\linewidth}

\centerline{\includegraphics[width=100px]{image4.jpg}}

\centerline{(d) Result 4}

\end{minipage}




%\end{tabular}

\caption{Example of placing a figure with experimental results.}

\label{fig:res}

\end{figure}



