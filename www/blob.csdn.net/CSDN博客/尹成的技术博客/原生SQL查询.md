
# 原生SQL查询 - 尹成的技术博客 - CSDN博客

2018年11月14日 16:16:20[尹成](https://me.csdn.net/yincheng01)阅读数：296


`type SqlController struct {
	beego.Controller
}`\#Prepare
sql语句在发送到服务器时会做语法检查和编译，然而我们多次执行同一功能的sql语句，只是每次传递的参数不一致，所以我们可以通过Prepare进行预处理，这样服务器只需要，做一次语法检查和编译，极大的提高了执行sql语句的效率
`func (this *SqlController) GetPrepare() {
	//1.创建orm
	orm := orm.NewOrm()
	p, err := orm.Raw("update user set name = ? where name = ?").Prepare()
	if err != nil {
		fmt.Println("err = ", err)
		this.Ctx.WriteString("查询出错!")
		return
	}
	res, err := p.Exec("吴俏祥", "123")
	if err != nil {
		fmt.Println("err = ", err)
		this.Ctx.WriteString("查询出错!")
	}
	n, err := res.RowsAffected()
	if err != nil {
		fmt.Println("err = ", err)
		this.Ctx.WriteString("查询出错!")
	}
	fmt.Println("n = ", n)
	res, err = p.Exec("尹成大魔王", "mike")
	if err != nil {
		fmt.Println("err = ", err)
		this.Ctx.WriteString("查询出错!")
	}
	n, err = res.RowsAffected()
	if err != nil {
		fmt.Println("err = ", err)
		this.Ctx.WriteString("查询出错!")
	}
	fmt.Println("n = ", n)
	res, err = p.Exec("兄弟连", "asde")
	if err != nil {
		fmt.Println("err = ", err)
		this.Ctx.WriteString("查询出错!")
	}
	n, err = res.RowsAffected()
	if err != nil {
		fmt.Println("err = ", err)
		this.Ctx.WriteString("查询出错!")
	}
	fmt.Println("n = ", n)
	this.Ctx.WriteString("成功!")
}`\#将将指定的字段解析到map
`func (this *SqlController) GetRawsToMap() {
	//1.创建map
	res := make(orm.Params)
	//2.创建orm
	orm := orm.NewOrm()
	//3.通过sql语句查询
	//将user表中name作为键，nickname作为值存入map中，其中键是string类型，
	// 如果类型不匹配beego会自动转换为string类型
	/*n, err := orm.Raw("select nickname, name from user").
		RowsToMap(&res, "name", "nickname")*/
	n, err := orm.Raw("select * from user").RowsToMap(&res, "age", "name")
	//4.处理错误
	if err != nil {
		fmt.Println("err = ", err)
		this.Ctx.WriteString("查询出错!")
		return
	}
	//5.打印结果
	fmt.Println("n = ", n)
	for key, value := range res {
		fmt.Println(key, " = ", value)
	}
	//6.给客户端返回数据
	this.Ctx.WriteString("查询成功!")
}`\#QueryRows
`func (this *SqlController) GetQueryRows(){
	//1.创建user切片
	var users []*models.User
	//2.创建orm
	orm := orm.NewOrm()
	//3.通过sql语句查询
	n, err := orm.Raw("select * from user where name = ?", "mike").QueryRows(&users)
	//4.处理错误
	if err != nil {
		fmt.Println("err = ", err)
		this.Ctx.WriteString("查询出错!")
		return
	}
	//5.打印结果
	fmt.Println("n = ", n)
	for _, user := range users {
		fmt.Println("user = ", user)
	}
	//6.给客户端返回数据
	this.Ctx.WriteString("查询成功!")
}`\#QueryRow
`func (this *SqlController) GetQueryRow() {
	//1.创建user对象
	//user := models.User{}
	var user models.User
	//2.创建orm
	orm := orm.NewOrm()
	//3.通过sql语句查询
	err := orm.Raw("select * from user where id = ?", 3).QueryRow(&user)
	//4.处理错误
	if err != nil {
		fmt.Println("err = ", err)
		this.Ctx.WriteString("查询出错!")
		return
	}
	//5.打印结果
	fmt.Println("user = ", user)
	this.Ctx.WriteString("查询成功!")
}`\#Exec
`func (this *SqlController) GetRawAndExec() {
	//创建orm
	orm := orm.NewOrm()
	//raw函数中的参数是sql语句，形式参数通过？占位，后面提供的实际参数和？的顺序一一对应
	//Exec()函数的功能是执行sql语句
	res, err := orm.Raw("update user set name = ? where id = ?", "admin", 7).Exec()
	if err != nil {
		this.Ctx.WriteString("查询失败!")
		return
	}
	n, err := res.RowsAffected()//查询受影响的行数
	if err != nil {
		this.Ctx.WriteString("查询失败!")
		return
	}
	this.Ctx.WriteString("mysql row affected nums = " + strconv.Itoa(int(n)))
}`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

