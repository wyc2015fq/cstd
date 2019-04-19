# iOS 11：几点值得关注的 UIKit 改进 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月22日 13:28:41[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：438
## [本文来自简书，原文地址:http://www.jianshu.com/p/0df8c58804c4](http://www.jianshu.com/p/0df8c58804c4)
## UIStackView
这个控件深受大家的喜爱，只有一个微调——但关键是，这正是它需要的微调。[stack
 view 复杂但灵活](https://medium.com/the-traveled-ios-developers-guide/uistackview-a-field-guide-c1b64f098f6d)——但考虑到它强大的能力和神奇的 AutoLayout，还是有一件事没有做好：已排列子视图间的可变间距。
在 iOS 11 中有所不同，只要一个新方法就可以轻松搞定：
```
let view1 = UIView()
let view2 = UIView()
let view3 = UIView()
let view4 = UIView()
let horizontalStackView = UIStackView(arrangedSubviews: [view1, view2, view3, view4])
horizontalStackView.spacing = 10
// view3 后面再多加 10 点间距
horizontalStackView.setCustomSpacing(10, after: view3)
```
之前我和 stack view 小伙伴已经一起遇到这个场景足够多次了，有点小生气，因为在实现 UIStackView 时，如果要应用一些 padding 的话，却发现只能全部拆下来或添加不符合直觉的”spacer“ view（这可能是 API 推出时的历史遗留问题）。
如果你需要让它产生动画或为其它 UI 考虑而腾出空间，它也可以随后被查询和参数化：
`let currentPadding = horizontalStackView.customSpacing(after: view3)`
## UITableView
社区里曾经讨论是否要用 UITableViewFlowLayout 或类似的东西来替代 table view。在 iOS 11 中，Apple 已经重申，这两个控件是独立且分离的工具，应在开发者的大脑中占据同等地位。
对于新手来说，table view 现在默认你要开启自动行高，就不再需要这一步了：
`tv.estimatedRowHeight = UITableViewAutomaticDimension`
这既是福音也是噩耗，它的确解决了很多令人头痛的问题，但也带来了一些问题（丢帧、content inset 计算、滑块解释性跳动等等）。
需要注意的是，如果你不需要这个行为——的确会有理由不需要，[可以回溯到石器时代的 iOS 10，像这样](https://twitter.com/smileyborg/status/871859045925232641)：
`tv.estimatedRowHeight = 0`
还有一种改进的方式来启用自定义操作，当用户在 cell 上向左向右滑时，或者更准确的说，从 leading 或 trailing 侧滑动时，这些 contextual 的 action 相对于目前 iOS 8 里增加的 UITableViewRowAction，是更为强大的 API：
```
let itemNameRow = 0
func tableView(_ tableView: UITableView, leadingSwipeActionsConfigurationForRowAt indexPath: IndexPath) -> UISwipeActionsConfiguration?
{
    if indexPath.row == itemNameRow
    {
        let editAction = UIContextualAction(style: .normal, title:  "Edit", handler: { (ac:UIContextualAction, view:UIView, success:(Bool) -> Void) in
             //做编辑
             //handler 将 context 重置为正常状态，true 是已完成的视觉指示
            success(true)
         })
        editAction.image = UIImage(named: "edit")
        editAction.backgroundColor = .purple
        let copyAction = UIContextualAction(style: .normal, title: "Copy", handler: { (ac:UIContextualAction, view:UIView, success:(Bool) -> Void) in
                 //做拷贝
                success(true)
        })
        return UISwipeActionsConfiguration(actions: [editAction, copyAction])
     }
    return nil
}
```
对于 trailing 动作，delegate 方法表现地完全相同。还有一个好的修改是，有一个属性可以用于当用户向左或向右滑到底时，发生”默认“滑动动作，就像”邮件“里删除邮件那样：
```
let contextualGroup = UISwipeActionsConfiguration(actions: [editAction, copyAction])
contextualGroup.performsFirstActionWithFullSwipe = true
return contextualGroup
```
这里的默认值是 true，所以如果你不想要就禁用它——尽管这好像打破了规则。
为了不过时，table view 还偷了它表弟提倡的方法，它现在也可以有 batch updates 了：
```
let tv = UITableView()
tv.performBatchUpdates({ () -> Void in
    tv.insertRows/deleteRows/insertSections/removeSections
}, completion:nil)
```
## UIPasteConfiguration
这个改进小巧好用，最初在 ”What’s New in Cocoa Touch“ session 里引起了我的兴趣。既为了粘贴操作，也为了支持拖拽数据传递，现在每个 UIResponder 都有了粘贴配置（paste configuration）属性：
`self.view.pasteConfiguration = UIPasteConfiguration()`
这个类主要从粘贴操作或拖拽中检测传入的数据，然后进行过滤，通过提供的同一类型标识符（uniform type identifiers），只接受你指定想处理的东西：
```
//表示这个类已了解想要的 UTI
UIPasteConfiguration(forAccepting: UIImage.self)
//或者也可以在更细节的层面指定
UIPasteConfiguration(acceptableTypeIdentifiers:["public.video"])
```
还有，这些 UTI 是可修改的，如果 App 流程允许，可以即时更改它们：
```
let pasteConfig = UIPasteConfiguration(acceptableTypeIdentifiers: ["public.video"])
//带来更多数据
pasteConfig.addAcceptableTypeIdentifiers(["public.image, public.item"])
//或是添加一个已经采用 NSItemProviderReading 的实例
pasteConfig.addTypeIdentifiers(forAccepting: NSURL.self)
```
因为在 iOS 11 里所有 UIResponder 都符合UIPasteConfigurationSupporting，现在就可以轻松对系统或用户拖拽或粘贴给我们的结果进行处理：
```
override func paste(itemProviders: [NSItemProvider])
{
    //处理粘贴的数据
}
```
```
```
