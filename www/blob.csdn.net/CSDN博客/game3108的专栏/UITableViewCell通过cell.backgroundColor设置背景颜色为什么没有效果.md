# UITableViewCell通过cell.backgroundColor设置背景颜色为什么没有效果 - game3108的专栏 - CSDN博客
2015年07月29日 17:38:34[game3108](https://me.csdn.net/game3108)阅读数：1038
**在做ios6的适配的时候发现，无论我是把cell的backgroundColor设置颜色还是增加一个view设置背景色，在cell上都没有效果。**
**查了一下，找到如下文章，解决了这个问题。**
**链接：http://blog.sina.com.cn/s/blog_6531b9b80101c30b.html**
**今天同事一个UITableViewCell得背景设置得时候产生了疑惑**
**用cell.backgroundColor = [UIColor blueColor];**
**设置背景没有效果。**
**就调研了下，很快在伟大的stackoverflow找到了答案：**
![UITableViewCell通过cell.backgroundColor设置背景颜色为什么没有效果](http://s2.sinaimg.cn/mw690/6531b9b8tx6BAYdNyZXd1&690)
查字典的结果：
有一个2009年或者2010的WWDC视频提到了这个问题。tableview会调整背景来管理cells的选择状态，这就是为什么你能且只能在willDisplayCell这个方法里面修改它的实现的原因。
下面是验证过程：
- (UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath*)indexPath
{
   static NSString* textCellIndentify = @"textCellIndentify";
UITableViewCell* cell  = [tableView dequeueReusableCellWithIdentifier: textCellIndentify];
if (!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault
 reuseIdentifier: textCellIndentify];
          cell.backgroundColor = [UIColor redColor];
    }
//断点, 可以看到我们的cell的背景颜色是已经修改了的。
![UITableViewCell通过cell.backgroundColor设置背景颜色为什么没有效果](http://s16.sinaimg.cn/mw690/6531b9b8tx6BAXAnz0r4f&690)
return cell;
}
- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell
 forRowAtIndexPath:(NSIndexPath *)indexPath
{
//断点，可以看到cell的颜色又被系统修改回来了
![UITableViewCell通过cell.backgroundColor设置背景颜色为什么没有效果](http://s10.sinaimg.cn/mw690/6531b9b8tx6BAXLWovD49&690)
    cell.backgroundColor = [UIColor blackColor];
//再次设置断点，可以看到我们的值又生效了，然后系统背景颜色被改变
![UITableViewCell通过cell.backgroundColor设置背景颜色为什么没有效果](http://s9.sinaimg.cn/mw690/6531b9b8tx6BAXZ9mPK88&690)
}
于是华丽丽的黑色背景cell就出现了。
![UITableViewCell通过cell.backgroundColor设置背景颜色为什么没有效果](http://s10.sinaimg.cn/mw690/6531b9b8tx6BAY5yZL399&690)
