# DataRow[]用作DataSource找不到列的问题 - 左直拳的马桶_日用桶 - CSDN博客
2011年02月23日 18:48:00[左直拳](https://me.csdn.net/leftfist)阅读数：2531
怪哉！如果将DataTable select后的结果，也就是一个DataRow[]数组，赋给控件做DataSource，居然会说有些列找不到！天地良心，这些列都在里面好不好。如果换成这个DataTable，什么问题也没有，
但DataRow[] drs = dt.Select("action='fuck'")，
***.DataSoure = drs;
***.DataBind();
就不行。
格老子。神马玩意。不知如何处理。
只好
DataRow[] drs = dtFuck.Select("action='fuck'");
DataTable dt = dtFuck.Clone();
foreach(DataRow dr in drs)
{
      dt.ImportRow(dr);
}
***.DataSoure = dt;
***.DataBind();
