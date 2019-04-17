# Spring JdbcTemplate No value specified for parameter 6 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月09日 15:01:20[boonya](https://me.csdn.net/boonya)阅读数：5972








很久没有在正式项目用Spring JdbcTemplate 来做测试了，看了一个别人的项目，用的是Spring 的JdbcTemplate 访问数据库。

具体Java代码如下：



```java
public class UserDaoImpl implements UserDao {

    @Autowired
    private JdbcTemplate jdbcTemplate;
    
    public User createUser(final User user) {
        final String sql = "insert into sys_user(organization_id, username, password, salt, locked) values(?,?,?,?,?,?)";

        GeneratedKeyHolder keyHolder = new GeneratedKeyHolder();
        jdbcTemplate.update(new PreparedStatementCreator() {
            @Override
            public PreparedStatement createPreparedStatement(Connection connection) throws SQLException {
                PreparedStatement psst = connection.prepareStatement(sql, new String[]{"id"});
                int count = 1;
                psst.setLong(count++, user.getOrganizationId());
                psst.setString(count++, user.getUsername());
                psst.setString(count++, user.getPassword());
                psst.setString(count++, user.getSalt());
                psst.setBoolean(count++, user.getLocked());
                return psst;
            }
        }, keyHolder);

        user.setId(keyHolder.getKey().longValue());
        return user;
    }
```
比对参数发现只有5个字段，结果values却需要6个，因为有6个？只需要去掉一个'?'即可。






