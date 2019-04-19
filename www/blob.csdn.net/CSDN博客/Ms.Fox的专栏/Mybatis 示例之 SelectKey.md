# Mybatis 示例之 SelectKey - Ms.Fox的专栏 - CSDN博客
2019年03月13日 16:36:27[lulu-lu](https://me.csdn.net/smbluesky)阅读数：9
SelectKey在Mybatis中是为了解决Insert数据时不支持主键自动生成的问题，他可以很随意的设置生成主键的方式。
|属性|描述|
|----|----|
|keyProperty|selectKey 语句结果应该被设置的目标属性。|
|resultType|结果的类型。MyBatis 通常可以算出来,但是写上也没有问题。MyBatis 允许任何简单类型用作主键的类型,包括字符串。|
|order|这可以被设置为 BEFORE 或 AFTER。如果设置为 BEFORE,那么它会首先选择主键,设置 keyProperty 然后执行插入语句。如果设置为 AFTER,那么先执行插入语句,然后是 selectKey 元素-这和如 Oracle 数据库相似,可以在插入语句中嵌入序列调用。|
|statementType|和前面的相 同,MyBatis 支持 STATEMENT ,PREPARED 和CALLABLE 语句的映射类型,分别代表 PreparedStatement 和CallableStatement 类型。|
SelectKey需要注意order属性，像Mysql一类支持自动增长类型的数据库中，order需要设置为after才会取到正确的值。
像Oracle这样取序列的情况，需要设置为before，否则会报错。
另外在用Spring管理事务时，SelectKey和插入在同一事务当中，因而Mysql这样的情况由于数据未插入到数据库中，所以是得不到自动增长的Key。取消事务管理就不会有问题。
下面是一个xml和注解的例子，SelectKey很简单，两个例子就够了：
<insert id="insert" parameterType="map">  
    insert into table1 (name) values (#{name})  
    <selectKey resultType="java.lang.Integer" keyProperty="id">  
      CALL IDENTITY()  
    </selectKey>  
</insert> 
上面xml的传入参数是map，selectKey会将结果放到入参数map中。用POJO的情况一样，但是有一点需要注意的是，keyProperty对应的字段在POJO中必须有相应的setter方法，setter的参数类型还要一致，否则会报错。
@Insert("insert into table2 (name) values(#{name})")  
@SelectKey(statement="call identity()", keyProperty="nameId", before=false, resultType=int.class)  
int insertTable2(Name name);  
上面是注解的形式。
在使用ibatis插入数据进数据库的时候，会用到一些sequence的数据，有些情况下，在插入完成之后还需要将sequence的值返回，然后才能进行下一步的操作。 
      使用ibatis的selectKey就可以得到sequence的值，同时也会将值返回。不过对于不同的数据库有不同的操作方式。 
      对于oracle： 
<insert id="insertUser" parameterClass="ibatis.User"> 
          <selectKey resultClass="long" keyProperty="id"> 
              select SEQ_USER_ID.nextval as id from dual 
          </selectKey> 
           insert into user 
          (id,name,password) 
          values 
          (#id#,#name#,#password#) 
</insert> 
 该句话执行完之后，传进来的参数User对象DO里的id字段就会被赋值成sequence的值。 
对于mysql 
<insert id="insertUser" parameterClass="ibatis.User"> 
          insert into user 
          (name,password) 
          values 
          (#name#,#password#) 
          <selectKey resultClass="long" keyProperty="id">  
             SELECT LAST_INSERT_ID() AS ID  
        </selectKey>  
</insert> 
  将selectKey放在insert之后，通过LAST_INSERT_ID() 获得刚插入的自动增长的id的值。
<insert id="addMetaReport" parameterClass="metaRpo">  
        <![CDATA[  
            insert IGNORE into rpo_trackingMeta(trackingMeta_id, trackingMeta_title, company_id, subCompany_id, meta_type,  
            delegation_at, report_cycle, start_at, end_at, matched_num_new, matched_num_takeoff, matched_num_total,  
            created_at, created_by, updated_at, updated_by)  
            select    #trackingMetaId#  
                    , #metaTitle#  
                    , #companyId#  
                    , #subCompanyId#  
                    , #metaType#  
                    , #delegationAt#  
                    , #reportCycle#  
                    , #startAt#  
                    , #endAt#  
                    , sum(case when created_at >= #startAt# and created_at < #endAt# then 1 else 0 end)  
                    , 0  
                    , count(*)  
                    , now()  
                    , #createdBy#  
                    , now()  
                    , #updatedBy#  
          from matchedPage where task_id = #orderId#  
          and verification = 'mediadna';  
           ]]>  
          <selectKey keyProperty="id" resultClass="int">  
            SELECT IF(row_count() > 0, last_insert_id(), 0) AS id FROM dual  
        </selectKey>  
</insert>
