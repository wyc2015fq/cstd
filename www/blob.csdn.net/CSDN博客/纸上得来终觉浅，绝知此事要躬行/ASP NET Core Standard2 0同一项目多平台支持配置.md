# ASP.NET Core Standard2.0同一项目多平台支持配置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年09月11日 17:43:07[boonya](https://me.csdn.net/boonya)阅读数：255








为了实现Core项目-netstandard2.0与.NetFramework4.5同在一个项目里面依赖，配置需要对环境的目标框架进行修改，并且代码在不兼容的地方需要预编译平台判断处理。大致情况可以参考下面：

## 什么是.NET Standard

参考：[https://github.com/dotnet/standard/blob/master/docs/versions.md](https://github.com/dotnet/standard/blob/master/docs/versions.md)

.NET Standard是一项API规范，每一个特定的版本，都定义了必须实现的基类库。

.NET Core是一个托管框架，针对构建控制台、云、ASP.NET Core和UWP应用程序进行了优化。每一种托管实现（如Xamarin、.NET Core或.NET Framework）都必须遵循.NET Standard实现BCL。

![](https://img-blog.csdn.net/20171013114306399)

## 类库编辑项目设置

项目右键：

![](https://img-blog.csdn.net/20180911174350228?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 类库项目配置

修改前：

```
<Project Sdk="Microsoft.NET.Sdk"> 
  <PropertyGroup>  
    <TargetFramework>netstandard2.0</TargetFramework>
 </PropertyGroup>
</Project>
```



修改后：

```
<PropertyGroup> 
  <TargetFrameworks>netstandard2.0;net45</TargetFrameworks> 
</PropertyGroup>
```

之后编辑类文件就可以看到两种平台的支持了：

![](https://img-blog.csdn.net/20180911174620849?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 依赖包配置

两种平台都支持：

```
<!--通用平台配置满足netstandard2.0和net45-->
  <ItemGroup>
    <PackageReference Include="log4net" Version="2.0.8" />
    <PackageReference Include="Newtonsoft.Json" Version="11.0.2" />   
  </ItemGroup>
```

指定平台依赖：

```
<!--平台配置满足netstandard2.0-->
  <ItemGroup  Condition="'$(TargetFramework)' == 'netstandard2.0'">
    <PackageReference Include="System.Drawing.Common" Version="4.5.0" />
    <PackageReference Include="System.Text.Encoding.CodePages" Version="4.5.0" />
  </ItemGroup>
```

## 不同平台代码处理

配置平台变量：

```
<PropertyGroup Condition="'$(TargetFramework)' =='net45' ">
    <DefineConstants>NET45</DefineConstants>
  </PropertyGroup>

  <PropertyGroup Condition="'$(TargetFramework)' =='netstandard2.0' ">
    <DefineConstants>NETSTANDARD20</DefineConstants>
  </PropertyGroup>
```

代码里面我们可以这样来判断：

```
#if NET45
        public int GetPlatform()
        {
            return "net45";
        }
#else  
        public int GetPlatform()
        {
            return "netstandard2.0";
        }  
#endif
```

或：

```
#if NETSTANDARD20
        public int GetPlatform()
        {
            return "netstandard2.0";
        }
#else  
        public int GetPlatform()
        {
            return "net45";
        }  
#endif
```

参考地址：[http://www.mamicode.com/info-detail-2327644.html](http://www.mamicode.com/info-detail-2327644.html)

注意：跨平台的项目一般是类库性质的，窗体应用或其他应用是可以根据平台来引用类库的，一个类库可能包含多个平台的代码， 也就是所谓的多平台支持项目。



