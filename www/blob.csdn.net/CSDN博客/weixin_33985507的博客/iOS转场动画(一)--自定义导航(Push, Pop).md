# iOS转场动画(一)--自定义导航(Push, Pop) - weixin_33985507的博客 - CSDN博客
2016年02月28日 06:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
自从iOS7开始开发者可以灵活的自定义转场动画,而不必局限于子类化**UINavigationController**来完成自定义. 总的来说, 自定义转场动画有两种类型: 1. 子类化**UINavigationControllerDelegate**, 并自定义一个animator(实现animated delegate)在重写协议中返回; 2. 让转场动画中的view controller(一般是目标VC来实现)实现**UIViewControllerTransitioningDelegate**, 并返回animator(与上同). 区分标准是哪个object控制动画, 当我们想重写*push, pop*等导航操作的时候, 就可以用到第一种方式来自定义; 第二种方式更加灵活, 不仅可以实现第一种的效果, 而且定制化程度更高(针对特定的VCs).
在这一篇中, 我们先来讲解第一种方式: 通过子类化**UINavigationControllerDelegate**来自定义转场动画. 先上效果图:
![625245-4afca308efbdbb4d.gif](https://upload-images.jianshu.io/upload_images/625245-4afca308efbdbb4d.gif)
效果图
上文说道, 自定义转场动画有2个关键点:一个是重写协议, 另一个就是创建自己的动画类. 我们先来看下要重写那些协议:
```
func navigationController(navigationController: UINavigationController, animationControllerForOperation
    operation: UINavigationControllerOperation, fromViewController fromVC: UIViewController, toViewController  
    toVC: UIViewController) -> UIViewControllerAnimatedTransitioning?{}
    
 func navigationController(navigationController: UINavigationController,
       interactionControllerForAnimationController animationController: 
       UIViewControllerAnimatedTransitioning) -> UIViewControllerInteractiveTransitioning?{}
```
第一个要重写的方法是返回一个实现了**UIViewControllerAnimatedTransitioning** delegate的动画object, 这个object定义了转场时呈现给用户的动画, 我们稍后再讲; 第二个方法会返回一个**UIPercentDrivenInteractiveTransition**对象, 这个对象是控制用户和动画交互的controller, 具体来说, 它控制着交互的百分比，存在周期是一个VC(fromVC)出现和另一个VC(toVC)消失期间。如果我们想支持用户通过手势进行转场的话, 这个object一定要有, 而且在手势识别的*target*中添加action来同步更新**UIPercentDrivenInteractiveTransition** object的进度.
下面来讲动画. 自定义的动画是**NSObject**类型, 实现**UIViewControllerAnimatedTransitioning**协议中的2个方法:
```
func transitionDuration(transitionContext: UIViewControllerContextTransitioning?) -> NSTimeInterval{}
 
 func animateTransition(transitionContext: UIViewControllerContextTransitioning){}
```
第一个方法是定义动画持续时间；第二个就是定义具体的animation了，代码如下:
```
func animateTransition(transitionContext: UIViewControllerContextTransitioning) {
  let toViewController: UIViewController = 
        transitionContext.viewControllerForKey(UITransitionContextToViewControllerKey)!
  let fromViewController: UIViewController =  
        transitionContext.viewControllerForKey(UITransitionContextFromViewControllerKey)!
    
  // Get container view from current transitioning context
  let containerView: UIView = transitionContext.containerView()!
  // Add dest view
  containerView.addSubview(toViewController.view)
    
  // Hide toVC first
  toViewController.view.alpha = 0
    
  UIView.animateWithDuration(self.transitionDuration(transitionContext), delay: 0, options: [.CurveLinear], animations: {
        () -> Void in
        // show to view controller
        toViewController.view.alpha = 1
        fromViewController.view.transform = CGAffineTransformMakeScale(0.1, 0.1)
 
        }) { (finished) -> Void in
            // Hide first view controller
            fromViewController.view.transform = CGAffineTransformIdentity
            // Must use transitionContext.transitionWasCancelled() to notify if the animation cancelled
            transitionContext.completeTransition(!transitionContext.transitionWasCancelled())
    }
}
```
该动画实现了转场时toViewController透明度渐变为1, fromViewController scale缩小至原来的十分之一的动画过程。在这个方法里，我们可以自定义很多有创意的动画来完成自定义转场, 重点是toVC和fromVC的转换以及生存周期，当前动画context.
代码已上传github, 纯Swift实现, <a href="https://github.com/Harryyan/Transitioning">点击这里获取</a>
