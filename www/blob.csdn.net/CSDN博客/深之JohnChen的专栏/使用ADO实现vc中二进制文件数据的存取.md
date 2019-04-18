# 使用ADO实现vc中二进制文件数据的存取 - 深之JohnChen的专栏 - CSDN博客

2017年09月03日 16:04:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：810


                
使用ADO实现vc中二进制文件数据的存取

转自：http://home.kaoyan.com/home.php?mod=space&uid=2820987&do=blog&id=46560

简述关键代码如下：

1、保存图片数据到数据库

以下是代码片段：

      //JPG图片保存到数据库    

          try    

          {    

              _RecordsetPtr    pRecordset;    

              pRecordset.CreateInstance(__uuidof(Recordset));    

              pRecordset->Open("SELECT * FROM jpg",_variant_t((IDispatch*)pConnection),adOpenStatic,adLockOptimistic,adCmdText);    

              pRecordset->AddNew();    

              pRecordset->Fields->Item["jpgid"]->Value = (_variant_t)m_JPGId;//jpgid    

              VARIANT pvList;    

           SetPictureToVariant(pvList,(unsigned char *)m_pJPGBuffer);    

           pRecordset->Fields->Item["jpgimage"]->AppendChunk(pvList); //JPG图像文件    

           VariantClear(&pvList);    

           pRecordset->Update();    

           pRecordset->Close();    

           AfxMessageBox("JPG图像保存成功！");    

           m_JPGId == "";    

           UpdateData(false);    

       }    

       catch(...)    

       {    

           AfxMessageBox("数据库读取失败");    

           return;    

       }

其中SetPictureToVariant如下：

以下是代码片段：

      void CBMPinDBDlg::SetPictureToVariant(VARIANT &pvList, unsigned char *sPicture)    

          {    

              SAFEARRAYBOUND saBound[1];    

              saBound[0].cElements = m_nFileLen;    

              saBound[0].lLbound = 0;    

              SAFEARRAY *pSA = SafeArrayCreate(VT_UI1, 1, saBound);       

              for (long l = 0; l < (long)m_nFileLen; l ++)    

              {    

                  SafeArrayPutElement( pSA, &l, (void*)&sPicture[l]);           

           }    

           VariantClear(&pvList);    

           pvList.vt = VT_UI1 | VT_ARRAY;    

           pvList.parray = pSA;    

       } 

2、从数据库读取图像文件并且显示在界面

    以下是代码片段：

          //从数据库里取JPG图像文件    

          try    

          {    

              _RecordsetPtr    pRecordset;    

              char sSql[129];    

              sprintf(sSql,"SELECT *FROM jpg WHERE jpgid=’%s’",m_JPGId);    

              pRecordset.CreateInstance(__uuidof(Recordset));    

              pRecordset->Open(sSql,_variant_t((IDispatch*)pConnection),adOpenStatic,adLockOptimistic,adCmdText);    

              if (pRecordset->adoEOF)    

           {    

               CString str;    

               str.Format("没有JPGid为: %s 的JPG图像!",m_JPGId );    

               AfxMessageBox(str);    

               Invalidate();    

               m_JPGId = "";    

               UpdateData(false);    

               m_EidtJPGId.SetFocus();    

               return;    

           }    

           _variant_t pvList ;           

           long lDataSize = pRecordset->GetFields()->GetItem("jpgimage")->ActualSize;    

           m_nFileLen = (DWORD)lDataSize;    

           if(lDataSize > 0)    

           {    

               _variant_t            varBLOB;    

               varBLOB = pRecordset->GetFields()->GetItem("jpgimage")->GetChunk(lDataSize);    

               //把二进制格式的图片转为图片格式    

               try    

               {    

                   if(varBLOB.vt == (VT_ARRAY | VT_UI1))    

                   {    

                       if(m_pJPGBuffer = new char[lDataSize+1])           

                       {       

                           char *pBuf = NULL;    

                           SafeArrayAccessData(varBLOB.parray,(void **)&pBuf);    

                           memcpy(m_pJPGBuffer,pBuf,lDataSize);                   

                           SafeArrayUnaccessData (varBLOB.parray);    

                           m_nFileLen = lDataSize;    

                           //m_pJPGBuffer -> pPicture                       

                           HGLOBAL hMem = ::GlobalAlloc( GMEM_MOVEABLE, m_nFileLen );    

                           LPVOID lpBuf = ::GlobalLock( hMem );                       

                           memcpy(lpBuf,m_pJPGBuffer,m_nFileLen);    

                           ::GlobalUnlock( hMem );    

                           if ( CreateStreamOnHGlobal( hMem, TRUE, &pStream ) !=S_OK )         

                               return ;    

                           if ( OleLoadPicture( pStream, m_nFileLen, TRUE, IID_IPicture, ( LPVOID * )&pPicture ) !=S_OK )    

                               return ;    

                           Invalidate();//在界面显示                     

                       }    

                   }    

               }    

               catch(...)    

               {    

                   AfxMessageBox("从数据库中读取jpg图像有错！");    

                   return;    

               }    

           }    

       }    

       catch(...)    

       {    

           AfxMessageBox("数据库读取失败");    

           return;    

       }                    

