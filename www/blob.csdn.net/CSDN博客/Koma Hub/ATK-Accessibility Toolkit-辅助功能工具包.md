# ATK - Accessibility Toolkit - 辅助功能工具包 - Koma Hub - CSDN博客
2019年03月02日 17:36:25[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：129
个人分类：[ATK																[GTK																[GNOME](https://blog.csdn.net/Rong_Toa/article/category/8611720)](https://blog.csdn.net/Rong_Toa/article/category/7156203)](https://blog.csdn.net/Rong_Toa/article/category/8715793)
辅助功能工具包
ATK provides the set of accessibility interfaces that are implemented by other toolkits and applications. Using the ATK interfaces, accessibility tools have full access to view and control running applications.
Available Versions:
- [1.32.0](https://developer.gnome.org/atk/1.32/)
- [2.0.1](https://developer.gnome.org/atk/2.0/)
- [2.2.0](https://developer.gnome.org/atk/2.2/)
- [2.4.0](https://developer.gnome.org/atk/2.4/)
- [2.6.0](https://developer.gnome.org/atk/2.6/)
- [2.8.0](https://developer.gnome.org/atk/2.8/)
- [2.10.0](https://developer.gnome.org/atk/2.10/)
- [2.12.0](https://developer.gnome.org/atk/2.12/)
- [2.13.90](https://developer.gnome.org/atk/2.13/)
- [2.14.0](https://developer.gnome.org/atk/2.14/)
- [2.16.0](https://developer.gnome.org/atk/2.16/)
- [2.18.0](https://developer.gnome.org/atk/2.18/)
- [2.20.0](https://developer.gnome.org/atk/2.20/)
- [2.22.0](https://developer.gnome.org/atk/2.22/)
- [2.24.0](https://developer.gnome.org/atk/2.24/)
- [2.26.1](https://developer.gnome.org/atk/2.26/)
- **[2.28.1](https://developer.gnome.org/atk/2.28/)**
[Overview](overview.html)
[Base accessibility object](base-object.html)
[AtkObject](AtkObject.html) — The base object class for the Accessibility Toolkit API.
[Event and toolkit support](toolkit.html)
[AtkUtil](AtkUtil.html) — A set of ATK utility functions for event and toolkit support.
[ATK Interfaces](interfaces.html)
[AtkAction](AtkAction.html) — The ATK interface provided by UI components which the user can activate/interact with.
[AtkComponent](AtkComponent.html) — The ATK interface provided by UI components which occupy a physical area on the screen. which the user can activate/interact with.
[AtkDocument](AtkDocument.html) — The ATK interface which represents the toplevel container for document content.
[AtkEditableText](AtkEditableText.html) — The ATK interface implemented by components containing user-editable text content.
[AtkHyperlinImpl](atk-AtkHyperlinkImpl.html) — An interface from which the AtkHyperlink associated with an AtkObject may be obtained.
[AtkHypertext](AtkHypertext.html) — The ATK interface which provides standard mechanism for manipulating hyperlinks.
[AtkImage](AtkImage.html) — The ATK Interface implemented by components which expose image or pixmap content on-screen.
[AtkSelection](AtkSelection.html) — The ATK interface implemented by container objects whose AtkObject children can be selected.
[AtkStreamableContent](AtkStreamableContent.html) — The ATK interface which provides access to streamable content.
[AtkTable](AtkTable.html) — The ATK interface implemented for UI components which contain tabular or row/column information.
[AtkTableCell](AtkTableCell.html) — The ATK interface implemented for a cell inside a two-dimentional AtkTable
[AtkText](AtkText.html) — The ATK interface implemented by components with text content.
[AtkValue](AtkValue.html) — The ATK interface implemented by valuators and components which display or select a value from a bounded range of values.
[AtkWindow](AtkWindow.html) — The ATK Interface provided by UI components that represent a top-level window.
[Basic accessible data types](data.html)
[AtkRange](atk-AtkRange.html) — A given range or subrange, to be used with AtkValue
[AtkRelation](AtkRelation.html) — An object used to describe a relation between a object and one or more other objects.
[AtkRelationSet](AtkRelationSet.html) — A set of AtkRelations, normally the set of AtkRelations which an AtkObject has.
[AtkState](atk-AtkState.html) — An AtkState describes a single state of an object.
[AtkStateSet](AtkStateSet.html) — An AtkStateSet contains the states of an object.
[Custom accessible objects](accessibles.html)
[AtkGObjectAccessible](AtkGObjectAccessible.html) — This object class is derived from AtkObject and can be used as a basis implementing accessible objects.
[AtkHyperlink](AtkHyperlink.html) — An ATK object which encapsulates a link or set of links in a hypertext document.
[AtkNoOpObject](AtkNoOpObject.html) — An AtkObject which purports to implement all ATK interfaces.
[AtkPlug](atk-AtkPlug.html) — Toplevel for embedding into other processes
[AtkSocket](atk-AtkSocket.html) — Container for AtkPlug objects from other processes
[Utilities](utilities.html)
[AtkNoOpObjectFactory](AtkNoOpObjectFactory.html) — The AtkObjectFactory which creates an AtkNoOpObject.
[AtkObjectFactory](AtkObjectFactory.html) — The base object class for a factory used to create accessible objects for objects of a specific GType.
[AtkRegistry](AtkRegistry.html) — An object used to store the GType of the factories used to create an accessible object for an object of a particular GType.
[Versioning macros](atk-Versioning-Utilities.html) — Variables and functions to check the ATK version
[Deprecated Interfaces](deprecated.html)
[AtkMisc](AtkMisc.html) — A set of ATK utility functions for thread locking
