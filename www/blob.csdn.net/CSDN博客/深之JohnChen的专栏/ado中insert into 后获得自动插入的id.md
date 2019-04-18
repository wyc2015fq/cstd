# ado中insert into 后获得自动插入的id - 深之JohnChen的专栏 - CSDN博客

2017年09月19日 18:05:21[byxdaz](https://me.csdn.net/byxdaz)阅读数：601


当运行完插入语句后，执行select @@identity就可得到自动生成的id

IDENT_CURRENT 返回为任何会话和任何作用域中的特定表最后生成的标识值。IDENT_CURRENT 不受作用域和会话的限制，而受限于指定的表。IDENT_CURRENT 返回为任何会话和作用域中的特定表所生成的值。

@@IDENTITY 返回为当前会话的所有作用域中的任何表最后生成的标识值。

SCOPE_IDENTITY 返回为当前会话和当前作用域中的任何表最后生成的标识值

SCOPE_IDENTITY 和 @@IDENTITY 返回在当前会话中的任何表内所生成的最后一个标识值。但是，SCOPE_IDENTITY 只返回插入到当前作用域中的值；@@IDENTITY 不受限于特定的作用域。

注意：

1、执行上面的语句（select @@identity）之后，VC通过_variant_t返回结果，结果_variant_t的vt始终是VT_DECIMAL。

2、数据库表中bigint字段，执行普通的语句（select * from table;等）之后，返回结果。结果_variant_t的vt是根据的具体值自动转换成VT_I2,VT_I4,VT_DECIMAL。

    代码：

```cpp
HRESULT hr = S_OK;
				_RecordsetPtr m_pRSet;
				//...
				//执行sql语句
				hr = m_pRSet->Open((char*)_bstr_t("select SCOPE_IDENTITY() as id"),m_pConn.GetInterfacePtr(),adOpenStatic,//adOpenDynamic
				adLockOptimistic,adCmdText) ;

			long nColCount = m_pRSet->GetFields()->GetCount();
			if(nColCount > 0)
			{
				//取得当前行当前列值
				_variant_t	varCol;
				_variant_t	varValue;
				varCol.vt = VT_I4;
				varCol.lVal = 0;
				varValue = m_pRSet->GetCollect(varCol);
				LONGLONG llValue = 0;
				switch(varValue.vt) 
				{
				case VT_EMPTY:
				case VT_UI1:	
					llValue = varValue.bVal;
					memcpy(pIdentifyId,&llValue,sizeof(BYTE));
					break;  
				case VT_I2:
					llValue = varValue.iVal;
					memcpy(pIdentifyId,&llValue,sizeof(SHORT));
					break;
				case VT_I4:
					llValue = varValue.lVal;
					memcpy(pIdentifyId,&llValue,sizeof(LONG));
					break;
				case VT_I8:
					llValue = varValue.llVal;
					memcpy(pIdentifyId,&llValue,sizeof(LONGLONG));
					break;
				case VT_DECIMAL:
					llValue = (LONGLONG)varValue;
					memcpy(pIdentifyId,&llValue,sizeof(LONGLONG));
					break;

				default:
					break;
				}
			}
```

    数据库中bigint关键字对于的_variant_t的类型是VT_DECIMAL

    VT_DECIMAL 转换为CString使用VariantChangeType函数做转换。

    ADO中_variant_t变量的使用与转换参考这篇文章http://blog.csdn.net/zyq5945/article/details/5581453

