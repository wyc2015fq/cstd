# Java程序员的存储过程 - 逍遥剑客 - CSDN博客
2006年11月30日 23:51:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1194标签：[存储																[java																[postgresql																[数据库																[jdbc																[sql](https://so.csdn.net/so/search/s.do?q=sql&t=blog)](https://so.csdn.net/so/search/s.do?q=jdbc&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=postgresql&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)
个人分类：[基础知识](https://blog.csdn.net/xoyojank/article/category/259553)
原文：[http://www.onjava.com/pub/a/onjava/2003/08/13/stored_procedures.html](http://www.onjava.com/pub/a/onjava/2003/08/13/stored_procedures.html)
by Nic Ferrier 
本文阐述了怎么使用DBMS存储过程。我阐述了使用存储过程的基本的和高级特性，比如返回ResultSet。本文假设你对DBMS和JDBC已经非常熟悉，也假设你能够毫无障碍地阅读其它语言写成的代码（即不是Java的语言），但是，并不要求你有任何存储过程的编程经历。 
存储过程是指保存在数据库并在数据库端执行的程序。你可以使用特殊的语法在Java类中调用存储过程。在调用时，存储过程的名称及指定的参数通过JDBC连接发送给DBMS，执行存储过程并通过连接（如果有）返回结果。 
使用存储过程拥有和使用基于EJB或CORBA这样的应用服务器一样的好处。区别是存储过程可以从很多流行的DBMS中免费使用，而应用服务器大都非常昂贵。这并不只是许可证费用的问题。使用应用服务器所需要花费的管理、编写代码的费用，以及客户程序所增加的复杂性，都可以通过DBMS中的存储过程所整个地替代。 
你可以使用Java，Python，Perl或C编写存储过程，但是通常使用你的DBMS所指定的特定语言。Oracle使用PL/SQL，PostgreSQL使用pl/pgsql，DB2使用Procedural SQL。这些语言都非常相似。在它们之间移植存储过程并不比在Sun的EJB规范不同实现版本之间移植Session Bean困难。并且，存储过程是为嵌入SQL所设计，这使得它们比Java或C等语言更加友好地方式表达数据库的机制。 
因为存储过程运行在DBMS自身，这可以帮助减少应用程序中的等待时间。不是在Java代码中执行4个或5个SQL语句，而只需要在服务器端执行1个存储过程。网络上的数据往返次数的减少可以戏剧性地优化性能。 
使用存储过程 
简单的老的JDBC通过CallableStatement类支持存储过程的调用。该类实际上是PreparedStatement的一个子类。假设我们有一个poets数据库。数据库中有一个设置诗人逝世年龄的存储过程。下面是对老酒鬼Dylan Thomas（old soak Dylan Thomas，不指定是否有关典故、文化，请批评指正。译注）进行调用的详细代码： 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)try
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int age =39;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    String poetName ="dylan thomas";
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    CallableStatement proc =
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        connection.prepareCall("{ call set_death_age(?, ?) }");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    proc.setString(1, poetName);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    proc.setInt(2, age);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    cs.execute();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)catch (SQLException e)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// ....
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
传给prepareCall方法的字串是存储过程调用的书写规范。它指定了存储过程的名称，？代表了你需要指定的参数。 
和JDBC集成是存储过程的一个很大的便利：为了从应用中调用存储过程，不需要存根（stub）类或者配置文件，除了你的DBMS的JDBC驱动程序外什么也不需要。 
当这段代码执行时，数据库的存储过程就被调用。我们没有去获取结果，因为该存储过程并不返回结果。执行成功或失败将通过例外得知。失败可能意味着调用存储过程时的失败（比如提供的一个参数的类型不正确），或者一个应用程序的失败（比如抛出一个例外指示在poets数据库中并不存在“Dylan Thomas”） 
结合SQL操作与存储过程 
映射Java对象到SQL表中的行相当简单，但是通常需要执行几个SQL语句；可能是一个SELECT查找ID，然后一个INSERT插入指定ID的数据。在高度规格化（符合更高的范式，译注）的数据库模式中，可能需要多个表的更新，因此需要更多的语句。Java代码会很快地膨胀，每一个语句的网络开销也迅速增加。 
将这些SQL语句转移到一个存储过程中将大大简化代码，仅涉及一次网络调用。所有关联的SQL操作都可以在数据库内部发生。并且，存储过程语言，例如PL/SQL，允许使用SQL语法，这比Java代码更加自然。下面是我们早期的存储过程，使用Oracle的PL/SQL语言编写： 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)createprocedure set_death_age(poet VARCHAR2, poet_age NUMBER)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)    poet_id NUMBER;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)begin
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)SELECT id INTO poet_id FROM poets WHERE name = poet;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)INSERTINTO deaths (mort_id, age) VALUES (poet_id, poet_age);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)end set_death_age;
很独特？不。我打赌你一定期待看到一个poets表上的UPDATE。这也暗示了使用存储过程实现是多么容易的一件事情。set_death_age几乎可以肯定是一个很烂的实现。我们应该在poets表中添加一列来存储逝世年龄。Java代码中并不关心数据库模式是怎么实现的，因为它仅调用存储过程。我们以后可以改变数据库模式以提高性能，但是我们不必修改我们代码。 
下面是调用上面存储过程的Java代码： 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)publicstaticvoid setDeathAge(Poet dyingBard, int age)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)throws SQLException
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   Connection con =null;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   CallableStatement proc =null;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)try
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      con  = connectionPool.getConnection();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      proc = con.prepareCall("{ call set_death_age(?, ?) }");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      proc.setString(1, dyingBard.getName());
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      proc.setInt(2, age);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      proc.execute();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)   }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)finally
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)try
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         proc.close();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)      }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)catch (SQLException e) ...{}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      con.close();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)   }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
为了确保可维护性，建议使用像这儿这样的static方法。这也使得调用存储过程的代码集中在一个简单的模版代码中。如果你用到许多存储过程，就会发现仅需要拷贝、粘贴就可以创建新的方法。因为代码的模版化，甚至也可以通过脚本自动生产调用存储过程的代码。 
Functions 
存储过程可以有返回值，所以CallableStatement类有类似getResultSet这样的方法来获取返回值。当存储过程返回一个值时，你必须使用registerOutParameter方法告诉JDBC驱动器该值的SQL类型是什么。你也必须调整存储过程调用来指示该过程返回一个值。 
下面接着上面的例子。这次我们查询Dylan Thomas逝世时的年龄。这次的存储过程使用PostgreSQL的pl/pgsql： 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)createfunction snuffed_it_when (VARCHAR) returnsinteger'
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)declare
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)    poet_id NUMBER;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)    poet_age NUMBER;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)begin
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)    -- first get the id associated with the poet.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)    SELECT id INTO poet_id FROM poets WHERE name = ＄1;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)    -- get and return the age.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)    SELECT age INTO poet_age FROM deaths WHERE mort_id = poet_id;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)    return age;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)end;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)' language 'pl/pgsql';
另外，注意pl/pgsql参数名通过Unix和DOS脚本的＄n语法引用。同时，也注意嵌入的注释，这是和Java代码相比的另一个优越性。在Java中写这样的注释当然是可以的，但是看起来很凌乱，并且和SQL语句脱节，必须嵌入到Java String中。 
下面是调用这个存储过程的Java代码： 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)connection.setAutoCommit(false);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CallableStatement proc =
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)    connection.prepareCall("{ ? = call snuffed_it_when(?) }");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)proc.registerOutParameter(1, Types.INTEGER);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)proc.setString(2, poetName);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)cs.execute();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int age = proc.getInt(2);
如果指定了错误的返回值类型会怎样？那么，当调用存储过程时将抛出一个RuntimeException，正如你在ResultSet操作中使用了一个错误的类型所碰到的一样。 
复杂的返回值 
关于存储过程的知识，很多人好像就熟悉我们所讨论的这些。如果这是存储过程的全部功能，那么存储过程就不是其它远程执行机制的替换方案了。存储过程的功能比这强大得多。 
当你执行一个SQL查询时，DBMS创建一个叫做cursor（游标）的数据库对象，用于在返回结果中迭代每一行。ResultSet是当前时间点的游标的一个表示。这就是为什么没有缓存或者特定数据库的支持，你只能在ResultSet中向前移动。 
某些DBMS允许从存储过程中返回游标的一个引用。JDBC并不支持这个功能，但是Oracle、PostgreSQL和DB2的JDBC驱动器都支持在ResultSet上打开到游标的指针（pointer）。 
设想列出所有没有活到退休年龄的诗人，下面是完成这个功能的存储过程，返回一个打开的游标，同样也使用PostgreSQL的pl/pgsql语言： 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)createprocedure list_early_deaths () return refcursor as'
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)declare
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)    toesup refcursor;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)begin
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)    open toesup for
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        SELECT poets.name, deaths.age
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        FROM poets, deaths
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        -- all entries in deaths are for poets.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        -- but the table might become generic.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        WHERE poets.id = deaths.mort_id
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)            AND deaths.age < 60;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)    return toesup;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)end;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)' language 'plpgsql'; 
下面是调用该存储过程的Java方法，将结果输出到PrintWriter： 
PrintWriter: 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)staticvoid sendEarlyDeaths(PrintWriter out)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    Connection con =null;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    CallableStatement toesUp =null;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)try
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        con = ConnectionPool.getConnection();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// PostgreSQL needs a transaction to do this...
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        con.setAutoCommit(false);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// Setup the call.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        CallableStatement toesUp
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)= connection.prepareCall("{ ? = call list_early_deaths () }");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        toesUp.registerOutParameter(1, Types.OTHER);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        getResults.execute();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        ResultSet rs = (ResultSet) getResults.getObject(1);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)while (rs.next())
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            String name = rs.getString(1);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int age = rs.getInt(2);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            out.println(name +" was "+ age +" years old.");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        rs.close();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)catch (SQLException e)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// We should protect these calls.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        toesUp.close();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        con.close();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
因为JDBC并不直接支持从存储过程中返回游标，我们使用Types.OTHER来指示存储过程的返回类型，然后调用getObject()方法并对返回值进行强制类型转换。 
这个调用存储过程的Java方法是mapping的一个好例子。Mapping是对一个集上的操作进行抽象的方法。不是在这个过程上返回一个集，我们可以把操作传送进去执行。本例中，操作就是把ResultSet打印到一个输出流。这是一个值得举例的很常用的例子，下面是调用同一个存储过程的另外一个方法实现： 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)publicclass ProcessPoetDeaths
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)publicabstractvoid sendDeath(String name, int age);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)staticvoid mapEarlyDeaths(ProcessPoetDeaths mapper)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    Connection con =null;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    CallableStatement toesUp =null;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)try
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        con = ConnectionPool.getConnection();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        con.setAutoCommit(false);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        CallableStatement toesUp
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)= connection.prepareCall("{ ? = call list_early_deaths () }");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        toesUp.registerOutParameter(1, Types.OTHER);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        getResults.execute();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        ResultSet rs = (ResultSet) getResults.getObject(1);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)while (rs.next())
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            String name = rs.getString(1);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int age = rs.getInt(2);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            mapper.sendDeath(name, age);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        rs.close();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)catch (SQLException e)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// We should protect these calls.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        toesUp.close();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        con.close();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
这允许在ResultSet数据上执行任意的处理，而不需要改变或者复制获取ResultSet的方法： 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)staticvoid sendEarlyDeaths(final PrintWriter out)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    ProcessPoetDeaths myMapper =new ProcessPoetDeaths()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)publicvoid sendDeath(String name, int age)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            out.println(name +" was "+ age +" years old.");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    };
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    mapEarlyDeaths(myMapper);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
这个方法使用ProcessPoetDeaths的一个匿名实例调用mapEarlyDeaths。该实例拥有sendDeath方法的一个实现，和我们上面的例子一样的方式把结果写入到输出流。当然，这个技巧并不是存储过程特有的，但是和存储过程中返回的ResultSet结合使用，是一个非常强大的工具。 
结论 
存储过程可以帮助你在代码中分离逻辑，这基本上总是有益的。这个分离的好处有： 
• 快速创建应用，使用和应用一起改变和改善的数据库模式。  
• 数据库模式可以在以后改变而不影响Java对象，当我们完成应用后，可以重新设计更好的模式。 
• 存储过程通过更好的SQL嵌入使得复杂的SQL更容易理解。 
• 编写存储过程比在Java中编写嵌入的SQL拥有更好的工具－－大部分编辑器都提供语法高亮！ 
• 存储过程可以在任何SQL命令行中测试，这使得调试更加容易。 
并不是所有的数据库都支持存储过程，但是存在许多很棒的实现，包括免费/开源的和非免费的，所以移植并不是一个问题。Oracle、PostgreSQL和DB2都有类似的存储过程语言，并且有在线的社区很好地支持。 
存储过程工具很多，有像TOAD或TORA这样的编辑器、调试器和IDE，提供了编写、维护PL/SQL或pl/pgsql的强大的环境。 
存储过程确实增加了你的代码的开销，但是它们和大多数的应用服务器相比，开销小得多。如果你的代码复杂到需要使用DBMS，我建议整个采用存储过程的方式。 
资源 
• JDBC specification  
• PostgreSQL  
• Oracle Corporation's Oracle database server  
• IBM's DB2 database server  
作者简介：Nic Ferrier 是Web应用方面的独立软件顾问。  
