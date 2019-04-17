# Mybatis批量CRUD操作 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月26日 16:41:12[boonya](https://me.csdn.net/boonya)阅读数：574








CRUD即增、删、改、查的功能。

## 批量新增



```
<!-- 批量插入 -->  
  <insert id="insertBatch" useGeneratedKeys="true" keyColumn="CDSG_ID" >
	   insert into cd_wh_package_${tableName}  
	   (CDSG_ID, CDSG_CDSK_CODE, CDSG_NUMBER, CDSG_NAME, CDSG_MAIN_RELATION, CDSG_SUPERIOR_RELATION,CDSG_LENGTH, CDSG_WIDTH, CDSG_HEIGTH,
	   CDSG_WEIGHT, CDSG_IS_DECIMAL, CDSG_LEVEL,CDSG_BASE_NUMBER, CDSG_IS_MAIN_UNIT)   values   
	    <foreach collection="list" item="item" index="index" separator="," >   
	       (#{item.cdsgId,jdbcType=INTEGER}, #{item.cdsgCdskCode,jdbcType=VARCHAR}, #{item.cdsgNumber,jdbcType=VARCHAR}, 
            #{item.cdsgName,jdbcType=VARCHAR}, #{item.cdsgMainRelation,jdbcType=INTEGER}, #{item.cdsgSuperiorRelation,jdbcType=INTEGER}, 
      		#{item.cdsgLength,jdbcType=DECIMAL}, #{item.cdsgWidth,jdbcType=DECIMAL}, #{item.cdsgHeigth,jdbcType=DECIMAL}, 
      		#{item.cdsgWeight,jdbcType=DECIMAL}, #{item.cdsgIsDecimal,jdbcType=INTEGER}, #{item.cdsgLevel,jdbcType=INTEGER}, 
      		#{item.cdsgBaseNumber,jdbcType=INTEGER}, #{item.cdsgIsMainUnit,jdbcType=INTEGER}
      		)
	   </foreach>   
  </insert>
```



## 批量修改



```
<!-- 批量更新-->  
  <update id="updateBatch" >
       <foreach collection="list" item="item" index="index" open="" close="" separator=";" >
      update cd_wh_package_${tableName} 
      <set>
          CDSG_CDSK_CODE = #{item.cdsgCdskCode,jdbcType=VARCHAR},
	      CDSG_NUMBER = #{item.cdsgNumber,jdbcType=VARCHAR},
	      CDSG_NAME = #{item.cdsgName,jdbcType=VARCHAR},
	      CDSG_MAIN_RELATION = #{item.cdsgMainRelation,jdbcType=INTEGER},
	      CDSG_SUPERIOR_RELATION = #{item.cdsgSuperiorRelation,jdbcType=INTEGER},
	      CDSG_LENGTH = #{item.cdsgLength,jdbcType=DECIMAL},
	      CDSG_WIDTH = #{item.cdsgWidth,jdbcType=DECIMAL},
	      CDSG_HEIGTH = #{item.cdsgHeigth,jdbcType=DECIMAL},
	      CDSG_WEIGHT = #{item.cdsgWeight,jdbcType=DECIMAL},
	      CDSG_IS_DECIMAL = #{item.cdsgIsDecimal,jdbcType=INTEGER},
	      CDSG_LEVEL = #{item.cdsgLevel,jdbcType=INTEGER},
	      CDSG_BASE_NUMBER = #{item.cdsgBaseNumber,jdbcType=INTEGER},
	      CDSG_IS_MAIN_UNIT = #{item.cdsgIsMainUnit,jdbcType=INTEGER}
	    </set>
        where CDSG_ID = #{item.cdsgId,jdbcType=INTEGER}
       </foreach>
  </update>
```

注意：批量修改的时候数据库JDBC连接需要允许多个处理allowMultiQueries=true。

![](https://img-blog.csdn.net/20161226164858353)




jdbc.jdbcUrl=jdbc:mysql://192.168.10.12:3306/waas15?characterEncoding=utf8&allowMultiQueries=true


## 批量删除



```
<!-- 批量删除-->  
   <delete id="deleteBatch"  >
    delete from cd_wh_package_${tableName} 
    where CDSG_ID in
     <foreach collection="list" item="item" index="index" open="(" close=")" separator="," >
       item
     </foreach>
  </delete>
```
此处list是一个存储ID值的集合。






