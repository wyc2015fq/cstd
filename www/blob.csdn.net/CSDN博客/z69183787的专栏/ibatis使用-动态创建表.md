# ibatis使用-动态创建表 - z69183787的专栏 - CSDN博客
2017年03月06日 15:44:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2037
```
<update id="OP_Platform_Fargo_Action.createTable" parameterClass="map">
 
    <![CDATA[
 
        CREATE TABLE Push_User_Subscribe_#activityId:INTEGER# (
 
        `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
 
        `user_id` int(11) NOT NULL COMMENT '用户id',
 
        `scene_id` int(11) NOT NULL COMMENT '场景id',
 
        `status` int(11) NOT NULL COMMENT '开关1 开 0 关',
 
        `add_time` datetime NOT NULL COMMENT '增加时间',
 
        `updatetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间',
 
        PRIMARY KEY (`id`),
 
        UNIQUE KEY `UN_Userid_sceneid` (`user_id`,`scene_id`),
 
        KEY `IX_Update` (`updatetime`),
 
        KEY `IX_Sceneid_Status_Userid` (`scene_id`,`status`,`user_id`)
 
        ) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COMMENT=#comment:VARCHAR#;
 
    ]]>
 
</update>
```
```java
@DAOAction(action = DAOActionType.UPDATE)
 
 void createTable(@DAOParam("activityId") Integer activityId,@DAOParam("comment") String comment);
```
