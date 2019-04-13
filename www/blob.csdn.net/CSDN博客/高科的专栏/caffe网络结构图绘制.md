
# caffe网络结构图绘制 - 高科的专栏 - CSDN博客

2018年10月08日 16:14:51[高科](https://me.csdn.net/pbymw8iwm)阅读数：52


绘制网络图通常有两种方法：
一种是利用python自带的draw_net.py，首先安装两个库：
sudo apt-get install graphviz
sudo pip install pydot
接下来就可以用python自带的draw_net.py文件来绘制网络图了。
draw_net.py执行时带三个参数：
一、网络模型的prototxt文件;
二、保存的图片路径和名字;
三、--rankdir=x , x 有四种选项，分别是LR, RL, TB, BT 。用来表示网络的方向，分别是从左到右，从右到左，从上到小，从下到上。默认为LR。
在终于输入命令：sudo python python/draw_net.py examples/mnist/lenet_train_test.prototxt test.png --rankdir=TB
另一种是利用Netscope绘制：
直接登录网站：http://ethereon.github.io/netscope/\#/editor
在左边编辑框里面将prototxt的信息粘贴进来，然后按“shift+enter”就能在右边将网络结构图显示出来
![](https://img-blog.csdn.net/20181008161407447?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3BieW13OGl3bQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

