# ProE复杂曲线方程：Python Matplotlib 版本代码（L系统，吸引子和分形) - wishchinYang的专栏 - CSDN博客
2017年12月28日 10:00:10[wishchin](https://me.csdn.net/wishchin)阅读数：1038
       对生长自动机的研究由来已久，并在计算机科学等众多学科中，使用[元胞自动机](https://baike.baidu.com/item/%E5%85%83%E8%83%9E%E8%87%AA%E5%8A%A8%E6%9C%BA/7085754?fr=aladdin#4_3)的概念，用于生长模拟。而复杂花纹的生成，则可以通过重写一定的生长规则，使用生成式来模拟自然纹理。当然，很多纹理是由人本身设计的，其形成过程本身就是在人脑中进行“原胞生成”的过程。
        基础理论抄自于基础百科。
        来自于[百度百科](https://baike.baidu.com/item/L%E7%B3%BB%E7%BB%9F/2365448?fr=aladdin)：L－系统是匈牙利生物学家Aristid LinderMayer于1968年提出的。.L－系统的本质是一个**重写系统**，通过对植物对象生长过程的经验式概括和抽象，初始状态与描述规则，进行有限次迭代，生成字符发展序列以表现植物的拓扑结构，并对产生的字符串进行几何解释，就能生成非常复杂的分形图形。
## 详细介绍
        一类动态细胞自动机，在每一（时间）步上，其中的各个细胞可以由给定状态变为一个新的状态，或消亡或分裂为具有某种状态组合的细胞串。A.林顿梅伊尔曾用这种细胞自动机描述丝状有机体的发育过程，所以叫作**林顿梅伊尔系统**，简称**L系统**。
在乔姆斯基形式语言理论中，字母表被分成终结字母表和非终结字母表部分，“字”是由终结字母组成的字母序列。在L系统中，没有单独的终结字母表,所有生成的字都在系统语言中；初始字母可以被初始字所代替；被注视的字所包含的各个字母同时进行改写。每个字母代表一个细胞，用字表示细胞阵列发展的阶段。生成式对应于发展指令，这些指令的应用使有机体生长成已知类型。消亡的细胞可以用空字*e*表示。细胞之间可以有，也可以没有交互作用（信息传递）。有交互作用的有1L系统和2L系统。没有交互作用的叫作0L系统。
0L系统是一个三元组Γ=(*G*，*g*，*δ*),其中*G*为一个有限非空集合，叫作字母表；*g*为*G*中元素的非空序列，即非空字；*δ*为一个(转移)函数，首先取作从*G*到*G**（*G*中元素所能构成的一切序列的集合）的有限非空子集的映射。然后,把*δ*扩充为从*G**到*G**的有限非空子集的映射。
如果空字*e*不能替换任何字母，即对 *G*中所有字母*ɑ*，都有e唘*δ*(*ɑ*)，就称Γ为增殖0L系统，简称P0L系统；如果对字母表内每一个字母有且只有一个转移规则，即对*G*中所有*ɑ*，在*G**中只有一个字*p*使*δ*(*ɑ*)={*p*}，就称Γ为确定的0L系统，简称 D0L系统。显然（P0L∪D0L）吇0L。而既增殖又确定的DL系统称为DP0L。
## 分别描述
       从另一角度，元胞自动机可视为动力系统，因而可将初始点、轨道、[不动点](https://baike.baidu.com/item/%E4%B8%8D%E5%8A%A8%E7%82%B9)、周期轨和终极轨等一系列概念用到元胞自动机的研究中，上述分类，又可以分别描述为（[谭跃进](https://baike.baidu.com/item/%E8%B0%AD%E8%B7%83%E8%BF%9B)，1996；[谢惠民](https://baike.baidu.com/item/%E8%B0%A2%E6%83%A0%E6%B0%91)，1994；李才伟、1997）；
      ⑴均匀状态，即点态[吸引子](https://baike.baidu.com/item/%E5%90%B8%E5%BC%95%E5%AD%90)，或称不动点；
      ⑵简单的周期结构，即周期性吸引子，或称周期轨；
      ⑶混沌的非周期性模式，即混沌吸引子；
      ⑷这第四类行为可以与[生命系统](https://baike.baidu.com/item/%E7%94%9F%E5%91%BD%E7%B3%BB%E7%BB%9F)等复杂系统中的[自组织现象](https://baike.baidu.com/item/%E8%87%AA%E7%BB%84%E7%BB%87%E7%8E%B0%E8%B1%A1)相比拟，但在[连续系统](https://baike.baidu.com/item/%E8%BF%9E%E7%BB%AD%E7%B3%BB%E7%BB%9F)中没有相对应的模式。但从研究元胞自动机的角度讲，最具研究价值的具有第四类行为的元胞自动机，因为这类元胞自动机被认为具有"突现计算"(Emergent Computation）功能，研究表明，可以用作广义计算机（Universal Computer）以[仿真](https://baike.baidu.com/item/%E4%BB%BF%E7%9C%9F)任意复杂的计算过程。另外，此类元胞自动机在发展过程中还表现出很强的不可逆（lrreversibility）特征，而且，这种元胞自动机在若干有限循环后，有可能会 "死"掉，即所有元胞的状态变为零。
Python代码：
提前升级matplotlib：
       为避免环境问题，切换至python3
或不担心环境问题，或者在python2中
```bash
pip install --upgrade matplotlib
```
     谨慎操作，容易出现问题
否则有可能找不到mpl_toolkits组件
L系统
```python
def mainex():
    #drawsemilogx();
    #drawLorenzAttractor();
    #drawLeaf();#未成功
    #drawLSystem();
    #drawIsoLine();
    #drawFourFlower();
    #drawFlowers();
    drawBranch();
    #drawStar();
    #drawPillar(); 
#为了画吸引子
def get_lines(rule):
    d = rule['direct']
    a = rule['angle']
    p = (0.0, 0.0)
    l = 1.0
    lines = []
    stack = []
    info = rule['S']
    for i in range(rule['iter']):
        ninfo = []
        for c in info:
            if c in rule:
                ninfo.append(rule[c])
            else:
                ninfo.append(c)
        info = "".join(ninfo)
        
    for c in info:
        if c in "Ff":
            r = d * pi / 180
            t = p[0] + l*cos(r), p[1] + l*sin(r)
            lines.append(((p[0], p[1]), (t[0], t[1])))
            p = t
        elif c == "+":
            d += a
        elif c == "-":
            d -= a
        elif c == "[":
            stack.append((p,d))
        elif c == "]":
            p, d = stack[-1]
            del stack[-1]
    return lines
def draw(ax, rule, iter=None):
    if iter!=None:
        rule["iter"] = iter
    lines = get_lines( rule )
    linecollections = collections.LineCollection(lines)
    ax.add_collection(linecollections, autolim=True)
    ax.axis("equal")
    ax.set_axis_off()
    ax.set_xlim(ax.dataLim.xmin, ax.dataLim.xmax)
    ax.invert_yaxis()
    
def drawBranch():
    fig = plt.figure(figsize=(8,6))
    fig.patch.set_facecolor("w")
    ax = fig.add_subplot(111);
    ruleBranch={
        "X":"F-[[X]+X]+F[+FX]-X", "F":"FF", "S":"X",
        "direct":-45,
        "angle":25,
        "iter":4,
        "title":"Plant"
    }
    draw(ax, ruleBranch);
    fig.subplots_adjust(left=0,right=1,bottom=0,top=1,wspace=0,hspace=0)
    plt.show();
```
洛伦兹吸引子：
```python
def drawLorenzAttractor():
    xs, ys, zs = [], [], [];
    def mkPoints():  
        a, b, c = 10.0, 28.0, 8.0 / 3.0  
        h = 0.01  
        x0, y0, z0 = 0.1, 0, 0  
        for i in xrange(10000):  
            x1 = x0 + h * a * (y0 - x0)  
            y1 = y0 + h * (x0 * (b - z0) - y0)  
            z1 = z0 + h * (x0 * y0 - c * z0)  
            x0, y0, z0 = x1, y1, z1  
            xs.append(x0)  
            ys.append(y0)  
            zs.append(z0);
            
    mpl.rcParams["legend.fontsize"] = 10;
    fig = plt.figure()  
    ax = Axes3D(fig)  
  
    mkPoints();
    
    ax.plot(xs, ys, zs, label = "Lorenz's strange attractor")  
    ax.legend()  
    plt.show()
```
显示结果：
![](https://img-blog.csdn.net/20171228095657476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171228095703082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
