
# anaconda 环境导出-导入 - 追求卓越,做到专业 - CSDN博客


2019年01月16日 14:46:23[Waldenz](https://me.csdn.net/enter89)阅读数：1059


从A机器中导出环境到B机器中；
首先在进入到A机器的环境中
conda env list    查找所有环境
![](https://img-blog.csdnimg.cn/20190116143858149.png)
激活进入到所要导出的环境中
activate python36   python36为要导出的环境名称
![](https://img-blog.csdnimg.cn/20190116143844289.png)
导出环境 到yml文件， 文件名为 python36_20190106.yml
```python
conda env export --file python36_20190106.yml
```
![](https://img-blog.csdnimg.cn/20190116144040697.png)
将yml文件复制到B机器中，执行以下命令导入
`conda env create -f  d:\python36_20190106.yml`

