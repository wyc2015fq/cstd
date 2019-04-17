# DUBBO admin服务端和前端UI项目实践 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年03月05日 13:18:00[boonya](https://me.csdn.net/boonya)阅读数：175








**目录**

[项目资源](#%E9%A1%B9%E7%9B%AE%E8%B5%84%E6%BA%90)

[启动Zookeeper](#%E5%90%AF%E5%8A%A8Zookeeper)

[启动dubbo-admin-server](#%E5%90%AF%E5%8A%A8dubbo-admin-server)

[启动dubbo-admin-ui](#%C2%A0%E5%90%AF%E5%8A%A8dubbo-admin-ui)

[前后端访问](#%E5%89%8D%E5%90%8E%E7%AB%AF%E8%AE%BF%E9%97%AE)

[SwaggerAPI](#SwaggerAPI)

## 项目资源

项目地址：[https://github.com/apache/incubator-dubbo-ops](https://github.com/apache/incubator-dubbo-ops)

![](https://img-blog.csdnimg.cn/20190305131244577.png)

## 启动Zookeeper

注意Zookeeper端口是2181.

![](https://img-blog.csdnimg.cn/2019030513135833.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

## 启动dubbo-admin-server

执行如下代码：

```java
/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.dubbo.admin;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.jdbc.DataSourceAutoConfiguration;
import org.springframework.boot.autoconfigure.orm.jpa.HibernateJpaAutoConfiguration;

@SpringBootApplication(exclude={
		DataSourceAutoConfiguration.class, HibernateJpaAutoConfiguration.class
})
public class DubboAdminApplication {

	public static void main(String[] args) {
		SpringApplication.run(DubboAdminApplication.class, args);
	}
}
```

![](https://img-blog.csdnimg.cn/2019030513111297.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

##  启动dubbo-admin-ui

启动过程中报错可以不用关注，是mac上编写的问题。

![](https://img-blog.csdnimg.cn/20190305131504809.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

## 前后端访问

后端端口是8080，前端端口是8081.

![](https://img-blog.csdnimg.cn/2019030513163295.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

## SwaggerAPI

访问：[http://localhost:8080/swagger-ui.html](http://localhost:8080/swagger-ui.html)

![](https://img-blog.csdnimg.cn/20190305132114399.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)



