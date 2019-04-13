
# keras CNTK 后端 - 丁丁的博客 - CSDN博客


2017年06月02日 23:42:35[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：2401


CNTK 刚刚发布了[2.0](https://github.com/Microsoft/CNTK/releases)（beta），引入了 Keras 支持。简单尝试下，离流畅使用还有一定差距。
# 使用 CNTK 后端
(官方文档见[链接](https://docs.microsoft.com/en-us/cognitive-toolkit/using-cntk-with-keras)。)
推荐使用[anaconda](https://www.continuum.io/downloads/)安装 python，已经包括所有的依赖。下面的操作使用 Ubuntu 16.04，python 版本为 2.7。
## 1.  设置虚拟环境
CNTK 目前处于开发阶段，问题较多（见讨论），不建议在正式环境下测试。这里使用虚拟python 环境先尝尝鲜。
```python
conda
```
```python
create
```
```python
-
```
```python
-
```
```python
name
```
```python
cntkkeras
```
```python
-
```
```python
-
```
```python
clone
```
```python
root
```
创建一个名为 cntkkeras 的虚拟环境，环境复制原始环境。
## 2. 切换环境
```python
source
```
```python
activate cntkkeras
```
## 3. 安装 keras
```python
pip install git+https://github
```
```python
.com
```
```python
/souptc/keras
```
```python
.git
```
> Update: keras 官方已经支持 CNTK 后端，keras 安装请参见
> [install](https://keras.io/#installation)
> 。
修改 ~/.keras/keras.json 中的 backend 字段为 cntk。
## 4. 安装 CNTK
```python
pip install <
```
```python
URL
```
```python
to
```
```python
CNTK wheel>
```
我使用上面指令安装失败。因此先下载了 wheel 文件，再运行下面指令：
```python
pip install
```
```python
<
```
```python
CNTK
```
```python
wheel
```
```python
file
```
```python
>
```
wheel地址参见[官网](https://docs.microsoft.com/en-us/cognitive-toolkit/setup-linux-python)。这里我们安装 python 2.7 cpu-only 版。
## 5. Run!
运行 keras/examples/mnist_mlp.py
```python
python mnist_mlp.py
```
运行结果如下：
![这里写图片描述](https://img-blog.csdn.net/20170602234004889?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 讨论
CNTK 原生 python （高层）接口已经与 keras 非常相似。可能先入为主的原因，还是 keras 的接口更舒服一些。
目前（02/06/2017） CNTK 后端好像还有不少问题，如上图中莫名其妙的 float64 的 warning。keras 示例中的 mnist_acgan.py 直接 run 不起来。
试着运行自定义的[keras layers](https://github.com/DingKe/nn_playground)程序，大部分都不能运行。

