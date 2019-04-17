# 十、MVC模式中的Model使用方法 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:41:37[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：2939












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_104_default_big.png)



model是MVC里的M，它是MVC里最简单而又复杂的部分，简单是因为有了它你就不必关注数据层，让你把精力集中在业务逻辑，说它复杂是因为要想实现这样的机制是比较困难的，好在symfony2帮我们实现了最困难的部分，连在model中编写业务需要的字段都做成了自动化，当然为了学习我们暂时不用自动化的工具。这一节我们就用model来把博客真正管理起来。

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com/)以及原始链接地址

## 创建一个model定义



创建src/AppBundle/Entity/BlogPost.php文件并编写如下内容：



```php
<?php

namespace AppBundle\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * BlogPost
 *
 * @ORM\Table()
 * @ORM\Entity
 */
class BlogPost
{
    /**
     * @var integer
     *
     * @ORM\Column(name="id", type="integer")
     * @ORM\Id
     * @ORM\GeneratedValue(strategy="AUTO")
     */
    private $id;

    /**
     * @var string
     *
     * @ORM\Column(name="title", type="string", length=255)
     */
    private $title;

    /**
     * @var string
     *
     * @ORM\Column(name="body", type="text")
     */
    private $body;

    /**
     * @var \DateTime
     *
     * @ORM\Column(name="create_time", type="datetime")
     */
    private $createTime;

    /**
     * Get id
     *
     * @return integer
     */
    public function getId()
    {
        return $this->id;
    }

    /**
     * Set title
     *
     * @param string $title
     * @return BlogPost
     */
    public function setTitle($title)
    {
        $this->title = $title;

        return $this;
    }

    /**
     * Get title
     *
     * @return string
     */
    public function getTitle()
    {
        return $this->title;
    }

    /**
     * Set body
     *
     * @param string $body
     * @return BlogPost
     */
    public function setBody($body)
    {
        $this->body = $body;

        return $this;
    }

    /**
     * Get body
     *
     * @return string
     */
    public function getBody()
    {
        return $this->body;
    }

    /**
     * @return \DateTime
     */
    public function getCreateTime()
    {
        return $this->createTime;
    }

    /**
     * @param \DateTime $createTime
     */
    public function setCreateTime($createTime)
    {
        $this->createTime = $createTime;
    }
}
```

BlogPost这个model从含义上表达了一篇博客，从实现上表达了数据库表的一行，有id、title、body、createTime几个属性，其中id是数据库的主键（自增1，不需要setId方法），title是博客的标题，body是博客内容，createTime是博客创建时间



下面我们就要自动化地根据这个model定义生成数据库表，在此之前，我们要把数据库配置好



安装和配置mysql见《[RHEL7或centos7安装mysql5.7方法和配置](http://www.shareditor.com/blogshow/15)》



请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com/)以及原始链接地址

## 配置网站数据库连接



修改app/config/parameters.yml文件，按照你自己的数据库配置改成如下：



```
parameters:
    database_host: 127.0.0.1
    database_port: 3306
    database_name: mywebsite
    database_user: root
    database_password: shareditor@126.COM
    mailer_transport: smtp
    mailer_host: 127.0.0.1
    mailer_user: null
    mailer_password: null
    secret: 1a0cb131fb193436d0f6ce467f2d8b6c7c5b02da
```

mailer部分和secret不知道是做什么的就保持不动

注意：这里我们设置了一个数据库名字：mywebsite，后面会创建这个数据库



下面该是隆重介绍app/console工具的时候了，这是symfony2工程都会带的一个自动化工具，直接执行
`[root@centos7vm mywebsite]# php app/console`
可以看帮助信息，它的功能包括：自动管理缓存、配置、路由、entity等



这次我们用它来初始化数据库，执行
`[root@centos7vm mywebsite]# php app/console doctrine:database:create`
通过mysql查看发现已经建立了mywebsite数据库

```
mysql> show databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| mywebsite          |
| performance_schema |
| sys                |
+--------------------+
```

还记得之前我们之前创建的entity吗？对，就是那个class BlogPost，现在我们用自动化工具把它直接映射成mysq的一张表，执行
`[root@centos7vm mywebsite]# php app/console doctrine:schema:update --force`
通过mysql查看发现已经成功建立了

```
mysql> use mywebsite;
mysql> show tables;
+---------------------+
| Tables_in_mywebsite |
+---------------------+
| blog_post           |
+---------------------+
mysql> desc blog_post;
+-------------+--------------+------+-----+---------+----------------+
| Field       | Type         | Null | Key | Default | Extra          |
+-------------+--------------+------+-----+---------+----------------+
| id          | int(11)      | NO   | PRI | NULL    | auto_increment |
| title       | varchar(255) | NO   |     | NULL    |                |
| body        | longtext     | NO   |     | NULL    |                |
| create_time | datetime     | NO   |     | NULL    |                |
+-------------+--------------+------+-----+---------+----------------+
```

而且表的列和BlogPost中的成员变量一致，(*@ο@*) 哇～amazing!有木有



下面我们手工往数据库里插入一行数据
`mysql> insert into blog_post(title,body,create_time) values('这是标题','这是内容',now());`


## model的读取



修改src/AppBundle/Controller/BlogController.php中的showAction方法如下：

```php
public function showAction(Request $request)
    {
        $blogPostRepository = $this->getDoctrine()->getRepository('AppBundle:BlogPost');
        $blogposts = $blogPostRepository->findAll();
        return $this->render('blog/show.html.twig', array('title' => $blogposts[0]->getTitle(), 'content' => $blogposts[0]->getBody()));
    }
```

这里面的getRepository('AppBundle:BlogPost')就是隐藏了和数据库的交互细节，暴露给你的就是$blogPostRepository这个对象，直接可以做各种find操作，直接能拿到blog_post表的数据

findAll是取出所有结果，所以我用了$blogposts[0]来只拿第一个结果演示



我们还要修改一下app/Resources/views/blog/show.html.twig如下：

```
{% extends "base.html.twig" %}

{% block title %}博客内容{% endblock title %}

{% block body %}
<div class="row jumbotron">
    <div class="col-md-1 col-xs-1"></div>
    <div class="col-md-10 col-xs-10"><h1>{{ title }}</h1></div>
    <div class="col-md-1 col-xs-1"></div>
</div>

<div class="row">
    <div class="col-md-1 col-xs-1"></div>
    <div class="col-md-10 col-xs-10"><h4>{{ content }}</h4></div>
    <div class="col-md-1 col-xs-1"></div>
</div>

{% endblock body %}
```

模板里面展示{{ title }}和{{ content }}，对应的就是array('title' => $blogposts[0]->getTitle(), 'content' => $blogposts[0]->getBody())取出来的title和content



现在看下网页的效果吧：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/3a3c59d191f20432a961b9abc44b9a47b35f3c44.png)



上面展示了model的读取流程：数据库表=>model类实例=>controller透传=>前端展示

model的写入方式也类似，后面章节会通过更高级的功能来继续介绍




