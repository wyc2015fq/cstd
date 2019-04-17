# Anaconda - qq_30006593的博客 - CSDN博客





2017年09月30日 19:47:41[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：147








Anaconda是一个打包的集合，里面预装好了conda、某个版本的python、众多packages、科学计算工具等等。 

Miniconda，只包含最基本的内容——python与conda，以及相关的必须依赖项。

# 环境操作

conda create -n bunnies python=3 

conda create -n bunnies python=3.6 anaconda 

conda remove -n flowers –all 

conda env list 

不能重命名：conda create –name new_name –clone old_name
# 包

conda list 

conda search xxx 

conda update –all 

update等同于upgrade
# 配置源

conda config –add channels [https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/](https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/)

conda config –set show_channel_urls yes





