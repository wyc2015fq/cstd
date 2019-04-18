# JavaScript插件推荐：AsyncBox，轻量级弹窗插件 - z69183787的专栏 - CSDN博客
2014年03月28日 15:02:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3152
[http://example.w3cworld.com/html-page/asyncbox/asyncbox_demo.html](http://example.w3cworld.com/html-page/asyncbox/asyncbox_demo.html)
AsyncBox是一款比较好用的JS弹窗插件，虽然目前官网已经宣布重构改善，但在快速和不复杂的开发中，还是一款比较好用的插件。
AsyncBox（异步盒子）是基于 jQuery 的弹窗插件。能够实现网站的整体风格效果，给用户一个新的视觉享受。主要模拟常用的 alert、confirm、prompt、open 和扩展了一些对话框。它通过回调函数触发事件动作并执行，使操作区域更加明了、统一。而且能够在主流浏览器中灵活运用。
AsyncBox主要有以下特点：
一、AysncBox 集成了 alert、confirm、prompt、open 和扩展了一些对话框；
二、支持静止定位、自定义位置、自动修复位置。极大的灵活性；
三、支持无限极层叠 iframe 框架或 frameset 框架集调用；
四、调用方式简单，API齐全；
五、支持IE6及大多数浏览器；
六、支持皮肤。
由于目前官网已经宣布重构改善，所以AsyncBox目前最新的版本是[2011-9-15] AsyncBox v1.4.5，让我们去看看它最新的更新日志：
> 
- **新增**
　　　1、增加 HTML 模式中显示 DOM 元素内容的支持。
- **调整**
　　　1、修改了拖动算法，减少窗口因为被限制在可视范围内对页面的遮挡。
　　　2、去除了对话框类与内容窗口中 $ 的部分简写支持，以免发生冲突。
　　　3、去掉了对话框类左上角的图标。
　　　4、open 中 url 模式弹出的页面中无需加 top。
- **修复**
　　　1、修复窗口大于浏览器可见范围时看不到标题的 BUG。
　　　2、修复 IE6 下，某些窗口无法遮住 select 的 BUG。
　　　3、修复 url 与 data 参数配合时最终拼接错误的 BUG。
- **更名**
　　参数配置：
　　　1、Fixed 为 open 私有,且更名为 fixed。
　　　2、autoReset 为 open 私有,且更名为 reset。
　　　3、icon 为 open 私有，且更名为 logo。
　　　4、clone 归为 drag ，在 drag : { clone : true } 使用。
　　辅助函数：
　　　1、$.box() 方法更名为 $.opener()。
　　　2、$.asyncbox() 更名为 $.exist()。
　　　3、$.iframe() 更名为 $.framer()。
　　open选项：
　　　1、scrolling 更名为 scroll。
　　　2、closeType 更名为 cache。
可以看出，AysncBox 的功能还是很全，很实用的。虽然目前停止更新，但应用于项目当中可以说是没什么问题的。
[查看示例](http://example.w3cworld.com/2013033130.html)
[ [下载AsyncBox v1.4.5 (2011-9-15)](http://pan.baidu.com/share/link?shareid=105433&uk=3476001392) ]
[ [下载jQuery.AsyncBox v1.4.5 API (2011-9-15)](http://pan.baidu.com/share/link?shareid=350845&uk=3476001392) ]
**常见问题：**
- **可以在项目中使用 AsyncBox 吗？**
> 
可以。在遵循相关协议的条件下，AsyncBox 无偿的提供给大家永久免费使用权。
- **为什么在调用 AsyncBox 时中文会出现乱码？**
> 
AsyncBox 的文件编码是 UTF-8 的，在遇到其他编码类型时可能会出现乱码现象，解决办法可以在引用 AsyncBox 时加上 charset=”utf-8″。例如：<script type=”text/javascript” src=”AsyncBox.js” charset=”utf-8″></script>
- **为什么 AsyncBox 不支持“if(asyncbox.confirm(‘Are you ok?’)){…}else{…}”这种判断方式？**
> 
AsyncBox 对事件的操作是通过回调函数实现的，模拟不了阻塞，所以无法支持这种调用方式。
- **为什么 AsyncBox 不支持“最大化”？**
> 
很遗憾，在历来更新 AsyncBox 中都没有提到支持“最大化”，但是可以通过 width、height 配置一个全屏的窗口，在 resize 中使用 $.size 修复。这样至少可以弥补一下缺陷。另外 AsyncBox 当然不会放弃这个支持，在往后的版本中将看到。
- **AsyncBox 支持 IPAD 移动设备浏览吗？**
> 
目前 AsyncBox 对手机浏览产生的不理想效果没有做相关定制。不建议在 IPAD 移动设备中使用 AsyncBox 集成开发。
- **为什么 AsyncBox 在 windows 7 系统下字体会显得很粗糙？**
> 
系统本身对中文字体的解析就那样。如果想达到更理想的视觉效果，可以了解相关的系统美化资源。
- **为什么 AsyncBox 在内容页中不能第一时间直接调用？**
> 
在内容页或者在 iframe 内中不需要事件触发调用 AsyncBox 时，例如“parent.asyncbox.alert(‘Hello AsyncBox !’);”，这样会报“缺少对象”的错误，原因是内容页未加载完毕。解决方案为：setTimeout(function(){parent.asyncbox.alert(‘Hello AsyncBox !’);},1000); 推荐延迟加载，尽量避免 AsyncBox 和页面同时加载，既等页面加载完毕后再弹出也不迟。
- **为什么在项目中载入 AsyncBox 后窗口布局会变得凌乱？**
> 
AsyncBox 的构建内核使用了 table 和 部分 a 标签，如果你对 table 和 a 标签进行了全局样式设置的话，可能会对 AsyncBox 造成影响。虽然 AsyncBox 也内置固化了部分样式，但总的来说设置全局样式并不是很好的编码习惯，所以建议尽量避免定义全局样式的写法。
- **为什么对话框的内容不能换行？**
> 
避免内容出现连续没有空格的字符串，如 “StringStringStringStringStringStringString…”。
- **为什么 AsyncBox 不能遮住 FLASH？**
> 
FLASH 相对比较特殊，如果需要遮住它的话，由开发人员决定在引用 FLASH 时是否加入 <param name=”WMode” value=”Transparent”> 参数。
- **为什么 AsyncBox 出现靠左上角，不能拖动？**
> 
缺少文档声明<!DOCTYPE html PUBLIC “-//W3C//DTD XHTML 1.0 Transitional//EN” “http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd”>。另外，避免在声明顶部出现 UFO。
- **为什么透明类的皮肤在 IE6 下显示不出来？**
> 
AsyncBox 目前就内置了一款 ZCMS 的透明皮肤，在 IE6 下需要对 asyncbox.css 文件中注释为 /*ie6 filter*/ 下的路径做绝对于当前页面的路径配置。例如：src=’asyncbox/skins/ZCMS/images/ie6/dialog_lt.png’，asyncbox.html && asyncbox文件夹，即文档页面与 asyncbox 文件夹呈同级目录。
- **为什么出现 “asyncbox 未定义”或者其他未定义错误？**
> 
出现此类错误，请检查你配置的插件路径是否正确。
- **为什么按钮按下后变为不可用状态了？**
> 
按钮做了 disabled 处理，简单保证了事件单次执行，避免重复执行。执行完毕后会变为可用状态。如果按钮按下后变为不可用，那说明在执行的过程中出错了，请检查你的代码。
- **为什么会报 “缺少标识符、字符串或数字” 或 “未结束的字符串常量”？**
> 
IE 下在配置 $.open(…) 参数时避免参数配置中缺少逗号或者最后一个参数后面出现逗号。
- **CHM 帮助文档无法显示？**
> 
关于 CHM 帮助文档无法显示问题，由于系统安全性的影响，可能会造成某些 CHM 帮助文档无法显示页面内容。
> 
解决方法：
> 
1）右键文档 – 解除锁定。
> 
2）打开文档时选择“运行”。
> 
如还未能解决就百度或谷歌了。
- **AsyncBox 关闭不了或关闭不“完整”？**
> 
检查是否存在相同的ID。
**操作流程遇到的问题：**
- **如何取得当前 AsyncBox 窗口ID？**
> 
AsyncBox 的回调函数处理机制为 callback 在 callback 中可以使用 this.id 得到当前 AsyncBox 的 ID。如果该 AsyncBox 被嵌套使用，那么 this 对象会被子 AsyncBox 覆盖，此时需得到正确的 ID ，则需将 this 对象存起来，例如：var id = this.id; 再用。 另外对话框类窗口ID为 asyncbox_ + [alert,confirm,success,error,prompt]。
- **如何在 AsyncBox 窗口中提交表单？**
> 
可以用自带的按钮操作子页面，也可以在子页面中自己新建按钮，但记住的是：页面刷新的话就调用不了部分JS了。
- **操作 AsyncBox 后如何刷新页面？**
> 
在判断 action 值内加入需要刷新页面的代码“window.location.reload()”。内容页刷新父页面：“top.window.location.reload()”。
- **如何在程序后台中调用 AsyncBox？**
> 
通过 AJAX。
官网：[http://asyncui.com](http://asyncui.com/)
```
```
