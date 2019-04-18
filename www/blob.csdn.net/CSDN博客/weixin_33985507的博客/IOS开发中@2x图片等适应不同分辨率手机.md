# IOS开发中@2x图片等适应不同分辨率手机 - weixin_33985507的博客 - CSDN博客
2015年08月29日 10:39:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
开发中，例如： nanshanImage.image=[UIImage imageNamed:@'index_pic.png'];
在项目中还保存中index_pic@2x.png的图片，此图为了只适应960*640的分辨率手机
注意：适应Iphone5的相关图片尺寸，也要加@2x.png。比如欢迎界面：Default-568h@2x.png；这个才能匹配上。
而index_pic.png是适应640*320的手机，但是在代码中必须写成[UIImage imageNamed:@'index_pic.png'];这样系统自动会根据不同分辨率手机决定是否调用index_pic@2x.png还是index_pic.png，会自动根据分辨率来匹配用哪张图。
可是如果写成[UIImage imageNamed:@'index_pic@2x.png'];那么系统无法自动判断，根据640*320分辨率来调用index_pic.png图片，系统只会将index_pic@2x.png这一张图当做某控件的图片，不会自动寻找匹配。
因此，开发中[UIImage imageNamed:@'index_pic.png'];里面的图片都是用小分辨率格式名的图片，这样会自动适应大分辨率图片，直接调用。
UIImage类处理了所有需要加载高分辨率图片到应用中的工作。当创建一个新的image对象后，只要以同样的名称来同时请求图片的标准版和高分辨率版即可。例如，如果有两张图片文件，名为Button.png和Button@2x.png,只要用以下代码请求图片即可:
UIImage* anImage = [UIImage imageNamed:@'Button'];
注意，在IOS4及以后的版本，可以省略文件扩展名而只提供图片名称即可。
在高分辨率显示屏下， imageNamed:, imageWithContentsOfFile:, 和initWithContentsOfFile: 方法自动查找具有@2x名称的图片（注意：如果@2x.png图片与.png图片必须放在项目同一包下，才能自动查找）。如果找到，则会加载该图片。如果没有提供给定图片的高分辨率版本，image对象仍会加载标准版的图片资源，并在绘画过程中将其缩放。如果有提供给定图片的高分辨率版本，但是没有标准版的图片，在标准版显示屏下不会加载高清的图片。
说明：1，Create folder references for any added folders
创建引入的是蓝色的文件夹结构，物理路径。 利用imageWithContentsOfFile获取图片，@2x.png与.png必须在同一蓝色目录下。
2，Create groups for any added folders
导入文件的时候会是逻辑结构，而不是物理结构，目录结构图标如下，黄色的文件夹结构，和构建的Group是一样的，@2x.png与.png可在黄色不同的文件夹下（因为没有物理路径，引用的图片路径其实在同一路径下）。
注意：xib中选择图片做背景时，也是一样的原理！也是选择不带@2x的，这样会自带根据分辨率匹配是否使用小图还是@2x大图！
赏
开发中，例如： nanshanImage.image=[UIImage imageNamed:@'index_pic.png'];
在项目中还保存中index_pic@2x.png的图片，此图为了只适应960*640的分辨率手机
注意：适应Iphone5的相关图片尺寸，也要加@2x.png。比如欢迎界面：Default-568h@2x.png；这个才能匹配上。
而index_pic.png是适应640*320的手机，但是在代码中必须写成[UIImage imageNamed:@'index_pic.png'];这样系统自动会根据不同分辨率手机决定是否调用index_pic@2x.png还是index_pic.png，会自动根据分辨率来匹配用哪张图。
可是如果写成[UIImage imageNamed:@'index_pic@2x.png'];那么系统无法自动判断，根据640*320分辨率来调用index_pic.png图片，系统只会将index_pic@2x.png这一张图当做某控件的图片，不会自动寻找匹配。
因此，开发中[UIImage imageNamed:@'index_pic.png'];里面的图片都是用小分辨率格式名的图片，这样会自动适应大分辨率图片，直接调用。
UIImage类处理了所有需要加载高分辨率图片到应用中的工作。当创建一个新的image对象后，只要以同样的名称来同时请求图片的标准版和高分辨率版即可。例如，如果有两张图片文件，名为Button.png和Button@2x.png,只要用以下代码请求图片即可:
UIImage* anImage = [UIImage imageNamed:@'Button'];
注意，在IOS4及以后的版本，可以省略文件扩展名而只提供图片名称即可。
在高分辨率显示屏下， imageNamed:, imageWithContentsOfFile:, 和initWithContentsOfFile: 方法自动查找具有@2x名称的图片（注意：如果@2x.png图片与.png图片必须放在项目同一包下，才能自动查找）。如果找到，则会加载该图片。如果没有提供给定图片的高分辨率版本，image对象仍会加载标准版的图片资源，并在绘画过程中将其缩放。如果有提供给定图片的高分辨率版本，但是没有标准版的图片，在标准版显示屏下不会加载高清的图片。
说明：1，Create folder references for any added folders
创建引入的是蓝色的文件夹结构，物理路径。 利用imageWithContentsOfFile获取图片，@2x.png与.png必须在同一蓝色目录下。
2，Create groups for any added folders
导入文件的时候会是逻辑结构，而不是物理结构，目录结构图标如下，黄色的文件夹结构，和构建的Group是一样的，@2x.png与.png可在黄色不同的文件夹下（因为没有物理路径，引用的图片路径其实在同一路径下）。
注意：xib中选择图片做背景时，也是一样的原理！也是选择不带@2x的，这样会自带根据分辨率匹配是否使用小图还是@2x大图！
赏
