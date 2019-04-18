# DataSet筛选数据然后添加到新的DataSet中引发的一系列血案 - weixin_33985507的博客 - CSDN博客
2015年09月16日 00:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
直入代码：
```
var ds2 = new DataSet();
            if (ds.Tables.Count > 0)
            {
                var rows = ds.Tables[0].Select(" usertype <> 'UU'");
                if (rows.Length>0)
                {
                    DataTable tmp = rows[0].Table.Clone(); // 复制DataRow的表结构
                    foreach (DataRow row in rows)
                        tmp.ImportRow(row); // 将DataRow添加到DataTable中
                    ds2.Tables.Add(tmp);
                }
            }
```
上面的代码看似简单，但是会有很多陷阱，比如，网上照的方法，将DataRow添加到DataTable中是tmp.Rows.Add,然而这种方式会报一个“该行已经属于另一个表”的错误，
而我上面的ImportRow已经解决此问题。
为什么会报这个错了，我们来往下分析：
我们在做项目的时候，一般会使用这三个方法去实现：
一、DataTable.Rows.Add(DataRow.ItemArray);
二、DataTable.ImportRow(DataRow)
三、设置DataTable的tablename,然后.Rows.Add
第一种方法在项目中用到，确实好用！不过感觉第二种应该更好用一些。
