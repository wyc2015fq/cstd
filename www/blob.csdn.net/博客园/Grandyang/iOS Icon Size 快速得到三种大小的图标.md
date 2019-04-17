# iOS Icon Size 快速得到三种大小的图标 - Grandyang - 博客园







# [iOS Icon Size 快速得到三种大小的图标](https://www.cnblogs.com/grandyang/p/8685539.html)







在iOS开发中，按钮图标可以放三种大小，常见的是22x22，44x44，66x66，一般来说我们可以在PS中做好图片后，再分别导出三种大小的图标，但是每次要修改图片的大小，操作比较繁琐。这里博主推荐一种使用命令行来快速对图片的大小进行转换，比如我们用PS做好了图以后，先导出66x66大小的图片，命名为img@3x.png，好，下面我们用命令行来生成一倍和二倍的图片。



这里用到了imagemagick这个库，可以使用homebrew来快速安装：

```
brew install imagemagick
```



安装好了以后，我们就可以使用如下命令来快速的生成 img@2x.png 和 img@1x.png

```
convert -resize '66.6666666%!' img@3x.png img@2x.png

convert -resize '33.3333333%!' img@3x.png img@1x.png
```



想生成其他大小的图片，只需要修改百分比就行了，注意，最好是大图生成小图，不要翻过来，否则分辨率太差啦～












