# 接口测试 Pytest的简单示例 - zhusongziye的博客 - CSDN博客





2018年04月08日 20:42:44[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：988










### Pytest是什么

Pytest是Python的一个测试工具，可以用于所有类型和级别的软件测试。Pytest是一个可以自动查找到你编写的用例并运行后输出结果的测试框架。




### Pytest有什么特点
- 
pytest是一个命令行工具

- 
pytest可以扩展第三方插件

- 
pytest易于持续集成和应用于web自动化测试

- 
pytest编写用例简单，并具有很强的可读性

- 
pytest可以直接采用assert进行断言，不必采用self.assertEqual()等

- 
pytest可以运行unittest编写的用例

- 
pytest可以运行以test或test开头或结尾的包、文件和方法

- 




### Pytest的简单示例

```
# test_simple.py
import requestsdef test_one():
    r = requests.get('https://api.github.com/events')    
    assert r.status_code == 200
```

运行测试用例可以直接在命令行中执行该py文件`pytest test_simple.py`




![](https://img-blog.csdn.net/20180408204120209?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



### 运行py文件中的单个用例

```
# test_simple.py
import requests

def test_one():
    r = requests.get('https://api.github.com/events')
    assert r.status_code == 200

def test_two():
    r = requests.get('https://api.github.com/events')
    assert r.encoding == 'utf'
```

运行py文件中的单个用例时，可以采用命令`pytest test_simple.py::test_two`




![](https://img-blog.csdn.net/20180408204145717?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




由于断言失败，从结果中可以看到失败的具体原因。







> 
作者： 乐大爷


博客：https://www.jianshu.com/u/39cef8a56bf9

声明：本文转载，著作权归作者所有。








