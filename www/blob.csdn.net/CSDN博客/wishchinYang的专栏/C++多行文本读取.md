# C++多行文本读取 - wishchinYang的专栏 - CSDN博客
2015年09月16日 18:46:11[wishchin](https://me.csdn.net/wishchin)阅读数：1630
                
      使用的多行读取的代码如下：
```cpp
//读取文本浮点数到多个模式 序列
bool CPicToolsDlg::readTxt2SeqMulti(
	std::string TxtName, std::vector<std::vector<float > >  &Seq )
{
	//三元组，使用三元组逐个写入
	float ViewValue,ViewValue2,ViewValue3;  
	Seq.resize(0);
	std::string TestTxt(TxtName);  //  
	std::ifstream  ifs;
	std::vector<std::string>  vec;
	std::string   line;
	int IdxLine    = 0;
	ifs.open(TestTxt.c_str() );
	if (!ifs.rdbuf()->is_open())
	{
		printf("Error in open file %s\n", TestTxt.c_str());
		return false;
	}
	else
	{
		while(getline(ifs,line))//循环取出每一行数据
		{
			std::vector<float >  Vline(0);
			std::istringstream  stream(line);
			while( stream>> ViewValue)//将一行的多个字段取出
			{//对一行中的每个字段进行处理
				Vline.push_back(ViewValue);
			}
			//++IdxLine;
			Seq.push_back(Vline);
		}
	}
	ifs.close();
	return true;
}
```
