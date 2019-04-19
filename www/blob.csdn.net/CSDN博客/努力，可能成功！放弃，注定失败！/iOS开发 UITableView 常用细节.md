# iOS开发 UITableView 常用细节 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月20日 16:59:43[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2774
> 
### 给 UITableView 贴上好瓷砖
#### 本文导航：
##### -1.隐藏分割线
##### -2.隐藏多余Cell
##### -3.分割线头部顶到底、分割线颜色
##### -4.自定义点击后效果 Cell 背景等更改
##### -5.类似button点击效果 Cell - 闪一下
##### -6.Tableview视图Cell进入动画 从底部往上弹
##### -7.TableviewCell 使用SB约束 根据大小自动布局
##### -8. Cell 点击展开
##### -附加-没有数据时候提示
```
# 1.隐藏分割线 
   # 2.隐藏多余Cell
   //##❤在ViewController初始化时候加载 如viewDidLoad
   //隐藏分割线
   tableView.separatorStyle = UITableViewCellSeparatorStyle.None
   //隐藏多余的cell
   tableView.tableFooterView = UIView(frame: CGRectZero)
```
```
# 3.Cell点击效果
    ///##❤分割线头部顶到底、分割线颜色
    //启动、旋转、视图大小位置发生改变、增加子视图等..都会调用
    override func viewDidLayoutSubviews() {
        tableView.separatorInset = UIEdgeInsetsZero
        tableView.layoutMargins = UIEdgeInsetsZero
        //articleTableView.separatorColor = UIColor.redColor() //分割线颜色
    }
    //没当cell即将出现屏幕时候都会调用此方法
    func tableView(tableView: UITableView, willDisplayCell cell: UITableViewCell, forRowAtIndexPath indexPath: NSIndexPath) {
        cell.separatorInset = UIEdgeInsetsZero
        cell.layoutMargins = UIEdgeInsetsZero
    }
```
```
# 4.自定义点击后效果 Cell 背景等更改
  //##❤在cellForRowAtIndexPath方法使用
  //点击Cell时，没有点击效果
  cell.selectionStyle = UITableViewCellSelectionStyle.None
  //系统默认的颜色  .Blue蓝色-默认 .Grap灰色 .None 无色
 //点击Cell时，自定义选中后的背景视图
  //背景颜色
  cell.selectedBackgroundView = UIView()
  cell.selectedBackgroundView?.backgroundColor = UIColor.clearColor()
  //背景图片
  cell.selectedBackgroundView = UIImageView(image: UIImage(named: article.avatarImage))
 //cell 右边的辅助的提示
 cell.accessoryType =  .DisclosureIndicator //>
 //.Checkmark //√    .DetailDisclosureButton // ! >    .DetailButton // !
```
```cpp
```cpp
# 5.类似button点击效果 Cell - 闪一下
   //##❤在 didSelectRowAtIndexPath 方法内使用
   //点击Cell时 一闪而过 适合转场时候交互 - 
  tableView.deselectRowAtIndexPath(indexPath, animated: false) // - true 动画慢吞吞，适合不转场时
```
```
![](http://upload-images.jianshu.io/upload_images/1397370-22ff7b3ee3b32d8d.gif?imageMogr2/auto-orient/strip)
Cell进入动画
```
# 6.Tableview视图Cell进入动画 从底部往上弹
    //##❤加载动画 Cell 往上冲 在 viewWillAppear 中使用
    func animateTable() {
        self.tableView.reloadData()
        let cells = tableView.visibleCells
        let tableHeight = tableView.bounds.size.height
        for i in cells {
            let cell: UITableViewCell = i as UITableViewCell
            cell.transform = CGAffineTransformMakeTranslation(0, tableHeight)
        }
        var index = 0  
        for a in cells {
            let cell: UITableViewCell = a as UITableViewCell
            UIView.animateWithDuration(1.0, delay: 0.05 * Double(index), usingSpringWithDamping: 0.8, initialSpringVelocity: 0, options: [], animations: {
                cell.transform = CGAffineTransformMakeTranslation(0, 0);
                }, completion: nil)
            index += 1
        }
    }
```
![](http://upload-images.jianshu.io/upload_images/1397370-aace456df9dadc99.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
SB自动布局
```cpp
```cpp
# 6.TableviewCell 使用SB约束好， 根据大小自动布局
     ///使用SB布局的Cell ，直接使用下面代码达到自动布局目的
     //##❤在ViewController初始化时候加载 如viewDidLoad
        tableView.estimatedRowHeight = 44
        tableView.rowHeight = UITableViewAutomaticDimension
    //详情可以参考这位兄弟的简文 -- http://www.jianshu.com/p/405391bfc2f2
```
```
![](http://upload-images.jianshu.io/upload_images/1397370-35b118c7ee3aa1c2.gif?imageMogr2/auto-orient/strip)
7.cell 点击展开
```
# 7.cell 点击展开
    //比如一个使用了SB约束好的label ，tag = 666 把他  属性 lines = 0 与 1转换 即显示单行或多行
    // -1.记得使用SB设置好约束
    override func viewDidLoad() {
        super.viewDidLoad()
        // 0.启动自动布局计划
        tableView.estimatedRowHeight = 44
        tableView.rowHeight = UITableViewAutomaticDimension
    }
    // 1.先声明的一个字典 - 记录每个cell展收状态
    var dict:Dictionary<Int,Bool> = [:]
    // 2.根据字典显示cell状态
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("Cell", forIndexPath: indexPath)
        let label = cell.contentView.viewWithTag(666) as! UILabel
        label.text = "本文导航 \n 1.隐藏分割线\n 2.隐藏多余Cell\n 3.分割线头部顶到底、分割线颜色\n 4.自定义点击后效果 Cell 背景等更改\n 5.类似button点击效果 Cell - 闪一下\n 6.Tableview视图Cell进入动画 从底部往上弹\n 7.TableviewCell使用SB约束 自动布局 \n 8. cell 点击展开"
        if dict[indexPath.row] == false {
            label.numberOfLines = 0
        } else {
            label.numberOfLines = 1
        }
        return cell
    }
    // 3. 在 beginUpdates() - endUpdates() 放代码 有连续动画效果
    override func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        tableView.deselectRowAtIndexPath(indexPath, animated: true) //点击闪动效果
        let cell = tableView.cellForRowAtIndexPath(indexPath)
        let label = cell!.contentView.viewWithTag(666) as! UILabel
        tableView.beginUpdates() //开始
        if label.numberOfLines == 0 {
            label.numberOfLines = 1
            dict[indexPath.row] = true
        } else {
            label.numberOfLines = 0
            dict[indexPath.row] = false
        }
        tableView.endUpdates()
    }
```
![](http://upload-images.jianshu.io/upload_images/1397370-7f91d78fe45927a6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
加载失败
```
##-附加-没有数据时候提示 可以自行加入空数据时候显示
    //判断有没有数据显示 提示
    func showIfNoAnswer() {
            let imageView = UIImageView(frame: CGRectMake(0, 0, 60, 60))
            let image = UIImage(named: "sad")
            imageView.image = image?.imageWithRenderingMode(.AlwaysTemplate)
            imageView.tintColor = UIColor.grayColor()
            imageView.center = CGPointMake(self.view.center.x, 145)
            imageView.tag = 33  // 方便 remove
            self.view.addSubview(imageView)
            let label = UILabel(frame: .zero)
            label.text = "加载失败"
            label.font = UIFont(name: "New Gulim", size: 20)
            label.textColor = UIColor.grayColor()
            label.textAlignment = .Center
            label.tag = 3
            label.sizeToFit()
            label.backgroundColor = UIColor.clearColor()
            label.center = CGPointMake(self.view.center.x, 200)
            view.addSubview(label)
        }
    }
```
文／逗牛（简书作者）
原文链接：http://www.jianshu.com/p/5bc7864ef3cd
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
