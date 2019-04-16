# SpringBoot整合Sharding-JDBC，实现从数据库读取sharding-jdbc数据源，实现多种数据库数据源切换，数据库方言动态切换 - 隔壁老王的专栏 - CSDN博客





置顶2019年03月21日 18:06:39[隔壁老王来也](https://me.csdn.net/haoyuyang)阅读数：40标签：[sharding-jdbc																[动态多数据源																[动态数据库方言](https://so.csdn.net/so/search/s.do?q=动态数据库方言&t=blog)
个人分类：[JAVA杂谈](https://blog.csdn.net/haoyuyang/article/category/6624402)





### SpringBoot整合Sharding-JDBC，实现从数据库读取sharding-jdbc数据源，实现多种数据库数据源切换，数据库方言动态切换

Hello，隔壁老王时隔很久很久又来了，今天给大家介绍的是一个分库分表的框架—sharding-jdbc。这个框架在我公司用到了，使用的还算比较复杂。这个项目有两种数据库，Mysql和SQLServer。因此需要动态的切换数据源，并且数据库方言也需要根据数据库类别进行切换，要不然就需要建两个一模一样的*Mapper.xml文件了。

在网上找了很久，很多的博客介绍的都是一种数据库类别。这也就导致了，找不到Spring的动态数据源与sharding-jdbc的整合。更别说数据库方言的切换了。

我一般不介绍概念之类的，需要看概念的，可以看[这里](http://cmsblogs.com/?cat=184)。讲的比较深入。废话不多说了，开整吧！

1、数据库中配置的sharing-jdbc的表结构

```
CREATE TABLE `sharding_data_source` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '自增主键',
  `rule_name` varchar(255) DEFAULT NULL COMMENT '环境前缀',
  `database_type` varchar(255) DEFAULT NULL COMMENT '0：Mysql  1：SQLServer',
  `url` varchar(255) DEFAULT NULL COMMENT '连接地址',
  `user_name` varchar(255) DEFAULT NULL COMMENT '数据源 用户名',
  `password` varchar(255) DEFAULT NULL COMMENT '数据源密码',
  `driver_class` varchar(255) DEFAULT NULL COMMENT '驱动全类名',
  `master_slave_type` varchar(255) DEFAULT NULL COMMENT '主(master)  从(slave)',
  `sharding_column` varchar(255) DEFAULT NULL COMMENT '分库的列',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;
```

```
CREATE TABLE `sharding_data_source_detail` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '自增id',
  `data_source_id` int(11) DEFAULT NULL COMMENT '数据源id',
  `logical_table` varchar(255) DEFAULT NULL COMMENT '逻辑表名',
  `sharding_column` varchar(255) DEFAULT NULL COMMENT '分表列',
  `actual_table` varchar(255) DEFAULT NULL COMMENT '物理表名',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;
```

其中sharding_data_source表用于存储数据库的相关信息，数据库地址、用户名、密码、数据库分片键之类的数据；sharding_data_source_detail则用于存储分表相关信息，逻辑表名、物理表名、表分片键。

2、程序启动时从数据库中获取数据源信息并创建sharding-jdbc数据源

先看一下sharding-jdbc与Spring动态数据源的整合

```java
@Configuration
public class DynamicDataSourceConfig {
    /**
     * 将动态数据源设置为主
     *
     * @param mysqlShardingDataSource
     * @param sqlServerShardingDataSource
     * @return
     */
    @Bean
    @Primary
    @DependsOn({"mysqlShardingDataSource", "sqlServerShardingDataSource"})
    public DynamicDataSource dataSource(@Qualifier("mysqlShardingDataSource") DataSource mysqlShardingDataSource,
                                        @Qualifier("sqlServerShardingDataSource") DataSource sqlServerShardingDataSource) {
        DynamicDataSource dynamicDataSource = new DynamicDataSource();
        Map<Object, Object> targetDataSources = new HashMap<>(2);
        targetDataSources.put(DatabaseType.MYSQL_LOWER_CASE, mysqlShardingDataSource);
        targetDataSources.put(DatabaseType.SQL_SERVER_LOWER_CASE, sqlServerShardingDataSource);
        dynamicDataSource.setTargetDataSources(targetDataSources);
        dynamicDataSource.setDefaultTargetDataSource(mysqlShardingDataSource);
        return dynamicDataSource;
    }

    /**
     * SQLServer数据源
     *
     * @param dataSourceTemplate
     * @param restTemplate
     * @return
     * @throws SQLException
     */
    @Bean("sqlServerShardingDataSource")
    public DataSource sqlServerDataSource(DataSourceTemplate dataSourceTemplate, RestTemplate restTemplate) throws SQLException {
        return dataSourceTemplate.getDataSource(DatabaseType.SQL_SERVER, restTemplate);
    }

    /**
     * Mysql数据源
     *
     * @param dataSourceTemplate
     * @param restTemplate
     * @return
     * @throws SQLException
     */
    @Bean("mysqlShardingDataSource")
    public DataSource mySqlDataSource(DataSourceTemplate dataSourceTemplate, RestTemplate restTemplate) throws SQLException {
        return dataSourceTemplate.getDataSource(DatabaseType.MYSQL, restTemplate);
    }

    /**
     * 配置数据库方言
     *
     * @return
     */
    @Bean
    public VendorDatabaseIdProvider databaseIdProvider() {
        VendorDatabaseIdProvider provider = new VendorDatabaseIdProvider();
        Properties properties = new Properties();
        properties.setProperty(DatabaseType.MYSQL_CAPITAL, DatabaseType.MYSQL_LOWER_CASE);
        properties.setProperty(DatabaseType.SQL_SERVER_CAPITAL,  DatabaseType.SQL_SERVER_LOWER_CASE);
        provider.setProperties(properties);
        return provider;
    }
    
    /**
     * 配置Mysql的SqlSessionFactory
     *
     * @param dataSource
     * @param provider
     * @return
     * @throws Exception
     */
    @Bean("mySqlSessionFactory")
    @Primary
    @ConditionalOnBean(DynamicDataSource.class)
    public SqlSessionFactory mySqlSessionFactory(@Qualifier("mysqlShardingDataSource") DataSource dataSource, DatabaseIdProvider provider) throws Exception {
        SqlSessionFactory sqlSessionFactory = getSqlSessionFactory(dataSource, provider);
        sqlSessionFactory.getConfiguration().setDatabaseId(DatabaseType.MYSQL_LOWER_CASE);
        return sqlSessionFactory;
    }

    /**
     * 配置SQLServer的SqlSessionFactory
     *
     * @param dataSource
     * @param provider
     * @return
     * @throws Exception
     */
    @Bean("sqlServerSessionFactory")
    @ConditionalOnBean(DynamicDataSource.class)
    public SqlSessionFactory sqlServerSessionFactory(@Qualifier("sqlServerShardingDataSource") DataSource dataSource, DatabaseIdProvider provider) throws Exception {
        SqlSessionFactory sqlSessionFactory = getSqlSessionFactory(dataSource, provider);
        sqlSessionFactory.getConfiguration().setDatabaseId(DatabaseType.SQL_SERVER_LOWER_CASE);
        return sqlSessionFactory;
    }

    /**
     * 配置SqlSessionTemplate为自定义的SqlSessionTemplate
     *
     * @param mySqlSessionFactory
     * @param sqlServerSessionFactory
     * @return
     */
    @Bean
    @ConditionalOnBean(DynamicDataSource.class)
    public SqlSessionTemplate sqlSessionTemplate(@Qualifier("mySqlSessionFactory") SqlSessionFactory mySqlSessionFactory,
                                                 @Qualifier("sqlServerSessionFactory") SqlSessionFactory sqlServerSessionFactory) {
        Map<Object, SqlSessionFactory> map = new HashMap<>(3);
        map.put(DatabaseType.MYSQL_LOWER_CASE, mySqlSessionFactory);
        map.put(DatabaseType.SQL_SERVER_LOWER_CASE, sqlServerSessionFactory);
        CustomSqlSessionTemplate sqlSessionTemplate = new CustomSqlSessionTemplate(mySqlSessionFactory);
        sqlSessionTemplate.setTargetSqlSessionFactorys(map);
        return sqlSessionTemplate;
    }

    private SqlSessionFactory getSqlSessionFactory(DataSource dataSource, DatabaseIdProvider provider) throws Exception {
        SqlSessionFactoryBean sessionFactoryBean = new SqlSessionFactoryBean();
        sessionFactoryBean.setDataSource(dataSource);
        sessionFactoryBean.setDatabaseIdProvider(provider);
        sessionFactoryBean.setMapperLocations(new PathMatchingResourcePatternResolver().getResources("classpath*:com/hyy/sharding/jdbc/mapper/*.xml"));
        return sessionFactoryBean.getObject();
    }

}
```

```
@Component
public class DataSourceTemplate {
    /**
     * 获取指定数据库类型的数据库信息列表
     *
     * @param databaseType
     * @param restTemplate
     * @return
     * @throws SQLException
     */
    public DataSource getDataSource(String databaseType, RestTemplate restTemplate) throws SQLException {
        DataSourceHandler.setDataSourceType(DatabaseType.BASE);
        ResponseEntity<List> responseEntity = restTemplate.getForEntity("http://base-datasource-service/base/datasource/" + databaseType + "/getDataSources", List.class);
        List<DataSourceEntity> dataSources = JSON.parseArray(JSON.toJSONString(responseEntity.getBody()), DataSourceEntity.class);
        Map<String, Map<String, DataSource>> masterSlaveDataSource = new HashMap<>();
        Map<String, DataSourceDetailEntity> map = getLogicalDataSourceDetailMap(dataSources, masterSlaveDataSource);

        Map<String, javax.sql.DataSource> dataSourceMap = new HashMap<>();

        ShardingRuleConfiguration shardingRuleConfig = getShardingRuleConfiguration(map, masterSlaveDataSource, dataSourceMap);
        Properties shardingProperties = getProperties();
        return ShardingDataSourceFactory.createDataSource(dataSourceMap, shardingRuleConfig, new HashMap<>(), shardingProperties);
    }


    /**
     * 获取属性配置
     *
     * @return
     */
    private Properties getProperties() {
        Properties shardingProperties = new Properties();
        shardingProperties.put("sql.show", true);
        return shardingProperties;
    }

    /**
     * 获取组装好的ShardingRuleConfiguration
     *
     * @param detailMap
     * @return
     */
    private ShardingRuleConfiguration getShardingRuleConfiguration(Map<String, DataSourceDetailEntity> detailMap, Map<String,
            Map<String, javax.sql.DataSource>> masterSlaveDataSource, Map<String, javax.sql.DataSource> dataSourceMap) {
        // 分片规则配置文件类
        ShardingRuleConfiguration shardingRuleConfig = new ShardingRuleConfiguration();
        for (String logicTable : detailMap.keySet()) {
            DataSourceDetailEntity dataSourceDetail = detailMap.get(logicTable);
            // 表分片规则配置类
            TableRuleConfiguration ruleConfig = new TableRuleConfiguration();

            // 分表策略
            ComplexShardingStrategyConfiguration databaseShardingStrategyConfig = new ComplexShardingStrategyConfiguration(dataSourceDetail.getDatabaseShardingColumn(),
                    ModuloDatabaseShardingAlgorithm.class.getName());

            // 设置分库策略，缺省表示使用默认分库策略，即ShardingRuleConfiguration中配置的分库策略
            ruleConfig.setDatabaseShardingStrategyConfig(databaseShardingStrategyConfig);

            // 分表规则
            ruleConfig.setTableShardingStrategyConfig(new ComplexShardingStrategyConfiguration(dataSourceDetail.getShardingColumn(),
                    ModuloTableShardingAlgorithm.class.getName()));

            // 真实的数据节点
            String actualDataNodes = dataSourceDetail.getActualDataNodes();
            /*
              1) 由数据源名 + 表名组成 以小数点分割，多个表以逗号隔开，支持inline表达式
              2) 缺省时，以已知 数据源名称和逻辑表名称生成数据节点
              3) 用于广播表 --即每个库中都需要一个同样的表进行关联查询，多为字典表
              4) 只分库不分表且所有库的表结构完全一致的情况
             */
            ruleConfig.setActualDataNodes(actualDataNodes);
            // 逻辑表名称
            ruleConfig.setLogicTable(logicTable);
            shardingRuleConfig.getTableRuleConfigs().add(ruleConfig);
        }

        for (String key : masterSlaveDataSource.keySet()) {
            Map<String, javax.sql.DataSource> temDatasourceMap = masterSlaveDataSource.get(key);
            MasterSlaveRuleConfiguration masterSlaveRuleConfiguration = new MasterSlaveRuleConfiguration();
            masterSlaveRuleConfiguration.setName(key);
            for (String tKey : temDatasourceMap.keySet()) {
                if (tKey.contains("master")) {
                    masterSlaveRuleConfiguration.setMasterDataSourceName(tKey);
                } else if (tKey.contains("slave")) {
                    masterSlaveRuleConfiguration.getSlaveDataSourceNames().add(tKey);
                }
                dataSourceMap.put(tKey, temDatasourceMap.get(tKey));
            }
            shardingRuleConfig.getMasterSlaveRuleConfigs().add(masterSlaveRuleConfiguration);
        }
        return shardingRuleConfig;
    }

    /**
     * Map<String, DataSourceDetail>  逻辑表名, 详细信息
     *
     * @param dataSources
     * @param masterSlaveDataSource
     * @return
     */
    private Map<String, DataSourceDetailEntity> getLogicalDataSourceDetailMap(List<DataSourceEntity> dataSources, Map<String, Map<String, DataSource>> masterSlaveDataSource) {
        Map<String, DataSourceDetailEntity> logicalMap = new HashMap<>();
        for (int i = 0; i < dataSources.size(); i++) {
            DataSourceEntity dataSource = dataSources.get(i);
            String dataSourceName = dataSource.getRuleName();

            //添加所有数据源
            if (masterSlaveDataSource.containsKey(dataSourceName)) {
                masterSlaveDataSource.get(dataSourceName).put(dataSource.getMasterSlaveType() + i, buildDataSource(dataSource));
            } else {
                Map<String, javax.sql.DataSource> stringDataSourceMap = new HashMap<>();
                masterSlaveDataSource.put(dataSourceName, stringDataSourceMap);
                stringDataSourceMap.put(dataSource.getMasterSlaveType() + i, buildDataSource(dataSource));
            }
            StringBuilder stringBuffer = new StringBuilder();

            for (DataSourceDetailEntity dataSourceDetail : dataSource.getDataSourceDetails()) {

                String logicalTable = dataSourceDetail.getLogicalTable().toLowerCase();
                String actualTable = dataSourceDetail.getActualTable().toLowerCase();
                if (!logicalMap.containsKey(logicalTable)) {
                    stringBuffer.append(dataSourceName).append(".").append(actualTable);
                    dataSourceDetail.setActualDataNodes(stringBuffer.toString());
                    dataSourceDetail.setDatabaseShardingColumn(dataSource.getShardingColumn());
                    logicalMap.put(logicalTable, dataSourceDetail);
                } else {
                    String actualDataNodes = logicalMap.get(logicalTable).getActualDataNodes();
                    stringBuffer.append(actualDataNodes).append(",").append(dataSourceName).append(".").append(actualTable);
                    logicalMap.get(logicalTable).setActualDataNodes(stringBuffer.toString());
                }
                stringBuffer.setLength(0);
            }
        }
        return logicalMap;
    }

    /**
     * 创建数据源
     *
     * @param dataSourceEntity
     * @return
     */
    private javax.sql.DataSource buildDataSource(DataSourceEntity dataSourceEntity) {
        DruidDataSource dataSource = new DruidDataSource();
        dataSource.setDriverClassName(dataSourceEntity.getDriverClass());
        dataSource.setUrl(dataSourceEntity.getUrl());
        dataSource.setUsername(dataSourceEntity.getUserName());
        dataSource.setPassword(dataSourceEntity.getPassword());
        dataSource.setMaxActive(100);
        dataSource.setInitialSize(10);
        dataSource.setTestWhileIdle(true);
        dataSource.setTestOnBorrow(false);
        dataSource.setTestOnReturn(false);
        dataSource.setMaxWait(10000);
        dataSource.setValidationQuery("select 'x'");
        dataSource.setTimeBetweenEvictionRunsMillis(60000);
        dataSource.setMinEvictableIdleTimeMillis(300000);
        dataSource.setRemoveAbandoned(true);
        dataSource.setRemoveAbandonedTimeout(80);
        return dataSource;
    }
}
```

因为需要在程序启动时就去查询数据库，但是这个时候，Spring的ApplicationContext还没有初始化，因为，没法通过注入的方式获取到*Mapper，也就没法直接通过*Mapper查询数据库了。

在这里，给大家提供以下两种解决方案：

（1）如果你的项目使用了Spring Cloud，那么可以通过上面的代码，调用其他服务来获取数据源信息。这种方式，就无法通过在RestTemplate上增加@LoadBalance来实现调用微服务了。因此在配置RsetTemplate时，可以使用如下方式：

```
@Bean
public RestTemplate restTemplate(final List<RestTemplateCustomizer> customizers) {
    RestTemplate restTemplate = new RestTemplate();
    for (RestTemplateCustomizer customizer : customizers) {
        customizer.customize(restTemplate);
    }
    return restTemplate;
}
```

（2）如果你的项目没有使用到Spring Cloud，那么就只能通过硬编码的方式来获取数据源信息了。具体配置方式如下：

```java
@Component
public class DataSourceTemplate {
    public static ConcurrentHashMap<String, List<DataSourceEntity>> concurrentHashMap = new ConcurrentHashMap<>();

    public DataSourceTemplate() {
        Connection connection = null;
        try {
            String driverClassName = "com.mysql.jdbc.Driver";
            String url = "xxxx";
            String username = "xxxx";
            String password = "xxxx";
            Class.forName(driverClassName);
            connection = DriverManager.getConnection(url, username, password);
            PreparedStatement preparedStatement = connection.prepareStatement("select * from sharding_data_source");
            ResultSet resultSet = preparedStatement.executeQuery();
            DataSourceEntity dataSource;
            List<DataSourceDetailEntity> dataSourceDetails;
            DataSourceDetailEntity dataSourceDetail;
            while (resultSet.next()) {
                dataSource = new DataSourceEntity();
                dataSource.setId(resultSet.getLong("id"));
                dataSource.setRuleName(resultSet.getString("rule_name"));
                dataSource.setDatabaseType(resultSet.getString("database_type"));
                dataSource.setUrl(resultSet.getString("url"));
                dataSource.setUserName(resultSet.getString("user_name"));
                dataSource.setPassword(resultSet.getString("password"));
                dataSource.setDriverClass(resultSet.getString("driver_class"));
                dataSource.setMasterSlaveType(resultSet.getString("master_slave_type"));
                dataSource.setShardingColumn(resultSet.getString("sharding_column"));
                preparedStatement = connection.prepareStatement("select * from sharding_data_source_detail where data_source_id = " + dataSource.getId());
                ResultSet resultSet1 = preparedStatement.executeQuery();
                dataSourceDetails = new ArrayList<>();
                while (resultSet1.next()) {
                    dataSourceDetail = new DataSourceDetail();
                    dataSourceDetail.setLogicalTable(resultSet1.getString("logical_table"));
                    dataSourceDetail.setShardingColumn(resultSet1.getString("sharding_column"));
                    dataSourceDetail.setActualTable(resultSet1.getString("actual_table"));
                    dataSourceDetails.add(dataSourceDetail);
                }
                dataSource.setDataSourceDetails(dataSourceDetails);
                concurrentHashMap.computeIfAbsent(dataSource.getDatabaseType(), k -> new ArrayList<>());
                concurrentHashMap.get(dataSource.getDatabaseType()).add(dataSource);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (connection != null) {
                try {
                    connection.close();
                } catch (SQLException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public DataSource getDataSource(String databaseType) throws SQLException {
	    DataSourceHandler.setDataSourceType(DatabaseType.BASE);
	    List<DataSourceEntity> dataSources = concurrentHashMap.get(databaseType);

	    Map<String, Map<String, DataSource>> masterSlaveDataSource = new HashMap<>();
	    Map<String, DataSourceDetailEntity> map = getLogicalDataSourceDetailMap(dataSources, masterSlaveDataSource);

	    Map<String, DataSource> dataSourceMap = new HashMap<>();

	    ShardingRuleConfiguration shardingRuleConfig = getShardingRuleConfiguration(map, masterSlaveDataSource, dataSourceMap);
	    Properties shardingProperties = getProperties();
	    return ShardingDataSourceFactory.createDataSource(dataSourceMap, shardingRuleConfig, new HashMap<>(), shardingProperties);
	}
}
```

其余代码与上面的一样，只是在实例化DataSourceTemplate类时，以硬编码的方式查询数据库，并将数据库信息放到ConcurrentHashMap中。后续需要某类型的数据库信息时，直接从ConcurrentHashMap中取就可以了。

到了这里，基本上可以实现数据源动态切换了。注意，只是可以切换数据源了。统统的Mapper文件，你还得写多份，以区分不同的数据库。如果在网上搜，你会发现让你配置VendorDatabaseIdProvider。在上面的配置，你会发现我们已经配置了，但是还是不起作用。那是因为网上的教程都是基于java.sql.DataSource的，因此只配置VendorDatabaseIdProvider是可以做到数据库方言根据数据源类型切换。但是，我们现在用的是Sharding-Jdbc的数据源，所以哪怕配置了VendorDatabaseIdProvider，也没法做到数据库方言的自动切换。我们还需要实现SqlSessionTemplate接口，重写getSqlSessionFactory方法。主要是为了根据线程中设置的数据类型返回对应的SqlSessionFactory。

```java
public class CustomSqlSessionTemplate extends SqlSessionTemplate {
    private final SqlSessionFactory sqlSessionFactory;
    private final ExecutorType executorType;
    private final SqlSession sqlSessionProxy;
    private final PersistenceExceptionTranslator exceptionTranslator;

    private Map<Object, SqlSessionFactory> targetSqlSessionFactorys;
    private SqlSessionFactory defaultTargetSqlSessionFactory;

    public void setTargetSqlSessionFactorys(Map<Object, SqlSessionFactory> targetSqlSessionFactorys) {
        this.targetSqlSessionFactorys = targetSqlSessionFactorys;
    }

    public void setDefaultTargetSqlSessionFactory(SqlSessionFactory defaultTargetSqlSessionFactory) {
        this.defaultTargetSqlSessionFactory = defaultTargetSqlSessionFactory;
    }

    public CustomSqlSessionTemplate(SqlSessionFactory sqlSessionFactory) {
        this(sqlSessionFactory, sqlSessionFactory.getConfiguration().getDefaultExecutorType());
    }

    private CustomSqlSessionTemplate(SqlSessionFactory sqlSessionFactory, ExecutorType executorType) {
        this(sqlSessionFactory, executorType, new MyBatisExceptionTranslator(sqlSessionFactory.getConfiguration()
                .getEnvironment().getDataSource(), true));
    }

    private CustomSqlSessionTemplate(SqlSessionFactory sqlSessionFactory, ExecutorType executorType,
                                     PersistenceExceptionTranslator exceptionTranslator) {

        super(sqlSessionFactory, executorType, exceptionTranslator);

        this.sqlSessionFactory = sqlSessionFactory;
        this.executorType = executorType;
        this.exceptionTranslator = exceptionTranslator;

        this.sqlSessionProxy = (SqlSession) newProxyInstance(
                SqlSessionFactory.class.getClassLoader(),
                new Class[] { SqlSession.class },
                new SqlSessionInterceptor());

        this.defaultTargetSqlSessionFactory = sqlSessionFactory;
    }

    @Override
    public SqlSessionFactory getSqlSessionFactory() {

        SqlSessionFactory targetSqlSessionFactory = targetSqlSessionFactorys.get(DataSourceHandler.getDataSourceType());
        if (targetSqlSessionFactory != null) {
            return targetSqlSessionFactory;
        } else if (defaultTargetSqlSessionFactory != null) {
            return defaultTargetSqlSessionFactory;
        } else {
            Assert.notNull(targetSqlSessionFactorys, "Property 'targetSqlSessionFactorys' or 'defaultTargetSqlSessionFactory' are required");
            Assert.notNull(defaultTargetSqlSessionFactory, "Property 'defaultTargetSqlSessionFactory' or 'targetSqlSessionFactorys' are required");
        }
        return this.sqlSessionFactory;
    }

    @Override
    public Configuration getConfiguration() {
        return this.getSqlSessionFactory().getConfiguration();
    }

    @Override
    public ExecutorType getExecutorType() {
        return this.executorType;
    }

    @Override
    public PersistenceExceptionTranslator getPersistenceExceptionTranslator() {
        return this.exceptionTranslator;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> T selectOne(String statement) {
        return this.sqlSessionProxy.<T> selectOne(statement);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> T selectOne(String statement, Object parameter) {
        return this.sqlSessionProxy.<T> selectOne(statement, parameter);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <K, V> Map<K, V> selectMap(String statement, String mapKey) {
        return this.sqlSessionProxy.<K, V> selectMap(statement, mapKey);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <K, V> Map<K, V> selectMap(String statement, Object parameter, String mapKey) {
        return this.sqlSessionProxy.<K, V> selectMap(statement, parameter, mapKey);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <K, V> Map<K, V> selectMap(String statement, Object parameter, String mapKey, RowBounds rowBounds) {
        return this.sqlSessionProxy.<K, V> selectMap(statement, parameter, mapKey, rowBounds);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <E> List<E> selectList(String statement) {
        return this.sqlSessionProxy.<E> selectList(statement);
    }

    /**
     * {@inheritDoc}
     */

    @Override
    public <E> List<E> selectList(String statement, Object parameter) {
        return this.sqlSessionProxy.<E> selectList(statement, parameter);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <E> List<E> selectList(String statement, Object parameter, RowBounds rowBounds) {
        return this.sqlSessionProxy.<E> selectList(statement, parameter, rowBounds);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void select(String statement, ResultHandler handler) {
        this.sqlSessionProxy.select(statement, handler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void select(String statement, Object parameter, ResultHandler handler) {
        this.sqlSessionProxy.select(statement, parameter, handler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void select(String statement, Object parameter, RowBounds rowBounds, ResultHandler handler) {
        this.sqlSessionProxy.select(statement, parameter, rowBounds, handler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int insert(String statement) {
        return this.sqlSessionProxy.insert(statement);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int insert(String statement, Object parameter) {
        return this.sqlSessionProxy.insert(statement, parameter);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int update(String statement) {
        return this.sqlSessionProxy.update(statement);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int update(String statement, Object parameter) {
        return this.sqlSessionProxy.update(statement, parameter);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int delete(String statement) {
        return this.sqlSessionProxy.delete(statement);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int delete(String statement, Object parameter) {
        return this.sqlSessionProxy.delete(statement, parameter);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> T getMapper(Class<T> type) {
        return getConfiguration().getMapper(type, this);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void commit() {
        throw new UnsupportedOperationException("Manual commit is not allowed over a Spring managed SqlSession");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void commit(boolean force) {
        throw new UnsupportedOperationException("Manual commit is not allowed over a Spring managed SqlSession");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void rollback() {
        throw new UnsupportedOperationException("Manual rollback is not allowed over a Spring managed SqlSession");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void rollback(boolean force) {
        throw new UnsupportedOperationException("Manual rollback is not allowed over a Spring managed SqlSession");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void close() {
        throw new UnsupportedOperationException("Manual close is not allowed over a Spring managed SqlSession");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void clearCache() {
        this.sqlSessionProxy.clearCache();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Connection getConnection() {
        return this.sqlSessionProxy.getConnection();
    }

    /**
     * {@inheritDoc}
     * @since 1.0.2
     */
    @Override
    public List<BatchResult> flushStatements() {
        return this.sqlSessionProxy.flushStatements();
    }

    /**
     * Proxy needed to route MyBatis method calls to the proper SqlSession got from Spring's Transaction Manager It also
     * unwraps exceptions thrown by {@code Method#invoke(Object, Object...)} to pass a {@code PersistenceException} to
     * the {@code PersistenceExceptionTranslator}.
     */
    private class SqlSessionInterceptor implements InvocationHandler {
        @Override
        public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
            final SqlSession sqlSession = getSqlSession(
                    CustomSqlSessionTemplate.this.getSqlSessionFactory(),
                    CustomSqlSessionTemplate.this.executorType,
                    CustomSqlSessionTemplate.this.exceptionTranslator);
            try {
                Object result = method.invoke(sqlSession, args);
                if (!isSqlSessionTransactional(sqlSession, CustomSqlSessionTemplate.this.getSqlSessionFactory())) {
                    // force commit even on non-dirty sessions because some databases require
                    // a commit/rollback before calling close()
                    sqlSession.commit(true);
                }
                return result;
            } catch (Throwable t) {
                Throwable unwrapped = unwrapThrowable(t);
                if (CustomSqlSessionTemplate.this.exceptionTranslator != null && unwrapped instanceof PersistenceException) {
                    Throwable translated = CustomSqlSessionTemplate.this.exceptionTranslator
                            .translateExceptionIfPossible((PersistenceException) unwrapped);
                    if (translated != null) {
                        unwrapped = translated;
                    }
                }
                throw unwrapped;
            } finally {
                closeSqlSession(sqlSession, CustomSqlSessionTemplate.this.getSqlSessionFactory());
            }
        }
    }
}
```

配置好后，我们的*Mapper.xml就只需要一份，相同的业务功能，指定对应的databaseId即可。

```
<mapper namespace="com.hyy.sharding.jdbc.mapper.CodeMapper">
    <select id="getCodes" resultType="java.lang.String" databaseId="mysql">
        SELECT CODE FROM coupon_seed LIMIT 10
    </select>

    <select id="getCodes" resultType="java.lang.String" databaseId="sqlServer">
        SELECT TOP 10 CODE FROM coupon_seed
    </select>
</mapper>
```

项目启动后，调用分别调用[http://localhost:8082/code/mysql/getCodes](http://localhost:8082/code/mysql/getCodes)和[http://localhost:8082/code/](http://localhost:8082/code/mysql/getCodes)sqlServer[/getCodes](http://localhost:8082/code/mysql/getCodes)，查看控制台输出：

![](https://img-blog.csdnimg.cn/20190322145302827.png)

![](https://img-blog.csdnimg.cn/20190322145327156.png)

demo看[这里](https://download.csdn.net/download/haoyuyang/11049889)](https://so.csdn.net/so/search/s.do?q=动态多数据源&t=blog)](https://so.csdn.net/so/search/s.do?q=sharding-jdbc&t=blog)




