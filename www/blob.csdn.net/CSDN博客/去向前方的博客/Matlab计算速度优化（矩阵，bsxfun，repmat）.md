# Matlab计算速度优化（矩阵，bsxfun，repmat） - 去向前方的博客 - CSDN博客





2018年09月24日 11:44:17[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：269











### 文章目录
- [前言](#_1)
- [正文](#_3)
- [测试](#_33)




# 前言

最近再写一个大的程序，需要考虑一下如何节约硬件资源，以及提高运算速度。

# 正文

bsxfun  Binary Singleton Expansion Function

C = bsxfun(FUNC,A,B) applies the element-by-element binary operation

specified by the function handle FUNC to arrays A and B, with singleton

expansion enabled. FUNC can be one of the following built-in functions:
```
@plus           Plus
               @minus          Minus
               @times          Array multiply
               @rdivide        Right array divide
               @ldivide        Left array divide
               @power          Array power
               @max            Binary maximum
               @min            Binary minimum
               @rem            Remainder after division
               @mod            Modulus after division
               @atan2	        Four-quadrant inverse tangent; result in radians
               @atan2d	        Four-quadrant inverse tangent; result in dgrees
               @hypot	        Square root of sum of squares
               @eq             Equal
               @ne             Not equal
               @lt             Less than
               @le             Less than or equal
               @gt             Greater than
               @ge             Greater than or equal
               @and            Element-wise logical AND
               @or             Element-wise logical OR
               @xor            Logical EXCLUSIVE OR
```

# 测试

```
%这段测试了循环运算和矩阵运算耗时的差异，结果是矩阵快了仅10倍
testa=rand(1,100);
testb=rand(100,100);
tic
result1=testa*testb;
toc
result2=zeros(1,100);
tic
for i=1:100
    result2(1,i)=testa*testb(:,i);
end
toc

%这段测试的循环运算和bsx运算的差异，bsxfun快了30倍
A=rand(200,100);
B=rand(200,100);
beta=0.5;
tic
K1 = zeros(size(A,1),size(B,1)); 
for i = 1 : size(A,1)
    for j = 1 : size(B,1)
        K1(i,j) = exp(-sum((A(i,:)-B(j,:)).^2)/beta); 
    end
end
toc
tic 
sA = (sum(A.^2, 2)); 
sB = (sum(B.^2, 2));
K2 = exp(bsxfun(@minus,bsxfun(@minus,2*A*B', sA), sB')/beta);
toc


%这段测试的repmat扩容后矩阵运算和bsxfun运算的差异，bsxfun快了4倍
 A = magic(5);
tic
 A = bsxfun(@minus, A, mean(A));
toc
A=magic(5);
tic
A=A(min(A,repmat(mean(A),size(A,1),1)));
toc
```

综上，最好使用bsxfun解决矩阵运算的问题，其次是repmat扩容后使用矩阵运算，最次是使用循环运算。




