# SVM超平面推导 - wsp_1138886114的博客 - CSDN博客





2018年09月02日 11:56:12[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：369








##### 目标：SVM模型是为求得使几何间隔最大的超平面：$y = w·x+b$
- 由点面之间的距离：


$\frac{y_i(wx_i+b)}{||w||} = \gamma_i$

其中 $γ$ 是几何间隔，$w$ 是超平面法向量，$b$ 是超平面截距，$y_i$ 是样本 $i$ 的标记。
考虑到**几何问题与函数问题**的关系： 


$\begin{align}\underset{w,b}{Max}  &~~~~\gamma  = \frac{\hat{\gamma}}{||w||}\tag{1}\\S.t.&~~~~\frac{y_i(wx_i+b)}{||w||} \geqslant \gamma~~~~i=1,2,3...N \tag{2}\end{align}$

因为：$\hat{\gamma}$ 的取值不会影响上述不等式的成立，所以不妨取其为1。 

注意到，$Max:  \frac{1}{||w||}	 \Leftrightarrow  Min: \frac{1}{2}||w||^2	（对w,b参数）$等价，于是:  

问题变为： 


$\begin{align}\underset{w,b}{Min} & ~~~~\frac{1}{2}||w||^2	\tag{3}\\S.t. &~~~~y_i(wx_i+b)-1 \geqslant  0	~~~~i=1,2,3….,N\tag{4}\end{align}$

**构建拉格朗日函数**其中，$\alpha=(α_1,α_2,α_3,....α_i,)^T ~~~ i=1,2,3...N$：  


$\begin{align}L(w,b, α)=&\frac{1}{2}||w||^2- \sum_{i=1}^{N}α(y_i (w·x_i+b)-1)  \tag{5}\\		=& \frac{1}{2}||w||^2-  \sum_{i=1}^{N}αy_i (w·x_i+b)+ \sum_{i=1}^{N}α  \tag{6}\end{align}$

根据拉格朗日对偶性，原始问题的对偶问题是 极大 极少问题： 


$\underset{α}{\max} \underset{w,b}{\min}L(w,b,α)$

可以分两步进行，先求**L 对 w,b 的最小值**，再求**L对 $α$ 的最大值**：

（1）求 $\underset{w,b}{\min}L(w,b,\alpha)$：对 w,b 求偏导,令其等于0：  


$\begin{align}▽_wL(w,b, α)= &w-\sum_{i}^{N}α~y_ix_i =0\tag{7}\\▽_bL(w,b, α)=& \sum_{i}^{N}α~y_i = 0\tag{8}\end{align}$

得到： 


$\begin{align}w=\sum_{i=1}^{N}α~y_ix_i\tag{A}\\\sum_{i=1}^{N}α~y_i = 0	\tag{B}\end{align}$
将（A）式代入拉格朗日函数，并使用（B）式，可以得到：$\underset{w,b}{\min}L(w,b,\alpha)$


$\begin{align}L(w,b,\alpha)=&\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_i\alpha_jy_iy_j(x_i·x_j)-\sum_{i=1}^{N}\alpha_iy_i\left(\left (\sum_{i=1}^{N}\alpha_jx_jy_j\right )\cdot x_i+b\right )+\sum_{i=1}^{N}\alpha_i\\=&-\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_i\alpha_jy_iy_j(x_i·x_j)+\sum_{i=1}^{N}\alpha_i\end{align}$

（2）求 $\underset{w,b}{\min}L(w,b,\alpha) 对 α$ 的极大值，即对偶问题:： 


$\begin{align}\underset{α}{\max}~~&-\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_i\alpha_jy_iy_j(x_i·x_j)+\sum_{i=1}^{N}\alpha_i \tag{9}\\S.t. ~~&\sum_{i=1}^{N}α~y_i = 0\tag{10}\end{align}$

由对偶问题转化为： 


$\begin{align}\underset{α}{\min}~~&\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_i\alpha_jy_iy_j(x_i·x_j)-\sum_{i=1}^{N}\alpha_i \tag{11}\\S.t. ~~&\sum_{i=1}^{N}α~y_i = 0\tag{12}\end{align}$

对(11)式关于$α_i$ 的偏导并令其为 0，结合约束条件可以解得 $α^*$， 

由于原始问题与对偶问题满足共解条件，所以可以使用α*代入式（A）和式（B） 

求得(C) 


$w^*=\sum_{i=1}^{N}α_i^*y_ix_i	$

由KKT互补条件知： 


$α_i^*(y_i (w^*x_i+b^*)-1）=0	~~~~~~i=1,2,3,……..N$

因为 $w^*$不为0，显然有$α_j^*>0$存在，对于这样的$α_j^*$，$j$ 可以使下式成立 


$\begin{align}由：&~~~~y_j(w^*·x_j+b^*)-1=0 \tag{15}\\\Rightarrow & ~~~~y_j(w^*·x_j+b^*)- y_j^2=0 \tag{16}\\\Rightarrow & ~~~~w^*x_j+b^*- y_j=0 \tag{17}\end{align}$

将（C）式代入上式，可以得到： 


$\sum_{i=1}^{N}α_i^*y_ix_ix_j+ b^* - y_j =0$


$b^*=y_j - \sum_{i=1}^{N}α_i^*y_ix_ix_j$

至此，超平面求出，即： 


$\sum_{i=1}^{N}α_i^*·y_i(x·x_i)+b^* =0$

分类决策函数可以写成 


$f(x)=sign\left ( \sum_{i=1}^{N}α_i^*·y_i(x·x_i)+b^*  \right )$





