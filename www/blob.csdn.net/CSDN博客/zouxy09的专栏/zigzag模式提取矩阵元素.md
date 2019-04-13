
# zigzag模式提取矩阵元素 - zouxy09的专栏 - CSDN博客


2013年10月28日 19:17:31[zouxy09](https://me.csdn.net/zouxy09)阅读数：14654


**zigzag模式提取矩阵元素**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
这节博文只是为了上传个代码而已。希望对需要的人有用。zig-zag模式如下图所示。它可以用来按照以下的顺序来提取一个矩阵的元素。
![](https://img-blog.csdn.net/20131028191537703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个东西用在哪呢？用在离散余弦变换的系数提取里面。离散余弦变换（DCT）是种图像压缩算法，JPEG-2000好像就是用它来进行图像压缩的。DCT将图像从像素域变换到频率域。然后一般图像都存在很多冗余和相关性的，所以转换到频率域之后，只有很少的一部分频率分量的系数才不为0，大部分系数都为0（或者说接近于0），这样就可以进行高效的编码，以达到压缩的目的。下图的右图是对lena图进行离散余弦变换（DCT）得到的系数矩阵图。从左上角依次到右下角，频率越来越高，由图可以看到，左上角的值比较大，到右下角的值就很小很小了。换句话说，图像的能量几乎都集中在左上角这个地方的低频系数上面了。
![](https://img-blog.csdn.net/20131028191551453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
而且，系数的重要性一定程度上是按照zigzag模式进行排列的。所以就可以通过zigzag模式来提取这个矩阵前面的重要性的元素，作为这个图像在频率域上的特征，然后可以拿去做分类啥的，以达到降维的功效。
具体代码如下：
zigzagCoder.m

```cpp
function dctVector = zigzagCoder(A, needNumCoefficient)
%// Select elements of a square matrix using zig-zag pattern
%// Author : zouxy  
%// Date   : 2013-10-28  
%// HomePage : http://blog.csdn.net/zouxy09  
%// Email  : zouxy09@qq.com
%% input: A : square matrix 
%%        needNumCoefficient : how many coefficient you want to maintain
%% output: dctVector: vector contain coefficients by col
	if size(A, 1) ~= size(A, 2)
		display('Warning: your matrix should be square!');
		if size(A, 1) > size(A, 2)
			A = A(1:size(A, 2), 1:size(A, 2));
		else
			A = A(1:size(A, 1), 1:size(A, 1));
		end
	end
	
	if needNumCoefficient > (size(A, 1) * size(A, 2))
		needNumCoefficient = size(A, 1) * size(A, 2);
	end
	
	A = A';
	count = 1;    
	for dim_sum = 2 : (size(A, 1) + size(A, 2))
		if mod(dim_sum, 2) == 0
			for i = 1 : size(A, 1)
				if dim_sum - i <= size(A, 1) & dim_sum - i > 0
					dctVector(count) = A(i, dim_sum - i);
					count = count + 1;
				end
			end
		else
			for i = 1 : size(A, 1)
				if dim_sum - i <= size(A, 1) & dim_sum - i >0
					dctVector(count) = A(dim_sum - i, i);
					count = count + 1;
				end                   
			end   
		end
	end
	dctVector = dctVector(1:needNumCoefficient);
	dctVector = dctVector';
	
end
```

例如：
testZigzagCode.m
`mat = [1 2 6; 3 5 7; 4 8 9];
num = size(mat, 1) * size(mat, 2);
code = zigzagCoder(mat, num);
disp(mat);
disp(code');`
输入方阵：
1    2     6
3    5     7
4    8     9
输出向量：
1    2     3     4    5     6     7    8     9

