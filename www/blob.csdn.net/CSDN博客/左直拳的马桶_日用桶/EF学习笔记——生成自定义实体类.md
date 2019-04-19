# EF学习笔记——生成自定义实体类 - 左直拳的马桶_日用桶 - CSDN博客
2014年05月03日 00:27:30[左直拳](https://me.csdn.net/leftfist)阅读数：15453
使用EF，采用DataBase 模式，实体类都是按照数据库的定义自动生成，我们似乎无法干预。如果要生成自定义的实体类，该怎么做呢？
思路是这样的：
1、我们要自定义生成的实体类，都是分部类（partial)，目的是对EF生成的实体类进行扩充；
2、扩充部分，预先写好在模板里，自动生成
3、每个实体类，都进行扩充
实施方法：
1、给VS2012安装两个插件：
- Devart T4 Editor：为VS提供智能提示功能。
- T4 Toolbox：在生成多文件时很有用。
2、新建文件夹T4，存放模板文件
1）创建T4 Toolbox模板文件EntityTemplate.tt，作用是生成多个实体类文件
![](https://img-blog.csdn.net/20140502235517781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
代码如下：
```
<#+
// <copyright file="Entity.tt" company="cqxm">
//  Copyright © . All Rights Reserved.
// </copyright>
public class Entity : CSharpTemplate
{
    private string _className;
 
    public Entity(string className)
    {
        _className = className;
    }
	public override string TransformText()
	{
		base.TransformText();
#>
namespace testEF
{
	public partial class <#= _className #> : IEntity
	{
		public string _ID 
        {
            get
            {
                return Id.ToString();
            }
        }
	}
}
<#+
        return this.GenerationEnvironment.ToString();
	}
}
#>
```
2）创建T4模板文件EntityOutput.tt，调用前面写的EntityTemplate.tt，真正生成各种实体类文件
![](https://img-blog.csdn.net/20140503000449890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
<#@ template debug="false" hostspecific="false" language="C#" #>
<#@ assembly name="System.Core" #>
<#@ import namespace="System.IO" #>
<#@ import namespace="System.Linq" #>
<#@ import namespace="System.Text" #>
<#@ import namespace="System.Collections.Generic" #>
<#@ include file="EF.Utility.CS.ttinclude"#>
<#@ include file="T4Toolbox.tt" #>
<#@ include file="EntityTemplate.tt" #>
<#
    string curPath = Path.GetDirectoryName(Host.TemplateFile);
    string destPath = Path.Combine(curPath, @"..\Partial");//将各个扩充实体类文件生成到文件夹Partial下
    if(!Directory.Exists(destPath))
    {
        Directory.CreateDirectory(destPath);
    }
    const string inputFile = @"..\ShowMe.edmx";
    var textTransform = DynamicTextTransformation.Create(this);
    var itemCollection = new EdmMetadataLoader(textTransform.Host, textTransform.Errors).CreateEdmItemCollection(inputFile);
    foreach (var entity in GetItemsToGenerate<EntityType>(itemCollection))
    {//每个实体类都要自动生成扩充文件，是自动哦，参考文章里介绍的是手动输入实体类名称
        Entity template = new Entity(entity.Name);
        string fileName = string.Format(@"{0}\{1}.cs", destPath, entity.Name);
        template.Output.Encoding = Encoding.UTF8;
        if(File.Exists(fileName))
        {
            File.Delete(fileName);
        }
        template.RenderToFile(fileName);
    }
#>
<#+
/*以下代码都是为了获得EF自动生成的所有实体类名单，抄自 *.edmx\*.Context.tt。
没办法，谁叫俺看不懂哪些代码呢
*/
public class EdmMetadataLoader
{
    private readonly IDynamicHost _host;
    private readonly System.Collections.IList _errors;
    public EdmMetadataLoader(IDynamicHost host, System.Collections.IList errors)
    {
        ArgumentNotNull(host, "host");
        ArgumentNotNull(errors, "errors");
        _host = host;
        _errors = errors;
    }
    public IEnumerable<GlobalItem> CreateEdmItemCollection(string sourcePath)
    {
        ArgumentNotNull(sourcePath, "sourcePath");
        if (!ValidateInputPath(sourcePath))
        {
            return new EdmItemCollection();
        }
        var schemaElement = LoadRootElement(_host.ResolvePath(sourcePath));
        if (schemaElement != null)
        {
            using (var reader = schemaElement.CreateReader())
            {
                IList<EdmSchemaError> errors;
                var itemCollection = MetadataItemCollectionFactory.CreateEdmItemCollection(new[] { reader }, out errors);
                ProcessErrors(errors, sourcePath);
                return itemCollection;
            }
        }
        return new EdmItemCollection();
    }
    public string GetModelNamespace(string sourcePath)
    {
        ArgumentNotNull(sourcePath, "sourcePath");
        if (!ValidateInputPath(sourcePath))
        {
            return string.Empty;
        }
        var model = LoadRootElement(_host.ResolvePath(sourcePath));
        if (model == null)
        {
            return string.Empty;
        }
        var attribute = model.Attribute("Namespace");
        return attribute != null ? attribute.Value : "";
    }
    private bool ValidateInputPath(string sourcePath)
    {
        if (sourcePath == "$" + "edmxInputFile" + "$")
        {
            _errors.Add(
                new CompilerError(_host.TemplateFile ?? sourcePath, 0, 0, string.Empty,
                    GetResourceString("Template_ReplaceVsItemTemplateToken")));
            return false;
        }
        return true;
    }
    public XElement LoadRootElement(string sourcePath)
    {
        ArgumentNotNull(sourcePath, "sourcePath");
        var root = XElement.Load(sourcePath, LoadOptions.SetBaseUri | LoadOptions.SetLineInfo);
        return root.Elements()
            .Where(e => e.Name.LocalName == "Runtime")
            .Elements()
            .Where(e => e.Name.LocalName == "ConceptualModels")
            .Elements()
            .Where(e => e.Name.LocalName == "Schema")
            .FirstOrDefault()
                ?? root;
    }
    private void ProcessErrors(IEnumerable<EdmSchemaError> errors, string sourceFilePath)
    {
        foreach (var error in errors)
        {
            _errors.Add(
                new CompilerError(
                    error.SchemaLocation ?? sourceFilePath,
                    error.Line,
                    error.Column,
                    error.ErrorCode.ToString(CultureInfo.InvariantCulture),
                    error.Message)
                {
                    IsWarning = error.Severity == EdmSchemaErrorSeverity.Warning
                });
        }
    }
    
    public bool IsLazyLoadingEnabled(EntityContainer container)
    {
        string lazyLoadingAttributeValue;
        var lazyLoadingAttributeName = MetadataConstants.EDM_ANNOTATION_09_02 + ":LazyLoadingEnabled";
        bool isLazyLoading;
        return !MetadataTools.TryGetStringMetadataPropertySetting(container, lazyLoadingAttributeName, out lazyLoadingAttributeValue)
            || !bool.TryParse(lazyLoadingAttributeValue, out isLazyLoading)
            || isLazyLoading;
    }
}
public static void ArgumentNotNull<T>(T arg, string name) where T : class
{
    if (arg == null)
    {
        throw new ArgumentNullException(name);
    }
}
    
private static readonly Lazy<System.Resources.ResourceManager> ResourceManager =
    new Lazy<System.Resources.ResourceManager>(
        () => new System.Resources.ResourceManager("System.Data.Entity.Design", typeof(MetadataItemCollectionFactory).Assembly), isThreadSafe: true);
public static string GetResourceString(string resourceName)
{
    ArgumentNotNull(resourceName, "resourceName");
    return ResourceManager.Value.GetString(resourceName, null);
}
private const string ExternalTypeNameAttributeName = @"http://schemas.microsoft.com/ado/2006/04/codegeneration:ExternalTypeName";
public IEnumerable<T> GetItemsToGenerate<T>(IEnumerable<GlobalItem> itemCollection) where T: EdmType
{
    return itemCollection
        .OfType<T>()
        .Where(i => !i.MetadataProperties.Any(p => p.Name == ExternalTypeNameAttributeName))
        .OrderBy(i => i.Name);
}
#>
```
生成各种实体类文件的方法很简单，只要保存一下这个EntityOutput.tt文件，各种实体类文件即可应声生成，很爽。
最后生成的文件如蓝线圈圈所示
![](https://img-blog.csdn.net/20140503002701328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
某个实体扩充类：
```
// <autogenerated>
//   This file was generated by T4 code generator EntityOutput.tt.
//   Any changes made to this file manually will be lost next time the file is regenerated.
// </autogenerated>
namespace testEF
{
	public partial class Show : IEntity
	{
		public string _ID 
        {
            get
            {
                return Id.ToString();
            }
        }
	}
}
```
这个模板文件看起来好复杂，事实上，绝大部分都是自动生成，和拷贝过来的，我自己其实并不懂。
参考文章：
# [MVC实用架构设计（三）——EF-Code First（3）：使用T4模板生成相似代码](http://www.cnblogs.com/guomingfeng/p/mvc-ef-t4.html)
http://www.cnblogs.com/guomingfeng/p/mvc-ef-t4.html
# [EF架构~终于实现了Update方法的统一](http://www.cnblogs.com/lori/archive/2011/07/25/2115982.html)
http://www.cnblogs.com/lori/archive/2011/07/25/2115982.html
