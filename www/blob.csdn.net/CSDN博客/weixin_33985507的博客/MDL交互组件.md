# MDL交互组件 - weixin_33985507的博客 - CSDN博客
2017年07月01日 10:09:04[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
# 4、MDL交互组件
> 
## [参考代码](https://github.com/whjin/MDL-Pro)
## 1. 徽章/Badge
`徽章/Badge`向用户提供了发现额外信息的视觉线索，它通常是圆型，内容为数字或其他字母，紧贴在宿主元素旁边：
![](http://i.imgur.com/FjDYFKr.png)
徽章可以用来`无侵入`地吸引用户的注意力，例如：
- 
一个`新消息`通知可以使用徽章提醒有几条未读信息
- 
一个`购物车未付款`提醒可以使用徽章提醒购物车内的商品数量
- 
一个`加入讨论！`按钮可以使用徽章提示当前已经加入讨论的用户数
使用`MDL徽章`组件很简单，为宿主元素添加`mdl-badge`样式类，然后在`data-badge`中设置徽章内容：
```
<any class="mdl-badge" data-badge="1">...</any>
```
因为徽章组件的尺寸很小，所以不要放太多内容，通常`data-badge`的值设置为`1~3`个字符。
![](http://i.imgur.com/ib6IIxW.jpg)
## 2. 提示框/Tooltip
当鼠标移动到元素上方时，`提示框/Tooltip`组件可以为界面元素提供额外的信息：
![](http://i.imgur.com/w5iviAP.png)
在`MDL`中，为一个元素添加`Tooltip`的步骤如下：
```
<!--1. 为宿主元素定义一个id -->
<button id="test">TEST</button>
<!--2. 声明一个tooltip组件，使用*for*属性绑定到宿主元素上-->
<div class="mdl-tooltip" for="test">这个按钮没什么用;-(</div>
```
尽管在提示框内可以使用`HTML`片段，但是`Material Design`设计语言不建议在提示框中加入图片等复杂的元素。
![](http://i.imgur.com/3LtbOG7.jpg)
## 3. 按钮/Button
`MDL`的`按钮/Button`组件是`标准HTML`元素`button`的增强版本。按钮组件有多种类型，并且可以添加显示及点击效果：
![](http://i.imgur.com/5sVE71X.png)
`MDL`按钮的显示类型包括：`flat`, `raised`, `fab`, `mini-fab`, `icon`. 这些类型都可以设置为`浅灰或彩色`，也可以禁止。`fab`, `mini-fab`和`icon`类型的按钮通常使用一个小图像而不是文字来表征其功能。
使用按钮组件很简单，为`button`元素声明`mdl-button`、`mdl-js-button`及其他可选的修饰样式类即可：
```
<!--缺省的扁平/flat按钮-->
<button class="mdl-button mdl-js-button">Save</button>
<!--凸起/raised按钮-->
<button class="mdl-button mdl-js-button mdl-button--raised">Save</button>
<!--浮动动作/FAB按钮-->
<button class="mdl-button mdl-js-button mdl-button--fab">Save</button>
<!--迷你浮动动作/MINI-FAB按钮-->
<button class="mdl-button mdl-js-button mdl-button--fab mdl-button--mini-fab">Save</button>
<!--彩色凸起/raised按钮-->
<button class="mdl-button mdl-js-button mdl-button--raised mdl-button--colored">Save</button>
<!--具有点击动效的凸起/raised按钮-->
<button class="mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect">Save</button>
```
![](http://i.imgur.com/whJJT5z.jpg)
## 4. 菜单/Menus
`菜单/menu`组件提供一组选项供用户选择，用户的选择将执行一个动作、变化设置或 其他可以观察到的效果。当需要用户选择时，显示菜单，当用户完成选择时，关闭菜单：
![](http://i.imgur.com/4XYB8om.png)
菜单是成熟然而未标准化的界面组件。
使用`mdl-menu`样式类声明菜单，使用`mdl-menu__item`样式类声明菜单项：
```
<any class="mdl-menu mdl-js-menu">
    <any class="mdl-menu__item">...</any>
    <any class="mdl-menu__item">...</any>
</any>
```
![](http://i.imgur.com/My2BZz9.jpg)
## 5. 滑动条/Sliders
`MDL`的`滑动条/slider`组件是`HTML5`新增元素`range input`的增强版本。 滑动条由一条水平线及其上的可移动滑块构成。当用户移动滑块时，就可以 从预设范围中选择一个值（左边是下界，右边是上界）：
![](http://i.imgur.com/AqFfcwX.png)
使用`MDL`的滑动条组件很简单，为`range input`元素应用样式类`mdl-slider`和`mdl-js-slider`即可：
```
<input type="range" min="0" max="100" class="mdl-slider mdl-js-slider" >
```
使用`range input`元素的`min`和`max`属性来设定值的范围，使用`value`属性来设置滑动条的初始值：
```
<input type="range" min="0" max="100" value="25" class="mdl-slider mdl-js-slider" >
```
![](http://i.imgur.com/B6PFjMo.jpg)
## 6. 复选按钮/Checkbox
`MDL`的`复选按钮/Checkbox`组件是标准`HTML`元素`checkbox input`的增强版本。复选按钮组件包含一个标签和一个开关选择按钮：
![](http://i.imgur.com/CE3JXD5.png)
`MDL`的`复选按钮/Checkbox`组件具有预定义的`HTML`结构：
```
<!--1. 声明组件容器-->
<label class="mdl-checkbox">
    <!--2. 为checkbox input元素应用mdl样式类-->
    <input type="checkbox" class="mdl-checkbox__input"/>
    <!--3. 为标签元素应用mdl样式类-->
    <span class="mdl-checkbox__label">标签</span>
</label>
```
可以使用`checkbox input`元素的`checked`属性设置复选按钮组件的初始选中状态。
![](http://i.imgur.com/o0UVelW.jpg)
## 7. 单选按钮/Radio button
`MDL`的`单选按钮/RadioButton`组件是标准`HTML`元素`radio input`的增强版本。 单选按钮组件`包含一个标签和一个开关选择按钮`：
![](http://i.imgur.com/6Fi8h88.png)
`MDL`的单选按钮组件具有固定的`HTML`结构：
```
<!--1. 声明组件容器-->
<label class="mdl-radio mdl-js-radio">
    <!--2.为input子元素应用mdl样式类-->
    <input type="radio" class="mdl-radio__button" name="options" value="1"/>
    <!--3.为label子元素应用mdl样式类-->
    <span class="mdl-radio__label">选项1</span>
</label>
<!--选项2-->
<label class="mdl-radio mdl-js-radio">
    <input type="radio" class="mdl-radio__button" name="options" value="2"/>
    <span class="mdl-radio__label">选项2</span>
</label>
```
和复选按钮不同，多个同时出现的单选按钮组件，其选中状态是互斥的，任何时刻最多只有一个可以被选中。
和复选按钮类似，使用`radio input`元素的`checked`属性设置单选按钮的选中状态。
![](http://i.imgur.com/LvKuwSu.jpg)
## 8. 图标开关/Icon toggle
`MDL`的`图标开关/IconToggle`组件是标准`HTML`元素`checkbox input`的增强版本。`图标开关组件包含一个标签和一个用户指定的图标按钮`，图标的着色与否用来传达 当前选项是否被选中：
![](http://i.imgur.com/ieZ0bFc.png)
`MDL`的`图标开关/IconToggle`组件具有预定义的`HTML`结构：
```
<!--1. 声明组件容器-->
<label class="mdl-icon-toggle mdl-js-icon-toggle">
    <!--2. 为checkbox input元素应用mdl样式类-->
    <input type="checkbox" class="mdl-icon-toggle__input"/>
    <!--3. 为图标元素应用mdl样式类-->
    <i class="mdl-icon-toggle__label material-icons">format_bold</i>
</label>
```
![](http://i.imgur.com/bl8CEuf.jpg)
## 9. 进度条/Progress bar
`MDL`的`进度条/progress bar`组件用来提供`后台活动的可视化反馈`。进度条是一个水平的长条，可以包含动画以传递工作中的感觉：
![](http://i.imgur.com/B3b07qa.png)
使用**MDL**`进度条/Progress bar`组件很简单：
```
<any class="mdl-progress mdl-js-progress "></any>
```
如果不需要提供给用户进度完成的具体百分比，可以附加一个动画：
```
<any class="mdl-progress mdl-js-progress mdl-progress__indeterminate"></any>
```
如果需要显示进度百分比，需要使用挂接在`DOM对象`上的`MaterialProgress`变量的`setProgress()`方法：
```
var el = document.querySelector("#p1");
//setProgress()方法接受一个0~100的值
el.MaterialProgress.setProgress(80);
```
如果需要同时显示一个`视频流的缓冲及播放情况`，可以使用`MaterialProgress`变量的`setBuffer()`方法，**这个方法将对未缓冲的部分播放一个动画来表达缓冲效果**：
```
var el = document.querySelector("#p1");
//setBuffer()方法接受一个0~100的值
el.MaterialProgress.setBuffer(80);
```
![](https://image-static.segmentfault.com/270/229/2702291039-59573fd5c8118_articlex)
## 10. 等待指示器/Spinner
`MDL`的`等待指示器/spinner`组件是等待图标的增强版本，它使用一个边框色彩动态变化的圆框，清晰地向用户传达作业已经开始、还未完成的状况：
![](https://image-static.segmentfault.com/103/276/1032765826-59573fd4846ab_articlex)
使用`spinner`组件非常简单：
```
<any class="mdl-spinner mdl-js-spinner"></any>
```
`spinner`默认是`隐藏`的，为其应用`is-active`样式进行激活：
```
<any class="mdl-spinner mdl-js-spinner is-active"></any>
```
![](https://image-static.segmentfault.com/317/958/3179583492-59573fd4d4c6b_articlex)
## 11. 文本输入/Text Field
`MDL`的`文本输入/Text Field`组件是对标准`HTMLtext input`元素的封装：
![](https://image-static.segmentfault.com/176/107/1761077457-59573fd494b35_articlex)
`文本输入`组件有特定的`HTML`结构：
```
<!--1.声明组件-->
<div class="mdl-textfield mdl-js-textfield">
    <!--2.声明组件的input元素-->
    <input type="text" class="mdl-textfield__input"/>
    <!--3.声明组件的label元素-->
    <label class="mdl-textfield__label">Your Name</label>
    <!--4.声明组件的error元素-->
    <span class="mdl-textfield__error">Error!</span>
</div>
```
`error`元素默认是隐藏的，用来向用户反馈输入的错误。可以为`input`元素设置`pattern`属性（这是`HTML5`的新特性），当用户的输入与`pattern`指定的正则表达式不符时，将显示`error`元素：
```
<input type="text" pattern="-?[0-9]*(\.[0-9]+)?"/>
```
上面的正则表达式将检测用户的输入是否是一个数值，例如：`-123.456` 。
默认情况下，当用户开始输入时，标签将消失。可以为组件应用`mdl-textfield--floating-label`样式开启浮动标签模式：
```
<!--用户输入时，标签将浮动在输入行上方-->
<div class="mdl-textfield mdl-js-textfield mdl-textfield--floating-label">...</div>
```
也可以将`input`元素换成`textarea`元素，这样将允许多行输入：
```
<div class="mdl-textfield mdl-js-textfield">
    <!--使用rows属性声明行数-->
    <textarea class="mdl-textfield__input" rows="3"></textarea>
    <label class="mdl-textfield__label">Memo</label>
</div>
```
![](https://image-static.segmentfault.com/389/508/3895089547-59573fd6f16a4_articlex)
## 12. 文本输入 - 动态展开式
一种常见的文本输入模式具有一个按钮，点击这个按钮将展开输入框，如果没有输入内容，那么当输入框失去焦点时将自动隐藏：
![](https://image-static.segmentfault.com/807/325/80732512-59573fd52ef02_articlex)
动态展开的文本输入组件有特定的`HTML`结构：
```
<!--1.使用expandable样式类声明动态展开的文本输入组件-->
<div class="mdl-textfield mdl-js-textfield mdl-textfield--expandable">
    <!--2. 声明触发按钮，使用属性for绑定到input元素-->
    <button class="mdl-button mdl-js-button" for="kw_inp">search</button>
    <!--3. 声明文本输入框容器-->
    <div class="mdl-textfield__expandable-holder">
        <!--4.声明input元素，使用属性id声明锚点-->
        <input type="text" class="mdl-textfield__input" id="kw_inp"/>
        <!--5.声明label元素-->
        <label class="mdl-textfield__label">keywords</label>
    </div>
</div>
```
![](https://image-static.segmentfault.com/160/313/1603136945-59573fd5e6437_articlex)
## 13. 数据表/Data table
`MDL`的`数据表/Data table`组件用来`呈现密集的数据集`：
![](https://image-static.segmentfault.com/310/802/3108023448-59573fd59f8aa_articlex)
使用很简单，为`table`元素应用样式即可：
![](https://image-static.segmentfault.com/222/776/2227769435-59573fd630938_articlex)
