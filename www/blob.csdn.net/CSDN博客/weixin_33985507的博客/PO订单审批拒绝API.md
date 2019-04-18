# PO订单审批拒绝API - weixin_33985507的博客 - CSDN博客
2017年05月26日 16:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2

```
DECLARE
  l_return_status    VARCHAR2(1);
  l_return_code      VARCHAR2(1000);
  l_exception_msg    VARCHAR2(4000);
  l_online_report_id NUMBER;
BEGIN
  --init
  mo_global.set_policy_context(p_access_mode => 'S',
                               p_org_id      => 81);
  fnd_global.apps_initialize(user_id      => 1150,
                             resp_id      => 50717,
                             resp_appl_id => 20005);
  -- Call the procedure
  po_document_action_pvt.do_reject(p_document_id      => 126003,
                                   p_document_type    => 'PO',
                                   p_document_subtype => 'STANDARD',
                                   p_note             => 'lalala',
                                   p_approval_path_id => 2,
                                   x_return_status    => l_return_status,
                                   x_return_code      => l_return_code,
                                   x_exception_msg    => l_exception_msg,
                                   x_online_report_id => l_online_report_id);
  IF l_return_status = 'S' THEN
    dbms_output.put_line('Successful!');
    dbms_output.put_line('l_return_code = ' || l_return_code);
    dbms_output.put_line('l_online_report_id = ' || l_online_report_id);
    --dbms_output.put_line('l_exception_msg = ' || l_exception_msg);
  
  ELSE
  
    dbms_output.put_line('Error Status = ' || l_return_status);
    dbms_output.put_line('l_return_code = ' || l_return_code);
    dbms_output.put_line('l_online_report_id = ' || l_online_report_id);
    dbms_output.put_line('l_exception_msg:' || l_exception_msg);
  END IF;
END;
```
