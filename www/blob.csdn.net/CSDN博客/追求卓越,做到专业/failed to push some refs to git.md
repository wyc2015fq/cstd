
# failed to push some refs to git - 追求卓越,做到专业 - CSDN博客


2019年04月08日 22:47:42[Waldenz](https://me.csdn.net/enter89)阅读数：4标签：[failed to push some refs to																](https://so.csdn.net/so/search/s.do?q=failed to push some refs to&t=blog)[git																](https://so.csdn.net/so/search/s.do?q=git&t=blog)[git push - u origin master																](https://so.csdn.net/so/search/s.do?q=git push - u origin master&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=git&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=failed to push some refs to&t=blog)个人分类：[Git																](https://blog.csdn.net/enter89/article/category/8837998)
[
																								](https://so.csdn.net/so/search/s.do?q=failed to push some refs to&t=blog)


很多人在push到github时,执行下列命令，可能会出错，
```python
git push -u origin master
```
出错信息：
error: failed to push some refs to 'git@github.com:XXXX/project.git'
![](https://img-blog.csdnimg.cn/20190408190521849.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
出现错误的原因是github中的README.md文件不在本地代码目录中
可以通过命令进行代码合并【pull = fetch + merge】
```python
git pull --rebase origin master
```
执行上面代码后可以看到本地代码库中多了README.md文件
此时再执行git push - u origin master即可完成代码上传到github
![](https://img-blog.csdnimg.cn/20190408224853826.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
命令解释：
git push origin master ：本地master分支的最新修改推送至GitHub
git push -u origin master：第一次推送master分支时，加上了-u参数，把本地的master分支内容推送的远程新的master分支，还会把本地的master分支和远程的master分支关联起来，在以后的推送或者拉取时就可以简化命令。


