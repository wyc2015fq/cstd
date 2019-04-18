# ssh框架配合easyUi实现分页 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月16日 10:56:57[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：165


```java
分页插件
$('#table的id').datagrid({    

	url:'请求的地址',
	method:'post',
	rownumbers: true,               ---行标
	pagination: true,               ---页码
	pageSize: 10,                   ---默认每页显示多少条记录
        pageList: [10,20,30,40,50],     ---可选每页多少条记录
	border: false,
	nowrap: true,
	singleSelect: false,
	toolbar: '#_toolbarResourcep1',
	fit:true
})

//前端获取input值 然后向服务器发送请求

 function onSearchResourcel() {
	
	var $sch_code = $("# input框的id ").combobox('getValue');
	var $jobtype = $("# input框的id ").combobox('getValue');
	var $start = $("# input框的id ").datebox('getValue');
	var $finish = $("# input框的id ").datebox('getValue');
	$('#table的id').datagrid('load',{
		"sch_code":$sch_code,
		"jobtype" : $jobtype,
		"startDay" : $start,
		"finishDay" : $finish
	}) 
}
```

后端 action--Struts2  的成员变量 可以自动接收前端传过来的参数，前提是成员变量的值和前端参数名一样

```java
//前端对应的4个参数
	  private String  sch_code;
	  private String jobtype;
	  private String startDay;
	  private String finishDay;
	  //接收当前页
	  private int page;
	  //接收行数
	  private int rows;
	@Action(value = "  ")
	public void getResourceLoad(){
					
			    Map<String, Object> params = new HashMap<>();
			    Map<String, Object> map;
			    //判断前端用户必选的参数是否为空
			    if (StringUtil.isNotBlank(sch_code)) {
			      //params.put("department", department);
			      params.put("sch_code", sch_code);
			      params.put("tech_code", jobtype);
			      //params.put("plantype", plantype);
			      params.put("sch_start_date", startDay);
			      params.put("sch_finish_date", finishDay);
			      try {
			        //params查询参数、page当前页数、rows每页数据条数
			    	  map = planresourceService.queryResourceLoad(params, page, rows);
			    	//将通过分页取出的部分表单信息传给前端  --将map转成json 前端框架能够自动获取
			        this.printJson(JsonUtil.ObjectToJson(map));
			      } catch (Exception e) {
			        // TODO Auto-generated catch block
			        e.printStackTrace();
			      }
			    }	
	}
```

```java

```
//service实现类 从dao层拿数据，dao层的数据放入map集合中，key必须和jsp中的filed值一样，然后对拿回来的集合根据前端传过来的page和rows对集合进行截取。
//截取之后，将截取的集合放入map中，key=rows，value="截取之后的集合",另一个key="total",value="数据库中的总数据数"。这两个是前端需要的。
	@Override
	public Map<String, Object> queryResourceLoad(Map<String, Object> params, int page, int rows) throws Exception {
		
		//把参数传给dao层，查数据--返回结果
		List<Map<String, String>> resource = dao.queryResourceLoad(params);
		
		//数据总条数
		 int totalCount = resource.size();
	        
		 	//创建一个map集合，用来存放
		    Map<String, Object> pageToJson = new HashMap<String, Object>();

	        // 判断是否有参数
	        if (params.size() == 0) {
	          pageToJson.put("rows",
	        		  resource.subList((page - 1) * rows, page * rows > totalCount ? totalCount : page * rows));
	          pageToJson.put("total", resource.size());
	        } else {
	          //截取之后的表单数据放入map
	          pageToJson.put("rows",resource.subList((page-1)*rows,page*rows>resource.size()?resource.size():page*rows));
	          //数据总条数放入map
	          pageToJson.put("total", resource.size());
	        }
	        
	        return pageToJson;
	}



