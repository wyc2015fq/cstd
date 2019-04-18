# JDBC批处理 - z69183787的专栏 - CSDN博客
2012年12月28日 09:56:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：871
在对数据库进行批量操作时，应分析操作的前后相关性，如果属于大批量的操作，而且前续操作的结果不依赖与后继操作，则完全可以使用批处理来操作DB。
使用批处理的**优点**：
1.  多个SQL语句的执行，共用一个Connection资源。在对数据库操作时，connection资源是很宝贵的，数据库的维护从某种角度来说，就是减少数据库的连接数，减轻对DB的压力。创建一个数据连接要远远比使用数据库连接消耗资源。这也正是数据库连接池存在的意义。
2. 批处理在效率上总是比逐条处理有优势，要处理的数据的记录条数越大，批处理的优势越明显。在处理大批量相同业务逻辑的DB操作可以使用批处理达到简化、高效处理。
3. 在单一时间段，提高应用与DB间的吞吐量，缩短DB响应时间。大部分应用都有DB操作，如果SQL语句操作不当会导致DB长时间处于不可用状态，或是使DB资源占用率升高，从而影响了应用，最终被DB拖垮。缩短DB的响应时间，一来可以提供应用性能，二来减轻DB压力，对维持高系能的应用有极大的帮助。
使用JDBC本身提供的批处理功能很简单，如下例子是根据主键批量更新test_table表。
public void updateStateBactch(List elms) {  
        Connection conn = null;  
        PreparedStatement ps = null;  
        String sql = "update test_table set state=? where keyid = ?";    
        conn = DBTools.getConnection();     
        if(conn == null){  
                log.error("[update][state][error][conn is null]");  
                return;  
        }  
        try {  
                ps = conn.prepareStatement(sql);  
                for(int i = 0; i < elms.size(); i++) {  
                        Element elm = (Element) elms.get(i);  
                        if(null == elm || null == elm.getUserId()   || null == elm.getState()) {  
                                continue;  
                        }  
                        ps.setInt(1, elm.getStatus());  
                        ps.setString(2, elm.getProcID());  
                        ps.addBatch();  
                }  
                ps.executeBatch();  
                ps.clearBatch();  
        } catch (SQLException sqlEx) {  
                log.warn("[update][state][error][SQLException]");  
        } catch (Exception e) {  
                log.warn("[update][state][error][SQLException]");  
        } finally {  
                DBTools.close(conn, ps, null);  
        }  
}  
使用批处理是有绝对的好处，可是凡事都利必有弊。在使用批处理应用注意以下几点，这些小的细节常常被我们忽略，但是这些**细节**都对你应用的性能有着至关重要的影响。
1. 使用批出量没有进行分批分量处理。在使用批处理首先应该注意一点，批处理也不是万能的，批处理都存在一次执行的最大吞吐量限制。正如上面所提到的，批处理在单一时间段提高了与DB间的吞吐量，但是任何DB都是有吞吐量最大限制。当达到或是超过，最大吞吐量的峰值时，容易导致DB过载，更严重会导致DB宕机。例如上面的示例代码，如果入参list长度很大，几万甚至几十万，想想会导致上面结果呢。当然是背道而驰使应用的系能急剧下降，而且给DB带来风险。正确的做法应该是分批分量进行提交。处理执行SQL的时候，批处理的分批的大小与数据库的吞吐量以及硬件配置有很大关系，需要通过测试找到最佳的分批大小，一般在200-2000之间。
try {  
        ps = conn.prepareStatement(sql);  
        for(int i = 0; i < elms.size(); i++) {  
                Element elm = (Element) elms.get(i);  
                if(null == elm || null == elm.getUserId() || null == elm.getState()) {  
                        continue;  
                }  
                ps.setInt(1, elm.getStatus());  
                ps.setString(2, elm.getProcID());  
                ps.addBatch();  
                if ((i != 0 && i % 2000 == 0) || i == elms.size() - 1) {  
                        ps.executeBatch();  
                        ps.clearBatch();  
                        ps.close();  
                        ps = conn.prepareStatement(sql);  
                }  
        }  
  } catch (SQLException sqlEx) {  
        log.warn("[update][state][error][SQLException]");  
        log.warn(sqlEx);  
  } catch (Exception e) {  
        log.warn("[update][state][error][SQLException]");  
        log.warn(e);  
  } finally {  
        DBTools.close(conn, ps, null);  
  } 
2. 使用批处理时，没有关注DB测异常情况，导致批处理失败。这里涉及到一些异常处理最基本的点。上述例程还有个小小的问题需要注意，当ps.executeBatch()执行时，如果该批次的SQL语句中有一条SQL抛出异常，那么后续的批处理将不会有执行的机会，导致漏执行。所以经过优化后：
try {  
        ps = conn.prepareStatement(sql);  
        for(int i = 0; i < elms.size(); i++) {  
        try {   
                Element elm = (Element) elms.get(i);  
                if(null == elm || null == elm.getUserId() || null == elm.getState()) {  
                        continue;  
                }  
                ps.setInt(1, elm.getStatus());  
                ps.setString(2, elm.getProcID());  
                ps.addBatch();  
                if ((i != 0 && i % 2000 == 0) || i == elms.size() - 1) {  
                        ps.executeBatch();  
                        ps.clearBatch();  
                        ps.close();  
                        ps = conn.prepareStatement(sql);  
                }  
} catch (SQLException e) {  
        log.warn("[update][state][error][SQLException]");  
        log.warn(e);  
        ps.clearBatch();  
        ps.close();  
        ps = conn.prepareStatement(sql);  
} catch (Exception e) {  
        log.warn("[update][state][error][SQLException]");  
        log.warn(e);  
        ps.executeBatch();  
        ps.clearBatch();  
        ps.close();  
        ps = conn.prepareStatement(sql);  
} finally {  
        DBTools.close(conn, ps, null); 
}  
3. 使用批处理时，当批处理中有一条SQL语句优化SQL异常而导致整个批处理失败。在打印日志时应该注意，以上的打印方式对问题定位没有任何帮助。如上如果其中的一条SQL语句执行失败，那么你不知道究竟是什么异常，因为没有打印异常列，而只打印了最顶层异常。例如：如上程序在DB2数据库中执行失败后，只返回了如下信息com.ibm.db2.jcc.c.vd: Non-atomic batch failure.  The batch was submitted, but at least one exception occurred
 on an individual member of the batch. Use getNextException() to retrieve the exceptions for specific batched elements.大概意思是批处理执行失败，批处理已经提交，但是其中至少有一条或者多条执行失败。使用getNextException来查看执行白失败的SQL语句异常信息，便于定位！可是这样还是有问题，你知道了SQL语句的异常了，但是你不知道究竟是那条SQL语句导致的异常，其实可以更具批处理执行的返回值来检查执行结果。
Int[] results = ps.executeBatch();
