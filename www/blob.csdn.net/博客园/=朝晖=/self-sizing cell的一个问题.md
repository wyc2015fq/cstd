# self-sizing cell的一个问题 - =朝晖= - 博客园
# [self-sizing cell的一个问题](https://www.cnblogs.com/dhcn/p/7130986.html)
如何TableViewCell里面再加上CollectionView这类的ScrollView玩意，那自动算高就失效了，还是得用
- override func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {  
-        //computing height   
-        return 200  
-     }  
     这个函数计算高度，真正的原因是ScrollView是无法自动撑大的，它有自己的ContentSize。

