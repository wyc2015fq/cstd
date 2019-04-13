
# ASP.NET Core 在 Swagger UI 中显示自定义的 Header或者在url中增加query参数 - 追求卓越,做到专业 - CSDN博客


2018年06月15日 09:48:18[Waldenz](https://me.csdn.net/enter89)阅读数：2292


会遇到有些api接口中，会在实际调用过程中在url加参数，或者加Header。在Swagger中该如何实现
```python
using Swashbuckle.AspNetCore.Swagger;
 using Swashbuckle.AspNetCore.SwaggerGen;    
    /// 控制swagger中是否需要添加accesstoken验证
    /// </summary>
    public class AddAuthTokenHeaderParameter : IOperationFilter
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="operation"></param>
        /// <param name="context"></param>
        public void Apply(Operation operation, OperationFilterContext context)
        {
            if (operation.Parameters == null) operation.Parameters = new List<IParameter>();
            var attrs = context.ApiDescription.ActionDescriptor.AttributeRouteInfo;
            //先判断是否是匿名访问,
            var descriptor = context.ApiDescription.ActionDescriptor as ControllerActionDescriptor;
            if (descriptor != null)
            {
                var actionAttributes = descriptor.MethodInfo.GetCustomAttributes(inherit: true);
                bool isAnonymous = actionAttributes.Any(a => a is AllowAnonymousAttribute);
                 //非匿名的方法,链接中添加accesstoken值
                if (!isAnonymous)
                {
                    operation.Parameters.Add(new NonBodyParameter()
                    {
                        Name = "accesstoken",
                        In = "query",//query header body path formData
                        Type = "string",
                        Required = true //是否必选
                    });
                }
            }
        }
    }
```
其中NonBodyParameter类中的In属性可以为query,header,body,path,formData. 分别为在url查询参数、header、body、url路径、表单数据中添加项。[这里可参考官网解释](https://github.com/OAI/OpenAPI-Specification/blob/master/versions/2.0.md#parameter-object)
然后在StartUp的ConfigureServices方法中绑定
```python
public void ConfigureServices(IServiceCollection services)
        {
            services.AddMvc(options => { options.Filters.Add<AuthActionFilter>(); });
            services.AddSwaggerGen(options =>
            {
                options.SwaggerDoc("v1", new Info
                {
                    Version = "v1",
                    Title = "Test API"
                });
                //swagger中控制请求的时候发是否需要在url中增加accesstoken
                options.OperationFilter<AddAuthTokenHeaderParameter>();
                //Determine base path for the application.  
                var basePath = PlatformServices.Default.Application.ApplicationBasePath;
                //Set the comments path for the swagger json and ui.  
                var xmlPath = Path.Combine(basePath, "TestDemo.xml");
                options.IncludeXmlComments(xmlPath);
            });
        }
```
最终Swagger界面如下：
![](https://img-blog.csdn.net/20180615100058405?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



