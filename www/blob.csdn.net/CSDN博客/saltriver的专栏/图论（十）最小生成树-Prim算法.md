
# 图论（十）最小生成树-Prim算法 - saltriver的专栏 - CSDN博客


2017年05月20日 11:14:01[saltriver](https://me.csdn.net/saltriver)阅读数：1421


前面说过，Prim算法是从顶点着手构建最小生成树的。应该说，Prim算法比Kruskal更简单。我们还是以前面的乡镇假设光纤网络为例：
![](https://img-blog.csdn.net/20170520105901948?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**Prim算法****工作步骤**
（1）**构建****全部顶点集****V****，选取****初始顶点****，****加入顶点集****U****。**
构建顶点集V={a,b,c,d,e,f,g,h,i}，从中选取任意一个顶点。我们假设从顶点a开始。将a加入到顶点集U={a}中。
（2）**找****U****中****顶****点与****V-U中顶点****的所有边。**
![](https://img-blog.csdn.net/20170520105934912?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
U中顶点只有a，V-U={b,c,d,e,f,g,h,i}，则U中顶点与V-U中顶点的所有边为a-b和a-f，图中紫色边。U中顶点为绿色背景顶点，V-U中顶点为黑色背景顶点。
（3）**选取所有****边中的最短边****加入****最小生成树。**
![](https://img-blog.csdn.net/20170520110000292?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
所有边a-b和a-f中的最短边a-b，将其加入最小生成树（图中绿色边）。
（4）**将****最短边****另一头****的顶点****，****加入****顶点****集合****U****。**
最短边a-b的另一头顶点为b，将b加入到U={a,b}。
![](https://img-blog.csdn.net/20170520110023819?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（5）**继续找****U****中****顶****点与****V-U中顶点****的所有边。**
此时，U={a,b}，V-U={c,d,e,f,g,h,i}，则U与V-U中顶点所有边为a-f， b-c， b-g，b-h。图中紫色边所示。
![](https://img-blog.csdn.net/20170520110045104?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（6）**继续****选取最短边****，****将最短边加入****最小****生成树****，****并****将****最短边另一头顶点加入****U****。**
此时最短边为a-f，将最短边加入最小生成树，并将最短边顶点f加入U={a,b,f}。此时V-U={c,d,e,g,h,i}。
![](https://img-blog.csdn.net/20170520110059292?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（7）**如此循环****反复，直至****U=V****。**
下面是按顺序的U和V-U以及最小生成树的变化过程：
![](https://img-blog.csdn.net/20170520110200042?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170520110231136?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
U={a,b,f,g};V-U={c,d,e,h,i}                                                   U={a,b,f,g,h};V-U={c,d,e,i}
![](https://img-blog.csdn.net/20170520110256073?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170520110325277?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
U={a,b,c,f,g,h};V-U={d,e,i}                                                    U={a,b,c,f,g,h,i};V-U={d,e}
![](https://img-blog.csdn.net/20170520110337809?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170520110536733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
U={a,b,c,e,f,g,h,i};V-U={d}                                                      U={a,b,c,d,e,f,g,h,i};V-U={}
**为什么不会****构成环**
因为我们在寻找边时，只是找U与V-U中顶点所构成的边，而U中内部顶点的边是不会找的。找到最短边后，直接将另一头顶点加入U了，也就是说这两个顶点都在U中了，即使这2个顶点还有其他路径，后面都不会再找，也就不会构成回路了。

