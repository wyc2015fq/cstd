# 使用latex的subfigure并排放置两张图 - 家家的专栏 - CSDN博客





2013年09月02日 17:02:01[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：30804








注意要使用宏包\usepackage{graphicx} 和\usepackage{subfigure}



> 
\begin{figure} \centering   

\subfigure[figure 1 title.] { \label{fig:a}    

\includegraphics[width=0.8\columnwidth]{fig1.eps} 

}    

\subfigure[figure 2 title.] { \label{fig:b}    

\includegraphics[width=0.8\columnwidth]{fig2.eps}    

}    

\caption{ general title. }    

\label{fig}    

\end{figure}







找了好久，都没找到一段可用的code。。。

这段可用，运行不过，可以留言，通知我。






