# TensorFlow升级1.4：Cannot remove entries from nonexistent file \lib\site-pack - wishchinYang的专栏 - CSDN博客
2017年11月17日 11:30:47[wishchin](https://me.csdn.net/wishchin)阅读数：812
0. coco数据集使用TensorFlow版本至少1.3，需要对TensorFlow进行升级。
出现此种问题：Cannot remove entries from nonexistent file \lib\site-pack
解决方法：
`conda remove setuptools`
导致不可逆转的错误，
最后找到
运行：pip install --upgrade --ignore-installed setuptools，问题解决！
1.问题是解决了，仍然不能使用TensorFlow1.4版本，依然是pip 在线安装的1.2.1,很悲剧，因此
参考：
卸载TensorFlow，把1.4.0卸载掉了
重装 tensorflow-1.4.0-cp35-cp35m-win_amd64.whl
这才安装成功TensorFlow1.4版本。
