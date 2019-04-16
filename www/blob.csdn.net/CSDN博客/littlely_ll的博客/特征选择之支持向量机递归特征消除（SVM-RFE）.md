# 特征选择之支持向量机递归特征消除（SVM-RFE） - littlely_ll的博客 - CSDN博客





2017年05月15日 14:01:14[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：16321
所属专栏：[特征提取与选择](https://blog.csdn.net/column/details/15615.html)









支持向量机递归特征消除(下文简称SVM-RFE)是由Guyon等人在对癌症分类时提出来的，最初只能对两类数据进行特征提取。它是一种基于Embedded方法。

# 支持向量机

支持向量机广泛用于模式识别，机器学习等领域，SVM采用结构风险最小化原则，同时最小化经验误差，以此提高学习的性能。详细的SVM介绍请看我的另一篇博文[《 线性支持向量机》](http://blog.csdn.net/littlely_ll/article/details/70194556)

在这简单介绍一下SVM。 

设训练集$\{(x_i,y_i)\}_{i=1}^N$，其中$x_i\in R^D, y_i \in \{+1,-1\}$，$x_i为第i$个样本，N为样本量，D为样本特征数。SVM寻找最优的分类超平面$\omega\cdot x+b=0$。 

SVM需要求解的优化问题为： 


$min \quad \frac{1}{2}||\omega||^2+C\Sigma_{i=1}^N\xi_i\\s.t.\quad y_i(\omega\cdot x_i+b)\ge 1-\xi_i,i=1,2,...,N\\\xi_i\ge0,i=1,2,...,N$
而原始问题可以转化为对偶问题： 


$min\quad \frac{1}{2}\Sigma_{i=1}^N\Sigma_{j=1}^N\alpha_i\alpha_jy_iy_j(x_i\cdot x_j)-\Sigma_{i=1}^N\alpha_i\\s.t.\quad \Sigma_{i=1}^Ny_i\alpha_i=0\\0\le \alpha_i\le C,i=1,2,...,N$

其中，$\alpha_i$为拉格朗日乘子。 

最后$\omega$的解为： 


$\omega=\Sigma_{i=1}^N\alpha_iy_ix_i$
# 两分类的SVM-RFE算法

SVM-RFE是一个基于SVM的最大间隔原理的序列后向选择算法。它通过模型训练样本，然后对每个特征进行得分进行排序，去掉最小特征得分的特征，然后用剩余的特征再次训练模型，进行下一次迭代，最后选出需要的特征数。而特征$i$的排序准则得分定义为： 


$c_i=w_i^2$
**两分类SVM-RFE算法：**

输入：训练样本$\{(x_i,y_i)\}_{i=1}^N, y_i \in \{+1,-1\}$

输出：特征排序集R 

1)初始化原始特征集合S={1，2，…,D}，特征排序集R=[] 

2)循环以下过程直至S=[] 
$\quad$获取带候选特征集合的训练样本； 
$\quad$用式$min\quad \frac{1}{2}\Sigma_{i=1}^N\Sigma_{j=1}^N\alpha_i\alpha_jy_iy_j(x_i\cdot x_j)-\Sigma_{i=1}^N\alpha_i$训练SVM分类器，得到$\omega$； 
$\quad$用式$c_i=w_i^2,k=1,2,...,|S|$计算排序准则得分; 
$\quad$找出排序得分最小的特征$p=arg\quad min_kc_k$; 
$\quad$更新特征集R=[p,R]； 
$\quad$在S中去除此特征：S=S/p。
# 多分类的SVM-RFE算法

多分类的SVM-RFE算法其实和两分类的SVM-RFE算法类似，只不过在处理多分类时，把类别进行两两配对，其中一类为正类，另一类为负类，这样需训练$\frac{N(N-1)}{2}$个分类器，这就是一对一（One vs. One,简称OvO）的多分类拆分策略（详细请看周志华的《机器学习》的第三章线性模型的多分类学习），这样就变成了多个两分类问题（当然，也可以使用一对其余（OvR）），每个两类问题用一个SVM-RFE进行特征选择，利用多个SVM-RFE获得多个排序准则得分，然后把多个排序准则得分相加后得到排序准则总分，以此作为特征剔除的依据，每次迭代消去最小特征，直到所有特征都被删除。 
**多分类SVM-RFE算法：**

输入：训练样本集$\{(x_i,v_i)\}_{i=1}^N, v_i \in \{1,2,...,l\},l为类别数$

输出：特征排序集R 

1）初始化原始特征集合S={1，2，…,D}，特征排序集R=[] 

2）生成$\frac{l(l-1)}{2}$个训练样本: 
$\quad$在训练样本$\{(x_i,v_i)\}_{i=1}^N$中找出不同类别的两两组合得到最后的训练样本： 
$\quad$$X_j=$
$\qquad$${\{(x_i,y_i)\}_{i=1}^{N_1+N_{j+1}},j=1,2,...,l;当v_i=1时,y_i=1,当v_i=j+1,y_i=-1}$
$\qquad$${\{(x_i,y_i)\}_{i=1}^{N_2+N_{j-l+3}},j=l,...,2l-3;当v_i=2时,y_i=1,当v_i=j-l+3,y_i=-1}$
$\qquad\cdots \qquad\cdots\qquad\cdots\qquad\cdots\qquad\cdots\qquad\cdots\qquad\cdots\qquad\cdots$
$\quad$${\{(x_i,y_i)\}_{i=1}^{N_l-1+N_{l}},j=\frac{l(l-1)}{2}-1,...,\frac{l(l-1)}{2};当v_i=l-1时,y_i=1,当v_i=l,y_i=-1}$

3）循环一下过程直至S=[]: 
$\quad$获取用l个训练子样本$X_j(j=1,2,...,l(l-1)/2)$； 
$\quad$分别用$X_j$训练SVM，分别得到$\omega_j(j=1,2,...,l)$； 
$\quad$计算排序准则得分$c_k=\Sigma_j\omega_{jk}^2(k=1,2,...,|S|)$； 
$\quad$找出排序准则得分最小的特征$p=arg\quad min_kc_k$； 
$\quad$更新特征集R=[p,R]； 
$\quad$在S中去除此特征S=S/p.
**参考**

【Isabelle Guyon, Jason Weston et.al】Gene Selection for Cancer Classification using Support Vector Machines 

【黄晓娟，张莉】改进的多类支持向量机递归特征消除在癌症多分类中的应用 

【周志华】机器学习







