# springMVC 接收数组参数，mybatis 接收数组参数，mybatis批量插入/批量删除案例 - BlueSky - CSDN博客
2016年01月20日 19:31:31[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：2412
此篇文章重要的是，下面这句话一个也不能少，尤其是separator=","
<foreach collection="privIds" index="index" item="item" separator=",">
案例是给一个用户赋予多个权限，多个权限用其对应的主键 id 为参数，组成了 一个id数组，传给springMVC，然后springMVC传给mybatis，然后mybatis批量插入。其实类似的场景还有批量删除多个，也是类似的。
**1.** 前台页面
```
<thead><tr><th>权限选择</th><th>name</th><th>permission</th></tr></thead>
                  <c:forEach var="priv" items="${list }">
                       <tr class="odd gradeX">
                      <td><input type="checkbox" name="priv_id" value="${priv.id}" /></td>
                      <td><c:out value="${priv.name}"/></td>
                      <td><c:out value="${priv.permission}"/></td>
                    </tr>
                  </c:forEach>
```
**2.** jquery获得选中的项的id值：
```
//jquery获取复选框值    
            var priv_ids =[];//定义一个数组    
            $('input[name="priv_id"]:checked').each(function(){    // 遍历每一个name为priv_id的复选框，其中选中的执行函数    
                priv_ids.push($.trim($(this).val()));    // 将选中的值添加到数组priv_ids中    
            });
            console.log(priv_ids);
            var indata = {userId:user_id, privIds:priv_ids};
            $.post("/ems/priv/setPrivilege", indata, function(data){
                if(data != null && data.result == 'ok'){
                    console.log(data.msg);
                    alert(data.msg);
                }else{
                    alert(data.msg);
                }
            }, 'json');
```
提交的json格式的数据：var indata = {userId:user_id, privIds:priv_ids};
其中的 priv_ids 是一个有 id 组成的数组。
**3.** springMVC接收数组参数：
```
@RequestMapping(value="/setPrivilege")
    @ResponseBody
    public void setPrivilege(@RequestParam(value = "privIds[]") Integer[] privIds, Integer userId, PrintWriter writer){
        int result = this.privilegeService.setPrivilegeForUser(privIds, userId);
        Map<String, String> map = new HashMap<>();
        if(result > 0){
            map.put("result", "ok");
            map.put("msg", "设置成功");
            writer.write(JSON.toJSONString(map));
        }
    }
```
我们看到使用了：**@RequestParam(value = "privIds[]"**) Integer[] privIds 来获取前台传来的数组参数。
springMVC接收参数时，最好不要使用 int, long等原始类型，而应该使用它们对应的包装类型，不然当传入的参数为空时，会报错，而包装类型可以使用null表示传入的空值。
**4.**service层的处理，很简单，直接使用map向mybatis传递参数：
```
@Service("privilegeService")
@Transactional
public class PrivilegeServiceImpl implements PrivilegeService {
    @Autowired
    private PrivilegeMapper privilegeMapper;
    @Override
    @Transactional(readOnly=true)
    public List<Privilege> getAllPrivilege() {
        return privilegeMapper.getAllPrivilege();
    }
    @Override
    public int setPrivilegeForUser(Integer[] privIds, Integer userId) {
        Map<String, Object> map = new HashMap<>();
        map.put("privIds", privIds);
        map.put("userId", userId);
        return this.privilegeMapper.setPrivilegeForUser(map);
    }
}
```
**5.**最后看 mybatis 的 xml 中的sql如何写：
```
<insert id="setPrivilegeForUser" parameterType="map">
      insert into user_privilege(user_id, privilege_id) values 
      <foreach collection="privIds" index="index" item="item" separator=",">
          ( #{userId}, #{item} )
      </foreach>
  </insert>
```
我们看到使用了 foreach 来循环传递进来的数组 privIds，最后组成的sql语句如下所示：
```
insert into user_privilege(user_id, privilege_id) values (**3**, **1**),(**3**,**2**),(**3，3**),(**3**,**4**)
```
user_id 不变，而privilege_id 是数组 privIds 中的循环出来的 id 值。其实就是数据库的批量插入。
**6.**批量删除多个的处理
删除时，和前面批量插入处理也是极其类似的，只在最后mybatis中xml中sql的写法有点区别：
```
<delete id="deleteByIds" parameterType="java.util.List">
      delete from user_privilege where id in
      <foreach collection="list" index="index" item="item" open="(" separator="," close=")">   
        #{item}   
      </foreach>  
  </delete>
```
比较批量删除和批量插入，可以看出 **foreach 中的 open="(" 和 close=")" 只在循环的开始和结束会加上**，而 **separator="," 是每循环一次，就加一次逗号**。
**7.** 批量插入传入对象List的例子：
```
<insert id="batchInsertStudent" parameterType="java.util.List">  
    insert into student (id,name,sex,tel,address) values
    <foreach collection="list" item="item" index="index" separator="," >  
        (#{item.id},#{item.name},#{item.sex},#{item.tel},#{item.address})  
    </foreach>  
</insert>
```
其实掌握了 批量插入和批量删除，批量更新也是一样的。
