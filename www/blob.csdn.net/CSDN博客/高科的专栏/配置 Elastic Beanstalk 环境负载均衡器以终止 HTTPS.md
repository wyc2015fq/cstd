
# 配置 Elastic Beanstalk 环境负载均衡器以终止 HTTPS - 高科的专栏 - CSDN博客

2018年06月08日 10:18:49[高科](https://me.csdn.net/pbymw8iwm)阅读数：243


﻿﻿
# 配置 Elastic Beanstalk 环境负载均衡器以终止 HTTPS
要更新您的 AWS Elastic Beanstalk 环境以使用 HTTPS，您需要为您的环境中的负载均衡器配置 HTTPS 侦听器。以下两种类型的负载均衡器支持 HTTPS 侦听器：传统负载均衡器和应用程序负载均衡器。
您可以使用 Elastic Beanstalk 控制台或配置文件来配置安全侦听器并分配证书。
注意
单实例环境没有负载均衡器并且不支持负载均衡器上的 HTTPS 终止。

## 使用 Elastic Beanstalk 控制台配置安全侦听器
**将证书分配至环境的负载均衡器**
打开[Elastic Beanstalk 控制台](https://console.aws.amazon.com/elasticbeanstalk)。
导航到您的环境的[管理页](https://docs.aws.amazon.com/zh_cn/elasticbeanstalk/latest/dg/environments-console.html)。
选择Configuration。
在负载均衡器配置卡上，选择修改。
注意
如果负载均衡器配置卡没有修改按钮，则表示您的环境没有[负载均衡器](https://docs.aws.amazon.com/zh_cn/elasticbeanstalk/latest/dg/using-features-managing-env-types.html#using-features.managing.changetype)。

在修改负载均衡器页上，根据与您的环境关联的负载均衡器类型，该过程将会有所不同。
传统负载均衡器
选择添加侦听器。
在传统负载均衡器侦听器对话框中，配置以下设置：
对于侦听器端口，请键入传入流量端口，通常为443。
对于侦听器协议，请选择HTTPS。
对于实例端口，请键入80。
对于实例协议，请选择HTTP。
对于SSL 证书，请选择您的证书。

选择Add。
应用程序负载均衡器
选择添加侦听器。
在应用程序负载均衡器侦听器对话框中，配置以下设置：
对于端口，请键入传入流量端口，通常为443。
对于协议，请选择HTTPS。
对于SSL 证书，请选择您的证书。

选择Add。
注意
如果下拉菜单中未显示任何证书，您应在[AWS Certificate Manager (ACM)](http://docs.aws.amazon.com/acm/latest/userguide/)中创建或上传证书 (首选)，或者使用 AWS CLI 上传证书到 IAM。

选择Save，然后选择Apply。

## 使用配置文件配置安全侦听器
您可以使用以下[配置文件](https://docs.aws.amazon.com/zh_cn/elasticbeanstalk/latest/dg/ebextensions.html)之一在负载均衡器上配置安全侦听器。
**例 .ebextensions/securelistener-clb.config**
在您的环境具有传统负载均衡器时，请使用该示例。该示例使用
```python
aws:elb:listener
```
命名空间中的选项通过指定的证书在端口 443 上配置 HTTPS 侦听器，并将解密的流量转发到端口 80 上的环境实例。

```python
option_settings:
  aws:elb:listener:443:
    SSLCertificateId:
```
```python
arn:
```
```python
aws:
```
```python
acm:
```
```python
us-east-
```
```python
2
```
```python
:
```
```python
1234567890123
```
```python
:certificate/
```
```python
####################################
```
```python
ListenerProtocol: HTTPS
    InstancePort: 80
```

将突出显示的文本替换为您的证书的 ARN。证书可以是您在 AWS Certificate Manager (ACM) 中创建或上传的证书 (首选)，也可以是通过 AWS CLI 上传到 IAM 的证书。
有关传统负载均衡器配置选项的更多信息，请参阅[传统负载均衡器配置命名空间](https://docs.aws.amazon.com/zh_cn/elasticbeanstalk/latest/dg/environments-cfg-clb.html#environments-cfg-clb-namespace)。
**例 .ebextensions/securelistener-alb.config**
在您的环境具有应用程序负载均衡器时，请使用该示例。该示例使用
```python
aws:elbv2:listener
```
命名空间中的选项通过指定的证书在端口 443 上配置 HTTPS 侦听器。该侦听器将流量路由到默认进程。

```python
option_settings:
  aws:elbv2:listener:443:
    Protocol: HTTPS
    SSLCertificateArns:
```
```python
arn:
```
```python
aws:
```
```python
acm:
```
```python
us-east-
```
```python
2
```
```python
:
```
```python
1234567890123
```
```python
:certificate/
```
```python
####################################
```

## 配置安全组
如果将负载均衡器配置为将流量转发到除端口 80 外的实例端口，则必须向安全组添加一条规则来允许来自负载均衡器的入站流量通过该实例端口。在自定义 VPC 中创建环境时，Elastic Beanstalk 会为您添加此规则。
您可以在`.ebextensions`目录的[配置文件](https://docs.aws.amazon.com/zh_cn/elasticbeanstalk/latest/dg/ebextensions.html)中为您的应用程序添加
```python
Resources
```
键，从而添加此规则。
下面的示例配置文件向
```python
AWSEBSecurityGroup
```
安全组添加一条入口规则，该规则允许端口 1000 接收来自负载均衡器的安全组的流量。

