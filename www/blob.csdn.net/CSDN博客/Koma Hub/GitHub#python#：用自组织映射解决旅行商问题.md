# GitHub#python#：用自组织映射解决旅行商问题 - Koma Hub - CSDN博客
2018年05月08日 11:40:14[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：286
**项目名称：som-tsp：用自组织映射解决旅行商问题**
（2018年1月21日发布，作者：Diego Vicente）
**项目地址：**
GitHub地址：[https://github.com/DiegoVicen/som-tsp](https://github.com/DiegoVicen/som-tsp)
原文地址（英文）：[https://diego.codes/post/som-tsp/](https://diego.codes/post/som-tsp/)
中文翻译（编译：Bing）：[https://mp.weixin.qq.com/s/O7UHeTFfcJ1FjNShVe9wtA](https://mp.weixin.qq.com/s/O7UHeTFfcJ1FjNShVe9wtA)
**跑项目：**
软件环境：CentOS7+python3.6
硬件环境：Inteli3+4G
项目的requirements.txt中有依赖的python库（matplotlib、numpy、pandas），如果没有安装的话利用pip安装。
```
$ python -m pip install matplotlib
$ python -m pip install pandas
$ python -m pip install numpy
```
**运行项目：**
```
$ cd som-tsp
$ ls assert
fi10639.tsp  it16862.tsp  qa194.tsp  uy734.tsp
$ python src/main.py assets/qa194.tsp
Problem with 194 cities read.
Network of 1552 neurons created. Starting the iterations:
Radius has completely decayed, finishing execution at 24487 iterations
Route found of length 9983.850809148884
```
结果：
![](https://img-blog.csdn.net/20180508113854229)
