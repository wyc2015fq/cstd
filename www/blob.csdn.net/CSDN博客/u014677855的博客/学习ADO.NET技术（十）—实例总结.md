# 学习ADO.NET技术（十）—实例总结 - u014677855的博客 - CSDN博客
2018年08月17日 15:32:13[xiaobigben](https://me.csdn.net/u014677855)阅读数：143
经过这几天的学习，对ADO.NET技术有了一个基本的认识。今天就通过一个实例来总结ADO.NET中常用到的知识点。
## 1、实例要求
目标功能：创建一个数据库HR，员工数据表Emp。实现对员工信息的展示、增、删、查、改、搜搜索、多行删除、多条件组合搜索等功能。 
要求：创建员工信息的实体类，将对数据库的操作封装在SqlHelper类中。
## 2、实现
### 2.1 创建数据库HR
打开SQL Server数据库管理系统，创建数据库。 
![这里写图片描述](https://img-blog.csdn.net/20180816153113843?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 2.2创建员工信息表Emp
![这里写图片描述](https://img-blog.csdn.net/20180816160825720?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 2.3 创建窗体，展示数据表在窗体中
```
string strSql = "select * from Emp ";
            using (SqlConnection conn = new SqlConnection(strConn))
            {
                SqlCommand cmd = new SqlCommand(strSql, conn);
                try
                {
                    conn.Open();
                    SqlDataReader sdr = cmd.ExecuteReader();
                    while (sdr.Read())
                    {
                        Emp emp = new Emp();
                        emp.Id = Convert.ToInt32(sdr["Id"]);
                        emp.Name = sdr["Name"] + "";
                        emp.Phone = sdr["Phone"] + "";
                        emp.Height = Convert.ToInt32(sdr["Height"]);
                        emp.Memo = sdr["Memo"] + "";
                        emps.Add(emp);
                    }
                    dataGridView1.DataSource = emps;
                    sdr.Close();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("异常信息：\n" + ex.Message);
                }
            }
```
上述代码是将数据表中所有数据展示到窗体中。 
运行结果如下所示： 
![这里写图片描述](https://img-blog.csdn.net/20180817142048349?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这里展示了表中所有的员工信息到界面中。
### 2.4新增员工记录到表中
```
//将新增信息添加到数据表中
            string name = txtName.Text;
            string phone = txtPhone.Text;
            string height = txtHeight.Text;
            string marks = txtMarks.Text;
            StringBuilder strSql = new StringBuilder();
            strSql.Append("insert into Emp(Name,Phone,Height,Memo) values(@Name,@Phone,@Height,@marks)");
            //构造参数
            SqlParameter[] paras = new SqlParameter[] { new SqlParameter("@Name", name),new SqlParameter("@Phone",phone),
                new SqlParameter("@Height",height),new SqlParameter("@marks",marks) };
            //执行命令
            using (SqlConnection conn = new SqlConnection(strConn))
            {
                SqlCommand cmd = new SqlCommand(strSql.ToString(), conn);
                cmd.Parameters.AddRange(paras);
                conn.Open();
                int rows = cmd.ExecuteNonQuery();
                if (rows > 0)
                    MessageBox.Show("新增成功！");
                Close();
            }
```
### 2.5 删除记录
```
//选中的第一行第一列的值
            int id = Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[0].Value);
            string strSql = "delete from Emp where Id = @Id";
            SqlParameter para1 = new SqlParameter("@Id", id);
            using (SqlConnection conn = new SqlConnection(strConn))
            {
                SqlCommand cmd = new SqlCommand(strSql, conn);
                cmd.Parameters.Add(para1);
                try
                {
                    conn.Open();
                    int rows = cmd.ExecuteNonQuery();
                    if(rows>0)
                    {
                        MessageBox.Show("删除成功");
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("异常信息：\n" + ex.Message);
                }
            }
```
### 2.6 修改信息
修改某一行选中的员工信息：
```
int id = Convert.ToInt32(dataGridView1.SelectedRows[0].Cells[0].Value);
            string strSql = "update Emp SET Name = @Name,Phone = @Phone,Height = @Height" +
                ",Memo = @Memo WHERE Id = @Id";
            SqlParameter[] paras = new SqlParameter[] {new SqlParameter("@Name","滴滴"),
            new SqlParameter("@Phone","15682019803"),new SqlParameter("@Height","162"),new SqlParameter("@Memo","滴滴"),
            new SqlParameter("@id",id),};
            using (SqlConnection conn = new SqlConnection(strConn))
            {
                SqlCommand cmd = new SqlCommand(strSql, conn);
                cmd.Parameters.AddRange(paras);
                try
                {
                    conn.Open();
                    int rows = cmd.ExecuteNonQuery();
                    if (rows > 0)
                        MessageBox.Show("修改成功！");
                }
                catch (Exception ex)
                {
                    MessageBox.Show("异常信息：" + ex.Message);
                }
            }
```
### 2.7根据员工姓名关键字查询员工信息
```
string strSQL = "select * from Emp where Name like @Name";
            SqlParameter para = new SqlParameter("@Name", "%" + txtName.Text + "%");
            using (SqlConnection conn = new SqlConnection(strConn))
            {
                try
                {
                    SqlCommand cmd = new SqlCommand(strSQL, conn);
                    cmd.Parameters.Add(para);
                    conn.Open();
                    SqlDataReader sdr = cmd.ExecuteReader();
                    while (sdr.Read())
                    {
                        Emp emp = new Emp();
                        emp.Id = Convert.ToInt32(sdr["Id"]);
                        emp.Name = sdr["Name"] + "";
                        emp.Phone = sdr["Phone"] + "";
                        emp.Height = Convert.ToInt32(sdr["Height"]);
                        emp.Memo = sdr["Memo"] + "";
                        emps.Add(emp);
                    }
                    dataGridView1.DataSource = emps;
                    sdr.Close();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("异常信息：\n" + ex.Message);
                }
            }
```
查询结果： 
![这里写图片描述](https://img-blog.csdn.net/20180817153202768?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
