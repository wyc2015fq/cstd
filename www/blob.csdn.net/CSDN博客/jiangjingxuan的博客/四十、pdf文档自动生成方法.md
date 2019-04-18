# 四十、pdf文档自动生成方法 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:57:20[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：746










![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_165_default_big.jpeg)



积累了很多博客，希望能产出一个pdf电子书来分发给可能感兴趣的人，但是编辑pdf是比较繁琐的，怎么能直接利用我们的html页面生成一篇pdf呢？本节讲解pdf的自动生成方法

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 安装配置KnpSnappyBundle扩展

先安装扩展，执行：
`composer require knplabs/knp-snappy-bundle`


再安装wkhtmltopdf工具

```
wget http://download.gna.org/wkhtmltopdf/0.12/0.12.3/wkhtmltox-0.12.3_linux-generic-amd64.tar.xz
xz -d wkhtmltox-0.12.3_linux-generic-amd64.tar.xz
```

拷贝二进制到/usr/local/bin

因为linux存在无法转中文的问题，所以下载一个simsun.ttc拷贝到/usr/share/fonts即可



## 创建pdf链接

配置路由如下：

```
pdf_latest:
    path:     /pdf/{title}_{year}_{month}_{day}.pdf
    requirements:
        "year": "201[6-9]"
        "month": "[01]\d"
        "day": "[0123]\d"
    defaults: { _controller: AppBundle:Pdf:generate }
```



这里的title后面会作为查数据的关键词来形成一类pdf（这样做的是为了按不同类别把文章生成多个pdf）

这里的年月日后面会用来限定博客的最后更新日志，这样这个pdf会看起来定期有更新

配置里的**requirements通过正则表达式约束url的值**



## 创建Pdf**Contoller**

**如下：**

```php
<?php

namespace AppBundle\Controller;

use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use Symfony\Component\HttpFoundation\BinaryFileResponse;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpKernel\Kernel;
use Doctrine\ORM\EntityManager;
use Doctrine\ORM\Query;
use Doctrine\Common\Persistence\ObjectRepository;

class PdfController extends Controller
{

    /**
     * @var ObjectRepository
     */
    protected $blogPostRepository;
    /**
     * @var Kernel
     */
    protected $kernel;

    /**
     * @var EntityManager
     */
    protected $em;

    public function generateAction(Request $request, $title, $year, $month, $day)
    {
        $time = mktime(0, 0, 0, $month, $day, $year);
        $now = time();
        if ($time > $now) {
            return new Response('file not exist');
        }
        $this->kernel = $this->get('kernel');
        $rootDir = $this->kernel->getRootDir();
        $pdfFilePath = $rootDir . '/../web/pdf/' . $title . '/' . $year . "-" .  $month . "-" .  $day . '.pdf';

        $this->em    = $this->get('doctrine.orm.entity_manager');

        $qb = $this->em->createQueryBuilder();
        $q = $qb->select(array('blogpost'))->from('AppBundle:BlogPost', 'blogpost')
            ->where($qb->expr()
                ->lt('blogpost.createTime', '\'' . date("Y-m-d 00:00:00", $time) . '\'')
            )
            ->andWhere($qb->expr()
                ->like('blogpost.title', '\'%' . $title . '%\'')
            )
            ->getQuery();

        if (file_exists($pdfFilePath)) {
            unlink($pdfFilePath);
        }
        $this->get('knp_snappy.pdf')->generateFromHtml(
            $this->renderView(
                'pdf/generate.html.twig',
                array('blogposts' => $q->getresult())
            ),
            $pdfFilePath
        );


        return new BinaryFileResponse($pdfFilePath);
    }

}
```

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

讲解一下：

首先要判断指定的年月日是不是超过当前日期，如果超过会返回错误

接下来确定生成本地的pdf文件路径

然后用url获取到的年月日和title来查数据库，获取博客集合

再利用**knp_snappy.pdf服务把网页渲染好并转成pdf**

**最后返回pdf文件作为Response**



## **创建渲染模板generate.html.twig**

```
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="utf-8">
    <!-- 新 Bootstrap 核心 CSS 文件 -->
    <link rel="stylesheet" href="https://cdn.bootcss.com/bootstrap/3.3.5/css/bootstrap.min.css">

    <!-- 可选的Bootstrap主题文件（一般不用引入） -->
    <link rel="stylesheet" href="https://cdn.bootcss.com/bootstrap/3.3.5/css/bootstrap-theme.min.css">

    <!-- jQuery文件。务必在bootstrap.min.js 之前引入 -->
    <script src="https://cdn.bootcss.com/jquery/1.11.3/jquery.min.js"></script>

    <!-- 最新的 Bootstrap 核心 JavaScript 文件 -->
    <script src="https://cdn.bootcss.com/bootstrap/3.3.5/js/bootstrap.min.js"></script>

</head>
<body>
{% for blogpost in blogposts %}

    <div class="container-fluid">
        <div class="row">
            <div class="col-sm-3 col-xs-1"></div>
            <div class="col-sm-6 col-xs-10">
                <div class="row">
                    <h1><a href="http://{{ app.request.httphost }}{{ path('blog_show', {'blogId':blogpost.id}) }}"
                           target="_blank">{{ blogpost.title }}</a></h1>
                </div>
                <div class="row">
                    <small>发表于 {{ blogpost.createTimeStr }}</small>
                </div>

                <div class="row">
                    <hr/>
                </div>
                <div class="row">
                    <br/>
                    {{ blogpost.body|replace({'img alt="" src="':"img src=\"http://#{app.request.httphost}"})|raw }}
                </div>
            </div>
        </div>
    </div>

    <br/>
    <br/>

{% endfor %}
</body>
</html>
```

讲解一下：

这依然是按照html的方式加载bootstrap框架，遍历所有的博客并展示他们的title、createTimeStr、body等

注意：这里的a和img里的链接都是用完整的url，这是因为如果用相对的url则wkhtmltopdf无法生成超链接和图片



直接访问http://localhost/app_dev.php/pdf/全栈工程师/2016-06-07.pdf就可以直接下载到2016-06-07以前title包含“全栈工程师”的所有博客拼成的一个pdf文件啦




