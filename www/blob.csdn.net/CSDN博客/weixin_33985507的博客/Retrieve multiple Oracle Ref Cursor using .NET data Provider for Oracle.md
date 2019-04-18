# Retrieve multiple Oracle Ref Cursor using .NET data Provider for Oracle - weixin_33985507的博客 - CSDN博客
2004年06月05日 19:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
##### Introduction
In my earlier article [Multiple Result Sets in ADO.net using SqlClient](http://www.code101.com/Code101/DisplayArticle.aspx?cid=15),we have seen how to retrieve multiple results using Sqlclient against SQL Server. This was a fairly simple and straight forward. We can achieve the same against Oracle database with a slight difference. We need to iron out couple of things before we get to nuts and bolts of it.
- We need **System.Data.OracleClient.Dll** assembly that is very similar to System.Data.SqlClient.Dll,.NET Managed Provider forOracle is not a part of .NET Version 1.0, a separate download [click here](http://msdn.microsoft.com/downloads/default.asp?URL=/downloads/sample.asp?url=/MSDN-FILES/027/001/940/msdncompositedoc.xml)to download . For .NET version 1.1, it’s going to be part of the bundle. If you are new to .Net framework provider for Oracle read my articles on code101 [Boost performance with .net data provider for oracle](http://www.code101.com/Code101/DisplayArticle.aspx?cid=21)
- We need a stored procedure to return multiple results set using REF CURSOR. For novice, A ref cursor is a PL/SQL data type that you can use in a query to fetch data. Each ref cursor query is associated with a PL/SQL function that returns a strongly typed ref cursor. 
##### Lets get feet dirty 
Create a oracle package, I assume you know what a package by the way it has two parts Specification and body 
##### Package Specification
CREATE OR REPLACE PACKAGE PKG_MUltiResultset as
 TYPE MyRefCur is REF CURSOR;
 procedure GetReadOnlyData(EmpCur OUT MyRefCur,
			  DeptCur OUT MyRefCur,
			  SalCur OUT MyRefCur);
END;
##### Package body 
CREATE OR REPLACE PACKAGE BODY PKG_MUltiResultset as
  PROCEDURE GetReadOnlyData(EmpCur OUT MyRefCur,
	   		   DeptCur OUT MyRefCur,
			   SalCur OUT MyRefCur)
  IS
  BEGIN 
    open EmpCur for select * from emp;
    open DeptCur for select * from dept;
    open SalCur for select * from salgrade;
  END;
 END;Lets us get to the client part to access this stored procedure. We will establish connection to Oracle database retrieve data from three tables in one shot and fill up the dataset and bind the data to DataGrids.I am using Dataset you can try it with OracleDataReader as well.
**NOTE:** The Prefix is always Oracle for all the classes in Syatem.Data.OracleClient name space. Reference name spaces using System.Data.OracleClient;
using System.Data; Private void GetData_Click(object sender, System.EventArgs e)
  {
    try
      {
	//Connect to Database
	OracleConnection oCon=new OracleConnection
        ("Data source=Firstdb;user id=scott;password=tiger");
	 oCon.Open();
	
        //Command Object
	OracleCommand oCmd =new OracleCommand
        ("PKG_MUltiResultset.GetReadOnlyData",oCon);
	//Stored Procedure
	oCmd.CommandType=CommandType.StoredProcedure;
	//Create Parameter Object
        oCmd.Parameters.Add(new OracleParameter
        ("empcur",OracleType.Cursor)).Direction=ParameterDirection.Output;
     
        oCmd.Parameters.Add(new OracleParameter
        ("DeptCur",OracleType.Cursor)).Direction=ParameterDirection.Output;
     
        oCmd.Parameters.Add(new OracleParameter
       ("SalCur",OracleType.Cursor)).Direction=ParameterDirection.Output;
	
	//Instatiate Dataset
	DataSet Ds=new DataSet();
	//Instatiate Data Adopter
	OracleDataAdapter oAdp=new OracleDataAdapter(oCmd);
	//Fill Data Set
	oAdp.Fill(Ds);
	//Bind Data to Grids
	dataGrid1.SetDataBinding(Ds,"Table");
	dataGrid2.SetDataBinding(Ds,"Table1");
	dataGrid3.SetDataBinding(Ds,"Table2");
	}
  	 catch (Exception x)
	{
	 MessageBox.Show(x.Message);
	}
      }
