# QuickTime文件格式规范 - 建建的博客 - CSDN博客
2017年08月07日 17:20:16[纪建](https://me.csdn.net/u013898698)阅读数：859

# QuickTime文件格式概述
QuickTime影片是存储在磁盘上，使用两个基本结构存储信息：*atoms*（也被称为*simple atoms*或*classic atoms*）和*QT atoms*。为了了解QuickTime影片的存储，你需要了解本章描述的基本的atoms结构。大多数你在QuickTime文件格式中看到的atoms都是simple 或classic atoms。即便如此，simple atoms和QT
 atoms，都允许构建任意复杂的分层数据结构，也都允许应用程序忽略不能识别的数据。
## 媒体描述
QuickTime文件把媒体的描述和媒体数据分开存储。
媒体描述被称为影片资源，影片atom，或简单的影片，包含信息包括：轨道的数量、视频压缩格式和定时信息等。影片资源还包含一个索引，该索引描述了存储所有的媒体数据的位置。
媒体数据是真实的采样点数据，如视频帧和音频样本，在影片中使用的。媒体数据可以存储在同一文件中作为QuickTime影片、或一个单独的文件中、或多个文件中、或替代资源，如数据库或实时流，或这些东西的某种组合。
## Atoms
QuickTime文件的基本数据单元是atom。每个atom都包含在其他数据之前的size和type字段。size字段表示atom中的字节总数，包括size和type字段的长度。type字段通过type的含义指定存储在atom中的数据的格式类型。在某些情况下，size和type字段后跟一个version字段和一个flags字段。有这些version和flags字段的atom有时被称为*full atom*。
> 
