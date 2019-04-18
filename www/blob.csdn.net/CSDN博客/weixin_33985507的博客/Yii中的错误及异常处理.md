# Yii中的错误及异常处理 - weixin_33985507的博客 - CSDN博客
2016年10月09日 10:48:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
Yii中的错误及异常处理
Yii已经默认已经在CApplication上实现了异常和错误的接管，这是通过php的set_exception_handler, set_error_handler实现的。通过这两个PHP内置函数，可以对程序中未捕获的异常以及错误进行接管处理，从而提高程序的可维护性。这在大型系统是至关重要的，当发生错误时，我们希望能将相关详细信息记录，甚至是即时发送报警，从而缩短故障修复时间，提高整个系统的稳定性。
默认情况下，Yii会将异常处理分配给CApplication::handleException, 将错误处理分配给CApplication::handleError，但是可以通过在入口文件中定义YII_ENABLE_EXCEPTION_HANDLER, YII_ENABLE_ERROR_HANDLER两个常量为false禁止使用Yii的异常和错误接管机制。
以下内容中，将异常和错误统称为错误，如有必要会进行详细区分说明。
YII_DEBUG常量（默认为false, 可以在入口文件中设置）对错误信息的显示有很重要的影响，debug模式下，错误的输出是最详细的。而程序一旦投入运行，则应将YII_DEBUG修改为false。
无论是否处于debug模式，Yii程序产生错误时均会将相关错误信息进行记录(错误级别为error, 分类为php)。不同之处是debug模式时会直接在web页上显示详细信息。
CApplication:: handleError($code,$message,$file,$line) 中实现了相关逻。特别注意restore_error_handler,
restore_exception_handler两个函数，如果没有这两个函数的调用，那么在后续的错误处理过程中，当再次产生异常或是错误时，又会调用CApplication:: handleError ，从而可能造成死循环，故Yii在此处临时禁止了使用CApplication:: handleError 接管后续的错误和异常(使用php默认的错误处理机制)，这就保证了不会因之产生循环调用。
PHP错误的处理
当产生错误时，PHP会在日志中记录哪些信息？
错误代码（即PHP的E_ERROR E_WARNING  E_STRICT E_DEPRECATED）
消息内容（如 Undefined vaiable $input）
产生错误的文件路径
产生错误的行号
额外的跟踪回溯信息（这是通过debug_backtrace实现的）
当前URL
除了记录相应日志之外，Yii还会对错误进行后续处理（如中断运行、显示错误页等），默认情况下错误的处理会交给CErrorHandler组件处理（但可以通过给CApplicaton绑定onError事件处理器而实现错误处理的二次接管，此处的设计很灵活！）。
此时将产生一个CErrorEvent(并包含$code,$message,$file,$line几项关键参数)，传递给CErrorHandler组件进行处理。具体是交给CErrorHandler::handleError处理之。这个流程主要是将错误相关信息进行整理，并以合适的方式进行显示。
是否为debug模式（YII_DEBUG==true），对错误信息的显示结果有极大影响。调试模式下我们希望能显示详细的错误跟踪信息，而在生产模式下，我们希望给用户显示友好的页面。所以，此处的错误显示有所不同，下面区分说明之。
当处于调试模式时，将直接渲染exception视图展示错误。将按以下路径搜索：
protected/views/system/exception.php
YII_PATH/views/exception.php
显然，默认情况下并没有在应用程序中定义views/system目录，故会使用系统框架自带的视图文件。最终包含的文件将是Yii框架中的views/exception.php。
从以上分析中可以得知，在调试模式下如果我们要使用自定义异常页面（一般这么做的意义可能不大），则需要配置文件protected/views/system/exception.php, 可使用的变量即$data。
当处于非调试模式下时，会作如下处理：
1.  配置文件中若为errorHandler组件定义了errorAction路由信息，则直接运行之，否则执行第2步流程。
2.  尝试加载error视图，按以下路径搜索（第一个搜索到的文件将被使用） 
protected/views/system/zh_cn/error500.php
protected/views/system/error500.php
protected/views/system/zh_cn/error.php
protected/views/system/error.php
YII_PATH/views/zh_cn/error500.php
YII_PATH/views/error500.php
YII_PATH/views/zh_cn/error.php
Y II_PATH/views/error.php
异常的处理
根据前面的分析，异常的处理机制与错误处理机制类似，也会记录日志，级别是error, 分类为"exception.$EXCEPTIONCLASS", 若是CHttpException类异常，分类名称则为exception.CHttpException.$STATUS_CODE。如数据的异常分类称为exception.CDbException。
接下来将错误事件CExceptionEvent交由errorHandler处理，所有错误信息都由CExceptionEvent对象传递而来。处理方法如下：
1.  如果是调试模式，则按以下顺序搜索视图文件，第一个搜索到的文件将被使用
protected/views/system/exception.php
YII_PATH/views/exception.php
2.  如果是非调试模式，并在配置文件中为errorHandler组件定义了errorAction属性路由，则运行之，否则进入第3步。
3.  按以下顺序尝试加载视图文件，第一个搜索到的文件将被使用
3.  protected/views/system/zh_cn/error500.php
protected/views/system/error500.php
protected/views/system/zh_cn/error.php
protected/views/system/error.php
YII_PATH/views/zh_cn/error500.php
YII_PATH/views/error500.php
YII_PATH/views/zh_cn/error.php
Y II_PATH/views/error.php
使用流程图描述，会更清楚一些：
搜寻视图文件流程比较重要，因为它关系到我们如何自定义错误页面的细节问题，后续的流程图详细描述其过程。如果有任何疑惑，请联系zhangxugg@163.com,  共同控讨。
![](http://dl.iteye.com/upload/attachment/0073/3061/cf91d72c-3e7e-3e75-bcca-d227e3de43fa.jpg)
从图中可以看出，最容易的方式还是给errorHandler组件设置errorAction属性指定错误发生的路由
![](http://dl.iteye.com/upload/attachment/0073/3063/29cf0d89-a7db-302f-aa31-69ff888fcf35.jpg)
一般而言，我们最关心的是生产模式下错误页面的显示问题，经过以上分析，有两种方法可用：
1. 配置文件中为errorHandler组件定义errorAction路由属性（应该优先使用这个方式，以达到灵活配置目的）
2. 定义以下文件中的任意一个，实现自定义错误页(不推荐) 
Protected/views/system/zh_cn/error500.php
protected/views/system/error500.php
protected/views/system/zh_cn/error.php
protected/views/system/error.php
第1种方式灵活可控，可以在控制器中指定视图文件，灵活可控。
- ![](http://dl2.iteye.com/upload/attachment/0073/3061/cf91d72c-3e7e-3e75-bcca-d227e3de43fa-thumb.jpg)
- 大小: 62.8 KB
- ![](http://dl2.iteye.com/upload/attachment/0073/3063/29cf0d89-a7db-302f-aa31-69ff888fcf35-thumb.jpg)
[链接](http://zhangxugg-163-com.iteye.com/blog/1671378)
