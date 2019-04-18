# SMO（Sequential minimal optimization）算法的详细实现过程 - 技术分享 - CSDN博客

2018年11月08日 19:41:44[丿一叶秋丶](https://me.csdn.net/zhuqiang9607)阅读数：575


SMO算法主要是为优化SVM（支持向量机）的求解而产生的，SVM的公式基本上都可以推到如下这步：

$max_{\alpha}\sum_{i=1}^{m}\alpha_{i}-\frac{1}{2}\sum_{i=1}^{m}\sum_{j=1}^{m}\alpha_{i}\alpha_{j}y_{i}y_{j}x_{i}^{T}x_{j}$

$s.t. \sum_{i}^{m}\alpha_{i}y_{i}=0$

$0≤\alpha_{i}≤C，i = 1, 2, 3,...,m$

其中，C是SVM中惩罚参数（或正则化常数），可令：

$\varphi(\alpha)=\sum_{i=1}^{m}\alpha_{i}-\frac{1}{2}\sum_{i=1}^{m}\sum_{j=1}^{m}\alpha_{i}\alpha_{j}y_{i}y_{j}x_{i}^{T}x_{j}$

#### SMO的具体步骤：

**第一步**：为了满足$\sum_{i}^{m}\alpha_{i}y_{i}=0$公式，首先要固定两个变量$\alpha_{i}和\alpha_{j}$，这里以$\alpha_{1}和\alpha_{2}$为例，其余的$\alpha_{i}(i=3,4,...,m)都是已知量$，则约束条件变成：

$\alpha_{1}y_{1}+\alpha_{2}y_{2}=c=-\sum_{i=3}^{m}\alpha_{i}y_{i}，(0≤\alpha_{1}≤C，0≤\alpha_{2}≤C)$

两边同乘$y_{1}$，并记$y_{1}y_{2}=h_{0}$得：

$\alpha_{1}+h_{0}\alpha_{2}=-y_{1}\sum_{i=3}^{m}\alpha_{i}y_{i}=\alpha_{1_{new}}+h_{0}\alpha_{2_{new}}$

令$H=-y_{1}\sum_{i=3}^{m}\alpha_{i}y_{i}$，可得：

$\alpha_{1_{new}}=H-h_{0}\alpha_{2_{new}}$  (1)

**第二步**：由于$\alpha_{1_{new}}$可以用$\alpha_{2_{new}}$来表示，且$\alpha_{i}(i=3,4,...,m)$都是已知量，此时$\varphi(\alpha)$只有一个未知变量$\alpha_{2_{new}}$，那么可以直接求导得到$\alpha_{2_{new}}$。具体实施过程如下：

**1**、展开$\varphi(\alpha)$可得：

$\varphi(\alpha)=\alpha_{1_{new}}+\alpha_{2_{new}}-\frac{1}{2}\alpha_{1_{new}}^{2}k_{11}-\frac{1}{2}\alpha_{2_{new}}^{2}k_{22}-\alpha_{1_{new}}\alpha_{2_{new}}y_{1}y_{2}k_{12}-\alpha_{1_{new}}y_{1}\sum_{i=3}^{m}\alpha_{i}y_{i}k_{i1}-\alpha_{2_{new}}y_{2}\sum_{i=3}^{m}\alpha_{i}y_{i}k_{i2}+\varphi_{constant}$   (2)

式中， $kij=k(x_{i},x_{j})$，表示核函数

$\varphi_{constant}=\sum_{i=3}^{m}\alpha_{i}-\frac{1}{2}\sum_{i=3}^{m}\sum_{j=3}^{m}\alpha_{i}\alpha_{j}y_{i}y_{j}k_{ij}$

**2**、SVM的超平面模型：$f(x_{j})=w^{T}+b=\sum_{i=1}^{m}\alpha_{i}y_{i}k_{ij}+b$

令$V_{j}=\sum_{i=3}^{m}\alpha_{i}y_{i}k_{ij}=f(x_{j})-b-\alpha_{1}y_{1}k_{1j}-\alpha_{2}y_{2}k_{2j}$  (3)

**3**、 将公式（1）、（3）代入（2）得：

$\varphi(\alpha)=H-h_{0}\alpha_{2_{new}}+\alpha_{2_{new}}-\frac{1}{2}(H-h_{0}\alpha_{2_{new}})^{2}k_{11}-\frac{1}{2}\alpha_{2_{new}}^{2}k_{22}-(H-h_{0}\alpha_{2_{new}})\alpha_{2_{new}}y_{1}y_{2}k_{12}-(H-h_{0}\alpha_{2_{new}})y_{1}V_{1}-\alpha_{2_{new}}y_{2}V_{2}+\varphi_{constant}$

对$\alpha_{2_{new}}$求导数可得：

$\frac{d\varphi(\alpha)}{d\alpha_{2_{new}}}=-(k_{11}+k_{22}-2k_{12})\alpha_{2_{new}}+h_{0}H(k_{11}-k_{22})+y_{2}(V_{1}-V_{2})-h_{0}+1=0$

求解可得：

$(k_{11}+k_{22}-2k_{12})\alpha_{2_{new}}=h_{0}H(k_{11}-k_{22})+y_{2}(V_{1}-V_{2})-h_{0}+1$  (4)

此时，将$H、V_{j}$代入公式（4）可得：

$(k_{11}+k_{22}-2k_{12})\alpha_{2_{new}}=(k_{11}+k_{22}-2k_{12})\alpha_{2}+y_{2}(f(x_{1})-y_{1}-f(x_{2})+y_{2}))$ (5)

令$\eta=k_{11}+k_{22}-2k_{12}，E_{i}=f(x_{i})-y_{i}$并代入公式（5）得：

$\alpha_{2_{new}}=\alpha_{2}+\frac{y_{2}(E_{1}-E_{2})}{\eta}$

**4**、由于$0≤\alpha_{1}≤C，0≤\alpha_{2}≤C$，且$\alpha_{1}y_{1}+\alpha_{2}y_{2}=c$，所以\alpha_{2_{new}}必落在如下区域内
![选择范围](https://img-blog.csdnimg.cn/20181108183131819.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXFpYW5nOTYwNw==,size_16,color_FFFFFF,t_70)

结合图形可以得到$\alpha_{2}$的范围：

$\left\{\begin{matrix}L=max\left \{ 0,\alpha_{1}+\alpha_{2}-C \right \}, H=min\left \{ C,\alpha_{1}+\alpha_{2}\right \},\: \: \: \:  if\:  y_{1}=y_{2}\\ L=max\left \{ 0,\alpha_{2}-\alpha_{1} \right \}, H=min\left \{ C,C+\alpha_{2}-\alpha_{1}\right \}, \: \: \: \:  if\:  y_{1}≠y_{2}\end{matrix}\right.$

此时$\alpha_{2_{new}}$取值为：

$\alpha_{2_{new}}=\left\{\begin{matrix}H\: \:  , \: \: \: \:  if\: \alpha_{2_{new}}≥H\: \: \: \: \: \\ \alpha_{2_{new}} , \: \: \: \:  if\: L<\alpha_{2_{new}}<H\\ L\: \:  , \: \: \: \:  if\: \alpha_{2_{new}}≤L\: \: \: \: \: \end{matrix}\right.$

**第三步**：重复第一、第二步直到$\alpha_{i_{new}}$收敛

1、由$\alpha_{i_{new}}$，根据公式$w=\sum_{i=1}^{m}\alpha_{i}y_{i}x_{i}求出w$

2、只有支持向量满足$1-y_{i}(w^{T}x_{i}+b)=0$，所以大于0的$\alpha_{i_{new}}$必然都是支持向量，否则$\alpha_{i_{new}}>0，1-y_{i}(w^{T}x_{i}+b)<0$，则$\alpha_{i_{new}}(1-y_{i}(w^{T}x_{i}+b))<0$与条件$\alpha_{i_{new}}(1-y_{i}(w^{T}x_{i}+b))=0$（KKT条件）相违背

3、现实中采用了一种鲁棒的方法求解b，方式为：

$b=\frac{1}{|S|}\sum_{s∈S}(\frac{1}{y_{s}}-wx_s)$

4、最终超平面为：

$wx+b=0$

根据分类决策函数$f(x)=sign(wx+b)$得：

$sign(x)=\left\{\begin{matrix}-1\:  , \: \:  if\: x<0\\\: \: \: \: \: 0\: ,\: \:  if\:x=0\: \\ \: \: \: \: \: 1\:,\: \:   if\: x>0\: \end{matrix}\right.$

