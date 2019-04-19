# 利用 DB 实现分布式锁的思路 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [Sam哥哥](http://www.jobbole.com/members/Sam哥哥) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
### 概述
以前参加过一个库存系统，由于其业务复杂性，搞了很多个应用来支撑。这样的话一份库存数据就有可能同时有多个应用来修改库存数据。比如说，有定时任务域xx.cron，和SystemA域和SystemB域这几个JAVA应用，可能同时修改同一份库存数据。如果不做协调的话，就会有脏数据出现。对于跨JAVA进程的线程协调，可以借助外部环境，例如DB或者Redis。下文介绍一下如何使用DB来实现分布式锁。
### 设计
本文设计的分布式锁的交互方式如下：
1、根据业务字段生成`transaction_id`，并线程安全的创建锁资源
2、根据`transaction_id`申请锁
3、释放锁
### 动态创建锁资源
在使用`synchronized`关键字的时候，必须指定一个锁对象。


```
synchronized(obj) {
}
```
进程内的线程可以基于obj来实现同步。obj在这里可以理解为一个锁对象。如果线程要进入`synchronized`代码块里，必须先持有obj对象上的锁。这种锁是JAVA里面的内置锁，创建的过程是线程安全的。那么借助DB，如何保证创建锁的过程是线程安全的呢？可以利用DB中的`UNIQUE KEY`特性，一旦出现了重复的key，由于`UNIQUE KEY`的唯一性，会抛出异常的。在JAVA里面，是`SQLIntegrityConstraintViolationException`异常。


```
create table distributed_lock
(
    id BIGINT UNSIGNED PRIMARY KEY AUTO_INCREMENT COMMENT '自增主键',
    transaction_id varchar(128) NOT NULL DEFAULT '' COMMENT '事务id',
    last_update_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP NOT NULL COMMENT '最后更新时间',
    create_time TIMESTAMP DEFAULT '0000-00-00 00:00:00' NOT NULL COMMENT '创建时间',
    UNIQUE KEY `idx_transaction_id` (`transaction_id`)
)
```
`transaction_id`是事务Id，比如说，可以用
> 
仓库 + 条码 + 销售模式
来组装一个`transaction_id`，表示某仓库某销售模式下的某个条码资源。不同条码，当然就有不同的`transaction_id`。如果有两个应用，拿着相同的`transaction_id`来创建锁资源的时候，只能有一个应用创建成功。
> 
一条`distributed_lock`记录插入成功了，就表示一份锁资源创建成功了。
### DB连接池列表设计
在写操作频繁的业务系统中，通常会进行分库，以降低单数据库写入的压力，并提高写操作的吞吐量。如果使用了分库，那么业务数据自然也都分配到各个数据库上了。在这种水平切分的多数据库上使用DB分布式锁，可以自定义一个`DataSouce`列表。并暴露一个`getConnection(String transactionId)`方法，按照`transactionId`找到对应的`Connection`。
实现代码如下：


```
package dlock;
import com.alibaba.druid.pool.DruidDataSource;
import org.springframework.stereotype.Component;
import javax.annotation.PostConstruct;
import java.io.FileInputStream;
import java.io.IOException;
import java.sql.Connection;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;
@Component
public class DataSourcePool {
    private List dlockDataSources = new ArrayList();
    @PostConstruct
    private void initDataSourceList() throws IOException {
        Properties properties = new Properties();
        FileInputStream fis = new FileInputStream("db.properties");
        properties.load(fis);
        Integer lockNum = Integer.valueOf(properties.getProperty("DLOCK_NUM"));
        for (int i = 0; i "DLOCK_USER_" + i);
            String password = properties.getProperty("DLOCK_PASS_" + i);
            Integer initSize = Integer.valueOf(properties.getProperty("DLOCK_INIT_SIZE_" + i));
            Integer maxSize = Integer.valueOf(properties.getProperty("DLOCK_MAX_SIZE_" + i));
            String url = properties.getProperty("DLOCK_URL_" + i);
            DruidDataSource dataSource = createDataSource(user,password,initSize,maxSize,url);
            dlockDataSources.add(dataSource);
        }
    }
    private DruidDataSource createDataSource(String user, String password, Integer initSize, Integer maxSize, String url) {
        DruidDataSource dataSource = new DruidDataSource();
        dataSource.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource.setUsername(user);
        dataSource.setPassword(password);
        dataSource.setUrl(url);
        dataSource.setInitialSize(initSize);
        dataSource.setMaxActive(maxSize);
        return dataSource;
    }
    public Connection getConnection(String transactionId) throws Exception {
        if (dlockDataSources.size() 0) {
            return null;
        }
        if (transactionId == null || "".equals(transactionId)) {
            throw new RuntimeException("transactionId是必须的");
        }
        int hascode = transactionId.hashCode();
        if (hascode 0) {
            hascode = - hascode;
        }
        return dlockDataSources.get(hascode % dlockDataSources.size()).getConnection();
    }
}
```
首先编写一个`initDataSourceList`方法，并利用Spring的`PostConstruct`注解初始化一个`DataSource` 列表。相关的DB配置从`db.properties`读取。


```
DLOCK_NUM=2
DLOCK_USER_0="user1"
DLOCK_PASS_0="pass1"
DLOCK_INIT_SIZE_0=2
DLOCK_MAX_SIZE_0=10
DLOCK_URL_0="jdbc:mysql://localhost:3306/test1"
DLOCK_USER_1="user1"
DLOCK_PASS_1="pass1"
DLOCK_INIT_SIZE_1=2
DLOCK_MAX_SIZE_1=10
DLOCK_URL_1="jdbc:mysql://localhost:3306/test2"
```
`DataSource`使用阿里的`DruidDataSource`。
接着最重要的一个实现`getConnection(String transactionId)`方法。实现原理很简单，获取`transactionId`的hashcode，并对`DataSource`的长度取模即可。
连接池列表设计好后，就可以实现往`distributed_lock`表插入数据了。


```
package dlock;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import java.sql.*;
@Component
public class DistributedLock {
    @Autowired
    private DataSourcePool dataSourcePool;
    /**
     * 根据transactionId创建锁资源
     */
    public String createLock(String transactionId) throws Exception{
        if (transactionId == null) {
            throw new RuntimeException("transactionId是必须的");
        }
        Connection connection = null;
        Statement statement = null;
        try {
            connection = dataSourcePool.getConnection(transactionId);
            connection.setAutoCommit(false);
            statement = connection.createStatement();
            statement.executeUpdate("INSERT INTO distributed_lock(transaction_id) VALUES ('" + transactionId + "')");
            connection.commit();
            return transactionId;
        }
        catch (SQLIntegrityConstraintViolationException icv) {
            //说明已经生成过了。
            if (connection != null) {
                connection.rollback();
            }
            return transactionId;
        }
        catch (Exception e) {
            if (connection != null) {
                connection.rollback();
            }
            throw  e;
        }
        finally {
            if (statement != null) {
                statement.close();
            }
            if (connection != null) {
                connection.close();
            }
        }
    }
}
```
### 根据transactionId锁住线程
接下来利用DB的`select for update`特性来锁住线程。当多个线程根据相同的`transactionId`并发同时操作`select for update`的时候，只有一个线程能成功，其他线程都`block`住，直到`select for update`成功的线程使用`commit`操作后，`block`住的所有线程的其中一个线程才能开始干活。我们在上面的`DistributedLock`类中创建一个`lock`方法。


```
public boolean lock(String transactionId) throws Exception {
        Connection connection = null;
        PreparedStatement preparedStatement = null;
        ResultSet resultSet = null;
        try {
            connection = dataSourcePool.getConnection(transactionId);
            preparedStatement = connection.prepareStatement("SELECT * FROM distributed_lock WHERE transaction_id = ? FOR UPDATE ");
            preparedStatement.setString(1,transactionId);
            resultSet = preparedStatement.executeQuery();
            if (!resultSet.next()) {
                connection.rollback();
                return false;
            }
            return true;
        } catch (Exception e) {
            if (connection != null) {
                connection.rollback();
            }
            throw  e;
        }
        finally {
            if (preparedStatement != null) {
                preparedStatement.close();
            }
            if (resultSet != null) {
                resultSet.close();
            }
            if (connection != null) {
                connection.close();
            }
        }
    }
```
### 实现解锁操作
当线程执行完任务后，必须手动的执行解锁操作，之前被锁住的线程才能继续干活。在我们上面的实现中，其实就是获取到当时`select for update`成功的线程对应的`Connection`，并实行`commit`操作即可。
那么如何获取到呢？我们可以利用`ThreadLocal`。首先在`DistributedLock`类中定义


```
private ThreadLocal threadLocalConn = new ThreadLocal();
```
每次调用`lock`方法的时候，把`Connection`放置到`ThreadLocal`里面。我们修改`lock`方法。


```
public boolean lock(String transactionId) throws Exception {
        Connection connection = null;
        PreparedStatement preparedStatement = null;
        ResultSet resultSet = null;
        try {
            connection = dataSourcePool.getConnection(transactionId);
            threadLocalConn.set(connection);
            preparedStatement = connection.prepareStatement("SELECT * FROM distributed_lock WHERE transaction_id = ? FOR UPDATE ");
            preparedStatement.setString(1,transactionId);
            resultSet = preparedStatement.executeQuery();
            if (!resultSet.next()) {
                connection.rollback();
                threadLocalConn.remove();
                return false;
            }
            return true;
        } catch (Exception e) {
            if (connection != null) {
                connection.rollback();
                threadLocalConn.remove();
            }
            throw  e;
        }
        finally {
            if (preparedStatement != null) {
                preparedStatement.close();
            }
            if (resultSet != null) {
                resultSet.close();
            }
            if (connection != null) {
                connection.close();
            }
        }
    }
```
这样子，当获取到`Connection`后，将其设置到`ThreadLocal`中，如果`lock`方法出现异常，则将其从`ThreadLocal`中移除掉。
有了这几步后，我们可以来实现解锁操作了。我们在`DistributedLock`添加一个`unlock`方法。


```
public void unlock() throws Exception {
        Connection connection = null;
        try {
            connection = threadLocalConn.get();
            if (!connection.isClosed()) {
                connection.commit();
                connection.close();
                threadLocalConn.remove();
            }
        } catch (Exception e) {
            if (connection != null) {
                connection.rollback();
                connection.close();
            }
            threadLocalConn.remove();
            throw e;
        }
    }
```
### 缺点
毕竟是利用DB来实现分布式锁，对DB还是造成一定的压力。当时考虑使用DB做分布式的一个重要原因是，我们的应用是后端应用，平时流量不大的，反而关键的是要保证库存数据的正确性。对于像前端库存系统，比如添加购物车占用库存等操作，最好别使用DB来实现分布式锁了。
### 进一步思考
如果想锁住多份数据该怎么实现？比如说，某个库存操作，既要修改物理库存，又要修改虚拟库存，想锁住物理库存的同时，又锁住虚拟库存。其实也不是很难，参考`lock`方法，写一个`multiLock`方法，提供多个`transactionId`的入参，for循环处理就可以了。这个后续有时间再补上。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/07/54f7206fd1f1907bec78ddf32f50d66d.png)
