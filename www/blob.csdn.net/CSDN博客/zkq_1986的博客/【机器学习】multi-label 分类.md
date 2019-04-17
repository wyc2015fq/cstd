# 【机器学习】multi-label 分类 - zkq_1986的博客 - CSDN博客





2017年10月27日 09:21:56[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：1354








**multi-label 分类**

多标记学习框架中,每个样本可能同时隶属于多个类别标记.因此,与单标记学习系统相比,多标记学习系统的评价准则要更加复杂些.到目前为止,已提出了许多多标记学习系统的性能评价准则.假设测试集,并根据预测函数*fl*(*x*),定义一个排序函数*rankf*(*x*,*l*)Î{1,…,*L*},如果*fl*(*x*)>*fk*(*x*),则*rankf*(*x*,*l*)<*rankf*(*x*,*k*). 本文选取了5种常用的评价准则,即Hamming
 Loss,One-Error,Coverage,Ranking Loss和Average Precision来评价多标记学习系统的性能,具体定义如下:

1) Hamming Loss:用于度量样本在单个标记上的真实标记和预测标记的错误匹配情况:

![](https://img-blog.csdn.net/20171027092301769)

2) One-Error:用来考察预测值排在第1位的标记却不隶属于该样本的情况:

![](https://img-blog.csdn.net/20171027092307599)

其中,![](https://img-blog.csdn.net/20171027092312661)

3) Coverage:用于度量平均上需要多少步才能遍历样本所有的相关标记:

![](https://img-blog.csdn.net/20171027092317271)

其中,*Ri*={*l*|*Yil*=+1}表示样本*xi*的相关标记集合,而![](https://img-blog.csdn.net/20171027092321786)表示样本*xi*的不相关标记集合.

4) Ranking Loss:用来考察样本的不相关标记的排序低于相关标记的排序的情况:

![](https://img-blog.csdn.net/20171027092326438)

5) Average Precision:用来考察排在隶属于该样本标记之前的标记仍属于样本的相关标记集合的情况:

![](https://img-blog.csdn.net/20171027092329655)

上述5个评价准则中,前4个值越小系统性能越优,最优值为0;最后1个值越大越好,最优值为1.





