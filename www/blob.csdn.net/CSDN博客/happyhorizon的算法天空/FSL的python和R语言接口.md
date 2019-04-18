# FSL的python和R语言接口 - happyhorizon的算法天空 - CSDN博客
2019年01月10日 15:50:51[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：159
FSL除了本身支持shell命令调用以外, 还有一些其他语言的工具包.例如 python和R. fsl的python编程库称为fslpy, 是可视化工具FSLeyes的一部分.
fslpy目前支持python 3.5, 3.6 and 3.7开发环境.
## FSL的python接口: fslpy
fslpy的项目网址是: [https://git.fmrib.ox.ac.uk/fsl/fslpy](https://git.fmrib.ox.ac.uk/fsl/fslpy)
## Dependencies配置
安装FSL的python版本之前, 先要安装一些依赖工具, 主要有三种: wxPython, indexed_gzip和
- 
**wxPython :** 跨平台的python GUI工具
项目网址:
pip安装:`pip install -f https://extras.wxpython.org/wxPython4/extras/linux/gtk2/ubuntu-16.04/ wxpython`
- 
**indexed_gzip :** 是一个读取NiFTI文件的工具.
项目网址: [https://github.com/pauldmccarthy/indexed_gzip](https://github.com/pauldmccarthy/indexed_gzip)
pip安装: `pip install indexed_gzip`
conda-forge安装: `conda install -c conda-forge indexed_gzip`
- **trimesh/rtree :** 网格化工具
项目网址:
安装: `pip install trimesh`
## fslpy 安装
安装命令很简单, 如果是Anaconda版的python:
```
conda install -c donda-forge fslpy
```
或者pip命令安装
```
pip install fslpy
```
## 安装帮助文档
fslpy的帮助文档需要sphinx工具生成, 安装时, 首先在fslpy的项目页面下载项目源文件, 解压后从shell终端进入该文件夹, 输入:
```
pip install -r requirements-dev.txt
python setup.py doc
```
运行完毕后文件夹中会出现doc/html文件夹, 用浏览器打开帮助文档:
`firefox ./doc/html/index.html`
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190110155029863.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
## 测试
安装是否成功测试: `python setup.py test`
执行完成后会生成 report.html, 其他的测试文件会保存在. htmlcov/
## FSL的R语言接口
fsl R接口: [https://www.rdocumentation.org/packages/fslr/versions/2.22.0](https://www.rdocumentation.org/packages/fslr/versions/2.22.0)
# 备注
conda-forge安装时如果遇到permission的问题可以参考:
[https://blog.csdn.net/jmzlzl/article/details/79464670](https://blog.csdn.net/jmzlzl/article/details/79464670)
关键就是: `sudo chown -R marley:marley /home/marley/anaconda3`
marley是自己的用户名，输入之后，敲击回车，问题就解决了.
