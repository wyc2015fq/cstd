# JdbcTemplate 批量处理数据 - z69183787的专栏 - CSDN博客
2012年12月28日 10:24:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7291
核心方法：
batchUpdate(String sql,BatchPreparedStatementSetter pss)
 Spring JdbcTemplate 批量插入或更新操作
用 JdbcTemplate 进行批量插入或更新操作
方法一：
代码只是测试用的一个小例子。
public int insertContractAch(List list) throws DataAccessException {   
    final List temList = list;   
    String sql = "insert into contract_ach_t " +   
            " values(?,to_date(?,'yyyy-mm-dd'),?,?) ";   
    try{   
        int[] ii = this.getJdbcTemplate().batchUpdate(sql, new MyBatchPreparedStatementSetter(temList));   
        return ii.length;   
    }catch (org.springframework.dao.DataAccessException e) {   
        e.printStackTrace();   
        throw new DataAccessException(e.getMessage());   
    }   
}   
/** 
* 处理批量插入的回调类 
* */   
private class MyBatchPreparedStatementSetter implements BatchPreparedStatementSetter{   
    final List temList;   
    /**通过构造函数把要插入的数据传递进来处理*/   
    public MyBatchPreparedStatementSetter(List list){   
        temList = list;   
    }   
    public int getBatchSize() {   
        return temList.size();   
    }   
    public void setValues(PreparedStatement ps, int i)   
            throws SQLException {   
        ContractAchVO contractAchVO = (ContractAchVO)temList.get(i);   
        ps.setString(1, contractAchVO.getContractCode());   
        ps.setString(2, contractAchVO.getCreateDate());   
        ps.setString(3, contractAchVO.getEmployeeId());   
        ps.setString(4, contractAchVO.getPercent());   
    }   
} 
方法二：
//插入很多书(批量插入用法) 
public void insertBooks(List<Book> book) 
{ 
  final List<Book> tempBook=book; 
  String sql="insert into book(name,pbYear) values(?,?)"; 
  jdbcTemplate.batchUpdate(sql, new BatchPreparedStatementSetter() 
  { 
   public void setValues(PreparedStatement ps,int i)throws SQLException 
   { 
    String name=tempBook.get(i).getName(); 
    int pbYear=tempBook.get(i).getPbYear(); 
    ps.setString(1, name); 
    ps.setInt(2, pbYear); 
   } 
   public int getBatchSize() 
   { 
    return tempBook.size(); 
   } 
  }); 
} 
自己的例子：
```java
/**
	 * 待办事项插入sql
	 */
	private static final String sqlInsert = "insert into t_todo_item(app,type,key,occurtime,title,data,userid,loginname,status) values(?,?,?,?,?,?,?,?,?)";
	
	/**
	 * 业务表扫描状态修改
	 */
	private static final String sqlUpdate = "update tasks t set t.scaned = 1 where t.taskid = ?";
	
	
	private JdbcTemplate jt;
	
	static Logger log = SimpleLogger.getLogger(DeptContactTodoInstance.class);
	
	
	@Override
	public void action(JdbcTemplate jt) {
		this.jt = jt;
		
		final List<Map<String,String>> list = fetchData();
		
		if(list.size()==0){
			log.debug("no data!");
			return;
		}
		
		BatchPreparedStatementSetter pssInsert = new BatchPreparedStatementSetter() {
			@Override
			public int getBatchSize() {
				return list.size();
			}
			@Override
			public void setValues(PreparedStatement ps, int i) throws SQLException {
				Map<String,String> map = list.get(i);
				
				String taskid = StringUtil.getNotNullValueString(map.get("taskid"));
				String theme = StringUtil.getNotNullValueString(map.get("theme"));
				String loginname = StringUtil.getNotNullValueString(map.get("assignedtouser"));
				String occurTime = StringUtil.getNotNullValueString(map.get("start_time"));
				
				map.put("content", StringUtil.subStr(map.get("content"), 100));
				
				ps.setString(1, appName);
				ps.setInt(2, TodoConstants.TYPE_DEPTCONTACT);
				ps.setString(3, taskid);
				ps.setString(4, occurTime);
				ps.setString(5, theme);
				ps.setString(6, gson.toJson(map));
				
				/*
				 * TODO 查询用户ID
				 */
				ps.setString(7, "");
				ps.setString(8, loginname);
				
				ps.setInt(9, TodoConstants.STATUS_TODO);
			}
		};
		
		jt.batchUpdate(sqlInsert, pssInsert);
		
		BatchPreparedStatementSetter pssUpdate = new BatchPreparedStatementSetter() {
			@Override
			public int getBatchSize() {
				return list.size();
			}
			@Override
			public void setValues(PreparedStatement ps, int i) throws SQLException {
				Map<String,String> map = list.get(i);
				String taskid = StringUtil.getNotNullValueString(map.get("taskid"));
				ps.setString(1, taskid);
			}
		};
		
		jt.batchUpdate(sqlUpdate, pssUpdate);
		
log.debug(list.size()+" records insert/update!");
	}
```
