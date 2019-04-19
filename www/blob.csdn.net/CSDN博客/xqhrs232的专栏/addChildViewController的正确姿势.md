# addChildViewController的正确姿势 - xqhrs232的专栏 - CSDN博客
2019年03月15日 16:55:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：59
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/chokshen/article/details/82314944](https://blog.csdn.net/chokshen/article/details/82314944)
 项目中经常出现一个控制器添加多个子控制器的情况，例如今日头条的标签切换视图，使用addChildViewController的正确姿势如下：
        先添加第一个控制器，其余控制器暂时不用添加：
        //A控制器
        let vcA = ViewControllerA()
        self.addChildViewController(vcA)
        vcA.didMove(toParentViewController: self)
        currentVC = vcA
        vcA.view.frame = self.view.bounds
        self.view.addSubview(vcA.view)
        点击按钮，切换子控制器：
    @objc func change(_ sender: UISegmentedControl) {
        if sender.selectedSegmentIndex == 0 {
            changeController(from: currentVC, to: ViewControllerA())
        } else if sender.selectedSegmentIndex == 1 {
            changeController(from: currentVC, to: ViewControllerB())
        } else if sender.selectedSegmentIndex == 2 {
            changeController(from: currentVC, to: ViewControllerC())
        }
    }
    func changeController(from oldController: UIViewController, to newController: UIViewController) {
        if currentVC != newController {
            self.addChildViewController(newController)
            newController.view.frame = self.view.bounds
            self.transition(from: oldController, to: newController, duration: 0.3, options: .curveEaseInOut, animations: {
            }) { (finished) in
                if finished {
                    newController.didMove(toParentViewController: self)
                    oldController.willMove(toParentViewController: nil)
                    oldController.removeFromParentViewController()
                    self.currentVC = newController
                }
            }
        }
    }
        经过测试，使用这种方式切换子控制器，旧的控制器会被释放，新的控制器会重新加载。
