# JdbcTemplate数据批量删除 - z69183787的专栏 - CSDN博客
2012年12月28日 10:37:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11830
个人分类：[Spring-知识																[批量处理																[Spring-模板类](https://blog.csdn.net/z69183787/article/category/2176815)](https://blog.csdn.net/z69183787/article/category/2176819)](https://blog.csdn.net/z69183787/article/category/2175163)
**由于数据的批量处理（比如往RDBMS插入几万条数据）非常耗资源，因此建议在所有场合能够借助于JDBC完成，而不要去采用Hibernate API。本文主要围绕Spring内置的各类JDBC API抽象支持展开的。**
**1.1.  JdbcTemplate内置的batchUpdate(final String[] sql)**
**这一方法适合于小批量数据的CUD（增、更新、删除）操作，而且SQL类型不限。由于其内部使用了Statement语句，所以数据的操作效率一般。**
**1.2.  JdbcTemplate内置的batchUpdate(String sql, final BatchPreparedStatementSetter pss)**
**这一方法也仅仅适合于小批量数据的CUD（增、更新、删除）操作，但始终是同一SQL（参数具有多样性）。由于其内部使用了PreparedStatement语句，所以数据的操作效率还是不错的。下面给出了操作示例：**
**final int[] no = new int[]{7369,7499,7521,7566,7654,7698};**
**jt.batchUpdate("update emp set sal = ? where empno = ?",**
**          new BatchPreparedStatementSetter(){**
**                    public void setValues(PreparedStatement ps, int i)**
**throws SQLException {**
**                        ps.setInt(1, no[i]);**
**                        ps.setFloat(2, no[i]);**
**                   }**
**                    public int getBatchSize() {**
**                        return no.length;**
**                    }**
**          });**
**1.3.  BatchSqlUpdate**
**这一方法适合于各种批量数据的CUD（增、更新、删除）操作，但始终是同一SQL（参数具有多样性）。由于其内部使用了PreparedStatement语句，所以数据的操作效率还是不错的。下面给出了操作示例：**
**DataSource ds = gbfa.getBean("dataSource");**
**final int[] no = new int[]{7369,7499,7521,7566,7654,7698};**
**BatchSqlUpdate bsu = new BatchSqlUpdate(ds, "update emp set sal=? where empno = ?");**
**bsu.setBatchSize(4);**
**bsu.setTypes(new int[]{Types.FLOAT, Types.INTEGER});**
**for(int i = 0; i < no.length; ++i){**
**log.info(bsu.update(new Object[]{no[i],no[i]})) ;**
**}**
**bsu.flush();**
**       同JdbcTemplate内置的batchUpdate(String sql, final BatchPreparedStatementSetter pss)相比，BatchSqlUpdate会自动分批待批量处理的数据。比如，如果需要批量操作10万条数据，则可以控制其batchSize，从而在时间（RDBMS可知性）和空间（内存）上达到平衡。**
**       务必注意，在使用BatchSqlUpdate的最后，不要忘记手工调用其暴露的flush()方法。**
[**体验新版博客**](http://zhaopeng879.i.sohu.com/blog/view/80758932.htm)
