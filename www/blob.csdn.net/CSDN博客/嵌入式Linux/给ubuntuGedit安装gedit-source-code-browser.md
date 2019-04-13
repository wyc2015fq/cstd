
# 给ubuntuGedit安装gedit-source-code-browser - 嵌入式Linux - CSDN博客

2015年11月26日 10:31:42[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1281


[插件链接下载](https://github.com/weiqifa0/gedit-source-code-browser)
[https://bugs.launchpad.net/ubuntu/+source/gedit-source-code-browser-plugin/+bug/1242126](https://bugs.launchpad.net/ubuntu/+source/gedit-source-code-browser-plugin/+bug/1242126)
下载这个安装完后还用不了，还要添加下面的补丁
```python
diff -Naur old-plugins/sourcecodebrowser/ctags.py plugins/sourcecodebrowser/ctags.py
```
```python
--- old-plugins/sourcecodebrowser/ctags.py  2013-11-06 14:02:44.771946459 -0800
```
```python
+++ plugins/sourcecodebrowser/ctags.py  2013-11-06 14:07:08.623943494 -0800
```
```python
@@ -88,7 +88,7 @@
```
```python
#args = [arg.replace('%20', ' ') for arg in shlex.split(command)] 
         args = shlex.split(command)
         p = subprocess.Popen(args, 0, shell=False, stdout=subprocess.PIPE, executable=executable)
```
```python
-        symbols = self._parse_text(p.communicate()[0])
```
```python
+        symbols = self._parse_text(p.communicate()[0].decode("utf-8"))
```
```python
def _parse_text(self, text):
         """
diff -Naur old-plugins/sourcecodebrowser/__init__.py plugins/sourcecodebrowser/__init__.py
```
```python
--- old-plugins/sourcecodebrowser/__init__.py   2013-11-06 14:02:44.771946459 -0800
```
```python
+++ plugins/sourcecodebrowser/__init__.py   2013-11-06 14:05:53.023944343 -0800
```
```python
@@ -1,3 +1,11 @@
```
```python
+import sys, os
```
```python
+
```
```python
+path = os.path.dirname(__file__)
```
```python
+
```
```python
+if not path in sys.path:
```
```python
+    sys.path.insert(0, path)
```
```python
+
```
```python
import plugin
 from plugin import SourceCodeBrowserPlugin
```
```python
+
```
```python
diff -Naur old-plugins/sourcecodebrowser.plugin plugins/sourcecodebrowser.plugin
```
```python
--- old-plugins/sourcecodebrowser.plugin    2013-11-06 14:02:44.843946458 -0800
```
```python
+++ plugins/sourcecodebrowser.plugin    2013-11-06 14:04:07.679945527 -0800
```
```python
@@ -1,5 +1,5 @@
```
```python
[Plugin]
```
```python
-Loader=python
```
```python
+Loader=python3
```
```python
Module=sourcecodebrowser
 IAge=3
 Name=Source Code Browser
```

