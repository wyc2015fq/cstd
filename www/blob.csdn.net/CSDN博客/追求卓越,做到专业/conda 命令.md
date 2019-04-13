
# conda 命令 - 追求卓越,做到专业 - CSDN博客


2018年12月27日 10:06:56[Waldenz](https://me.csdn.net/enter89)阅读数：52



### conda命令
[查看conda版本](#conda_1)
[conda环境](#conda_3)
[关于包的操作](#_22)

# 查看conda版本
conda -V
# conda环境
创建一个名为python36的环境，指定Python版本是3.6
conda create -n python36 python=3.6
激活某个环境
activate python36 \# for Windows
source activate python36 \# for Linux & Mac
退出某个环境
deactivate python36 \# for Windows
source deactivate python36 \# for Linux & Mac
删除一个已有的环境
conda remove --name python36 --all
conda remove -n yourenvname --all
查看已安装的环境，当前环境的路径前会有*号显示
conda info -e
conda env list
复制一个环境
通过克隆来复制一个环境。这儿将通过克隆snowfllakes来创建一个称为flowers的副本。
conda create -n flowers --clone snowflakes
关于包的操作
查看当前环境下已安装包
conda list
查看指定环境下已安装包
conda list -n python36
查找package信息
conda search package-name
安装包
conda install ****
安装package，不用-n指定环境名称，则安装在当前活跃环境
conda install -n python34 numpy
更新所有包
conda upgrade  --all
更新包
conda update package-name
更新指定环境中的包
conda update -n python34 package-name
更新conda，保持conda最新
conda update conda
更新anaconda
conda update anaconda
更新python
conda update python
删除包
conda remove package-name

