# struts2 注解方式注册action中 namespace 的注意事项 - z69183787的专栏 - CSDN博客
2012年10月24日 10:02:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5357
```java
@SuppressWarnings("serial")
@ParentPackage("struts-default")
@Namespace(value="/deptConfig")
@Controller("deptConfigAction")
@Scope("prototype")
public class DeptConfigAction extends ActionSupport implements ModelDriven<DeptConfigSearchVo>{
	public ActionContext actionContext = ActionContext.getContext();
	public HttpServletRequest request = (HttpServletRequest) actionContext.get(ServletActionContext.HTTP_REQUEST);
	public HttpServletResponse response = (HttpServletResponse) actionContext.get(ServletActionContext.HTTP_RESPONSE);
	public HttpSession session = request.getSession();
	private ActionWriter aw = new ActionWriter(response);
	private DeptConfigSearchVo vo = new DeptConfigSearchVo();
	private Page page = new Page();
	private QueryUtil queryUtil;
	private Map<String,String> statusMap = new HashMap<String,String>();
	private String hideOrShowParam = "show";
	private DeptConfigService deptConfigService;
	private List<TDeptContactRelation> relationList = new ArrayList<TDeptContactRelation>();
	public DeptConfigAction(){
		statusMap.put(String.valueOf(DeptConfigConstants.active), "启用");
		statusMap.put(String.valueOf(DeptConfigConstants.freeze), "禁用");
	}
	/**
	 * @return
	 */
	@Action(value="queryList")
     //此处若配置为 /queryList 则url通过 http://IP:port/context/queryList.action访问；
      // 正确方式应配为queryList，则可通过http://IP:port/context/namespace/queryList.action 
      // 其中namespace为@Namespace(value="/deptConfig") 此处定义。
      public String queryList(){
		try{		
			String[] queryNameArr = {"RECEIVER_NAME","DEPTNAME"};
			String[] queryTypeArr = { "textType", "textType"};
			String[] queryResultArr = {vo.userName,vo.deptName};
			
			String baseSQL = QueryUtil.generateSQLByType(DeptConfigConstants.baseSQL, queryNameArr, queryResultArr, queryTypeArr);
			
			pageResult(baseSQL);
			aw.writeJson(page);
		}catch(Exception e){
			e.printStackTrace();
		}
		return null;
	}
```
方法queryList处：
若配置为 /queryList 
若该项目中仅有这一个以 queryList  命名的action，则url通过 http://IP:port/context/queryList.action 或  http://IP:port/context/namespace/queryList.action 均可访问,
正确方式应配为queryList，则通过http://IP:port/context/namespace/queryList.action 
其中namespace为@Namespace(value="/deptConfig") 此处定义。
