# eclipse的项目和配置文件 .project .cproject .classpath .metadata - DoubleLi - 博客园






# Clean out Eclipse workspace metadata

[http://stackoverflow.com/questions/11768106/clean-out-eclipse-workspace-metadata](http://stackoverflow.com/questions/11768106/clean-out-eclipse-workspace-metadata)

There is no easy way to remove the "outdated" stuff from an existing workspace. 
Using the "clean" parameter will not really help, as many of the files you refer to are "free form data", 
only known to the plugins that are no longer available.

Your best bet is to optimize the re-import, where I would like to point out the following:
- When creating a new workspace, you can already choose to have some settings being copied from the current to the new workspace.
- You can export the preferences of the current workspace (using the *Export* menu) and re-import them in the new workspace.
- There are lots of recommendations on the Internet to just copy the
`${old_workspace}/.metadata/.plugins/org.eclipse.core.runtime/.settings` folder from the old to the new workspace. 
This is surely the fastest way, but it may lead to weird behaviour, because some of your plugins may depend on these settings 
*and* on some of the mentioned "free form data" stored elsewhere. 
(There are even people symlinking these folders over multiple workspaces, but this really requires to use the same plugins on all workspaces.)
- You may want to consider using more project specific settings than workspace preferences in the future. 
So for instance all the Java compiler settings can either be set on the workspace level or on the project level. 
If set on the project level, you can put them under version control and are independent of the workspace.











