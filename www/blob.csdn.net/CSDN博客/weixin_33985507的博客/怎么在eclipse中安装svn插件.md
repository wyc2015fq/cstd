# 怎么在eclipse中安装svn插件 - weixin_33985507的博客 - CSDN博客
2016年12月23日 18:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
# Subclipse
Subclipse is an Eclipse Team Provider plug-in providing support for Subversion within the Eclipse IDE.  Visit the [Subclipse Website(link is external)](http://subclipse.tigris.org/) for more information.
## Version
- CentOS 5.x
- Eclipse 3.4.x
- Subclipse 1.6.x
## Requirements
- [Install and Configure Eclipse](http://dev.antoinesolutions.com/eclipse)
## Install Subclipse
- Open Eclipse
- Select the **Help > Software Updates** menu option
- Select the **Available Software** tab
- Click the **Add Site…** button- Set the Location field
**[http://subclipse.tigris.org/update_1.6.x(link is external)](http://subclipse.tigris.org/update_1.6.x)**
- Click the **OK** button
- Expand the **[http://subclipse.tigris.org/update_1.6.x(link is external)](http://subclipse.tigris.org/update_1.6.x)**site
- Excpand the **Core SVNKit Library (required)** section
- Select the following components
**SVNKit Library**
- Expand the **Subclipse** section
- Select the following components
**Subclipse (Required)Subversion Client Adapter (Required)Subversion Revision GraphSVNKit Client Adapter BETA (Not required)**
- Click the **Install…** button
- Click the **Next** button
- Select the **I accept the terms of the license agreements** radio button
- Click the **Finish** button
- Click **Yes** to restart Eclipse
## Browse a Subversion Repository
- Open the Subclipse repository perspective
- Select the **Window > Open Perspective > Other…** menu option
- Select the **SVN Repository Exploring** option
- Click **OK**
- Right Click in the **SVN Repositories** panel
- Select the **New > Repository Location…** option
- Enter your repository **URL** (must begin with http or https)
- Click **Finish**
## Create a project from a repository
- Right Click a repository in the **SVN Repositories** panel
- Select the **Checkout…** option
- Select the **Check out as a project in the workspace** option
- Enter a **Project Name**
- Select the **Revision** you wish to check out
- Click **Next**
- Select a workspace location
- Click **Finish**

