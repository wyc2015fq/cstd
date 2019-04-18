# python try/except/finally - aiaiai010101的博客 - CSDN博客

2017年04月27日 20:39:54[aiaiai010101](https://me.csdn.net/aiaiai010101)阅读数：238



稍微总结一下，否则总是忘。

**[python]**[view plain](#)[copy](#)

[print](#)[?](#)

- x = 'abc'
- def fetcher(obj, index):  
- return obj[index]  
- 
- fetcher(x, 4)  

![](http://static.blog.csdn.net/images/save_snippets.png)

```python
x = 'abc'
def fetcher(obj, index):
    return obj[index]

fetcher(x, 4)
```

输出：

**[plain]**[view plain](#)[copy](#)

[print](#)[?](#)

-   File "test.py", line 6, in <module>  
-     fetcher(x, 4)  
-   File "test.py", line 4, in fetcher  
-     return obj[index]  
- IndexError: string index out of range  

![](http://static.blog.csdn.net/images/save_snippets.png)

```
File "test.py", line 6, in <module>
    fetcher(x, 4)
  File "test.py", line 4, in fetcher
    return obj[index]
IndexError: string index out of range
```

**第一： try不仅捕获异常，而且会恢复执行**

**[python]**[view plain](#)[copy](#)

[print](#)[?](#)

- def catcher():  
- try:  
-         fetcher(x, 4)  
- except:  
- print"got exception"
- print"continuing"

![](http://static.blog.csdn.net/images/save_snippets.png)

```python
def catcher():
    try:
        fetcher(x, 4)
    except:
        print "got exception"
    print "continuing"
```
输出：

**[plain]**[view plain](#)[copy](#)

[print](#)[?](#)

- got exception  
- continuing  

![](http://static.blog.csdn.net/images/save_snippets_01.png)

```
got exception
continuing
```

**第二：无论try是否发生异常，finally总会执行**

**[python]**[view plain](#)[copy](#)

[print](#)[?](#)

- def catcher():  
- try:  
-         fetcher(x, 4)  
- finally:  
- print'after fecth'

![](http://static.blog.csdn.net/images/save_snippets.png)

```python
def catcher():
    try:
        fetcher(x, 4)
    finally:
        print 'after fecth'
```
输出：

**[plain]**[view plain](#)[copy](#)

[print](#)[?](#)

- after fecth  
- Traceback (most recent call last):  
-   File "test.py", line 55, in <module>  
-     catcher()  
-   File "test.py", line 12, in catcher  
-     fetcher(x, 4)  
-   File "test.py", line 4, in fetcher  
-     return obj[index]  
- IndexError: string index out of range  

![](http://static.blog.csdn.net/images/save_snippets.png)

```
after fecth
Traceback (most recent call last):
  File "test.py", line 55, in <module>
    catcher()
  File "test.py", line 12, in catcher
    fetcher(x, 4)
  File "test.py", line 4, in fetcher
    return obj[index]
IndexError: string index out of range
```

**第三：try无异常，才会执行else**

**[python]**[view plain](#)[copy](#)

[print](#)[?](#)

- def catcher():  
- try:  
-         fetcher(x, 4)  
- except:  
- print"got exception"
- else:  
- print"not exception"

![](http://static.blog.csdn.net/images/save_snippets.png)

```python
def catcher():
    try:
        fetcher(x, 4)
    except:
        print "got exception"
    else:
        print "not exception"
```
输出：

**[plain]**[view plain](#)[copy](#)

[print](#)[?](#)

- got exception  

![](http://static.blog.csdn.net/images/save_snippets_01.png)

`got exception`

**[python]**[view plain](#)[copy](#)

[print](#)[?](#)

- def catcher():  
- try:  
-         fetcher(x, 2)  
- except:  
- print"got exception"
- else:  
- print"not exception"

![](http://static.blog.csdn.net/images/save_snippets.png)

```python
def catcher():
    try:
        fetcher(x, 2)
    except:
        print "got exception"
    else:
        print "not exception"
```
输出：

**[plain]**[view plain](#)[copy](#)

[print](#)[?](#)

- not exception  

![](http://static.blog.csdn.net/images/save_snippets_01.png)

`not exception`else作用：没有else语句，当执行完try语句后，无法知道是没有发生异常，还是发生了异常并被处理过了。通过else可以清楚的区分开。

**第四：利用raise传递异常**

**[python]**[view plain](#)[copy](#)

[print](#)[?](#)

- def catcher():  
- try:  
-         fetcher(x, 4)  
- except:  
- print"got exception"
- raise

![](http://static.blog.csdn.net/images/save_snippets.png)

```python
def catcher():
    try:
        fetcher(x, 4)
    except:
        print "got exception"
        raise
```
输出：

**[plain]**[view plain](#)[copy](#)

[print](#)[?](#)

- got exception  
- Traceback (most recent call last):  
-   File "test.py", line 37, in <module>  
-     catcher()  
-   File "test.py", line 22, in catcher  
-     fetcher(x, 4)  
-   File "test.py", line 4, in fetcher  
-     return obj[index]  
- IndexError: string index out of range  

![](http://static.blog.csdn.net/images/save_snippets.png)

```
got exception
Traceback (most recent call last):
  File "test.py", line 37, in <module>
    catcher()
  File "test.py", line 22, in catcher
    fetcher(x, 4)
  File "test.py", line 4, in fetcher
    return obj[index]
IndexError: string index out of range
```
raise语句不包括异常名称或额外资料时，会重新引发当前异常。如果希望捕获处理一个异常，而又不希望

异常在程序代码中消失，可以通过raise重新引发该异常。

**第五：except(name1, name2)**

**[python]**[view plain](#)[copy](#)

[print](#)[?](#)

- def catcher():  
- try:  
-         fetcher(x, 4)  
- except(TypeError, IndexError):  
- print"got exception"
- else:  
- print"not exception"

![](http://static.blog.csdn.net/images/save_snippets.png)

```python
def catcher():
    try:
        fetcher(x, 4)
    except(TypeError, IndexError):
        print "got exception"
    else:
        print "not exception"
```
捕获列表列出的异常，进行处理。若except后无任何参数，则捕获所有异常。

**[python]**[view plain](#)[copy](#)

[print](#)[?](#)

- def catcher():  
- try:  
-         fetcher(x, 4)  
- except:  
- print"got exception"

![](http://static.blog.csdn.net/images/save_snippets.png)

```python
def catcher():
    try:
        fetcher(x, 4)
    except:
        print "got exception"
```


