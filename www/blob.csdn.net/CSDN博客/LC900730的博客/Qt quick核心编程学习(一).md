# Qt quick核心编程学习(一) - LC900730的博客 - CSDN博客
2017年08月20日 16:35:38[lc900730](https://me.csdn.net/LC900730)阅读数：674
## 第三章
## QML支持ECMAScript表达式
```
BUtton{
        text:"Quit";
        style:ButtonStyle{
            background:Rectangle{
                implicitWidth:70;
                implicitHeight:25;
                border.width:control.activeFocus?2:1;
            }
        }
    }
//定制了一个按钮风格，指定一个Rectangle对象作为按钮背景。
在按钮有焦点时候边框宽度为2，没有焦点的时候宽度为1.
control.activeFocus则是引用了其他对象和属性。当使用这个，待赋值的属性就和你所引用的对象那个属性建立了管理，当被引用的属性发生变化，表达式的值就会重新计算，而待赋值的属性也会变化。
```
### 如何引用对象
通过对象的id值。 
id属性：值必须是小写字母或下划线，并且不能包含字母、数字、下划线以外的字符。
### 列表属性
QML对象中列表属类似于下面这样 
Item{ 
    children:[ 
            Image{}, 
            Text{} 
    ] 
} 
列表是包含在方括号中，以逗号分隔的多个元素的集合 
。其实列表和ECMAScript的数组是类似的，器访问方式也一样。 
- 可以用[value1,value2,…valueN]这种形式给list对象赋值。
- length属性提供了列表内元素的个数
- 列表内的元素通过数组下标来访问
## 信号处理器
信号处理器其实等价于Qt中的槽。 
名字一般是on`<Signal>`
```
Rectangle{
        width:320;
        height:480;
    Button{
        id:quit;
        text:"退出";
        anchors.left:parent.left;
        onClicked:{
            Qt.quit();
        }
    }
}
//当信号是clicked(),信号处理器就是onClicked。
```
### 分组属性
```
Text{
    font.pixelSize:18;
    font.bold:true;
}
//更贴切的分组写法如下
Text{
    font {
        pixelSize:12;
        bold:true;
    }
}
//即font属性的类型本身也是一个对象，这个对象又有pixelSize、bold、italic、underline属性。
```
### 附加属性
附加到一个对象的额外属性
```
Item{
        width:320;
        height:480;
        focus:true;
        Keys.enabled:false;
        //Keys就是Qt Quick提供的供Item处理按键事件的附加属性。
}
```
