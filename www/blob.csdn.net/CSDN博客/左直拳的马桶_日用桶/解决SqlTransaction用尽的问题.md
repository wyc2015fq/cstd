# 解决SqlTransaction用尽的问题 - 左直拳的马桶_日用桶 - CSDN博客
2005年10月07日 22:31:00[左直拳](https://me.csdn.net/leftfist)阅读数：2460
解决SqlTransaction用尽的问题
有时候程序处理的数据量比较小时，四平八稳，一切安然无恙，但数据量一大，原先潜伏的问题就暴露无遗了。
我做的一个项目，是负责一个厂的考勤的。厂里有员工1000多号人。按每人每天打4次卡，一个月30天，则产生的考勤记录数目为1000 * 4 * 30 = 120,000条。在处理这些记录时，我采用的办法是先生成SQL语句，然后执行这些SQL语句：
SqlConnection conn = new SqlConnection(strConn);

conn.Open();

SqlTransaction trans = conn.BeginTransaction();

try

{

CEngine.ExecuteNonQuery(trans, CommandType.Text, sql);

trans.Commit();

}

catch(SqlException ex)

{

trans.Rollback();

ErrorCode = ex.Number;

Info = "数据操作失败：" + ex.Message;

}

finally

{

trans.Dispose();

conn.Close();

}


但运行的时候却出错。错误提示为“**SqlTransaction****已经用完；它再也不能使用。**”

开始时，我怀疑是跟内存有关。因为系统需要做好事务回滚的准备，每执行一条插入或修改的SQL，都要有一定的开销，数据量一大，恐怕就吃不消了。不过我查了一下SQL SERVER的资料，未见提到内存的问题。

后来想到，数据库连接SqlConnection有个时间限制问题。默认是15秒。数据量大的时候，这个时间很可能就不够了。于是改为：
SqlConnection conn = new SqlConnection(strConn);

conn.Open();

SqlTransaction trans = conn.BeginTransaction();

try

{

SqlCommand cmd = new SqlCommand();

cmd.CommandType = CommandType.Text;

//连接时限改为300秒
           cmd.CommandTimeout = 300;

cmd.CommandText = sql;

cmd.Connection = conn;

cmd.Transaction = trans;


cmd.ExecuteNonQuery();


trans.Commit();

}

catch(SqlException ex)

{

trans.Rollback();

ErrorCode = ex.Number;

Info = "数据操作失败：" + ex.Message;

}

finally

{

trans.Dispose();

conn.Close();

}

问题解决。

