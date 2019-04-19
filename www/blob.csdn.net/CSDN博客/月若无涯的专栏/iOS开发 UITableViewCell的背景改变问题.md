# iOS开发 UITableViewCell的背景改变问题 - 月若无涯的专栏 - CSDN博客
2013年04月17日 11:35:54[月若无涯](https://me.csdn.net/u010124617)阅读数：819
前几天开发时忽然遇到一个很奇怪的问题，发现UITableView，在通过在cellForRowAtIndex代理中，
使用 [cell setBackgroundColor:]; 居然无法改变cell的背景色。貌似是tableview本身，对cell产生了一些影响。之前一些其他项目中也有用过此方法，但是可以更改背景，因此这问题的出现情况有点费解。
产生原因不明，直接上解决方案
解决方案有两个：
方法1.）仍然在cellForRowAtIndex这代理方法内，改为使用 [cell.contentView setBackgroundColor:]; 可以实现背景更改，但是要注意范围仅限于contentView，会被编辑cell时进行缩进。如果想更改整个的背景，不会受编辑或删除cell时的缩进影响，请使用方法2。
另外，如果在cell中有重写drawRect方法的，请使用方法2，否则contentView的背景色会遮盖手动绘制的内容。
方法2.）在tableView的willDisplayCell代理方法对cell.backgroudColor进行设置
-(void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath{
    if (indexPath.row%2==0) {
        cell.backgroundColor=[UIColor grayColor];
    }else{
        cell.backgroundColor=[UIColor whiteColor];
    }
}
