
# py3 安装 box2d-py - 颹蕭蕭 - CSDN博客


2019年04月01日 17:15:19[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：71


直接 pip 安装试试
`>>>pip install -i https://pypi.tuna.tsinghua.edu.cn/simple box2d-py
...
 error: Microsoft Visual C++ 14.0 is required. Get it with "Microsoft Visual C++ Build Tools": https://visualstudio.microsoft.com/downloads/
  ----------------------------------------
  Failed building wheel for box2d-py
  ...`只好去找 wheel
[https://www.lfd.uci.edu/~gohlke/pythonlibs/\#pybox2d](https://www.lfd.uci.edu/~gohlke/pythonlibs/#pybox2d)
`>>> pip install Box2D-2.3.2-cp36-cp36m-win_amd64.whl`测试下面这段代码：
```python
import
```
```python
gym
env
```
```python
=
```
```python
gym
```
```python
.
```
```python
make
```
```python
(
```
```python
'CarRacing-v0'
```
```python
)
```
```python
env
```
```python
.
```
```python
reset
```
```python
(
```
```python
)
```
```python
for
```
```python
_
```
```python
in
```
```python
range
```
```python
(
```
```python
1000
```
```python
)
```
```python
:
```
```python
env
```
```python
.
```
```python
render
```
```python
(
```
```python
)
```
```python
env
```
```python
.
```
```python
step
```
```python
(
```
```python
env
```
```python
.
```
```python
action_space
```
```python
.
```
```python
sample
```
```python
(
```
```python
)
```
```python
)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040117135966.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)
atari 的wheel 在这里
[https://github.com/Kojoley/atari-py/releases](https://github.com/Kojoley/atari-py/releases)
[
						](https://img-blog.csdnimg.cn/2019040117135966.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)
[
	](https://img-blog.csdnimg.cn/2019040117135966.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)
