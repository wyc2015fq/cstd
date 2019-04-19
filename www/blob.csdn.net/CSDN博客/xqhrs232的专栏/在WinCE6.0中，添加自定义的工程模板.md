# 在WinCE6.0中，添加自定义的工程模板 - xqhrs232的专栏 - CSDN博客
2013年08月21日 22:38:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：665
原文地址::[http://blog.csdn.net/nanjianhui/article/details/2612428](http://blog.csdn.net/nanjianhui/article/details/2612428)
相关网帖
1、[http://bbs.csdn.net/topics/120027003](http://bbs.csdn.net/topics/120027003)

作者：ARM-WinCE
目前，创建WinCE6.0的工程的时候，都是打开VS2005，然后创建一个Platform Builder for
 CE6.0的工程，然后根据Wizard的提示，一步一步下去。在整个创建过程中，首先选择BSP，然后就是选择模板，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/test.JPG)
上面这些模板都是微软提供的，这些模板的描述文件可以在” /WINCE600/PUBLIC/CEBASE/CATALOG”下面找到。实际上，这些模板都是一些基于xml的描述文件。其中最简单的应该是”
 odtsmall_footprint.pbcxml”。对应上图中的模板应该是”Small Footprint Device”。下面我先对该模板做个简单介绍：
用记事本打开”odtsmall_footprint.pbcxml”文件，我们可以看到整个模板的架构如下：
<CatalogFile …>
  <FileInformation …>
              …
         </FileInformation>
         <OSDesignTemplate …>
              …
         </OSDesignTemplate>
      </Catalogfile>
**A. FileInformation****，格式如下**：
<FileInformation …>
              <Title>…</Title>
              <Vendor>…</Vendor>
              <OSVersion>…</OSVersion>
              <FileVersion>…<FileVersion>
       </FileInformation>
这部分主要提供一些模板的信息，比如模板的名字，厂家，OS的版本等。
**B. OSDesignTemplate****，格式如下：**
       <OSDesignTemplate …>
              <Title>…</Title>
              <Description>…</Description>
              <CoreOSId>…</CoreOSId>
              <Settings>
                     …
              </Settings>
              <SelectionPage>
                     …
              </SelectionPage>
              <SelectionPage>
                     …
              </SelectionPage>
       </OSDesignTemplate>
这部分包含与OS相关的设置以及对OS的定制，比如编译选项，选择了OS中的哪些模块，后面会举个例子来详细介绍。
同在一个目录下还有很多其他的模板，比如”odtpda.pbcxml”是一个PDA模板，这些都是基于xml描述性语言的，都可以用记事本打开，也比较容易理解。
我们在创建工程的时候可以根据自己产品的需要选择相似的模板。问题是我们的产品不可能和微软的模板完全一致，所以在创建工程后还是要手动添加或删除一些组件，或者是更改一些设置。
**下面介绍根据自己产品的需要来为自己的产品创建一个模板：**
1. 首先复制” odtsmall_footprint.pbcxml”文件，然后改名为” odtYourProduct.pbcxml”。
2. 用记事本打开”odtYourProduct.pbcxml”，编辑**FileInformation**部分，如下：
       <FileInformation Id="FileInformation:CompanyName:YourProduct">
           <Title>Your Product Template Device</Title>
           <Vendor>CompanyName</Vendor>
           <OSVersion>6.00</OSVersion>
           <FileVersion>6.00</FileVersion>
       </FileInformation>
可以根据需要在Title中设置产品模板的名称，在Vendor中设置厂家。这部分比较简单，不多介绍了。
3. 编辑**OSDesignTemplate**部分中开始的Title和Description，如下：
       <OSDesignTemplate Id="OSDesignTemplate:CompanyName:YourProduct">
           <Title>Your Company's Product Device</Title>
           <Description>Select the product's template for your device</Description>
           <CoreOSId>CoreOS:MS:cebase</CoreOSId>
4. 编辑**OSDesignTemplate**部分中的Setting，如下：
              <Settings>
                 <!-- ################ Applications - End User ###################### -->
                 <ItemId>Item:MS:sysgen_as_base</ItemId>
                 <ItemId>Item:MS:sysgen_as_file</ItemId>
                    …
                 <!-- ########## Applications and Services Development ############## -->
                 <ItemId>Item:MS:sysgen_dotnetv35</ItemId>
                 <ItemId>Item:MS:sysgen_dotnetv35_support</ItemId>
                    …
                 <!-- ########## Communication services and Networking ############## -->
                 <ItemId>Item:MS:sysgen_eap</ItemId>
                 <ItemId>Item:MS:sysgen_gateway</ItemId>
                    …
                 <!-- #################### Core OS services ######################### -->
                 <ItemId>Item:MS:sysgen_battery</ItemId>
                 <ItemId>Item:MS:sysgen_cerdisp</ItemId>
                    …
                 <!-- ############### File System and Data Store #################### -->
                 <ItemId>Item:MS:sysgen_fsreplbit</ItemId>
                 <ItemId>Item:MS:sysgen_nkcompr</ItemId>
                    …
                 <!-- ############## USB #####################-->
                 <ItemId>Item:MS:sysgen_usb</ItemId>
                 <ItemId>Item:MS:sysgen_usb_hid</ItemId>
                    …
                 <BuildOption Configuration="Retail">IMGNODEBUGGER</BuildOption>
                 <BuildOption Configuration="Retail">IMGNOKITL</BuildOption>
                 <BuildOption Configuration="All">IMGNOTALLKMODE</BuildOption>
                 <BuildOption Configuration="Retail">WINCESHIP</BuildOption>
                 <BuildOption Configuration="All">IMGRAM64</BuildOption>  
           </Settings>
这部分包括了OS的定制，所有需要的模块都可以在这部分定义，同时对编译选项作了一些设置。比如对于Release模式，取消KITL，Full
 Kernel Mode以及允许image大于64M等。这里只是一个简单的例子，里面列举了一些OS中的模块，实际用户可以根据自己的需要添加更多的模块。
5. 在**OSDesignTemplate**中添加SelectionPage，如下：
这部分不是必须有的，没有也可以，完全是根据需要。比如有两套产品，一套是多媒体产品(MultiMedia Product)，另一套是网络产品(Network
 Product)，这两套产品根据功能不同，对OS内部模块的要求也不同，这时候就可以用SelectionPage来让用户选择所需的模块。
              <SelectionPage>
                 <Title>Company's Product</Title>
                 <Description>Select the Product Number.</Description>
                 <OptionGroup ChildMultiSelect="false">
                      <Option Default="true">
                     <Title>Company's Product Device</Title>
                     <Description>Select the Company's Product Device</Description>
                     <OptionGroup ChildMultiSelect="false">
                          <Option Default="true">
                         <Title>MultiMedia Product</Title>
                         <Description>Company's MultiMedia Device</Description>
                         <Settings>
                              <ItemId>Item:MS:sysgen_audio</ItemId>
                                          <ItemId>Item:MS:sysgen_localaudio</ItemId>
                                          <ItemId>Item:MS:sysgen_streamaudio</ItemId>
                                          <ItemId>Item:MS:sysgen_streamav</ItemId>
                                          …
                         </Settings>
                   </Option>
                   <Option>
                         <Title>Network Product</Title>
                         <Description>Company's Network Device</Description>
                         <Settings>
                              <ItemId>Item:MS:sysgen_auth</ItemId>
                                          <ItemId>Item:MS:sysgen_ethernet</ItemId>
                                    <ItemId>Item:MS:sysgen_gateway</ItemId>
                                    <ItemId>Item:MS:sysgen_gateway_logging</ItemId>
                                    <ItemId>Item:MS:sysgen_ipsec</ItemId>
                                    …
                         </Settings>
                   </Option>
                     </OptionGroup>         
               </Option>       
                 </OptionGroup>
           </SelectionPage>
在SeletionPage中包含了两个Option，一个是MultiMedia
 Product，另一个是Network Product，每个Option中的Settings里面又添加了和该产品相关的OS组件。
6. 在**OSDesignTemplate**中添加SelectionPage，如下：
可以使用SelectionPage继续添加需要用户选择的功能，比如语言，用户可以选择不同的语言。
              <SelectionPage>
                 <Title>Language Types and Features</Title>
                 <Description>Select the desired language types and features</Description>
                 <OptionGroup>
               <Option Default="true">
                     <Title>Select the desired language types</Title>         
                     <OptionGroup ChildMultiSelect="false">
                   <Option>
                         <Title>Multilingual User Interface</Title>
                         <Description>Include English, SC and TC</Description>
                         <Settings>
                              <ItemId>Item:MS:sysgen_multiui</ItemId>
                              <ItemId>Item:MS:sysgen_fonts_simsun_2_50</ItemId>
                              <ItemId>Item:MS:sysgen_mspy3_db_1</ItemId>
                              <ItemId>Item:MS:sysgen_spim_mspy_large</ItemId>
                              <ItemId>Item:MS:sysgen_gb18030</ItemId>               
                              <ItemId>Item:MS:sysgen_mspy3_sc</ItemId>
                                          …
                         </Settings>
                   </Option>
                   <Option>
                         <Title>Simplified Chinese</Title>
                         <Description>This Chinese is used in mainland China</Description>
                         <Settings>
                              <ItemId>Item:MS:sysgen_fonts_simsun_2_50</ItemId>
                              <ItemId>Item:MS:sysgen_mspy3_db_1</ItemId>
                              <ItemId>Item:MS:sysgen_spim_mspy_large</ItemId>
                              <ItemId>Item:MS:sysgen_gb18030</ItemId>                              
                              <ItemId>Item:MS:sysgen_agfa_font</ItemId>
                              <ItemId>Item:MS:sysgen_mspy3_sc</ItemId>
                         </Settings>
                   </Option>
                   <Option>
                         <Title>Traditional Chinese</Title>
                         <Description>This Chinese is used in Taiwan</Description>
                         <Settings>
                              <ItemId>Item:MS:sysgen_pime_tc</ItemId>
                              <ItemId>Item:MS:sysgen_phonim</ItemId>
                              <ItemId>Item:MS:sysgen_chajeiim</ItemId>
                              <ItemId>Item:MS:sysgen_fonts_mingliu_2_70</ItemId>
                              <ItemId>Item:MS:sysgen_agfa_font</ItemId>               
                         </Settings>
                   </Option>
                   <Option Default="true">
                         <Title>Western: Standard</Title>
                         <Description>This includes English, German, French, etc</Description>
                         <Settings>
                         </Settings>
                   </Option>        
                     </OptionGroup>
               </Option>
                 </OptionGroup>
           </SelectionPage>
7. 完成**OSDesignTemplate**部分
最后只要保证<OSDesignTemplate>…</OSDesignTemplate>就可以了。
上述步骤就算是完成了一个自定义的模板，然后我们来看看效果，首先打开VS2005创建一个WinCE6.0的工程，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/d1.JPG)
名字就命名为”YourProduct”，然后选择”Next”，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/d2.JPG)
根据自己的需要选择相应的BSP，这里选的是x86的BSP，然后下一步，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/d3.JPG)
这时候，我们可以看到我们添加的模板了，名字是”Your Company’s Product Device”，选择它，然后下一步，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/d4.JPG)
可以看到有两个产品，多媒体产品和网络产品，这里就选”MultiMedia Product”，然后下一步，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/d5.JPG)
可以看到语言选择对话框，默认选择”Western: Standard”，然后下一步，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/d6.JPG)
到这一步，已经完成了，工程已经被创建出来，这个工程就是根据自己的模板创建出来的，所以不需要做什么修改和设置，直接编译就可以了。
       最后再总结一下：其实微软提供的所有的工程模板都是一些基于xml的文件，这些文件的内容还是比较容易理解的。我们可以选择微软的模板创建工程，然后根据需要来更该设置或者添加删除组件。另一种方法就是根据产品需要添加自己的模板，在该模板中选择必要的OS组件以及设置，这样任何人都可以使用该模板来创建并编译工程了。
    下面是我自己创建的一个工程模板的代码，应该可以使用，仅供参考：
<?xml version="1.0" encoding="utf-8" ?>
<!--
Copyright (c) Microsoft Corporation. All rights reserved.
-->
<!--
Use of this source code is subject to the terms of the Microsoft end-user
license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
If you did not accept the terms of the EULA, you are not authorized to use
this source code. For a copy of the EULA, please see the LICENSE.RTF on your
install media.
-->
<CatalogFile xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" RequiredVersion="6.00" xsi:schemaLocation="urn:Microsoft.PlatformBuilder/Catalog PbcXml600.xsd" xmlns="urn:Microsoft.PlatformBuilder/Catalog">
 <FileInformation Id="FileInformation:CompanyName:YourProduct">
    <Title>Your Product Template Device</Title>
    <Vendor>CompanyName</Vendor>
    <OSVersion>6.00</OSVersion>
    <FileVersion>6.00</FileVersion>
 </FileInformation>
 <OSDesignTemplate Id="OSDesignTemplate:CompanyName:YourProduct">
    <Title>Your Company's Product Device</Title>
    <Description>Select the product's template for your device</Description>
    <CoreOSId>CoreOS:MS:cebase</CoreOSId>
    <Settings>
      <!-- ##################### Applications - End User ###################### -->
      <ItemId>Item:MS:sysgen_as_base</ItemId>
      <ItemId>Item:MS:sysgen_as_file</ItemId>
      <ItemId>Item:MS:sysgen_wceload</ItemId>
      <ItemId>Item:MS:sysgen_help</ItemId>
      <ItemId>Item:MS:sysgen_rdp</ItemId>
      <ItemId>Item:MS:sysgen_rdp_audio</ItemId>
      <ItemId>Item:MS:sysgen_rdp_clipboard</ItemId>
      <ItemId>Item:MS:sysgen_rdp_drive</ItemId>
      <ItemId>Item:MS:sysgen_rdp_printer</ItemId>
      <ItemId>Item:MS:sysgen_rdp_port</ItemId>
      <ItemId>Item:MS:sysgen_rdp_smartcard</ItemId>
      <ItemId>Item:MS:sysgen_rdp_ui</ItemId>
      <ItemId>Item:MS:sysgen_pegterm</ItemId>
      <!-- ############### Applications and Services Development ############## -->
      <ItemId>Item:MS:sysgen_dotnetv35</ItemId>
      <ItemId>Item:MS:sysgen_dotnetv35_support</ItemId>
      <ItemId>Item:MS:sysgen_atl</ItemId>
      <ItemId>Item:MS:sysgen_cpp_eh_and_rtti</ItemId>
      <ItemId>Item:MS:sysgen_full_crt</ItemId>
      <ItemId>Item:MS:sysgen_stdio</ItemId>
      <ItemId>Item:MS:sysgen_stdioa</ItemId>
      <ItemId>Item:MS:sysgen_corestra</ItemId>
      <ItemId>Item:MS:sysgen_ole</ItemId>
      <ItemId>Item:MS:sysgen_ole_guids</ItemId>
      <ItemId>Item:MS:sysgen_ole_stg</ItemId>
      <ItemId>Item:MS:sysgen_dcom</ItemId>
      <ItemId>Item:MS:sysgen_dcom_stg</ItemId>
      <ItemId>Item:MS:sysgen_ldap</ItemId>
      <ItemId>Item:MS:sysgen_msmq</ItemId>
      <ItemId>Item:MS:sysgen_msmq_srmp</ItemId>
      <ItemId>Item:MS:sysgen_soaptk_client</ItemId>
      <ItemId>Item:MS:sysgen_msxml_dom</ItemId>
      <ItemId>Item:MS:sysgen_msxml_http</ItemId>
      <ItemId>Item:MS:sysgen_msxml_xql</ItemId>
      <ItemId>Item:MS:sysgen_msxml_sax</ItemId>
      <ItemId>Item:MS:sysgen_msxml_mini</ItemId>
      <!-- ############### Communication services and Networking ############## -->
      <ItemId>Item:MS:sysgen_eap</ItemId>
      <ItemId>Item:MS:sysgen_gateway</ItemId>
      <ItemId>Item:MS:sysgen_ipsec</ItemId>
      <ItemId>Item:MS:sysgen_ndisuio</ItemId>
      <ItemId>Item:MS:sysgen_ndis</ItemId>
      <ItemId>Item:MS:sysgen_netutils</ItemId>
      <ItemId>Item:MS:sysgen_remoteconfig_framework</ItemId>
      <ItemId>Item:MS:sysgen_tcpip</ItemId>
      <ItemId>Item:MS:sysgen_iphlpapi</ItemId>
      <ItemId>Item:MS:sysgen_tcpip6</ItemId>
      <ItemId>Item:MS:sysgen_upnp_ctrl</ItemId>
      <ItemId>Item:MS:sysgen_redir</ItemId>
      <ItemId>Item:MS:sysgen_winsock</ItemId>
      <ItemId>Item:MS:sysgen_ethernet</ItemId>
      <ItemId>Item:MS:sysgen_ppp</ItemId>
      <ItemId>Item:MS:sysgen_autoras</ItemId>
      <ItemId>Item:MS:sysgen_modem</ItemId>
      <ItemId>Item:MS:sysgen_pppoe</ItemId>
      <ItemId>Item:MS:sysgen_tapi</ItemId>
      <ItemId>Item:MS:sysgen_unimodem</ItemId>
      <ItemId>Item:MS:sysgen_l2tp</ItemId>
      <ItemId>Item:MS:sysgen_pptp</ItemId>
      <ItemId>Item:MS:sysgen_services</ItemId>
      <ItemId>Item:MS:sysgen_smb_file</ItemId>
      <ItemId>Item:MS:sysgen_smb_ui</ItemId>
      <ItemId>Item:MS:sysgen_ftpd</ItemId>
      <ItemId>Item:MS:sysgen_ppp_server</ItemId>
      <ItemId>Item:MS:sysgen_timesvc_dst</ItemId>
      <ItemId>Item:MS:sysgen_telnetd</ItemId>
      <ItemId>Item:MS:sysgen_httpd</ItemId>
      <ItemId>Item:MS:sysgen_asp</ItemId>
      <ItemId>Item:MS:sysgen_asp_jscript</ItemId>
      <ItemId>Item:MS:sysgen_asp_vbscript</ItemId>
      <ItemId>Item:MS:sysgen_isapi_config</ItemId>
      <!-- ######################### Core OS services ######################### -->
      <ItemId>Item:MS:sysgen_battery</ItemId>
      <ItemId>Item:MS:sysgen_cerdisp</ItemId>
      <ItemId>Item:MS:sysgen_toolhelp</ItemId>
      <ItemId>Item:MS:sysgen_device</ItemId>
      <ItemId>Item:MS:sysgen_display</ItemId>
      <ItemId>Item:MS:sysgen_fiber</ItemId>
      <ItemId>Item:MS:sysgen_fmtmsg</ItemId>
      <ItemId>Item:MS:sysgen_nkmapfile</ItemId>
      <ItemId>Item:MS:sysgen_msgqueue</ItemId>
      <ItemId>Item:MS:sysgen_notify</ItemId>
      <ItemId>Item:MS:sysgen_pm</ItemId>
      <ItemId>Item:MS:sysgen_ceddk</ItemId>
      <!-- ######################## Device management ######################### -->
      <ItemId>Item:MS:sysgen_snmp</ItemId>
      <!-- #################### File System and Data Store #################### -->
      <ItemId>Item:MS:sysgen_fsreplbit</ItemId>
      <ItemId>Item:MS:sysgen_nkcompr</ItemId>
      <ItemId>Item:MS:sysgen_fsdbase</ItemId>
      <ItemId>Item:MS:sysgen_fsreghive</ItemId>
      <ItemId>Item:MS:sysgen_fsramrom</ItemId>
      <ItemId>Item:MS:sysgen_storemgr</ItemId>
      <ItemId>Item:MS:sysgen_udfs</ItemId>
      <ItemId>Item:MS:sysgen_fatfs</ItemId>
      <ItemId>Item:MS:sysgen_mspart</ItemId>
      <ItemId>Item:MS:sysgen_storemgr_cpl</ItemId>
      <ItemId>Item:MS:sysgen_fspassword</ItemId>
      <ItemId>Item:MS:sysgen_exfat</ItemId>
      <ItemId>Item:MS:sysgen_tfat</ItemId>
      <!-- ############################## Fonts ############################### -->
      <ItemId>Item:MS:sysgen_fonts_arial_1_30</ItemId>
      <ItemId>Item:MS:sysgen_fonts_arialk</ItemId>
      <ItemId>Item:MS:sysgen_fonts_arialbd</ItemId>
      <ItemId>Item:MS:sysgen_fonts_arialbi</ItemId>
      <ItemId>Item:MS:sysgen_fonts_ariali</ItemId>
      <ItemId>Item:MS:sysgen_fonts_cour_1_30</ItemId>
      <ItemId>Item:MS:sysgen_fonts_courbd</ItemId>
      <ItemId>Item:MS:sysgen_fonts_courbi</ItemId>
      <ItemId>Item:MS:sysgen_fonts_cour_1_30</ItemId>
      <ItemId>Item:MS:sysgen_fonts_symbol</ItemId>
      <ItemId>Item:MS:sysgen_fonts_tahoma_1_07</ItemId>
      <ItemId>Item:MS:sysgen_fonts_couri</ItemId>
      <ItemId>Item:MS:sysgen_fonts_timesbd</ItemId>
      <ItemId>Item:MS:sysgen_fonts_timesbi</ItemId>
      <ItemId>Item:MS:sysgen_fonts_timesi</ItemId>
      <ItemId>Item:MS:sysgen_fonts_webdings</ItemId>
      <ItemId>Item:MS:sysgen_fonts_wingding</ItemId>
      <!-- ############## Graphics and Multimedia Technologies ################ -->
      <ItemId>Item:MS:sysgen_gradfill</ItemId>
      <ItemId>Item:MS:sysgen_imaging</ItemId>
      <ItemId>Item:MS:sysgen_imaging_bmp_decode</ItemId>
      <ItemId>Item:MS:sysgen_imaging_gif_decode</ItemId>
      <ItemId>Item:MS:sysgen_imaging_ico_decode</ItemId>
      <ItemId>Item:MS:sysgen_imaging_jpg_decode</ItemId>
      <ItemId>Item:MS:sysgen_imaging_bmp_encode</ItemId>
      <ItemId>Item:MS:sysgen_imaging_jpg_encode</ItemId>
      <ItemId>Item:MS:sysgen_imaging_gif_encode</ItemId>
      <ItemId>Item:MS:sysgen_imaging_png_encode</ItemId>
      <!-- ########################## Internal ################################ -->
      <ItemId>Item:MS:sysgen_imm</ItemId>
      <ItemId>Item:MS:sysgen_coreloc</ItemId>
      <!-- ################## Internet Client Services ######################## -->
      <!-- #################################################################### -->
      <ItemId>Item:MS:sysgen_ie</ItemId>
      <ItemId>Item:MS:sysgen_iesample</ItemId>
      <ItemId>Item:MS:sysgen_shdocvw</ItemId>
      <ItemId>Item:MS:sysgen_mshtml</ItemId>
      <ItemId>Item:MS:sysgen_mshtml_htmlapp</ItemId>
      <ItemId>Item:MS:sysgen_ie_imgutil</ItemId>
      <ItemId>Item:MS:sysgen_ie_pngfilt</ItemId>
      <ItemId>Item:MS:sysgen_mlang</ItemId>
      <ItemId>Item:MS:sysgen_mlangall</ItemId>
      <ItemId>Item:MS:sysgen_mlangreg</ItemId>
      <ItemId>Item:MS:sysgen_urlmon</ItemId>
      <ItemId>Item:MS:sysgen_wininet</ItemId>
      <ItemId>Item:MS:sysgen_inetcpl</ItemId>
      <ItemId>Item:MS:sysgen_ie_jscript</ItemId>
      <ItemId>Item:MS:sysgen_jscript_author</ItemId>
      <ItemId>Item:MS:sysgen_jscript_encode</ItemId>
      <ItemId>Item:MS:sysgen_ie_vbscript</ItemId>
      <ItemId>Item:MS:sysgen_vbscript_msgbox</ItemId>
      <ItemId>Item:MS:sysgen_vbscript_author</ItemId>
      <ItemId>Item:MS:sysgen_vbscript_encode</ItemId>
      <!-- ############################## Security ############################ -->
      <ItemId>Item:MS:sysgen_auth</ItemId>
      <ItemId>Item:MS:sysgen_auth_kerberos</ItemId>
      <ItemId>Item:MS:sysgen_auth_ntlm</ItemId>
      <ItemId>Item:MS:sysgen_auth_schannel</ItemId>
      <ItemId>Item:MS:sysgen_credman</ItemId>
      <ItemId>Item:MS:sysgen_crypto</ItemId>
      <ItemId>Item:MS:sysgen_certs</ItemId>
      <ItemId>Item:MS:sysgen_crypto_dssdh</ItemId>
      <ItemId>Item:MS:sysgen_crypto_scwcsp</ItemId>
      <!-- ##################### Shell and User Interface ##################### -->
      <ItemId>Item:MS:sysgen_mingdi</ItemId>
      <ItemId>Item:MS:sysgen_mingwes</ItemId>
      <ItemId>Item:MS:sysgen_mininput</ItemId>
      <ItemId>Item:MS:sysgen_minwmgr</ItemId>
      <ItemId>Item:MS:sysgen_aygshell</ItemId>
      <ItemId>Item:MS:sysgen_cmd</ItemId>
      <ItemId>Item:MS:sysgen_console</ItemId>
      <ItemId>Item:MS:sysgen_standardshell</ItemId>
      <ItemId>Item:MS:sysgen_commctrl</ItemId>
      <ItemId>Item:MS:sysgen_commdlg</ItemId>
      <ItemId>Item:MS:sysgen_ctlpnl</ItemId>
      <ItemId>Item:MS:sysgen_xpskin</ItemId>
      <ItemId>Item:MS:sysgen_cursor</ItemId>
      <ItemId>Item:MS:sysgen_connmc</ItemId>
      <ItemId>Item:MS:sysgen_msim</ItemId>
      <ItemId>Item:MS:sysgen_softkb</ItemId>
      <ItemId>Item:MS:sysgen_touch</ItemId>
      <!-- ############## SerialPort #####################-->
      <ItemId>Item:MS:sysgen_serdev</ItemId>
      <!-- ############## ParallelPort #####################-->
      <ItemId>Item:MS:sysgen_parallel</ItemId>
      <!-- ############## USB #####################-->
      <ItemId>Item:MS:sysgen_usb</ItemId>
      <ItemId>Item:MS:sysgen_usb_hid</ItemId>
      <ItemId>Item:MS:sysgen_usb_hid_clients</ItemId>
      <ItemId>Item:MS:sysgen_usb_hid_keyboard</ItemId>
      <ItemId>Item:MS:sysgen_usb_hid_mouse</ItemId>
      <ItemId>Item:MS:sysgen_usb_printer</ItemId>
      <ItemId>Item:MS:sysgen_eth_usb_host</ItemId>
      <ItemId>Item:MS:sysgen_usb_storage</ItemId>
      <ItemId>Item:MS:sysgen_printing</ItemId>
      <ItemId>Item:MS:sysgen_pcl</ItemId>     
      <BuildOption Configuration="Retail">IMGNODEBUGGER</BuildOption>
      <BuildOption Configuration="Retail">IMGNOKITL</BuildOption>
      <BuildOption Configuration="All">IMGNOTALLKMODE</BuildOption>
      <BuildOption Configuration="Retail">WINCESHIP</BuildOption>
      <BuildOption Configuration="All">IMGRAM64</BuildOption>  
    </Settings>
    <SelectionPage>
      <Title>Company's Product</Title>
      <Description>Select the Product Number.</Description>
      <OptionGroup ChildMultiSelect="false">
        <Option Default="true">
          <Title>Company's Product Device</Title>
          <Description>Select the Company's Product Device</Description>
          <OptionGroup ChildMultiSelect="false">
            <Option Default="true">
              <Title>MultiMedia Product</Title>
              <Description>Company's MultiMedia Device</Description>
              <Settings>
                <ItemId>Item:MS:sysgen_audio</ItemId>
               <ItemId>Item:MS:sysgen_auth</ItemId>
               <ItemId>Item:MS:sysgen_auth_ntlm</ItemId>
               <ItemId>Item:MS:sysgen_auth_schannel</ItemId>
               <ItemId>Item:MS:sysgen_certs</ItemId>
               <ItemId>Item:MS:sysgen_cmd</ItemId>
               <ItemId>Item:MS:sysgen_connmc</ItemId>
               <ItemId>Item:MS:sysgen_console</ItemId>
               <ItemId>Item:MS:sysgen_crypto</ItemId>
               <ItemId>Item:MS:sysgen_cursor</ItemId>
               <ItemId>Item:MS:sysgen_ddraw</ItemId>
               <ItemId>Item:MS:sysgen_dshow_display</ItemId>
               <ItemId>Item:MS:sysgen_iabase</ItemId>
               <ItemId>Item:MS:sysgen_iesample</ItemId>
               <ItemId>Item:MS:sysgen_msxml_dom</ItemId>
               <ItemId>Item:MS:sysgen_netutils</ItemId>
               <ItemId>Item:MS:sysgen_notify</ItemId>
               <ItemId>Item:MS:sysgen_pm</ItemId>
               <ItemId>Item:MS:sysgen_redir</ItemId>
               <ItemId>Item:MS:sysgen_services</ItemId>
               <ItemId>Item:MS:sysgen_shell</ItemId>
               <ItemId>Item:MS:sysgen_standardshell</ItemId>
               <ItemId>Item:MS:sysgen_touch</ItemId>
               <ItemId>Item:MS:sysgen_usb</ItemId>
               <ItemId>Item:MS:sysgen_usb_printer</ItemId>
               <ItemId>Item:MS:sysgen_usb_storage</ItemId>
               <ItemId>Item:MS:sysgen_wininet</ItemId>
              <ItemId>Item:MS:sysgen_localaudio</ItemId>
              <ItemId>Item:MS:sysgen_streamaudio</ItemId>
              <ItemId>Item:MS:sysgen_streamav</ItemId>
              </Settings>
            </Option>
            <Option>
              <Title>Network Product</Title>
              <Description>Company's Network Device</Description>
              <Settings>
                <ItemId>Item:MS:sysgen_auth</ItemId>
              <ItemId>Item:MS:sysgen_ethernet</ItemId>
               <ItemId>Item:MS:sysgen_fsdbase</ItemId>
               <ItemId>Item:MS:sysgen_gateway</ItemId>
               <ItemId>Item:MS:sysgen_gateway_logging</ItemId>
               <ItemId>Item:MS:sysgen_ipsec</ItemId>
               <ItemId>Item:MS:sysgen_nkcompr</ItemId>
               <ItemId>Item:MS:sysgen_shell</ItemId>
               <ItemId>Item:MS:sysgen_timesvc_api</ItemId>
               <ItemId>Item:MS:sysgen_timesvc_dst</ItemId>
               <ItemId>Item:MS:sysgen_timesvc_sntp</ItemId>
              <ItemId>Item:MS:sysgen_watson_dmpgen</ItemId>
              <ItemId>Item:MS:sysgen_watson_client</ItemId>
              <ItemId>Item:MS:sysgen_watson_client</ItemId>
              <ItemId>Item:MS:sysgen_watson_client_ui</ItemId>
              <ItemId>Item:MS:sysgen_watson_xfer</ItemId>
              </Settings>
            </Option>
          </OptionGroup>         
        </Option>       
      </OptionGroup>
    </SelectionPage>
    <SelectionPage>
      <Title>Language Types and Features</Title>
      <Description>Select the desired language types and features</Description>
      <OptionGroup>
        <Option Default="true">
          <Title>Select the desired language types</Title>         
          <OptionGroup ChildMultiSelect="false">
            <Option>
              <Title>Multilingual User Interface</Title>
              <Description>This include English, Simplified Chinese and Traditional Chinese</Description>
              <Settings>
                <ItemId>Item:MS:sysgen_multiui</ItemId>
                <ItemId>Item:MS:sysgen_fonts_simsun_2_50</ItemId>
                <ItemId>Item:MS:sysgen_mspy3_db_1</ItemId>
                <ItemId>Item:MS:sysgen_spim_mspy_large</ItemId>
                <ItemId>Item:MS:sysgen_gb18030</ItemId>               
                <ItemId>Item:MS:sysgen_mspy3_sc</ItemId>
                <ItemId>Item:MS:sysgen_pime_tc</ItemId>
                <ItemId>Item:MS:sysgen_phonim</ItemId>
                <ItemId>Item:MS:sysgen_chajeiim</ItemId>
                <ItemId>Item:MS:sysgen_fonts_mingliu_2_70</ItemId>
                <ItemId>Item:MS:sysgen_agfa_font</ItemId>
                <ItemId>Item:MS:sysgen_fonts_msgothic</ItemId>
              </Settings>
            </Option>
            <Option>
              <Title>Simplified Chinese</Title>
              <Description>This Chinese is used in mainland China</Description>
              <Settings>
                <ItemId>Item:MS:sysgen_fonts_simsun_2_50</ItemId>
                <ItemId>Item:MS:sysgen_mspy3_db_1</ItemId>
                <ItemId>Item:MS:sysgen_spim_mspy_large</ItemId>
                <ItemId>Item:MS:sysgen_gb18030</ItemId>                              
                <ItemId>Item:MS:sysgen_agfa_font</ItemId>
                <ItemId>Item:MS:sysgen_mspy3_sc</ItemId>
              </Settings>
            </Option>
            <Option>
              <Title>Traditional Chinese</Title>
              <Description>This Chinese is used in Taiwan and Hongkong</Description>
              <Settings>
                <ItemId>Item:MS:sysgen_pime_tc</ItemId>
                <ItemId>Item:MS:sysgen_phonim</ItemId>
                <ItemId>Item:MS:sysgen_chajeiim</ItemId>
                <ItemId>Item:MS:sysgen_fonts_mingliu_2_70</ItemId>
                <ItemId>Item:MS:sysgen_agfa_font</ItemId>               
              </Settings>
            </Option>
           <Option Default="true">
              <Title>Western: Standard</Title>
              <Description>This includes English, German, French, Italian, etc</Description>
              <Settings>
              </Settings>
            </Option>        
          </OptionGroup>
        </Option>
      </OptionGroup>
    </SelectionPage>
 </OSDesignTemplate>
</CatalogFile>
