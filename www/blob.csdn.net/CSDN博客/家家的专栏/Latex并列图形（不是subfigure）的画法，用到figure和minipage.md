# Latex并列图形（不是subfigure）的画法，用到figure和minipage - 家家的专栏 - CSDN博客





2012年10月25日 10:19:05[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：4704
个人分类：[数学 Matlab 基本知识](https://blog.csdn.net/yihaizhiyan/article/category/749753)










- \begin{figure}   
-   \begin{minipage}[t]{0.5\linewidth} % 如果一行放2个图，用0.5，如果3个图，用0.33  
-     \centering   
-     \includegraphics[width=1in]{graphic.eps}   
-     \caption{Small Box}   
-     \label{fig:side:a}   
-   \end{minipage}%   
-   \begin{minipage}[t]{0.5\linewidth}   
-     \centering   
-     \includegraphics[width=1.5in]{graphic.eps}   
-     \caption{Big Box}   
-     \label{fig:side:b}   
-   \end{minipage}   
- \end{figure} 

如果是双栏排版的话，则使用
- \begin{figure*}
- \end{figure*}   

 转自：[http://blog.csdn.net/lovelytigerphd/article/details/7206552](http://blog.csdn.net/lovelytigerphd/article/details/7206552)



