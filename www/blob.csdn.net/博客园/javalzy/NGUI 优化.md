# NGUI 优化 - javalzy - 博客园
# [NGUI 优化](https://www.cnblogs.com/javalzy/p/6607988.html)
1. Update
　　Ngui 组件继承关系是  UIWidget : UIRect : MonoBehaviour. 
因此由每个组件的独自调用update变更为，由某个更新点，统一调用会效率提升。并且可以调整更新频率。
2. NGUI UIDrawcall 优化：
**参考：NGUI开发技巧(上，下)**
**      https://v.qq.com/x/page/j0336jncwn5.html**
**　     https://v.qq.com/x/page/r0342tl5e47.html**
  工具：NGUI->Open-> Panel Tool && UIDrawCall Tool 
  A. UIDrawCall 个数优化。
　　　一个panel可以多个UIDrawcall UIDrawcall个数 = 将此panel下的widget按depth排序，材质相同的划归为一个UIDrawCall. 
     所以减少DrawCall个数就要尽可能的避免材质交叉，即把材质相同的widget的depth调到相邻。
　　 优化方法：
        a. 文字与图片depth规划好。
        b. 调 widget.depth
  B. UIDrawCall.UpdateGeometry:  Set the draw call's geometry
　　  包括某一个UIDrawCall内部三角形重建，另一种情况是此UIPanel下的所有DrawCall的重建。
     第一种情况常出现在：此DrawCall内顶点数据变更（如transform位移，缩放，颜色变化）都会触发此mydrawCall.updateGeometry().
     第二种情况（重建所有DrawCall）出现在 UIPanel.mRebuild = true 的情况。如
　　　　　　a. UIPanel.RemoveWidget (UIWidget w): w 正好是某个drawCall的边界组件。
          b. 材质交叉：使用了a材质的widget.depth插入了使用b材质的某个drawcall. 
          c. panel.OnInit 及 OnEnable时。
　　 优化原理：
     　　a. 避免重建.  
　　　　　b. 不可避免重建时，减少重建的量。
     常用方法：
        a. 动静分离：加UIPanel.重建时只重建动态部分的三角形
  C. UITexture不是使用的图集所以会单独占用一个DrawCall.  
  D. UI上的粒子特效: 不会影响UIDrawCall个数。
3. UILable:  OutLine, Shadow. 效果
4. UISprite: 平铺类型
5. UIFont: 
文字的材质合并不到一起，可能的原因是使用了不同的字体。

