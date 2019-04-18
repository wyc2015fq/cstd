# 逆向生成的Dimac.JMail工程及测试项目 - weixin_33985507的博客 - CSDN博客
2006年02月17日 10:51:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
**严重申明：本方案中的两个工程由Dimac.JMail逆向生成，版权归Dimac公司所。任何单位或个人不得将工程里的任何代码用于商业用途。本工程只做讨论及学习使用。对于其它行为的，对当事人自己负全部的责任。对于下载本测试代码的任何人，都视为同意此申明里的所有内容。**
不知道MS对此会有怎样的说明。对于一个生成DLL的文件，可以用工具轻松的逆向转化为工程文件，而且可以通过编译而后编译成与源DLL字节大小完全一样的文件来，而且可以正常使用。
对于一些大一点的DLL文件，可能逆向出来的文件很多，所以编译一时不能通过，但经过细心的修改后，也还是可以的。测试过三个项目，一个是MS自己的Web Controls，第二个是MS的WebService文件，而另一个是Dimac的JMail。前面两个有些复杂，短时间内无法修改让其通过编译，而对于Dimac.JMail，经过小的改动，可以通过编译，且正常工作。
索性加上自己以前写的Mail管理类，然后一起做一个小测试项目。
部份代码：
IMail接口：
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)using System;
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)namespace Webb.WAVE.Controls
![ExpandedBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)/**////<summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)/// Summary description for Webb.
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)///</summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicinterface IEMail:IDisposable
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)string ServerName![dot.gif](https://www.cnblogs.com/Images/dot.gif){get;set;}
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)string ServerLoginName![dot.gif](https://www.cnblogs.com/Images/dot.gif){get;set;}
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)string ServerLoginPassword![dot.gif](https://www.cnblogs.com/Images/dot.gif){set;}
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)string Subject![dot.gif](https://www.cnblogs.com/Images/dot.gif){set;get;}
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)string Body![dot.gif](https://www.cnblogs.com/Images/dot.gif){set;get;}
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)string HTMLBody![dot.gif](https://www.cnblogs.com/Images/dot.gif){set;get;}
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)string From![dot.gif](https://www.cnblogs.com/Images/dot.gif){set;get;}
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)void AddToAddress(string i_address);
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)void AddCcAddress(string i_address);
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)void SendEMail();
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![ExpandedBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
JMailDotNet类：
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)using System;
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)using Dimac.JMail;
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)using System.Data;
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)namespace Webb.WAVE.Controls
![ExpandedBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)/**////<summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)///</summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicclass JMailDotNet:IEMail
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)Fields#region Fields
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)privatestring m_server;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)privatestring m_loginName;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)privatestring m_password;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)privatestring m_from;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)private Message m_message;
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)Properties#region Properties
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//        public Message MailMessage
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//        {
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            get{return this.m_message;}
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            set{this.m_message    = value;}
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//        }
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)public JMailDotNet()
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            m_message    =new Message();
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)public JMailDotNet(bool i_systemServer)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            m_message    =new Message();
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            this.m_password        = WaveSystem.SystemVarible["EmailServerPassword"].ToString();
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            this.m_loginName    = WaveSystem.SystemVarible["EmailServerLoginName"].ToString();
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            this.m_server        = WaveSystem.SystemVarible["EmailServer"].ToString();
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)IEMail Members#region IEMail Members
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicstring ServerName
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)get![dot.gif](https://www.cnblogs.com/Images/dot.gif){returnthis.m_server;}
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)set![dot.gif](https://www.cnblogs.com/Images/dot.gif){this.m_server    = value;}
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicstring ServerLoginName
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)get![dot.gif](https://www.cnblogs.com/Images/dot.gif){returnthis.m_loginName;}
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)set![dot.gif](https://www.cnblogs.com/Images/dot.gif){this.m_loginName = value;}
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicstring ServerLoginPassword
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)set![dot.gif](https://www.cnblogs.com/Images/dot.gif){this.m_password    = value;}
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicstring Subject
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)get![dot.gif](https://www.cnblogs.com/Images/dot.gif){returnthis.m_message.Subject;}
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)set![dot.gif](https://www.cnblogs.com/Images/dot.gif){this.m_message.Subject = value;}
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicstring Body
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)get![dot.gif](https://www.cnblogs.com/Images/dot.gif){returnthis.m_message.BodyText;}
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)set![dot.gif](https://www.cnblogs.com/Images/dot.gif){this.m_message.BodyText = value;}
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicstring HTMLBody
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)get![dot.gif](https://www.cnblogs.com/Images/dot.gif){returnthis.m_message.BodyHtml;}
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)set![dot.gif](https://www.cnblogs.com/Images/dot.gif){this.m_message.BodyHtml = value;}
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicstring From
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)get![dot.gif](https://www.cnblogs.com/Images/dot.gif){returnthis.m_from;}
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)set![dot.gif](https://www.cnblogs.com/Images/dot.gif){this.m_from = value;}
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicvoid AddToAddress(string i_address)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_message.To.Add(new Address(i_address));
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicvoid AddCcAddress(string i_address)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_message.Cc.Add(new Address(i_address));
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicvoid SendEMail()
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            Smtp m_SMTPServer        =new Smtp();
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            m_SMTPServer.Domain        =this.m_server;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            m_SMTPServer.Password    =this.m_password;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            m_SMTPServer.UserName    =this.m_loginName;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            m_SMTPServer.HostName    =this.m_server;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_message.From        =new Address(this.m_from);
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            m_SMTPServer.Authentication    =SmtpAuthentication.Login;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)try
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                m_SMTPServer.Send(this.m_message);
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)/**//*
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                catch(SmtpAuthNotSupportedException  ex)
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                {
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                    //WaveTrace.TraceMsg(ex.Message);
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                    //throw new Exception("EMail sent error, all message send faild.");
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                catch(LoginFailedException  ex)
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                {
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                    //WaveTrace.TraceMsg(ex.Message);
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                    //throw new Exception("EMail sent error, all message send faild.");
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                catch(SmtpException  ex)
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                {
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                    //WaveTrace.TraceMsg(ex.Message);
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                    //throw new Exception("EMail sent error, all message send faild.");
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                catch(ConnectionException  ex)
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                {
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                    //WaveTrace.TraceMsg(ex.Message);
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                    //throw new Exception("EMail sent error, all message send faild.");
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                catch(ArgumentNullException  ex)
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                {
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                    //WaveTrace.TraceMsg(ex.Message);
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                    //throw new Exception("EMail sent error, all message send faild.");
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                }
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)*/
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)catch(JMailException)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//                WaveHelper.TraceMsg(ex.Message);
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)thrownew Exception("EMail sent error, all message send faild.");
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)finally
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                m_SMTPServer        =null;
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)IDisposable Members#region IDisposable Members
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicvoid Dispose()
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_message        =null;
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![ExpandedBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)
邮件管理类：其中注释掉了在其它项目里使用的函数，但这些函数也可以做为学习的例子。
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)using System;
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)using System.Data;
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)using Dimac.JMail;
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)namespace Webb.WAVE.Controls
![ExpandedBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)/**////<summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)/// Summary description for Webb.
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)///</summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicenum MailType
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        JMailCom        =0,
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        JMailDotNet        =1,
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        EMailMessage    =2,
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        Default            =3
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicclass WaveEMailManager:IDisposable
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){        
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)Fields#region Fields
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)private IEMail m_email;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)private MailType m_mailType;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//        private WaveEMail m_dbmail;
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)Properties#region Properties
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)public IEMail EMail
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)get![dot.gif](https://www.cnblogs.com/Images/dot.gif){returnthis.m_email;}
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)public MailType EMailType
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)get![dot.gif](https://www.cnblogs.com/Images/dot.gif){returnthis.m_mailType;}
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)/**////<summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)///</summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)public WaveEMailManager()
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)/**////<summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///</summary>
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)///<param name="i_MailType"></param>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)public WaveEMailManager(MailType i_MailType)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_mailType    = i_MailType;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)if(i_MailType==MailType.JMailCom)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_email    =new JMailCOM(true) as IEMail;
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)elseif(i_MailType==MailType.JMailDotNet)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_email    =new JMailDotNet(true) as IEMail;
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)elseif(i_MailType==MailType.EMailMessage)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_email    =new EMailMessage(true) as IEMail;
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)elseif(i_MailType==MailType.Default)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)string m_control    ="msg";//WaveSystem.SystemVarible["EmailControl"].ToString();
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)switch(m_control)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)case"com":
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_email    =new JMailCOM(true) as IEMail;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_mailType    = MailType.JMailCom;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)break;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)case"net":
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_email    =new JMailDotNet(true) as IEMail;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_mailType    = MailType.JMailDotNet;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)break;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)case"msg":
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_email    =new EMailMessage(true) as IEMail;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_mailType    = MailType.EMailMessage;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)break;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)default:
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_email    =new EMailMessage(true) as IEMail;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)this.m_mailType    = MailType.EMailMessage;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)break;
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)                }
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)/**////<summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///</summary>
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)///<param name="i_mailID"></param>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicvoid SendDbEmailByID(long i_mailID)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            if(this.m_dbmail==null){this.m_dbmail    = new WaveEMail();}
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            m_dbmail.LoadData(i_mailID);
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            this.m_email.HTMLBody    = this.m_dbmail.HTMLBody;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            this.m_email.AddToAddress(this.m_dbmail.To);
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            this.m_email.Subject    = this.m_dbmail.Subject;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            try{this.m_email.SendEMail();}
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            catch(Exception ex){WaveHelper.TraceMsg(ex.Message);throw ex;}
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            m_dbmail.SentTimes++;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            m_dbmail.UpdateData();
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)Static functions#region Static functions
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)/**////<summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///</summary>
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)///<param name="i_mailID"></param>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)staticpublicvoid DeleteEMail(long i_mailID)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            this.m_dbManager            = new OleDBManager(true) as IOleDBManager;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            this.m_dbManager.Command    = new System.Data.OleDb.OleDbCommand();
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            ISqlDBManager m_dbManager    = new SqlDBManager(true) as ISqlDBManager;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            m_dbManager.Command            = new System.Data.SqlClient.SqlCommand();
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            m_dbManager.Command.CommandText    = SQLCommandText.T_Emails.T_Emails_DeleteByID;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            m_dbManager.Command.Parameters.Add("@log_id",i_mailID);
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            try{m_dbManager.ExecuteNonQuery();}
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            catch(Exception ex){WaveHelper.TraceMsg(ex.Message);throw ex;}
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            m_dbManager.Dispose();
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)/**////<summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///</summary>
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)///<param name="i_table"></param>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)staticpublicvoid GetAllEmails(DataTable i_table)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            this.m_dbManager            = new OleDBManager(true) as IOleDBManager;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            this.m_dbManager.Command    = new System.Data.OleDb.OleDbCommand();
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            ISqlDBManager m_dbManager    = new SqlDBManager(true) as ISqlDBManager;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            m_dbManager.Command            = new System.Data.SqlClient.SqlCommand();
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            m_dbManager.Command.CommandText    = SQLCommandText.T_Emails.T_Emails_SelectAllMails;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            try{m_dbManager.FillDataTable(i_table);}
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            catch(Exception ex){WaveHelper.TraceMsg(ex.Message);throw ex;}
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            m_dbManager.Dispose();
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)/**////<summary>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)///</summary>
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)///<param name="i_send"></param>
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)staticpublicvoid DeleteAllSendMail(int i_send)
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            this.m_dbManager            = new OleDBManager(true) as IOleDBManager;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            this.m_dbManager.Command    = new System.Data.OleDb.OleDbCommand();
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            ISqlDBManager m_dbManager    = new SqlDBManager(true) as ISqlDBManager;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            m_dbManager.Command            = new System.Data.SqlClient.SqlCommand();
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            m_dbManager.Command.CommandText    = SQLCommandText.T_Emails.T_Emails_DeleteBySent;
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            m_dbManager.Command.Parameters.Add("@int_sent",i_send);
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            try{m_dbManager.ExecuteNonQuery();}
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            catch(Exception ex){WaveHelper.TraceMsg(ex.Message);throw ex;}
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            m_dbManager.Dispose();
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)IDisposable Members#region IDisposable Members
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)publicvoid Dispose()
![ExpandedSubBlockStart.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)if(this.m_email!=null)this.m_email.Dispose();
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
![InBlock.gif](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![ExpandedSubBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![ExpandedBlockEnd.gif](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![None.gif](https://blog.csdn.net/Images/OutliningIndicators/None.gif)
请不要修改邮箱的密码，如果须要，请自行申请邮箱帐号。另外，JMailCOM须要安装JMail的COM组件，工程里引用了这个文件，但对于没有安装该组件的不能正常使用这个类型的邮件服务。
![Dimac.JMail.TestForm.JPG](https://images.cnblogs.com/cnblogs_com/wucountry/2006_02/Dimac.JMail.TestForm.JPG)
项目源代码下载：［请认阅读申明］
**严重申明：本方案中的两个工程由Dimac.JMail逆向生成，版权归Dimac公司所。任何单位或个人不得将工程里的任何代码用于商业用途。本工程只做讨论及学习使用。对于其它行为的，对当事人自己负全部的责任。对于下载本测试代码的任何人，都视为同意此申明里的所有内容。**[http://files.cnblogs.com/WuCountry/Webb.JMail.zip](http://files.cnblogs.com/WuCountry/Webb.JMail.zip)
不好意思，刚才搞错了文件，对于已经下载的朋友表示报谦。。。。
