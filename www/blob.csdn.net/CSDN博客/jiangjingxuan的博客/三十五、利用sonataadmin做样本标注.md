# 三十五、利用sonataadmin做样本标注 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:54:57[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：601












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_139_default_big.png)



要想实现对新发现的公众号文章做自动分类，需要对样本做训练，而样本属于哪一类别是需要我们做人工标注的，直接操作数据库非常不方便，我想到了我们的网站后台管理系统，直接把样本数据整合进来就可以方便标注了

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 创建CrawlPage实体



在sonataadmin中每一张表都要对应一个Entity实体，也就是MVC里的model，因此我们在网站目录中创建src/AppBundle/Entity/CrawlPage.php，内容如下：

```php
<?php

namespace AppBundle\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * CrawlPage
 *
 * @ORM\Table()
 * @ORM\Entity
 */
class CrawlPage
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
     * @ORM\Column(name="title", type="string", length=255, unique=true)
     */
    private $title;

    /**
     * @var string
     *
     * @ORM\Column(name="body", type="text")
     */
    private $body;

    /**
     * @var string
     *
     * @ORM\Column(name="content", type="text", nullable=true)
     */
    private $content;

    /**
     * @var \DateTime
     *
     * @ORM\Column(name="create_time", type="datetime")
     */
    private $createTime;

    /**
     * @var string
     *
     * @ORM\Column(name="source", type="string", length=255)
     */
    private $source;

    /**
     * @var string
     * @ORM\Column(name="isTec", type="string", length=8, options={"default": "0"})
     */
    private $isTec;

    /**
     * @var string
     * @ORM\Column(name="isSoup", type="string", length=8, options={"default": "0"})
     */
    private $isSoup;

    /**
     * @var string
     * @ORM\Column(name="isMR", type="string", length=8, options={"default": "0"})
     */
    private $isMR;

    /**
     * @var string
     * @ORM\Column(name="isMath", type="string", length=8, options={"default": "0"})
     */
    private $isMath;
……
```

省略的部分是getter和setter

这里面除了网页表本身的几个字段（title、body、content、createTime、source）之外，创建几个0-1标注字段：isTec（是否纯技术）、isSoup（是否鸡汤文）、isMR（是否机器学习）、isMath（是否数学），这个用来做分类标注

另外，我们为title添加了unique=true属性，为了保证表里的title是唯一key，避免重复写入数据。同时为几个标注字段添加了options={"default": "0"}选项，默认都是非选中



## 创建CrawlPage管理类

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

为了在sonataadmin后台页面中方便标注数据，创建CrawlPage的管理类，创建src/AppBundle/Admin/CrawlPageAdmin.php文件，内容如下：

```php
<?php

namespace AppBundle\Admin;

use Sonata\AdminBundle\Admin\Admin;
use Sonata\AdminBundle\Datagrid\ListMapper;
use Sonata\AdminBundle\Form\FormMapper;
use Sonata\AdminBundle\Datagrid\DatagridMapper;

class CrawlPageAdmin extends Admin
{
    protected function configureFormFields(FormMapper $formMapper)
    {
        $formMapper
            //->tab('Post')
            ->with('Content', array('class' => 'col-md-9'))
            ->add('title', 'text')
            ->add('source', 'text')
            ->add('body', 'ckeditor', array('autoload' => true))
            ->add('create_time', 'sonata_type_date_picker', array(
                'format'=>'yyyy-MM-dd HH:mm:ss',
                'dp_default_date'        => date('Y-m-d H:i:s'),))
            ->end()

            ->with('Meta data', array('class' => 'col-md-3'))
            ->end();
    }

    protected function configureListFields(ListMapper $listMapper)
    {
        $listMapper
            ->addIdentifier('title')
            ->add('isTec', 'boolean', array('editable' => 'Yes'))
            ->add('isSoup', 'boolean', array('editable' => 'Yes'))
            ->add('isMR', 'boolean', array('editable' => 'Yes'))
            ->add('isMath', 'boolean', array('editable' => 'Yes'))
            ->add('source')
            ->add('createTime')
            ;
    }

    public function toString($object)
    {
        return $object instanceof BlogPost
            ? $object->getTitle()
            : 'Crawl Page'; // shown in the breadcrumb on the create view
    }

    protected function configureDatagridFilters(DatagridMapper $datagridMapper)
    {
        $datagridMapper
            ->add('title')
            ;
    }
}
```

注意这里的configureListFields方法中的

```php
->add('isTec', 'boolean', array('editable' => 'Yes'))
            ->add('isSoup', 'boolean', array('editable' => 'Yes'))
            ->add('isMR', 'boolean', array('editable' => 'Yes'))
            ->add('isMath', 'boolean', array('editable' => 'Yes'))
```

使用boolean类型可以在列表页中通过ajax修改数据，这样就不必点开文章、修改、保存那么麻烦了



## 修改配置并生效



修改app/config/services.yml添加：

```
admin.crawl_page:
        class: AppBundle\Admin\CrawlPageAdmin
        arguments: [~, AppBundle\Entity\CrawlPage, ~]
        tags:
            - { name: sonata.admin, manager_type: orm, label: Crawl page }
```

执行
`php app/console doctrine:schema:update --force`
来生效数据库表



## 灌入微信公众号网页数据



如果还没有抓取公众号文章，请见《[教你成为全栈工程师(Full Stack Developer) 三十二-scrapy爬虫抓取网页写入mysql数据库](http://www.shareditor.com/blogshow/45)》

因为之前写入了一个单独的数据库，我们现在把这部分网页导入进来，执行sql语句：
`insert into CrawlPage(title, create_time, source, body, content) select title, post_date, post_user, body, content from test.page;`


打开后台管理页面看下效果：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/6d7b08b278dc296d50a8bee9dc3f4b5bbecdf68f.png)



现在可以直接点击“no”标签来标注数据啦




