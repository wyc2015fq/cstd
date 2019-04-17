# currentRowChanged 的注意事项 - Grandyang - 博客园







# [currentRowChanged 的注意事项](https://www.cnblogs.com/grandyang/p/4289575.html)







Qt中的表单控件QListWidget类提供了许多信号函数，可以和用户交互，其中有个currentRowChanged ( int currentRow ) 是检测当前选中行是否发生了改变，如果改变了，该信号函数被触发。

```
void QListWidget::currentRowChanged ( int currentRow ) [signal]
```

This signal is emitted whenever the current item changes.
currentRow is the row of the current item. If there is no current item, the currentRow is -1.



我们要注意的是最后一句话，当没有当前项时，currentRow 赋值为-1，由于有这点存在，所以再写该信号函数的内容开始，一定要先判断currentRow的值是否大于等于0，若忽略了这一步，在下面直接用currentRow当做参数取访问数组时会出错，而通常这错误得费老半天劲才能找的出来，正确写法如下：



```
void YourClass::on_lwidget_currentRowChanged(int currentRow) {
    // Note: lwdiget is the object name of listwidget
    if (currentRow >= 0) {
        // Implement here!
    }
}
```














