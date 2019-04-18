# PLSA模型的再理解以及源码分析 - bigfacesafdasgfewgf - CSDN博客





2015年01月30日 17:48:58[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：3390








**PLSA模型的再理解以及源码分析**



    之前写过一篇PLSA的博文，其中的收获就是知道PLSA是LSA在概率层面的扩展，知道了PLSA是一种主题模型，知道了PLSA中的参数估计使用的是EM算法。当时我就认为，这样子经典好用的算法，我是会回头再来理解它的，这样子才会有更加深刻的心得。所以有了这篇PLSA模型的再理解。




# 1. 两种思路解PLSA模型




    参考了很多资料，发现大体上有两种解决PLSA模型的思路。下面我们大致说一下它们的思路。




**思路一：EM算法中需要更新两个概率**




    PLSA模型的示意图如下：

![](https://img-blog.csdn.net/20150130170343212?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其中包括的概率有：

![](https://img-blog.csdn.net/20150130170433179?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





也就是说我们知道的是P(d,w)，需要求解的是P(z,d)和P(w,z)。

    我们根据P(d,w)构建似然函数，而且我们需要最大化这个似然函数，如下：

![](https://img-blog.csdn.net/20150130170659976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


需要明确的是，这个似然函数中的自变量是P(z,d)和P(w,z),不巧的是，这两个变量在Log函数的内部是相加的关系，我们知道这为对似然函数的求导带来了很多麻烦。所以我们使用的是EM算法。

   EM算法的基本实现思想是：

(1)E步骤：求隐含变量Given当前估计的参数条件下的后验概率。


(2)M步骤：最大化Complete data对数似然函数的期望，此时我们使用E步骤里计算的隐含变量的后验概率，得到新的参数值。


两步迭代进行直到收敛。





    具体而言，

    在PLSA模型中的E步骤，我们直接用贝叶斯公式计算隐含变量zk在当前参数取值条件下的后验概率，记住是后验概率，如下：





![](https://img-blog.csdn.net/20150130171231729?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



这里的![](https://img-blog.csdn.net/20150130171236655?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)都是已知的，因为我们一开始会对它们设置一个初始值，这样后面的迭代就可以依据于此。使用上面的公式我们计算出了隐含变量的后验概率![](https://img-blog.csdn.net/20150130171346715?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，然后我们进入M步骤。

    M步骤中，我们需要计算complete data的对数似然函数的期望，而且要最大化这个期望。那么首先我们要先计算出这个对数似然期望，如下：

![](https://img-blog.csdn.net/20150130171548848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（整篇算法就是这个无法理解，为什么期望是这样子的公式）

注意，这个时候的P(z|d,w)是已知的了，因为在E步骤中已经计算过了，这时候我们需要最大化这个期望，其中包含的自变量有P(z,d)和P(w,z)，所以我们需要分别对它们求偏导，令偏导为0：

![](https://img-blog.csdn.net/20150130171910934?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





解方程可得如下的结果：





![](https://img-blog.csdn.net/20150130172029492?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





这个就是我们在M步骤中最大化complete data对似然函数的期望，得到的两个参数的更新公式。

    不断重复EM两个步骤，可以得到最终的两个参数P(z,d)和P(w,z)的值。




**思路二：EM算法中需要更新3个参数**




    这个思路和上面思路的区别就在于对P(d,w)的展开公式使用的不同，思路二使用的是3个概率来展开的，如下：

![](https://img-blog.csdn.net/20150130172621320?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这样子我们后面的EM算法的大致思路都是相同的，就是表达形式变化了，最后得到的EM步骤的更新公式也变化了。当然，思路二得到的是3个参数的更新公式。如下：

![](https://img-blog.csdn.net/20150130172917213?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





你会发现，其实多了一个参数是P(z)，参数P（d|z）变化了（之前是P(z|d)），然后P(w|z)是不变的，计算公式也相同。




# 2. PLSA算法的应用效果




    对于数据X，如下：

![](https://img-blog.csdn.net/20150130173225175?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





我们使用思路二的PLSA算法，更新许多次之后的概率为：




iteration 27:

p(d/z)=

0.0000 0.0769 0.2931 0.2934 0.0000 0.0733 0.1467 0.1166 0.0000

0.1952 0.3223 0.0002 0.0000 0.1952 0.0000 0.0000 0.0918 0.1952


p(w/z)=

0.0733 0.0733 0.0000 0.0823 0.2878 0.0000 0.0000 0.1467 0.0000 0.2200 0.1166 0.0000

0.0651 0.0652 0.1302 0.1222 0.0049 0.1302 0.1302 0.0000 0.1302 0.0000 0.0918 0.1302


p(z)=

0.4702 0.5298





迭代了27次，得到了这三个概率。那么这三个概率有什么用呢？




   我们知道LSA模型是利用SVD把原来的数据矩阵X降维，使用两个词之间的语义关系被挖掘了出来。两个原始空间中不相关的却语义上相关的词，在LSA模型中是比较相关的，这个是LSA直接的作用。那么PLSA有什么作用呢？




   给定一个文档d，我们可以将其分类到一些主题词类别下。

    PLSA算法可以通过训练样本的学习得到三个概率，而对于一个测试样本，其中P(w|z)概率是不变的，但是P(z)和P(d|z)都是需要重新更新的，我们也可以使用上面的EM算法，假如测试样本d的数据，我们可以得到新学习的P(z)和P(d|z)参数。这样我们就可以计算：

![](https://img-blog.csdn.net/20150130173923037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


为什么要计算P(z|d)呢？因为给定了一个测试样本d，要判断它是属于那些主题的，我们就需要计算P(z|d)，就是给定d，其在主题z下成立的概率是多少，不就是要计算![](https://img-blog.csdn.net/20150130174127092?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)吗。这样我们就可以计算文档d在所有主题下的概率了。

    这样既可以把一个测试文档划归到一个主题下，也可以给训练文档打上主题的标记，因为我们也是可以计算训练文档它们的的![](https://img-blog.csdn.net/20150130174310848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。如果从这个应用思路来说，思路一说似乎更加直接，因为其直接计算出来了![](https://img-blog.csdn.net/20150130174310848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。




    如果从上面这个应用的角度来说，LSA和PLSA的目的就是从一群文档集中找到潜在的语义因子latent factors。由于提取到的主题词比文档中的词的数量要少很多，而且我们在学习的过程中不需要知道文档的类型信息，所以说LSA和PLSA是无监督的特征降维方法。



# 3. PLSA源码分析




    知道PLSA算法的理论和作用，我们还要会用，下面提供一个MATLAB源码，人家写的挺简单的，我加了一些分析，帮助理解。




main函数：



```
% function demo
% 
close all; clear all;
load data.mat;
k=2;
[pz pdz pwz pzdw]=plsa(X,k);  % 返回的是4种概率
imagesc(pz); colormap(gray); colorbar; 
title('Probability of topics pz'); 
xlabel('topic z_i'); ylabel('probability');

figure;
imagesc(pdz); colormap(gray); colorbar;
title('Category probability of document pdz'); 
xlabel('document d_i (pdz_{(i,j)} is the prob. that d_i belongs to z_j)'); ylabel('topic z_j');

figure;
imagesc(pwz); colormap(gray); colorbar;
title('Category probability of word pwz'); 
xlabel('word w_i (pwz_{(i,j)} is the prob. that w_i belongs to z_j)'); ylabel('topic z_j');
```






PLSA子函数：






```
function [pz pdz pwz pzdw]=plsa(X,k)
% function [pz pdz pwz pzdw]=plsa(X,k)
% Return pLSA probability matrix p of m*n matrix X
% X is the document-word co-occurence matrix
% k is the number of the topics--z
% document--collums,word--rows

err=0.0001;
x = X; 
[m n]=size(x);  % m个词，n个文档
pz=rand(1,k);  % 随机初始化pz, k个主题词
pz2=pz;
pz2(1)=pz2(1)+2*err;

% 初始化的都是随机值
pdz=rand(k,n);  % 二维
pwz=rand(k,m);  % 二维
pzdw=rand(m,n,k);     % 三维   %initialize
h=0;

deno=zeros(1,k);        %denominator of p(d/z) and p(w/z)
denopzdw=zeros(m,n);       %denominator of p(z/d,w)
numepdz=zeros(k,n);        %numerator of p(d/z)
numepwz=zeros(k,m);        % numerator of p(w/z)
R=sum(sum(x));  %  所有的文档的词数总和

for ki=1:k
    for i=1:m
        for j=1:n
            % P(w|z)和P(d|z)的分母，是每个zk一个值，所以是个向量
            deno(ki)=deno(ki)+x(i,j)*pzdw(i,j,ki); % 叠加，计算的是P(w|z)和P(d|z)的分母
        end
    end
end
% p(d/z)
for ki=1:k
    for j=1:n
        for i=1:m
            % P(d|z)的分子，每个zk和dj一个值，所以是一个矩阵
            numepdz(ki,j)=numepdz(ki,j)+x(i,j)*pzdw(i,j,ki); % 叠加，计算的是P(d|z)的分子
        end
        pdz(ki,j)=numepdz(ki,j)/deno(ki);  % 分子除以分母，一个矩阵
    end
end
% disp(pdz);

% p(w/z)
for ki=1:k
    for i=1:m
        for j=1:n
            % 计算的是P(w|z)的分子，每个zk和wi一个值，一个矩阵
            numepwz(ki,i)=numepwz(ki,i)+x(i,j)*pzdw(i,j,ki);
        end
        % 分子除以分母，矩阵中的每个元素
        pwz(ki,i)=numepwz(ki,i)/deno(ki);
    end
end
% disp(pwz);

% p(z)
for ki=1:k
    pz(ki)=deno(ki)./R;  % 即为前面的分母
end
%denominator of p(z/d,w)
for i=1:m
    for j=1:n
        for ki=1:k
            % 计算的是P(z|d,w)的分母，对zk叠加，每个wi和dj一个值，一个矩阵
            denopzdw(i,j)=denopzdw(i,j)+pz(ki)*pdz(ki,j)*pwz(ki,i);
        end
    end
end
% p(z/d,w)
for i=1:m
     for j=1:n
         for ki=1:k
             % 计算的是P(z|d,w)，每个zk和wi和dj一个值，一个三维矩阵
             pzdw(i,j,ki)=pz(ki)*pdz(ki,j)*pwz(ki,i)/denopzdw(i,j);
         end
     end
end
% fprintf('p(z/d,w)=\n');
% disp(pzdw)

%  iteration 
iteration=0;
fprintf('iteration:\n');
while abs(pz2(1)-pz(1))>err || abs(pz2(2)-pz(2))>err
    iteration=iteration+1;  % 迭代次数
    deno=zeros(1,k);        %denominator of p(d/z) and p(w/z)
    denopzdw=zeros(m,n);       %denominator of p(z/d,w)
    numepdz=zeros(k,n);        %numerator of p(d/z)
    numepwz=zeros(k,m);        % numerator of p(w/z)
    fprintf('iteration %d:\n',iteration);
    for ki=1:k
        for i=1:m
            for j=1:n
                deno(ki)=deno(ki)+x(i,j)*pzdw(i,j,ki); % 继续迭代，deno回到0，因为需要叠加，不过pzdw就是上次的新值
            end
        end
    end
% p(d/z)
    for ki=1:k
        for j=1:n
            for i=1:m
                numepdz(ki,j)=numepdz(ki,j)+x(i,j)*pzdw(i,j,ki);  % 计算公式同上，只不过此时的pzdw是更新过后的值
            end
            pdz(ki,j)=numepdz(ki,j)/deno(ki);
        end
    end
    fprintf('p(d/z)=\n');
    disp(pdz)
    % p(w/z)
    for ki=1:k
        for i=1:m
            for j=1:n
                numepwz(ki,i)=numepwz(ki,i)+x(i,j)*pzdw(i,j,ki);
            end
            pwz(ki,i)=numepwz(ki,i)/deno(ki);
        end
    end
    fprintf('p(w/z)=\n');
    disp(pwz)

    % p(z)
    pz=pz2;
    for ki=1:k
        pz2(ki)=deno(ki)./R;
    end
    fprintf('p(z)=\n');
    disp(pz2)
    %denominator of p(z/d,w)
    for i=1:m
        for j=1:n
            for ki=1:k
                denopzdw(i,j)=denopzdw(i,j)+pz2(ki)*pdz(ki,j)*pwz(ki,i);
            end
        end
    end
    % p(z/d,w)
    for i=1:m
         for j=1:n
             for ki=1:k
                 pzdw(i,j,ki)=pz2(ki)*pdz(ki,j)*pwz(ki,i)/(denopzdw(i,j)+eps);
             end
         end
    end
end  %end while

return;
```



参考：

http://www.cnblogs.com/ywl925/p/3552815.html


http://blog.csdn.net/huangxy10/article/details/8091478

http://blog.csdn.net/huangxy10/article/details/8091558


LSA和PLSA 唐克坦

http://blog.csdn.net/puqutogether/article/details/41720073

























