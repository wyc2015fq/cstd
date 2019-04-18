# Swift-UIView扩展CGRect - weixin_33985507的博客 - CSDN博客
2017年01月20日 22:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
UIView设置坐标位置经常会遇到，正常的姿势是先获取原有的frame修改其中的某个坐标值，然后将新的frame赋值给UIView，其实很简单可以扩展UIView，一步到位，对比代码如下:
<pre><code>`        var frame:CGRect = self.view.frame
frame.size.width = 100
self.view.frame = frame
```
self.view.width = 100`</code></pre>
```
扩展代码如下:
<pre><code>`extension UIView { // FlyElephant
```
// x
var x:CGFloat {
    get {
        return frame.origin.x
    }
    set(newValue) {
        var tempFrame:CGRect = frame
        tempFrame.origin.x = newValue
        frame = tempFrame
    }
}
// y
var y:CGFloat {
    get {
        return frame.origin.x
    }
    set(newValue) {
        var tempFrame:CGRect = frame
        tempFrame.origin.y = newValue
        frame = tempFrame
    }
}
// top
var top:CGFloat {
    get {
        return frame.minY
    }
    set(newValue) {
        var tempFrame:CGRect = frame
        tempFrame.origin.y = newValue
        frame = tempFrame
    }
}
// bottom
var bottom:CGFloat {
    get {
        return frame.maxY
    }
    set(newValue) {
        var tempFrame:CGRect = frame
        tempFrame.origin.y = newValue
        frame = tempFrame
    }
}
// left
var left:CGFloat {
    get {
        return frame.minX
    }
    set(newValue) {
        var tempFrame:CGRect = frame
        tempFrame.origin.x = newValue
        frame = tempFrame
    }
}
// right
var right:CGFloat {
    get {
        return frame.maxX
    }
    set(newValue) {
        var tempFrame:CGRect = frame
        tempFrame.origin.x = newValue
        frame = tempFrame
    }
}
// CenterX
var centerX:CGFloat {
    get {
        return frame.midX
    }
    set(newValue) {
        var tempFrame:CGRect = frame
        tempFrame.origin.x = newValue - frame.size.width/2
        frame = tempFrame
    }
}
// CenterY
var centerY:CGFloat {
    get {
        return frame.minY
    }
    set(newValue) {
        var tempFrame:CGRect = frame
        tempFrame.origin.y = newValue - frame.size.height/2
        frame = tempFrame
    }
}
// Width
var width:CGFloat {
    get {
        return frame.size.width
    }
    set(newValue) {
        var tempFrame:CGRect = frame
        tempFrame.size.width = newValue
        frame = tempFrame
    }
}
// Height
var height:CGFloat {
    get {
        return frame.size.height
    }
    set(newValue) {
        var tempFrame:CGRect = frame
        tempFrame.size.height = newValue
        frame = tempFrame
    }
}
```
}`</code></pre>
