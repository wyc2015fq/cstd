
# Datatable转换成List实体对象列表 - 追求卓越,做到专业 - CSDN博客


2015年11月20日 15:56:08[Waldenz](https://me.csdn.net/enter89)阅读数：2580




```python
/// <summary>
        /// 将Datatable转换为List集合
        /// </summary>
        /// <typeparam name="T">类型参数</typeparam>
        /// <param name="dt">datatable表</param>
        /// <returns></returns>
        public static List<T> DataTableToList<T>(DataTable dt)
        {
            var list = new List<T>();
            Type t = typeof(T);
            var plist = new List<PropertyInfo>(typeof(T).GetProperties());
            foreach (DataRow item in dt.Rows)
            {
                T s = System.Activator.CreateInstance<T>();
                for (int i = 0; i < dt.Columns.Count; i++)
                {
                    PropertyInfo info = plist.Find(p => p.Name == dt.Columns[i].ColumnName);
                    if (info != null)
                    {
                        if (!Convert.IsDBNull(item[i]))
                        {
                            info.SetValue(s, item[i], null);
                        }
                    }
                }
                list.Add(s);
            }
            return list;
        }
```


