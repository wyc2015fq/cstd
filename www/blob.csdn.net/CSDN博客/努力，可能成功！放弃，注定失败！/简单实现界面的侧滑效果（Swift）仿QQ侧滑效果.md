# 简单实现界面的侧滑效果（Swift）仿QQ侧滑效果 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月07日 17:20:54[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1806
废话不多说，看效果图
![](http://upload-images.jianshu.io/upload_images/1429890-1e5c2689d50bd1e3.gif?imageMogr2/auto-orient/strip)
侧滑.gif
# viewController
```
//
//  ViewController.swift
//  MGSlideViewDemo
//
//  Created by ming on 16/6/5.
//  Copyright © 2016年 ming. All rights reserved.
//
import UIKit
// 定义几个常量，可以自己修改值
let MGScreenWidth = UIScreen.mainScreen().bounds.width
let offsetDetailRight:CGFloat = 90
let offsetMainRight:CGFloat = 320
class ViewController: UIViewController,UIGestureRecognizerDelegate {
    var detailView: UIView?
    var coverView: UIView?
    var mainView: UIView?
    override func viewDidLoad() {
        super.viewDidLoad()
        setUpChildrenView()
        setupGestureRecognizer()
    }
    // 创建右边的控制器
    func setUpChildrenView(){
        let detailViewController = MGDetailViewController()
        self.addChildViewController(detailViewController)
        detailViewController.view.frame = CGRectMake(-offsetDetailRight,self.view.frame.origin.y, self.view.frame.width, self.view.frame.height)
        self.view.addSubview(detailViewController.view)
        self.detailView = detailViewController.view
        let mainViewController = MGMainTViewController()
        self.addChildViewController(mainViewController)
        mainViewController.view.frame = self.view.bounds
        self.view.addSubview(mainViewController.view)
        self.mainView = mainViewController.view;
        self.mainView!.backgroundColor = UIColor.greenColor()
        self.mainView!.addObserver(self, forKeyPath: "frame", options: NSKeyValueObservingOptions.Old, context: nil)
        // 添加coverView
        let coverView = UIView()
        coverView.frame = self.mainView!.frame
        coverView.hidden = true
        coverView.alpha = 0.9
        self.mainView!.addSubview(coverView)
        self.coverView = coverView;
        let tap = UITapGestureRecognizer(target: self, action: Selector("tap"))
        self.coverView?.addGestureRecognizer(tap)
    }
    func tap(){
        if self.mainView!.frame.origin.x != 0 {
            UIView.animateWithDuration(0.25, animations: { () -> Void in
                self.mainView!.frame = CGRectMake(0, 0, self.view.frame.width, self.view.frame.height);
                self.detailView!.frame = CGRectMake(-offsetDetailRight, self.view.frame.origin.y, self.view.frame.width, self.view.frame.height);
                });
        }
    }
    // KVO 监听mainView的frame的变化
    override func observeValueForKeyPath(keyPath: String?, ofObject object: AnyObject?, change: [String : AnyObject]?, context: UnsafeMutablePointer<Void>) {
        if self.mainView!.frame.origin.x == offsetMainRight {
            self.coverView?.hidden = false;
        } else {
            self.coverView?.hidden = true;
        }
    }
    // 添加pan手势
    func setupGestureRecognizer(){
        let pan = UIPanGestureRecognizer(target: self, action: Selector("pan:"))
//        pan.delegate = self;
        self.mainView!.addGestureRecognizer(pan)
    }
    func pan(pan:UIPanGestureRecognizer){
        let transP = pan.translationInView(self.view)
        self.mainView!.frame = mainFrameWithoffsetX(transP.x)
        pan.setTranslation(CGPointZero, inView: self.mainView!)
        self.detailView!.frame = detailFrameWithoffsetX(transP.x)
        pan.setTranslation(CGPointZero, inView: self.detailView!)
        if pan.state == UIGestureRecognizerState.Ended {
            var targetMain:CGFloat = 0
            var targetDetail:CGFloat = offsetDetailRight
            if (self.mainView!.frame.origin.x > MGScreenWidth * 0.5) {
                targetMain = offsetMainRight
                targetDetail = 0;
            }
            let offsetMainX:CGFloat = targetMain - self.mainView!.frame.origin.x
            let offsetDetailX:CGFloat = -targetDetail
            UIView.animateWithDuration(0.25, animations: { () -> Void in
                self.mainView!.frame = self.mainFrameWithoffsetX(offsetMainX)
                self.detailView!.frame = CGRectMake(offsetDetailX, 0, self.view.frame.width, self.view.frame.height)
            })
        }
    }
    // 返回主View的frame
    func mainFrameWithoffsetX(offsetX: CGFloat) -> CGRect{
        var frame = self.mainView!.frame
        frame.origin.x += offsetX
        if frame.origin.x <= 0 {
            frame.origin.x = 0
        }
        return frame
    }
    // 返回detailView的frame
    func detailFrameWithoffsetX(offsetX: CGFloat) -> CGRect{
        var frame = self.detailView?.frame
        frame!.origin.x += (offsetX * offsetDetailRight / offsetMainRight)
        if frame!.origin.x >= 0 {
            frame!.origin.x = 0
        }
        return frame!
    }
    // MARK: - UIGestureRecognizerDelegate
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }    
}
```
# MGDetailViewController
```
//
//  MGDetailViewController.swift
//  MGSlideViewDemo
//
//  Created by ming on 16/6/5.
//  Copyright © 2016年 ming. All rights reserved.
//
import UIKit
let cellId = "cellId"
class MGDetailViewController: UIViewController,UITableViewDataSource,UITableViewDelegate {
    @IBOutlet weak var tableView: UITableView!
    var array:[String]?
    override func viewDidLoad() {
        super.viewDidLoad()
        self.array = ["开通会员", "QQ钱包", "个性装扮", "我的收藏", "我的相册", "我的文件", "我的名片夹"]
        self.tableView.separatorStyle = UITableViewCellSeparatorStyle.None
        self.tableView.registerClass(UITableViewCell.self, forCellReuseIdentifier: cellId)
    }
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    // MARK: - UITableViewDataSource
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return self.array!.count
    }
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier(cellId)
        cell!.backgroundColor = UIColor.clearColor()
        cell!.textLabel!.text = self.array![indexPath.row]
        cell!.textLabel!.textColor = UIColor.purpleColor()
        cell!.textLabel!.font = UIFont.systemFontOfSize(16)
        let indexname = self.array![indexPath.row]
        cell!.imageView!.image = UIImage(named: indexname)
        return cell!
    }
}
```
![](http://upload-images.jianshu.io/upload_images/1429890-0ad771b21923993a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
MGDetailViewController.Xib
# MGMainTViewController
```
//
//  MGMainTViewController.swift
//  MGSlideViewDemo
//
//  Created by ming on 16/6/6.
//  Copyright © 2016年 ming. All rights reserved.
//
import UIKit
class MGMainTViewController: UITableViewController {
    override func viewDidLoad() {
        super.viewDidLoad()
        self.tableView.registerClass(UITableViewCell.self, forCellReuseIdentifier: "reuseIdentifier")
    }
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    // MARK: - Table view data source
    override func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        // #warning Incomplete implementation, return the number of sections
        return 1
    }
    override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        // #warning Incomplete implementation, return the number of rows
        return 40
    }
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("reuseIdentifier", forIndexPath: indexPath)
        cell.textLabel?.text = "明明就是你\(indexPath.row)行测试"
        return cell
    }
}
```
![](http://upload-images.jianshu.io/upload_images/1429890-075acd3bbba4ff3a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
MGMainTViewController.view
源码暂时没有放到网上，有需要再私信
文／Mg明明就是你（简书作者）
原文链接：http://www.jianshu.com/p/2acd50d8cc2c
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
