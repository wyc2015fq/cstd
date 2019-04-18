# Algorithmic Toolbox week4 Divide and Conquer --- Polynomial Multiplication - weixin_33985507的博客 - CSDN博客
2018年01月02日 20:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
## Problem Overview and Native Solution
![5409095-9f6809c328fb3265.png](https://upload-images.jianshu.io/upload_images/5409095-9f6809c328fb3265.png)
Multiplying Polynomials
![5409095-6341c0a681eb8129.png](https://upload-images.jianshu.io/upload_images/5409095-6341c0a681eb8129.png)
视频中把2n-2写成了2n-1
![5409095-ba955c3212ab371b.png](https://upload-images.jianshu.io/upload_images/5409095-ba955c3212ab371b.png)
Multiplying Polynomials
Well in that case, what you you could do is just pad out the smaller polynomial, the lower degree polynomial, to have zeros for its earlier coefficients. (填充0，让我想起来了AES加密的填充，哈哈)
## Naive Divide and Conquer Algorithm
![5409095-002b149a177b0016.png](https://upload-images.jianshu.io/upload_images/5409095-002b149a177b0016.png)
image.png
**Plus, then,in order to take the results and do our addition that's goingto take order n time. So some constant k, times that.**
![5409095-fe3650b9f8ae867e.png](https://upload-images.jianshu.io/upload_images/5409095-fe3650b9f8ae867e.png)
image.png
![5409095-7d9a8b9bd3a20e06.png](https://upload-images.jianshu.io/upload_images/5409095-7d9a8b9bd3a20e06.png)
image.png
![5409095-765c62e39951e5fa.png](https://upload-images.jianshu.io/upload_images/5409095-765c62e39951e5fa.png)
image.png
![5409095-1d7bc59a76e915c8.png](https://upload-images.jianshu.io/upload_images/5409095-1d7bc59a76e915c8.png)
image.png
