# iOS小问题（四） - game3108的专栏 - CSDN博客
2017年05月09日 22:58:17[game3108](https://me.csdn.net/game3108)阅读数：191标签：[ios](https://so.csdn.net/so/search/s.do?q=ios&t=blog)
个人分类：[iOS](https://blog.csdn.net/game3108/article/category/2844789)
## 随手记几个最近碰到的小问题
tips：如果有错误，或者有更好的详细解答，请随时联系我进行修改
#### 1.CocoaPods的组件化小问题
在组件化的过程中，使用CocoaPods发现了一些小问题，记录一下 
1. 以path形式调试时，新增文件问题 
需要在bundle project中先添加文件，然后回到main project中pod update一下才能正确加入 
2. 指定版本号失败，继续从Head上拉取最新代码 
需要在podspec中定义source后加上, :tag => spec.version，来确定版本号和tag的关系
#### 2.reloadSections的cell消失问题
对于`reloadSections：withRowAnimation`的动画，如果按照以下方式写，会出现很神奇的全部消失事件。
```
_cellArray = NSArray.new;
    _cell1 = [[UITableViewCell alloc]initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 100)];
    _cell1.textLabel.text = @"111111";
    _cell1.textLabel.textColor = [UIColor blackColor];
    _cell2 = [[UITableViewCell alloc]initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 100)];
    _cell2.textLabel.text = @"222222";
    _cell2.textLabel.textColor = [UIColor blackColor];
    _cell3 = [[UITableViewCell alloc]initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 100)];
    _cell3.textLabel.text = @"333333";
    _cell3.textLabel.textColor = [UIColor blackColor];
    _cellArray = @[_cell1, _cell2, _cell3];
```
```
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    return 100;
}
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return [_cellArray count];
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    NSInteger row = indexPath.row;
    UITableViewCell *cell = [_cellArray objectAtIndex:row];
    return cell;
}
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    [tableView reloadSections:[NSIndexSet indexSetWithIndex:0] withRowAnimation:UITableViewRowAnimationFade];
}
```
其实原因就是`reloadSections：withRowAnimation`的动画会对cell进行消失动画处理，那么如果是同一个cell就会消失。 
所以这边的做法是应该用新的cell作为动画完成后的cell来显示。
#### 3.tableview reloaddata导致内部textfield失焦问题
当在tableview上，有cell有UITextField，并且是firstResponder的时候，`[tableview reloaddata]`会导致UITextField失焦。比较好的处理方法是`- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath;`让他重新获得焦距。
#### 4.关于watchpoint去监控view的frame变化问题
在调试的时候，我们经常会用`watchpoint set variable`去调试变量的设置，但frame的调整却无法设置。其实frame可以用break的方式进行监控。 
**模拟器**
`break set -F '-[CALayer setBounds:]' -c '$rdi == 0x...'`
**真机**
`break set -F '-[CALayer setBounds:]' -c '$arg1 == 0x...'`
具体原因可以参考参考连接的两个文章。
## 参考连接
1.[How do I set an lldb watchpoint on a property of self.view?](http://stackoverflow.com/questions/13976219/how-do-i-set-an-lldb-watchpoint-on-a-property-of-self-view)
2.[Using Xcode’s debugger to find where UITabBar frame is being set](http://stackoverflow.com/questions/27575855/using-xcodes-debugger-to-find-where-uitabbar-frame-is-being-set)
