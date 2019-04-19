# datagridview的数据存取 - 毛小亮 - 博客园
# [datagridview的数据存取](https://www.cnblogs.com/xianerwonder/p/4122362.html)
这里主要是复习DataSet等数据或ADO.NET方面的知识。下面是一个简单的数据存储（在DataGridView上增加一行然后并存储到数据库的过程）：
private void button1_Click(object sender, EventArgs e)
        {
            //插入一行内容
            SqlConnection con = new SqlConnection("Data Source=.;Initial Catalog=数据库名;User ID=sa;pwd=***");
            con.Open();
            SqlDataAdapter thisAdapter = new SqlDataAdapter("select itemId,productId,listPrice,unitCost,supplier,status,name,image from Item", con);
            SqlCommandBuilder scb = new SqlCommandBuilder(thisAdapter);
            DataSet ds = new DataSet();
            thisAdapter.Fill(ds, "Item");
            try
            {
                if (txtItemId.Text != "" && txtPicture.Text != "" && txtProductCount.Text != "" && txtProductId.Text != "" && txtProductName.Text != "" && txtProductPrice.Text != "" && txtYouhuiPrice.Text != "")
                {
                    //TODO:判断txtitemid里的内容已经存在数据库（写个bool方法，然后再次判断调用即可）
                    DataRow newRow = ds.Tables["Item"].NewRow();
                    newRow["itemId"] = this.txtItemId.Text;
                    newRow["productId"] = this.txtProductId.Text;
                    newRow["listPrice"] = this.txtProductPrice.Text;
                    newRow["unitCost"] = this.txtYouhuiPrice.Text;
                    newRow["supplier"] = this.txtProductCount.Text;
                    newRow["status"] = this.comboSellMode.Text;
                    newRow["name"] = this.txtProductName.Text;
                    newRow["image"] = this.txtPicture.Text;
                    ds.Tables["Item"].Rows.Add(newRow);
                    thisAdapter.Update(ds, "Item");
                }
                //更新datagridview表格
                //此处因为是绑定数据，不能用此句代码直接插入一行数据
                //dataGridView1.Rows.Add(new object[] { txtItemId.Text, txtProductId.Text, txtProductPrice.Text, txtYouhuiPrice.Text, txtProductCount.Text, comboSellMode.Text, txtProductName.Text, txtPicture.Text });//{}是对应列的值
                else
                {
                    MessageBox.Show("请将信息填写完整，并符合规则！", "Message to you:", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                con.Close();
                            }
            catch (Exception ex)
            {
                MessageBox.Show("请检查信息输入格式！" + ex.Message, "Message to you,", MessageBoxButtons.OK, MessageBoxIcon.Error);
                //这里有个bug，没有实现表的同步更新
            }
        }
    一般关于使用DataSet的用法基本上都是一个模式，或者说数据连接这一块都是一个模式。
    1. 首先就是SqlConnection 来new出来一个连接数据库的字符串；
    2. 接着用DataAdapter对数据进行许多不同种类的操作，包括查询、更新和删除。它比DataReader对象更加通用一些；
    3.然后并没有编写SQL语句来进行更新，而是创建了更简单实用的CommandBuilder对象，它可以为我们创建正确的SQL语句，然后并将其自动与DataAdaper相关联；
    4.接下来就到了ado.net中重要的DataSet对象了，所有的复杂操作都会使用到它。它包含一组相关的DataTable对象，代表要使用的数据库表。每一个DataTable对象都有子DataRow和DataColumn对象，分别代表数据库表的行列。通过这些对象就可以获得表的所有元素；
     5.然后就是DataAdapter的填充方法 ：thisAdapter.Fill(ds, "Item");
      6.最后要注意的是：数据库连接打开后，最后一定要关闭；对数据表操作后，一定要执行UpDate（）方法，才能将修改保存到数据库；它隶属于DataAdapter。
       上面六步基本就是各种套路的东西了，如果有对数据的操作，就可以把这些操作的语句放在填充之后，也就是Fill（）方法之后，因为这些的更改只是停留在内存，需要最后执行DataAdapter的UpDate（）方法才能最终得以更新到数据库中。
        当这样的操作比较多的时候，就会发现很多方法的代码都是重复的，也就是上面那些套路。这个时候，我们就要用到面向对象了，也就是把这些相同的东西提取出来写成一个方法让大家共享，这就是一个面向对象或是不断重载等进行代码简化的过程，这就是真正严谨的开发的开始了吧。

