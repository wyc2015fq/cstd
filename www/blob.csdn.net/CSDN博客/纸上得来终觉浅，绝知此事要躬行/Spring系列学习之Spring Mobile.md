# Spring系列学习之Spring Mobile - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 22:09:17[boonya](https://me.csdn.net/boonya)阅读数：359








英文原文：[https://projects.spring.io/spring-mobile/](https://projects.spring.io/spring-mobile/)

**目录**

[特性](#%E7%89%B9%E6%80%A7)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[设备检测](#%E8%AE%BE%E5%A4%87%E6%A3%80%E6%B5%8B)

[网站偏好](#%E7%BD%91%E7%AB%99%E5%81%8F%E5%A5%BD)

[设备感知视图分辨率](#%E8%AE%BE%E5%A4%87%E6%84%9F%E7%9F%A5%E8%A7%86%E5%9B%BE%E5%88%86%E8%BE%A8%E7%8E%87)

[Sample Projects](#sample-projects)

[Getting Started Guides](#getting-started-guides)

Spring Mobile是Spring MVC的扩展，旨在简化移动Web应用程序的开发。

Spring Mobile是一个框架，它提供了检测向Spring网站发出请求的设备类型以及基于该设备提供备用视图的功能。 像所有Spring项目一样，Spring Mobile的真正强大之处在于它可以轻松扩展。

## 特性
-      用于移动设备和平板电脑设备的服务器端检测的设备解析器抽象
-      网站偏好管理，允许用户指示他或她是否更喜欢“正常-normal”，“移动-mobile”或“平板电脑-tablet”体验
-      一种站点切换器，能够根据用户的设备和可选的站点偏好，将用户切换到最合适的站点（移动设备，平板电脑或普通站点）
-      设备感知视图管理，用于组织和管理特定设备的不同视图

## 快速开始

在项目中使用spring-mobile的推荐方法是使用依赖管理系统 - 下面的代码片段可以复制并粘贴到您的构建中。 需要帮忙？ 请参阅我们的[Maven](https://spring.io/guides/gs/maven/)和[Gradle](https://spring.io/guides/gs/gradle/)构建入门指南。

### 设备检测

添加以下基于Java的配置以在Spring Web应用程序中启用设备检测：

```java
@Bean
public DeviceResolverHandlerInterceptor 
        deviceResolverHandlerInterceptor() {
    return new DeviceResolverHandlerInterceptor();
}

@Bean
public DeviceHandlerMethodArgumentResolver 
        deviceHandlerMethodArgumentResolver() {
    return new DeviceHandlerMethodArgumentResolver();
}

@Override
public void addInterceptors(InterceptorRegistry registry) {
    registry.addInterceptor(deviceResolverHandlerInterceptor());
}

@Override
public void addArgumentResolvers(
        List<HandlerMethodArgumentResolver> argumentResolvers) {
    argumentResolvers.add(deviceHandlerMethodArgumentResolver());
}
```

将设备注入控制器：

```java
@Controller
public class HomeController {

    private static final Logger logger = 
            LoggerFactory.getLogger(HomeController.class);

    @RequestMapping("/")
    public void home(Device device) {
        if (device.isMobile()) {
            logger.info("Hello mobile user!");
        } else if (device.isTablet()) {
            logger.info("Hello tablet user!");
        } else {
            logger.info("Hello desktop user!");         
        }
    }

}
```

### 网站偏好

添加以下基于Java的配置以在Spring Web应用程序中启用站点首选项管理：

```java
@Bean
public DeviceResolverHandlerInterceptor 
        deviceResolverHandlerInterceptor() {
    return new DeviceResolverHandlerInterceptor();
}

@Bean
public SitePreferenceHandlerInterceptor 
        sitePreferenceHandlerInterceptor() {
    return new SitePreferenceHandlerInterceptor();
}

@Bean
public SitePreferenceHandlerMethodArgumentResolver 
        sitePreferenceHandlerMethodArgumentResolver() {
    return new SitePreferenceHandlerMethodArgumentResolver();
}

@Override
public void addInterceptors(InterceptorRegistry registry) {
    registry.addInterceptor(deviceResolverHandlerInterceptor());
    registry.addInterceptor(sitePreferenceHandlerInterceptor());
}
@Override
public void addArgumentResolvers(
        List<HandlerMethodArgumentResolver> argumentResolvers) {
    argumentResolvers.add(sitePreferenceHandlerMethodArgumentResolver());
}
```

将SitePreference注入您的控制器：

```java
@Controller
public class HomeController {

    private static final Logger logger = 
            LoggerFactory.getLogger(HomeController.class);

    @RequestMapping("/")
    public String home(SitePreference sitePreference, Model model) {
        if (sitePreference == SitePreference.NORMAL) {
            logger.info("Site preference is normal");
            return "home";
        } else if (sitePreference == SitePreference.MOBILE) {
            logger.info("Site preference is mobile");
            return "home-mobile";
        } else if (sitePreference == SitePreference.TABLET) {
            logger.info("Site preference is tablet");
            return "home-tablet";
        } else {
            logger.info("no site preference");
            return "home";
        }
    }

}
```

### 设备感知视图分辨率

添加以下基于Java的配置以在Spring Web应用程序中启用设备感知视图。 这指定Spring Mobile将在/WEB-INF/views/tablet/中的/ WEB-INF/views/mobile/和平板电脑视图中查找移动视图。 

```java
@Bean
public LiteDeviceDelegatingViewResolver liteDeviceAwareViewResolver() {
    InternalResourceViewResolver delegate = 
            new InternalResourceViewResolver();
    delegate.setPrefix("/WEB-INF/views/");
    delegate.setSuffix(".jsp");
    LiteDeviceDelegatingViewResolver resolver = 
            new LiteDeviceDelegatingViewResolver(delegate);
    resolver.setMobilePrefix("mobile/");
    resolver.setTabletPrefix("tablet/");
    return resolver;
}
```

### Sample Projects
- [Spring Mobile Samples](https://github.com/SpringSource/spring-mobile-samples)

### Getting Started Guides
- [Detecting a Device](https://spring.io/guides/gs/device-detection)
- [Handling Web Site Preference](https://spring.io/guides/gs/handling-site-preference)
- [Serving Mobile Web Content with Spring MVC](https://spring.io/guides/gs/serving-mobile-web-content)



