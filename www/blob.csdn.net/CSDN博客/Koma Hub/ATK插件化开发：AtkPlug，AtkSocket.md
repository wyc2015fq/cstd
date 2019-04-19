# ATK插件化开发：AtkPlug，AtkSocket - Koma Hub - CSDN博客
2019年03月03日 17:10:28[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：48
个人分类：[ATK																[GTK																[设计模式																[软件工程](https://blog.csdn.net/Rong_Toa/article/category/7996455)](https://blog.csdn.net/Rong_Toa/article/category/7651431)](https://blog.csdn.net/Rong_Toa/article/category/7156203)](https://blog.csdn.net/Rong_Toa/article/category/8715793)
**Table of Contents**
[介绍](#%E4%BB%8B%E7%BB%8D)
[接口：](#%E6%8E%A5%E5%8F%A3%EF%BC%9A)
[atk_plug_get_id ()](#atk_plug_get_id%C2%A0())
[atk_socket_embed ()](#atk_socket_embed%C2%A0())
[AtkPlug例子](#AtkPlug%E4%BE%8B%E5%AD%90)
[ta-plug.c](#ta-plug.c)
[ta-plug-accessible.c](#ta-plug-accessible.c)
[ta-plug-accessible.h](#ta-plug-accessible.h)
[ta-plug-vbox.c](#ta-plug-vbox.c)
[ta-plug-vbox.h](#ta-plug-vbox.h)
[Makefile](#Makefile)
[AtkSocket例子](#AtkSocket%E4%BE%8B%E5%AD%90)
[ta-socket.c](#ta-socket.c)
[ta-socket-accessible.c](#ta-socket-accessible.c)
[ta-socket-accessible.h](#ta-socket-accessible.h)
[ta-socket-label.c](#ta-socket-label.c)
[ta-socket-label.h](#ta-socket-label.h)
[Makefile](#Makefile)
# 介绍
AtkPlug介绍：[https://developer.gnome.org/atk/unstable/atk-AtkPlug.html](https://developer.gnome.org/atk/unstable/atk-AtkPlug.html)
AtkSocket介绍：[https://developer.gnome.org/atk/stable/atk-AtkSocket.html](https://developer.gnome.org/atk/stable/atk-AtkSocket.html)
- AtkPlug-嵌入其他进程的顶层
- AtkSocket-用于来自其他进程的atkPlug对象的容器
## 接口：
> 
### atk_plug_get_id ()
```cpp
gchar *atk_plug_get_id (AtkPlug *plug);
```
获取atkPlug对象的唯一ID，该对象可用于使用atk_socket_embed（）嵌入atkSocket的内部。
在内部，这会调用一个类函数，该函数应该由IPC层（通常是AT-SPI2-ATK）注册。atkPlug对象的实现者应该调用这个函数（在加载atk桥之后），并将值传递给实现atkSocket的进程，这样它就可以嵌入这个插件。
> 
### atk_socket_embed ()
```cpp
void atk_socket_embed (AtkSocket *obj,gchar *plug_id);
```
嵌入atkPlug的子级作为atkSocket的子级。插头可能处于同一进程或不同进程中。
这个函数使用的类项应该由IPC层（通常是AT-SPI2-ATK）填充。atksocket的实现者应该调用这个函数并传递atk_plug_get_id（）返回的插件ID。应用程序负责根据需要将插件ID传递给实现atksocket的进程。
# AtkPlug例子
## ta-plug.c
```cpp
/*
 * Copyright (C) 2011 Igalia S.L.
 * Authors: Mario Sanchez Prada <msanchez@igalia.com>
 *
 * Based on a C# example written in C# by Mike Gorse:
 * http://mgorse.freeshell.org/plug-socket-test.tar.gz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */
#include "ta-plug-vbox.h"
#include <gtk/gtk.h>
#define WINDOW_WIDTH 350
#define WINDOW_HEIGHT -1
static void
_button_clicked_cb (GtkButton *button, gpointer data)
{
  GtkWindow *window = GTK_WINDOW (data);
  GtkWidget *dialog =
    gtk_message_dialog_new (window,
                            GTK_DIALOG_MODAL,
                            GTK_MESSAGE_INFO,
                            GTK_BUTTONS_CLOSE,
                            "I told you so:\nthe button does ALMOST nothing",
                            NULL);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}
int
main (int argc, char**argv)
{
  GtkWidget *window = NULL;
  GtkWidget *hbox = NULL;
  GtkWidget *plug = NULL;
  GtkWidget *widget = NULL;
  gchar *plug_id = NULL;
  gchar *entry_txt = NULL;
  gtk_init (&argc, &argv);
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  plug = ta_plug_vbox_new (FALSE, 0);
  plug_id = ta_plug_vbox_get_id (TA_PLUG_VBOX (plug));
  if (plug_id)
    g_print ("[PLUG] Id is %s\n", plug_id);
  else
    g_print ("Not a valid ID found\n");
  hbox = gtk_hbox_new (FALSE, 0);
  widget = gtk_label_new ("Plug ID: ");
  gtk_box_pack_start (GTK_BOX (hbox), widget, FALSE, FALSE, 6);
  widget = gtk_entry_new ();
  entry_txt = g_strdup_printf ("%s", plug_id);
  gtk_entry_set_text (GTK_ENTRY (widget), entry_txt);
  gtk_box_pack_start (GTK_BOX (hbox), widget, TRUE, TRUE, 6);
  gtk_box_pack_start (GTK_BOX (plug), hbox, TRUE, TRUE, 6);
  widget = gtk_button_new_with_label ("A button which does almost nothing");
  g_signal_connect (widget, "clicked", G_CALLBACK (_button_clicked_cb), window);
  gtk_box_pack_start (GTK_BOX (plug), widget, FALSE, FALSE, 6);
  g_free (entry_txt);
  g_free (plug_id);
  gtk_container_add (GTK_CONTAINER (window), plug);
  gtk_window_set_default_size (GTK_WINDOW (window),
                               WINDOW_WIDTH, WINDOW_HEIGHT);
  gtk_widget_show_all (window);
  gtk_main ();
}
```
## ta-plug-accessible.c
```cpp
/*
 * Copyright (C) 2011 Igalia S.L.
 * Authors: Mario Sanchez Prada <msanchez@igalia.com>
 *
 * Based on a C# example written in C# by Mike Gorse:
 * http://mgorse.freeshell.org/plug-socket-test.tar.gz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */
#include "ta-plug-accessible.h"
#include <atk/atk.h>
#include <glib.h>
#include <gtk/gtk.h>
struct _TaPlugAccessiblePrivate {
  GtkWidget *widget;
};
#define TA_PLUG_ACCESSIBLE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), TA_TYPE_PLUG_ACCESSIBLE, TaPlugAccessiblePrivate))
G_DEFINE_TYPE (TaPlugAccessible, ta_plug_accessible, ATK_TYPE_PLUG);
static void ta_plug_accessible_initialize (AtkObject *object, gpointer data)
{
  g_return_if_fail (TA_IS_PLUG_ACCESSIBLE (object));
  TaPlugAccessible *plug = NULL;
  TaPlugAccessiblePrivate *priv = NULL;
  ATK_OBJECT_CLASS(ta_plug_accessible_parent_class)->initialize(object, data);
  plug = TA_PLUG_ACCESSIBLE (object);
  priv = TA_PLUG_ACCESSIBLE_GET_PRIVATE(plug);
  priv->widget = NULL;
  if (GTK_IS_WIDGET (data))
    priv->widget = GTK_WIDGET (data);
}
static const gchar *ta_plug_accessible_get_name (AtkObject *object)
{
  g_return_val_if_fail(ATK_IS_OBJECT (object), NULL);
  return "The Plug";
}
static AtkRole ta_plug_accessible_get_role (AtkObject *object)
{
  g_return_val_if_fail(ATK_IS_OBJECT (object), ATK_ROLE_UNKNOWN);
  return ATK_ROLE_PANEL;
}
static gint ta_plug_accessible_get_n_children (AtkObject *object)
{
  g_return_val_if_fail (TA_IS_PLUG_ACCESSIBLE (object), 0);
  TaPlugAccessible *plug = NULL;
  TaPlugAccessiblePrivate *priv = NULL;
  GList *children = NULL;
  GList *item = NULL;
  gint n_children = 0;
  plug = TA_PLUG_ACCESSIBLE (object);
  priv = TA_PLUG_ACCESSIBLE_GET_PRIVATE(plug);
  if (!priv->widget)
    return 0;
  if (!GTK_IS_CONTAINER (priv->widget))
    return 0;
  children = gtk_container_get_children (GTK_CONTAINER (priv->widget));
  n_children = g_list_length (children);
  g_list_free (children);
  return n_children;
}
static AtkObject *ta_plug_accessible_ref_child (AtkObject *object, gint i)
{
  g_return_val_if_fail (TA_IS_PLUG_ACCESSIBLE (object), NULL);
  TaPlugAccessible *plug = NULL;
  TaPlugAccessiblePrivate *priv = NULL;
  GList *children = NULL;
  GList *item = NULL;
  gint n_children = 0;
  GtkWidget *child = NULL;
  AtkObject *axChild = NULL;
  plug = TA_PLUG_ACCESSIBLE (object);
  priv = TA_PLUG_ACCESSIBLE_GET_PRIVATE(plug);
  if (!priv->widget)
    return NULL;
  if (!GTK_IS_CONTAINER (priv->widget))
    return NULL;
  children = gtk_container_get_children (GTK_CONTAINER (priv->widget));
  if (i < 0 || i >= g_list_length (children))
    return NULL;
  child = GTK_WIDGET (g_list_nth_data (children, i));
  if (!child)
    return NULL;
  g_list_free (children);
  axChild = gtk_widget_get_accessible (child);
  return g_object_ref (axChild);
}
static void ta_plug_accessible_dispose (GObject *object)
{
  TaPlugAccessible *plug = NULL;
  TaPlugAccessiblePrivate *priv = NULL;
  plug = TA_PLUG_ACCESSIBLE (object);
  priv = TA_PLUG_ACCESSIBLE_GET_PRIVATE(plug);
  if (priv->widget)
    {
      g_object_unref (priv->widget);
      priv->widget = NULL;
    }
  G_OBJECT_CLASS(ta_plug_accessible_parent_class)->dispose(object);
}
static void ta_plug_accessible_class_init (TaPlugAccessibleClass *klass)
{
  GObjectClass *g_object_class = G_OBJECT_CLASS(klass);
  AtkObjectClass *atk_object_class = NULL;
  g_object_class->dispose = ta_plug_accessible_dispose;
  atk_object_class = ATK_OBJECT_CLASS (klass);
  atk_object_class->initialize = ta_plug_accessible_initialize;
  atk_object_class->get_name = ta_plug_accessible_get_name;
  atk_object_class->get_role = ta_plug_accessible_get_role;
  atk_object_class->get_n_children = ta_plug_accessible_get_n_children;
  atk_object_class->get_index_in_parent = NULL;
  atk_object_class->ref_child = ta_plug_accessible_ref_child;
  g_type_class_add_private(g_object_class, sizeof(TaPlugAccessiblePrivate));
}
static void ta_plug_accessible_init(TaPlugAccessible *plug)
{
}
AtkObject *ta_plug_accessible_new ()
{
  return ATK_OBJECT (g_object_new(TA_TYPE_PLUG_ACCESSIBLE, 0));
}
```
## ta-plug-accessible.h
```cpp
/*
 * Copyright (C) 2011 Igalia S.L.
 * Authors: Mario Sanchez Prada <msanchez@igalia.com>
 *
 * Based on a C# example written in C# by Mike Gorse:
 * http://mgorse.freeshell.org/plug-socket-test.tar.gz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */
#ifndef TaPlugAccessible_h
#define TaPlugAccessible_h
#include <atk/atk.h>
G_BEGIN_DECLS
#define TA_TYPE_PLUG_ACCESSIBLE                      (ta_plug_accessible_get_type ())
#define TA_PLUG_ACCESSIBLE(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), TA_TYPE_PLUG_ACCESSIBLE, TaPlugAccessible))
#define TA_PLUG_ACCESSIBLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST ((klass), TA_TYPE_PLUG_ACCESSIBLE, TaPlugAccessibleClass))
#define TA_IS_PLUG_ACCESSIBLE(obj)                   (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TA_TYPE_PLUG_ACCESSIBLE))
#define TA_IS_PLUG_ACCESSIBLE_CLASS(klass)           (G_TYPE_CHECK_CLASS_TYPE ((klass), TA_TYPE_PLUG_ACCESSIBLE))
#define TA_PLUG_ACCESSIBLE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TA_TYPE_PLUG_ACCESSIBLE, TaPlugAccessibleClass))
typedef struct _TaPlugAccessible                TaPlugAccessible;
typedef struct _TaPlugAccessibleClass           TaPlugAccessibleClass;
typedef struct _TaPlugAccessiblePrivate         TaPlugAccessiblePrivate;
struct _TaPlugAccessible {
    AtkPlug parent;
};
struct _TaPlugAccessibleClass {
    AtkPlugClass parentClass;
};
GType ta_plug_accessible_get_type(void) G_GNUC_CONST;
AtkObject *ta_plug_accessible_new (void);
G_END_DECLS
#endif // TaPlugAccessible_h
```
## ta-plug-vbox.c
```cpp
/*
 * Copyright (C) 2011 Igalia S.L.
 * Authors: Mario Sanchez Prada <msanchez@igalia.com>
 *
 * Based on a C# example written in C# by Mike Gorse:
 * http://mgorse.freeshell.org/plug-socket-test.tar.gz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */
#include "ta-plug-vbox.h"
#include "ta-plug-accessible.h"
#include <glib.h>
#include <gtk/gtk.h>
G_DEFINE_TYPE (TaPlugVBox, ta_plug_vbox, GTK_TYPE_VBOX);
static AtkObject *_get_accessible (GtkWidget *widget)
{
  static AtkObject *new = NULL;
  if (!new)
    {
      new = ta_plug_accessible_new ();
      atk_object_initialize (new, G_OBJECT (widget));
    }
  return new;
}
static void ta_plug_vbox_class_init (TaPlugVBoxClass *klass)
{
  GtkWidgetClass *widget_class = (GtkWidgetClass *)klass;
  widget_class->get_accessible = _get_accessible;
}
static void ta_plug_vbox_init(TaPlugVBox *plug)
{
}
GtkWidget *ta_plug_vbox_new (gboolean homogeneous, gint spacing)
{
  return GTK_WIDGET (g_object_new(TA_TYPE_PLUG_VBOX,
                                  "homogeneous", homogeneous,
                                  "spacing", spacing,
                                  0));
}
gchar *ta_plug_vbox_get_id (TaPlugVBox *plug)
{
  AtkObject *plugAxObject = NULL;
  gchar *text_id = NULL;
  plugAxObject = gtk_widget_get_accessible (GTK_WIDGET (plug));
  if (ATK_IS_PLUG (plugAxObject))
    text_id = atk_plug_get_id (ATK_PLUG (plugAxObject));
  else
    g_print ("Not an instance of AtkPlug\n");
  return text_id;
}
```
## ta-plug-vbox.h
```cpp
/*
 * Copyright (C) 2011 Igalia S.L.
 * Authors: Mario Sanchez Prada <msanchez@igalia.com>
 *
 * Based on a C# example written in C# by Mike Gorse:
 * http://mgorse.freeshell.org/plug-socket-test.tar.gz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */
#ifndef TaPlugVBox_h
#define TaPlugVBox_h
#include <gtk/gtk.h>
G_BEGIN_DECLS
#define TA_TYPE_PLUG_VBOX            (ta_plug_vbox_get_type ())
#define TA_PLUG_VBOX(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TA_TYPE_PLUG_VBOX, TaPlugVBox))
#define TA_PLUG_VBOX_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TA_TYPE_PLUG_VBOX, TaPlugVBoxClass))
#define TA_IS_PLUG_VBOX(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TA_TYPE_PLUG_VBOX))
#define TA_IS_PLUG_VBOX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TA_TYPE_PLUG_VBOX))
#define TA_PLUG_VBOX_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TA_TYPE_PLUG_VBOX, TaPlugVBoxClass))
typedef struct _TaPlugVBox                TaPlugVBox;
typedef struct _TaPlugVBoxClass           TaPlugVBoxClass;
typedef struct _TaPlugVBoxPrivate         TaPlugVBoxPrivate;
struct _TaPlugVBox {
    GtkVBox parent;
};
struct _TaPlugVBoxClass {
    GtkVBoxClass parentClass;
};
GType ta_plug_vbox_get_type(void) G_GNUC_CONST;
GtkWidget *ta_plug_vbox_new (gboolean homogeneous, gint spacing);
gchar *ta_plug_vbox_get_id (TaPlugVBox *plug);
G_END_DECLS
#endif // TaPlugVBox_h
```
## Makefile
```
CC=gcc
TARGET=ta-plug
SOURCES=ta-plug.c \
 	ta-plug-vbox.h \
 	ta-plug-vbox.c \
 	ta-plug-accessible.h \
 	ta-plug-accessible.c
LIBS=atk gtk+-3.0
CFLAGS=`pkg-config --cflags $(LIBS)`
LDFLAGS=`pkg-config --libs $(LIBS)`
all: compile
compile:
	$(CC) -o $(TARGET) $(SOURCES) $(CFLAGS) $(LDFLAGS)
clean:
	rm -rf $(TARGET)
	rm -rf *~
```
# AtkSocket例子
## ta-socket.c
```cpp
/*
 * Copyright (C) 2011 Igalia S.L.
 * Authors: Mario Sanchez Prada <msanchez@igalia.com>
 *
 * Based on a C# example written in C# by Mike Gorse:
 * http://mgorse.freeshell.org/plug-socket-test.tar.gz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */
#include "ta-socket-label.h"
#include <gtk/gtk.h>
#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT -1
static GtkWidget *entry = NULL;
static GtkWidget *button = NULL;
static GtkWidget *socket = NULL;
static void
_button_clicked_cb (GtkButton *button, gpointer data)
{
  AtkObject *socketAxObject = NULL;
  gchar *text_id = NULL;
  text_id = g_strdup (gtk_entry_get_text (GTK_ENTRY (entry)));
  if (text_id && text_id[0] != '\0')
    {
      socketAxObject = gtk_widget_get_accessible (socket);
      if (ATK_IS_SOCKET (socketAxObject))
        {
          g_print ("[SOCKET] Embedding object with ID %s\n", text_id);
          atk_socket_embed (ATK_SOCKET (socketAxObject), text_id);
          g_print ("[SOCKET] Done\n");
        }
      else
        g_print ("Not embedding anything: Not an instance of AtkSocket\n");
    }
  else
    g_print ("Not embedding: you need to provide an ID\n");
  g_free (text_id);
}
static void
_create_widgets (GtkWidget *window)
{
  GtkWidget *vbox = NULL;
  vbox = gtk_vbox_new (FALSE, 0);
  /* Label + entry */
  entry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 6);
  button = gtk_button_new_with_label ("Connect to plug");
  g_signal_connect (button, "clicked", G_CALLBACK (_button_clicked_cb), NULL);
  gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 6);
  /* Our socket accessible */
  socket = ta_socket_label_new ("Socket accessible goes here");
  gtk_box_pack_start (GTK_BOX (vbox), socket, FALSE, FALSE, 6);
  gtk_container_add (GTK_CONTAINER (window), vbox);
}
int
main (int argc, char**argv)
{
  GtkWidget *window = NULL;
  gtk_init (&argc, &argv);
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  _create_widgets (window);
  gtk_window_set_default_size (GTK_WINDOW (window),
                               WINDOW_WIDTH, WINDOW_HEIGHT);
  gtk_widget_show_all (window);
  gtk_main ();
}
```
## ta-socket-accessible.c
```cpp
/*
 * Copyright (C) 2011 Igalia S.L.
 * Authors: Mario Sanchez Prada <msanchez@igalia.com>
 *
 * Based on a C# example written in C# by Mike Gorse:
 * http://mgorse.freeshell.org/plug-socket-test.tar.gz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */
#include "ta-socket-accessible.h"
#include <atk/atk.h>
#include <glib.h>
#include <gtk/gtk.h>
G_DEFINE_TYPE (TaSocketAccessible, ta_socket_accessible, ATK_TYPE_SOCKET);
static const gchar *ta_socket_accessible_get_name (AtkObject *object)
{
  g_return_val_if_fail(ATK_IS_OBJECT (object), NULL);
  return "The Socket";
}
static AtkRole ta_socket_accessible_get_role (AtkObject *object)
{
  g_return_val_if_fail(ATK_IS_OBJECT (object), ATK_ROLE_UNKNOWN);
  return ATK_ROLE_PANEL;
}
static void ta_socket_accessible_class_init (TaSocketAccessibleClass *klass)
{
  AtkObjectClass *atk_object_class = NULL;
  atk_object_class = ATK_OBJECT_CLASS (klass);
  atk_object_class->get_name = ta_socket_accessible_get_name;
  atk_object_class->get_role = ta_socket_accessible_get_role;
}
static void ta_socket_accessible_init(TaSocketAccessible *socket)
{
}
AtkObject *ta_socket_accessible_new (void)
{
  return ATK_OBJECT (g_object_new(TA_TYPE_SOCKET_ACCESSIBLE, 0));
}
```
## ta-socket-accessible.h
```cpp
/*
 * Copyright (C) 2011 Igalia S.L.
 * Authors: Mario Sanchez Prada <msanchez@igalia.com>
 *
 * Based on a C# example written in C# by Mike Gorse:
 * http://mgorse.freeshell.org/plug-socket-test.tar.gz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */
#ifndef TaSocketAccessible_h
#define TaSocketAccessible_h
#include <atk/atk.h>
G_BEGIN_DECLS
#define TA_TYPE_SOCKET_ACCESSIBLE                      (ta_socket_accessible_get_type ())
#define TA_SOCKET_ACCESSIBLE(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), TA_TYPE_SOCKET_ACCESSIBLE, TaSocketAccessible))
#define TA_SOCKET_ACCESSIBLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST ((klass), TA_TYPE_SOCKET_ACCESSIBLE, TaSocketAccessibleClass))
#define TA_IS_SOCKET_ACCESSIBLE(obj)                   (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TA_TYPE_SOCKET_ACCESSIBLE))
#define TA_IS_SOCKET_ACCESSIBLE_CLASS(klass)           (G_TYPE_CHECK_CLASS_TYPE ((klass), TA_TYPE_SOCKET_ACCESSIBLE))
#define TA_SOCKET_ACCESSIBLE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TA_TYPE_SOCKET_ACCESSIBLE, TaSocketAccessibleClass))
typedef struct _TaSocketAccessible                TaSocketAccessible;
typedef struct _TaSocketAccessibleClass           TaSocketAccessibleClass;
typedef struct _TaSocketAccessiblePrivate         TaSocketAccessiblePrivate;
struct _TaSocketAccessible {
    AtkSocket parent;
};
struct _TaSocketAccessibleClass {
    AtkSocketClass parentClass;
};
GType ta_socket_accessible_get_type(void) G_GNUC_CONST;
AtkObject *ta_socket_accessible_new (void);
G_END_DECLS
#endif // TaSocketAccessible_h
```
## ta-socket-label.c
```cpp
/*
 * Copyright (C) 2011 Igalia S.L.
 * Authors: Mario Sanchez Prada <msanchez@igalia.com>
 *
 * Based on a C# example written in C# by Mike Gorse:
 * http://mgorse.freeshell.org/plug-socket-test.tar.gz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */
#include "ta-socket-label.h"
#include "ta-socket-accessible.h"
#include <glib.h>
#include <gtk/gtk.h>
G_DEFINE_TYPE (TaSocketLabel, ta_socket_label, GTK_TYPE_LABEL);
static AtkObject *_get_accessible (GtkWidget *widget)
{
  static AtkObject *new = NULL;
  if (!new)
    {
      new = ta_socket_accessible_new ();
      atk_object_initialize (new, G_OBJECT (widget));
    }
  return new;
}
static void ta_socket_label_class_init (TaSocketLabelClass *klass)
{
  GtkWidgetClass *widget_class = (GtkWidgetClass *)klass;
  widget_class->get_accessible = _get_accessible;
}
static void ta_socket_label_init(TaSocketLabel *socket)
{
}
GtkWidget *ta_socket_label_new (const gchar *text)
{
  return GTK_WIDGET (g_object_new(TA_TYPE_SOCKET_LABEL,
                                  "label", text,
                                  0));
}
```
## ta-socket-label.h
```cpp
/*
 * Copyright (C) 2011 Igalia S.L.
 * Authors: Mario Sanchez Prada <msanchez@igalia.com>
 *
 * Based on a C# example written in C# by Mike Gorse:
 * http://mgorse.freeshell.org/plug-socket-test.tar.gz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */
#ifndef TaSocketLabel_h
#define TaSocketLabel_h
#include <gtk/gtk.h>
G_BEGIN_DECLS
#define TA_TYPE_SOCKET_LABEL            (ta_socket_label_get_type ())
#define TA_SOCKET_LABEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TA_TYPE_SOCKET_LABEL, TaSocketLabel))
#define TA_SOCKET_LABEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TA_TYPE_SOCKET_LABEL, TaSocketLabelClass))
#define TA_IS_SOCKET_LABEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TA_TYPE_SOCKET_LABEL))
#define TA_IS_SOCKET_LABEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TA_TYPE_SOCKET_LABEL))
#define TA_SOCKET_LABEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TA_TYPE_SOCKET_LABEL, TaSocketLabelClass))
typedef struct _TaSocketLabel                TaSocketLabel;
typedef struct _TaSocketLabelClass           TaSocketLabelClass;
typedef struct _TaSocketLabelPrivate         TaSocketLabelPrivate;
struct _TaSocketLabel {
    GtkLabel parent;
};
struct _TaSocketLabelClass {
    GtkLabelClass parentClass;
};
GType ta_socket_label_get_type(void) G_GNUC_CONST;
GtkWidget *ta_socket_label_new (const gchar *text);
G_END_DECLS
#endif // TaSocketLabel_h
```
## Makefile
```
RGET=ta-socket
SOURCES=ta-socket.c \
	ta-socket-label.h \
	ta-socket-label.c \
	ta-socket-accessible.h \
	ta-socket-accessible.c
LIBS=atk gtk+-3.0
CFLAGS=`pkg-config --cflags $(LIBS)`
LDFLAGS=`pkg-config --libs $(LIBS)`
all: compile
compile:
	$(CC) -o $(TARGET) $(SOURCES) $(CFLAGS) $(LDFLAGS)
clean:
	rm -rf $(TARGET)
	rm -rf *~
```
