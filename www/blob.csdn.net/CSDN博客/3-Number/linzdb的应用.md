
# linzdb的应用 - 3-Number - CSDN博客


置顶2015年05月20日 15:17:52[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：397


**0x01 背景**
前段时间(也许是去年了吧)已经大概介绍了libzdb开源数据库连接池。后期在项目中应用过程中也遇到了一些问题，下面把一些应用场景和遇到的问题简单的说一下，希望对大家有帮助：
**0x02 应用场景**
linux C程序读写数据库，利用开源数据库链接池libzdb。
**0x03 应用实例和遇到的问题**
1、mysql数据库：
服务器上安装mysql客户端和服务端，然后安装libzdb库，步骤一般都是./configure;make;make install;
程序样例（具体程序怎么封装就看你们程序模块划分了）：
```python
#include <zdb.h>
        #include <stdio.h>
        int main(void)
        {
            int i;
            PreparedStatement_T p;
            
            char insert_sql[] = "insert into user(id, name, age) value(?, ?, ?)";
            char select_sql[] = "select * from user";
            ResultSet_T r;
            
            URL_T url = URL_new("mysql://localhost:3306/db?user=user&password=123456");
            ConnectionPool_T pool = ConnectionPool_new(url);
            ConnectionPool_start(pool);
            Connection_T con = ConnectionPool_getConnection(pool);
            
            TRY
            {
                /* insert delete update*/
                p = Connection_prepareStatement(con, insert_sql);
                  PreparedStatement_setInt(p, 1, 1);
                PreparedStatement_setInt(p, 2, "庞叶蒙");
                PreparedStatement_setString(p, 2, 25);
                PreparedStatement_execute(p);
                /*select*/
                ResultSet_T r = Connection_executeQuery(con, select_sql);
                while (ResultSet_next(r))
                {
                    printf("%-22s\t %s\n", ResultSet_getString(r, 1), ResultSet_getString(r, 2));
                }
            }
            CATCH(SQLException)
            {
                    printf("SQLException -- %s\n", Exception_frame.message);
            }
            FINALLY
            {
                    Connection_close(con);
            }
            END_TRY;
            ConnectionPool_free(&pool);
            URL_free(&url);
        }
```
mysql应用过程中没遇到比较困惑的地方。
2、oracle数据库：
服务器上安装oracle客户端或者服务端，然后安装libzdb库，安装步骤和mysql有区别，因为要利用oci去操作oracle数据库，为了让客户端应用程序连上远端oracle服务器，所以必须安装oracle客户端，找到对应的头文件和库。
./configure --help 可以查找一些相关有用信息。
安装步骤如下：
./configure --with-oci-include=/usr/include/oracle/11.1.0.1/client --with-oci-lib=/usr/lib/oracle/11.1.0.1/client/lib
make
make install
程序样例（具体程序怎么封装就看你们程序模块划分了）：
```python
#include <zdb.h>
            #include <stdio.h>
            int main(void)
            {
                int i;
                PreparedStatement_T p;
                
                char insert_sql[] = "insert into user(id, name, age) value(?, ?, ?)";
                char select_sql[] = "select * from user";
                ResultSet_T r;
                
                URL_T url = URL_new("oracle://localhost:1521/db?user=user&password=123456");
                ConnectionPool_T pool = ConnectionPool_new(url);
                ConnectionPool_start(pool);
                Connection_T con = ConnectionPool_getConnection(pool);
                
                TRY
                {
                    /* insert delete update*/
                    p = Connection_prepareStatement(con, insert_sql);
                      PreparedStatement_setInt(p, 1, 1);
                    PreparedStatement_setInt(p, 2, "庞叶蒙");
                    PreparedStatement_setString(p, 2, 25);
                    PreparedStatement_execute(p);
                    /*select*/
                    ResultSet_T r = Connection_executeQuery(con, select_sql);
                    while (ResultSet_next(r))
                    {
                        printf("%-22s\t %s\n", ResultSet_getString(r, 1), ResultSet_getString(r, 2));
                    }
                }
                CATCH(SQLException)
                {
                        printf("SQLException -- %s\n", Exception_frame.message);
                }
                FINALLY
                {
                        Connection_close(con);
                }
                END_TRY;
                ConnectionPool_free(&pool);
                URL_free(&url);
            }
```
与mysql不同之处在于url;
**0x04 应用中遇到的问题**
1、本想写一个兼容性较强的代码，一份代码能随时切换到不同的数据库，而且代码看起来比较简约，只需切换url就可以进行数据库的切换。
但在实际开发中，发现有一些特殊的函数是各自数据库所特有，而在libzdb中没有这样的函数，如oracle日期的转换等。可能有吧，只是我们充分了解，也许没有，需要开发这样的接口。
2、oracle乱码的问题：
oracle的编码已经在另一篇博文中已经说了。我们遇到的情况是当我们把编码都设置对后，发现我们数据库中还是出现乱码的现象；
经过分析发现，我们的程序在环境变量生效前就已经运行了，导致我们数据插入数据库中的编码出现乱码。最后在/etc/rc.d/rc.local添加对应的环境变量，才解决问题；
**0x05 开源程序使用过程的感想**
1、在使用过程中要充分调研和实践，多查相关资料和应用案例。只有这样才能尽量减少新技术引入的风险。
2、尽量熟悉开源的源码，为后期维护做好准备。
3、如果想分析技能，可以按我转发一篇博文中方法去做。
各位读者写得不好和错误，请大家指正！


