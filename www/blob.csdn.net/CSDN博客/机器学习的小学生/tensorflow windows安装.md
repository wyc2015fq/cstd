
# tensorflow windows安装 - 机器学习的小学生 - CSDN博客


2017年05月23日 16:57:04[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：463个人分类：[tensorflow																](https://blog.csdn.net/xuluhui123/article/category/6934245)



tensorflow windows安装教程:
[https://www.tensorflow.org/install/install_windows](https://www.tensorflow.org/install/install_windows)
我使用的是python3.5.2对应Miniconda3-4.2.11-Windows-x86_64.exe
其对应的是 tensorflow-1.1.0-cp35-cp35m-win_amd64.whl
miniconda的下载地址：
[https://mirrors.tuna.tsinghua.edu.cn/anaconda/miniconda/](https://mirrors.tuna.tsinghua.edu.cn/anaconda/miniconda/)
直接使用指令,即可完成安装。
```python
pip
```
```python
install
```
```python
-
```
```python
-
```
```python
upgrade
```
```python
tensorflow
```
可能会提示下面的错误信息:
```python
Cannot remove entries from nonexistent file d:
```
```python
\miniconda
```
```python
3
```
```python
\lib
```
```python
\site
```
```python
-packages
```
```python
\easy
```
```python
-install.pth
```
则需要在上面的指令中加上`--ignore-installed`
即：
```python
pip
```
```python
install
```
```python
-
```
```python
-
```
```python
upgrade
```
```python
tensorflow
```
```python
-
```
```python
-
```
```python
ignore
```
```python
-
```
```python
installed
```
python2 和python3同时安装
使用miniconda2和miniconda3管理。分别对miniconda2和miniconda3的script目录下conda.exe和对应的conda-script.py 重命名为conda2.exe，conda2-script.py和conda3.exe,conda3-script.py。则可以在dos环境下，利用conda2 install或者conda3 install 命令的形式分别安装。
参考：[http://blog.csdn.net/raby_gyl/article/details/73289192](http://blog.csdn.net/raby_gyl/article/details/73289192)

