# java 调用存储过程实现分页 sql server 2012 版 - z69183787的专栏 - CSDN博客
2013年11月29日 16:23:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1675
                
存储过程：（sql语句中必须有order by  仅支持2012）
```
USE [stww]
GO
/****** Object:  StoredProcedure [dbo].[SplitPage]    Script Date: 2013/11/29 16:20:19 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCedure [dbo].[SplitPage] 
@sqlQuery varchar(2000), --//输入参数：SQL检索语句或表名 
@pageIndex int, --//输入参数：当前页码 
@pageSize int --//输入参数：每页显示记录条数 
as SET NOCOUNT ON 
SET ANSI_WARNINGS OFF 
declare @indexStart varchar(20),@indexEnd varchar(20) 
set @indexStart = cast((@pageIndex-1)*@pageSize+1 as varchar(20))
if @indexStart <0 
set @indexStart = 0
 --//数据起始行ID 
exec( @sqlQuery + ' offset  ' + @indexStart + ' row fetch next ' + @pageSize +' rows only') --//检索该页数据 
GO
```
java：
```java
try{
			String sql = s_sql;
			sql += " order by "+s_pk+" "+s_sort;
			PreparedStatement pstmt = conn.prepareStatement("{call SplitPage(?,?,?)}"); 
			System.out.println("sql"+sql);
			pstmt.setString(1, sql);
			pstmt.setInt(2, Integer.parseInt(pageNo));
			pstmt.setInt(3, pageSize);	
			rs = pstmt.executeQuery();
			 
		}catch(Exception e){
			System.out.println(e.getMessage());
			return null;
		}
		//System.out.println(sql);
		return rs;
```

