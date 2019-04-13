
# Hessian Matrix（海森矩阵） - 郭云飞的专栏 - CSDN博客


2017年10月16日 09:58:41[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：1190


Hessian Matrix，译作黑塞矩阵、海森矩阵、海瑟矩阵、海塞矩阵等。是一个多元函数的二阶偏导数构成的*方阵*，描述了函数的*局部曲率*。Hessian
 Matrix最早于19世纪由德国数学家Ludwig Otto Hesse提出，并以其名字命名。
Hessian Matrix常用于*牛顿法解决优化问题*，利用Hessian Matrix可判定多元函数的极值问题。
在工程实际问题的优化设计中，所列的目标函数往往很复杂，为了使问题简化，常常将目标函数在某点邻域展开成泰勒多项式来逼近原函数，此时函数在某点泰勒展开式的矩阵形式中会涉及到Hessian Matrix。
**一、二元函数的Hessian Matrix**
由高等数学知识可知，若一元函数f(x)在点X0的某个邻域内具有任意阶导数，则f(x)在点X0处的泰勒展开式为：
![](https://img-blog.csdn.net/20171016101707905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对于二元函数![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D61/sign=859b56290c0828386c0ddf15b999b199/30adcbef76094b360e4be6dba8cc7cd98d109d2f.jpg)在点![](https://gss0.bdstatic.com/-4o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D96/sign=b3f6071d9616fdfadc6ccae8b48f3b50/9f510fb30f2442a7cd2a0193da43ad4bd11302db.jpg)处的泰勒展开式为：
![](https://img-blog.csdn.net/20171016102144381?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
将上式写成矩阵的形式：
![](https://gss0.bdstatic.com/94o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D571/sign=8ccc6d0c12d8bc3ec20806cdb38ba6c8/1ad5ad6eddc451da6186d4ddbdfd5266d0163290.jpg)
上式缩写为：
![](https://gss0.bdstatic.com/-4o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D387/sign=bacfd97cd1b44aed5d4eb8ec841d876a/58ee3d6d55fbb2fb7fc18224444a20a44623dc1a.jpg)
其中：
![](https://gss3.bdstatic.com/7Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D312/sign=b330f23d50b5c9ea66f305e2e738b622/55e736d12f2eb9381cfbbec4de628535e5dd6f76.jpg)
![](https://gss3.bdstatic.com/7Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D51/sign=de2bc8072a3fb80e08d161d637d12cfb/50da81cb39dbb6fd8da421100224ab18972b370d.jpg)就是![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D61/sign=8fd623ceac86c9170c035138c93d6f52/a08b87d6277f9e2f1d14da3e1430e924b899f3e7.jpg)在点![](https://gss0.bdstatic.com/94o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D28/sign=c7704b3e90504fc2a65fb70de5dd2518/34fae6cd7b899e51e7edfd2249a7d933c8950da1.jpg)处的Hessian
 Matrix，它是函数![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D61/sign=8fd623ceac86c9170c035138c93d6f52/a08b87d6277f9e2f1d14da3e1430e924b899f3e7.jpg)在点![](https://gss0.bdstatic.com/94o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D28/sign=c7704b3e90504fc2a65fb70de5dd2518/34fae6cd7b899e51e7edfd2249a7d933c8950da1.jpg)处的二阶导数组成的*方阵*。
**二、多元函数的Hessian Matrix**
将二元函数的泰勒展开式推广到多元函数，则![](https://gss2.bdstatic.com/-fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D108/sign=1da428cba41ea8d38e227004af0b30cf/dcc451da81cb39db98a3f826d2160924ab18305a.jpg)在点![](https://gss0.bdstatic.com/94o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D28/sign=c7704b3e90504fc2a65fb70de5dd2518/34fae6cd7b899e51e7edfd2249a7d933c8950da1.jpg)处的泰勒展开式的矩阵形式为：
![](https://gss0.bdstatic.com/94o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D387/sign=4a1e3e17798b4710ca2ffbc4f4cfc3b2/d043ad4bd11373f072bda9a9af0f4bfbfbed0402.jpg)
其中：
![](https://gss0.bdstatic.com/94o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D252/sign=0a35da7580d4b31cf43c93beb5d7276f/08f790529822720e4c2b23cc70cb0a46f21fab1b.jpg)，它是![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D36/sign=5d6306d8bf8f8c54e7d3c3293a29376e/83025aafa40f4bfb09df5e62084f78f0f73618fd.jpg)在![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D36/sign=5d6306d8bf8f8c54e7d3c3293a29376e/83025aafa40f4bfb09df5e62084f78f0f73618fd.jpg)处的梯度。

![](https://gss0.bdstatic.com/-4o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D305/sign=5e5b5318a3d3fd1f3209a43a054f25ce/80cb39dbb6fd52660cc6fc22a018972bd4073613.jpg)为![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D36/sign=5d6306d8bf8f8c54e7d3c3293a29376e/83025aafa40f4bfb09df5e62084f78f0f73618fd.jpg)在![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D36/sign=5d6306d8bf8f8c54e7d3c3293a29376e/83025aafa40f4bfb09df5e62084f78f0f73618fd.jpg)处的Hessian
 Matrix。

**三、利用Hessian Matrix判定多元函数的极值**
设n多元实函数![](https://gss2.bdstatic.com/-fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D108/sign=1da428cba41ea8d38e227004af0b30cf/dcc451da81cb39db98a3f826d2160924ab18305a.jpg)在点![](https://gss3.bdstatic.com/-Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D112/sign=82e8f225d654564ee165e03881df9cde/d788d43f8794a4c2b6a2291d05f41bd5ad6e3907.jpg)的邻域内有二阶连续偏导，若有：
![](https://gss0.bdstatic.com/94o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D201/sign=6b467e0451afa40f38c6c9dd9a65038c/b999a9014c086e060523f67d09087bf40ad1cb79.jpg)
且
![](https://gss0.bdstatic.com/94o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D244/sign=5f751422b399a9013f355c3229950a58/bf096b63f6246b604c767a42e0f81a4c510fa288.jpg)
则：
当A正定矩阵时，![](https://gss2.bdstatic.com/-fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D108/sign=1da428cba41ea8d38e227004af0b30cf/dcc451da81cb39db98a3f826d2160924ab18305a.jpg)在![](https://gss3.bdstatic.com/-Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D112/sign=82e8f225d654564ee165e03881df9cde/d788d43f8794a4c2b6a2291d05f41bd5ad6e3907.jpg)处是极小值
当A负定矩阵时，![](https://gss2.bdstatic.com/-fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D108/sign=1da428cba41ea8d38e227004af0b30cf/dcc451da81cb39db98a3f826d2160924ab18305a.jpg)在![](https://gss3.bdstatic.com/-Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D112/sign=82e8f225d654564ee165e03881df9cde/d788d43f8794a4c2b6a2291d05f41bd5ad6e3907.jpg)处是极大值
当A不定矩阵时，![](https://gss3.bdstatic.com/-Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D112/sign=82e8f225d654564ee165e03881df9cde/d788d43f8794a4c2b6a2291d05f41bd5ad6e3907.jpg)不是极值点
当A为半正定矩阵或半负定矩阵时，![](https://gss3.bdstatic.com/-Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D112/sign=82e8f225d654564ee165e03881df9cde/d788d43f8794a4c2b6a2291d05f41bd5ad6e3907.jpg)是“可疑”极值点，尚需要利用其他方法来判定。






