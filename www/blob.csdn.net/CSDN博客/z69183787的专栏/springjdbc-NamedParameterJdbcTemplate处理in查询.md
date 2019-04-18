# springjdbc-NamedParameterJdbcTemplate处理in查询 - z69183787的专栏 - CSDN博客
2014年12月11日 19:04:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4848
*原文*[http://blog.csdn.net/brianyeye/article/details/37650461](http://blog.csdn.net/brianyeye/article/details/37650461)
```java
package com.test;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.jdbc.core.namedparam.BeanPropertySqlParameterSource;
import org.springframework.jdbc.core.namedparam.MapSqlParameterSource;
import org.springframework.jdbc.core.namedparam.NamedParameterJdbcTemplate;
import com.java.common.util.BeanFactory;
import com.java.ssmb.entity.domain.UserInfo;
/**
 * 使用NamedParameterJdbcTemplate查询条件sql中in查询和javabean查询
 * 一般spring项目中都存在JdbcTemplate，
 * 例子使用JdbcTemplate创建NamedParameterJdbcTemplate
 * 
 * @author yeshun
 *
 */
public class NamedParameterJdbcTemplateTest {
  private NamedParameterJdbcTemplate namedJdbcTemplate;
  
  public NamedParameterJdbcTemplateTest(){
    JdbcTemplate jdbcTemplate = BeanFactory.getBean("jdbcTemplate",JdbcTemplate.class);
    this.namedJdbcTemplate = new NamedParameterJdbcTemplate(jdbcTemplate);
  }
  
  /*
   * 使用map查询
   */
  public void queryListByMap(List<String> userids){
    String sql = "select * from user_table u where u.user_id in (:userids )";
    
    Map<String,Object> parameters = new HashMap<String,Object>();
    parameters.put("userids", userids);
    
    List<Map<String,Object>> list = namedJdbcTemplate.queryForList(sql, parameters);
    
    for (Map<String, Object> map : list) {
      System.out.println("-------------------");
      System.out.println(map.get("user_id"));
      System.out.println(map.get("user_name"));
    }
  }
  
  /*
   * 使用MapSqlParameterSource查询
   */
  public void queryListByMapSqlParameterSource(List<String> userids){
    String sql = "select * from user_table u where u.user_id in (:userids )";
    
    MapSqlParameterSource parameters = new MapSqlParameterSource();
    parameters.addValue("userids", userids);
    
    List<Map<String,Object>> list = namedJdbcTemplate.queryForList(sql, parameters);
    
    for (Map<String, Object> map : list) {
      System.out.println("-------------------");
      System.out.println(map.get("user_id"));
      System.out.println(map.get("user_name"));
    }
  }
  
  /*
   * 使用BeanPropertySqlParameterSource查询，javabean查询
   */
  public void querySingleByBeanPropertySqlParameterSource(){
    //:userId -- 对应UserInfo.userId
    String sql = "select * from user_table u where u.user_id = :userId ";
    
    UserInfo user = new UserInfo();
    user.setUserId("user0");
    BeanPropertySqlParameterSource parameters = new BeanPropertySqlParameterSource(user);
    
    Map<String,Object> map = namedJdbcTemplate.queryForMap(sql, parameters);
    
    System.out.println(map.get("user_id"));
    System.out.println(map.get("user_name"));
    
  }
  
  /*
   * 查询单个返回值
   */
  public void querySingle(List<String> userids){
    String sql = "select count(*) from user_table u where u.user_id in (:userids )";
    MapSqlParameterSource parameters = new MapSqlParameterSource();
    parameters.addValue("userids", userids);
    
    Integer count = namedJdbcTemplate.queryForObject(sql, parameters, Integer.class);
    // queryForInt 
//		Integer count = namedJdbcTemplate.queryForInt(sql, parameters);
    System.out.println(count);
  }
  
  /*
   * 使用MapSqlParameterSource查询,使用RowMapper返回
   */
  public void queryUsersByMapSqlParameterSource(List<String> userids){
    String sql = "select * from user_table u where u.user_id in (:userids )";
    
    MapSqlParameterSource parameters = new MapSqlParameterSource();
    parameters.addValue("userids", userids);
    
    List<UserInfo> userList = namedJdbcTemplate.query(sql, parameters,new RowMapper<UserInfo>(){
      @Override
      public UserInfo mapRow(ResultSet rs, int rowNum) throws SQLException {
        // TODO Auto-generated method stub
        UserInfo user = new UserInfo();
        user.setUserId(rs.getString("user_id"));
        user.setUserName(rs.getString("user_name"));
        user.setPassword(rs.getString("password"));
        return user;
      }
    });
    
    for (UserInfo userInfo : userList) {
      System.out.println(userInfo.getUserId()+"--"+userInfo.getUserName());
    }
  }
  
  public static void main(String[] args) {
    NamedParameterJdbcTemplateTest test = new NamedParameterJdbcTemplateTest();
    List<String> userids= new ArrayList<String>();
    userids.add("user0");
    userids.add("user1");
    userids.add("user2");
    userids.add("user3");
    
//		test.queryListByMap(userids);
//		test.queryListByMapSqlParameterSource(userids);
//		test.querySingle(userids);
//		test.querySingleByBeanPropertySqlParameterSource();
    test.queryUsersByMapSqlParameterSource(userids);
  }
}
```
