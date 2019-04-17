# MyBatis解决BLOB中文乱码问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年11月23日 13:46:34[boonya](https://me.csdn.net/boonya)阅读数：2871








Blob在java中是java.sql.Blob，这个对象不能用来做Java实体变量，而应该使用byte[]。对于bolb，一般用于对图片的数据库存储，原理是把图片流存为二进制字节码，然后进行的一种存储方式，在java中对应byte［］数组。

## java 与mysql 数据类型对照





|类型名称|显示长度|数据库类型|JAVA类型|JDBC类型索引(int)|描述|
|----|----|----|----|----|----|
|||||||
|VARCHAR|L+N|VARCHAR|java.lang.String|12||
|CHAR|N|CHAR|java.lang.String|1||
|BLOB|L+N|BLOB|java.lang.byte[]|-4||
|TEXT|65535|VARCHAR|java.lang.String|-1||
|||||||
|INTEGER|4|INTEGER UNSIGNED|java.lang.Long|4||
|TINYINT|3|TINYINT UNSIGNED|java.lang.Integer|-6||
|SMALLINT|5|SMALLINT UNSIGNED|java.lang.Integer|5||
|MEDIUMINT|8|MEDIUMINT UNSIGNED|java.lang.Integer|4||
|BIT|1|BIT|java.lang.Boolean|-7||
|BIGINT|20|BIGINT UNSIGNED|java.math.BigInteger|-5||
|FLOAT|4+8|FLOAT|java.lang.Float|7||
|DOUBLE|22|DOUBLE|java.lang.Double|8||
|DECIMAL|11|DECIMAL|java.math.BigDecimal|3||
|BOOLEAN|1|同TINYINT||||
|||||||
|ID|11|PK (INTEGER UNSIGNED)|java.lang.Long|4||
|||||||
|DATE|10|DATE|java.sql.Date|91||
|TIME|8|TIME|java.sql.Time|92||
|DATETIME|19|DATETIME|java.sql.Timestamp|93||
|TIMESTAMP|19|TIMESTAMP|java.sql.Timestamp|93||
|YEAR|4|YEAR|java.sql.Date|91| |

下面用示例来讲解如何处理数据库字段BLOB类型。

## BLOB示例讲解

SQL：构建一张数据表



```
/*
Navicat MySQL Data Transfer

Source Server         : 10.10.10.117-8066-test
Source Server Version : 50629
Source Host           : 10.10.10.117:8066
Source Database       : cvnavidb

Target Server Type    : MYSQL
Target Server Version : 50629
File Encoding         : 65001

Date: 2017-11-23 13:40:53
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `t_transparent_info_send_log`
-- ----------------------------
DROP TABLE IF EXISTS `t_transparent_info_send_log`;
CREATE TABLE `t_transparent_info_send_log` (
  `F_ID` bigint(20) NOT NULL COMMENT '数据ID',
  `F_TYPE` smallint(6) NOT NULL COMMENT '类型',
  `F_ORDER_ID` bigint(20) NOT NULL COMMENT '命令编号',
  `F_DATA` blob,
  `F_ENTERPRISE_ID` bigint(20) DEFAULT NULL COMMENT '运维用户该值为 00000000',
  PRIMARY KEY (`F_ID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='数据发送记录';
```





Entity：F_DATA字段是BLOB类型的



```java
@TableName("t_transparent_info_send_log")
public class TransparentinfoSendlog implements Serializable{

    private static final long serialVersionUID = 1L;

    @TableField("F_ID")
    private Long id;

    @TableField("F_TYPE")
    private Integer type;

    @TableField("F_ORDER_ID")
    private Long orderId;

    @TableField("F_ENTERPRISE_ID")
    private Long enterpriseId;

    @TableField("F_DATA")
    private byte[] data;
    private String dataStr;

    private Long vehicleId;
    private String plateCode;
    private String plateColor;
    private Date sendTime;

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Integer getType() {
        return type;
    }

    public void setType(Integer type) {
        this.type = type;
    }

    public Long getOrderId() {
        return orderId;
    }

    public void setOrderId(Long orderId) {
        this.orderId = orderId;
    }

    public Long getEnterpriseId() {
        return enterpriseId;
    }

    public void setEnterpriseId(Long enterpriseId) {
        this.enterpriseId = enterpriseId;
    }

    public byte[] getData() {
        return data;
    }

    public void setData(byte[] data) {
        this.data = data;
    }

    public Long getVehicleId() {
        return vehicleId;
    }

    public void setVehicleId(Long vehicleId) {
        this.vehicleId = vehicleId;
    }

    public String getPlateCode() {
        return plateCode;
    }

    public void setPlateCode(String plateCode) {
        this.plateCode = plateCode;
    }

    public String getPlateColor() {
        return plateColor;
    }

    public void setPlateColor(String plateColor) {
        this.plateColor = plateColor;
    }

    public Date getSendTime() {
        return sendTime;
    }

    public void setSendTime(Date sendTime) {
        this.sendTime = sendTime;
    }

    public String getDataStr() {
        if(null!=data){
            dataStr=new String(data);
        }
        return dataStr;
    }
}
```
Mapper接口：定义SQL接口访问方法




```java
package com.cvnavi.service.transparentinfo.mapper;

import com.baomidou.mybatisplus.plugins.pagination.Pagination;
import com.cvnavi.bean.sys.user.User;
import com.cvnavi.bean.transparentinfo.TransparentinfoSendlog;
import com.cvnavi.core.base.BaseMapper;
import org.apache.ibatis.annotations.Param;
import java.util.List;

public interface TransparentinfoSendlogMapper extends BaseMapper<TransparentinfoSendlog> {

    /**查询数据下发列表*/
    public List<TransparentinfoSendlog> getTransparentinfoSendlogList(@Param("transparentinfoSendlog") TransparentinfoSendlog transparentinfoSendlog, @Param("usr") User user, Pagination pagination);

    /**新增数据下发操作*/
    public int insertTransparentinfoSendlog(@Param("transparentinfoSendlog") TransparentinfoSendlog transparentinfoSendlog);

}
```





Mapper XML配置:主要看插入和查询方法



```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
<mapper namespace="com.cvnavi.service.transparentinfo.mapper.TransparentinfoSendlogMapper" >
  <resultMap id="TransparentinfoSendlogResultMap" type="com.cvnavi.bean.transparentinfo.TransparentinfoSendlog" >
    <id column="F_ID" property="id" jdbcType="BIGINT" />
    <result column="F_TYPE" property="type" jdbcType="SMALLINT" />
    <result column="F_ORDER_ID" property="orderId" jdbcType="BIGINT" />
    <result column="F_ENTERPRISE_ID" property="enterpriseId" jdbcType="BIGINT" />
    <result column="F_DATA" property="data"  jdbcType="BLOB"/>

    <result column="vehicleId" property="vehicleId" jdbcType="BIGINT" />
    <result column="plateCode" property="plateCode" jdbcType="VARCHAR" />
    <result column="plateColor" property="plateColor" jdbcType="VARCHAR" />
    <result column="sendTime" property="sendTime" jdbcType="TIMESTAMP" />
  </resultMap>

  <sql id="Transparentinfo_Sendlog_Column_List" >
    F_ID, F_TYPE, F_ORDER_ID, F_ENTERPRISE_ID,F_DATA
  </sql>

  <select id="getTransparentinfoSendlogList" resultMap="TransparentinfoSendlogResultMap"  >
    select
    s.F_ID,
    s.F_TYPE,
    s.F_ORDER_ID,
    s.F_ENTERPRISE_ID,
    s.F_DATA,
    v.F_PLATE_CODE plateCode,
    v.F_PLATE_COLOR plateColor,
    o.F_SEND_TIME sendTime
    from t_transparent_info_send_log s
    left join t_orderinfo o on o.F_ID=s.F_ORDER_ID
    left join t_vehicle v on v.F_ID=o.F_VEHICLE_ID
    LEFT JOIN T_VEHICLE_FLEET TF on TF.F_VEHICLE_ID = v.F_ID and TF.F_TYPE = 1
    where 1=1
    <if test="transparentinfoSendlog.id != null and transparentinfoSendlog.id != ''">
      AND s.F_ID = #{transparentinfoSendlog.id}
    </if>
    <if test="transparentinfoSendlog.type != null and transparentinfoSendlog.type != ''">
      AND s.F_TYPE = #{transparentinfoSendlog.type}
    </if>
    <if test="transparentinfoSendlog.vehicleId != null and transparentinfoSendlog.vehicleId != ''">
      AND o.F_VEHICLE_ID = #{transparentinfoSendlog.vehicleId}
    </if>
    <!--用户数据权限-->
    <if test="usr.accountType != 5 and usr.accountType != 6" >
      AND v.F_ENTERPRISE_ID IN (${usr.enterpriseIds})
      <if test="usr.accountType != 4 and usr.accountType != 3" >
        <choose>
          <when test="usr.fleetIds !=null and usr.fleetIds !='' ">
            AND TF.F_FLEET_ID IN (${usr.fleetIds})
          </when>
          <otherwise>
            AND 1 = 2
          </otherwise>
        </choose>
      </if>
    </if>
    order by o.F_ID desc
  </select>

  <insert id="insertTransparentinfoSendlog" parameterType="com.cvnavi.bean.transparentinfo.TransparentinfoSendlog" >
    insert into t_transparent_info_send_log
    <trim prefix="(" suffix=")" suffixOverrides="," >
      <if test="transparentinfoSendlog.id != null" >
        F_ID,
      </if>
      <if test="transparentinfoSendlog.type != null" >
        F_TYPE,
      </if>
      <if test="transparentinfoSendlog.orderId != null" >
        F_ORDER_ID,
      </if>
      <if test="transparentinfoSendlog.enterpriseId != null" >
        F_ENTERPRISE_ID,
      </if>
      <if test="transparentinfoSendlog.data != null" >
        F_DATA,
      </if>
    </trim>
    <trim prefix="values (" suffix=")" suffixOverrides="," >
      <if test="transparentinfoSendlog.id != null" >
        #{transparentinfoSendlog.id,jdbcType=BIGINT},
      </if>
      <if test="transparentinfoSendlog.type != null" >
        #{transparentinfoSendlog.type,jdbcType=SMALLINT},
      </if>
      <if test="transparentinfoSendlog.orderId != null" >
        #{transparentinfoSendlog.orderId,jdbcType=BIGINT},
      </if>
      <if test="transparentinfoSendlog.enterpriseId != null" >
        #{transparentinfoSendlog.enterpriseId,jdbcType=BIGINT},
      </if>
      <if test="transparentinfoSendlog.data != null" >
        #{transparentinfoSendlog.data},
      </if>
    </trim>
  </insert>
</mapper>
```

处理Blob后前端只需要使用dataStr字段就可以看到效果了。











