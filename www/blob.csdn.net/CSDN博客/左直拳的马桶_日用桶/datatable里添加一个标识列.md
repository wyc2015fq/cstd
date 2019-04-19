# datatable里添加一个标识列 - 左直拳的马桶_日用桶 - CSDN博客
2011年06月15日 17:45:00[左直拳](https://me.csdn.net/leftfist)阅读数：1842
        DataTable dt = new DataTable();
        DataColumn id = new DataColumn("Id");
        id.AutoIncrement = true;
        id.AutoIncrementSeed = 1;
        id.AutoIncrementStep = 1;
        dt.Columns.Add(id);
