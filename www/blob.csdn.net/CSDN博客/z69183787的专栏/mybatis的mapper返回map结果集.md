# mybatis的mapper返回map结果集 - z69183787的专栏 - CSDN博客
2018年08月27日 21:41:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：320
[https://blog.csdn.net/huanyinghanlang/article/details/79194005](https://blog.csdn.net/huanyinghanlang/article/details/79194005)
## 通过MapKey指定map的key值
- 
`@MapKey("id")`
- 
`Map<Long, UserInfo> getUserInfoMap();`
- 
- 
`@MapKey("id")`
- 
`Map<Long, Map<String,Object>> getUserValueMap();`
## map的value为java类
- 
`<resultMap id="UserResultMap" type="com.xixicat.domain.UserInfo">`
- 
`<result property="id" column="id" />`
- 
`<result property="username" column="username" />`
- 
`<result property="sex" column="sex" />`
- 
`</resultMap>`
- 
`<select id="getUserInfoMap" resultMap="UserResultMap">`
- 
`select id,username,sex from user_info`
- 
`</select>`
## map的value为map
- 
`<select id="getUserValueMap" resultType="map" >`
- 
`select id,username,sex from user_info`
- 
`</select>`
