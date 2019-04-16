# 机器学习导论（张志华）：EM算法 - 去向前方的博客 - CSDN博客





2018年11月01日 14:57:29[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：68








#前言

这个笔记是北大那位老师课程的学习笔记，讲的概念浅显易懂，非常有利于我们掌握基本的概念，从而掌握相关的技术。

#basic concepts

EM算法的核心是，首先假设模型符合什么分布，然后计算相关参数，再根据计算出的结果，重新划分样本分布，然后再计算相关参数，直到收敛为止。

公式证明比较繁琐，这里就不贴了，附上一个python实现的EM
```
#! -*- coding=utf-8 -*-
 
#模拟两个正态分布的均值估计
 
from numpy import *
import numpy as np
import random
import copy
 
SIGMA = 6
EPS = 0.0001
#生成方差相同,均值不同的样本
def generate_data():	
	Miu1 = 20
	Miu2 = 40
	N = 1000
	X = mat(zeros((N,1)))
	for i in range(N):
		temp = random.uniform(0,1)
		if(temp > 0.5):
			X[i] = temp*SIGMA + Miu1
		else:
			X[i] = temp*SIGMA + Miu2
	return X
 
#EM算法
def my_EM(X):
	k = 2
	N = len(X)
	Miu = np.random.rand(k,1)
	Posterior = mat(zeros((N,2)))
	dominator = 0
	numerator = 0
	#先求后验概率
	for iter in range(1000):
		for i in range(N):
			dominator = 0
			for j in range(k):
				dominator = dominator + np.exp(-1.0/(2.0*SIGMA**2) * (X[i] - Miu[j])**2)
				#print dominator,-1/(2*SIGMA**2) * (X[i] - Miu[j])**2,2*SIGMA**2,(X[i] - Miu[j])**2
				#return
			for j in range(k):
				numerator = np.exp(-1.0/(2.0*SIGMA**2) * (X[i] - Miu[j])**2)
				Posterior[i,j] = numerator/dominator			
		oldMiu = copy.deepcopy(Miu)
		#最大化	
		for j in range(k):
			numerator = 0
			dominator = 0
			for i in range(N):
				numerator = numerator + Posterior[i,j] * X[i]
				dominator = dominator + Posterior[i,j]
			Miu[j] = numerator/dominator
		print (abs(Miu - oldMiu)).sum() 
			#print '\n'
		if (abs(Miu - oldMiu)).sum() < EPS:
			print Miu,iter
			break
 
if __name__ == '__main__':
	X = generate_data()
	my_EM(X)
```




