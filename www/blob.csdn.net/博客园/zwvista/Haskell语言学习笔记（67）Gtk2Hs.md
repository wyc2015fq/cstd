# Haskell语言学习笔记（67）Gtk2Hs - zwvista - 博客园

## [Haskell语言学习笔记（67）Gtk2Hs](https://www.cnblogs.com/zwvista/p/8149012.html)

### Gtk2Hs

```
$ brew cask install xquartz
$ brew install glib cairo gtk gettext fontconfig freetype
$ export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:
$ cabal install alex happy
$ cabal install gtk2hs-buildtools
Installed gtk2hs-buildtools-0.13.3.1
$ cabal install glib
$ cabal install gtk -f have-quartz-gtk
Installed gtk-0.14.7
```

### window

```
import Graphics.UI.Gtk

main :: IO ()
main = do
  initGUI
  window <- windowNew
  widgetShowAll window
  mainGUI
```


