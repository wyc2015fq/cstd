# Latex中IEEEtran中参考文献中斜体问题 - 家家的专栏 - CSDN博客





2013年04月22日 09:10:47[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：4213








这个一直困扰我两天的问题，终于解决了~

必须要记录下来~



罗嗦一下吧~

在IEEEtran模版中，进行参考文献编辑的过程中，不知道为什么~

参考文献期刊名字应该为**斜体**，但是我编辑出来的确实带有**下划线非斜体**~

后来发现

\bibliographystyle{IEEEtran}

\bibliography{IEEEabrv}

和

**\usepackage{ulem}**

有冲突~

注释掉 \usepackage{ulem} 就可以正常显示斜体了~



前提要在tex目录下有IEEEtran.cls文件，本文件定义了参考文献的样式~

 而且参考文献会自动按照引用顺序进行排序~



罗嗦了这么多，不知道说清楚了没有~



