# python 中 常用到的 numpy 函数 整理 - 数据之美的博客 - CSDN博客
2017年03月17日 16:01:09[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：463
个人分类：[python](https://blog.csdn.net/oppo62258801/article/category/6738404)

1. 创建二维数组  array()   ：
set = array([[1., 2, ],[3., 4.],[5., 6.],[7., 9.]])
求 数组的 行数：
>>> set.shape[0]
4
求 数组的列数：
>>> set.shape[1]
>>> set.shape
(4, 2)
>>> set.dtype
dtype('float64')
#### 记得 >>> from numpy import * 
2  empty() 函数：
**[python]**[view
 plain](http://blog.csdn.net/timidsmile/article/details/16963699#)[copy](http://blog.csdn.net/timidsmile/article/details/16963699#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/82520/fork)
- >>> a = empty([2, 2])  
- >>> a  
- array([[  2.01269048e-313,   4.44659081e-323],  
-        [  5.03965339e+223,   6.48588014e-310]])  
- >>> b = empty([2, 2], dtype = int)  
- >>> b  
- array([[19988563, 36534944],  
-        [ 2460004,  2460004]])  
- >>> c = empty([2, 2], dtype = int, order = 'C')  
- >>> c  
- array([[19988581, 36534944],  
-        [ 2460004,  2460004]])  
- >>> d = empty([2, 2], dtype = int ,order = 'F')  
- >>> d  
- array([[19857521,  2460004],  
-        [36534944,  2460004]])  
- >>>   
最后一个参数，返回数组在内存中的存放顺序，
C代表[C语言](http://lib.csdn.net/base/c)风格， row major
F代表····，column  major
3.  eye（）
**[python]**[view
 plain](http://blog.csdn.net/timidsmile/article/details/16963699#)[copy](http://blog.csdn.net/timidsmile/article/details/16963699#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/82520/fork)
- >>> e1 = eye(2, 3, 0, dtype = int)  
- >>> e1  
- array([[1, 0, 0],  
-        [0, 1, 0]])  
- >>> e2 = eye(3)  
- >>> e2  
- array([[ 1.,  0.,  0.],  
-        [ 0.,  1.,  0.],  
-        [ 0.,  0.,  1.]])  
- >>> e3 = eye(3, dtype = int)  
- >>> e3  
- array([[1, 0, 0],  
-        [0, 1, 0],  
-        [0, 0, 1]])  
- >>> e4 = eye(3, 1, dtype = int)  
- >>> e4  
- array([[1],  
-        [0],  
-        [0]])  
- >>> e5 = eye(3, k = 1, dtype = int)  
- >>> e5  
- array([[0, 1, 0],  
-        [0, 0, 1],  
-        [0, 0, 0]])  
- >>> e6 = eye(3, k = -1, dtype = int)  
- >>> e6  
- array([[0, 0, 0],  
-        [1, 0, 0],  
-        [0, 1, 0]])  
- >>>   
第一个参数N = 列数
第二个参数 M = 行数，省略代表M = N 
第三个参数 k 代表对角线位置， = 0 代表主对角线， +1就向右上方偏移1， -1 就向左下角偏移1
第四个参数表示类型 dtype 默认为 float 类型
4 。 创建 方阵 identity（）
**[python]**[view
 plain](http://blog.csdn.net/timidsmile/article/details/16963699#)[copy](http://blog.csdn.net/timidsmile/article/details/16963699#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/82520/fork)
- >>> i1 = identity(3)  
- >>> i1  
- array([[ 1.,  0.,  0.],  
-        [ 0.,  1.,  0.],  
-        [ 0.,  0.,  1.]])  
- >>> i2 = identity(3, dtype = int)  
- >>> i2  
- array([[1, 0, 0],  
-        [0, 1, 0],  
-        [0, 0, 1]])  
- >>>   
只有两个参数，第一个表示 行（列）数，第二个表示类型（默认为float）类型
5.  生成一个元素全为1的数组
**[python]**[view
 plain](http://blog.csdn.net/timidsmile/article/details/16963699#)[copy](http://blog.csdn.net/timidsmile/article/details/16963699#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/82520/fork)
- >>> o1 = ones(3)  
- >>> o1  
- array([ 1.,  1.,  1.])  
- >>> o1.shape  
- (3,)  
要指定完整的shape（完整的行数和列数）的话：
**[python]**[view
 plain](http://blog.csdn.net/timidsmile/article/details/16963699#)[copy](http://blog.csdn.net/timidsmile/article/details/16963699#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/82520/fork)
- >>> o4 = ones( (2, 3), dtype = int)  
- >>> o4  
- array([[1, 1, 1],  
-        [1, 1, 1]])  
- >>>   
6. zeros（） 全是0 的矩阵
**[python]**[view
 plain](http://blog.csdn.net/timidsmile/article/details/16963699#)[copy](http://blog.csdn.net/timidsmile/article/details/16963699#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/82520/fork)
- <pre code_snippet_id="82520" snippet_file_name="blog_20131126_6_5493221" name="code"class="python">>>> from numpy import *  
- >>> z1 = zeros(3)  
- >>> z1  
- array([ 0.,  0.,  0.])  
- >>> z1.shape  
- (3,)  
- >>> z2 = zeros((2, 3), dtype = int)  
- >>> z2  
- array([[0, 0, 0],  
-        [0, 0, 0]])  
- >>> z2.shape  
- (2, 3)  
- >>> s = (3, 2)  
- z4 = zeros(s)  
- >>> z4  
- array([[ 0.,  0.],  
-        [ 0.,  0.],  
-        [ 0.,  0.]])  
- >>> z4.shape  
- (3, 2)</pre><pre code_snippet_id="82520" snippet_file_name="blog_20131126_12_2832072" name="code"class="python"></pre><pre code_snippet_id="82520" snippet_file_name="blog_20131126_12_2832072" name="code"class="python"></pre><pre code_snippet_id="82520" snippet_file_name="blog_20131126_9_956672" name="code"class="python">ones_like（）  zeros_like()</pre><pre code_snippet_id="82520" snippet_file_name="blog_20131126_12_2832072" name="code"class="python"></pre><pre code_snippet_id="82520" snippet_file_name="blog_20131126_11_8513673" name="code"class="python"><pre code_snippet_id="82520" snippet_file_name="blog_20131126_11_8513673" name="code"class="python">>>> from numpy import *  
- >>> s = (3, 2)  
- >>> a = array(s)  
- >>> a  
- array([3, 2])  
- >>> a.shape  
- (2,)  
- >>> z = zeros(s, dtype = int)  
- >>> zz = zeros_like(z)  
- >>> zz  
- array([[0, 0],  
-        [0, 0],  
-        [0, 0]])  
- >>> ooo = ones_like(z)  
- >>> ooo = ones_like(z)  
- >>> ooo  
- array([[1, 1],  
-        [1, 1],  
-        [1, 1]])  
- >>> </pre><br>  
- <br>  
- <p></p>  
- <pre></pre>  
- <pre code_snippet_id="82520" snippet_file_name="blog_20131126_12_2832072" name="code"class="python"></pre>  
- <pre></pre>  
- <p></p>  
- 
- </pre>  
