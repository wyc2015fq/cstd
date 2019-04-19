# UITableViewCell嵌套UITableView的正确姿势 - =朝晖= - 博客园
# [UITableViewCell嵌套UITableView的正确姿势](https://www.cnblogs.com/dhcn/p/7131003.html)
        内嵌UiTableView的高度计算起来太麻烦了，如何解决，就是把二级TableVIew里面的model item做到一级，然后对不同的item类型做不同的Cell，这样就Ok了。给一个得到Cell的源码供大家参考
- override func tableView(_ tableView: UITableView,  
-                             cellForRowAt indexPath: IndexPath) -> UITableViewCell {  
-         // Get a new or recycled cell  
-         let item = tableItems[indexPath.row]  
-         if type(of: item) == Review.self{  
-             let cell = tableView.dequeueReusableCell(withIdentifier: "ReviewCell",  
-                                                      for: indexPath) as! ReviewCell  
-             cell.review = item as! Review  
-             cell.updateViews()  
-             return cell  
- 
- 
-         }else{  
-             let cell = tableView.dequeueReusableCell(withIdentifier: "ReplyCell",  
-                                                      for: indexPath) as! ReplyCell  
-             cell.reply = item as! Reply  
-             cell.updateViews()  
-             return cell  
- 
-         }  
- 
- 
- 
- 
-     }  
       参考：https://segmentfault.com/q/1010000005595959

