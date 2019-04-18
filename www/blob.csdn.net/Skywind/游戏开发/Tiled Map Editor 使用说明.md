# Skywind Inside » Tiled Map Editor 使用说明
## Tiled Map Editor 使用说明
April 14th, 2011[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
1. 首先下载 Tiled Map Editor：
地图编辑器：[http://www.skywind.me/mw/images/8/81/TiledMapEditor.7z](http://www.skywind.me/mw/images/8/81/TiledMapEditor.7z)
资源图片集：[http://www.skywind.me/mw/images/0/09/Server_in_12h_images.7z](http://www.skywind.me/mw/images/0/09/Server_in_12h_images.7z)
解压，然后下载资源图片解压，保存在examples目录下面
（点击Read more 阅读全文）
我们最终将编辑下图的地图，并用Python读出来：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb1.png)
2. 图块命名如下：
|![Tile01](http://www.skywind.me/blog/wp-content/uploads/2011/04/Tile01_thumb.png)|![Tile02](http://www.skywind.me/blog/wp-content/uploads/2011/04/Tile02_thumb.png)|![Tile04](http://www.skywind.me/blog/wp-content/uploads/2011/04/Tile04_thumb.png)|
|----|----|----|
|tile01.png|tile02.png|tile03.png|
3. 运行 tiled.exe，选择：文件->新文件：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb2.png)
按照这个对话框填写，然后“确定”
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb3.png)
4. 打开资源管理器将文件“Tile01.png” 拖入上图右下角“图块”面板：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb4.png)
设置宽度，高度为32，边距，间距为0，然后确定。
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb5.png)
这时tile01.png已经被按照刚才的规则切割好了，放在右下角的面板中。点击工具栏上的“图章刷”或者按键盘B就可以进入刷子绘制模式了，然后从右下角选择你需要绘制的图块，就可以在中间区域绘制了。
如法炮制，我们再将tile02.png拖入到右下角的图块面板：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb6.png)
右下角就有两个图块选项卡了，一个代表刚才的tile01.png, 一个是现在的tile02.png。接着选择一个草的图块，并在工具栏上选择“填充”（或者按F），将背景全部刷成草地：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb7.png)
5. 图块一次性可以选择很多块（用鼠标拖着，我们先简单的画一些沙子地形上去，还有池塘：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb8.png)
接着到主菜单“图层”选择“添加块层”，右上角的“图层”面板就多了一个图层2：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb9.png)
我们选中“块层2”以后就可以在这个图层上编辑了，不会影响到刚才的图层，接着在这个图层上添加树木和房屋，拖入tile3.png以后在“图块”面板拖动鼠标整体选择房屋图块：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb10.png)
注意房屋图块在tile03的右下角，需要再“图块”面板的“tile03”这个标签下，将滚动条拖到右下，接着在”块层2”上建房顶：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb11.png)
在右上角选择块层1，然后右下角选择对应图块给房子建墙：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb12.png)
然后接着编辑“块层2”，在墙上绘制门窗，然后同样的方法绘制另外一间木屋：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb13.png)
6. 继续绘制地图，添加希望添加的内容：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb14.png)
然后新建一个图块层4，用来表示可以走不可以走的地方，随便选一个颜色明显的图块，在图块层4上表示不能走的区域：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb15.png)
编辑好不可走的区域后，如果不想它影响画面，可以将右上“图块层4”前面的勾去掉，就不显示了。
7. 最终我们要导出成python可以识别的格式：选择主菜单->编辑->参数：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb16.png)
将“另存为数据层”选择为“CSV”，然后关闭。
这时候用“文件”-“保存”功能保存成“game1.tmx”
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb17.png)
8. 用文本编辑器，打开game1.tmx文件，可以看到如下内容：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb18.png)
呵呵，一看就明白，就是CSV，前面记录了右下角“图块”的资源文件以及切割方式，ColorKey等，后面每个图层都用CSV格式来记录了里面的数据，0代表没有，1代表tile01.png的左上角第一个图块，2代表tile01.png左上角左数第二个图块：
![image](http://www.skywind.me/blog/wp-content/uploads/2011/04/image_thumb19.png)
tile01被按照32×32的大小切割成了20×28个小图块。按照行优先存储，在tile01左上角的第一个图块编号为1，往右一个图块是2，再往右就是3，第二行左数第一个图块就是编号21。
在game1.tmx的如下部分：
<tilesetfirstgid="1"name="Tile01"tilewidth="32"tileheight="32"><imagesource="Tile01.PNG"trans="ff00ff"width="640"height="896"/></tileset><tilesetfirstgid="561"name="Tile02"tilewidth="32"tileheight="32"><imagesource="Tile02.PNG"trans="ff00ff"width="640"height="960"/></tileset><tilesetfirstgid="1161"name="Tile03"tilewidth="32"tileheight="32"><imagesource="Tile03.PNG"trans="ff00ff"width="640"height="480"/></tileset>
其中 firstgid代表这个资源文件的左上角第一个图块的开始编号，如Tile02图块，它左上角第一个图块编号就是561.非常清爽明白的定义。
好了，有了这些，就可以在python中方便的解析了。
