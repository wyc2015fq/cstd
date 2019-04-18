# R-MAX - 知乎
# 

## 简介

![\mathrm{R-MAX}](https://www.zhihu.com/equation?tex=%5Cmathrm%7BR-MAX%7D) (*[R-MAX- A General Polynomial Time Algorithm for Near-Optimal Reinforcement Learning](https://link.zhihu.com/?target=https%3A//ie.technion.ac.il/~moshet/brafman02a.pdf)*)是第1个对于多智能体在对抗(adversarial)环境下、有理论保证效用(effectiveness)和效率(efficiency)的算法。作为一个特例，当把环境建模为对手的时候，该方法可认为是单智能体的基于模型(Model-based)的一个算法。

## 背景与定义
- **Stochastic Games**

原文采用的是博弈论的定义方法，这里对Stochastic Games做一个简单的回顾。

**Definition 1 **对于一个固定和(fix-sum)，2个智能体的stochastic-game[SG] ![M](https://www.zhihu.com/equation?tex=M) , 其状态集 ![S=\{1, \cdots, N\}](https://www.zhihu.com/equation?tex=S%3D%5C%7B1%2C+%5Ccdots%2C+N%5C%7D) , 动作集 ![A = \{a_1, \cdots, a_k\} ](https://www.zhihu.com/equation?tex=A+%3D+%5C%7Ba_1%2C+%5Ccdots%2C+a_k%5C%7D+) 。双人博弈[SG]包含多个Stage Game, Stage Game之间由Probabilistic Transition Function联系。
- Stage Games：在单个时间步上，拥有 ![s \in S](https://www.zhihu.com/equation?tex=s+%5Cin+S) 和 ![(a, a') \in A \times A](https://www.zhihu.com/equation?tex=%28a%2C+a%27%29+%5Cin+A+%5Ctimes+A) , 其中 ![a](https://www.zhihu.com/equation?tex=a) 是智能体的动作， ![a'](https://www.zhihu.com/equation?tex=a%27) 是对手的动作。用 ![R^i =(R^i_\cdot, R^i_{\cdot'})](https://www.zhihu.com/equation?tex=R%5Ei+%3D%28R%5Ei_%5Ccdot%2C+R%5Ei_%7B%5Ccdot%27%7D%29) 表示stage-game ![i](https://www.zhihu.com/equation?tex=i) 的奖励矩阵。
- Probabilistic Transition Function： ![P_M(s, t, a, a')](https://www.zhihu.com/equation?tex=P_M%28s%2C+t%2C+a%2C+a%27%29) 是给定行为 ![a](https://www.zhihu.com/equation?tex=a) 和 ![a'](https://www.zhihu.com/equation?tex=a%27) ，从状态 ![s](https://www.zhihu.com/equation?tex=s) 转移到状态 ![t](https://www.zhihu.com/equation?tex=t) 的概率

2. **Average Reward Criterion**

首先，定义 ![\mathrm{expected \,\, T-step \,\, undiscounted \,\, average \,\, reward}](https://www.zhihu.com/equation?tex=%5Cmathrm%7Bexpected+%5C%2C%5C%2C+T-step+%5C%2C%5C%2C+undiscounted+%5C%2C%5C%2C+average+%5C%2C%5C%2C+reward%7D)![U_M(s, \pi, \rho, T)](https://www.zhihu.com/equation?tex=U_M%28s%2C+%5Cpi%2C+%5Crho%2C+T%29) ：从初始状态s开始，在智能体采取策略 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) ，对手采取策略 ![\rho](https://www.zhihu.com/equation?tex=%5Crho) 的情况下， ![T](https://www.zhihu.com/equation?tex=T) 步的无折扣平均奖励。

![U_M(s,\pi, T) = \min_{\rho}U_M(s, \pi, \rho, T)\\](https://www.zhihu.com/equation?tex=U_M%28s%2C%5Cpi%2C+T%29+%3D+%5Cmin_%7B%5Crho%7DU_M%28s%2C+%5Cpi%2C+%5Crho%2C+T%29%5C%5C)

![U_M(s, \pi) = \lim_{T \rightarrow \infty}{U_M(s, \pi, T)} \\](https://www.zhihu.com/equation?tex=U_M%28s%2C+%5Cpi%29+%3D+%5Clim_%7BT+%5Crightarrow+%5Cinfty%7D%7BU_M%28s%2C+%5Cpi%2C+T%29%7D+%5C%5C)

![U_M(\pi) = \min_{s \in S} U_M(s, \pi) \\](https://www.zhihu.com/equation?tex=U_M%28%5Cpi%29+%3D+%5Cmin_%7Bs+%5Cin+S%7D+U_M%28s%2C+%5Cpi%29+%5C%5C)

然后，定义 ![\epsilon-\mathrm{return \,\, mixting \,\, time} \,\, T](https://www.zhihu.com/equation?tex=%5Cepsilon-%5Cmathrm%7Breturn+%5C%2C%5C%2C+mixting+%5C%2C%5C%2C+time%7D+%5C%2C%5C%2C+T) ：策略 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) 属于集合 ![\Pi(\epsilon, T)](https://www.zhihu.com/equation?tex=%5CPi%28%5Cepsilon%2C+T%29) 如果对于任意起始状态 ![s](https://www.zhihu.com/equation?tex=s) 和对手策略 ![\rho](https://www.zhihu.com/equation?tex=%5Crho) ，策略 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) 能在时间步 ![T](https://www.zhihu.com/equation?tex=T) 之后(即对于每个 ![t \geq T](https://www.zhihu.com/equation?tex=t+%5Cgeq+T) )保证其收益 ![U(s, \pi, \rho, t) > U(\pi) - \epsilon](https://www.zhihu.com/equation?tex=U%28s%2C+%5Cpi%2C+%5Crho%2C+t%29+%3E+U%28%5Cpi%29+-+%5Cepsilon)；并且我们认为策略 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) 的 ![\epsilon-\mathrm{return \,\, mixting \,\, time}](https://www.zhihu.com/equation?tex=%5Cepsilon-%5Cmathrm%7Breturn+%5C%2C%5C%2C+mixting+%5C%2C%5C%2C+time%7D) 至多为 ![T](https://www.zhihu.com/equation?tex=T) ，而策略 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) 的 ![\epsilon-\mathrm{return \,\, mixting \,\, time}](https://www.zhihu.com/equation?tex=%5Cepsilon-%5Cmathrm%7Breturn+%5C%2C%5C%2C+mixting+%5C%2C%5C%2C+time%7D)为使得 ![\pi \in \Pi(\epsilon, T)](https://www.zhihu.com/equation?tex=%5Cpi+%5Cin+%5CPi%28%5Cepsilon%2C+T%29) 成立的最小的 ![T](https://www.zhihu.com/equation?tex=T) 。

最后，定义 ![\mathrm{Opt(\Pi(\epsilon, T))}](https://www.zhihu.com/equation?tex=%5Cmathrm%7BOpt%28%5CPi%28%5Cepsilon%2C+T%29%29%7D) 为集合 ![\Pi(\epsilon, T)](https://www.zhihu.com/equation?tex=%5CPi%28%5Cepsilon%2C+T%29) 中的策略可以取得最大的![\mathrm{expected \,\, T-step \,\, undiscounted \,\, average \,\, reward}](https://www.zhihu.com/equation?tex=%5Cmathrm%7Bexpected+%5C%2C%5C%2C+T-step+%5C%2C%5C%2C+undiscounted+%5C%2C%5C%2C+average+%5C%2C%5C%2C+reward%7D)。

## ![\mathrm{R-MAX}](https://www.zhihu.com/equation?tex=%5Cmathrm%7BR-MAX%7D) 算法

下面是一个简化的理解版，随后更新具体版。
![](https://pic1.zhimg.com/v2-957014bcfcbd7bfa895dcff453e64500_b.jpg)R-MAX算法的简化版。图片来自http://lamda.nju.edu.cn/yuy/GetFile.aspx?File=adl-rl/CIPS2018-RLtutorial.pdf。
## 理论分析

首先，需要定义 ![\alpha-\mathrm{approxiamtion}](https://www.zhihu.com/equation?tex=%5Calpha-%5Cmathrm%7Bapproxiamtion%7D) 。

**Definition 3** 假设 ![M](https://www.zhihu.com/equation?tex=M) 和 ![\bar{M}](https://www.zhihu.com/equation?tex=%5Cbar%7BM%7D) 是具有相同状态空间、动作空间的[SG]，我们认为 ![\bar{M}](https://www.zhihu.com/equation?tex=%5Cbar%7BM%7D) 是 ![M](https://www.zhihu.com/equation?tex=M) 的一个 ![\alpha-\mathrm{approxiamtion}](https://www.zhihu.com/equation?tex=%5Calpha-%5Cmathrm%7Bapproxiamtion%7D) 如果对于每个状态 ![s](https://www.zhihu.com/equation?tex=s) 有：1) 两者的stage-game的reward matrix是相同的；2） ![P_M(s, t, a, a') - \alpha \leq P_{\bar{M}}(s, t, a, a') \leq P_M(s, t, a, a') + \alpha ](https://www.zhihu.com/equation?tex=P_M%28s%2C+t%2C+a%2C+a%27%29+-+%5Calpha+%5Cleq+P_%7B%5Cbar%7BM%7D%7D%28s%2C+t%2C+a%2C+a%27%29+%5Cleq+P_M%28s%2C+t%2C+a%2C+a%27%29+%2B+%5Calpha+)  。

**Lemma 4** 假设 ![M](https://www.zhihu.com/equation?tex=M) 和 ![\bar{M}](https://www.zhihu.com/equation?tex=%5Cbar%7BM%7D) 具有 ![N](https://www.zhihu.com/equation?tex=N) 个状态，奖励的界限为 ![[0, R_{max}]](https://www.zhihu.com/equation?tex=%5B0%2C+R_%7Bmax%7D%5D) ，并且 ![\bar{M}](https://www.zhihu.com/equation?tex=%5Cbar%7BM%7D) 是 ![M](https://www.zhihu.com/equation?tex=M) 的一个 ![\frac{\epsilon}{NTR_{max}}-\mathrm{approxiamtion}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cepsilon%7D%7BNTR_%7Bmax%7D%7D-%5Cmathrm%7Bapproxiamtion%7D) ，那么对于任意起始状态 ![s](https://www.zhihu.com/equation?tex=s) ，智能体策略 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) ，对手策略 ![\rho](https://www.zhihu.com/equation?tex=%5Crho) ，有：

![|U_\bar{M}(s, \pi, \rho, T) - U_M(s, \pi, \rho, T)| \leq \epsilon \\](https://www.zhihu.com/equation?tex=%7CU_%5Cbar%7BM%7D%28s%2C+%5Cpi%2C+%5Crho%2C+T%29+-+U_M%28s%2C+%5Cpi%2C+%5Crho%2C+T%29%7C+%5Cleq+%5Cepsilon+%5C%5C)

> ![\mathrm{Proof}](https://www.zhihu.com/equation?tex=%5Cmathrm%7BProof%7D) :
为了证明Lemma 4，我们需要证明：
![\sum_{p}|\Pr_M(p)U_M(p) - \Pr_{\bar M}(p) U_{\bar M}(p)| \leq \epsilon \\](https://www.zhihu.com/equation?tex=%5Csum_%7Bp%7D%7C%5CPr_M%28p%29U_M%28p%29+-+%5CPr_%7B%5Cbar+M%7D%28p%29+U_%7B%5Cbar+M%7D%28p%29%7C+%5Cleq+%5Cepsilon+%5C%5C)
其中 ![p](https://www.zhihu.com/equation?tex=p) 表示一个T-step的轨迹， ![\Pr_M(p)](https://www.zhihu.com/equation?tex=%5CPr_M%28p%29) (和 ![\Pr_{\bar M} (p)](https://www.zhihu.com/equation?tex=%5CPr_%7B%5Cbar+M%7D+%28p%29) )分别为由 ![M](https://www.zhihu.com/equation?tex=M) (相应的 ![\bar M](https://www.zhihu.com/equation?tex=%5Cbar+M) )、 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) 、 ![\rho](https://www.zhihu.com/equation?tex=%5Crho) 构成的随机过程引入的轨迹概率， ![U_M(p)](https://www.zhihu.com/equation?tex=U_M%28p%29) (和 ![U_{\bar M}(p)](https://www.zhihu.com/equation?tex=U_%7B%5Cbar+M%7D%28p%29) 为相应的轨迹的无折扣平均奖励。由 ![U_M(p)](https://www.zhihu.com/equation?tex=U_M%28p%29) 和 ![U_{\bar M}(p)](https://www.zhihu.com/equation?tex=U_%7B%5Cbar+M%7D%28p%29)的界限为 ![[0, R_{max}]](https://www.zhihu.com/equation?tex=%5B0%2C+R_%7Bmax%7D%5D) ，所以：
![\sum_{p}|\Pr_M(p)U_M(p) - \Pr_{\bar M}(p) U_{\bar M}(p)| \leq \sum_p |\Pr_M(p) - \Pr_{\bar M}(p)| R_{max} \\](https://www.zhihu.com/equation?tex=%5Csum_%7Bp%7D%7C%5CPr_M%28p%29U_M%28p%29+-+%5CPr_%7B%5Cbar+M%7D%28p%29+U_%7B%5Cbar+M%7D%28p%29%7C+%5Cleq+%5Csum_p+%7C%5CPr_M%28p%29+-+%5CPr_%7B%5Cbar+M%7D%28p%29%7C+R_%7Bmax%7D+%5C%5C)
所以，我们只需要证明：
![\sum_p |\Pr_M(p) - \Pr_{\bar M}(p)| \leq \frac{\epsilon}{R_{max}} \\](https://www.zhihu.com/equation?tex=%5Csum_p+%7C%5CPr_M%28p%29+-+%5CPr_%7B%5Cbar+M%7D%28p%29%7C+%5Cleq+%5Cfrac%7B%5Cepsilon%7D%7BR_%7Bmax%7D%7D+%5C%5C)
为了证明上式，需要使用一个小技巧：假设 ![h_i](https://www.zhihu.com/equation?tex=h_i) 表示前 ![i](https://www.zhihu.com/equation?tex=i) 步采用 ![\bar M](https://www.zhihu.com/equation?tex=%5Cbar+M) 的状态转移矩阵， ![i](https://www.zhihu.com/equation?tex=i) 步以后采用 ![M](https://www.zhihu.com/equation?tex=M) 的状态转移矩阵；只考虑 ![T](https://www.zhihu.com/equation?tex=T) 个时间步，![h_0](https://www.zhihu.com/equation?tex=h_0) 就是 ![M](https://www.zhihu.com/equation?tex=M) ， ![h_T](https://www.zhihu.com/equation?tex=h_T) 就是 ![\bar M](https://www.zhihu.com/equation?tex=%5Cbar+M) 。这个技巧在证明[策略迭代](https://link.zhihu.com/?target=https%3A//www.jianshu.com/writer%23/notebooks/21685579/notes/33968230/preview)的时候也会用到。利用三角不等式和数列求和，有：

![\begin{align*} \sum_p |\Pr_M(p) - \Pr_{\bar M}(p)| &= \sum_p |\Pr_{h_0}(p) - \Pr_{h_T}(p)| \\&= \sum_p |\Pr_{h_0}(p) - \Pr_{h_1}(p) +   \Pr_{h_1}(p)  -  \Pr_{h_2}(p) + \cdots +\Pr_{h_T}(p)| \\  &\leq \sum_{i=0}^{T-1} \sum_{p} |\Pr_{h_i}(p) - \Pr_{h_{i+1}}(p)| \end{align*}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%2A%7D+%5Csum_p+%7C%5CPr_M%28p%29+-+%5CPr_%7B%5Cbar+M%7D%28p%29%7C+%26%3D+%5Csum_p+%7C%5CPr_%7Bh_0%7D%28p%29+-+%5CPr_%7Bh_T%7D%28p%29%7C+%5C%5C%26%3D+%5Csum_p+%7C%5CPr_%7Bh_0%7D%28p%29+-+%5CPr_%7Bh_1%7D%28p%29+%2B+++%5CPr_%7Bh_1%7D%28p%29++-++%5CPr_%7Bh_2%7D%28p%29+%2B+%5Ccdots+%2B%5CPr_%7Bh_T%7D%28p%29%7C+%5C%5C++%26%5Cleq+%5Csum_%7Bi%3D0%7D%5E%7BT-1%7D+%5Csum_%7Bp%7D+%7C%5CPr_%7Bh_i%7D%28p%29+-+%5CPr_%7Bh_%7Bi%2B1%7D%7D%28p%29%7C+%5Cend%7Balign%2A%7D)
那么只需要证明：
![\sum_{p} |\Pr_{h_i}(p) - \Pr_{h_{i+1}}(p)| \leq  \frac{\epsilon}{TR_{max}} \\](https://www.zhihu.com/equation?tex=%5Csum_%7Bp%7D+%7C%5CPr_%7Bh_i%7D%28p%29+-+%5CPr_%7Bh_%7Bi%2B1%7D%7D%28p%29%7C+%5Cleq++%5Cfrac%7B%5Cepsilon%7D%7BTR_%7Bmax%7D%7D+%5C%5C)
将 ![\Pr(p)](https://www.zhihu.com/equation?tex=%5CPr%28p%29) 进行展开：
![\begin{align*} \sum_{p} |\Pr_{h_i}(p) - \Pr_{h_{i+1}}(p)|  = \sum_{x_i}\sum_{pre(x_i)}\sum_{x_{i+1}}\sum_{suf(x_{i+1})} &|\Pr_{h_i}(pre(x_i))\Pr_{h_i}(x_i \to x_{i+1}) \Pr_{h_i}(suf(x_{i+1}) - \\ & \Pr_{h_{i+1}}(pre(x_i))\Pr_{h_{i+1}}(x_i \to x_{i+1})\Pr_{h_{i+1}}(suf(x_{i+1})| \end{align*}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%2A%7D+%5Csum_%7Bp%7D+%7C%5CPr_%7Bh_i%7D%28p%29+-+%5CPr_%7Bh_%7Bi%2B1%7D%7D%28p%29%7C++%3D+%5Csum_%7Bx_i%7D%5Csum_%7Bpre%28x_i%29%7D%5Csum_%7Bx_%7Bi%2B1%7D%7D%5Csum_%7Bsuf%28x_%7Bi%2B1%7D%29%7D+%26%7C%5CPr_%7Bh_i%7D%28pre%28x_i%29%29%5CPr_%7Bh_i%7D%28x_i+%5Cto+x_%7Bi%2B1%7D%29+%5CPr_%7Bh_i%7D%28suf%28x_%7Bi%2B1%7D%29+-+%5C%5C+%26+%5CPr_%7Bh_%7Bi%2B1%7D%7D%28pre%28x_i%29%29%5CPr_%7Bh_%7Bi%2B1%7D%7D%28x_i+%5Cto+x_%7Bi%2B1%7D%29%5CPr_%7Bh_%7Bi%2B1%7D%7D%28suf%28x_%7Bi%2B1%7D%29%7C+%5Cend%7Balign%2A%7D)
前缀概率 ![\Pr(suf(x_i))](https://www.zhihu.com/equation?tex=%5CPr%28suf%28x_i%29%29) 和后缀概率 ![\Pr(sux(x_{i+1})](https://www.zhihu.com/equation?tex=%5CPr%28sux%28x_%7Bi%2B1%7D%29) 对于 ![h_i](https://www.zhihu.com/equation?tex=h_i) 和 ![h_{i+1}](https://www.zhihu.com/equation?tex=h_%7Bi%2B1%7D) 是相同的。
![\mathrm{RHS} = \sum_{x_i}\sum_{pre(x_i)} \Pr_{h_i / h_{i+1}}(pre(x_i)) \sum_{x_{i+1}} \sum_{suf(x_{i+1})} \Pr_{h_i / h_{i+1}}(suf(x_{i+1}) | \Pr_{h_i}(x_i \to x_{i+1})  - \Pr_{h_i}(x_i \to x_{i+1}) | \\](https://www.zhihu.com/equation?tex=%5Cmathrm%7BRHS%7D+%3D+%5Csum_%7Bx_i%7D%5Csum_%7Bpre%28x_i%29%7D+%5CPr_%7Bh_i+%2F+h_%7Bi%2B1%7D%7D%28pre%28x_i%29%29+%5Csum_%7Bx_%7Bi%2B1%7D%7D+%5Csum_%7Bsuf%28x_%7Bi%2B1%7D%29%7D+%5CPr_%7Bh_i+%2F+h_%7Bi%2B1%7D%7D%28suf%28x_%7Bi%2B1%7D%29+%7C+%5CPr_%7Bh_i%7D%28x_i+%5Cto+x_%7Bi%2B1%7D%29++-+%5CPr_%7Bh_i%7D%28x_i+%5Cto+x_%7Bi%2B1%7D%29+%7C+%5C%5C)
又根据 ![\mathrm{\alpha-approximation}](https://www.zhihu.com/equation?tex=%5Cmathrm%7B%5Calpha-approximation%7D) 的定义，有：
![\mathrm{RHS} \leq [\sum_{x_i}\sum_{pre(x_i)} \Pr_{h_i / h_{i+1}}(pre(x_i)) ][\sum_{x_{i+1}}\sum_{suf(x_{i+1})}\Pr_{h_i / h_{i+1}}(suf(x_{i+1})) \times \frac{\epsilon}{NTR_{max}}] \\](https://www.zhihu.com/equation?tex=%5Cmathrm%7BRHS%7D+%5Cleq+%5B%5Csum_%7Bx_i%7D%5Csum_%7Bpre%28x_i%29%7D+%5CPr_%7Bh_i+%2F+h_%7Bi%2B1%7D%7D%28pre%28x_i%29%29+%5D%5B%5Csum_%7Bx_%7Bi%2B1%7D%7D%5Csum_%7Bsuf%28x_%7Bi%2B1%7D%29%7D%5CPr_%7Bh_i+%2F+h_%7Bi%2B1%7D%7D%28suf%28x_%7Bi%2B1%7D%29%29+%5Ctimes+%5Cfrac%7B%5Cepsilon%7D%7BNTR_%7Bmax%7D%7D%5D+%5C%5C)
显然，第一项——从任意起始状态 ![x_i](https://www.zhihu.com/equation?tex=x_i) 和其前缀 ![pre(x_i)](https://www.zhihu.com/equation?tex=pre%28x_i%29) 的概率积的和为1；第二项，从给定的 ![x_{i+1}](https://www.zhihu.com/equation?tex=x_%7Bi%2B1%7D) ，其后缀概率和 ![suf(x_{i+1})](https://www.zhihu.com/equation?tex=suf%28x_%7Bi%2B1%7D%29) 也为1；则只剩下对 ![x_{i+1}](https://www.zhihu.com/equation?tex=x_%7Bi%2B1%7D) 的遍历。所以，
![\mathrm{RHS} = \sum_p |\Pr_{h_i}(p) - \Pr_{h_{i+1}}(p)| \leq 1 \times N \times \frac{\epsilon}{NTR_{max}} \\](https://www.zhihu.com/equation?tex=%5Cmathrm%7BRHS%7D+%3D+%5Csum_p+%7C%5CPr_%7Bh_i%7D%28p%29+-+%5CPr_%7Bh_%7Bi%2B1%7D%7D%28p%29%7C+%5Cleq+1+%5Ctimes+N+%5Ctimes+%5Cfrac%7B%5Cepsilon%7D%7BNTR_%7Bmax%7D%7D+%5C%5C)
至此，Lemma 4证明完毕。

(后面的好难，还没看懂-。-）

