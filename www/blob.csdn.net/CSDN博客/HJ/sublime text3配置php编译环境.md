# sublime  text3配置php编译环境 - HJ - CSDN博客
2017年01月25日 15:57:30[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：5893
首先在Window环境安装配置好运行环境（安装PHP执行引擎），或者使用xampp集成环境 
如图（表示已经配置成功） 
![这里写图片描述](https://img-blog.csdn.net/20170125154759901?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
(2) 安装 SublimeLinter 和 SublimeLinter-php 组件 
Ctrl + Shift + P 唤醒后输入：pic，打开包管理文件，然后输入 sublimelinter，选择选装；然后再pic唤醒输入：sublimelinter-php 
安装完插件以后去配置一下插件相关配置，在 Sublime Text 3菜单打开：Preferences -> Package Setting -> SublimeLinter -> Setting User，把如下内容填充进去：
```
{
    "user": {
        "debug": false,
        "delay": 0.25,
        "error_color": "D02000",
        "gutter_theme": "Packages/SublimeLinter/gutter-themes/Default/Default.gutter-theme",
        "gutter_theme_excludes": [],
        "lint_mode": "background",
        "linters": {
            "annotations": {
                "@disable": false,
                "args": [],
                "errors": [
                    "FIXME"
                ],
                "excludes": [],
                "warnings": [
                    "NOTE",
                    "README",
                    "TODO",
                    "XXX",
                    "@todo"
                ]
            },
            "php": {
                "@disable": false,
                "args": [],
                "excludes": []
            },
            "summitlinter": {
                "@disable": false,
                "args": [],
                "excludes": [],
                "globals": "",
                "ignore": "channel",
                "limit": null,
                "only": ""
            }
        },
        "mark_style": "outline",
        "no_column_highlights_line": false,
        "passive_warnings": false,
        "paths": {
            "linux": [],
            "osx": [],
            "windows": [
                "c:\\xampp\\php"
            ]
        },
        "python_paths": {
            "linux": [],
            "osx": [],
            "windows": []
        },
        "rc_search_limit": 3,
        "shell_timeout": 10,
        "show_errors_on_save": false,
        "show_marks_in_minimap": true,
        "sublimelinter": "save-only",
        "sublimelinter_executable_map": {
            "php": "c:\\xampp\\php\\php.exe"
        },
        "syntax_map": {
            "html (django)": "html",
            "html (rails)": "html",
            "html 5": "html",
            "javascript (babel)": "javascript",
            "magicpython": "python",
            "php": "php",
            "python django": "python",
            "pythonimproved": "python"
        },
        "warning_color": "DDB700",
        "wrap_find": true
    }
}
```
说明：注意里面的两个php路径必须跟我们实际php脚本引擎路径一致。
(3) 保存配置文件后重启Sublime编辑器，然后编辑任何php代码文件，如果代码语法有错误都会及时提醒啦。 
![这里写图片描述](https://img-blog.csdn.net/20170125155712718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
