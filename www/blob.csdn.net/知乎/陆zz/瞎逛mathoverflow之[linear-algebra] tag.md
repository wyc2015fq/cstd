# 瞎逛mathoverflow之[linear-algebra] tag - 知乎
# 

逛mathoverflow发现了一些有趣的问题，记录于此。

下面的问题都带有[linear-algebra]的tag:

1.[Using linear algebra to classify vector bundles over P^1](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/16434/using-linear-algebra-to-classify-vector-bundles-over-p1)

(Grothendieck) Every vector bundle of finite rank over ![\Bbb P^1_k](https://www.zhihu.com/equation?tex=%5CBbb+P%5E1_k) is direct sum of line bundles and the decomposition is uniquely up to isomorphism.

通过取标准仿射开集，只需证明[Birkhoff factorization](https://link.zhihu.com/?target=http%3A//en.wikipedia.org/wiki/Birkhoff_factorization)：

![\begin{equation} \text{GL}_n(k[t]) \backslash \text{GL}_n(k[t, t^{-1}]) / \text{GL}_n(k[t^{-1}]) \end{equation}=\{\left(\begin{array}{cccc} t^{n_1} & & & 0 \\ & t^{n_2}& & \\ & & \ddots & \\ 0 & & & t^{n_r}\end{array}\right) |\quad n_1 \leq n_2 \leq ... \leq n_r\}](https://www.zhihu.com/equation?tex=%5Cbegin%7Bequation%7D+%5Ctext%7BGL%7D_n%28k%5Bt%5D%29+%5Cbackslash+%5Ctext%7BGL%7D_n%28k%5Bt%2C+t%5E%7B-1%7D%5D%29+%2F+%5Ctext%7BGL%7D_n%28k%5Bt%5E%7B-1%7D%5D%29+%5Cend%7Bequation%7D%3D%5C%7B%5Cleft%28%5Cbegin%7Barray%7D%7Bcccc%7D+t%5E%7Bn_1%7D+%26+%26+%26+0+%5C%5C+%26+t%5E%7Bn_2%7D%26+%26+%5C%5C+%26+%26+%5Cddots+%26+%5C%5C+0+%26+%26+%26+t%5E%7Bn_r%7D%5Cend%7Barray%7D%5Cright%29+%7C%5Cquad+n_1+%5Cleq+n_2+%5Cleq+...+%5Cleq+n_r%5C%7D)

2.[Applications of the Cayley-Hamilton theorem](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/232132/applications-of-the-cayley-hamilton-theorem)

其思想可用于证明著名的Nakayama's lemma，有许多应用如 ![\text{GL}_n(\mathbb F_q)](https://www.zhihu.com/equation?tex=%5Ctext%7BGL%7D_n%28%5Cmathbb+F_q%29) 中元的阶都不超过 ![q^n-1](https://www.zhihu.com/equation?tex=q%5En-1)

3.[Perron-Frobenius theorem](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Perron%25E2%2580%2593Frobenius_theorem)

元素均为正实数的方阵必有实特征值，并且存在一个特征向量其分量也是正实数(Brouwer fixed point theorem)，并且这一特征值被唯一确定(为最长的特征值)

4.[Over which fields are symmetric matrices diagonalizable ?](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/118680/over-which-fields-are-symmetric-matrices-diagonalizable)

实数域上对称矩阵都可对角化，一般来说对称矩阵都可对角化的域即实闭域的交

5.[A slick proof of the Bruhat Decomposition for GL_n(k)?](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/15438/a-slick-proof-of-the-bruhat-decomposition-for-gl-nk)

![GL_n(k)](https://www.zhihu.com/equation?tex=GL_n%28k%29) 关于上三角阵全体 ![B](https://www.zhihu.com/equation?tex=B) 的双陪集一一对应于置换矩阵( ![W=S_n](https://www.zhihu.com/equation?tex=W%3DS_n) )，而问题里则以群作用的方式给了一个证明，其在n=2时是简洁的。

6.linear algebra over ![\Bbb F_2 ](https://www.zhihu.com/equation?tex=%5CBbb+F_2+) to solve the game "[Lights Out](https://link.zhihu.com/?target=http%3A//en.wikipedia.org/wiki/Lights_Out_%2528game%2529)"

![A=(a_{ij}) \in M_n(\Bbb F_2), A^t=A \Rightarrow (a_{ii}) \in \text{Im}A](https://www.zhihu.com/equation?tex=A%3D%28a_%7Bij%7D%29+%5Cin+M_n%28%5CBbb+F_2%29%2C+A%5Et%3DA+%5CRightarrow+%28a_%7Bii%7D%29+%5Cin+%5Ctext%7BIm%7DA)

7.[When is $SL(n,R) \rightarrow SL(n,R/q)$ surjective?](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/78404/when-is-sln-r-rightarrow-sln-r-q-surjective)

这个问题的背景是：![SL_n(\mathbb Z) \rightarrow SL_n(\mathbb F_p)](https://www.zhihu.com/equation?tex=SL_n%28%5Cmathbb+Z%29+%5Crightarrow+SL_n%28%5Cmathbb+F_p%29) 是满射

8.[Condition for two matrices to share at least one eigenvector?](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/144392/condition-for-two-matrices-to-share-at-least-one-eigenvector)

Let  ![A,B](https://www.zhihu.com/equation?tex=A%2CB)  be two n×n matrices with entries in a field ![ K](https://www.zhihu.com/equation?tex=+K). Then ![A,B](https://www.zhihu.com/equation?tex=A%2CB) have a common eigenvector iff  ![\cap_{k,l=1}^{n-1}\ker([A^k,B^l])\not=\{0\}](https://www.zhihu.com/equation?tex=%5Ccap_%7Bk%2Cl%3D1%7D%5E%7Bn-1%7D%5Cker%28%5BA%5Ek%2CB%5El%5D%29%5Cnot%3D%5C%7B0%5C%7D)

9.[Linear transformation that preserves the determinant](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/522/linear-transformation-that-preserves-the-determinant)

保持行列式的线性映射的形状

10.[Non-degenerate alternating bilinear form on a finite abelian group](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/58825/non-degenerate-alternating-bilinear-form-on-a-finite-abelian-group%3Fanswertab%3Dactive%23tab-top)

如果一个有限Abel群上有非退化的反对称双线性型，那么其阶一定是平方数。这可类比反对称矩阵的秩是偶数

11.[Who stated and proved the "Hopf lemma" on bilinear maps?](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/156674/who-stated-and-proved-the-hopf-lemma-on-bilinear-maps)

If ![A\otimes B\rightarrow C](https://www.zhihu.com/equation?tex=A%5Cotimes+B%5Crightarrow+C) is a linear map such that if the image of ![a⊗b](https://www.zhihu.com/equation?tex=a%E2%8A%97b) is zero then either ![a](https://www.zhihu.com/equation?tex=a) or  ![b](https://www.zhihu.com/equation?tex=b)  is zero., where ![A,B,C](https://www.zhihu.com/equation?tex=A%2CB%2CC)  are vector spaces over an algebraically closed field.

Then  ![\dim C\ge \dim A + \dim B -1](https://www.zhihu.com/equation?tex=%5Cdim+C%5Cge+%5Cdim+A+%2B+%5Cdim+B+-1)

12.[What is the size of the category of finite dimensional F_q vector spaces?](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/1047/what-is-the-size-of-the-category-of-finite-dimensional-f-q-vector-spaces)

q-组合数是组合数的类比，许多定理如二项式定理 ![\prod_{i=0}^{n-1} (1+xq^i) = \sum_{k=0}^n x^k q^{{k\choose 2}}{n\choose k}_q](https://www.zhihu.com/equation?tex=%5Cprod_%7Bi%3D0%7D%5E%7Bn-1%7D+%281%2Bxq%5Ei%29+%3D+%5Csum_%7Bk%3D0%7D%5En+x%5Ek+q%5E%7B%7Bk%5Cchoose+2%7D%7D%7Bn%5Cchoose+k%7D_q) 都有q-组合数上的推广。

13.[Under what conditions a linear automorphism is an isometry of some norm?](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/209152/under-what-conditions-a-linear-automorphism-is-an-isometry-of-some-norm)

线性变换可以保持某个范数 等价于 可以保持某个内积

14.[Approximating commuting matrices by commuting diagonalizable matrices](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/111581/approximating-commuting-matrices-by-commuting-diagonalizable-matrices)

The set of pairs of n x n matrices (A,B) where AB = BA and A and B have distinct eigenvalues is Zariski dense in the set of commuting pairs of matrices

15.[Classification of Tori of GL2, up to conjugation](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/121959/classification-of-tori-of-gl2-up-to-conjugation)

The conjugacy classes of maximal  ![k](https://www.zhihu.com/equation?tex=k) -tori in ![GL_n=GL(V)](https://www.zhihu.com/equation?tex=GL_n%3DGL%28V%29) are in 1-1 correspondence with isomorphism classes of étale  ![k](https://www.zhihu.com/equation?tex=k) -algebras  ![E](https://www.zhihu.com/equation?tex=E)  of dimension ![n](https://www.zhihu.com/equation?tex=n) over ![k](https://www.zhihu.com/equation?tex=k) .

补充一个线性代数问题：

k是特征0的域， ![GL_n(k)](https://www.zhihu.com/equation?tex=GL_n%28k%29) 共轭作用在 ![V=\{A \in M_n(\mathbb C) | trA=0 \}](https://www.zhihu.com/equation?tex=V%3D%5C%7BA+%5Cin+M_n%28%5Cmathbb+C%29+%7C+trA%3D0+%5C%7D) ，则这个表示是不可约的。


这个系列暂定两三天一更新，希望不要坑

9.23


