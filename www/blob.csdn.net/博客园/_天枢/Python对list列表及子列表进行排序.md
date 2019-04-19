# Python对list列表及子列表进行排序 - _天枢 - 博客园
## [Python对list列表及子列表进行排序](https://www.cnblogs.com/yhleng/p/9407763.html)
2018-08-02 16:08 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9407763)
python代码，对list进行升序排序，所有子列表也要进行排序
```
def iterList(listVar):
    listVar = sorted(listVar)
    for i,v in enumerate(listVar):
        if type(v).__name__=="list":
            listVar[i]=list(iterList(sorted(v)))
            return listVar
    return listVar
listk = ['a',1,4,3,8,2,'b',[4,3,2,1,[9,8,7,6]]]
print iterList(listk)
```
运行结果：
```
D:\Python27\python.exe D:/untitled4/tmp6.py
[1, 2, 3, 4, 8, [1, 2, 3, 4, [6, 7, 8, 9]], 'a', 'b']
Process finished with exit code 0
```
