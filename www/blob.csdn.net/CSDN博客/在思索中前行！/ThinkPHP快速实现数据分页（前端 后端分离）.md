# ThinkPHP快速实现数据分页（前端/后端分离） - 在思索中前行！ - CSDN博客





2014年10月22日 09:32:18[_Tham](https://me.csdn.net/txl16211)阅读数：4299







数据分页 可能是web 编程里最常用到的功能之一。thinkphp 实现分页功能十分简洁。只需要定义 几个参数 就能搞定。当然，扩展也是十分方便的。
让我们现在就开始thinkphp的分页实现吧。

1:首先，我们得创建一个用于分页测试的数据库 test。sql代码 如下。
复制PHP 内容到剪贴板PHP代码:
CREATE TABLE `test` (
   `id` int(10) unsigned NOT NULL auto_increment,
   `name` char(100) NOT NULL,
   `content` varchar(300) NOT NULL,
    PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=27 ;

INSERT INTO `test` (`id`, `name`, `content`) VALUES
(19, '123', '123'),
(20, '1231', '123123123'),
(21, '123123', '123123123'),
(26, '24', '123123'),
(25, '321123', '321123'),
(24, 'age', 'age'),
(23, '123123', '123123'),
(22, '213', '123');


2:接着，我们得新建一个thinkphp项目 。新版tp已经内置了项目自动 生成目录 功能。
在htdocs(也就是你的网站根目录)下新建一个test文件 夹,把THINKPHP核心文件夹放进test根目录，并在test根目录新建文件index .php。
复制PHP内容到剪贴板PHP代码:
// 定义ThinkPHP框架路径
define('THINK_PATH', './Thinkphp');
//定义项目名称和路径。这2句是重点。
define('APP_NAME', 'test');
define('APP_PATH', './test');
// 加载 框架入口文件
require(THINK_PATH."/ThinkPHP.php");
//实例化一个网站应用实例
$App = new App();
//应用程序初始化
$App->run();
?>

运行"http://localhost/test/index.php"。会看到thinkphp欢迎页面 。再打开你的test目录看看，发现在根目录下多了一个test文件夹，此时，你的项目目录已经生成了。

打开/test/test/conf/目录，新建“config.php” ，配置好你的数据库连接。
复制PHP内容到剪贴板PHP代码:

<?php
return array(
    'DB_TYPE'=>'mysql',
    'DB_HOST'=>'localhost',
    'DB_NAME'=>'test',
    'DB_USER'=>'root',
    'DB_PWD'=>'',
    'DB_PORT'=>'3306',
);
?>

如果你想打开调试模式 ,请在数组中加入“debug_mode”=>true.

3:基本页面输入与输出 的实现。

（1）打开/test/test/lib/action/IndexAction .class .php，会发现以下代码
复制PHP内容到剪贴板PHP代码:
<?php 
// 本类由系统 自动生成，仅供测试用途
class IndexAction extends Action{
    public function index(){
        header("Content-Type:text/html ;
 charset=utf-8");
        echo "<div style='font-weight:normal;color:blue;float:left;width:345px;text-align:center;border:1px solid silver;background:#E8EFFF;padding:8px;font-size:14px;font-family:Tahoma'>^_^
 Hello,欢迎使用<span style='font-weight:bold;color:red'>ThinkPHP</span></div>";
    }
} 
?>

由系统自动生成的indexaction类中的index()函数 是默认的首页调用函数。你可以使用http://localhost/test/index.php或者http://localhost/test/index.php/index来访问他

（2）我们暂时不管他。首先我们需要一个表单 提交的页面。打开"/test/test/tpl/default/index/"，新建一个文件add.html.
复制PHP内容到剪贴板PHP代码:
<form method="post" action="__URL __/insert">
<p>姓名:<input name="name" type="text" ></p>
<p>内容:<input name="content" type="text"></p>
<p>提交:<input type="submit" value="submit"></p>
</form>

保存后，输入 http://localhost/test/index.php/index/add，你就能看到你新增的页面了。其中，__URL__(url要大写)被转换为相应地址/test/index.php/Index/。

这里简单说一下模板 和action之间的关系。每一个action，对应的模板是与之名字相同的html文件。例如index类下的index()，对应default/index/index.html，而add.html，则显然对应的是index类下的add()。

我们甚至可以在只有add.html而没有相应的add()动作情况下，用访问add()的形式(http://localhost/test /index.php/index/add)来访问add.html模板。add.html模板下的占位符会被替换成相应的数据。效果如下。

（3）从form的"action=__URL__/insert"中可以看出，进行表单处理的动作是/test/index.php/index/insert,所以我们得新增insert动作来处理表单提交数据。在此之前，我们还有一件重要的事情要做，那就是新增model 文件。通过model文件的建立，我们将能在insert动作中使用便捷的方法来操作数据库了
打开/test/test/lib/model/文件夹,新建文件TestModel.class.php。打开他，输入并保存以下代码
复制PHP内容到剪贴板PHP代码:

<?php
class TestModel extends Model {

}
?>

简单的说，这是ActiveRecord实现的基本文件。命名规则是你数据库中的表后面加Model.例如我们将要使用到的表是test,我的文件命名必须是TestModel.class.php，而文件下的类命名必须是TestModel.
接着，我们回到indexaction.class.php文件，删除原来的代码，加入如下代码。
复制PHP内容到剪贴板PHP代码:
class IndexAction extends Action{

    //表单数据添加到数据库
    public function insert() {
        //实例化我们刚才新建的testmodel.
        $test = D('Test'); 
        if ($test->create()) {
        //保存表单数据就这一步。thinkphp已经全部做完了。
            $test->add();
            $this->redirect();
        }else{
            exit($test->getError().'[ <A HREF="javascript:history.back()">返 回</A> ]');
        }
    }        
}

(4)接下来，我们需要在IndexAction类中增加一个首页默认显示动作index()来调用表单数据。
复制PHP内容到剪贴板PHP代码:
public function index() {
//依旧是实例化我们新建的对应相应表名的model.这是我们进行快捷表操作的重要关键。
        $test = D('Test');
        //熟悉这段代码么？计算所有的行数
        $count = $test->count('','id');
        //每页显示的行数
        $listRows = '3';
        //需要查询 哪些字段
        $fields = 'id,name,content';
        //导入分页类 /ThinkPHP/lib/ORG/Util/Page.class.php
import ("ORG.Util.Page");
 //通过类的构造函数来改变page的参数。$count为总数，$listrows为每一页的显示条目。
        $p = new Page($count,$listRows);
 //设置 查询参数。具体见“ThinkPHP/Lib/Think/Core/Model.class.php"1731行。
        $list = $test->findall('',$fields,'id desc',$p->firstRow.','.$p->listRows);
        //分页类做好了。
        $page = $p->show();
        //模板输出
        $this->assign('list',$list);
        $this->assign('page',$page);
        $this->display();
    }

我们该设置一个模板了。在/test/test/tpl/default/index/下新建index.html（因为默认对应了index().所以程序中可以直接assign.而不用去指定模板文件。当然，这是可以配置的。）
复制PHP内容到剪贴板PHP代码:
<hr><a href="__URL__/add">填写</a>
//分页显示，这一行
<hr>{$page}<hr>
//数据显示。下面的参数很快会再进行详解。它很好理解。
<volist name="list"
 id="vo">
<p>姓名：{$vo.name}</p>
<p>内容:{$vo.content}</p>
<hr>
</volist>
保存他。接着输入 http://localhost/test/


