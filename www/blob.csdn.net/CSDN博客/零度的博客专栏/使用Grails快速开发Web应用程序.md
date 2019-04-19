# 使用Grails快速开发Web应用程序 - 零度的博客专栏 - CSDN博客
2015年08月14日 10:11:26[零度anngle](https://me.csdn.net/zmx729618)阅读数：2109
通常情况下，Web 开发可以概括为以下五方面内容：
- 表单提交（验证）与接收（接收用户数据的主要方式）；
- 表单输出（允许用户修改数据的主要方式）；
- 数据列表（显示用户查询结果的主要方式）；
- 超级链接的构造与响应（使用户透明的提交简单数据或者跳转到其它页面）；
- 使用 Session 维持用户会话；
下面，我们将通过阅读 Grails 生成的代码，来理解它是如何分别实现这几方面内容的。
再创建一个名为 Member 的 Domain Class：
grails create-domain-class Member
编辑 Team 和 Member 这两个类，代码如下：
//Team.groovy
class Team { 
    String teamName
    Date foundDate
    static constraints = {
        teamName(size:3..50,blank:false,unique:true)
    }
    static hasMany = [members:Member]
    String toString(){
        return teamName
    }
}
//Member.groovy
class Member { 
    Team team
    String name
    String gender = 'F'
    String address
    String email
    String phone
    static belongsTo = [Team]
    static constraints = {
        name(size:3..20,blank:false,unique:true)
        email(email:true,blank:false)
        gender(inList:["F", "M"] )
    }
}
上述代码描述了 Member 类的属性，并使用 `belongsTo` 和 `hasmany` 描述了 Team 类与 Member 类之间的一对多关系和多对一关系。Grails 会自动在数据库中为这两张表创建主外键。
接下来为 Member 类创建实现 CRUD 的 Controller 和 View。在控制台运行命令行 `grails generate-all Member`，同时也输入`grails generate-all Team` 更新 Team 类的 Controller 和 View。
下面按照前面定义的五个 Web 开发的基本任务对 Member 类自动生成的代码进行分析：
表单提交与接收
打开 grails-app\views\member\creat.gsp，关注表单提交相关的代码（仅摘录表单相关的代码）：
<g:form action="save" method="post">
<label for='dateOfBirth'>Date Of Birth:</label>
<g:datePicker name='dateOfBirth' value="${member?.dateOfBirth}">
</g:datePicker>
<label for='phone'>Phone:</label>
<input 	type="text" value="${member?.phone?.encodeAsHTML()}" name='phone'/>
<label for='email'>Email:</label>
<input 	type="text" value="${member?.email?.encodeAsHTML()}" name='email' />
<label for='name'>Name:</label>
<input 	type="text" value="${member?.name?.encodeAsHTML()}" name='name' />
<label for='gender'>Gender:</label>
<input 	type="text" value="${member?.gender?.encodeAsHTML()}" name='gender' />
<label for='team'>Team:</label>
<g:select optionKey="id" from="${Team.list()}" name='team.id' value="${member?.team?.id}">
</g:select>
<input 	type="submit" value="Create"></input>
</g:form>
g:form 会生成 <from> 标签，同时指定的接收表单数据的 action 为 save。gsp 的表单提交，大量使用标准的 Html tag，如 <input>，这样可以降低学习成本，无需学习过多特定的 tag。Grails 在表单接收端的代码同样也十分简单。Grails 不再使用 Servlet 中冗长的`request.getParameter(XXX)`，它有类似 Struts 却比 Struts 更简单的方式。如上述代码，接收用户表单提交的数据并把它构造为 Domain
 Class，只需要一行代码：`member.properties = params`。完整的 Controller Action 代码摘录如下：
class MemberController {
 …
    def save = {
        def member = new Member()
        member.properties = params
        if(member.save()) {
            //保存成功，重定向到 show Action
            redirect(action:show, id:member.id)
        }
        else {
            //保存失败，返回并报错
            render(view:'create', model:[member:member])
        }
}
…
在上述代码中，params 是一个 Map，包含全部表单提交的数据。把 Map 的值赋给 Domain Class 的 properties 属性后，Domain Class 就会把 Map 类的同名键的值赋给自身相应的属性，自动完成类型转换。这一行代码实现的功能通常需要十几行 Java 代码才能实现，由此也体现了 Grails 开发的“快”。
再补充一点，使用构造函数也可以实现用 params 给 Domain Class 赋值，如：
def member = new Member(params)
由于 Member 是 Domain 类，所以它会被自动添加用于实现数据库增删查改的方法，执行 save 方法进行保存。如果保存成功，if 语句会执行“真”，否则会执行假。虽然数据库读写的任务并不是 Web 开发的基本任务，但如果能够简化这一操作，无疑会极大的提高开发效率，Grails 提供了一个良好的范例。
使用 Domain 类的 properties 属性去获取表单的值是 Grails 下最常见的表单接收方式，但不是唯一的方式。如果要获取某个单一的属性值，还有更简单的方法：可以使用 Map 的 Key 取值。如只获取表单项 <input name="email"/> 的值，可使用：params.email 或者 params['email']。
更进一步，表单提交的内容还可以自动全部写入 Java 类的实例中。还是以上述表单为例把表单内容写入 Java 实例中，定义 Java 类如下：
JMember.java
class JMember {
    private String phone;
    private String email;
    private String name;
    private Team team;
    private Date dateOfBirth;
    private String gender = 'F';
    private String address;
    public String getPhone(){ return phone;}
    public String setPhone(String phone){this.phone = phone}
    //…以下省略其它的 setter 和 getter
}
在接收的表单中编写如下代码：
class MemberController {
 …
    def save = {
        JMember member = new JMember()
        // 自动绑定
		bindData(member,params)
…
}
…
}
也很神奇，不是吗？JMember 是个 JavaBean，Controller 类特有的“bindData”方法可以为 JavaBean 填充数据。（提示：在 Groovy 语言中，JavaBean 的使用也被简化了，如 JMember 的实例 member 可以通过 member.name 去访问或修改 name 属性，而无需显示的调用`member.getName()` 和
`member.setName(xxx)`）
除了给 Domain 类的 properties 属性赋值和使用 Controller 类的 bindData 方法外，Grais 还提供了一种表单提交的方法，叫做 Command Object。Domain Class 可以很好的支持表单验证，但它必须与数据库的一张表关联，如果某一表单提交不是为了保存数据到数据库中，则不能使用 Domain Class。那么如何让非 Domain Class 支持表单的验证？Command Object 正好可以解决这个问题。
在文件夹 src/groovy 中创建一个名为 XXXCommand 的 Groovy 类，比如 SearchCommand.groovy，然后在 Command 类中加入约束条件（加入方法与 Domain Class 相同）。然后在 Controller 类里引用 Command Object，引入方法如下：
class MemberController {
    def search = { SearchCommand cmd ->
        if (cmd.hasErrors()) {
            redirect(action:'error')
        } else {
            // do something else
        }
    }
}
SearchCommand cmd-> 表示 Grails 在调用 search Action 的时候，会传递给 Action 一个参数，这个参数就是 SearchCommand 的实例，名叫 cmd。SearchCommand 和表单对应的代码如下：
class SearchCommand {
    String teamname
    String username
    String gender
    static constraints = {
        gender (inList:[ "F", "M"])
    }
}
<g:form controller="member"　action=”search” method="post" >
<label for='teamName'>Team Name:</label>
<input type="text" id='teamName' name='teamName'/>
<label for='name'>Name:</label>
<input type="text" id='name' name='name'/>
<label for='gender'>Gender:</label>
<g:select id='gender' name='gender' from=' ${[""] + 
    new Member().constraints.gender.inList.collect{it.encodeAsHTML()}}' />
<g:submit　value="search"/>
</g:form>
从上面的代码可以看出，Command 对象在表单的构造并无太大区别，只是在接收时存在一点区别，使用起来并不复杂。
表单的提交在 Grails 中十分简单，同样，表单输出也不难。正如上文，对于常见的 HTML 表单控件，只需要修改 value 属性，就可以显示控件中的内容：
<input type="text" value="${member?.phone?.encodeAsHTML()}" name='phone'/>
上述代码重点在两个地方，一是问号“?”的作用，二是 `encodeAsHTML` 方法。`member?.phone` 表达的意思是：如果 member 不为空，返回 member.phone，否则返回空字符串，同样道理，如果 member.phone 不为空，返回`member.phone.encodeAsHTML()`。从字面的意思可以看出，这个方法可以把字符串中的“<”、“>”等转换为 HTML 对应的 < > 等。这个方法可以根据具体业务，决定是否调用（其实对于
 phone 这个属性而言，是没有必要调用它的，但 Grails 自动生成的代码调用了此方法，用户可以自己手动改为 member?.phone）。对于相对复杂一点的表单输出，如下拉框（select box），可以使用 grails 自带的标签库`<g:select>`：
<g:select optionKey="id" from="${Team.list()}" name='team.id' value="${member?.team?.id}">
此时就可以同样使用 value 属性来指定表单项的内容了。但是 `${}` 中的变量又是从哪来的呢？与 Struts 类似，它来自 Controller。观察 edit Action：
def edit = {
    def member = Member.get( params.id )
    if(!member) {
        flash.message = "Member not found with id ${params.id}"
        redirect(action:list)
    } else {
        return [ member : member ]
    }
}
从 return 行对应的代码可以看到，Controller 向 Veiw 传递了 Model，这是典型的 MVC 模式。
Grails 对提供 Domain 类和 Command Object 的表单提供了“一栈式”的验证支持。首先在 Domain 类或 Command 类中添加约束信息，比如 Member 类的代码：
class Member { 
    Team team
    String name
    String gender = 'F'
    String address
    String email
    String phone
    static belongsTo = [Team]
    static constraints = {
        name(size:3..20,blank:false,unique:true)
        email(email:true,blank:false)
        gender(inList:["F", "M"] )
    }
}
在上述代码中加入对 Member 类的约束，调用 Domain 类对象的 `validate` 方法或 `save` 方法检验当前数据是否符合约束条件（Command 类可使用`hasError()` 方法）。如果不符合，则返回
`false`。
在相应的 gsp 页面中也需要有相应代码用于输出验证结果：
<g:hasErrors bean="${member}">
    <div class="errors">
        <g:renderErrors bean="${member}" as="list" />
    </div>
</g:hasErrors>
上述代码实现以列表的形式输出 member 对象包含的全部错误信息。相应的，如果想更有针对性的指明具体是哪个表单项提交的数据有问题，可以使用如下的代码：
<td valign='top'
 class='value ${hasErrors(bean:member,field:'name','errors')}'>
<input type="text" name='name' 
value="${member?.name?.encodeAsHTML()}"/>
</td>
.error 的 CSS 项：
.errors { border: 1px solid red }
Grails 对错误信息提供了 i18n（国际化）支持。可以通过修改 Contact\grails-app\i18n 不同语言的资源文件来定制输出错误信息。这里修改的是 messages_zh_CN.properties：
member.name.blank=用户名不能为空
member.name.unique=用户名{2}已经存在，请使用其它用户名
member.name.size.toosmall=用户名长度不得少于{3}个字符
member.name.size.toobig=用户名长度不得超过{3}个字符
member.email.blank=Email地址不能为空
member.email.email.invalid=Email地址不合法
member.gender.not.inList=无效的性别
team.teamname.blank=Team名称不能为空
team.teamname.size.toosmall=Team名称长度不得少于{3}个字符
team.teamname.size.toobig=Team名称长度不得超过{3}个字符
team.teamname.unique=Team名称{2}已经存在，请使用其它名称….
这里要强调一点，一定要使用 UTF-8 格式来保存上述文件（不能使用 GB2312），保存完成后，无需手工调用 JDK 的 native2ascii.exe 去转换，因为在 Grails 启动的时候，会自动完成该转换。关于不同约束项对应的名称，可以通过[官方文档](http://www.grails.org/Validation+Reference) 获取。
数据列表是 Web 应用最重要的一种数据输出方式，使用 Grails 实现数据列表十分容易。用一句话概括就是：由 Controller 提供数据，使用 
```
<g:each>
```
 遍历数据，再用 `${}` 输出数据。
<g:each in="${memberList}">
   <tr>
<td>${it.id?.encodeAsHTML()}</td>
<td>${it.dateOfBirth?.encodeAsHTML()}</td>
<td>${it.phone?.encodeAsHTML()}</td>
<td>${it.email?.encodeAsHTML()}</td>
<td>${it.address?.encodeAsHTML()}</td>
<td>${it.name?.encodeAsHTML()}</td>
   </tr>
</g:each>
`<g:each>` 可以遍历数据，memberList 是 Controller 类中对应 `list` Action 的返回值。遍历过程中每次取出一个元素，默认名称为“`it`”，如果想改用其它名称，可以用`var` 属性声明：`<g:each in=”${memberList}” var=”member”>`。
超级链接通常在 Web 应用中可以实现类似按扭的功能。一方面指明要跳转到的页面，另一方面还要提交少量的数据。比如，在显示 Member 列表的页面里，加入一个用于显示 Member 的 Link，只需要加入如下代码：
<g:each in="${memberList}">
   <tr>
      	…
        <td><g:link action="show" id="${it.id}">Show</g:link></td>
   </tr>
</g:each>
通过指定 `controller` 和 `action` 两个属性可以确定当前 Link 会跳转到哪个页面（如果不指定 controller，则应用当前的 controller），`id` 属性可以传递一个用于标记唯一性的属性值。而`<g:link>XXX</g:link>` 中间的内容则为该链接显示的文本。
对应的 Action 接收 Link 数据的代码很简单：
def show = {
    [ member : Member.get( params.id ) ]
}
通过 `params.id` 可以得到 Link 提交的数据。
对于传统的 `?ParameterName=value` 的参数组织方式的链接，Grails 也提供了支持：可使用 `g:link` 标签的`params` 属性，用一个 Map 给它赋值，则 Map 的 Key 被输出为参数名，而 Key 对应的 value 被输出为参数的值：
<g:link controller="member" 
action="create" params="['team.id':team.id]">Add Members</g:link>
上面的 Tag 最终会输出为 URL: `/Contact/member/create?team.id=2` （假定 team.id 的值是 2）。
在 Grails 中使用 Session 十分简单，Controller/GSP 中可以访问一个名叫 `session` 的对象，它与 Map 的用法十分相似。比如说，在`session` 中保存一个名叫 age 的对象，在 Controller/GSP 中，可以写成
`session.age=XX` 或`session['age']=XX`。调用 session 的 `invalidate` 方法，则可以清空 Session 的内容。
上面所讲的内容涵盖了 Web 开发的几个基本方面，理论上讲，掌握了这几个基本内容就可以进行 Web 开发了，但是 Grails 还提供了一些可以极大提高开发效率的内容，比如 DB 读写。DB 的读写虽然不属于 Web 范畴，但无可争议的是，几乎没有 Web 应用是不使用 DB 的。下面对 Grails 简化的 Hibernate 操作进行一下简单介绍：
**表 2.DB 相关操作**
|`save`|保存 Domain 对象的数据到对应的库表中(可能是 Insert 也可能是 Update)|
|----|----|
|`findBy`|动态方法，查找并返回第一条记录，方法名可以变化如：findByName("Tom") 会返回所有 name 属性为 Tom 的对象（只返回第一条记录）findByNameAndPassword("Tom","Mot")会返回所有 name 属性为”Tom”并且 password 属性为”Mot”的对象|
|`findAllBy`|与 findBy 类似，返回全部记录|
|`executeQuery`|执行一个 HQL 的查询（HQL 的使用，请参考 Hibernate 官方文档）|
|`createCriteria`|执行一个 Hibernate 的 Criteria 查询（下文会有一个 Criteria 的试例）|
|`get`|返回指定 id 的对象|
|`count`|执行”select count(*) from XX”的操作|
|`delete`|执行删除操作。|
更多 DB 相关的操作，请参考 [相关文档](http://www.grails.org/DomainClass+Dynamic+Methods)。
完整的PDF文档请下载：[http://download.csdn.net/detail/zmx729618/9003765](http://download.csdn.net/detail/zmx729618/9003765)
