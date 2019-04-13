
# shell if else - 嵌入式Linux - CSDN博客

2018年11月20日 14:09:37[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：32个人分类：[Shell																](https://blog.csdn.net/weiqifa0/article/category/2102961)



```python
if ["$IMAGE_FOR_FACTORY_TEST" = true];then                                                     
  echo "ro.xl.factoryrebootcheck=true"
else
  echo "ro.xl.factoryrebootcheck=false"
fi
```
注意**if**前面有个空格 才到**[**

