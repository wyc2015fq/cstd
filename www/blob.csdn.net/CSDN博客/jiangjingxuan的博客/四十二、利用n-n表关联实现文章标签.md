# 四十二、利用n-n表关联实现文章标签 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:58:28[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：382












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_179_default_big.jpeg)



当文章积累了比较多时，总是希望能够从某一个方面把整理到一起，但一篇文章可能同时属于大数据类和机器学习类，所以这会是一个多对多的关系，那么如何实现这种关系呢？本节我们通过一种标签功能来说明

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 创建标签Entity

首先我们要创建标签实体类，创建src/AppBundle/Entity/Tag.php文件，内容为：

```php
<?php

namespace AppBundle\Entity;

use Doctrine\ORM\Mapping as ORM;
use Doctrine\Common\Collections\ArrayCollection;

/**
 * Tag
 *
 * @ORM\Table()
 * @ORM\Entity
 */
class Tag
{
    /**
     * @ORM\ManyToMany(targetEntity="BlogPost", mappedBy="tag")
     */
    private $blogPosts;

    public function __construct()
    {
        $this->blogPosts = new ArrayCollection();
    }

    public function getBlogPosts()
    {
        return $this->blogPosts;
    }


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
     * @ORM\Column(name="name", type="string", length=255)
     */
    private $name;

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
     * Set name
     *
     * @param string $name
     * @return Tag
     */
    public function setName($name)
    {
        $this->name = $name;

        return $this;
    }

    /**
     * Get name
     *
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }
}
```

## 创建关联关系

有了标签实体Tag，我们要把BlogPost和Tag关联起来，这是一个多对多的关系，因此在src/AppBundle/Entity/BlogPost.php中添加：

```php
/**
 * @ORM\ManyToMany(targetEntity="Tag")
 * @ORM\JoinTable(name="blogpost_tag",
 *   joinColumns={@ORM\JoinColumn(name="blogpost_id", referencedColumnName="id")},
 *   inverseJoinColumns={@ORM\JoinColumn(name="tag_id", referencedColumnName="id")}
 * )
 */
private $tags;
```



同时生成它的get和set方法

```php
/**
 * @return mixed
 */
public function getTags()
{
    return $this->tags;
}

/**
 * @param mixed $tags
 */
public function setTags($tags)
{
    $this->tags = $tags;
}
```

## 创建mysql表

执行
`php app/console doctrine:schema:update --force`
会自动生成两个表：

```
Tag(id, name)
blogpost_tag(blogpost_id, tag_id)
```



## 增加Tag的Admin类

为了能在sonataadmin管理后台对Tag进行管理，我们创建src/AppBundle/Admin/TagAdmin.php内容如下：

```php
<?php

namespace AppBundle\Admin;

use Sonata\AdminBundle\Admin\Admin;
use Sonata\AdminBundle\Datagrid\ListMapper;
use Sonata\AdminBundle\Datagrid\DatagridMapper;
use Sonata\AdminBundle\Form\FormMapper;

class TagAdmin extends Admin
{
    protected function configureFormFields(FormMapper $formMapper)
    {
        $formMapper->add('name', 'text');
    }

    protected function configureDatagridFilters(DatagridMapper $datagridMapper)
    {
        $datagridMapper->add('name');
    }

    protected function configureListFields(ListMapper $listMapper)
    {
        $listMapper->addIdentifier('name');
    }
}
```

## 增加tag的service配置

修改app/config/services.yml，增加：

```
admin.tag:
    class: AppBundle\Admin\TagAdmin
    arguments: [~, AppBundle\Entity\Tag, ~]
    tags:
        - { name: sonata.admin, manager_type: orm, label: Tag }
```



## 增加BlogPost的tag管理控件

修改src/AppBundle/Admin/BlogPostAdmin.php，在configureFormFields中增加

```php
->with('Meta data', array('class' => 'col-md-3'))
->add('tags', null, array(
    'class' => 'AppBundle\Entity\Tag',
    'property' => 'name',
))
->end()
```

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

好，现在看一下效果，打开后台管理页面，我们看到多了Tag的一组管理：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/2de59854e84c1e59b382dacf44c749cee1daf631.png)

点开List可以看到我刚刚添加的两个标签

![](http://www.shareditor.com/uploads/media/my-context/0001/01/b1c0a9628c36c65a733dde4b78c017e42f935701.png)





新建一个BlogPost，也会发现在右侧多了Tag的填写框：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/d288e127c4c46be78eb55767fcb725ae79fa298d.png)



因为BlogPost和Tag是多对多的关系，所以这里是可以填写多个值的：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/cbdac405632b7d011131f2d9251cd88feded0824.png)





## Tag的展示

Tag的管理功能已经完成了，那么怎么才能把tag展示出来呢？我们希望有两处展示，一处是单独一个页面，列出指定tag的所有文章列表，另一处是在文章展示页面展示出它的所有标签，同时链接到tag文章列表

为BlogPost创建listbytagAction，修改src/AppBundle/Controller/BlogController.php，在适当位置增加如下内容（如果你看了前面的文章，你就知道放到什么位置了）：

```php
use Doctrine\ORM\EntityManager;
use Doctrine\ORM\QueryBuilder;



/**
 * @var EntityManager
 */
protected $em;
/**
 * @var QueryBuilder
 */
protected $builder;





public function listbytagAction(Request $request)
{
    $tagName = $request->get('tagname');
    $this->em = $this->get('doctrine.orm.entity_manager');
    $this->builder = $this->em->createQueryBuilder();
    $query = $this->builder->select('b')
        ->add('from', 'AppBundle:BlogPost b INNER JOIN b.tags t')
        ->where('t.name=:tag_name')
        ->setParameter('tag_name', $tagName)
        ->getQuery();

    $paginator = $this->get('knp_paginator');
    $pagination = $paginator->paginate(
        $query,
        $request->query->get('page', 1)/*page number*/,
        100/*limit per page*/
    );

    return $this->render('blog/listbytag.html.twig', array('pagination' => $pagination,
        'tagname' => $tagName,
        'latestblogs' => BlogController::getLatestBlogs($this),
        'tophotblogs' => BlogController::getTopHotBlogs($this)));
}
```

创建app/Resources/views/blog/listbytag.html.twig，内容如下：

```
{% extends "base.html.twig" %}

{% block title %}{{ tagname }} - SharEDITor - 关注大数据技术{% endblock title %}

{% block body %}

<div class="row">
    <div class="col-sm-3 col-xs-1"></div>
    <div class="col-sm-6 col-xs-10">
        <h1>{{ tagname }}</h1>
    </div>
    <div class="col-sm-3 col-xs-1"></div>
</div>
<div class="row">
    <div class="col-sm-3 col-xs-1"></div>
    <div class="col-sm-6 col-xs-10">
        <br />
        {% for article in pagination %}
            <h4><a href="{{ path('blog_show', {'blogId':article.id}) }}">{{ article.title }}</a>({{ article.createDate }})</h4>
        {% endfor %}
        <div class="navigation">
            {{ knp_pagination_render(pagination) }}
        </div>

    </div>
    <div class="col-sm-3 col-xs-1"></div>
</div>

{% endblock body %}
```

为这个action创建路由，修改app/config/routing.yml，增加如下内容：

```
blog_listbytag:
    path:     /bloglistbytag/
    defaults: { _controller: AppBundle:Blog:listbytag }
```



修改app/Resources/views/blog/show.html.twig，在展示subject和category两个标签的后面添加：

```
{% for tag in blogpost.tags %}
<a class="btn btn-warning btn-xs" href="{{ path('blog_listbytag', {'tagname':tag.name}) }}">
    {{ tag.name }}
</a>
{% endfor %}
```





在app/Resources/views/blog/list.html.twig也同样加入如下内容：

```
{% for tag in article.tags %}
    <a class="btn btn-warning btn-xs" href="{{ path('blog_listbytag', {'tagname':tag.name}) }}">
        {{ tag.name }}
    </a>
{% endfor %}
```

下面欣赏一下最终效果：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/753513655c95c682cdc324481008d49194caf9d9.png)



![](http://www.shareditor.com/uploads/media/my-context/0001/01/7f48b430c1ce93fda7eb5aef872dce3958d7fc74.png)



![](http://www.shareditor.com/uploads/media/my-context/0001/01/817704703fb5c0b98096847315745d43ab686f1a.png)




