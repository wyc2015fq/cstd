# 上传本地代码及更新代码到GitHub教程 - 站在巨人的肩膀上coding - CSDN博客





2018年06月06日 23:06:36[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：488









# 上传本地代码及更新代码到GitHub教程

## 上传本地代码

## 第一步：去github上创建自己的Repository，名称和自己本地仓库的一致，创建页面如下图所示：

![](https://images2017.cnblogs.com/blog/1249529/201710/1249529-20171025111734035-171097772.png)

## 红框为新建的仓库的https地址

![](https://images2017.cnblogs.com/blog/1249529/201710/1249529-20171025111740348-1861677831.png)

## 第二步：
`echo "# Test" >> README.md`
## 第三步：建立git仓库

```
git init
```

## 第四步：将项目的所有文件添加到仓库中

```
git add .
```

## 第五步：

```
git add README.md
```

## 第六步：提交到仓库

```
git commit -m "注释语句"
```

## 第七步：将本地的仓库关联到GitHub，后面的https改成刚刚自己的地址，上面的红框处

```
git remote add origin git@github.com:zuwuwang/qt_ros_ws.git
```

## 第八步：上传github之前pull一下

```
git pull origin master
```

## 第九步：上传代码到GitHub远程仓库

```
git push -u origin master
```

## 中间可能会让你输入Username和Password，你只要输入github的账号和密码就行了。执行完后，如果没有异常，等待执行完就上传成功了。

## 更新代码

## 第一步：查看当前的git仓库状态，可以使用git status

```
git status
```

## 第二步：更新全部

```
git add *
```

## 第三步：接着输入git commit -m "更新说明"

```
git commit -m "更新说明"
```

## 第四步：先git pull,拉取当前分支最新代码

```
git pull
```

## 第五步：push到远程master分支上

```
git push origin master
```

## 不出意外，打开GitHub已经同步了




