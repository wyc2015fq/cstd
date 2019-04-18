# arcgis js :图层数据刷新问题 - weixin_33985507的博客 - CSDN博客
2018年05月30日 16:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：86
代码中更新了图层的数据，但是地图显示却没有更新，在拖动了地图后才会更新数据。
解决方法：在更新数据后刷新图层（或重新绘制图层）
featureLayer.refresh();
featureLayer.redraw();
问题聊天记录截图
![9628812-1710af2e0fca3e66.png](https://upload-images.jianshu.io/upload_images/9628812-1710af2e0fca3e66.png)
image.png
![9628812-8995a4acc996248b.png](https://upload-images.jianshu.io/upload_images/9628812-8995a4acc996248b.png)
image.png
参考api:
[https://developers.arcgis.com/javascript/3/jsapi/featurelayer.html#redraw](https://developers.arcgis.com/javascript/3/jsapi/featurelayer.html#redraw)
其他人遇到的类似问题：
[http://zhihu.esrichina.com.cn/question/15409](http://zhihu.esrichina.com.cn/question/15409)
