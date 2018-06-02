#if 0
BOOL OnInitDialog()
{
#define FEATURE_LEN 4330
    typedef struct
    {
        char m_Feature[FEATURE_LEN];
        DEV_CID m_PID;
        unsigned char m_Flag;
    } asdf;
    return (0);
}
BOOL OnInitDialog()
{
    int ddd = (1 << 31) - 1;
    int cb = sizeof(DEV_DATE);
    m_menu.InserMenuItem(IDM_DEVSTATIS, ("统计信息"));
    m_menu.InserMenuItem(IDM_DEVSET, ("工作属性"));
    m_menu.InserMenuItem(IDM_NETWORKSET, ("网络设置"));
    m_menu.InserMenuItem(IDR_MENU_SYN_TIME, ("时间同步"));
    m_menu.InserMenuItem(IDR_TIMEGROUP, ("时间组"));
    m_menu.InserMenuItem(IDR_RIGHT, ("权限"));
    m_menu.InserMenuItem(IDR_UGROUP, ("用户组"));
    m_menu.InserMenuItem(IDR_DEV_FORMAT, ("格式化"));
    m_menu.InserMenuItem(IDR_DEVAUTH, ("设置验证"));
    m_menu.InserMenuItem(IDR_DEVCUT, ("断开连接"));
    m_menu.InserMenuItem(IDR_SCREEN, ("检验屏幕"));
    InitDev();
    InitUser();
    InitRecord();
    InitLog();
    InitCImage();
    m_cmbCondition.SetCurSel(0);
    m_cmbRecordCondition.AddString(("所有记录"));
    m_cmbRecordCondition.AddString(("最新识别记录"));
    m_cmbRecordCondition.AddString(("所有报警记录"));
    m_cmbRecordCondition.AddString(("时间段识别记录"));
    m_cmbRecordCondition.AddString(("最新报警记录"));
    m_cmbRecordCondition.AddString(("时间段报警记录"));
    m_cmbRecordCondition.AddString(("区间段识别记录"));
    m_cmbRecordCondition.AddString(("区间段报警记录"));
    m_cmbRecordCondition.SetCurSel(0);
    ((CButton*)(GetDlgItem(IDC_RADIO_ALL)))->SetCheck(TRUE);
    SLG_InitSys(TRUE);
    SLG_RegDevConnectStatusCB(::DevConnectNotify, this);
    SLG_RegOperResultNotifyCB(::DevOperResult, this);
    GetDlgItem(IDC_EDIT_FEAT)->SetWindowText(("20"));
    GetDlgItem(IDC_EDIT_PIC)->SetWindowText(("20"));
    SetTimer(CLEARLCLICKTIME, 1200, NULL);
    UpdateData(FALSE);
    m_IPDev.SetAddress(192, 168, 1, 159);
    m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    return TRUE; // return TRUE unless you set the focus to a control
}
int InitDev()
{
    RECT eRt;
    m_dev.GetClientRect(&eRt);
    int width = (eRt.right - eRt.left) / 3;
    m_dev.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
    m_dev.InsertColumn(0, "IP", LVCFMT_LEFT, width);
    m_dev.InsertColumn(1, "设备类型", LVCFMT_LEFT, width);
    m_dev.InsertColumn(2, "人数上线", LVCFMT_LEFT, width);
}
int InitUser()
{
    RECT eRt;
    m_user.GetClientRect(&eRt);
    int width = (eRt.right - eRt.left) / 7;
    // m_user.SetBkColor( RGB(231, 246, 253) );
    // m_user.ModifyStyle( 0, LBS_NOTIFY | LVS_OWNERDRAWFIXED );
    m_user.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
    m_user.InsertColumn(0, "ID", LVCFMT_LEFT, width);
    m_user.InsertColumn(1, "卡号", LVCFMT_LEFT, width);
    m_user.InsertColumn(2, "姓名", LVCFMT_LEFT, width);
    m_user.InsertColumn(3, "权限", LVCFMT_LEFT, width);
    m_user.InsertColumn(4, "类型", LVCFMT_LEFT, width);
    m_user.InsertColumn(5, "黑名单", LVCFMT_LEFT, width);
    m_user.InsertColumn(6, "失效时间", LVCFMT_LEFT, width);
    CString sql = ("select * from [user]");
    CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(sql);
    if (ptr->IsEmpty())
    {
        return;
    }
    ptr->MoveFirst();
    int i = 0;
    int n = 0;
    char values[5] = {0};
    for (; !(ptr->IsEOF()); ptr->MoveNext(), ++i)
    {
        n = m_user.InsertItem(i, ptr->GetCString(("UserID")));
        m_user.SetItemText(n, 0, ptr->GetCString(("UserID")));
        m_user.SetItemText(n, 1, ptr->GetCString(("AppendID")));
        m_user.SetItemText(n, 2, ptr->GetCString(("UserName")));
        sprintf(values, "%d", ptr->GetLong(("RightID")));
        m_user.SetItemText(n, 3, values);
        sprintf(values, "%d", ptr->GetLong(("UserType")));
        m_user.SetItemText(n, 4, values);
        sprintf(values, "%d", ptr->GetLong(("UserFlag")));
        m_user.SetItemText(n, 5, values);
        m_user.SetItemText(n, 6, ptr->GetCString(("InvalidTime")));
    }
}
int InitRecord()
{
    UI_Init(m_record.GetSafeHwnd(), &aa, SW_CSuperGridCtrl);
    aa.m_crCellRGB = -1;
    //HIMAGELIST hIml = CImageList_Create( IDB_FOLDERS, 16, 1, RGB( 0, 255, 255 ) );
    //CListCtrl_SetImageList( aa.m_hWnd, hIml, LVSIL_SMALL );
    HIMAGELIST pImageList = CListCtrl_GetImageList(aa.m_hWnd, LVSIL_SMALL);
    if (pImageList)
    {
        ImageList_GetIconSize(pImageList, &aa.m_cxImage, &aa.m_cyImage);
    }
    //CListCtrl_SetImageList( aa.m_hWnd, hIml, LVSIL_SMALL );
    CListCtrl_SetExtendedStyle(aa.m_hWnd, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
    LV_COLUMN lvColumn;
    //initialize the columns
    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvColumn.fmt = LVCFMT_LEFT;
    RECT eRt;
    m_record.GetClientRect(&eRt);
    int width = (eRt.right - eRt.left) / 5;
    lvColumn.cx = width;
    lvColumn.pszText = "记录ID";
    CListCtrl_InsertColumn(aa.m_hWnd, 0, &lvColumn);
    lvColumn.pszText = "用户ID";
    CListCtrl_InsertColumn(aa.m_hWnd, 1, &lvColumn);
    lvColumn.pszText = "记录类型";
    CListCtrl_InsertColumn(aa.m_hWnd, 2, &lvColumn);
    lvColumn.pszText = "验证模式";
    CListCtrl_InsertColumn(aa.m_hWnd, 3, &lvColumn);
    lvColumn.pszText = "记录来源";
    CListCtrl_InsertColumn(aa.m_hWnd, 4, &lvColumn);
}
int InitLog()
{
    RECT eRt;
    m_log.GetClientRect(&eRt);
    int width = (eRt.right - eRt.left) / 5;
    m_log.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
    m_log.InsertColumn(0, "日志ID", LVCFMT_LEFT, width);
    m_log.InsertColumn(1, "类型", LVCFMT_LEFT, width);
    m_log.InsertColumn(2, "操作员", LVCFMT_LEFT, width);
    m_log.InsertColumn(3, "被操作员", LVCFMT_LEFT, width);
    m_log.InsertColumn(4, "时间", LVCFMT_LEFT, width);
    // m_log.InsertItem(0, "test");
}
int InitCImage()
{
    UI_Init(m_image.GetSafeHwnd(), &bb, SW_CSuperGridCtrl);
    bb.m_crCellRGB = -1;
    //HIMAGELIST hIml = CImageList_Create( IDB_FOLDERS, 16, 1, RGB( 0, 255, 255 ) );
    //CListCtrl_SetImageList( aa.m_hWnd, hIml, LVSIL_SMALL );
    HIMAGELIST pImageList = CListCtrl_GetImageList(bb.m_hWnd, LVSIL_SMALL);
    if (pImageList)
    {
        ImageList_GetIconSize(pImageList, &bb.m_cxImage, &bb.m_cyImage);
    }
    //CListCtrl_SetImageList( aa.m_hWnd, hIml, LVSIL_SMALL );
    CListCtrl_SetExtendedStyle(bb.m_hWnd, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
    LV_COLUMN lvColumn;
    //initialize the columns
    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvColumn.fmt = LVCFMT_LEFT;
    RECT eRt;
    m_image.GetClientRect(&eRt);
    int width = (eRt.right - eRt.left) / 3;
    lvColumn.cx = width;
    lvColumn.pszText = "流水号ID";
    CListCtrl_InsertColumn(bb.m_hWnd, 0, &lvColumn);
    lvColumn.pszText = "用户ID";
    CListCtrl_InsertColumn(bb.m_hWnd, 1, &lvColumn);
    lvColumn.pszText = "时间";
    CListCtrl_InsertColumn(bb.m_hWnd, 2, &lvColumn);
    // lvColumn.pszText = "验证模式";
    // CListCtrl_InsertColumn( aa.m_hWnd, 3, &lvColumn );
    // lvColumn.pszText = "记录来源";
    // CListCtrl_InsertColumn( aa.m_hWnd, 4, &lvColumn );
    //
    // m_image.InsertColumn(0, "流水号ID", LVCFMT_LEFT, width );
    // m_image.InsertColumn(1, "用户ID", LVCFMT_LEFT, width );
    // m_image.InsertColumn(2, "时间", LVCFMT_LEFT, width);
}
//搜索设备
int OnButtonSerch()
{
    static BOOL serch = TRUE;
    UpdateData(TRUE);
    BYTE field0 = 0, field1 = 0, field2 = 0, field3 = 0;
    m_IPCtrl.GetAddress(field0, field1, field2, field3);
    KillTimer(SERCH_EVENT_TIME);
    memset(m_IP, 0x0, sizeof(m_IP));
    sprintf(m_IP, "%d.%d.%d.%d", field0, field1, field2, field3);
    char ip[16] = {0};
    // BYTE field0 = 0, field1=0, field2=0, field3=0;
    m_IPServer.GetAddress(field0, field1, field2, field3);
    sprintf(ip, "%d.%d.%d.%d", field0, field1, field2, field3);
    // SLG_SerchDev( ip//, m_IP*/);
    // serch ? SetTimer(SERCH_EVENT_TIME, 1000*10, NULL): KillTimer( SERCH_EVENT_TIME );
    // serch = FALSE;
}
int OnButtonSetaddr()
{
    POSITION pos = m_dev.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(("请选择一个设备"));
        return;
    }
    int nIndex = m_dev.GetNextSelectedItem(pos);
    CString str_ip = m_dev.GetItemText(nIndex, 0);
    UpdateData(TRUE);
    BYTE field0 = 0, field1 = 0, field2 = 0, field3 = 0;
    m_IPCtrl.GetAddress(field0, field1, field2, field3);
    char ip[16] = {0};
    sprintf(ip, "%d.%d.%d.%d", field0, field1, field2, field3);
    // SLG_ULDevRegionAddr(str_ip.GetBuffer(0), ip);
}
int OnCommandMenu(UINT nID)
{
    POSITION pos = m_dev.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        return;
    }
    int nIndex = m_dev.GetNextSelectedItem(pos);
    CString str_ip = m_dev.GetItemText(nIndex, 0);
    if (IDM_DEVSTATIS == nID)
    {
        DevStatisDlg dlg(str_ip.GetBuffer(0));
        dlg.DoModal();
        SLG_RegOperResultNotifyCB(::DevOperResult, this);
    }
    else if (IDM_DEVSET == nID)
    {
        DevSetDlg dlg(m_ado, str_ip.GetBuffer(0));
        dlg.DoModal();
        SLG_RegOperResultNotifyCB(::DevOperResult, this);
    }
    else if (IDM_NETWORKSET == nID)
    {
        CWebServiceMrg dlg(str_ip.GetBuffer(0));
        dlg.DoModal();
        SLG_RegOperResultNotifyCB(::DevOperResult, this);
    }
    else if (IDR_MENU_SYN_TIME == nID)
    {
        DEV_DATETIME dev_time;
        sftime st;
        dev_time.m_Date.m_Year = st.year();
        dev_time.m_Date.m_Month = st.month();
        dev_time.m_Date.m_Day = st.day();
        dev_time.m_Time.m_Hour = st.hour();
        dev_time.m_Time.m_Minute = st.minute();
        dev_time.m_Time.m_Second = st.second();
        dev_time.m_Time.m_Msec = st.millis();
        SLG_ULDevTime(str_ip.GetBuffer(0), dev_time);
    }
    else if (IDR_TIMEGROUP == nID)
    {
        CUDDlg dlg(m_ado, str_ip.GetBuffer(0), OPER_TIMEGROUP);
        dlg.DoModal();
        SLG_RegOperResultNotifyCB(::DevOperResult, this);
    }
    else if (IDR_RIGHT == nID)
    {
        CUDDlg dlg(m_ado, str_ip.GetBuffer(0), OPER_RIGHT);
        dlg.DoModal();
        SLG_RegOperResultNotifyCB(::DevOperResult, this);
    }
    else if (IDR_DEVCUT == nID)
    {
        SLG_DCNDev(str_ip.GetBuffer(0));
        m_dev.DeleteItem(nIndex);
    }
    else if (IDR_DEV_FORMAT == nID)
    {
        BOOL ret = SLG_ULFormat(str_ip.GetBuffer(0));
    }
    else if (IDR_DEVAUTH == nID)
    {
        AuthDlg dlg(str_ip.GetBuffer(0));
        dlg.DoModal();
    }
    else if (IDR_UGROUP == nID)
    {
        CUDDlg dlg(m_ado, str_ip.GetBuffer(0), OPER_USERGROUP);
        dlg.DoModal();
        SLG_RegOperResultNotifyCB(::DevOperResult, this);
    }
    else if (IDR_DMYPWD == nID)
    {
        CCreateDynamicDlg dlg(str_ip.GetBuffer(0), this);
        dlg.DoModal();
        SLG_RegOperResultNotifyCB(::DevOperResult, this);
    }
    else if (IDR_SCREEN == nID)
    {
        //AfxMessageBox("adwewew");
        BOOL bRet = SLG_ResetScreen(str_ip.GetBuffer(0));
    }
}
//上传数据
int OnButtonUp()
{
    if (((CButton*)(GetDlgItem(IDC_CHECK_USER_EX)))->GetCheck())
    {
        UpUserEx();
        return;
    }
    POSITION pos = m_user.GetFirstSelectedItemPosition();
    CString str_id;
    int nIndex = 0;
    while (pos)
    {
        nIndex = m_user.GetNextSelectedItem(pos);
        str_id = str_id + ("\'") + m_user.GetItemText(nIndex, 0) + ("\',");
    }
    str_id = str_id.Left(str_id.GetLength() - 1);
    CString str_sql;
    str_sql.Format(("select * from [user] where [UserID] in (%s) order by [UserID]"), str_id);
    CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(str_sql);
    if (ptr->IsEmpty())
    {
        return;
    }
    uutime t_t;
    uutime t_test;
    POSITION pos1 = m_dev.GetFirstSelectedItemPosition();
    CString str_ip;
    while (pos1)
    {
        nIndex = m_dev.GetNextSelectedItem(pos1);
        str_ip = m_dev.GetItemText(nIndex, 0);
        ptr->MoveFirst();
        for (; !(ptr->IsEOF()) ; ptr->MoveNext())
        {
            DEV_USER user = {0};
            memcpy(user.m_UserID.m_ID, ptr->GetCString(("UserID")).GetBuffer(0), ptr->GetCString(("UserID")).GetLength());
            memcpy(user.m_AppendID.m_ID, ptr->GetCString(("AppendID")).GetBuffer(0), ptr->GetCString(("AppendID")).GetLength());
            memcpy(user.m_ImageSID.m_ID, ptr->GetCString(("ChangeID")).GetBuffer(0), ptr->GetCString(("ChangeID")).GetLength());
            memcpy(user.m_UserName.m_ID, ptr->GetCString(("UserName")).GetBuffer(0), ptr->GetCString(("UserName")).GetLength());
            memcpy(user.m_Department.m_ID, ptr->GetCString(("Department")).GetBuffer(0), ptr->GetCString(("Department")).GetLength());
            memcpy(user.m_Admin.m_ID, ptr->GetCString(("RegAdmin")).GetBuffer(0), ptr->GetCString(("RegAdmin")).GetLength());
            user.m_RightID = ptr->GetLong(("RightID"));
            user.m_UserFlag = ptr->GetLong(("UserFlag"));
            user.m_UserType = DEV_USER_TYPE(ptr->GetLong(("UserType")));
            COleDateTime time;
            if (time.ParseDateTime(ptr->GetCString(("InvalidTime"))))
            {
                user.m_DeadLineTime.m_Date.m_Year = time.GetYear();
                user.m_DeadLineTime.m_Date.m_Month = time.GetMonth();
                user.m_DeadLineTime.m_Date.m_Day = time.GetDay();
                user.m_DeadLineTime.m_Time.m_Hour = time.GetHour();
                user.m_DeadLineTime.m_Time.m_Minute = time.GetMinute();
                user.m_DeadLineTime.m_Time.m_Second = time.GetSecond();
            }
            if (time.ParseDateTime(ptr->GetCString(("RegistTime"))))
            {
                user.m_RegistTime.m_Date.m_Year = time.GetYear();
                user.m_RegistTime.m_Date.m_Month = time.GetMonth();
                user.m_RegistTime.m_Date.m_Day = time.GetDay();
                user.m_RegistTime.m_Time.m_Hour = time.GetHour();
                user.m_RegistTime.m_Time.m_Minute = time.GetMinute();
                user.m_RegistTime.m_Time.m_Second = time.GetSecond();
            }
            if (time.ParseDateTime(ptr->GetCString(("LastUpdTime"))))
            {
                user.m_LastUpdTime.m_Date.m_Year = time.GetYear();
                user.m_LastUpdTime.m_Date.m_Month = time.GetMonth();
                user.m_LastUpdTime.m_Date.m_Day = time.GetDay();
                user.m_LastUpdTime.m_Time.m_Hour = time.GetHour();
                user.m_LastUpdTime.m_Time.m_Minute = time.GetMinute();
                user.m_LastUpdTime.m_Time.m_Second = time.GetSecond();
            }
            if (time.ParseDateTime(ptr->GetCString(("TypeUpdTime"))))
            {
                user.m_TypeUpdTime.m_Date.m_Year = time.GetYear();
                user.m_TypeUpdTime.m_Date.m_Month = time.GetMonth();
                user.m_TypeUpdTime.m_Date.m_Day = time.GetDay();
                user.m_TypeUpdTime.m_Time.m_Hour = time.GetHour();
                user.m_TypeUpdTime.m_Time.m_Minute = time.GetMinute();
                user.m_TypeUpdTime.m_Time.m_Second = time.GetSecond();
            }
            char buf[130 * 1024] = {0};
            ptr->GetChunk(("Block"), (BYTE*)buf, ptr->GetLong(("FeatBytes")) + ptr->GetLong(("PicBytes")));
            user.m_FeatLen = ptr->GetLong(("FeatBytes"));
            user.m_PicLen = ptr->GetLong(("PicBytes"));
            user.m_FeatData = buf;
            user.m_PicData = buf + user.m_FeatLen;
            t_test.restart();
            if (!SLG_ULUser(str_ip.GetBuffer(0), user))
            {
                DEBUG_INFO(("%s 数据上传失败"), user.m_UserID.m_ID);
            }
            else
            {
                double dd = t_test.elapsed();
                /// DEBUG_INFO( ("%s 数据上传成功 %0.2f"), user.m_UserID.m_ID, dd );
            }
            // WaitForSingleObject(m_hEvent, 60*1000);
            /// Sleep(1000);
        }
        DEBUG_INFO(("%s %0.2f"), ("数据上传完成"), t_t.elapsed());
    }
}
#define USER_UP_PIC_BIN_MAX 10
#define USER_UP_FEAT_BIN_MAX 20
#define USER_UP_TEXT_MAX 2000
int UpUserEx()
{
    UpdateData(TRUE);
    int nIndex = 0;
    POSITION pos1 = m_dev.GetFirstSelectedItemPosition();
    CString str_ip;
    if (pos1 == NULL)
    {
        return ;
    }
    nIndex = m_dev.GetNextSelectedItem(pos1);
    str_ip = m_dev.GetItemText(nIndex, 0);
    POSITION pos = m_user.GetFirstSelectedItemPosition();
    CString str_id;
    while (pos)
    {
        nIndex = m_user.GetNextSelectedItem(pos);
        str_id = str_id + ("\'") + m_user.GetItemText(nIndex, 0) + ("\',");
    }
    str_id = str_id.Left(str_id.GetLength() - 1);
    CString str_sql;
    str_sql.Format(("select * from [user] where [UserID] in (%s) order by [UserID]"), str_id);
    CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(str_sql);
    if (ptr->IsEmpty())
    {
        return;
    }
    int pic_select = ((CButton*)GetDlgItem(IDC_CHECK_PIC))->GetCheck();
    int feat_select = ((CButton*)GetDlgItem(IDC_CHECK_FEAT))->GetCheck();
    int text_select = ((CButton*)GetDlgItem(IDC_CHECK_TEXT))->GetCheck();
    int flag[3] = {0};
    if (text_select)
    {
        flag[0] = DL_DEV_USER_TEXT;
    }
    if (feat_select)
    {
        flag[1] = DL_DEV_USER_FEAT;
    }
    if (pic_select)
    {
        flag[2] = DL_DEV_PIC;
    }
    // int MAX = (flag == DL_DEV_USER_TEXT) ? USER_UP_TEXT_MAX: (flag == DL_DEV_USER_FEAT) ? USER_UP_FEAT_BIN_MAX: USER_UP_PIC_BIN_MAX;
    DEV_BATCH_USER users;
    users.m_nCount = m_user.GetSelectedCount();
    users.m_pUserArray = new DEV_USER[ users.m_nCount + 1 ];
    DEV_USER* pCurUser = users.m_pUserArray;
    for (int j = 0; j < users.m_nCount + 1; ++j)
    {
        pCurUser->m_FeatData = NULL;
        pCurUser->m_PicData = NULL;
        ++pCurUser;
    }
    pCurUser = users.m_pUserArray;
    for (int i = 0; (flag == DL_DEV_USER_FEAT || flag == DL_DEV_PIC) && * / i < USER_UP_FEAT_BIN_MAX && i < users.m_nCount; ++i)
    {
        pCurUser->m_FeatData = new char[512 * 1024];
        ++pCurUser;
    }
    users.m_nCount = 0;
    int MAX = USER_UP_TEXT_MAX;
    if (flag[1] == DL_DEV_USER_FEAT || flag[2] == DL_DEV_PIC)
    {
        MAX = USER_UP_PIC_BIN_MAX;
    }
    uutime total_time ;
    for (i = 0; i < 3; ++i)
    {
        if (flag[i] == 0)
        {
            continue;
        }
        DEBUG_INFO(("开始上传%s"), (flag[i] == DL_DEV_USER_TEXT) ? ("用户文字信息") : (flag[i] == DL_DEV_USER_FEAT) ? ("用户特征信息") : ("用户注册照信息"));
        int tmp_flag = flag[i];
        if (flag[i] == DL_DEV_USER_TEXT)
        {
            tmp_flag = DL_DEV_USER_TEXT;
            tmp_flag = tmp_flag | ((flag[1] == DL_DEV_USER_FEAT) ? DL_DEV_USER_FEAT : 0);
            tmp_flag = tmp_flag | ((flag[2] == DL_DEV_PIC) ? DL_DEV_PIC : 0);
        }
        pCurUser = users.m_pUserArray;
        users.m_nCount = 0;
        uutime t_t;
        for (ptr->MoveFirst(); !(ptr->IsEOF()) ; ptr->MoveNext())
        {
            DEV_USER& user = *pCurUser;
            char* buf = user.m_FeatData;
            memset(pCurUser, 0x0, sizeof(DEV_USER));
            user.m_FeatData = buf;
            memcpy(user.m_UserID.m_ID, ptr->GetCString(("UserID")).GetBuffer(0), ptr->GetCString(("UserID")).GetLength());
            memcpy(user.m_AppendID.m_ID, ptr->GetCString(("AppendID")).GetBuffer(0), ptr->GetCString(("AppendID")).GetLength());
            memcpy(user.m_ImageSID.m_ID, ptr->GetCString(("ChangeID")).GetBuffer(0), ptr->GetCString(("ChangeID")).GetLength());
            memcpy(user.m_UserName.m_ID, ptr->GetCString(("UserName")).GetBuffer(0), ptr->GetCString(("UserName")).GetLength());
            memcpy(user.m_Department.m_ID, ptr->GetCString(("Department")).GetBuffer(0), ptr->GetCString(("Department")).GetLength());
            memcpy(user.m_Admin.m_ID, ptr->GetCString(("RegAdmin")).GetBuffer(0), ptr->GetCString(("RegAdmin")).GetLength());
            user.m_RightID = ptr->GetLong(("RightID"));
            user.m_UserFlag = ptr->GetLong(("UserFlag"));
            user.m_UserType = DEV_USER_TYPE(ptr->GetLong(("UserType")));
            COleDateTime time;
            if (time.ParseDateTime(ptr->GetCString(("InvalidTime"))))
            {
                user.m_DeadLineTime.m_Date.m_Year = time.GetYear();
                user.m_DeadLineTime.m_Date.m_Month = time.GetMonth();
                user.m_DeadLineTime.m_Date.m_Day = time.GetDay();
                user.m_DeadLineTime.m_Time.m_Hour = time.GetHour();
                user.m_DeadLineTime.m_Time.m_Minute = time.GetMinute();
                user.m_DeadLineTime.m_Time.m_Second = time.GetSecond();
            }
            if (time.ParseDateTime(ptr->GetCString(("RegistTime"))))
            {
                user.m_RegistTime.m_Date.m_Year = time.GetYear();
                user.m_RegistTime.m_Date.m_Month = time.GetMonth();
                user.m_RegistTime.m_Date.m_Day = time.GetDay();
                user.m_RegistTime.m_Time.m_Hour = time.GetHour();
                user.m_RegistTime.m_Time.m_Minute = time.GetMinute();
                user.m_RegistTime.m_Time.m_Second = time.GetSecond();
            }
            if (time.ParseDateTime(ptr->GetCString(("LastUpdTime"))))
            {
                user.m_LastUpdTime.m_Date.m_Year = time.GetYear();
                user.m_LastUpdTime.m_Date.m_Month = time.GetMonth();
                user.m_LastUpdTime.m_Date.m_Day = time.GetDay();
                user.m_LastUpdTime.m_Time.m_Hour = time.GetHour();
                user.m_LastUpdTime.m_Time.m_Minute = time.GetMinute();
                user.m_LastUpdTime.m_Time.m_Second = time.GetSecond();
            }
            if (time.ParseDateTime(ptr->GetCString(("TypeUpdTime"))))
            {
                user.m_TypeUpdTime.m_Date.m_Year = time.GetYear();
                user.m_TypeUpdTime.m_Date.m_Month = time.GetMonth();
                user.m_TypeUpdTime.m_Date.m_Day = time.GetDay();
                user.m_TypeUpdTime.m_Time.m_Hour = time.GetHour();
                user.m_TypeUpdTime.m_Time.m_Minute = time.GetMinute();
                user.m_TypeUpdTime.m_Time.m_Second = time.GetSecond();
            }
            if (flag[1] == DL_DEV_USER_FEAT || flag[2] == DL_DEV_PIC)
            {
                ptr->GetChunk(("Block"), (BYTE*)buf, ptr->GetLong(("FeatBytes")) + ptr->GetLong(("PicBytes")));
                user.m_FeatLen = ptr->GetLong(("FeatBytes"));
                user.m_PicLen = ptr->GetLong(("PicBytes"));
                user.m_PicData = buf + user.m_FeatLen;
                ASSERT((user.m_FeatLen + user.m_PicLen) < 258 * 1024);
            }
            ++users.m_nCount;
            ++pCurUser;
            if (users.m_nCount == MAX)
            {
                ///DEBUG_INFO( ("%s "), ("数据上传---") );
                if (! SLG_ULUserEx(str_ip.GetBuffer(0), users, tmp_flag))
                {
                    DEBUG_INFO(("%s "), ("数据上传失败"));
                }
                users.m_nCount = 0;
                pCurUser = users.m_pUserArray;
                WaitForSingleObject(m_hEvent, INFINITE);
            }
        }
        if (users.m_nCount)
        {
            DEBUG_INFO(("%s "), ("数据上传---"));
            if (! SLG_ULUserEx(str_ip.GetBuffer(0), users, tmp_flag))
            {
                DEBUG_INFO(("%s "), ("数据上传失败"));
            }
        }
        // WaitForSingleObject(m_hEvent, INFINITE);
        DEBUG_INFO(("数据上传完成%0.2f"), t_t.elapsed());
        if (flag[i] == DL_DEV_USER_TEXT)
        {
            break;
        }
    }
    // return;
    DEBUG_INFO(("%s%0.2f"), ("数据上传完成---"), total_time.elapsed());
    pCurUser = users.m_pUserArray;
    for (int m = 0; (flag == DL_DEV_USER_FEAT || flag == DL_DEV_PIC) && * / m < USER_UP_FEAT_BIN_MAX && m < users.m_nCount ; ++m)
    {
        delete [] pCurUser->m_FeatData ;
        pCurUser->m_FeatData = NULL;
        ++pCurUser;
    }
    delete [] users.m_pUserArray;
}
//下载数据
int OnButtonDown()
{
    if (((CButton*)(GetDlgItem(IDC_CHECK_USER_EX)))->GetCheck())
    {
        DownUserEx();
        return;
    }
    CString str_ip;
    if (m_bNet)
    {
        POSITION pos = m_dev.GetFirstSelectedItemPosition();
        if (pos == NULL)
        {
            AfxMessageBox(("请选择一个设备"));
            return;
        }
        int nIndex = m_dev.GetNextSelectedItem(pos);
        str_ip = m_dev.GetItemText(nIndex, 0);
    }
    else
    {
        GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str_ip);
        if (str_ip.IsEmpty())
        {
            AfxMessageBox(("请选择一个U盘数据库"));
            return;
        }
    }
    int pic_select = ((CButton*)GetDlgItem(IDC_CHECK_PIC))->GetCheck();
    int feat_select = ((CButton*)GetDlgItem(IDC_CHECK_FEAT))->GetCheck();
    if (1 != pic_select && 1 != feat_select)
    {
        AfxMessageBox(("请选择需下载的数据"));
        return;
    }
    int flag = 0x00;
    flag |= pic_select ? DL_DEV_PIC : 0x00;
    flag |= feat_select ? DL_DEV_USER_FEAT : 0x00 ;
    int Index = m_cmbCondition.GetCurSel();
    BOOL ret = FALSE;
    // m_hTestEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    switch (Index)
    {
    case 0:
        ret = SLG_DLAllUser(str_ip.GetBuffer(0), 11);
        break;
    default:
        break;
    }
    if (!ret)
    {
        DEBUG_INFO(("%s"), ("用户数据请求下载发送失败"));
    }
    else
    {
        DEBUG_INFO(("%s"), ("用户数据请求下载发送成功"));
    }
}
int DownUserEx()
{
    CString str_ip;
    {
        POSITION pos = m_dev.GetFirstSelectedItemPosition();
        if (pos == NULL)
        {
            AfxMessageBox(("请选择一个设备"));
            return;
        }
        int nIndex = m_dev.GetNextSelectedItem(pos);
        str_ip = m_dev.GetItemText(nIndex, 0);
    }
    int pic_select = ((CButton*)GetDlgItem(IDC_CHECK_PIC))->GetCheck();
    int feat_select = ((CButton*)GetDlgItem(IDC_CHECK_FEAT))->GetCheck();
    int text_select = ((CButton*)GetDlgItem(IDC_CHECK_TEXT))->GetCheck();
    if (1 != pic_select && 1 != feat_select && 1 != text_select)
    {
        AfxMessageBox(("请选择需下载的数据"));
        return;
    }
    int flag = 0x00;
    if (text_select)
    {
        flag = DL_DEV_USER_TEXT;
        if (feat_select)
        {
            flag |= DL_DEV_USER_FEAT;
        }
        if (pic_select)
        {
            flag |= DL_DEV_PIC;
        }
    }
    else
    {
        if (feat_select)
        {
            flag = DL_DEV_USER_FEAT;
        }
        else if (pic_select)
        {
            flag = DL_DEV_PIC;
        }
    }
    BOOL ret = FALSE;
    int nCount = m_user.GetSelectedCount();
    DEV_CID* pIDArray = new DEV_CID[nCount + 1];
    DEV_CID* pTmp = pIDArray;
    {
        POSITION pos = m_user.GetFirstSelectedItemPosition();
        int nIndex = 0;
        while (pos)
        {
            memset(pTmp, 0x0, sizeof(DEV_CID));
            nIndex = m_user.GetNextSelectedItem(pos);
            strcpy(pTmp->m_ID, m_user.GetItemText(nIndex, 0));
            ++pTmp;
        }
    }
    uutime total;
    ret = SLG_DLUserEx(str_ip.GetBuffer(0), pIDArray, nCount, flag);
    if (!ret)
    {
        DEBUG_INFO(("%s"), ("用户数据请求下载发送失败"));
    }
    else
    {
        WaitForSingleObject(m_hEvent , INFINITE);
        DEBUG_INFO(("%s -- %0.2f"), ("用户数据下载完成"), total.elapsed());
    }
    delete [] pIDArray;
}
//增加用户
int OnButtonNew()
{
    // TODO: Add your control notification handler code here
    AddUserDlg dlg(m_ado);
    if (IDOK == dlg.DoModal())
    {
        char values[5] = {0};
        int items = m_user.GetItemCount();
        int n = m_user.InsertItem(items, dlg.m_ID);
        m_user.SetItemText(n, 0, dlg.m_ID);
        m_user.SetItemText(n, 1, dlg.m_Card);
        m_user.SetItemText(n, 2, dlg.m_Name);
        int R_ID = dlg.m_Right;
        sprintf(values, "%d", R_ID);
        m_user.SetItemText(n, 3, values);
        sprintf(values, "%d", dlg.m_UserType);
        m_user.SetItemText(n, 4, values);
        sprintf(values, "%d", dlg.m_userFlag);
        m_user.SetItemText(n, 5, values);
        m_user.SetItemText(n, 6, dlg.m_invalideTime);
        DEV_CID imageID = {0};
        //>>Liush =================
        // sftime st;
        // sprintf( imageID.m_ID, ("%d%d%d%d%d%d%d"), st.year(), st.month(),st.day(),st.hour(),st.minute(),st.second(),st.millis());
        //<<========================
        CString sql ;
        sql.Format(("insert into [user]([UserID], [AppendID], [UserName], [Department], [RightID], [UserType], [UserFlag], [InvalidTime], [ChangeID]) \
 values (\'%s\', \'%s\', \'%s\', \'%s\', %d, %d, %d, \'%s\', \'%s\')"),
                   dlg.m_ID, dlg.m_Card, dlg.m_Name, dlg.m_Dep, R_ID, dlg.m_UserType, dlg.m_userFlag , dlg.m_invalideTime, imageID.m_ID);
        m_ado.ExecuteNoSelectSQL(sql);
        UpdateData(FALSE);
    }
}
//删除用户
int OnButtonDel()
{
    // TODO: Add your control notification handler code here
    if (((CButton*)(GetDlgItem(IDC_CHECK_USER_EX)))->GetCheck())
    {
        DelUserEx();
        return;
    }
    int local_select = ((CButton*)GetDlgItem(IDC_CHECK_LOCAL))->GetCheck();
    int dsp_select = ((CButton*)GetDlgItem(IDC_CHECK_DSP))->GetCheck();
    if (local_select != 1 && dsp_select != 1)
    {
        return;
    }
    POSITION pos = m_user.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        return;
    }
    int i = 0 ;
    while (pos)
    {
        int nIndex = m_user.GetNextSelectedItem(pos);
        CString str_id = m_user.GetItemText(nIndex, 0);
        if (local_select)
        {
            CString str_sql;
            str_sql.Format(("delete from [user] where ( [UserID] = \'%s\')"), str_id.GetBuffer(0));
            m_ado.ExecuteNoSelectSQL(str_sql);
            m_user.DeleteItem(nIndex);
            int items = m_user.GetItemCount();
            if (items > 0)
            {
                m_dev.SetItemState(items - 1, LVIS_SELECTED, LVIS_SELECTED);
                ShowImage(items - 1);
            }
            pos = m_user.GetFirstSelectedItemPosition();
        }
        if (dsp_select)
        {
            CString str_ip;
            if (m_bNet)
            {
                POSITION pos = m_dev.GetFirstSelectedItemPosition();
                if (pos == NULL)
                {
                    continue;
                }
                int nIndex = m_dev.GetNextSelectedItem(pos);
                str_ip = m_dev.GetItemText(nIndex, 0);
            }
            else
            {
                GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str_ip);
                if (str_ip.IsEmpty())
                {
                    continue;
                }
            }
            DEV_CID user_id = {0};
            memcpy(user_id.m_ID, str_id.GetBuffer(0), str_id.GetLength());
            if (SLG_DELUser(str_ip.GetBuffer(0), user_id))
            {
                DEBUG_INFO(("%s 删除请求发送成功"), str_id.GetBuffer(0));
            }
            else
            {
                DEBUG_INFO(("%s 删除请求发送失败"), str_id.GetBuffer(0));
            }
        }
        ++i;
    }
    DEBUG_INFO(("删除请求发送完成 %d"), i);
}
int DelUserEx()
{
    POSITION pos = m_dev.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        return;
    }
    int nIndex = m_dev.GetNextSelectedItem(pos);
    CString str_ip = m_dev.GetItemText(nIndex, 0);
    int nCount = m_user.GetSelectedCount();
    DEV_CID* pIDArray = new DEV_CID[nCount + 1];
    DEV_CID* pTmp = pIDArray;
    {
        POSITION pos = m_user.GetFirstSelectedItemPosition();
        int nIndex = 0;
        while (pos)
        {
            memset(pTmp, 0x0, sizeof(DEV_CID));
            nIndex = m_user.GetNextSelectedItem(pos);
            strcpy(pTmp->m_ID, m_user.GetItemText(nIndex, 0));
            ++pTmp;
        }
    }
    int res = SLG_DELUserEx(str_ip.GetBuffer(0), pIDArray, nCount);
    DEBUG_INFO(("删除请求发送完成 %d"), res);
    delete [] pIDArray;
}
int OnLClickUserList(NMHDR* pNotifyStruct, LRESULT* result)
{
    *result = CDRF_DODEFAULT;
    NMCLICK* pclick = (NMCLICK*)pNotifyStruct;
    if (NULL == pclick || (pclick->dwItemSpec) < 0 || (pclick->dwItemSpec) > m_user.GetItemCount())
    {
        return;
    }
    int nIndex = pclick->dwItemSpec;
    ShowImage(nIndex);
}
int OnLClickRecordList(NMHDR* pNotifyStruct, LRESULT* result)
{
    *result = CDRF_DODEFAULT;
    NMCLICK* pclick = (NMCLICK*)pNotifyStruct;
    if (NULL == pclick || (pclick->dwItemSpec) < 0 || (pclick->dwItemSpec) > m_record.GetItemCount())
    {
        return;
    }
    int nIndex = pclick->dwItemSpec;
    CString str_id = m_record.GetItemText(nIndex, 0);
    ShowRecordImage(nIndex, str_id);
}
int OnLClickCImageList(NMHDR* pNotifyStruct, LRESULT* result)
{
    *result = CDRF_DODEFAULT;
    NMCLICK* pclick = (NMCLICK*)pNotifyStruct;
    if (NULL == pclick || (pclick->dwItemSpec) < 0 || (pclick->dwItemSpec) > m_image.GetItemCount())
    {
        return;
    }
    int nIndex = pclick->dwItemSpec;
    CString str_id = m_image.GetItemText(nIndex, 0);
    ShowRecordImage(nIndex, str_id);
}
int ShowImage(int index)
{
    CString str_id = m_user.GetItemText(index, 0);
    CString str_sql;
    str_sql.Format(("select [FeatBytes], [PicBytes], [Block] from [user] where ([UserId] = \'%s\')"), str_id);
    CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(str_sql);
    if (ptr->IsEmpty())
    {
        return;
    }
    int FeatBytes = ptr->GetLong(("FeatBytes"));
    int PicBytes = ptr->GetLong(("PicBytes"));
    unsigned int len = FeatBytes + PicBytes;
    if (PicBytes <= 0)
    {
        return;
    }
    char* buf = new char[FeatBytes + PicBytes];
    memset(buf, 0x0, FeatBytes + PicBytes);
    ptr->GetChunk(("Block"), (BYTE*)buf, len);
    CDC* pDC = m_static.GetDC();
    RECT rect;
    m_static.GetClientRect(&rect);
    CPicture pic;
    pic.LoadPictureData((BYTE*)(buf + FeatBytes), PicBytes);
    pic.UpdateSizeOnDC(pDC);
    pic.Show(pDC, rect);
    pic.FreePictureData();
    delete [] buf;
}
int ShowRecordImage(int index, const CString& str_id)
{
    CString str_sql;
    str_sql.Format(("select [PicBytes], [Block] from [record] where ([SerialID] = \'%s\')"), str_id);
    CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(str_sql);
    if (ptr->IsEmpty())
    {
        return;
    }
    unsigned int len = ptr->GetLong(("PicBytes"));
    if (len <= 0)
    {
        return;
    }
    char* buf = new char[len];
    memset(buf, 0x0, len);
    ptr->GetChunk(("Block"), (BYTE*)buf, len);
    CDC* pDC = m_staticRecord.GetDC();
    RECT rect;
    m_staticRecord.GetClientRect(&rect);
    CPicture pic;
    pic.LoadPictureData((BYTE*)(buf), len);
    pic.UpdateSizeOnDC(pDC);
    pic.Show(pDC, rect);
    pic.FreePictureData();
    delete [] buf;
}
int OnTimer(UINT nIDEvent)
{
    if (nIDEvent == SERCH_EVENT_TIME)
    {
        // BOOL ret = SLG_StartDevSerch(TRUE, m_IP);//SERCH_DEVICE(true);
        // DEBUG_INFO( ("搜索服务器 %d"), ret );
    }
    if (nIDEvent == CLEARLCLICKTIME)
    {
        m_nLClickTimes = 0;
    }
}
int OnButtonPic()
{
    // TODO: Add your control notification handler code here
    CString str_ip;
    POSITION pos;
    int nIndex;
    if (m_bNet)
    {
        pos = m_dev.GetFirstSelectedItemPosition();
        if (pos == NULL)
        {
            AfxMessageBox(("请选择一个设备"));
            return;
        }
        nIndex = m_dev.GetNextSelectedItem(pos);
        str_ip = m_dev.GetItemText(nIndex, 0);
    }
    else
    {
        GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str_ip);
        if (str_ip.IsEmpty())
        {
            AfxMessageBox(("请选择一个U盘数据库"));
            return;
        }
    }
    pos = m_user.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(("请选择一条记录"));
        return;
    }
    nIndex = m_dev.GetNextSelectedItem(pos);
    CString str_id = m_user.GetItemText(nIndex, 0);
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "jpg Files (*.jpg)|*.jpg||");
    if (dlg.DoModal() != IDOK)
    {
        return;
    }
    CString str_sql;
    str_sql.Format(("select * from [user] where ([UserId] = \'%s\')"), str_id);
    CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(str_sql);
    if (ptr->IsEmpty())
    {
        return;
    }
    DEV_USER user = {0};
    memcpy(user.m_UserID.m_ID, ptr->GetCString(("UserID")).GetBuffer(0), ptr->GetCString(("UserID")).GetLength());
    memcpy(user.m_AppendID.m_ID, ptr->GetCString(("AppendID")).GetBuffer(0), ptr->GetCString(("AppendID")).GetLength());
    //>>Liush 2011-04-03管理端上传用户，带上了注册照，
    // 1.如果是自己生成的注册照，必须使用此图片生成的ImageSID---ChangeID
    // 2.如果此照片是从DSP端down下来，
    // 其实也就相当于每一张注册照?急匦攵杂ξㄒ坏腟ID，如果是管理端生成注册照，则管理端生成SID
    // DSP端生成的注册照，则由DSP端生成SID
    memcpy(user.m_ImageSID.m_ID, ptr->GetCString(("ChangeID")).GetBuffer(0), ptr->GetCString(("ChangeID")).GetLength());
    memcpy(user.m_UserName.m_ID, ptr->GetCString(("UserName")).GetBuffer(0), ptr->GetCString(("UserName")).GetLength());
    memcpy(user.m_Department.m_ID, ptr->GetCString(("Department")).GetBuffer(0), ptr->GetCString(("Department")).GetLength());
    memcpy(user.m_Admin.m_ID, ptr->GetCString(("RegAdmin")).GetBuffer(0), ptr->GetCString(("RegAdmin")).GetLength());
    user.m_UserType = DEV_USER_TYPE(ptr->GetLong(("UserType")));
    user.m_RightID = ptr->GetLong(("RightID"));
    user.m_UserFlag |= ptr->GetLong(("UserFlag"));
    COleDateTime time;
    time.ParseDateTime(ptr->GetCString(("InvalidTime")));
    user.m_DeadLineTime.m_Date.m_Year = time.GetYear();
    user.m_DeadLineTime.m_Date.m_Month = time.GetMonth();
    user.m_DeadLineTime.m_Date.m_Day = time.GetDay();
    user.m_DeadLineTime.m_Time.m_Hour = time.GetHour();
    user.m_DeadLineTime.m_Time.m_Minute = time.GetMinute();
    user.m_DeadLineTime.m_Time.m_Second = time.GetSecond();
    time.ParseDateTime(ptr->GetCString(("RegistTime")));
    user.m_RegistTime.m_Date.m_Year = time.GetYear();
    user.m_RegistTime.m_Date.m_Month = time.GetMonth();
    user.m_RegistTime.m_Date.m_Day = time.GetDay();
    user.m_RegistTime.m_Time.m_Hour = time.GetHour();
    user.m_RegistTime.m_Time.m_Minute = time.GetMinute();
    user.m_RegistTime.m_Time.m_Second = time.GetSecond();
    time.ParseDateTime(ptr->GetCString(("LastUpdTime")));
    user.m_LastUpdTime.m_Date.m_Year = time.GetYear();
    user.m_LastUpdTime.m_Date.m_Month = time.GetMonth();
    user.m_LastUpdTime.m_Date.m_Day = time.GetDay();
    user.m_LastUpdTime.m_Time.m_Hour = time.GetHour();
    user.m_LastUpdTime.m_Time.m_Minute = time.GetMinute();
    user.m_LastUpdTime.m_Time.m_Second = time.GetSecond();
    time.ParseDateTime(ptr->GetCString(("TypeUpdTime")));
    user.m_TypeUpdTime.m_Date.m_Year = time.GetYear();
    user.m_TypeUpdTime.m_Date.m_Month = time.GetMonth();
    user.m_TypeUpdTime.m_Date.m_Day = time.GetDay();
    user.m_TypeUpdTime.m_Time.m_Hour = time.GetHour();
    user.m_TypeUpdTime.m_Time.m_Minute = time.GetMinute();
    user.m_TypeUpdTime.m_Time.m_Second = time.GetSecond();
    char block[192 * 1024] = {0};
    ptr->GetChunk(("Block"), (BYTE*)block, ptr->GetLong(("FeatBytes")) + ptr->GetLong(("PicBytes")));
    int PicBytes = 0;
    CString str_path = dlg.GetPathName();
    FILE* pfile = fopen(str_path.GetBuffer(0), "rb");
    if (pfile)
    {
        fseek(pfile, 0, SEEK_END);
        PicBytes = ftell(pfile);
        if (PicBytes > 50 * 1024)
        {
            fclose(pfile);
            return ;
        }
        fseek(pfile, 0, SEEK_SET);
        fread(block + ptr->GetLong(("FeatBytes")), 1, PicBytes, pfile);
        fclose(pfile);
        ptr->SetLong(("PicBytes") , PicBytes);
        //>>Liush 用户自己上传注册照时，修改在源数据表中的ChangeID字段以及，将要被发送出去的user.m_ImageSID.m_ID 内容
        sftime st;
        sprintf(user.m_ImageSID.m_ID, ("%d%d%d%d%d%d%d"), st.year(), st.month(), st.day(), st.hour(), st.minute(), st.second(), st.millis());
        CString newChangeID = user.m_ImageSID.m_ID;
        ptr->SetCString(("ChangeID") , newChangeID);
        ptr->AppendChunk(("Block"), (BYTE*)block, ptr->GetLong(("FeatBytes")) + PicBytes);
        ptr->Update();
    }
    user.m_FeatLen = ptr->GetLong(("FeatBytes"));
    user.m_PicLen = ptr->GetLong(("PicBytes"));
    user.m_FeatData = block;
    user.m_PicData = block + user.m_FeatLen;
    if (!SLG_ULUser(str_ip.GetBuffer(0), user))
    {
        DEBUG_INFO(("%s 数据上传失败"), str_id.GetBuffer(0));
    }
    else
    {
        DEBUG_INFO(("%s 数据上传成功"), str_id.GetBuffer(0));
    }
}
int OnButton1()
{
    // TODO: Add your control notification handler code here
#define FEATURE_LEN 4330
    typedef struct
    {
        char m_Feature[ FEATURE_LEN ];
        DEV_CID m_PID;
        unsigned char m_Flag;
    } SPFeatInfo;
    CString str_sql = ("select [UserName],[RegistTime], [RegAdmin],[FeatBytes],[PicBytes],[Block] from [user] where ([UserId] = \'3005\')") ;
    CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(str_sql);
    if (ptr->IsEmpty())
    {
        return;
    }
    unsigned int FeatBytes = ptr->GetLong(("FeatBytes"));
    unsigned int PicBytes = ptr->GetLong(("PicBytes"));
    CString str_name = ptr->GetCString(("UserName"));
    CString str_RegTime = ptr->GetCString(("RegistTime"));
    CString str_RegAdmin = ptr->GetCString(("RegAdmin"));
    char* buf = new char[FeatBytes + PicBytes + 1024];
    memset(buf, 0x0, FeatBytes + PicBytes + 1024);
    ptr->GetChunk(("Block"), (BYTE*)buf, FeatBytes + PicBytes);
    str_sql = ("select [UserId], [UserName],[RegistTime],[FeatBytes],[RegAdmin],[PicBytes],[Block] from [user] where ([UserId] >= \'7001\')") ;
    ptr = m_ado.ExecuteSelectSQL(str_sql);
    if (ptr->IsEmpty())
    {
        return;
    }
    ptr->MoveFirst();
    CString str_id;
    for (; !(ptr->IsEOF()); ptr->MoveNext())
    {
        str_id = ptr->GetCString(("UserId"));
        ptr->SetLong(("FeatBytes"), FeatBytes);
        ptr->SetLong(("PicBytes"), PicBytes);
        ptr->SetCString(("RegistTime"), str_RegTime);
        ptr->SetCString(("RegAdmin"), str_RegAdmin);
        SPFeatInfo* p = (SPFeatInfo*)buf;
        for (int i = 0 ; i < 20; ++i)
        {
            memset(&(p->m_PID), 0x0, sizeof(DEV_CID));
            sprintf(p->m_PID.m_ID, "%s", str_id);
            p++;
        }
        ptr->AppendChunk(("Block"), (BYTE*)buf, FeatBytes + PicBytes);
        ptr->Update();
    }
    DEBUG_INFO(("%s"), ("更改完成"));
}
int OnButtonDlrecord()
{
    // TODO: Add your control notification handler code here
    POSITION pos;
    CString str_ip;
    if (m_bNet)
    {
        pos = m_dev.GetFirstSelectedItemPosition();
        if (pos == NULL)
        {
            AfxMessageBox(("请选择一个设备"));
            return;
        }
        int n = m_dev.GetNextSelectedItem(pos);
        str_ip = m_dev.GetItemText(n, 0);
    }
    else
    {
        GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str_ip);
        if (str_ip.IsEmpty())
        {
            AfxMessageBox(("请选择一个U盘数据库"));
            return;
        }
    }
    int pic_select = ((CButton*)GetDlgItem(IDC_CHECK_RECORD_PIC))->GetCheck();
    int flag = 0x00;
    flag |= pic_select;
    int nIndex = m_cmbRecordCondition.GetCurSel();
    BOOL ret = FALSE;
    switch (nIndex)
    {
    case 0:
        ret = SLG_DLAllIdentifyRecord(str_ip.GetBuffer(0));
        break;
    case 1:
        // ret = SLG_DLNewIdentifyRecord( str_ip.GetBuffer(0) );
        break;
    case 2:
        ret = SLG_DLAllAlarmRecord(str_ip.GetBuffer(0));
        break;
    case 3:
    {
        UpdateData(TRUE);
        DEV_DATE start = {m_STimeRec.GetYear(), m_STimeRec.GetMonth(), m_STimeRec.GetDay()};
        DEV_DATE end = {m_ETimeRec.GetYear(), m_ETimeRec.GetMonth(), m_ETimeRec.GetDay()};
        /// ret = SLG_DLSegTimeIdentifyRecord( str_ip.GetBuffer(0), start, end);
    }
    break;
    case 4:
        // ret = SLG_DLNewAlarmRecord( str_ip.GetBuffer(0) );
        break;
    case 5:
    {
        UpdateData(TRUE);
        DEV_DATE start = {m_STimeRec.GetYear(), m_STimeRec.GetMonth(), m_STimeRec.GetDay()};
        DEV_DATE end = {m_ETimeRec.GetYear(), m_ETimeRec.GetMonth(), m_ETimeRec.GetDay()};
        // ret = SLG_DLSegTimeAlarmRecord( str_ip.GetBuffer(0), start, end );
    }
    break;
    case 6:
    {
        UpdateData(TRUE);
        DEV_DL_RECORD_RANGE range = {0};
        CString count ;
        GetDlgItem(IDC_EDIT_COUNT)->GetWindowText(count);
        range.m_Count = atoi(count.GetBuffer(0));
        CString id ;
        GetDlgItem(IDC_EDIT_ID)->GetWindowText(id);
        memcpy(range.m_SID.m_ID, id.GetBuffer(0), id.GetLength());
        //ret = SLG_GetRangeIdentifyRecCount( m_serverIP, str_ip.GetBuffer(0), range, flag );
        ret = SLG_DLRangeRec(str_ip.GetBuffer(0), range, flag);
    }
    break;
    case 7:
    {
        UpdateData(TRUE);
        DEV_DL_RECORD_RANGE range = {0};
        CString count ;
        GetDlgItem(IDC_EDIT_COUNT)->GetWindowText(count);
        range.m_Count = atoi(count.GetBuffer(0));
        CString id ;
        GetDlgItem(IDC_EDIT_ID)->GetWindowText(id);
        memcpy(range.m_SID.m_ID, id.GetBuffer(0), id.GetLength());
        //ret = SLG_GetRangeAlarmRecCount( m_serverIP, str_ip.GetBuffer(0), range, flag );
        ret = SLG_DLRangeAlarm(str_ip.GetBuffer(0), range, flag);
    }
    break;
    default:
        break;
    }
    if (!ret)
    {
        DEBUG_INFO(("%s"), ("记录请求下载发送失败"));
    }
    else
    {
        DEBUG_INFO(("%s"), ("记录请求下载发送成功"));
    }
}
int OnButtonDelrecord()
{
    // TODO: Add your control notification handler code here
    CString str_sql(("delete from [record]"));
    m_ado.ExecuteNoSelectSQL(str_sql);
    CTreeCtrl_DeleteAllItems(aa.m_hWnd);
}
#define USERNAME ("admin")
#define PASSWORD ("201031")
int DevConnectNotify(const char* dev_addr, DEV_CONNECT_STATUS flag)
{
    if (!m_bNet)
    {
        return -1;
    }
    char ip[16] = {0};
    BYTE field0 = 0, field1 = 0, field2 = 0, field3 = 0;
    m_IPServer.GetAddress(field0, field1, field2, field3);
    sprintf(ip, "%d.%d.%d.%d", field0, field1, field2, field3);
    switch (flag)
    {
    case DEV_CONNECT_NONE:
    {
        SLG_CNDev(dev_addr);
    }
    break;
    case DEV_CONNECT_FAILUE:
        DEBUG_INFO(("%s 连接失败"), dev_addr);
        break;
    case DEV_CONNECT_SUCCESS:
    {
        DEBUG_INFO(("%s 建立连接"), dev_addr);
        DEV_CID user = {0};
        DEV_CID pswd = {0};
        memcpy(user.m_ID , USERNAME , sizeof(USERNAME));
        memcpy(pswd.m_ID , PASSWORD , sizeof(PASSWORD));
        SLG_ULDevAuth(dev_addr, user, pswd, 1);
    }
    break;
    case DEV_CONNECT_CUT:
    {
        SLG_DCNDev(dev_addr);
        int items = m_dev.GetItemCount();
        CString str_ip(dev_addr);
        CString temp ;
        for (int i = 0; i < items; ++i)
        {
            temp = m_dev.GetItemText(i, 0);
            if (str_ip == temp)
            {
                m_dev.DeleteItem(i);
                break;
            }
        }
        DEBUG_INFO(("%s 断开连接"), dev_addr);
    }
    break;
    case DEV_QUERY_ENCRYPT:
    {
        //when we connect to an unit, the SDK will ask whether or not to encrypt through this callback.
        //0: don't encrypt
        //1: need encrypt and it is server-side
        //2: need encrypt and it is client-side
        return ((CButton*)GetDlgItem(IDC_CHECK_SSL))->GetCheck() ? 2 : 0;
    }
    break;
    case SRV_QUERY_ENCRYPT:
    {
        //when we connect to transit server, the SDK will ask whether or not to encrypt through this callback.
        //0: don't encrypt
        //1: need encrypt and it is server-side
        //2: need encrypt and it is client-side
    }
    return 0;
    default:
        break;
    }
    return 0;
}
#endif
int DevWpaInfoOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if (pvContent)
    {
        WAPINFO* wap = (WAPINFO*)pvContent;
        DEBUG_INFO("%s isopen=%d username=%s password=%s\n", cszDevAddr, wap->isopen, wap->username, wap->password);
    }
    else
    {
        DEBUG_INFO("DevWpaInfoOper eResult %d ok\n", eResult);
    }
    return 0;
}
int DevBaseInfoOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if (OPER_SUCCESS == eResult && pvContent)
    {
        DEV_BASEINFO* info;
        info = ((DEV_BASEINFO*)pvContent);
        int items = strlist_count(m_dev);
        strlist_insert_fmt(&m_dev, items, "%s|%d|%d", cszDevAddr, info->m_DevType, info->m_LimitUser);
        DEBUG_INFO("%s 获取设备信息成功", cszDevAddr);
        printf("cszDevAddr=%s|m_DevTy=%d|m_LimitUser=%d\n", cszDevAddr, info->m_DevType, info->m_LimitUser);
    }
    return 0;
}
int DevNetProOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if (OPER_SUCCESS == eResult && pvContent)
    {
        DEV_NETPRO* info;
        info = ((DEV_NETPRO*)pvContent);
        DEBUG_INFO("%s 获取设备网络属性成功", cszDevAddr);
        printf("cszDevAddr=%s|m_WebUrl1=%s|m_WebUrl2=%s|m_IPAdr=%s|m_GateWay=%s\n", cszDevAddr, info->m_WebUrl1, info->m_WebUrl2, info->m_IPAdr, info->m_GateWay);
    }
    return 0;
}
int DevStatisOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if (OPER_SUCCESS == eResult && pvContent)
    {
        DEV_STATIS* dev_sinfo;
        dev_sinfo = ((DEV_STATIS*)pvContent);
        DEBUG_INFO("%s 获取设备统计信息", cszDevAddr);
        printf("cszDevAddr=%s|总用户数=%d|总记录数=%d\n", cszDevAddr, dev_sinfo->m_TotalUsers, dev_sinfo->m_TotalRecords);
    }
    return 0;
}
int DevUserInfoOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if (OPER_SUCCESS == eResult && pvContent && (DEV_USER_OPER == eType))
    {
        if (DEV_OPER_DELETE == eFlag)
        {
            DEV_USER* dev_user = ((DEV_USER*)pvContent);
            DEBUG_INFO("eResult == %d , 用户删除 ID == %s", eResult, dev_user->m_UserID.m_ID);
        }
        if (DEV_OPER_DOWNLOAD == eFlag)
        {
            DEV_USER* dev_user = ((DEV_USER*)pvContent);
            // DEBUG_INFO( ("%s 用户信息下载完成%d"), dev_user->m_UserID.m_ID, dev_user->m_bRTUser );
            // return ;
#if 0
            char sql;
            sql.Format(("select * from [user] where ([UserID]=\'%s\')"), dev_user->m_UserID.m_ID);
            CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(sql);
            if (ptr->IsEmpty())
            {
                ptr->NewRecord();
                ptr->SetCString(("UserID"), dev_user->m_UserID.m_ID);
                DEV_USER* pUser = new DEV_USER;
                *pUser = *((DEV_USER*)pvContent);
                //this->PostMessage(USER_MSG, DotIpToLong(cszDevAddr), (LPARAM)pUser);
                //LRESULT DevUserMsg(WPARAM wparam, LPARAM lpparam)
                {
                    char values[100] = {0};
                    int items = m_user.GetItemCount();
                    int n = m_user.InsertItem(items, dev_user->m_UserID.m_ID);
                    m_user.SetItemText(n, 0, dev_user->m_UserID.m_ID);
                    m_user.SetItemText(n, 1, dev_user->m_AppendID.m_ID);
                    m_user.SetItemText(n, 2, dev_user->m_UserName.m_ID);
                    int R_ID = dev_user->m_RightID;
                    sprintf(values, "%d", R_ID);
                    m_user.SetItemText(n, 3, values);
                    sprintf(values, "%d", dev_user->m_UserType);
                    m_user.SetItemText(n, 4, values);
                    sprintf(values, "%d", dev_user->m_UserFlag);
                    m_user.SetItemText(n, 5, values);
                    sprintf(values, "%02d/%02d/%04d %02d:%02d:%02d", dev_user->m_DeadLineTime.m_Date.m_Month,
                            dev_user->m_DeadLineTime.m_Date.m_Day,
                            dev_user->m_DeadLineTime.m_Date.m_Year,
                            dev_user->m_DeadLineTime.m_Time.m_Hour,
                            dev_user->m_DeadLineTime.m_Time.m_Minute,
                            dev_user->m_DeadLineTime.m_Time.m_Second
                           );
                    m_user.SetItemText(n, 6, values);
                    m_user.SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
                    delete pvContent;
                }
            }
            if (!(ptr->IsEmpty()))
            {
                //#if 0
                ptr->SetCString(("AppendID"), dev_user->m_AppendID.m_ID);
                ptr->SetCString(("UserName"), dev_user->m_UserName.m_ID);
                ptr->SetCString(("Department"), dev_user->m_Department.m_ID);
                ptr->SetLong(("RightID"), dev_user->m_RightID);
                ptr->SetCString(("ChangeID"), dev_user->m_ImageSID.m_ID);
                ptr->SetCString(("RegAdmin"), dev_user->m_Admin.m_ID);
                ptr->SetLong(("UserType") , dev_user->m_UserType);
                ptr->SetLong(("UserFlag"), dev_user->m_UserFlag);
                //#endif
                ptr->SetLong(("FeatBytes"), dev_user->m_FeatLen);
                ptr->SetLong(("PicBytes") , dev_user->m_PicLen);
                //#if 0
                char values[100] = {0};
                sprintf(values, "%02d/%02d/%04d %02d:%02d:%02d", dev_user->m_RegistTime.m_Date.m_Month,
                        dev_user->m_RegistTime.m_Date.m_Day,
                        dev_user->m_RegistTime.m_Date.m_Year,
                        dev_user->m_RegistTime.m_Time.m_Hour,
                        dev_user->m_RegistTime.m_Time.m_Minute,
                        dev_user->m_RegistTime.m_Time.m_Second
                       );
                ptr->SetCString(("RegistTime"), values);
                sprintf(values, "%02d/%02d/%04d %02d:%02d:%02d", dev_user->m_LastUpdTime.m_Date.m_Month,
                        dev_user->m_LastUpdTime.m_Date.m_Day,
                        dev_user->m_LastUpdTime.m_Date.m_Year,
                        dev_user->m_LastUpdTime.m_Time.m_Hour,
                        dev_user->m_LastUpdTime.m_Time.m_Minute,
                        dev_user->m_LastUpdTime.m_Time.m_Second
                       );
                ptr->SetCString(("LastUpdTime"), values);
                sprintf(values, "%02d/%02d/%04d %02d:%02d:%02d", dev_user->m_DeadLineTime.m_Date.m_Month,
                        dev_user->m_DeadLineTime.m_Date.m_Day,
                        dev_user->m_DeadLineTime.m_Date.m_Year,
                        dev_user->m_DeadLineTime.m_Time.m_Hour,
                        dev_user->m_DeadLineTime.m_Time.m_Minute,
                        dev_user->m_DeadLineTime.m_Time.m_Second
                       );
                ptr->SetCString(("InvalidTime"), values);
                sprintf(values, "%02d/%02d/%04d %02d:%02d:%02d", dev_user->m_TypeUpdTime.m_Date.m_Month,
                        dev_user->m_TypeUpdTime.m_Date.m_Day,
                        dev_user->m_TypeUpdTime.m_Date.m_Year,
                        dev_user->m_TypeUpdTime.m_Time.m_Hour,
                        dev_user->m_TypeUpdTime.m_Time.m_Minute,
                        dev_user->m_TypeUpdTime.m_Time.m_Second
                       );
                ptr->SetCString(("TypeUpdTime"), values);
                char block[192 * 1024] = {0};
                memcpy(block, dev_user->m_FeatData, dev_user->m_FeatLen);
                memcpy(block + dev_user->m_FeatLen, dev_user->m_PicData, dev_user->m_PicLen);
                ptr->AppendChunk(("Block"), (BYTE*)block, dev_user->m_FeatLen + dev_user->m_PicLen);
                ptr->Update();
            }
#endif
        }
    }
    return 0;
}
int DevUserRemoteRecap(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    DEV_RCAPINFO* info = (DEV_RCAPINFO*)pvContent;
    DEBUG_INFO("DevUserRemoteRecap:: %d %d %s", info->m_Count, info->m_Total, info->m_UserID.m_ID);
    return 0;
}
int DevUserExOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if ((OPER_SUCCESS == eResult || eResult == OPER_BATCH_DL_COMPLETE) && pvContent)
    {
        switch (eFlag)
        {
        case DEV_OPER_UPLOAD:
        {
            DEBUG_INFO("%s ", "数据上传完成");
            DEV_BATCH_USER* pUsers = (DEV_BATCH_USER*)pvContent;
            DEV_USER* pUser = pUsers->m_pUserArray;
            int i = 0;
            for (i = 0; i < pUsers->m_nCount; ++i)
            {
                DEBUG_INFO("用户%s上传完成", pUser->m_UserID.m_ID);
                ++pUser;
            }
        }
        break;
        case DEV_OPER_DOWNLOAD:
        {
            DEV_BATCH_USER* pUsers = (DEV_BATCH_USER*)pvContent;
            DEBUG_INFO("%s%d", "数据下载完成xxx", pUsers->m_nCount);
            if (eResult == OPER_BATCH_DL_COMPLETE)
            {
            }
            // return;
            DEV_USER* pUser = pUsers->m_pUserArray;
            int i = 0;
            for (i = 0; i < pUsers->m_nCount; ++i)
            {
                DEBUG_INFO(("用户%s, pic=%d, feat=%d下载完成"), pUser->m_UserID.m_ID, pUser->m_PicLen, pUser->m_FeatLen);
                DevUserInfoOper(cszDevAddr, DEV_USER_OPER , DEV_OPER_DOWNLOAD, pUser, nSeq , OPER_SUCCESS);
                ++pUser;
            }
        }
        break;
        case DEV_OPER_DELETE:
        {
            DEV_BATCH_USER* pUsers = (DEV_BATCH_USER*)pvContent;
            DEV_USER* pUser = pUsers->m_pUserArray;
            int i = 0;
            for (i = 0; i < pUsers->m_nCount; ++i)
            {
                DEBUG_INFO(("用户%s删除完成"), pUser->m_UserID.m_ID);
                ++pUser;
            }
        }
        }
    }
    return 0;
}
int DevLogInfoOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if (OPER_SUCCESS == eResult && pvContent && DEV_LOG_OPER == eType)
    {
        DEV_LOG* log = ((DEV_LOG*)pvContent);
        int items = strlist_count(m_log);
        const char* rec_type = NULL;
        switch (log->m_LogType)
        {
        case DEV_LOG_ADDUSER: //
            rec_type = "增加用户";
            break;
        case DEV_LOG_DELUSER: //
            rec_type = "删除用户";
            break;
        case DEV_LOG_REREGIST:
            rec_type = "重新采集";
            break;
        case DEV_LOG_CAHNGETYPE:
            rec_type = "改变用户类型";
            break;
        case DEV_UDISK_ULUSER: //
            rec_type = "U盘上传用户";
            break;
        case DEV_UDISK_DLUSER: //
            rec_type = "U盘下载用户";
            break;
        case DEV_UDISK_DLRECORD: //
            rec_type = "U盘下载记录";
            break;
        case DEV_UDISK_UPDATEAPP: //
            rec_type = "U盘更新程序";
            break;
        default:
            break;
        }
        char time[32];
        sprintf(time, "%02d/%02d/%04d %02d:%02d:%02d", log->m_RecTime.m_Date.m_Month,
                log->m_RecTime.m_Date.m_Day,
                log->m_RecTime.m_Date.m_Year,
                log->m_RecTime.m_Time.m_Hour,
                log->m_RecTime.m_Time.m_Minute,
                log->m_RecTime.m_Time.m_Second
               );
        strlist_insert_fmt(&m_log, items, "%s|%s|%s|%s|%s", log->m_SerialID.m_ID, rec_type, log->m_Admin.m_ID, log->m_BeOptUser.m_ID, time);
    }
    return 0;
}
int DevLogExOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if ((OPER_SUCCESS == eResult || eResult == OPER_BATCH_DL_COMPLETE) && pvContent)
    {
        DEV_BATCH_LOG* pRecords = (DEV_BATCH_LOG*)pvContent;
        DEV_LOG* pRecord = pRecords->m_pLogArray;
        for (int i = 0; i < pRecords->m_nCount; ++i)
        {
            DevLogInfoOper(cszDevAddr, eType , eFlag , pRecord, nSeq, OPER_SUCCESS);
            ++pRecord;
        }
    }
    return 0;
}
LONG count = 0;
#define DEVRECORDTYPEDEF_DEF(DEVRECORDTYPEDEF) \
  DEVRECORDTYPEDEF(DEV_VERIFY_SUCC, "验证成功记录") \
  DEVRECORDTYPEDEF( DEV_VERIFY_FAIL, "验证失败记录") \
  DEVRECORDTYPEDEF( DEV_ADMIN_SUCC, "管理员验证成功记录") \
  DEVRECORDTYPEDEF( DEV_EMER_OPEN, "紧急开门记录") \
  DEVRECORDTYPEDEF( DEV_RIGHT_OPEN, "权限开门") \
  DEVRECORDTYPEDEF( DEV_GROUP_OPEN, "组合开门") \
  DEVRECORDTYPEDEF( DEV_BUTTON_OPEN, "按钮开门") \
  DEVRECORDTYPEDEF( DEV_START_VERIFY, "开始验证") \
  DEVRECORDTYPEDEF( DEV_STOP_VERIFY, "停止验证") \
  DEVRECORDTYPEDEF( DEV_ALARM_HD_MANGET_TIMEOUT, "门磁超时") \
  DEVRECORDTYPEDEF( DEV_ALARM_HD_MANGET_ILLOPEN, "门磁非法开门") \
  DEVRECORDTYPEDEF( DEV_ALARM_HD_OFFLINE, "前端掉线报警") \
  DEVRECORDTYPEDEF( DEV_ALARM_HD_BREAK, "防拆报警") \
  DEVRECORDTYPEDEF( DEV_ALARM_HD_SHOCK, "震动报警") \
  DEVRECORDTYPEDEF( DEV_ALARM_HD_FPOWR, "前端供电异常报警") \
  DEVRECORDTYPEDEF( DEV_ALARM_HD_UPS_ON, "UPS备用电池开启") \
  DEVRECORDTYPEDEF( DEV_ALARM_HD_UPS_OFF, "UPS备用电池关闭") \
  DEVRECORDTYPEDEF( DEV_ALARM_HD_ASSIST, "辅助输入触发") \
  DEVRECORDTYPEDEF( DEV_ALARM_SF_BACKLIST, "黑名单验证报警") \
  DEVRECORDTYPEDEF( DEV_ALARM_SF_ILLCARD, "无效卡(挂失)") \
  DEVRECORDTYPEDEF( DEV_ALARM_SF_ILLTIME, "非法时间识别") \
  DEVRECORDTYPEDEF( DEV_ALARM_SF_DEADLINE, "失效时间") \
  DEVRECORDTYPEDEF( DEV_ALARM_SF_DANGER_OPEN, "胁迫开门") \
  DEVRECORDTYPEDEF( DEV_ALARM_SF_SUPER_OPEN, "超级密码开门")
int DevRecordInfoOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if (OPER_SUCCESS == eResult && pvContent && (eType == DEV_RECORD_OPER || eType == DEV_ALARM_OPER))
    {
        // return ;
        //sftime time;
        //DEBUG_INFO5(("recv Record %02d:%02d:%02d:%02d--%d"), time.hour(), time.minute(), time.second(), time.millis(), InterlockedIncrement(&count));
        // return;
        // if ( count > 1 ) return;
        DEV_RECORD* dev_rec = ((DEV_RECORD*)pvContent);
        const char* rec_type = "";
        switch (dev_rec->m_RecType)
        {
#define DEVRECORDTYPEDEF(a, b) case a: rec_type = b; break;
            DEVRECORDTYPEDEF_DEF(DEVRECORDTYPEDEF)
#undef DEVRECORDTYPEDEF
        }
        printf(("m_SerialID=%s %s %s\n"), dev_rec->m_SerialID.m_ID, rec_type, dev_rec->m_UserID.m_ID);
        switch (dev_rec->m_Source)
        {
        case DEV_DOOR_SOURCE:   //门相关来源
        {
            printf(("门点 % d的 % 02X号设备"), (dev_rec->m_ConcretSource >> 4) & 0x0f , dev_rec->m_ConcretSource & 0x0f);
        }
        break;
        case DEV_SIGNAL_SOURCE:   //信号输入输出相关来源
        {
            printf(("辅助输入输出端子 % d"), dev_rec->m_ConcretSource);
        }
        break;
        case DEV_NO_NUM_DEV_SOURCE:   //无编号设备来源
        {
            printf("无编号设备来源");
        }
        break;
        default:
            break;
        }
    }
    return 0;
}
int DevRecExOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if ((OPER_SUCCESS == eResult || eResult == OPER_BATCH_DL_COMPLETE) && pvContent)
    {
        DEV_BATCH_RECORD* pRecords = (DEV_BATCH_RECORD*)pvContent;
        DEV_RECORD* pRecord = pRecords->m_pRecordArray;
        for (int i = 0; i < pRecords->m_nCount; ++i)
        {
            DevRecordInfoOper(cszDevAddr, eType , DEV_OPER_DOWNLOAD , pRecord, nSeq, OPER_SUCCESS);
            ++pRecord;
        }
    }
    return 0;
}
int DevChangeImageOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if (OPER_SUCCESS == eResult)
    {
        DEV_CHANGEIMAGE* pCImage = (DEV_CHANGEIMAGE*)pvContent;
#if 0
        CString strSerialID ;
        strSerialID.Format(" % s", pCImage->m_SerialID.m_ID);
        CString tmp_sql;
        tmp_sql.Format(("select * from [record] where([SerialID] = \'%s\')"), strSerialID);
        CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(tmp_sql);
        if (tmp->IsEmpty())
        {
            CString sql;
            sql.Format(("insert into [record] ([SerialID], [PicBytes])values ( \'%s\', %d)"), strSerialID, pCImage->m_PicBytes);
            m_ado.ExecuteNoSelectSQL(sql);
            sql.Format(("select [Block] from [record] where ([SerialID]=\'%s\')"), strSerialID);
            CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(sql);
            ptr->AppendChunk(("Block"), (BYTE*)(pCImage->m_PicData), pCImage->m_PicBytes);
            ptr->Update();
        }
        {
            // return 0;
            DEBUG_INFO(("接受到变更注册照 %s"), pCImage->m_SerialID.m_ID);
            COLORREF color = -1;
            TLCONTROLINFO info[] =
            {
                {TLCT_TEXT, color, ""},
                {TLCT_TEXT, color, ""},
                {TLCT_TEXT, color, ""}
            };
            HTREEITEM fa = TVI_ROOT;
            LPARAM lp = (LPARAM)info;
            int items = ListView_GetItemCount(bb.m_hWnd);
            CTreeCtrl_InsertItem(bb.m_hWnd, pCImage->m_SerialID.m_ID, fa, TVI_LAST, lp);
            CTreeList_SetItem(bb.m_hWnd, items, 1, pCImage->m_UserID.m_ID, lp);
            CString str_value;
            str_value.Format(("%02d/%02d/%04d %02d:%02d:%02d"), pCImage->m_RecTime.m_Date.m_Month,
                             pCImage->m_RecTime.m_Date.m_Day,
                             pCImage->m_RecTime.m_Date.m_Year,
                             pCImage->m_RecTime.m_Time.m_Hour,
                             pCImage->m_RecTime.m_Time.m_Minute,
                             pCImage->m_RecTime.m_Time.m_Second
                            );
            CTreeList_SetItem(bb.m_hWnd, items, 2, str_value, lp);
            delete pCImage;
            return 0;
        }
#endif
    }
    return 0;
}
int DevCImageExOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if ((OPER_SUCCESS == eResult || eResult == OPER_BATCH_DL_COMPLETE) && pvContent)
    {
        DEV_BATCH_CIMAGE* pRecords = (DEV_BATCH_CIMAGE*)pvContent;
        DEV_CHANGEIMAGE* pRecord = pRecords->m_pCImageArray;
        for (int i = 0; i < pRecords->m_nCount; ++i)
        {
            DevChangeImageOper(cszDevAddr, eType, eFlag, pRecord, nSeq , OPER_SUCCESS);
            ++pRecord;
        }
    }
    return 0;
}
int DevAuthOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if (OPER_SUCCESS == eResult)
    {
        //SLG_DLDevBaseInfo(cszDevAddr);
        DEBUG_INFO(("%s"), ("请求获取设备信息"));
    }
    else
    {
        SLG_DCNDev(cszDevAddr);
        DEBUG_INFO(("%s"), ("验证失败"));
    }
    return 0;
}
int DevAddOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    SLG_DLDevBaseInfo(cszDevAddr , nSeq);
    return 0;
}
int DevFormatOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    DEBUG_INFO(("%s%d") , ("格式化res = "), eResult);
    return 0;
}
int DevRealLoadUser(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    if (pvContent)
    {
        DEV_USER_REAL_LOAD& load_mode = *((DEV_USER_REAL_LOAD*)pvContent);
#if 0
        CString str_sql;
        if (!load_mode.m_bApeendID)
        {
            str_sql.Format(("select * from [user] where ([UserId] = \'%s\')"), load_mode.m_ID.m_ID);
        }
        else
        {
            str_sql.Format(("select * from [user] where ([AppendID] = \'%s\')"), load_mode.m_ID.m_ID);
        }
        CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(str_sql);
        if (ptr->IsEmpty())
        {
            DEV_USER user = {0};
            if (!load_mode.m_bApeendID)
            {
                memcpy(user.m_UserID.m_ID, load_mode.m_ID.m_ID, sizeof(DEV_CID));
            }
            else
            {
                memcpy(user.m_AppendID.m_ID, load_mode.m_ID.m_ID, sizeof(DEV_CID));
            }
            //SLG_AddUser(cszDevAddr, user, load_mode);
        }
        else
        {
            DEV_USER user = {0};
            memcpy(user.m_UserID.m_ID, ptr->GetCString(("UserID")).GetBuffer(0), ptr->GetCString(("UserID")).GetLength());
            memcpy(user.m_ImageSID.m_ID, ptr->GetCString(("ChangeID")).GetBuffer(0), ptr->GetCString(("ChangeID")).GetLength());
            memcpy(user.m_AppendID.m_ID, ptr->GetCString(("AppendID")).GetBuffer(0), ptr->GetCString(("AppendID")).GetLength());
            memcpy(user.m_UserName.m_ID, ptr->GetCString(("UserName")).GetBuffer(0), ptr->GetCString(("UserName")).GetLength());
            memcpy(user.m_Department.m_ID, ptr->GetCString(("Department")).GetBuffer(0), ptr->GetCString(("Department")).GetLength());
            memcpy(user.m_Admin.m_ID, ptr->GetCString(("RegAdmin")).GetBuffer(0), ptr->GetCString(("RegAdmin")).GetLength());
            user.m_UserFlag = ptr->GetLong(("UserFlag"));
            user.m_UserType = DEV_USER_TYPE(ptr->GetLong(("UserType")));
            user.m_RightID = ptr->GetLong(("RightID"));
            COleDateTime time;
            time.ParseDateTime(ptr->GetCString(("InvalidTime")));
            user.m_DeadLineTime.m_Date.m_Year = time.GetYear();
            user.m_DeadLineTime.m_Date.m_Month = time.GetMonth();
            user.m_DeadLineTime.m_Date.m_Day = time.GetDay();
            user.m_DeadLineTime.m_Time.m_Hour = time.GetHour();
            user.m_DeadLineTime.m_Time.m_Minute = time.GetMinute();
            user.m_DeadLineTime.m_Time.m_Second = time.GetSecond();
            time.ParseDateTime(ptr->GetCString(("RegistTime")));
            user.m_RegistTime.m_Date.m_Year = time.GetYear();
            user.m_RegistTime.m_Date.m_Month = time.GetMonth();
            user.m_RegistTime.m_Date.m_Day = time.GetDay();
            user.m_RegistTime.m_Time.m_Hour = time.GetHour();
            user.m_RegistTime.m_Time.m_Minute = time.GetMinute();
            user.m_RegistTime.m_Time.m_Second = time.GetSecond();
            time.ParseDateTime(ptr->GetCString(("LastUpdTime")));
            user.m_LastUpdTime.m_Date.m_Year = time.GetYear();
            user.m_LastUpdTime.m_Date.m_Month = time.GetMonth();
            user.m_LastUpdTime.m_Date.m_Day = time.GetDay();
            user.m_LastUpdTime.m_Time.m_Hour = time.GetHour();
            user.m_LastUpdTime.m_Time.m_Minute = time.GetMinute();
            user.m_LastUpdTime.m_Time.m_Second = time.GetSecond();
            time.ParseDateTime(ptr->GetCString(("TypeUpdTime")));
            user.m_TypeUpdTime.m_Date.m_Year = time.GetYear();
            user.m_TypeUpdTime.m_Date.m_Month = time.GetMonth();
            user.m_TypeUpdTime.m_Date.m_Day = time.GetDay();
            user.m_TypeUpdTime.m_Time.m_Hour = time.GetHour();
            user.m_TypeUpdTime.m_Time.m_Minute = time.GetMinute();
            user.m_TypeUpdTime.m_Time.m_Second = time.GetSecond();
            char buf[150 * 1024] = {0};
            if ((load_mode.m_LoadFlag & DL_DEV_PIC) || (load_mode.m_LoadFlag & DL_DEV_USER_FEAT))
            {
                ptr->GetChunk(("Block"), (BYTE*)buf, ptr->GetLong(("FeatBytes")) + ptr->GetLong(("PicBytes")));
                user.m_FeatLen = (load_mode.m_LoadFlag & DL_DEV_USER_FEAT) ? ptr->GetLong(("FeatBytes")) : 0;
                user.m_PicLen = (load_mode.m_LoadFlag & DL_DEV_PIC) ? ptr->GetLong(("PicBytes")) : 0;
                user.m_FeatData = (load_mode.m_LoadFlag & DL_DEV_USER_FEAT) ? buf : NULL;
                user.m_PicData = (load_mode.m_LoadFlag & DL_DEV_PIC) ? (buf + user.m_FeatLen) : NULL;
            }
            SLG_ULRealTimeUser(cszDevAddr, user, load_mode) ;
        }
#endif
        DEBUG_INFO(("%s 请求实时用户加载"), cszDevAddr);
    }
    return 0;
}
int DevLogNumsOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    DEV_DL_RECORD_ARRAY* pRange = (DEV_DL_RECORD_ARRAY*)pvContent;
    if (pRange->m_Count <= 0)
    {
        DEBUG_INFO(("%s"), ("操作日志下载失败"));
    }
    else
    {
        DEV_DL_RECORD_ARRAY* pArray = new DEV_DL_RECORD_ARRAY;
        pArray->m_Count = pRange->m_Count;
        pArray->m_pRange = new DEV_DL_RECORD_RANGE[ pRange->m_Count];
        memcpy(pArray->m_pRange, pRange->m_pRange, sizeof(DEV_DL_RECORD_RANGE)*pRange->m_Count);
        //LRESULT DevLogNumsMsg(WPARAM wparam, LPARAM lpparam)
        {
            DEV_DL_RECORD_ARRAY* pRange = pArray;
            char IP[50] = {0};
            if (cszDevAddr == 0)
            {
                //CString str;
                //GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str);
                //strcpy(IP, str.GetBuffer(0));
            }
            else
            {
                //LongToDotIp(IP, wparam);
            }
            int i = 0;
            for (i = 0; i < pRange->m_Count; ++i)
            {
                pRange->m_pRange[i].m_bOpenRange = FALSE;
                SLG_DLRangeLog(IP, pRange->m_pRange[i], 0x01, TRUE);
            }
            delete [] pRange->m_pRange;
            pRange->m_pRange = NULL;
            delete pRange;
        }
    }
    return 0;
}
int DevUserNumsOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    DEV_DL_USER_RANGE* pRange = (DEV_DL_USER_RANGE*)pvContent;
    if (pRange->m_Count <= 0)
    {
        DEBUG_INFO(("%s"), ("用户下载失败"));
    }
    else
    {
        DEBUG_INFO(("%s"), ("开始用户下载"));
        DEV_DL_USER_RANGE* pArray = new DEV_DL_USER_RANGE;
        pArray->m_Count = pRange->m_Count;
        pArray->m_pArray = new DEV_VUSER[pArray->m_Count];
        memcpy(pArray->m_pArray, pRange->m_pArray, sizeof(DEV_VUSER)*pRange->m_Count);
        //this->PostMessage(USER_NUMS_MSG, DotIpToLong(cszDevAddr), (LPARAM)pArray);
        //LRESULT DevUserNumsMsg(WPARAM wparam, LPARAM lpparam)
        {
            DEV_DL_USER_RANGE* pRange = pArray;
            char IP[50] = {0};
            int flag = DL_DEV_PIC | DL_DEV_USER_FEAT;
            int i = 0;
            BOOL ret = TRUE;
            DEV_VUSER* pTmp = (DEV_VUSER*)(pRange->m_pArray);
            for (; i < pRange->m_Count; ++i)
            {
                do
                {
                    if (SLG_DLSingleUser(IP, pTmp[i].m_UserID , flag))
                    {
                        break;
                    }
                    Sleep(100);
                }
                while (TRUE);
            }
            delete [] pRange->m_pArray;
            pRange->m_pArray = NULL;
            delete pRange;
            //return 0;
        }
    }
    return 0;
}
int DevChangeImageNumsOper(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    DEV_DL_RECORD_ARRAY* pRange = (DEV_DL_RECORD_ARRAY*)pvContent;
    if (pRange->m_Count <= 0)
    {
        DEBUG_INFO(("%s"), ("变更注册照下载失败"));
    }
    else
    {
        DEV_DL_RECORD_ARRAY* pArray = new DEV_DL_RECORD_ARRAY;
        pArray->m_Count = pRange->m_Count;
        pArray->m_pRange = new DEV_DL_RECORD_RANGE[ pRange->m_Count];
        memcpy(pArray->m_pRange, pRange->m_pRange, sizeof(DEV_DL_RECORD_RANGE)*pRange->m_Count);
        //this->PostMessage(CIMAGE_NUMS_MSG, DotIpToLong(cszDevAddr), (LPARAM)pArray);
#if 0
        //LRESULT DevCImageNumsMsg(WPARAM wparam, LPARAM lpparam)
        {
            DEV_DL_RECORD_ARRAY* pRange = pArray;
            char IP[50] = {0};
            int i = 0;
            for (i = 0; i < pRange->m_Count; ++i)
            {
                pRange->m_pRange[i].m_bOpenRange = FALSE;
                if (((CButton*)(GetDlgItem(IDC_CHECK_USER_EX)))->GetCheck())
                {
                    SLG_DLRecEx(IP, pRange->m_pRange[i]);
                    continue;
                }
                SLG_DLRangeCImage(IP, pRange->m_pRange[i], 0x01, TRUE, 2);
            }
            delete [] pRange->m_pRange;
            pRange->m_pRange = NULL;
            delete pRange;
            return 0;
        }
#endif
    }
    return 0;
}
int DevRecNums(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    DEV_DL_RECORD_ARRAY* pRange = (DEV_DL_RECORD_ARRAY*)pvContent;
    if (pRange->m_Count <= 0)
    {
        DEBUG_INFO(("%s"), ("记录下载失败"));
    }
    else
    {
#if 0
        DEV_DL_RECORD_ARRAY* pArray = new DEV_DL_RECORD_ARRAY;
        pArray->m_Count = pRange->m_Count;
        pArray->m_pRange = new DEV_DL_RECORD_RANGE[ pRange->m_Count];
        memcpy(pArray->m_pRange, pRange->m_pRange, sizeof(DEV_DL_RECORD_RANGE)*pRange->m_Count);
        //this->PostMessage(REC_NUMS_MSG, DotIpToLong(cszDevAddr), (LPARAM)pArray);
        //LRESULT DevRecNumsMsg(WPARAM wparam, LPARAM lpparam)
        {
            DEV_DL_RECORD_ARRAY* pRange = (DEV_DL_RECORD_ARRAY*)lpparam;
            char IP[50] = {0};
            if (wparam == 0)
            {
                CString str;
                GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str);
                strcpy(IP, str.GetBuffer(0));
            }
            else
            {
                LongToDotIp(IP, wparam);
            }
            int ntotal = 0;
            int i = 0;
            DEV_DL_RECORD_RANGE* pTemp = (DEV_DL_RECORD_RANGE*)(pRange->m_pRange);
            for (i = 0; i < pRange->m_Count; ++i)
            {
                pTemp->m_bOpenRange = FALSE;
                if (((CButton*)(GetDlgItem(IDC_CHECK_USER_EX)))->GetCheck())
                {
                    // cpm_( IP, pRange->m_pRange[i] );
                    continue;
                }
                if (!SLG_DLRangeRec(IP, pTemp[i] , 0x01 , true))
                {
                    break;
                }
            }
            delete [] pRange->m_pRange;
            pRange->m_pRange = NULL;
            delete pRange;
            return 0;
        }
#endif
    }
    return 0;
}
int DevAlarmNums(const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
    DEV_DL_RECORD_ARRAY* pRange = (DEV_DL_RECORD_ARRAY*)pvContent;
    if (pRange->m_Count <= 0)
    {
        DEBUG_INFO(("%s"), ("记录下载失败"));
    }
    else
    {
#if 0
        DEV_DL_RECORD_ARRAY* pArray = new DEV_DL_RECORD_ARRAY;
        pArray->m_Count = pRange->m_Count;
        pArray->m_pRange = new DEV_DL_RECORD_RANGE[ pRange->m_Count];
        memcpy(pArray->m_pRange, pRange->m_pRange, sizeof(DEV_DL_RECORD_RANGE)*pRange->m_Count);
        //this->PostMessage(ALARMS_NUMS_MSG, DotIpToLong(cszDevAddr), (LPARAM)pArray);
        //LRESULT DevAlarmNumsMsg(WPARAM wparam, LPARAM lpparam)
        {
            DEV_DL_RECORD_ARRAY* pRange = (DEV_DL_RECORD_ARRAY*)lpparam;
            char IP[50] = {0};
            if (wparam == 0)
            {
                CString str;
                GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str);
                strcpy(IP, str.GetBuffer(0));
            }
            else
            {
                LongToDotIp(IP, wparam);
            }
            int i = 0;
            for (i = 0; i < pRange->m_Count; ++i)
            {
                pRange->m_pRange[i].m_bOpenRange = FALSE;
                if (((CButton*)(GetDlgItem(IDC_CHECK_USER_EX)))->GetCheck())
                {
                    SLG_DLRecEx(IP, pRange->m_pRange[i]);
                    continue;
                }
                SLG_DLRangeAlarm(IP, pRange->m_pRange[i], 0x01, TRUE);
            }
            delete [] pRange->m_pRange;
            pRange->m_pRange = NULL;
            delete pRange;
        }
#endif
    }
    return 0;
}
#if 0
int OnClose()
{
    KillTimer(SERCH_EVENT_TIME);
    SLG_InitSys(FALSE);
    // return;
    CDialog::OnClose();
}
int OnButtonTimegroup()
{
    // TODO: Add your control notification handler code here
    CGroupTimeDlg dlg(m_ado);
    dlg.DoModal();
}
int OnButtonRright()
{
    // TODO: Add your control notification handler code here
    CRightDlg dlg(m_ado);
    dlg.DoModal();
}
int OnButtonUG()
{
    // TODO: Add your control notification handler code here
    UserGroupDlg dlg(m_ado);
    dlg.DoModal();
}
int OnButtonOpenDoor()
{
    // TODO: Add your control notification handler code here
    static BOOL bOpen = TRUE;
    POSITION pos = m_dev.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(("请选择一个设备"));
        return;
    }
    int nIndex = m_dev.GetNextSelectedItem(pos);
    CString str_ip = m_dev.GetItemText(nIndex, 0);
    SLG_ULOpenDoorState(str_ip.GetBuffer(0), DEV_DOOR_OPEN) ;
    GetDlgItem(IDC_BUTTON_OPEN_DOOR)->SetWindowText(bOpen ? ("紧急关门") : ("紧急开门"));
    bOpen = !bOpen;
    UpdateData(TRUE);
}
int OnButtonCancelOpenDoor()
{
    // TODO: Add your control notification handler code here
    POSITION pos = m_dev.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(("请选择一个设备"));
        return;
    }
    int nIndex = m_dev.GetNextSelectedItem(pos);
    CString str_ip = m_dev.GetItemText(nIndex, 0);
    SLG_ULOpenDoorState(str_ip.GetBuffer(0), DEV_DOOR_CLOSE) ;
    SLG_DLOpenDoorState(str_ip.GetBuffer(0));
}
int OnButtonROpenDoor()
{
    // TODO: Add your control notification handler code here
    static BOOL bOpen = TRUE;
    POSITION pos = m_dev.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(("请选择一个设备"));
        return;
    }
    int nIndex = m_dev.GetNextSelectedItem(pos);
    CString str_ip = m_dev.GetItemText(nIndex, 0);
    SLG_ULRemoteOpenDoor(str_ip.GetBuffer(0), bOpen) ;
    GetDlgItem(IDC_BUTTON_REMOTE)->SetWindowText(bOpen ? ("远程关门") : ("远程开门"));
    bOpen = !bOpen;
    UpdateData(TRUE);
}
int OnButtonECloseDoor()
{
    static BOOL bOpen = FALSE;
    POSITION pos = m_dev.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(("请选择一个设备"));
        return;
    }
    int nIndex = m_dev.GetNextSelectedItem(pos);
    CString str_ip = m_dev.GetItemText(nIndex, 0);
    SLG_ULOpenDoorState(str_ip.GetBuffer(0), DEV_DOOR_NOMAL) ;
}
int OnButtonRCloseDoor()
{
    static BOOL bOpen = FALSE;
    POSITION pos = m_dev.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(("请选择一个设备"));
        return;
    }
    int nIndex = m_dev.GetNextSelectedItem(pos);
    CString str_ip = m_dev.GetItemText(nIndex, 0);
    SLG_ULRemoteOpenDoor(str_ip.GetBuffer(0), bOpen) ;
}
int OnButtonEdit()
{
    // TODO: Add your control notification handler code here
    POSITION pos = m_user.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(("请选择一条记录"));
        return;
    }
    int n = m_user.GetNextSelectedItem(pos);
    CString str_id = m_user.GetItemText(n, 0);
    AddUserDlg dlg(m_ado, str_id.GetBuffer(0));
    if (IDOK == dlg.DoModal())
    {
        char values[5] = {0};
        int items = m_user.GetItemCount();
        int count = m_user.GetItemCount();
        CString id(dlg.m_ID);
        CString tmp;
        m_user.SetItemText(n, 0, dlg.m_ID);
        m_user.SetItemText(n, 1, dlg.m_Card);
        m_user.SetItemText(n, 2, dlg.m_Name);
        int R_ID = dlg.m_Right;
        sprintf(values, "%d", R_ID);
        m_user.SetItemText(n, 3, values);
        sprintf(values, "%d", dlg.m_UserType);
        m_user.SetItemText(n, 4, values);
        sprintf(values, "%d", dlg.m_userFlag);
        m_user.SetItemText(n, 5, values);
        m_user.SetItemText(n, 6, dlg.m_invalideTime);
        CString sql ;
        sql.Format(("update [user] set [AppendID]=\'%s\', [UserName]=\'%s\', [Department]=\'%s\', \
 [RightID]=%d, [UserType]=%d, [UserFlag]=%d, [InvalidTime]=\'%s\' where [UserID]=\'%s\'") ,
                   dlg.m_Card, dlg.m_Name, dlg.m_Dep, R_ID, dlg.m_UserType, dlg.m_userFlag , dlg.m_invalideTime, dlg.m_ID
                  );
        m_ado.ExecuteNoSelectSQL(sql);
        UpdateData(FALSE);
        m_user.SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
    }
}
#endif
#if 0
int OnButtonDisconnect()
{
    // TODO: Add your control notification handler code here
    POSITION pos = m_dev.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(("请选择一个设备"));
        return;
    }
    int nIndex = m_dev.GetNextSelectedItem(pos);
    CString str_ip = m_dev.GetItemText(nIndex, 0);
    SLG_DCNDev(str_ip.GetBuffer(0));
}
int OnUpdate()
{
    UpdateData(TRUE);
    POSITION pos = m_dev.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(("请选择一个设备"));
        return;
    }
    int nIndex = m_dev.GetNextSelectedItem(pos);
    CString str_ip = m_dev.GetItemText(nIndex, 0);
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "(*.*)|*.*||");
    if (dlg.DoModal() != IDOK)
    {
        return;
    }
    CString file = dlg.GetPathName();
    // GetDlgItem(IDC_EDIT_FILE)->GetWindowText( file );
    FILE* pfile = fopen(dlg.GetPathName(), "rb");
    if (pfile)
    {
        DEV_APPUPDATE app = {0};
        fseek(pfile, 0, SEEK_END);
        app.m_Bytes = ftell(pfile);
        fseek(pfile, 0, SEEK_SET);
        app.m_Buf = new char[app.m_Bytes];
        memset(app.m_Buf, 0x0, app.m_Bytes);
        fread(app.m_Buf, 1, app.m_Bytes, pfile);
        app.m_FileIndex = 1;
        app.m_TotalFiles = 1;
        int nCount = m_dev.GetItemCount();
        for (int i = 0; i < nCount; ++i)
        {
            CString str_ip = m_dev.GetItemText(i, 0);
            SLG_ULUpdate(str_ip.GetBuffer(0), app);
        }
        delete [] app.m_Buf;
        fclose(pfile);
    }
}
int OnFile()
{
    // CFileDialog dlg(TRUE, NULL,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "(*.*)|*.*||" );
    // if ( dlg.DoModal() != IDOK ) return;
    // GetDlgItem(IDC_EDIT_FILE)->SetWindowText( dlg.GetPathName() );
    // UpdateData( FALSE );
    CPasswordDlg dg;
    DWORD dw = dg.DoModal();
    if (IDOK != dw)
    {
        return;
    }
    POSITION pos = m_dev.GetFirstSelectedItemPosition();
    int nIndex = 0;
    m_dev.GetNextSelectedItem(pos);
    CString str_ip = m_dev.GetItemText(nIndex, 0);
    CCreateDynamicDlg dlg(str_ip.GetBuffer(0), this);
    dlg.DoModal();
    SLG_RegOperResultNotifyCB(::DevOperResult, this);
    GetDlgItem(IDC_BUTTON_FILE)->ShowWindow(SW_HIDE);
}
int OnButtonDLLog()
{
    POSITION pos;
    int nIndex;
    CString str_ip;
    if (m_bNet)
    {
        pos = m_dev.GetFirstSelectedItemPosition();
        if (pos == NULL)
        {
            AfxMessageBox(("请选择一个设备"));
            return;
        }
        nIndex = m_dev.GetNextSelectedItem(pos);
        str_ip = m_dev.GetItemText(nIndex, 0);
    }
    else
    {
        GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str_ip);
        if (str_ip.IsEmpty())
        {
            AfxMessageBox(("请选择一个U盘数据库"));
            return;
        }
    }
    UpdateData(TRUE);
    DEV_DL_RECORD_RANGE range = {0};
    CString count ;
    GetDlgItem(IDC_EDIT_COUNT)->GetWindowText(count);
    range.m_Count = atoi(count.GetBuffer(0));
    CString id ;
    GetDlgItem(IDC_EDIT_ID)->GetWindowText(id);
    memcpy(range.m_SID.m_ID, id.GetBuffer(0), id.GetLength());
    bool flag = 1;
    if (((CButton*)(GetDlgItem(IDC_RADIO_ALL)))->GetCheck())
    {
        flag = true;
    }
    else if (((CButton*)(GetDlgItem(IDC_RADIO_LOG_ID)))->GetCheck())
    {
        flag = false;
    }
    BOOL ret = flag ? SLG_DLLog(str_ip) : SLG_DLRangeLog(str_ip, range);
    if (!ret)
    {
        DEBUG_INFO(("%s"), ("请求下载操作日志具体数据"));
    }
    else
    {
        DEBUG_INFO(("%s"), ("请求下载操作日志ID集合"));
    }
}
int OnButtonDelLog()
{
    strlist_free(m_log);
}
int OnButtonDLImage()
{
    POSITION pos;
    int nIndex;
    CString str_ip;
    if (m_bNet)
    {
        pos = m_dev.GetFirstSelectedItemPosition();
        if (pos == NULL)
        {
            AfxMessageBox(("请选择一个设备"));
            return;
        }
        nIndex = m_dev.GetNextSelectedItem(pos);
        str_ip = m_dev.GetItemText(nIndex, 0);
    }
    else
    {
        GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str_ip);
        if (str_ip.IsEmpty())
        {
            AfxMessageBox(("请选择一个U盘数据库"));
            return;
        }
    }
    UpdateData(TRUE);
    DEV_DL_RECORD_RANGE range = {0};
    CString count ;
    GetDlgItem(IDC_EDIT_COUNT)->GetWindowText(count);
    range.m_Count = atoi(count.GetBuffer(0));
    CString id ;
    GetDlgItem(IDC_EDIT_ID)->GetWindowText(id);
    memcpy(range.m_SID.m_ID, id.GetBuffer(0), id.GetLength());
    bool flag = 1;
    if (((CButton*)(GetDlgItem(IDC_RADIO_ALL_IMAGE)))->GetCheck())
    {
        flag = true;
    }
    else if (((CButton*)(GetDlgItem(IDC_RADIO_IMAGE_ID)))->GetCheck())
    {
        flag = false;
    }
    BOOL ret = flag ? SLG_DLChangeImage(str_ip, 2) : SLG_DLRangeCImage(str_ip, range, 0x01);
    if (!ret)
    {
        DEBUG_INFO(("%s"), ("记录请求下载发送失败"));
    }
    else
    {
        DEBUG_INFO(("%s"), ("记录请求下载发送成功"));
    }
}
int OnButtonDelImage()
{
    CString str_sql(("delete from [record]"));
    m_ado.ExecuteNoSelectSQL(str_sql);
    m_nImageItems = 0;
    m_image.DeleteAllItems();
}
LRESULT OnUpdateImage(WPARAM wparam, LPARAM lpparam)
{
    int i = 0;
    int count = m_pImageList.size();
    for (; i < count; ++i)
    {
        DEV_CHANGEIMAGE* pCImage = (DEV_CHANGEIMAGE*)m_pImageList.front();
        COLORREF color = -1;
        TLCONTROLINFO info[] =
        {
            {TLCT_TEXT, color, ""},
            {TLCT_TEXT, color, ""},
            {TLCT_TEXT, color, ""}
        };
        HTREEITEM fa = TVI_ROOT;
        LPARAM lp = (LPARAM)info;
        int items = ListView_GetItemCount(bb.m_hWnd);
        CTreeCtrl_InsertItem(bb.m_hWnd, pCImage->m_SerialID.m_ID, fa, TVI_LAST, lp);
        CTreeList_SetItem(bb.m_hWnd, items, 1, pCImage->m_UserID.m_ID, lp);
        CString str_value;
        str_value.Format(("%02d/%02d/%04d %02d:%02d:%02d"), pCImage->m_RecTime.m_Date.m_Month,
                         pCImage->m_RecTime.m_Date.m_Day,
                         pCImage->m_RecTime.m_Date.m_Year,
                         pCImage->m_RecTime.m_Time.m_Hour,
                         pCImage->m_RecTime.m_Time.m_Minute,
                         pCImage->m_RecTime.m_Time.m_Second
                        );
        CTreeList_SetItem(bb.m_hWnd, items, 2, str_value, lp);
        CString strSerialID ;
        strSerialID.Format("%s", pCImage->m_SerialID.m_ID);
        CString tmp_sql;
        tmp_sql.Format(("select * from [record] where ([SerialID]=\'%s\')"), strSerialID);
        CHCRecordsetPtr tmp = m_ado.ExecuteSelectSQL(tmp_sql);
        if (tmp->IsEmpty())
        {
            CString sql;
            sql.Format(("insert into [record] ([SerialID], [PicBytes])values ( \'%s\', %d)"), strSerialID, pCImage->m_PicBytes);
            m_ado.ExecuteNoSelectSQL(sql);
            sql.Format(("select [Block] from [record] where ([SerialID]=\'%s\')"), strSerialID);
            CHCRecordsetPtr ptr = m_ado.ExecuteSelectSQL(sql);
            ptr->AppendChunk(("Block"), (BYTE*)(pCImage->m_PicData), pCImage->m_PicBytes);
            ptr->Update();
        }
        m_pImageList.pop_front();
        count--;
        delete [] pCImage->m_PicData;
        pCImage->m_PicData = NULL;
        delete pCImage;
    }
    return 0;
}
int OnButtonCNServer()
{
    UpdateData(TRUE);
    char ip[16] = {0};
    BYTE field0 = 0, field1 = 0, field2 = 0, field3 = 0;
    m_IPServer.GetAddress(field0, field1, field2, field3);
    sprintf(m_serverIP, "%d.%d.%d.%d", field0, field1, field2, field3);
    if (strlen(m_serverIP) == 0)
    {
        DEBUG_INFO(("%s") , ("中转服务器IP为空"));
        return ;
    }
    //SLG_CNServer(m_serverIP);
}
int OnButtonDCNServer()
{
    UpdateData(TRUE);
    char ip[16] = {0};
    BYTE field0 = 0, field1 = 0, field2 = 0, field3 = 0;
    m_IPServer.GetAddress(field0, field1, field2, field3);
    sprintf(ip, "%d.%d.%d.%d", field0, field1, field2, field3);
    SLG_DCNDev(ip);
}
int OnCmdNetOrUDisk(UINT nID)
{
    if (IDC_RADIO_NET == nID)
    {
        m_bNet = TRUE;
    }
    else
    {
        m_bNet = FALSE;
    }
}
int OnBtnUDiskScan()
{
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "db Files (*.db)|*.db||");
    if (dlg.DoModal() != IDOK)
    {
        return;
    }
    GetDlgItem(IDC_EDIT_UPATH)->SetWindowText(dlg.GetPathName());
    UpdateData(FALSE);
}
int OnBtnUDiskCN()
{
    UpdateData(TRUE);
    CString str;
    GetDlgItem(IDC_EDIT_UPATH)->GetWindowText(str);
    SLG_CNDev(str.GetBuffer(0));
}
int OnBtnStartVideo()
{
    VideoDlg* pDlg = new VideoDlg;
    CStringArray IPArray;
    int nCount = m_dev.GetItemCount();
    int i = 0;
    for (i = 0; i < nCount; ++i)
    {
        IPArray.Add(m_dev.GetItemText(i, 0));
    }
    pDlg->Init(IPArray);
    pDlg->Create(IDD_DIALOG_VIDEO);
    pDlg->ShowWindow(SW_SHOW);
    // POSITION pos = m_dev.GetFirstSelectedItemPosition();
    // if ( pos == NULL )
    // {
    // return;
    // }
    // int nIndex = m_dev.GetNextSelectedItem(pos);
    // CString str_ip = m_dev.GetItemText(nIndex, 0);
    // SLG_StartVideo( str_ip.GetBuffer(0), (long)( GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd() ) );
}
int OnBtnCap()
{
    UpdateData(TRUE);
    CString str;
    GetDlgItem(IDC_EDIT_ID_USER)->GetWindowText(str);
    POSITION pos = m_dev.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        return;
    }
    int nIndex = m_dev.GetNextSelectedItem(pos);
    CString str_ip = m_dev.GetItemText(nIndex, 0);
    DEV_USER user = { 0 };
    strcpy(user.m_UserID.m_ID , str.GetBuffer(0));
    SLG_ULRealTimeUserCap(str_ip.GetBuffer(0), user , -1);
}
int OnBtnReboot()
{
    POSITION pos = m_dev.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        AfxMessageBox(("请选择一个设备"));
    }
    int nIndex = m_dev.GetNextSelectedItem(pos);
    CString str_ip = m_dev.GetItemText(nIndex, 0);
    SLG_RebootDev(str_ip.GetBuffer(0));
}
int OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (IsClickDmyButton(point))
    {
        m_nLClickTimes++;
        if (m_nLClickTimes == 6)
        {
            GetDlgItem(IDC_BUTTON_FILE)->ShowWindow(SW_SHOW);
            m_nLClickTimes = 0;
        }
    }
    else
    {
        m_nLClickTimes = 0;
    }
    CDialog::OnLButtonUp(nFlags, point);
}
bool IsClickDmyButton(CPoint pt)
{
    CRect rc;
    GetDlgItem(IDC_BUTTON_FILE)->GetWindowRect(&rc);
    ScreenToClient(&rc);
    if (rc.PtInRect(pt))
    {
        return TRUE;
    }
    return false;
}
#endif

