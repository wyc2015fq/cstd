# c 一行一行都文件 - weixin_33985507的博客 - CSDN博客
2011年06月12日 11:26:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
```
void ParseResourcePath()
{
    g_vecResourcePath.clear();
    wxChar fname[] = "ResourcePath.config";
    FILE   *fp; 
    wxChar buf[MAX_PATH] ;
    if((fp   =   fopen(fname, "r "))   ==   NULL)
    {
        Logger::getSingleton().logEvent( "ParseResourcePath() - ResourcePath.config cannot find", Errors );
        return;
    }
    //fgets(buf,MAXBLINE,fp); 
    while   (!feof(fp))
    { 
        fgets(buf,MAX_PATH,fp);
        g_vecResourcePath.push_back(wxString(buf));
    }
    fclose(fp);
}
```
