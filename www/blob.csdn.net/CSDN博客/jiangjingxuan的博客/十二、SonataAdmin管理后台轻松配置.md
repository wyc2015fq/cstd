# 十二、SonataAdmin管理后台轻松配置 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:42:56[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：1340
个人分类：[做一个全栈工程师](https://blog.csdn.net/jiangjingxuan/article/category/6694850)










![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_104_default_big.png)



sonata-admin管理后台不只是简单地把数据库表的管理映射到网页上，它有非常多的支持对各种数据类型的编辑组件，本节来举几个例子，包括文本编辑插件ckeditor的支持，它能让你像编辑word文档一样所见即所得，还包括时间选择器，让你方便填写时间类型。

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 特殊字段的困扰



上节中我们编辑一个blogpost时是这样的：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/1bd4e9b5d2a414b8ce752bcb295320e3c4f31387.png)



这里的body本应该有图文并茂的内容，但是我们只能输入一些纯文本，createTime的填写也非常不方便，下面我们来解决这个问题



## ckeditor插件支持



首先安装MediaBundle扩展和SonataFormatterBundle扩展，执行：

```
[root@centos7vm mywebsite]# composer require sonata-project/media-bundle "2.3.*"
[root@centos7vm mywebsite]# composer require sonata-project/formatter-bundle "2.3.*"
```

注册bundle，修改app/AppKernel.php，添加注册如下：

```php
new Ivory\CKEditorBundle\IvoryCKEditorBundle(),
            new Sonata\FormatterBundle\SonataFormatterBundle(),
            new Knp\Bundle\MarkdownBundle\KnpMarkdownBundle(),
            new Sonata\MediaBundle\SonataMediaBundle(),
            new JMS\SerializerBundle\JMSSerializerBundle(),
            new Sonata\EasyExtendsBundle\SonataEasyExtendsBundle(),
            new Application\Sonata\MediaBundle\ApplicationSonataMediaBundle(),
```



修改app/config/config.yml，把doctrine配置组改成如下的样子：

```
doctrine:
    dbal:
        driver:   pdo_mysql
        host:     "%database_host%"
        port:     "%database_port%"
        dbname:   "%database_name%"
        user:     "%database_user%"
        password: "%database_password%"
        charset:  UTF8
        types:
            json: Sonata\Doctrine\Types\JsonType

    orm:
        auto_generate_proxy_classes: "%kernel.debug%"
        entity_managers:
            default:
                mappings:
                    AppBundle:
                        type:      ~
                        dir:       "Entity"
                        prefix:    "AppBundle\Entity"
                        is_bundle: ~
```

并添加如下配置：

```
sonata_formatter:
    formatters:
        markdown:
            service: sonata.formatter.text.markdown
            extensions:
                - sonata.formatter.twig.control_flow
                - sonata.formatter.twig.gist
                - sonata.media.formatter.twig

        text:
            service: sonata.formatter.text.text
            extensions:
                - sonata.formatter.twig.control_flow
                - sonata.formatter.twig.gist
                - sonata.media.formatter.twig

        rawhtml:
            service: sonata.formatter.text.raw
            extensions:
                - sonata.formatter.twig.control_flow
                - sonata.formatter.twig.gist
                - sonata.media.formatter.twig

        richhtml:
            service: sonata.formatter.text.raw
            extensions:
                - sonata.formatter.twig.control_flow
                - sonata.formatter.twig.gist
                - sonata.media.formatter.twig

        twig:
            service: sonata.formatter.text.twigengine
            extensions: [] # Twig formatter cannot have extensions
    ckeditor:
        templates:
            browser: 'SonataFormatterBundle:Ckeditor:browser.html.twig'
            upload: 'SonataFormatterBundle:Ckeditor:upload.html.twig'

sonata_media:
    default_context: default
    db_driver: doctrine_orm # or doctrine_mongodb, doctrine_phpcr
    contexts:
        default:  # the default context is mandatory
            providers:
                - sonata.media.provider.dailymotion
                - sonata.media.provider.youtube
                - sonata.media.provider.image
                - sonata.media.provider.file

            formats:
                small: { width: 100 , quality: 70}
                big:   { width: 500 , quality: 70}

    cdn:
        server:
            path: /uploads/media # http://media.sonata-project.org/

    filesystem:
        local:
            directory:  %kernel.root_dir%/../web/uploads/media
            create:     false

ivory_ck_editor:
    default_config: default
    configs:
        default:
            filebrowserBrowseRoute: admin_sonata_media_media_ckeditor_browser
            filebrowserImageBrowseRoute: admin_sonata_media_media_ckeditor_browser
                # Display images by default when clicking the image dialog browse button
            filebrowserImageBrowseRouteParameters:
                provider: sonata.media.provider.image
            filebrowserUploadRoute: admin_sonata_media_media_ckeditor_upload
            filebrowserUploadRouteParameters:
                provider: sonata.media.provider.file
                    # Upload file as image when sending a file from the image dialog
            filebrowserImageUploadRoute: admin_sonata_media_media_ckeditor_upload
            filebrowserImageUploadRouteParameters:
                provider: sonata.media.provider.image
                context: my-context # Optional, to upload in a custom context
```

修改app/config/routing.yml，添加如下内容：

```
gallery:
    resource: '@SonataMediaBundle/Resources/config/routing/gallery.xml'
    prefix: /media/gallery

media:
    resource: '@SonataMediaBundle/Resources/config/routing/media.xml'
    prefix: /media
```





OK, 清缓存(不行就rm -rf app/cache/*)
`[root@centos7vm mywebsite]# php app/console cache:clear`
安装ckeditor静态文件到web目录：
`[root@centos7vm mywebsite]# php app/console assets:install web`


这时重新打开[http://172.16.142.130/app_dev.php/admin](http://172.16.142.130/app_dev.php/admin)，你应该会看到多出了下面的板块

![](http://www.shareditor.com/uploads/media/my-context/0001/01/5b61e203b773698963790c475de3dbe2e43d491c.png)

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

这是用来管理图片视频等信息的板块，但尚未配置好还不能用，暂时也不需要，我们继续来看我们需要的内容，还是点BlogPost的link_add，看效果：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/9f9a9957af013f4ae2dbbf692ab931aa1bc9312d.png)



这回body可以用来编辑图文并茂的内容啦



但虽然这里可以编辑文本格式，但是如果要上传图片还不能用，我们再来准备些东西



执行：
`[root@centos7vm mywebsite]# php app/console sonata:easy-extends:generate --dest=src SonataMediaBundle`
如果执行成功，会自动生成src/Application/Sonata/MediaBundle，说明我们成功生成了我们自定义的MediaBundle，但是我们一行代码都不需要写



在app/AppKernel.php中注册

```php
new Sonata\IntlBundle\SonataIntlBundle(),
            new Application\Sonata\MediaBundle\ApplicationSonataMediaBundle(),
```



修改app/config/config.yml，把doctrine的orm组改成如下：

```
orm:
        auto_generate_proxy_classes: "%kernel.debug%"
        entity_managers:
            default:
                mappings:
                    ApplicationSonataMediaBundle: ~
                    SonataMediaBundle: ~
                    AppBundle:
                        type:      ~
                        dir:       "Entity"
                        prefix:    "AppBundle\Entity"
                        is_bundle: ~
```

更新数据库表：
`[root@centos7vm mywebsite]# php app/console doctrine:schema:update --force`


创建图片视频等上传目录：

```
[root@centos7vm mywebsite]# mkdir web/uploads
[root@centos7vm mywebsite]# mkdir web/uploads/media
[root@centos7vm mywebsite]# chmod -R 0777 web/uploads
```



好，大功告成，我们试一下上传图片吧，点击ckeditor（上面的编辑器）的图片按钮

![](http://www.shareditor.com/uploads/media/my-context/0001/01/7db3e33e8a63828f5cc6ff864fee524007eabbd5.png)



点击上传选择文件

![](http://www.shareditor.com/uploads/media/my-context/0001/01/818abd92a3f889790fe7061bddddb7fc89b7a443.png)





选好文件，点上传到服务器后，显示

![](http://www.shareditor.com/uploads/media/my-context/0001/01/6609147211b25d18508612bb53d4e8a36d71a8d2.png)





这时已经把图片上传到服务器的web/uploads/media目录下了，点确定就可以插入到我们要编辑的内容里了



ckeditor还有很多丰富的内容可以配置，具体可以参考官方文档，比如代码高亮显示，像下面的样子：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/2ccbe7c1843e48fca3696ede5886272e4d409326.png)



## 漂亮的时间选择器



下面我们来解决时间类型数据的填写问题，我们现在的时间选择是这样的难看难用：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/2013df2eb0a5a1fcd8ace65495f099453ad3ef9f.png)



我们来介绍一种sonata_type_date_picker类型的时间选择器，它是SonataCore中自带的组件



在app/config/config.yml的twig组下添加模板配置：

```
form:
        resources:
            - 'SonataCoreBundle:Form:datepicker.html.twig'
```

修改src/AppBundle/Admin/BlogPostAdmin.php的configureFormFields方法，如下：

```php
$formMapper
            ->add('title', 'text')
            ->add('body', 'ckeditor', array('autoload' => true))
            ->add('create_time', 'sonata_type_date_picker', array(
                'format'=>'yyyy-MM-dd HH:mm:ss',
                'dp_default_date'        => date('Y-m-d H:i:s'),));
```



在管理后台重新编辑一个blog看效果：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/4efffe98b9d0e50b412af39f428ee501985f8e81.png)





