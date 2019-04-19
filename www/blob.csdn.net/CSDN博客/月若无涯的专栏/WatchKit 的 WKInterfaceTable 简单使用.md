# WatchKit 的 WKInterfaceTable 简单使用 - 月若无涯的专栏 - CSDN博客
2015年07月17日 09:22:27[月若无涯](https://me.csdn.net/u010124617)阅读数：991标签：[iOS开发																[Watchkit																[WKInterfaceTable](https://so.csdn.net/so/search/s.do?q=WKInterfaceTable&t=blog)](https://so.csdn.net/so/search/s.do?q=Watchkit&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS开发&t=blog)
个人分类：[WatchKit																[iOS开发](https://blog.csdn.net/u010124617/article/category/1393991)](https://blog.csdn.net/u010124617/article/category/5657757)
转自：http://nonomori.farbox.com/post/watchkit-de-wkinterfacetable-jian-dan-shi-yong
WKInterfaceTable 类似 UITableView, 其用途即展示一个表格并且用户可以与之交互。但不同的是 WKInterfaceTable 的功能更加简单，使用上也很粗暴直接。
本文一步步演示了创建一个简单的包含 WKInterfaceTable 的 Apple Watch App！
我们首先按照老办法，创建一个 Single View Application 模板的 iOS 应用。
工程创建好后，点击 File -> Add -> Target，选择 Apple Watch 标签下的 Watch App。Xcode 会添加一个工程名 Watch App 目录。同时，运行 Watch App 的 scheme 也会被添加。
![](https://img-blog.csdn.net/20150717092541867?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选中 Watch App 的 scheme，编译运行。模拟器并不会同时启用 Apple Watch 模拟器，需要点击 Hardware -> External Displays -> Apple Watch。
![](https://img-blog.csdn.net/20150717092555101?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Watch App 成功运行。
接下来就是添加 WKInterfaceTable。
点击 Watch App 目录下的 storyboard，拖拽一个 WKInterfaceTable 到界面上。然后拖一个 table 的 outlet 到 InterfaceController 上，我们命名为 tableView。
![](https://img-blog.csdn.net/20150717092609947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
不同于 UITableView 每一个 Cell 的原型对应一个 UITableViewCell，WKInterfaceTable 是使用 TableRowController 作为原型，其就是 NSObject。
我们在 Watch Extension 目录添加一个继承自 NSObject 的类，取名 RowType，将 storyboard 中的 RowController 的类改为刚创建的 RowType，并且赋一个 identifier：rowType。
拖拽一个 WKInterfaceLabel 到 row 上，同样拖一个 outlet 到 RowType。注意，要在 RowType 上添加 WatchKit 模块：
importUIKitimportWatchKitclassRowType:NSObject{@IBOutletweakvarlabelNumber:WKInterfaceLabel!}
接下来就是告诉 table 有多少数据，该如何展示了。我们直接在 willActivate 方法中对 table 进行初始化。
不同于 UITableView 还需要写 DataSource 方法回调，WKInterfaceTable 非常简单，直接使用setNumberOfRows:withRowType 方法定义行数与 row 原型。这里 RowType 就是 RowController 的 identifier。对每个 row 进行配置也是通过 rowControllerAtIndex 方法遍历得到每一个
 rowController，依次配置。
overridefuncwillActivate(){super.willActivate()//设置行数与原型类型self.tableView.setNumberOfRows(,withRowType:"rowType")//遍历，获取每一个 rowController，配置foriin0..<self.tableView.numberOfRows{letrow:RowType=self.tableView.rowControllerAtIndex(i)asRowTyperow.labelNumber.setText("\(i)")}}
运行，得如下结果：
![](https://img-blog.csdn.net/20150717092625075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
接下来是定义交互，我们需要点击某一行的回调，这里很像 UITableView 的 didSelectRowAtIndex, 我们直接在 InterfaceController 中添加 table:didSelectRowAtIndex 方法。
overridefunctable(table:WKInterfaceTable,didSelectRowAtIndexrowIndex:Int){NSLog("\(rowIndex)")}
运行：
![image](http://nonomori.farbox.com/WatchTable/005.gif)
至此，一个简单的包含 WKInterfaceTable 的 Watch App 就完成了。

