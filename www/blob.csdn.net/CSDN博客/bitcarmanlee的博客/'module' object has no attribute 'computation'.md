
# 'module' object has no attribute 'computation' - bitcarmanlee的博客 - CSDN博客


2019年03月16日 20:36:19[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：58


在使用tensorflow跑CNN模型的时候，发现报了如下异常：
`AttributeError: 'module' object has no attribute 'computation'`经过一番搜索，发现可能跟dask这个包有关系。dask包可能版本太老，于是执行
`pip install --upgrade dask`将dask升级到最新版本，搞定收工

