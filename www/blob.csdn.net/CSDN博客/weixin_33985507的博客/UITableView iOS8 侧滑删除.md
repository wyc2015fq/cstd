# UITableView iOS8 侧滑删除 - weixin_33985507的博客 - CSDN博客
2016年08月23日 17:15:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
![9294-a98ad6a3318a6631.png](https://upload-images.jianshu.io/upload_images/9294-a98ad6a3318a6631.png)
TableView 侧滑删除
```
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath{
    //iOS 8 需要实现
}
- (NSArray *)tableView:(UITableView *)tableView editActionsForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewRowAction *deleteAction = [UITableViewRowAction rowActionWithStyle:UITableViewRowActionStyleDefault title:@"删除" handler:^(UITableViewRowAction *action, NSIndexPath *indexPath) {
        [self deleteCheckerHttpRequestWith:indexPath];
    }];
    deleteAction.backgroundColor = [UIColor redColor];
    UITableViewRowAction *editorAction = [UITableViewRowAction rowActionWithStyle:UITableViewRowActionStyleDefault title:@"编辑" handler:^(UITableViewRowAction *action, NSIndexPath *indexPath) {
    }];
    editorAction.backgroundColor = [UIColor blueColor];
    NSArray *arr = @[deleteAction,editorAction];
    return arr;
}
```
> 
刷新列表
```
NSIndexSet * index = [NSIndexSet indexSetWithIndex:i];
[_tableView reloadSections:index withRowAnimation:UITableViewRowAnimationAutomatic];
[_tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
```
> 
UITableView 相关
[UITableView 索引详细](https://www.jianshu.com/p/60130ed7e596)
