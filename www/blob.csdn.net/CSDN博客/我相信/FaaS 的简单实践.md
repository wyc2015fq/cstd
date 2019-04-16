# FaaS 的简单实践 - 我相信...... - CSDN博客





2018年12月17日 08:00:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：161










FaaS 或者说serverless是一种云计算模型，其主要特点是用户根本不需要租用任何虚拟机ーー从启动虚拟机，执行代码，返回结果和停止虚拟机这些由云提供商处理的整个过程。这比其他云计算实现更具成本效益。它还使开发人员能够更加专注于开发业务逻辑，因为应用程序的某些部分由云提供程序处理。



要启动执行代码的整个过程，必须触发它。触发器可以是一个特定的事件，也可以是对API 管理系统的请求，然后将该代码作为API 端点公开。



最流行的serverless服务之一是 AWS Lambda，它可以与AWS API 网关集成，创建一个serverless的RESTAPI。



## REST API配置



API的配置将由AWS API 网关处理。这包括创建路由、处理输入和输出格式、身份验证等等，而实际代码将由Lambda 管理。



当开启 API 网关仪表板时，为您的网站创建一个新的API。然后，单击操作创建资源在API 中创建一个新的URL 路径。每个资源都可以支持一个或多个方法(GET，POST，put/ patch，DELETE) ，这些方法通过*Actions > Create Method*来添加。

例如，我们可以创建一个名为"post"的资源，它的路径是"/posts"，它有两种方法:
- 
GET —      fetch all posts 把所有的帖子都拿来

- 
POST —      create a new post 创建一个新的帖子


在这一点上，屏幕应该是这样的:

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrVfibfZLBkpZktdqsarBEvUVFdubwYicShtcTjNiaH82gvj8QYIbhYNibTxlbKtrDyTqFAcB43R7xUQQ/640?wx_fmt=jpeg)





还需要处理显示一个单一的帖子，更新一个帖子和删除一个帖子。这些操作在REST API 中会有一个不同的路径，这意味着需要创建一个新的资源。由于这个资源的路径是"/posts / { post id }"，因此它将作为一个子资源创建。要做到这一点，首先单击"posts"资源，然后去操作创建资源。这个资源将在路径(post的 ID)中有一个参数，可以通过将参数名包装为"/posts / { post ID }"的括号来实现。创建资源后，将GET、 PUT 和DELETE 方法添加到其中。



API 现在看起来是这样的:





![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrVfibfZLBkpZktdqsarBEvUFuemrpjRVtx6YOyf8G8MHMibkibmZHoAbpc19mH4QS4lu81rSWq0VkTA/640?wx_fmt=jpeg)



每个方法将执行相应的AWS Lambda 函数。先创建这些函数，然后将它们映射到适当的API 方法。



## 创建 Lambda 函数



点击AWS Lambda，点击"Create a Lambdafunction"。下一个屏幕允许选择编程语言(Node.js或 Python)和预定义的模板之一。选择microservice-http-endpoint，然后在下面的页面中选择API 名称。也可以选择空白函数，并且不用任何预先编写的代码来编写它。



最后，在可以插入代码的页面。可以直接在页面上写这个函数，或者将它作为压缩存档上传(如果它包含自定义库，则需要)。当我们使用预定义模板时，函数是自动生成的，看起来是这样的:

from __future__ importprint_function



import boto3

import json



print('Loading function')





def respond(err,res=None):

    return {

        'statusCode': '400' if err else '200',

        'body': err.message if err elsejson.dumps(res),

        'headers': {

            'Content-Type': 'application/json',

        },

    }





def lambda_handler(event,context):

    '''Demonstrates a simple HTTP endpointusing API Gateway. You have full

    access to the request and response payload,including headers and

    status code.



    To scan a DynamoDB table, make a GETrequest with the TableName as a

    query string parameter. To put, update, ordelete an item, make a POST,

    PUT, or DELETE request respectively,passing in the payload to the

    DynamoDB API as a JSON body.

    '''

    #print("Received event: " +json.dumps(event, indent=2))



    operations = {

        'DELETE': lambda dynamo, x:dynamo.delete_item(**x),

        'GET': lambda dynamo, x:dynamo.scan(**x),

        'POST': lambda dynamo, x:dynamo.put_item(**x),

        'PUT': lambda dynamo, x:dynamo.update_item(**x),

    }



    operation = event['httpMethod']

    if operation in operations:

        payload = event['queryStringParameters']if operation == 'GET' else json.loads(event['body'])

        dynamo =boto3.resource('dynamodb').Table(payload['TableName'])

        return respond(None,operations[operation](dynamo, payload))

    else:

        returnrespond(ValueError('Unsupported method "{}"'.format(operation)))



虽然大多数情况下不需要很多代码(许多人会使用关系数据库而不是NoSQL DynamoDB 数据库) ，但它为如何访问HTTP 请求参数和如何输出响应设置了一个很好的例子。



在创建Lambda 函数时需要注意的另一件事是handler字段。它告诉Lambda 要执行哪个函数，以及函数所在的文件。例如，如果main.py 文件中有一个名为"myfunction"的函数，那么处理程序的值将是"main.myfunction"。在创建函数之后，它们可以映射到相应的API 端点。





要使API 调用 Lambda 函数，请单击一个API 方法，然后进入集成请求。在该页上，将集成类型设置为Lambda 函数，并输入您的亚马逊区域和所需函数的名称。对于所有的API 方法都这样做。



在部署之前，可以测试API。每个API 方法都有一个测试按钮，它将执行它并显示输出。





![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrVfibfZLBkpZktdqsarBEvUonllVmWI6J7wBhw6ehWZpzTKFjKbcqFgyILQjO1moEqIZiaFdxqvd1A/640?wx_fmt=jpeg)

一旦一切准备就绪，去action Deploy API 部署你的REST API。第一次，需要创造一个新的阶段(例如，它可以被称为prod 或生产)，就像一个部署环境。可以有多个阶段，不同的阶段有不同的基础url 和配置。可以在屏幕左侧的Mywebsite API Stages 下找到各个阶段。点击该阶段的名称以获取API 的公共 URL，以及其他配置选项，如缓存和节流。



## 这里展示了一个基本的例子，一个serverless的REST API，使用AWS API 网关和Lambda 构建。它展示了如何在不需要开发常见的API 管理特性的情况下轻松地创建REST API，比如认证、路由、缓存和速率限制等。



更进一步， 物联网的快速采用受到正在通过技术创新改变其业务的公司的支持; 制造商正在提供低成本和高端的设备和物联网平台，使设备集成和管理成为可能。物联网应该转向灵活、可靠和高成本效益的平台，而在基础设施、软件、知识和员工方面投入最少。

## IoT的无服务架构

如何从零开始构建一个物联网解决方案，它的基础设施和维护成本为零，只需要很少的营运成本。为了实现这个概念，可以使用AWS的云功能。 例如，创建实时报告遥测数据的设备模拟器，并通过 API 实时访问这些信息。

AWS IoT 平台是一个强大的物联网框架。 它支持 MQTT 协议，MQTT 协议是应用最广泛的通信协议之一。 选择支持持久化和处理数据的服务也是基于其定价和维护成本。

AWS中所使用的组件列表如下:
- 
AWS IoT ： 用于数据收集和设备管理,

- 
DynamoDB： 文档存储以持久化数据读数,

- 
AWS Lambda ： 无服务器数据处理,

- 
S3：用作静态网站托管的块存储,

- 
Gateway API ：REST 访问数据


![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1Gjczo0LkOFQygKDIjqibxZLJtDNSvEnkMrchVlkkibGehiaLbRGicCaPjqCRHjrwlqGsbzuNicnRhCOULkXuA/640?wx_fmt=jpeg)

总体数据流是以下方式工作的:
- 
设备向 AWS IoT 发送小量数据(每5秒) ,

- 
物联网将数据存储到 DynamoDB 表中* 

- 
Lambda函数每分钟和每小时被触发去做数据分析并将结果存储回 DynamoDB,

- 
API Gateway 通过 REST API 将 DynamoDB 的数据公开

- 
静态 HTML 网站托管在 S3上，并使用 RESTAPI 来显示实时数据图表和分析


第二点乍看起来可能有点傻，因为可能会认为 DynamoDB 不是存储原始时间序列数据的最佳选择。 然而，这里是为了演示的目的。可以考虑使用 Firehose 作为从物联网到 S3/reshift 和 EMR 集群的传输流来进行数据处理，但对于这个简单实践而言，这里只是一个临时的做法。

架构设置了以下关键参数:
- 
免费，如果没有设备报告任何数据。 另外，通过亚马逊的免费版，可以免费获得少量的资源

- 
由于每个选定组件的性质，高度可扩展且可以从AWS中获取

- 
启动只需的最基本知识，只需要定义规则和用一种非常流行的语言编写逻辑: JavaScript，Python 或者 Java


## IoT无服务架构的成本分析

假设后端操作每分钟只需要处理几个请求，这意味着大部分时间您的 CPU 处于空闲状态。 假设不想为空闲时间买单。 因此，这里提出了无服务器架构。

假设有10000个设备每15分钟报告一小部分数据，这就导致每月平均730个小时，每月约有2920万个请求。AWS物联网每100万个请求花费5美元，DynamoDB 每秒花费0.0065美元，每秒需要花费50次。

通过 AWS IoT，每月将付出146美元左右的，14美元用于在 DynamoDB 中运行的最小存储容量，总共有160美元，相当于每台设备每月0.02美元或者每次0.000005美元。 尽管这没有考虑到 lambda、存储器和 API 网关的使用，但它们实际上只是这些数字的一小部分，因此可以省略。

这是令人印象深刻的。物联网解决方案与数以千计的设备连接，这将花费不到200美元每月。 然而，让我们想象一下，如果一个企业的设备每秒钟都在报告关键数据(而不是每隔15分钟) ，而且有成千上万的数据。 还愿意为FaaS付多少钱呢？

如果一万台设备每秒发送一条消息，月付款将超过1.36万美元。如果是10万台设备， 每月每台设备的费用增加到13.61美元，还是挺贵的。



## 无服务架构IoT方案的优缺点

所有这些数字意味着优化的请求率将会立即和几乎线性地导致月度费用减少。这就带来了必须考虑的第二个重要结论，即所有权的总成本。 有一个虚拟的门槛值，超过这个阈值，无关紧要的方法就会变得非常昂贵，而且可能不会有效。

例如，传统的体系结构实现成本可能不是很大程度上取决于设备的数量或每秒请求的数量，而是取决于额外的运营费用，使用开源解决方案也可以降低成本。

毫无疑问，无服务架构有许多优点:
- 
它将资本支出转化为经营支出，并通常降低经营成本;

- 
不必考虑内部系统管理流程;

- 
它减少了开发和部署成本和时间框架(更快的上市时间) ;

- 
它具有可扩展性和容错性


要考虑的第一个因素是为项目的需求， 如果不关心云锁定，而且是一家创业公司，需要快速验证想法，或者有一个很短的时间去营销，或者解决方案不需要频繁地将数据从设备传输到云，因此可以将每台设备的成本保持在相对较低的水平。

另一方面，如果正在构建一个与云无关的、高度可定制的解决方案，并且使用实时数据进行操作，可以考虑使用自定义或开源物联网解决方案。






「本文编译自：

http://www.devx.com/enterprise/creating-a-serverless-api-using-api-gateway-and-lambda.html

https://www.dataart.com/downloads/dataart_white_paper_art_of_low_cost_iot_solution.pdf」



