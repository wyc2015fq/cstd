# C#通过RFC连接sap系统 - _天枢 - 博客园
## [C#通过RFC连接sap系统](https://www.cnblogs.com/yhleng/p/6731820.html)
2017-04-19 09:50 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=6731820)
先理解一下
RFC(Romote Function Call)远程函数调用
调用前提：
1.要想通过C# 通过RFC调用SAP端，SAP端要存在RFC远程调用的函数才行（例如SAP端通过SE37创建），要不然是无法调用的。
2.C#调用RFC要有NCO DLL支持（我们使用NCO3.0，VS2013，framework2.0才行否则会报错)
注：好多人64位系统，开发的时候报错，到处找支持64位的NCO3.0，这里可以说一下，是木有的，报错是困为Framework的原因。NCO3.0只支持2.0，在开发环境中改一下就好。
开发背景：
因为要做SAP外围系统IPM接口测试，这个是个异步接口
调用接口之后，返回结果要通过另外一支接口去获得，由于某些原因不好实现，所以采取别一种方法。
直接通过RFC在sap端查接口调用日志表ZIMPSTXS00330（手工查询方法，sap端 se11事务，输入表名，找到MESSAGE字段S代表接口成功，E代码失败）
——————下面直接上代码，其实这个不难——————
引用NCO3.0后
![](https://images2015.cnblogs.com/blog/1149221/201704/1149221-20170419094237915-349469931.png)
```
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
//using System.Linq;
using System.Text;
using System.Windows.Forms;
using SAP.Middleware.Connector;
namespace WindowsFormsApplication7
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }
        private void button1_Click(object sender, EventArgs e)
        {
            //rfc配置
            RfcConfigParameters argsP = new RfcConfigParameters();
            argsP.Add(RfcConfigParameters.Name, "QS7");
            argsP.Add(RfcConfigParameters.AppServerHost, "10.5.91.131");
            argsP.Add(RfcConfigParameters.SystemNumber, "00");
            // argsP.Add(RfcConfigParameters.SystemID, "QS7");
            argsP.Add(RfcConfigParameters.User, "ATPSUSER");
            argsP.Add(RfcConfigParameters.Password, "q123456");
            argsP.Add(RfcConfigParameters.Client, "800");
            argsP.Add(RfcConfigParameters.Language, "zh");
            argsP.Add(RfcConfigParameters.PoolSize, "5");
            argsP.Add(RfcConfigParameters.MaxPoolSize, "10");
            argsP.Add(RfcConfigParameters.IdleTimeout, "60");
            //获取rfc配置
            RfcDestination sapConfig = RfcDestinationManager.GetDestination(argsP); //NCO3.0如果framework不是2.0此处会报错，跟系统64还32无关
            RfcRepository rfcRepository = sapConfig.Repository;
           
            //调用
            IRfcFunction invoke = rfcRepository.CreateFunction("SE11_FUNC"); //调用函数名 ZRFC_MARA_INFO
            invoke.SetValue("PSPID", ""); //设置参数 项目编号
            invoke.Invoke(sapConfig); //执行函数
            IRfcTable rfcTable = invoke.GetTable("ZIMPSTXS00330"); //获取内表
            string message = rfcTable.GetValue("MESSAGE").ToString();
            MessageBox.Show(message);
        }
    }
}
```
一些传的参数，是结构，就不能用setValue来传参了，要先给结构值
SetStructValue是我封装好的，给结构值的方法，参数结果名字，结构传真字符串，model**fc_invoke.SetValue(0, BAPIACHE09IMPORT)   把结构赋值给，函数对象，索引根据，参数位置决定。**
```
IRfcFunction rfc_invoke = configModel.rfcRepository.CreateFunction(configModel.IFUNC);
                    var BAPIACHE09IMPORT = SetStructValue("BAPIACHE09", BAPIACHE09Arr, configModel);
                    var BAPIACPA09IMPORT = SetStructValue("BAPIACPA09", BAPIACPA09Arr, configModel);
                    var BAPIACCAHDIMPORT = SetStructValue("BAPIACCAHD", BAPIACCAHDArr, configModel);
                    rfc_invoke.SetValue(0, BAPIACHE09IMPORT);
                    rfc_invoke.SetValue(1, BAPIACPA09IMPORT);
                    rfc_invoke.SetValue(1, BAPIACCAHDIMPORT);
                    IRfcTable rfcTable = rfc_invoke.GetTable("RETURN"); //获取内表
                    string message = rfcTable.GetValue("MESSAGE").ToString();
                    return message;
```
封装好的baseClass.cs
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
///sapnco.dll
///sapnco_utils
///**************************************************************///
namespace RC_RFC_LIB
{
    /// <summary>
    /// RFC接口调用
    /// </summary>
    public  class BaseClass
    {
        
        /// <summary>
        /// 登录配置
        /// </summary>
        /// <returns>bool</returns>
        public bool rfc_config(Model.baseConfigModel configModel)
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
                //argsP.Add(RfcConfigParameters.AbapDebug, "DEBUG");
                
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
        #region //item传参
        /// <summary>
        /// 循环设置函数参数，不包含结构
        /// </summary>
        /// <param name="rfc_invoke">函数对象</param>
        /// <param name="itemArr">参数数组“name@_@value”</param>
        public void SetItemParaValue(IRfcFunction rfc_invoke ,string[] itemArr)
        {
            for (var i = 0; i < itemArr.Length; i++)
            {
                var tmpitVal = itemArr[i].Split(new string[] { "@_@" }, StringSplitOptions.RemoveEmptyEntries);
                rfc_invoke.SetValue(tmpitVal[0], tmpitVal[1]); //设置参数 (参数名，参数值)
            }
        }
        #endregion
        #region Struct类型 字段赋值
        /// <summary>
        /// Struct类型 字段赋值
        /// </summary>
        /// <param name="structName">ZAPWZ035 结构字段名</param>
        /// <param name="structFieldArr">结构内字段</param>
        /// <param name="configModel">配置model</param>
        /// <returns>import结构，需要当参数用</returns>
        public IRfcStructure SetStructValue(string structName, string[] structFieldArr,Model.baseConfigModel configModel)
        {
            try
            {
                IRfcStructure import = null;
                //IRfcTable table = rfc_invoke.GetTable("IS_ZAPWZ035");
                //ZAPWZ035
                //rfc_invoke.SetValue(0,import);
                import = configModel.rfcRepository.GetStructureMetadata(structName.Trim().ToUpper()).CreateStructure();
                for (var i = 0; i < structFieldArr.Length; i++)
                {
                    var tmptableVal = structFieldArr[i].Split(new string[] { "@_@" }, StringSplitOptions.None);
                    import.SetValue(tmptableVal[0], tmptableVal[1]); //设置参数 (参数名，参数值)
                }
                return import;
            }
            catch (Exception ex)
            {
                return null;
            }
        }
        #endregion
        #region property赋值
        /// <summary>
        /// property赋值
        /// </summary>
        /// <param name="arrObj">array object</param>
        /// <param name="modelObj">model object instance</param>
        public void SetPropertyValue(string[] arrObj, object modelObj)
        {
            var modelInstance = modelObj.GetType();
            for (var i = 0; i < arrObj.Length; i++)
            {
                var tmpArr = arrObj[i].Split(new string[] { "@_@" }, StringSplitOptions.None);
                foreach (System.Reflection.PropertyInfo py in modelInstance.GetProperties())
                {
                    if (py.Name.ToLower() == tmpArr[0].ToLower())
                    {
                        modelInstance.GetProperty(py.Name).SetValue(modelObj, tmpArr[1]);
                    }
                }
            }
        }
        #endregion
    }
}
```
baseConfigmodel.cs
```
using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
//using System.Threading.Tasks;
using SAP.Middleware.Connector;
namespace RC_RFC_LIB.Model
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
