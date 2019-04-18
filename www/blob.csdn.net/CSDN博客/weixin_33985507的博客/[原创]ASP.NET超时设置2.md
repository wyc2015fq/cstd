# [原创]ASP.NET超时设置2 - weixin_33985507的博客 - CSDN博客
2007年05月06日 21:17:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
1.IIS->[网站]－>属性－》连接超时。默认为120秒
2.WEB.CONFIG 手工添加httpRuntime，如
<system.web>
 <httpRuntime maxRequestLength="1000000" executionTimeout="2000" />
</system.web>
3.同步执行WEBSERVICE时，需要设置TIMEOUT属性，如
 CompilerSvr.MyFavoritesService compiler=new FDN.DMS.Controls.CompilerSvr.MyFavoritesService(); 
   compiler.Timeout =2000000; //毫秒
引用请声明本文来源http://www.cnblogs.com/linn/!!!
如果使用FILL一个很大的数据到table或dataset
DataTable retTable = new DataTable();
            retTable.TableName = tablename;
            SqlConnection con = new SqlConnection( m_SqlConnectionString );
            con.Open ();
            SqlDataAdapter sda = new SqlDataAdapter( sql, con );
            sda.Fill ( retTable );
            con.Close ();
            return retTable;
即使你设置了connectstring <add key="connectionString" value="data source=(local);initial catalog=http://www.cnblogs.com/linn/;persist security info=False;user id=sa;password=;packet size=8192;Connection Timeout=600" />仍然会超时，这时只有使用CommandTimeout：
            SqlDataAdapter sda = new SqlDataAdapter();
            SqlCommand sqlcmd = new SqlCommand();
            DataSet dt = new DataSet();            
            SqlConnection con = new SqlConnection( m_SqlConnectionString );            
            con.Open ();
            sqlcmd.Connection = con;
            sqlcmd.CommandText = sql;
            sqlcmd.CommandTimeout = 600;
            //SqlDataAdapter sda = new SqlDataAdapter( sql, con );        
            sda.SelectCommand = sqlcmd;
            sda.Fill(dt) ;
            con.Close ();
            return dt.Tables[0];
