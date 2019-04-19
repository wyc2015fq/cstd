# C#——数据翻页 - yixianfeng41的专栏 - CSDN博客
2014年12月23日 10:33:54[第2梦](https://me.csdn.net/yixianfeng41)阅读数：2411
## 一、页面设计：
### 1、所需控件： 
        bindingNavigator
### 2、操作：
       网页面中拖一个bindinNavigator
### 3、控件设计样式：
![](https://img-blog.csdn.net/20141223104413531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWl4aWFuZmVuZzQx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
### 4、控件中butoon功能：
        前四个是在同一页中“第一条，上一条，下一条，最后一条”，后面butoon实现翻页
## 二、代码部分：
### 存储过程
```cpp
-- ================================================
-- Template generated from Template Explorer using:
-- Create Procedure (New Menu).SQL
--
-- Use the Specify Values for Template Parameters 
-- command (Ctrl-Shift-M) to fill in the parameter 
-- values below.
--
-- This block of comments will not be included in
-- the definition of the procedure.
-- ================================================
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
 Create PROCEDURE UP_CommonPagerImprove  
        @Sql NVARCHAR(3200),                --需要执行的SQL语句  
        @CurrPageIndex INT = 1,             --当前页索引  
        @PageSize INT = 10,             --每页记录数量  
        @OrderCondition NVARCHAR(160) = ''      --排序条件,不带ORDER BY  
    AS  
        DECLARE @ExecSQL NVARCHAR(3200)         --最终执行的SQL语句  
        DECLARE @RecordStartIndex INT           --本次搜索开始时的索引  
        DECLARE @RecordEndIndex INT         --本次搜索结束时的索引  
        --计算开始和结束的索引  
        SET @RecordStartIndex = ((@CurrPageIndex-1) * (@PageSize)) + 1  
        SET @RecordEndIndex = @RecordStartIndex + @PageSize - 1  
      
        --拼接SQL命令,使用TOP命令的方式进行查询  
        --SUBSTRING(@Sql,7,LEN(@Sql))会跳过语句(SELECT ID FROM TABLE)中的(SELECT )  
        SET @Sql = 'SELECT TOP @RecordEndIndex TEMPCOL=0,' + SUBSTRING(@Sql,7,LEN(@Sql))   
        SET @Sql = REPLACE(@Sql,'@RecordEndIndex',CONVERT(varchar(10),@RecordEndIndex))  
        --如果存在排序语句,将排序语句添加到SQL查询命令中,否则不进行排序  
        IF @OrderCondition != ''  
            SET @Sql = @Sql + ' ORDER BY ' + @OrderCondition  
        --使用WITH对子查询定义一个别名  
        SET @ExecSQL = 'WITH TEMPTABLE AS (  
            SELECT ROW_NUMBER() OVER (ORDER BY TEMPCOL) AS RowIndex,UP_CommonPager_TempTbl.*   
            FROM (@Sql) UP_CommonPager_TempTbl)  
            SELECT * FROM TEMPTABLE WHERE RowIndex >= @RecordStartIndex'  
        SET @ExecSQL = REPLACE(@ExecSQL,'@RecordStartIndex',Convert(varchar(10),@RecordStartIndex))  
        SET @ExecSQL = REPLACE(@ExecSQL,'@Sql',@Sql)  
        --执行SQL语句  
        EXEC (@ExecSQL)
```
### 封装使用存储过程(此方法将返回一个Table数据集)
```cpp
public DataTable SelectDataByProc(string strSQL, int pageIndex, int pageSize, string orderCondition)
    {
        DataTable table = null;
        using (SqlConnection conn = new SqlConnection(ConfigurationManager.ConnectionStrings["ConStr"].ToString()))
        {
            //指定存储过程的名称  
            SqlCommand cmd = new SqlCommand("UP_CommonPagerImprove", conn);
            //指定cmd类型是Proc,并且给参数赋值  
            cmd.CommandType = CommandType.StoredProcedure;
            cmd.Parameters.Clear();
            SqlParameter[] paramters = new SqlParameter[]  
            {  
                new SqlParameter("@Sql",strSQL),  
                new SqlParameter("@CurrPageIndex",pageIndex),  
                new SqlParameter("@PageSize",pageSize),  
                new SqlParameter("@OrderCondition",orderCondition)  
            };
            cmd.Parameters.AddRange(paramters);
            try
            {
                conn.Open();
            }
            catch (Exception ex)
            {
                throw ex;
            }
            SqlDataAdapter dataAdapter = new SqlDataAdapter(cmd);
            table = new DataTable();
            dataAdapter.Fill(table);
            conn.Close();
        }
        return table;
    }
```
### 每一个页面具体实现：
### 1、   控制变量：
            pageSize ;       //页面大小
            currentPage;   //当前页面
             totalPage;        //总的页面数
             totalCount;      //总的记录数
             firstpage;         //第一页
             lastpage;         //最后一页
### 2、  业务代码：
        //绑定数据源（不一定非要用DatagridView,我用了)
        public void bind()
        {
            wker_gdview.DataSource = db.SelectDataByProc("select * from WKER_INFO", currentPage, pageSize, "");
            wker_gdview.DataBind();
        }
        //Butoon是否可操作控制
        public void limit()
        {
            if (CurrentPage== Firstpage)
            {
                toolStripButton_previousPage.Enabled = false;
                toolStripButton_nextPage.Enabled = true;
                toolStripButton_firstPage.Enabled = false;
                toolStripButton_lastPage.Enabled = true;
            }
            else if (CurrentPage == Lastpage)
            {
                toolStripButton_nextPage.Enabled = false;
                toolStripButton_lastPage.Enabled = false;
                toolStripButton_previousPage.Enabled = true;
                toolStripButton_firstPage.Enabled = true;
            }
            else if (CurrentPage == Firstpage && Firstpage ==TotalPage)
            {
                toolStripButton_nextPage.Enabled = false;
                toolStripButton_lastPage.Enabled = false;
                toolStripButton_previousPage.Enabled = false;
                toolStripButton_firstPage.Enabled = false;
            }
            else
            {
                toolStripButton_firstPage.Enabled = true;
                toolStripButton_lastPage.Enabled = true;
                toolStripButton_previousPage.Enabled = true;
                toolStripButton_nextPage.Enabled = true;
            }
        }
        //第一页
        private void toolStripButton_firstPage_Click(object sender, EventArgs e)
        {
             currentPage = 1;
             bind();
             limit();
        }
        //上一页
        private void toolStripButton_previousPage_Click(object sender, EventArgs e)
        {
            currentPage--;
             if (currentPage == 0)
                return;
             bind();
             limit();
        }
        //下一页
        private void toolStripButton_nextPage_Click(object sender, EventArgs e)
        {
             currentPage++;
             if (currentPage > totalPage)
                 return;
             bind();
             limit();
        }
        //最后一页
        private void toolStripButton_lastPage_Click(object sender, EventArgs e)
        {
             currentPage--;
             if (currentPage == 0)
                return;
             bind();
             limit();
        }
**至此，翻页功能完成！**
