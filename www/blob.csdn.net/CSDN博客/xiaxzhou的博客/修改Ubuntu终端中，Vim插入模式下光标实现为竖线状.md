# 修改Ubuntu终端中，Vim插入模式下光标实现为竖线状 - xiaxzhou的博客 - CSDN博客





2017年11月12日 20:57:31[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：2831








原文：[Change cursor shape in different modes](http://vim.wikia.com/wiki/Change_cursor_shape_in_different_modes)

不同终端版本修改代码不同。

使用如下命令查看Gnome-Terminal终端的版本：
`aptitude show gnome-terminal`
To change the shape of the cursor in different modes, you can add the following into your vimrc.

将不同版本终端对应的代码添加到vimrc文件中即可。

我的终端版本为3.18***，vimrc文件位于etc/vim/vimrc

# For the Gnome-Terminal (version 2.26)Edit

This makes your cursor change in all open terminals. Upgrade your terminal and use the version above instead.

```
if has("autocmd")
  au InsertEnter * silent execute "!gconftool-2 --type string --set /apps/gnome-terminal/profiles/Default/cursor_shape ibeam"
  au InsertLeave * silent execute "!gconftool-2 --type string --set /apps/gnome-terminal/profiles/Default/cursor_shape block"
  au VimLeave * silent execute "!gconftool-2 --type string --set /apps/gnome-terminal/profiles/Default/cursor_shape ibeam"
endif
```

# For the Gnome-Terminal (version 3.x)Edit

First make a script gnome-terminal-cursor-shape.sh

```bash
#!/bin/sh
DEFAULTPROF=`dconf read /org/gnome/terminal/legacy/profiles:/default`
DEFAULTPROF=`echo "$DEFAULTPROF" | sed -e "s/^'/:/" -e "s/'$//"`
dconf write /org/gnome/terminal/legacy/profiles:/$DEFAULTPROF/cursor-shape "'$1'"
```

Make it executable & put it in /usr/local/bin Next, add this to .vimrc

```php
if has("autocmd")
    au InsertEnter *
        \ if v:insertmode == 'i' |
        \   silent execute "!gnome-terminal-cursor-shape.sh ibeam" |
        \ elseif v:insertmode == 'r' |
        \   silent execute "!gnome-terminal-cursor-shape.sh underline" |
        \ endif
    au InsertLeave * silent execute "!gnome-terminal-cursor-shape.sh block"
    au VimLeave * silent execute "!gnome-terminal-cursor-shape.sh block"
endif
```

If you use more than one profile in gnome-terminal, you might have to adapt this to your profiles.

# For the Gnome-Terminal (version ≥3.16) Edit

This method has the advantage compared to the method above that it influences the given tab only, and not all the tabs that use the given profile. Add the following into .vimrc

```php
if has("autocmd")
  au VimEnter,InsertLeave * silent execute '!echo -ne "\e[2 q"' | redraw!
  au InsertEnter,InsertChange *
    \ if v:insertmode == 'i' | 
    \   silent execute '!echo -ne "\e[6 q"' | redraw! |
    \ elseif v:insertmode == 'r' |
    \   silent execute '!echo -ne "\e[4 q"' | redraw! |
    \ endif
  au VimLeave * silent execute '!echo -ne "\e[ q"' | redraw!
endif
```

Above snippet disables cursor blinking. If you want cursor blink, just decrement all the 3 numbers above 2,4,6 by one each.

```php
if has("autocmd")
  au VimEnter,InsertLeave * silent execute '!echo -ne "\e[1 q"' | redraw!
  au InsertEnter,InsertChange *
    \ if v:insertmode == 'i' | 
    \   silent execute '!echo -ne "\e[5 q"' | redraw! |
    \ elseif v:insertmode == 'r' |
    \   silent execute '!echo -ne "\e[3 q"' | redraw! |
    \ endif
  au VimLeave * silent execute '!echo -ne "\e[ q"' | redraw!
endif
```



