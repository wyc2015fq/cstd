# uft调用rfc接口 - _天枢 - 博客园
## [uft调用rfc接口](https://www.cnblogs.com/yhleng/p/7262835.html)
2017-07-31 14:08 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7262835)
RFC接口函数调用：
以下代码是封装好的，为了提供给UFT工具调用，使用c#写成dll。
类型项目分成两个：
1.baseConfigModel.cs  //sap配置登录信息，属性实体类
```
using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
//using System.Threading.Tasks;
using SAP.Middleware.Connector;
namespace I_RFC_LIB.Model
{
    public class baseConfigModel
    {
        public string Name { get; set; } //系统名字QH5
        public string AppServerHost { get; set; }//系统ip地址
        public string SystemNumber { get; set; } //系统实例
        public string SystemID { get; set; }//系统id
        public string User { get; set; }//用户ATPSUSER
        public string Password { get; set; }//密码
        public string Client { get; set; }//客户端号800
        public string Language { get; set; } //语言zh en
        public string IFUNC { get; set; } //接口函数名
        public RfcDestination sapConfig { get; set; } //sap登录配置
        public RfcRepository rfcRepository { get; set; } //Repository对象
        public string resultConfig { get; set; } //配置结果
    }
}
```
2.C_RFC.cs //具体实现
```
using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using System.Reflection;
//using System.Threading.Tasks;
///**************************************************************///
using SAP.Middleware.Connector;
///引用合名空间SAP.Middleware.Connector需要引用sapnco3.0 dll
///需要framework2.0支持
///sapnco.dll
///sapnco_utils
///**************************************************************///
namespace I_RFC_LIB
{
    public class C_RFC
    {
        Model.baseConfigModel   configModel = new Model.baseConfigModel(); //实例类  登录配置属性
        /// <summary>
        /// 调用rfc接口函数
        /// </summary>
        /// <param name="headstr">包含配置信息的头</param>
        /// <param name="itemstr">接口需要参数</param>
        /// <returns></returns>
        public string invoke(string headstr,string itemstr)
        {
            string[] headArr = headstr.Split(new string[] { "@__@" }, StringSplitOptions.RemoveEmptyEntries);
            string[] itemArr = itemstr.Split(new string[] { "@__@" }, StringSplitOptions.RemoveEmptyEntries);
            #region //head属性赋值
            //rfc config
            var proValue = configModel.GetType(); //获取属性实例
            for (var i = 0; i < headArr.Length; i++)
            {
                var tmpVal = headArr[i].Split(new string[] { "@_@" }, StringSplitOptions.None);
                foreach (PropertyInfo py in proValue.GetProperties())
                {
                    if (py.Name.ToUpper() == tmpVal[0].ToUpper())
                    {
                        proValue.GetProperty(py.Name).SetValue(configModel, tmpVal[1], null);
                    }
                }
            }
            #endregion
            if (rfc_config()) //执行配置，获取对象
            {
                IRfcFunction rfc_invoke = configModel.rfcRepository.CreateFunction(configModel.IFUNC); //调用"ZFUC_APWZ_AP_PREVIEW"
                rfc_invoke.Invoke(configModel.sapConfig); //执行函数
                #region //item传参
                for (var i = 0; i < itemArr.Length; i++)
                {
                    var tmpitVal = itemArr[i].Split(new string[] { "@_@" }, StringSplitOptions.RemoveEmptyEntries);
                    rfc_invoke.SetValue(tmpitVal[0], tmpitVal[1]); //设置参数 (参数名，参数值)
                }
                #endregion
                //rfc_invoke.SetValue("PSPID", ""); //设置参数 项目编号
                IRfcTable rfcTable = rfc_invoke.GetTable("RETURN"); //获取内表
                string message = rfcTable.GetValue("MESSAGE").ToString();
                return message;
            }
            else
            {
                return "配置登录时出现问题，请查检配置！"+configModel.resultConfig;
            }
        }
        /// <summary>
        /// 登录配置
        /// </summary>
        /// <returns>bool</returns>
        public bool rfc_config()
        {
            try
            {
                //rfc配置
                RfcConfigParameters argsP = new RfcConfigParameters();
                argsP.Add(RfcConfigParameters.Name, configModel.Name);
                argsP.Add(RfcConfigParameters.AppServerHost, configModel.AppServerHost);
                argsP.Add(RfcConfigParameters.SystemNumber, configModel.SystemNumber);
                argsP.Add(RfcConfigParameters.SystemID, configModel.SystemID);
                argsP.Add(RfcConfigParameters.User, configModel.User);
                argsP.Add(RfcConfigParameters.Password, configModel.Password);
                argsP.Add(RfcConfigParameters.Client, configModel.Client);
                argsP.Add(RfcConfigParameters.Language, configModel.Language);
                argsP.Add(RfcConfigParameters.PoolSize, "5");
                argsP.Add(RfcConfigParameters.MaxPoolSize, "10");
                argsP.Add(RfcConfigParameters.IdleTimeout, "60");
                //获取rfc配置
                configModel.sapConfig = RfcDestinationManager.GetDestination(argsP);
                configModel.rfcRepository = configModel.sapConfig.Repository;
            }
            catch (RfcBaseException ex)
            {
                configModel.resultConfig = ex.Message;
                return false;
            }
            return true;
        }
    }
}
```
3.uft调用
需要设置，程序集com可见
![](https://images2017.cnblogs.com/blog/1149221/201707/1149221-20170731135854240-1624404098.png)
 4、uft调用方法：
set rfc = dotnetfactory.CreateInstance("I_RFC_LIB.C_RFC","d:\\rfc.dll")
5、c#类库调用结果。接口返回“ATPSUSER不是该步骤允许的审批人”
![](https://images2017.cnblogs.com/blog/1149221/201707/1149221-20170731140637021-676488712.png)
