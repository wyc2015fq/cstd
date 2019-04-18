# 二十三、番外篇之搜索引擎优化（SEO） - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:49:01[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：255
个人分类：[做一个全栈工程师](https://blog.csdn.net/jiangjingxuan/article/category/6694850)













![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_112_default_big.jpeg)



网站发布了，怎么能快速被百度等搜索引擎收录呢？怎么能更容易被用户搜到呢？这是SEO范畴的知识，本节来讲述一下，并帮你把你的网站做一些优化

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 网站结构

搜索引擎分为三部分：抓取、建索引、检索。抓取就是通过爬虫软件自动爬取你的网站内容存储到搜索引擎的网页库中，建索引就是对抓取到的网页内容做分析并建成倒排索引，检索就是让用户在搜索框中能够搜到你的网页。

抓取的过程是通过外部指向你网站的某个链接或者你的主动推送的种子链接开始广度加深度遍历，最终抓取完你的整个网站，当然后续还会不断的重试抓取你的网页，如果发现新的链接还会继续抓取，保持时新性。

为了让搜索引擎抓取我们的网站，就需要把我们的网站结构做的足够简单清晰，建议尽量减少链接深度：爬虫是优先做广度遍历的，链接深度约小，就更容易被快速爬取



## 内链外链

爬虫是通过超链接来遍历网站的，根据排名算法，指向网站的链接越多是会提升网站排名的，所以尽量建立足够多的优质外链，这里提到优质，不是说外链越多越好，过多会被搜索引擎antispam掉，重要的是优质，有较多转化流量才能带来用户的访问，所以可以优化我们的网站，增加网页里的链接数目，比如可以在网站底部加上“最新文章”，如果你按照前面章节一步一步实现了我们的博客网站的话，那么可以修改app/Resources/views/base.html.twig，在版权说明前加上：



```
<div class="row">
        <div class="col-sm-1 col-xs-1"></div>
        <div class="col-sm-4 col-xs-4">
            <h4 style="color: #FFFFFF; border-bottom: 1px solid #695d69; padding-bottom: 10px; margin-top: 30px;">
                最新文章</h4>
            {% for article in latestblogs %}

                <div class="row" style="margin: 10px;margin-left: 0; overflow:hidden;text-overflow:ellipsis;white-space:nowrap;">
                    <a style="color: #959595;"
                       href="{{ path('blog_show', {'blogId':article.id}) }}">{{ article.title }}</a>
                </div>

            {% endfor %}
            <br/>
        </div>
        <div class="col-sm-1 col-xs-1"></div>

        <div class="col-sm-4 col-xs-4">
            <h4 style="color: #FFFFFF; border-bottom: 1px solid #695d69; padding-bottom: 10px; margin-top: 30px;">
                为你推荐</h4>
            {% for article in recommends %}

                <div class="row" style="margin: 10px;margin-left: 0;overflow:hidden;text-overflow:ellipsis;white-space:nowrap;">
                    <a style="color: #959595;"
                       href="{{ path('blog_show', {'blogId':article.id}) }}">{{ article.title }}</a>
                </div>

            {% endfor %}
            <br/>
        </div>

        <div class="col-sm-2 col-xs-2"></div>
    </div>
```

并修改src/AppBundle/Controller/BlogController.php，为BlogController增加如下方法：

```php
public static function getLatestBlogs($contoller)
    {
        $blogPostRepository = $contoller->getDoctrine()->getRepository('AppBundle:BlogPost');
        $blogposts = $blogPostRepository->findBy(array(), array('createTime' => 'DESC'), 5);
        return $blogposts;
    }

    public static function getRecommends($contoller)
    {
        $blogPostRepository = $contoller->getDoctrine()->getRepository('AppBundle:BlogPost');
        $allrecommends = $blogPostRepository->findBy(array(), array('createTime' => 'DESC'), 100);

        $randList = BlogController::genRandList(0, sizeof($allrecommends), 5);
        $recommends = array();
        foreach ($randList as $index => $value) {
            $recommends[] = $allrecommends[$index];
        }
        return $recommends;
    }

    public static function genRandList($min, $max, $num)
    {
        $num = min($num, $max-$min);
        $map = array();
        while (sizeof($map) < $num) {
            $r = rand($min, $max-1);
            $map[$r] = 1;
        }
        return $map;
    }
```

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

并修改listAction方法的render语句如下：

```php
return $this->render('blog/list.html.twig', array('pagination' => $pagination,
            'subject' => $subject,
            'latestblogs' => BlogController::getLatestBlogs($this),
            'recommends' => BlogController::getRecommends($this)));
```

修改showAction方法的render语句如下:

```php
public function showAction($blogId)
    {
        $this->blogPostRepository = $this->getDoctrine()->getRepository('AppBundle:BlogPost');
        return $this->render('blog/show.html.twig', array('blogpost' => $this->blogPostRepository->find($blogId),
            'latestblogs' => BlogController::getLatestBlogs($this),
            'recommends' => BlogController::getRecommends($this)));
    }
```

同时修改src/AppBundle/Controller/DefaultController.php里的indexAction方法的render语句，如下：

```php
return $this->render('default/index.html.twig', array(
            'subjects' => $subjects,
            'blogcounts' => $this->getSubjectBlogCountMap($subjects),
            'latestblogs' => BlogController::getLatestBlogs($this),
            'recommends' => BlogController::getRecommends($this),
        ));
```

看下效果：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/02879e933a81e035f3f127cde28d0f480ef5d141.png)



解释一下，这里的getLatestBlogs（最新文章）就是获取最近发布的几篇文章，这里的getRecommends（为你推荐）其实是一个伪的推荐，里面就是做了一些随机性，这样的随机性有一些好处，就是可以让搜索引擎每次抓取的结果都有所不同，这样它会认为你的网站时刻有更新，从而缩短抓取周期，更容易发现你的新链接



## 内容质量

网站的内容质量是搜索引擎优化的关键，搜索引擎是基于文本的，所以你的页面里要尽量包含足够多的文本信息，图片和js对于搜索引擎来说无疑增加了它的分析难度。努力上传优秀的原创文章一定会受搜索引擎的青睐，一味的转载，搜索引擎是有办法知道谁是原创谁是抄袭的，原创一定会被排在前面的



## 主动提交

百度站长平台提供给了我们主动提交链接的方法，到[http://zhanzhang.baidu.com/](http://zhanzhang.baidu.com/)注册一个账号，获取到自动提交代码嵌入到网页里，可以在你发布文章的第一时间传给百度知晓，这块相信你自己能够搞定，我的实例代码如下，仅供参考：

```java
<script>
(function(){
    var bp = document.createElement('script');
    bp.src = '//push.zhanzhang.baidu.com/push.js';
    var s = document.getElementsByTagName("script")[0];
    s.parentNode.insertBefore(bp, s);
})();
</script>
```






