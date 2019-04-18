# 【mybatis】mybatis中update更新原来的值加1 - weixin_33985507的博客 - CSDN博客
2018年06月07日 15:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：301
示例代码： floor的值 = floor原来的值+要更新的差距值
```
<update id="updateFloor" parameterType="com.pisen.cloud.luna.ms.goods.base.common.UpdateGoodsTypeFloorBean">
        update goods_type
        set floor = floor+#{floorGap}
        where uid in
        <foreach collection="list" item="item" index="index" open="(" separator="," close=")">
            #{item}
        </foreach>
    </update>
```
