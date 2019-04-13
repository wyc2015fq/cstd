
# Docker删除大量停止的container - sinat_33731745的博客 - CSDN博客

2018年02月10日 16:42:58[Tao-Tao-Tao](https://me.csdn.net/sinat_33731745)阅读数：1067


## 1. 怎么做
官方建议的批量删除停止容器使用docker rm $(sudo docker ps -a -q)
千万不要用 docker rm -f $(sudo docker ps -a -q),会删除全部容器的
## 2. 为什么这么做
### 1. docker ps -a -q
docker ps 命令的解释：
`docker ps -a -q 列出所有容器的数字ID`
```python
root@haha:~# docker ps --help
Usage:  docker ps [OPTIONS]
List containers
Options:
  -
```
```python
a, --all
```
```python
Show
```
```python
all containers (default shows just running)
```
```python
-
```
```python
f, --filter value
```
```python
Filter
```
```python
output based on conditions provided (default [])
```
```python
-
```
```python
-format string
```
```python
Pretty
```
```python
-print containers using a
```
```python
Go
```
```python
template
```
```python
-
```
```python
-help
```
```python
Print
```
```python
usage
```
```python
-
```
```python
n, --last int
```
```python
Show
```
```python
n last created containers (includes all states) (default -
```
```python
1
```
```python
)
```
```python
-
```
```python
l, --latest
```
```python
Show
```
```python
the latest created container (includes all states)
```
```python
-
```
```python
-no-trunc
```
```python
Don
```
```python
‘t truncate output
```
```python
-
```
```python
q, --quiet
```
```python
Only
```
```python
display numeric
```
```python
IDs
```
```python
-
```
```python
s, --size
```
```python
Display
```
```python
total file sizes
```
具体看看，docker ps 是列出容器的命令
`- a 列出所有的容器
- q 只显示数字ID`
### 2. docker rm命令的解释：
```python
root@haha:~# docker rm --help
Usage:  docker rm [OPTIONS] CONTAINER [CONTAINER...]
Remove one or more containers
Options:
  -
```
```python
f, --force
```
```python
Force
```
```python
the removal of a running container (uses
```
```python
SIGKILL
```
```python
)
```
```python
-
```
```python
-help
```
```python
Print
```
```python
usage
```
```python
-
```
```python
l, --link
```
```python
Remove
```
```python
the specified link
```
```python
-
```
```python
v, --volumes
```
```python
Remove
```
```python
the volumes associated with the container
```
`- f 强制删除，可以删除正在运行的容器
- v 容器启动后，数据会以volumes的形式存在于硬盘中，即使删除了container数据也不会删除，加上这个参数那么容器执行的数据也会被删除`

