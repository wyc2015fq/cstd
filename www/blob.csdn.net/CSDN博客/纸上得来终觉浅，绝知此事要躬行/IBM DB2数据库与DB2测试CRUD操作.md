# IBM DB2数据库与DB2测试CRUD操作 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年09月02日 11:02:51[boonya](https://me.csdn.net/boonya)阅读数：656








      很早之前就听闻过IBM DB2的威名一直没有机会得以实践，今天就来一瞥DB2的风采。增删改查（CRUD）操作使用JDBC是最简单同时也是最原始的数据库连接测试方式。

# 什么是DB2

**     IBM DB2** 是[美国](http://baike.baidu.com/view/2398.htm)[IBM](http://baike.baidu.com/view/1937.htm)公司开发的一套[关系型数据库管理系统](http://baike.baidu.com/view/1450387.htm)，它主要的运行环境为[UNIX](http://baike.baidu.com/view/8095.htm)（包括IBM自家的[AIX](http://baike.baidu.com/view/349664.htm)）、[Linux](http://baike.baidu.com/view/1634.htm)、IBM i（旧称OS/400）、[z/OS](http://baike.baidu.com/view/1245216.htm)，以及[Windows](http://baike.baidu.com/view/4821.htm)服务器版本。


# DB2的特性

      DB2除了可以提供主流的[OS/390](http://baike.baidu.com/view/744300.htm)和VM[操作系统](http://baike.baidu.com/view/880.htm)，以及中等规模的AS/400系统之外，IBM还提供了跨平台（包括基于[UNIX](http://baike.baidu.com/view/8095.htm)的[LINUX](http://baike.baidu.com/view/1634.htm)，[HP-UX](http://baike.baidu.com/view/58963.htm)，[Sun](http://baike.baidu.com/view/24856.htm)[Solaris](http://baike.baidu.com/view/329359.htm)，以及[SCO](http://baike.baidu.com/view/276950.htm)UnixWare；还有用于个人电脑的[OS/2](http://baike.baidu.com/view/108542.htm)操作系统，以及微软的[Windows 2000](http://baike.baidu.com/view/153378.htm)和其早期的系统）的DB2产品。DB2[数据库](http://baike.baidu.com/view/1088.htm)可以通过使用微软的开放数据库连接（[ODBC](http://baike.baidu.com/view/41321.htm)）接口，Java数据库连接（[JDBC](http://baike.baidu.com/view/25611.htm)）接口，或者[CORBA](http://baike.baidu.com/view/153815.htm)接口代理被任何的[应用程序](http://baike.baidu.com/view/330120.htm)访问。


# DB2的应用场景

       DB2主要应用于大型应用系统，具有较好的可伸缩性，可支持从大型机到单用户环境，应用于所有常见的服务器操作系统平台下。 DB2提供了高层次的数据利用性、完整性、安全性、可恢复性，以及小规模到大规模应用程序的执行能力，具有与平台无关的基本功能和SQL命令。DB2采用了数据分级技术，能够使大型机数据很方便地下载到LAN数据库服务器，使得客户机/服务器用户和基于LAN的应用程序可以访问大型机数据，并使数据库本地化及远程连接透明化。 DB2以拥有一个非常完备的查询优化器而著称，其外部连接改善了查询性能，并支持多任务并行查询。 DB2具有很好的网络支持能力，每个子系统可以连接十几万个分布式用户，可同时激活上千个活动线程，对大型分布式应用系统尤为适用。


# DB2 JDBC CRUD 操作

```java
//:Db2ConnTest  
     /** 
     *功能：实现连接DB2数据库，并实现基本获取数据功能 
     *jiangbin 
     */  
    import java.sql.*;  
    import java.util.*;     
      
    @SuppressWarnings("unchecked")  
    public class Db2ConnTest{  
        Connection conn;  
        Statement stat;  
        ResultSet rs;  
        ResultSetMetaData rsmd;  
        PreparedStatement pstat; //预编译  
        List ll = new ArrayList();  
          
        /** 
        *建立连接 
        */  
        public void connDb2(){    
            try{  
                //第一步：加载驱动  
                Class.forName("COM.ibm.db2.jdbc.app.DB2Driver");  
                //第二步：得到连接  
                conn = DriverManager.getConnection("jdbc:db2:mydb","db2admin","123");  
                //第三步：建立statement,同一时间每个 Statement 对象在只能打开一个 ResultSet 对象。  
                stat = conn.createStatement();        
                }  
                catch(ClassNotFoundException ee){  
                    System.out.println("ClassNotFoundException:"+ee.getMessage());  
                    }  
                catch(SQLException e){  
                    System.out.println(e);  
                    }             
            }  
          
        /** 
        *断开连接 
        */  
        public void closeDb2(){  
            try{  
                if(rs!=null) rs.close();  
                if(stat!=null) stat.close();  
                if(conn!=null) conn.close();  
          }  
        catch(SQLException e){  
            System.out.println(e);  
            }  
            }  
          
        /** 
        *查询数据 
        */  
        public void searchData(){  
            try{  
             //得到结果集  
                rs = stat.executeQuery("select * from db.tblStudent");  
                //用于获取关于 ResultSet 对象中列的类型和属性信息的对象  
                rsmd = rs.getMetaData();  
                while(rs.next()){  
                    Map rowData = new HashMap();  
                    for(int i=1;i<=rsmd.getColumnCount();i++)  
                    rowData.put(rsmd.getColumnName(i),rs.getString(i));  
                    ll.add(rowData);  
                    }     
                }  
                catch(SQLException e){  
            System.out.println(e);  
            }  
            }  
          
        /** 
        *打印数据 
        */    
        public void printData(){  
             for(int i=0;i<ll.size();i++ ){  
                    System.out.println(ll.get(i));  
                    }  
            }  
              
        /** 
        *插入数据 
        */  
        public void insert(){  
            try{  
            //插入语句  
            String str = "INSERT INTO db.tblStudent(strName,intAge,strAddress,strSex,grandId)"  
                          +"VALUES(?,?,?,?,?)";  
            pstat = conn.prepareStatement(str);  
            pstat.setString(1,"王文远");  
            pstat.setInt(2,20);  
            pstat.setString(3,"北京");  
            pstat.setString(4,"T");  
            pstat.setInt(5,4);  
            int record = pstat.executeUpdate();  
            System.out.println("插入"+record+"数据");  
          }  
         catch(SQLException e){  
            System.out.println(e);  
            }  
            }  
              
        /** 
        *删除数据 
        */  
        public void delete(){  
            try{  
            //删除语句  
            String str = "DELETE FROM db.tblStudent where strName='王五'";  
            stat.executeUpdate(str);  
          }  
         catch(SQLException e){  
            System.out.println(e);  
            }  
            System.out.println("删除数据成功");  
            }  
              
        /** 
        *修改数据 
        */  
        public void update(){  
            try{  
            //修改语句  
            String str = "UPDATE db.tblstudent SET strAddress='北京',intAge=intAge+5 where intId=1";  
            stat.executeUpdate(str);  
          }  
         catch(SQLException e){  
            System.out.println(e);  
            }  
            System.out.println("修改数据成功");  
            }  
              
        public static void main(String[] args){  
            Db2ConnTest dc = new Db2ConnTest();  
            dc.connDb2();  
          dc.insert();  
            //dc.searchData();  
            //dc.delete();  
            //dc.update();  
            dc.searchData();  
            dc.printData();  
            dc.closeDb2();  
       }  
  }
```


# 参考资料

1.[JavaJDBC操作DB2 CRUD](http://robinjiang-126-com.iteye.com/blog/1708749)

2. [DB2-百度百科](http://baike.baidu.com/link?url=V1OEVbnOf8uQtBnITWcJ7p_yJUQ1HrM_pRph2XLOavU_FgntpukSjmwZbi6u6Y_Bt2oE5Q6I5i3S2Hjngi2g4q)

3.[IMB DB2数据库软件](http://www-01.ibm.com/software/cn/data/db2/)

DB2下载地址：[ftp://public.dhe.ibm.com/software/hk/cobra/（一般下载ESE-win7旗舰版也支持）](ftp://public.dhe.ibm.com/software/hk/cobra/)

[DB2_ESE_9.7_trial_VA_32bit.exe](ftp://public.dhe.ibm.com/software/hk/cobra/DB2_ESE_9.7_trial_VA_32bit.exe)
||||----|2110054 KB||2110054 KB|2009/6/30|上午12:00:00|
|----|----|----|----|----|
|| | | | |
||[DB2_ESE_9.7_trial_VA_32bit.rar](ftp://public.dhe.ibm.com/software/hk/cobra/DB2_ESE_9.7_trial_VA_32bit.rar)||----||[DB2_ESE_9.7_trial_VA_32bit.rar](ftp://public.dhe.ibm.com/software/hk/cobra/DB2_ESE_9.7_trial_VA_32bit.rar)|2110054 KB|2009/7/3|上午12:00:00|
|[DB2_ESE_9.7_trial_VA_32bit.rar](ftp://public.dhe.ibm.com/software/hk/cobra/DB2_ESE_9.7_trial_VA_32bit.rar)| | | | |
||[db2exc_952beta_MAC_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_952beta_MAC_x86_64.tar.gz)||----||[db2exc_952beta_MAC_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_952beta_MAC_x86_64.tar.gz)|144347 KB|2009/6/30|上午12:00:00|
|[db2exc_952beta_MAC_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_952beta_MAC_x86_64.tar.gz)| | | | |
||[db2exc_970_LNX_ppc.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_LNX_ppc.tar.gz)||----||[db2exc_970_LNX_ppc.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_LNX_ppc.tar.gz)|365342 KB|2009/6/30|上午12:00:00|
|[db2exc_970_LNX_ppc.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_LNX_ppc.tar.gz)| | | | |
||[db2exc_970_LNX_x86.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_LNX_x86.tar.gz)||----||[db2exc_970_LNX_x86.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_LNX_x86.tar.gz)|409249 KB|2009/6/30|上午12:00:00|
|[db2exc_970_LNX_x86.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_LNX_x86.tar.gz)| | | | |
||[db2exc_970_LNX_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_LNX_x86_64.tar.gz)||----||[db2exc_970_LNX_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_LNX_x86_64.tar.gz)|451445 KB|2009/6/30|上午12:00:00|
|[db2exc_970_LNX_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_LNX_x86_64.tar.gz)| | | | |
||[db2exc_970_SUN_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_SUN_x86_64.tar.gz)||----||[db2exc_970_SUN_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_SUN_x86_64.tar.gz)|398956 KB|2009/6/30|上午12:00:00|
|[db2exc_970_SUN_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_SUN_x86_64.tar.gz)| | | | |
||[db2exc_970_WIN_x86.zip](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_WIN_x86.zip)||----||[db2exc_970_WIN_x86.zip](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_WIN_x86.zip)|404869 KB|2009/6/30|上午12:00:00|
|[db2exc_970_WIN_x86.zip](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_WIN_x86.zip)| | | | |
||[db2exc_970_WIN_x86_64.zip](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_WIN_x86_64.zip)||----||[db2exc_970_WIN_x86_64.zip](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_WIN_x86_64.zip)|460759 KB|2009/6/30|上午12:00:00|
|[db2exc_970_WIN_x86_64.zip](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_970_WIN_x86_64.zip)| | | | |
||[db2exc_client_952beta_MAC_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_client_952beta_MAC_x86_64.tar.gz)||----||[db2exc_client_952beta_MAC_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_client_952beta_MAC_x86_64.tar.gz)|85180 KB|2009/6/30|上午12:00:00|
|[db2exc_client_952beta_MAC_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_client_952beta_MAC_x86_64.tar.gz)| | | | |
||[db2exc_guide_952_MAC_x86_64.pdf](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_guide_952_MAC_x86_64.pdf)||----||[db2exc_guide_952_MAC_x86_64.pdf](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_guide_952_MAC_x86_64.pdf)|1456 KB|2009/6/30|上午12:00:00|
|[db2exc_guide_952_MAC_x86_64.pdf](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_guide_952_MAC_x86_64.pdf)| | | | |
||[db2exc_nlpack_970_LNX_ppc.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_nlpack_970_LNX_ppc.tar.gz)||----||[db2exc_nlpack_970_LNX_ppc.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_nlpack_970_LNX_ppc.tar.gz)|319613 KB|2009/6/30|上午12:00:00|
|[db2exc_nlpack_970_LNX_ppc.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_nlpack_970_LNX_ppc.tar.gz)| | | | |
||[db2exc_nlpack_970_LNX_x86.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_nlpack_970_LNX_x86.tar.gz)||----||[db2exc_nlpack_970_LNX_x86.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_nlpack_970_LNX_x86.tar.gz)|275373 KB|2009/6/30|上午12:00:00|
|[db2exc_nlpack_970_LNX_x86.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_nlpack_970_LNX_x86.tar.gz)| | | | |
||[db2exc_nlpack_970_LNX_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_nlpack_970_LNX_x86_64.tar.gz)||----||[db2exc_nlpack_970_LNX_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_nlpack_970_LNX_x86_64.tar.gz)|325758 KB|2009/6/30|上午12:00:00|
|[db2exc_nlpack_970_LNX_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_nlpack_970_LNX_x86_64.tar.gz)| | | | |
||[db2exc_nlpack_970_SUN_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_nlpack_970_SUN_x86_64.tar.gz)||----||[db2exc_nlpack_970_SUN_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_nlpack_970_SUN_x86_64.tar.gz)|187829 KB|2009/6/30|上午12:00:00|
|[db2exc_nlpack_970_SUN_x86_64.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_nlpack_970_SUN_x86_64.tar.gz)| | | | |
||[db2exc_vsai_970_WIN_x86.zip](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_vsai_970_WIN_x86.zip)||----||[db2exc_vsai_970_WIN_x86.zip](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_vsai_970_WIN_x86.zip)|29663 KB|2009/6/30|上午12:00:00|
|[db2exc_vsai_970_WIN_x86.zip](ftp://public.dhe.ibm.com/software/hk/cobra/db2exc_vsai_970_WIN_x86.zip)| | | | |
||[v9.7_aix64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_aix64_server.tar.gz)||----||[v9.7_aix64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_aix64_server.tar.gz)|897988 KB|2009/6/30|上午12:00:00|
|[v9.7_aix64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_aix64_server.tar.gz)| | | | |
||[v9.7_hpipf64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_hpipf64_server.tar.gz)||----||[v9.7_hpipf64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_hpipf64_server.tar.gz)|873956 KB|2009/6/30|上午12:00:00|
|[v9.7_hpipf64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_hpipf64_server.tar.gz)| | | | |
||[v9.7_linux390x64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_linux390x64_server.tar.gz)||----||[v9.7_linux390x64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_linux390x64_server.tar.gz)|460874 KB|2009/6/30|上午12:00:00|
|[v9.7_linux390x64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_linux390x64_server.tar.gz)| | | | |
||[v9.7_linuxia32_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_linuxia32_server.tar.gz)||----||[v9.7_linuxia32_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_linuxia32_server.tar.gz)|657271 KB|2009/6/30|上午12:00:00|
|[v9.7_linuxia32_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_linuxia32_server.tar.gz)| | | | |
||[v9.7_linuxppc64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_linuxppc64_server.tar.gz)||----||[v9.7_linuxppc64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_linuxppc64_server.tar.gz)|642490 KB|2009/6/30|上午12:00:00|
|[v9.7_linuxppc64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_linuxppc64_server.tar.gz)| | | | |
||[v9.7_linuxx64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_linuxx64_server.tar.gz)||----||[v9.7_linuxx64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_linuxx64_server.tar.gz)|776774 KB|2009/6/30|上午12:00:00|
|[v9.7_linuxx64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_linuxx64_server.tar.gz)| | | | |
||[v9.7_nt32_server.zip](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_nt32_server.zip)||----||[v9.7_nt32_server.zip](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_nt32_server.zip)|559758 KB|2009/6/30|上午12:00:00|
|[v9.7_nt32_server.zip](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_nt32_server.zip)| | | | |
||[v9.7_ntx64_server.zip](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_ntx64_server.zip)||----||[v9.7_ntx64_server.zip](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_ntx64_server.zip)|617464 KB|2009/6/30|上午12:00:00|
|[v9.7_ntx64_server.zip](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_ntx64_server.zip)| | | | |
||[v9.7_sun64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_sun64_server.tar.gz)||----||[v9.7_sun64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_sun64_server.tar.gz)|734868 KB|2009/6/30|上午12:00:00|
|[v9.7_sun64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_sun64_server.tar.gz)| | | | |
||[v9.7_sunamd64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_sunamd64_server.tar.gz)||----||[v9.7_sunamd64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_sunamd64_server.tar.gz)|602428 KB|2009/6/30|上午12:00:00|
|[v9.7_sunamd64_server.tar.gz](ftp://public.dhe.ibm.com/software/hk/cobra/v9.7_sunamd64_server.tar.gz)| | | | |
602428 KB2009/6/30上午12:00:00



