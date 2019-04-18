# struts2的@Result annotation 如何添加params,并且在页面取值 - z69183787的专栏 - CSDN博客
2013年11月15日 17:02:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8968
1、action跳转至jsp
后台：（需有get set 方法）
```java
@SuppressWarnings("rawtypes")
	@Action(value="/loadFileList",results={
			@Result(name="attachList",location="/attach/attachList.jsp",params={"attachMemo1","${attachMemo1}"}),
			@Result(name="attachList1",location="/attach/attachList1.jsp"),
			@Result(name="attachListHT",location="/attach/attachListHT.jsp")})
```
前台：
`<%out.println(request.getAttribute("attachMemo1")); %>`
2、action跳转至action
后台：（stptuser为 action中的bo）
```java
@Action(value="stptUserUpdate",results={
			@Result(name="stptUserEdited",params={"actionName","stptUserView","stptUserId","%{stptUser.id}"},type="redirectAction")
			})
```
或者
```java
@Result(name="success", 
  type="redirectAction", 
  location="d-list", 
  params={"id", "%{id}"} 
)
```
另一个action获取
```java
@Action(value="stptUserView",results={
			@Result(name="success",location="/userManage/stptUserView.jsp")
			})		
	public String stptUserView(){
		String id = StringUtil.getNotNullValueString(super.getServletRequest().getParameter("stptUserId"));
		this.stptUser = this.stptUserService.findStptUserById(Long.parseLong(id));
		List<ManagerVo> list = this.stptUserService.getUserInfo(id);
		List<ManagerVo> agentList = this.stptUserService.getAgentInfo(id);
		super.getServletRequest().setAttribute("voList", list);
		super.getServletRequest().setAttribute("agentList", agentList);
		return SUCCESS;
	}
```
