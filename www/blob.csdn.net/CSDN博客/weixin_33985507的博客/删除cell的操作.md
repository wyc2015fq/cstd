# 删除cell的操作 - weixin_33985507的博客 - CSDN博客
2018年06月11日 10:39:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
##### 要删除一行cell需要的操作：
> 
1、删除动作的用户交互
> 
2、删除行对应的数据
> 
3、形影行从界面上消失
```
override func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCellEditingStyle, forRowAt indexPath: IndexPath) {
        if editingStyle == .delete {
            // Delete the row from the data source
            areas.remove(at: indexPath.row)
            provinces.remove(at: indexPath.row)
            parts.remove(at: indexPath.row)
            areaImages.remove(at: indexPath.row)
            visited.remove(at: indexPath.row)
            
            tableView.deleteRows(at: [indexPath], with: .fade)
        } else if editingStyle == .insert {
            // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
        }    
    }
```
