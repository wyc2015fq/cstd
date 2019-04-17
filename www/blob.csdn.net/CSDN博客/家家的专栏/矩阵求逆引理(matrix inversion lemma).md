# 矩阵求逆引理(matrix inversion lemma) - 家家的专栏 - CSDN博客





2010年12月18日 19:47:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：15224标签：[matrix																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=matrix&t=blog)
个人分类：[数学 Matlab 基本知识](https://blog.csdn.net/yihaizhiyan/article/category/749753)










#### 关于“矩阵求逆引理”


      转自：http://xialulee.spaces.live.com/blog/cns!4EE324C8ACFA82DB!1218.entry

      
若矩阵A∈CN×N，C∈CN×N，均为非奇异矩阵，矩阵B∈CN×M，D∈CM×N，则矩阵A+BCD具有逆矩阵： 


(A+BCD)-1=A-1-A-1B(DA-1B+C-1)-1DA-1

      
我试图推导，花了好多时间，却没有什么收获。这个式子仿佛四年前见过，有点印象。推导这个不是为了别的，只是想到今后可能要用，如果手头没有相关的书籍，又不记得这个公式，就只能靠自己来算了。 


      后来找到了一个比较简单的方法，虽然可能不很严密，但是如果需要使用这个引理而又不记得具体表达时，就可以用手推出来。 


      
首先必须记住的是可逆矩阵A+BCD的逆可以表示成A-1+X，其中X为未知矩阵，这样一来，确定A+BCD阵的逆的问题就转化为解一个关于X的方程的问题。 


∵A-1+X=(A+BCD)-1; 


∴(A+BCD)(A-1+X)=E(E为单位阵，eye阵); 


E+AX+BCDA-1+BCDX=E; 


AX+BCDA-1+BCDX=0(0为0矩阵); 


(A+BCD)X+BCDA-1=0; 


X=-(A+BCD)-1BCDA-1; 


X=-[B(B-1A+CD)]-1BCDA-1; 


X=-(B-1A+CD)-1CDA-1; 


X=-[C(C-1B-1A+D)]-1CDA-1; 


X=-(C-1B-1A+D)-1DA-1; 


X=-[(C-1B-1+DA-1)A]-1DA-1; 


X=-A-1(C-1B-1+DA-1)-1DA-1; 


X=-A-1[(C-1+DA-1B)B-1]-1DA-1; 


X=-A-1B(C-1+DA-1B)-1DA-1; 


      将其带入A-1+X，就得到了引理的结果： 


(A+BCD)-1=A-1-A-1B(C-1+DA-1B)-1DA-1; 


      总之只要记住A+BCD的逆可以表示成A-1+X的形式，那么这个矩阵求逆引理是无论如何能够推出来的。 





