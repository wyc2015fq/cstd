
# 将ipynb文件转换为pdf - 机器学习的小学生 - CSDN博客


2017年04月17日 16:36:15[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：6166



## 英文
1.安装python或者安装miniconda(带有python)
2.安装MiKTeX :[https://miktex.org/download](https://miktex.org/download)
3.利用conda命令安装 Jupyter
运行：
```python
jupyter notebook
```
会将当前目录下的文件加载到浏览器本地网站目录下（确保.ipynb文件也在当前目录下)，然后在网站上点开如00.ipynb。
![这里写图片描述](https://img-blog.csdn.net/20170417164936024?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFieV9neWw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**但是上面的方式只能保存英文，中文好像不可以。**
## 中文
下面我们使用文献1中的方法，当然也要确保上面的都安装好。
**ipynb2pdf.py**:
```python
# coding:utf-8
```
```python
import
```
```python
sys
```
```python
import
```
```python
os
```
```python
import
```
```python
re
```
```python
import
```
```python
shutil
notebook = sys.argv[
```
```python
1
```
```python
]
texFile = notebook.replace(
```
```python
'.ipynb'
```
```python
,
```
```python
'.tex'
```
```python
)
```
```python
# 1.convert .ipynb to latex file .tex
```
```python
# 将ipynb文件转为tex文件
```
```python
print
```
```python
'1. convert '
```
```python
+ notebook +
```
```python
' to '
```
```python
+ texFile
```
```python
print
```
```python
'------ \n'
```
```python
os.system(
```
```python
r'jupyter nbconvert --to latex '
```
```python
+ notebook)
```
```python
print
```
```python
'convert over'
```
```python
# 2. add Chinese support by adding the string below
```
```python
# 加入引用的包使支持中文（直接转换中文会丢失）
```
```python
# \usepackage{fontspec, xunicode, xltxtra}
```
```python
# \setmainfont{Microsoft YaHei}
```
```python
# \usepackage{ctex}
```
```python
print
```
```python
'2. add Chinese support to .tex file'
```
```python
print
```
```python
'------'
```
```python
file = open(texFile,
```
```python
'r'
```
```python
)
str_file = file.read()
strinfo = re.compile(
```
```python
'(documentclass[\d\D]+\{article\})'
```
```python
)
```
```python
#查找的字符line0
```
```python
m=re.findall(strinfo,str_file)
```
```python
if
```
```python
len(m) ==
```
```python
0
```
```python
:
```
```python
print
```
```python
r'can not find documentclass[**pt]{article}'
```
```python
sys.exit(
```
```python
1
```
```python
)
str_file = strinfo.sub(
```
```python
'\\1 \n \\usepackage{fontspec, xunicode, xltxtra} \n \\setmainfont{Microsoft YaHei} \r \\usepackage{ctex}'
```
```python
,str_file)
```
```python
#  替换的字符line1
```
```python
file.close()
file = open(texFile,
```
```python
'w'
```
```python
)
file.write(str_file)
file.close()
```
```python
print
```
```python
'add Chinese support successed'
```
```python
# 3. convert .tex to .pdf by xelatex
```
```python
# 使用xelatex命令编译.tex文件得到pdf
```
```python
print
```
```python
'3. convert tex to pdf'
```
```python
print
```
```python
'------'
```
```python
os.system(
```
```python
'xelatex '
```
```python
+ texFile)
```
```python
print
```
```python
'convert pdf successed'
```
```python
# 4. delete the auxiliary files
```
```python
# 清理生成的中间文件
```
```python
# change there if latex file is needed
```
```python
print
```
```python
'4. delete auxiliary files'
```
```python
print
```
```python
'------'
```
```python
os.remove(notebook.replace(
```
```python
'.ipynb'
```
```python
,
```
```python
'.aux'
```
```python
))
os.remove(notebook.replace(
```
```python
'.ipynb'
```
```python
,
```
```python
'.log'
```
```python
))
os.remove(notebook.replace(
```
```python
'.ipynb'
```
```python
,
```
```python
'.out'
```
```python
))
```
```python
# change there if latex file is needed
```
```python
os.remove(notebook.replace(
```
```python
'.ipynb'
```
```python
,
```
```python
'.tex'
```
```python
))
```
```python
if
```
```python
os.path.isdir(notebook.replace(
```
```python
'.ipynb'
```
```python
,
```
```python
'_files'
```
```python
)):
    shutil.rmtree(notebook.replace(
```
```python
'.ipynb'
```
```python
,
```
```python
'_files'
```
```python
))
```
```python
print
```
```python
'delete auxiliary files successed'
```
将上面的文件ipynb2pdf.py拷贝到python.exe文件所在目录，同时需要转换的00.ipynb也在此目录（我们用的相对路径），利用下面的指令：
```python
python ipynb2pdf
```
```python
.py
```
```python
00.
```
```python
ipynb
```
此过程MiKTeX可能会自动下载所需要的包，最后可以在同目录下生成00.pdf文件。
## 手动将.ipynb 转换为latex，然后再转换为pdf
利用上面的方法转换的pdf格式是默认的，例如是A4大小，有些代码可能显示不出来，我们需要将纸张的大小设置为A3的，但是自己又不会修改上面的代码，因此可以先转换为latex，然后利用texlive或者MiKTeX ，打开latex文件，自己在上面修改纸张的大小，如：
```python
\documentclass[
```
```python
11
```
```python
pt,a3paper]
```
```python
{article}
```
具体转换如下：
1.将.ipynb转换为.tex
```python
jupyter
```
```python
-nbconvert
```
```python
--
```
```python
to
```
```python
latex
```
```python
00.
```
```python
ipynb
```
编辑生成的.tex文件，添加支持中文：
```python
\usepackage
```
```python
{fontspec, xunicode, xltxtra}
```
```python
\setmainfont
```
```python
{Microsoft YaHei}
```
```python
\usepackage
```
```python
{ctex}
```
2 将.tex 转换为pdf
即利用texlive或者MiKTeX打开运行即可。
参考文献:
1.[http://www.cnblogs.com/fly2wind/p/6294831.html](http://www.cnblogs.com/fly2wind/p/6294831.html)[直接执行的python代码ipynb2pdf.py]

