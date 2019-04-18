# vue-draggable-resizable 可拖拽缩放的组件 - weixin_33985507的博客 - CSDN博客
2018年09月18日 19:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：92
> 
Vue 用于可调整大小和可拖动元素的组件并支持组件之间的冲突检测与组件对齐
### 前言
> 
17年就应用此组件到了项目中，当时正式版的功能不能满足项目需求，还拉取了`dev`分支的测试版进行了简单的更改。(项目中主要功能之一需要用到此组件)
今年因为需求变更，项目重构(手动泪奔)，然后去看了看`github`,该组件的正式版本更新到了`1.7.x`，于是把正式版拉下来根据自己的需求进行了修改并发布新版到npm上。
### 特征
- 没有依赖
- 可拖动，可调整大小或者两者都行
- 拥有用于调整大小的控制点
- 限制组件调整大小和移动超出父元素
- 自定义网格移动
- 将拖动限制为垂直或水平移动
**新增特征**
- 组件之间的冲突检测（不允许组件之间重叠）
- 组件与组件之间进行对齐（类似于吸附效果）
### 项目地址
原组件地址：[vue-draggable-resizable](https://github.com/mauricius/vue-draggable-resizable)
新组件地址：[vue-draggable-resizable](https://github.com/gorkys/vue-draggable-resizable)
### 安装使用
```
npm install --save vue-draggable-resizable-gorkys
```
[更多API请在项目说明文档中查看](https://github.com/gorkys/vue-draggable-resizable)
### Demo
[Demo](https://mauricius.github.io/vue-draggable-resizable/)
## 修改过程记录
#### 提出建议
![12792466-74e8054096e093a0.png](https://upload-images.jianshu.io/upload_images/12792466-74e8054096e093a0.png)
github
> 
在原组件的`Issues`中提出了建议，作者表示不打算让此组件进行跨越组件之外的操作。
好吧，既然作者不打算进行这方面的支持，那只好自己动手了。
#### Clone项目
`Fork`项目到自己的仓库，然后`Clone`项目到本地进行修改。
#### 需求说明
1.组件之间的冲突检测
> 
两个组件不允许重叠，如果重叠，将回到移动或缩放前位置
2.组件与组件之间进行对齐（[参照Jquery UI的`draggable`](http://www.runoob.com/try/try.php?filename=jqueryui-example-draggable-snap-to)）
> 
用户移动一个组件到另一个组件边缘的时候，进行对齐操作。类似于吸附效果
#### 代码修改
**1.组件之间的冲突检测**
首先是组件之间的冲突检测，组件与组件的边界检测需要一个`标记`进行判断。
> 
先在`props`中加入一个`isConflictCheck`，让使用者自己选择是否使用此功能。
```
props:{
    /* 定义组件是否开启冲突检测 */
    isConflictCheck: {
      type: Boolean, default: false
    }
    ...
}
```
> 
当我们拿到`isConflictCheck`后，在`setConflictCheck`方法中给组件的`Dom`设置一个`data-*`的属性。
```
setConflictCheck: function () {
      if (this.isConflictCheck) {
        this.$el.setAttribute('data-is-check', 'true')
      } else {
        this.$el.setAttribute('data-is-check', 'false')
      }
    }
```
> 
然后就是如何去检测组件之间的冲突，代码如下，此代码是在测试版本中使用的，看到这些判断都可怕，为了头发，就没有去优化了(反正能使用)。
```
conflictCheck: function () {
      if (this.isConflictCheck) {
        let p = this.$el.parentNode.childNodes // 获取当前父节点下所有子节点
        if (p.length > 1) {
          for (let i = 0; i < p.length; i++) {
            if (p[i] !== this.$el && p[i].className !== undefined && p[i].getAttribute('data-is-check') !== 'false') {
              let tw = p[i].offsetWidth
              let th = p[i].offsetHeight
              let tl = p[i].offsetLeft
              let tt = p[i].offsetTop
              // 如果冲突，就将回退到移动前的位置
              if (this.top >= tt && this.left >= tl && tt + th > this.top && tl + tw > this.left ||
                this.top <= tt && this.left < tl && this.top + this.height > tt && this.left + this.width > tl) { /* 左上角与右下角重叠 */
                this.top = this.restoreY
                this.left = this.restoreX
                this.width = this.restoreW
                this.height = this.restoreH
              } else if (this.left <= tl && this.top >= tt && this.left + this.width > tl && this.top < tt + th ||
                this.top < tt && this.left > tl && this.top + this.height > tt && this.left < tl + tw) { /* 右上角与左下角重叠 */
                this.top = this.restoreY
                this.left = this.restoreX
                this.width = this.restoreW
                this.height = this.restoreH
              } else if (this.top < tt && this.left <= tl && this.top + this.height > tt && this.left + this.width > tl ||
                this.top > tt && this.left >= tl && this.top < tt + th && this.left < tl + tw) { /* 下边与上边重叠 */
                this.top = this.restoreY
                this.left = this.restoreX
                this.width = this.restoreW
                this.height = this.restoreH
              } else if (this.top <= tt && this.left >= tl && this.top + this.height > tt && this.left < tl + tw ||
                this.top >= tt && this.left <= tl && this.top < tt + th && this.left > tl + tw) { /* 上边与下边重叠（宽度不一样） */
                this.top = this.restoreY
                this.left = this.restoreX
                this.width = this.restoreW
                this.height = this.restoreH
              } else if (this.left >= tl && this.top >= tt && this.left < tl + tw && this.top < tt + th ||
                this.top > tt && this.left <= tl && this.left + this.width > tl && this.top < tt + th) { /* 左边与右边重叠 */
                this.top = this.restoreY
                this.left = this.restoreX
                this.width = this.restoreW
                this.height = this.restoreH
              } else if (this.top <= tt && this.left >= tl && this.top + this.height > tt && this.left < tl + tw ||
                this.top >= tt && this.left <= tl && this.top < tt + th && this.left + this.width > tl) { /* 左边与右边重叠（高度不一样） */
                this.top = this.restoreY
                this.left = this.restoreX
                this.width = this.restoreW
                this.height = this.restoreH
              }
            }
          }
        }
      }
    }, // 冲突检测
```
> 
最后就是在停止移动和缩放时调用上面的方法就可以了（代码精简过）。
```
handleUp: function (e) {
      this.handle = null
      if (this.resizing) {
        this.resizing = false
        this.conflictCheck() // 冲突检测
      }
      if (this.dragging) {
        this.dragging = false
        this.conflictCheck() // 冲突检测
      }
    } // 鼠标松开
```
**2.组件与组件之间进行对齐**
与冲突检测一样的套路。
> 
先在`props`中加入一个`snap`，让使用者自己选择是否使用此功能。为了更灵活，这里多添加了一个`snapTolerance`，当调用对齐时，用来设置组件与组件之间的对齐距离，以像素为单位。
```
/* 是否开启元素对齐 */
    snap: {
      type: Boolean, default: false
    },
    /* 当调用对齐时，用来设置组件与组件之间的对齐距离，以像素为单位。 */
    snapTolerance: {
      type: Number,
      default: 5,
      validator: function (val) {
        return typeof val === 'number'
      }
```
> 
然后就是设置`data-*`属性
```
setSnap: function () {
      if (this.snap) {
        this.$el.setAttribute('data-is-snap', 'true')
      } else {
        this.$el.setAttribute('data-is-snap', 'false')
      }
    }, // 设置对齐元素
```
> 
再然后就是主要方法`snapCheck`的编写。这里我翻看了一下JQuery UI中的`draggable`源码，并近乎copy的借鉴了过来。
```
snapCheck: function () {
      if (this.snap) {
        let p = this.$el.parentNode.childNodes // 获取当前父节点下所有子节点
        if (p.length > 1) {
          let x1 = this.left
          let x2 = this.left + this.width
          let y1 = this.top
          let y2 = this.top + this.height
          for (let i = 0; i < p.length; i++) {
            if (p[i] !== this.$el && p[i].className !== undefined && p[i].getAttribute('data-is-snap') !== 'false') {
              let l = p[i].offsetLeft // 对齐目标的left
              let r = l + p[i].offsetWidth // 对齐目标右侧距离窗口的left
              let t = p[i].offsetTop// 对齐目标的top
              let b = t + p[i].offsetHeight // 对齐目标右侧距离窗口的top
              let ts = Math.abs(t - y2) <= this.snapTolerance
              let bs = Math.abs(b - y1) <= this.snapTolerance
              let ls = Math.abs(l - x2) <= this.snapTolerance
              let rs = Math.abs(r - x1) <= this.snapTolerance
              if (ts) {
                this.top = t - this.height
              }
              if (bs) {
                this.top = b
              }
              if (ls) {
                this.left = l - this.width
              }
              if (rs) {
                this.left = r
              }
            }
          }
        }
      }
    }, // 检测对齐元素
```
> 
好了，最后就是在鼠标移动组件时及时调用就可以了。
```
handleMove: function (e) {
        ...
        this.snapCheck()
        this.$emit('dragging', this.left, this.top)
      }
    }, // 鼠标移动
```
### 总结
这次的修改还算是非常顺利，顺便还把之前的一些代码进行了优化。
如果发现什么bug或者可以将代码优化的地方**请劳烦告知我**。
