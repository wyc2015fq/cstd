# Rendering views on the screen - game3108的专栏 - CSDN博客
2017年02月09日 14:07:13[game3108](https://me.csdn.net/game3108)阅读数：210
个人分类：[iOS																[转载](https://blog.csdn.net/game3108/article/category/2926393)](https://blog.csdn.net/game3108/article/category/2844789)
很不错的一篇讲整体渲染的文章，这里记录一下，顺便当作记录一下作者blog。
来源于：http://nsomar.com/rendering-views-on-the-screen/
这里再多贴一篇关于setNeedsLayout和layoutIfNeeded区别的文章
http://www.iosinsight.com/setneedslayout-vs-layoutifneeded-explained/
In this article I will try to explain and review the path that the view takes from initialisation to display, and what part does the CPU and GPU take in that process.
### Rendering overview
The application will create a CoreAnimation `CALayer` Hierarchy using one of the following methods:
- `UIKit` which by creating `UIViews` will indirectly creates Layers
- `CoreAnimations` By adding `CALayers` manually
Application will then send the created hierarchy to the Render server (which is an external process) in `Commit Transaction` Phase
The CA Hierarchy is submitted to the `CA render server`, which render the hierarchy of Layers on screen using `OpenGl` or `Metal`
![image](http://i1348.photobucket.com/albums/p740/o_abdelhafith/Untitled_zps722567f9.png)image
### Rendering UI
- Layout (2 Passes)
- Display
- Prepare Commit
- Commit
#### Layout
In this phase the view hierarchy is build by composing views either by `[UIView addSubview]` or `[CALayer addSublayer]`,
 Then the frames are set by calculating the constraints.
The layout phase is done in two consequent passes:
- Constraints pass (**setting constraints**)
- Layout pass (**calculating frames**)
##### Constraints Pass
Happens in `updateConstraints`, subclasses can override `updateConstraints` to add constraints to its subviews.
 This pass is executed bottom-up from the subviews to parent views.
The layout engine will call `updateConstraintsIfNeeded` several times, however this method will only call `updateConstraints` if
 the needs update flag is set by calling `setNeedsUpdateConstraints`.
Calling `setNeedsUpdateConstraints` will set the needs update flag so any consequent calls to `updateConstraintsIfNeeded` (or
 the internal system layout pass methods). The constraints will be updated asynchronously.
Calling `updateConstraintsIfNeeded` will cause the layout system to call `updateConstraints`synchronously.
 However `updateConstraints` is only called if `setNeedsUpdateConstraints` have been called before.
Each time the Constraints pass happens is followed by the Layout pass.
##### Layout Pass
After the constraints are set from the previous pass, the Layout system can now process these constraints and calculate and set the correct frames for each view.
Happens in `layoutSubviews`, subclasses can override `layoutSubviews` to participate in the layout process
 and adjust the frames of the subviews. This pass is executed top-down from the parent view to the subviews. After this method the frame of the view is set.
Calling `setNeedsLayout` will set the needs layout flag so any consequent calls to `layoutIfNeeded`(or the
 internal system layout pass methods). The layout frames will be updated asynchronously.
Calling `layoutIfNeeded` will cause the layout system to call `layoutSubviews` synchronously. However `layoutSubviews` is
 only called if `setNeedsLayout` have been called before.
#### Display
In this phase the view will use `GoreGraphics` to draw on the `CALayer` that is backing the `UIView`,
 this phase is also top-down.
The display phase take place in `drawRect` and can be triggered by calling `setNeedsDisplay` will will fire
 a future redraw.
##### Relation between Constraint, Layout and Display
- `Display` pass depends on `Layout` pass which depends on `Constraints` pass.
- Adding/Removing/Updating Constraints will trigger changes to the layout by calling `setNeedsLayout`.
- Layout pass (`layoutIfNeeded/layoutSubviews`) makes sure that the constraints are up-to data by calling an internal copy of `updateConstraintsIfNeeded`.
##### Constraints/Layout rule of thumb
#### Prepare
In this phase the PNG/JPEG Images are decoded to bitmaps and are converted to a GPU optimised image in order to be served to the GPU.
#### Commit
Now that the layout is ready and the hierarchy is build, the layout system will package all the `CALayer` hierarchy and send it to the 
```
rendering
 server
```
.
### Animations
Rendering an animation is very similar to rendering a static UI with the distinction that when rendering the animations the animation also has to be package and sent to the `render server`.
 The application will create the animation using `animateWithDuration` or `CABasicAnimations` and this animation
 will be package and sent ti the render server which in turn will render each frame for the animation.
### CPU vs GPU bound operations
The CPU and CPU both participate in the rendering process each of them has its own role.
The main idea is to minimise the CPU involvement and increase the CPU one.
##### GPU operations
- Blending multiple images together
- Antialias images
- Composition of Image
##### CPU operations
- Masking of different views and images and clipping views to that mask
- Creating shadows around views
- Visual effects such as blurring
- Drawing with `CoreGraphics`
