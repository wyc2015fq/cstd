# GDAL之OGR入门 - L_Andy的专栏 - CSDN博客

2015年12月30日 14:38:12[卡哥](https://me.csdn.net/L_Andy)阅读数：6301


这篇文档是为了说明怎样从一个文件里用OGR的C++类读取和写入数据。强烈建议在读此文档之前首先阅读介绍OGR体系结构介绍文档，里面介绍了OGR里主要的类以及他们所完成的功能。

从OGR 读取数据

    为了描述如何通过OGR读取数据，我们编写一个小的示例，从OGR文件里读出数据，并按照一定格式输出。

    首先我们需要注册我们渴望读取的所有格式。这个通过调用OGRRegisterAll()就能很容易完成，这个函数注册了GDAL/OGR支持的所有格式。

＃i nclude "ogrsf_frmts.h"

int main()

{

        OGRRegisterAll();

    下一步我们将打开输入的OGR数据文件。数据文件可以是文件，关系型数据库，文件路径，甚至可能是远程的网络服务，这点取决于我们使用的驱动。但是，数据源的名字通常只是一个简单的字符串。既然这样拿我们就编写一个打开shapefile的程序。第二个参数（FLALSE）告诉OGRSFDriverRegistrar::Open() 函数我们不需要update access。如果失败返回NULL，并报错。

    OGRDataSource       *poDS;

    poDS = OGRSFDriverRegistrar::Open( "point.shp", FALSE );

    if( poDS == NULL )

    {

        printf( "Open failed.\n" );

        exit( 1 );

    }

    一个OGRDataSource可能包含很多的层。所包含层的数量我们可以用过调OGRDataSource::GetLayerCount()得到，并且其中每一个曾我们利用索引调用OGRDataSource::GetLayer()得到。不过，我们现在利用层的名字。

   OGRLayer *poLayer;

    poLayer = poDS->GetLayerByName( "point" );

    现在我们开始读取层里面的features。在开始之前我们需要指定一个attribute或者spatial filter来严格控制我们得到的feature。不过现在我们只是得到所有的features。

    自从我们开始fresh with这个层，就没有这么严格了。很明智地我们需要调用Layer::ResetReading()来确保我们是从层的开头开始。我们不断地调用OGRLayer::GetNextFeature()函数来遍历所有的features，当遍历完所有的features后返回NULL。

OGRFeature *poFeature;

    poLayer->ResetReading();

    while( (poFeature = poLayer->GetNextFeature()) != NULL )

    {

    为了得到一个feature的所有属性fields，调用OGRFeatureDefn将很方便。这是一个object，与层相关联，包含所有fields的定义。我们循环完所有的fields，得到属性数据并将之显示出来。

   OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();

        int iField;

        for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )

        {

            OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );

            if( poFieldDefn->GetType() == OFTInteger )

                printf( "%d,", poFeature->GetFieldAsInteger( iField ) );

            else if( poFieldDefn->GetType() == OFTReal )

                printf( "%.3f,", poFeature->GetFieldAsDouble(iField) );

            else if( poFieldDefn->GetType() == OFTString )

                printf( "%s,", poFeature->GetFieldAsString(iField) );

            else

                printf( "%s,", poFeature->GetFieldAsString(iField) );

        }

    实际中field的种类比上面列出来的多，但是我们可以通过调用OGRFeature::GetFieldAsString()的方法将之统一提取出来。实际上如果我们用OGRFeature::GetFieldAsString()将会使程序更简短。

    下一步我们想从feature里面提取出几何（geometry）数据，并且n他的x和y坐标标出。几何数据通过统一的 OGRGeometry指针返回。然后我们确定这个几何数据的类型，如果是点，我们将他标为点并且进行操作，如果是其他的内省我们write占位符。

    OGRGeometry *poGeometry;

        poGeometry = poFeature->GetGeometryRef();

        if( poGeometry != NULL 

            && wkbFlatten(poGeometry->getGeometryType()) == wkbPoint )

        {

            OGRPoint *poPoint = (OGRPoint *) poGeometry;

            printf( "%.3f,%3.f\n", poPoint->getX(), poPoint->getY() );

        }

        else

        {

            printf( "no point geometry\n" );

        }

    上面我们使用的这个wkbFlatten()宏是将一个wkbPoint25D（具有Z坐标的点）转化为基于2D的类型（wkbPoint）。对于每一个2D几何类型都有一个2.5D的type code。但是，我们只有基于2D和3D的C++类。因此，我们的代码可以完全处理2D或3D的例子。

    注意 OGRFeature::GetGeometryRef()返回一个指向属于OGRFeature的内部几何数据的指针。我们并没有实际地删除返回的几何数据。但是， OGRLayer::GetNextFeature()函数返回了一个现在属于我们自身的feature的拷贝。因此，在用完之后，我们需要释放这个feature。我们可以仅仅“delete”它，but this can cause problems in windows builds where the GDAL DLL has a different
 "heap" from the main program.为了安全起见我们利用一个GDAL函数去删除它。

        OGRFeature::DestroyFeature( poFeature );

    }

    OGRDataSource::GetLayerByName()函数返回的OGRLayer是OGRDataSource中的一个层，因此我们没有必要删除它，但是我们需要删除这个数据文件从而关闭输入的文件。再一次我们利用这个custom delete来避免win32 heap 问题。

   OGRDataSource::DestroyDataSource( poDS );

}

    以上所有的放在一起，我们的程序如下：

＃i nclude "ogrsf_frmts.h"

int main()

{

    OGRRegisterAll();

    OGRDataSource       *poDS;

    poDS = OGRSFDriverRegistrar::Open( "point.shp", FALSE );

    if( poDS == NULL )

    {

        printf( "Open failed.\n%s" );

        exit( 1 );

    }

    OGRLayer *poLayer;

    poLayer = poDS->GetLayerByName( "point" );

    OGRFeature *poFeature;

    poLayer->ResetReading();

    while( (poFeature = poLayer->GetNextFeature()) != NULL )

    {

        OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();

        int iField;

        for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )

        {

            OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );

            if( poFieldDefn->GetType() == OFTInteger )

                printf( "%d,", poFeature->GetFieldAsInteger( iField ) );

            else if( poFieldDefn->GetType() == OFTReal )

                printf( "%.3f,", poFeature->GetFieldAsDouble(iField) );

            else if( poFieldDefn->GetType() == OFTString )

                printf( "%s,", poFeature->GetFieldAsString(iField) );

            else

                printf( "%s,", poFeature->GetFieldAsString(iField) );

        }

        OGRGeometry *poGeometry;

        poGeometry = poFeature->GetGeometryRef();

        if( poGeometry != NULL 

            && wkbFlatten(poGeometry->getGeometryType()) == wkbPoint )

        {

            OGRPoint *poPoint = (OGRPoint *) poGeometry;

            printf( "%.3f,%3.f\n", poPoint->getX(), poPoint->getY() );

        }

        else

        {

            printf( "no point geometry\n" );

        }       

        OGRFeature::DestroyFeature( poFeature );

    }

    OGRDataSource::DestroyDataSource( poDS );

}

Writing TO　OGR

    作为一个用OGR写的例子，我们粗略地跟上面的程序向反。这个小程序利用OGR将从文件输入的用逗号分隔的值写到一个shapefile的点文件。

    通常，我们在一开始就注册所有的驱动，然后取得Shapefile的驱动创建我们的输出文件。

＃i nclude "ogrsf_frmts.h"

int main()

{

    const char *pszDriverName = "ESRI Shapefile";

    OGRSFDriver *poDriver;

    OGRRegisterAll();

    poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(

                pszDriverName );

    if( poDriver == NULL )

    {

        printf( "%s driver not available.\n", pszDriverName );

        exit( 1 );

    }

    下一步我们创建数据文件(DataSource)。ESRI Shapefile驱动容许我们创建一个全是shapefiles的路径或者单独一个shapefile文件。在这里我们创建一个单独的文件by including the extension in the name。其他类型驱动处理不一样。第二个参数是一组参数值，但在这个例子里我们用默认的值。参数的详细值也随格式的不同而不同。

    OGRDataSource *poDS;

    poDS = poDriver->CreateDataSource( "point_out.shp", NULL );

    if( poDS == NULL )

    {

        printf( "Creation of output file failed.\n" );

        exit( 1 );

    }

    现在我们就可以创建输出图层了。由于这个例子里只是一个单独的文件，因此我们也只需要创建一个层。我们通过wkPoint来指定这个层支持的几何类型。在这个例子里我们没有传递任何坐标系统信息或者别的特殊的层的创建信息。

OGRLayer *poLayer;

    poLayer = poDS->CreateLayer( "point_out", NULL, wkbPoint, NULL );

    if( poLayer == NULL )

    {

        printf( "Layer creation failed.\n" );

        exit( 1 );

    }

    现在层已经创建，我们需要创建任何可能出现在层里的属性fields。Fields必须在如何features在写入之前加入到图层里面。创建field我们用包含field信息的OGRField。在Shapefiles文件里，field的width和精度对于输出的.dbf文件非常重要。，因此我们特定设定它，尽管默认的也OK。在这个例子里，我们只有一个attribute，名字字串associated with x和y点。

    确保我们传给CreateFidld()的OGRField模版is copied internally。我们保留这个哦oject的所有权。

    OGRFieldDefn oField( "Name", OFTString );

    oField.SetWidth(32);

    if( poLayer->CreateField( &oField ) != OGRERR_NONE )

    {

        printf( "Creating Name field failed.\n" );

        exit( 1 );

    }

\编码（encode）

下面这个循环结构从标准输入读取"x,y,name"值，并分析他们。The following snipping loops reading lines of the form "x,y,name" from stdin, and parsing them.

\代码code

    double x, y;

    char szName[33];

    while( !feof(stdin) 

           && fscanf( stdin, "%lf,%lf,%32s", &x, &y, szName ) == 3 )

    {

    为了把一个feature写道磁盘上，我们必须创建一个本地的OGRFeature，在试图将之写入图层前设置属性并加载几何信息。必须注意的是这个feature必须和将要写入的图层的OGRFeatureDefn给出的事例一致。

       OGRFeature *poFeature;

        poFeature = new OGRFeature( poLayer->GetLayerDefn() );

        poFeature->SetField( "Name", szName );

我们创建了一个本地的几何文件，并且让他直接指向feature。OGRFeature::SetGeometryDirectly()和OGRFeature::SetGeometry()的不同之处是前者给了feature对几何数据的所有权。This is generally more efficient as it avoids an extra deep object copy of the geometry.

        OGRPoint *poPoint = new OGRPoint();

        poPoint->setX( x );

        poPoint->setY( y );

        poFeature->SetGeometryDirectly( poPoint );

现在我们就在这个文件里创建了一个feature，OGRLayer::CreateFeature()没有取消对feature的拥有因此我们需要在创建完之后清除干净。

        if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )

        {

           printf( "Failed to create feature in shapefile.\n" );

           exit( 1 );

        }

        delete poFeature;

   }

Finally we need to close down the datasource in order to ensure headers are written out in an orderly way and all resources are recovered.

    OGRDataSource::DestroyDataSource( poDS );

}

The same program all in one block looks like this:

＃i nclude "ogrsf_frmts.h"

int main()

{

    const char *pszDriverName = "ESRI Shapefile";

    OGRSFDriver *poDriver;

    OGRRegisterAll();

    poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(

                pszDriverName );

    if( poDriver == NULL )

    {

        printf( "%s driver not available.\n", pszDriverName );

        exit( 1 );

    }

    OGRDataSource *poDS;

    poDS = poDriver->CreateDataSource( "point_out.shp", NULL );

    if( poDS == NULL )

    {

        printf( "Creation of output file failed.\n" );

        exit( 1 );

    }

    OGRLayer *poLayer;

    poLayer = poDS->CreateLayer( "point_out", NULL, wkbPoint, NULL );

    if( poLayer == NULL )

    {

        printf( "Layer creation failed.\n" );

        exit( 1 );

    }

    OGRFieldDefn oField( "Name", OFTString );

    oField.SetWidth(32);

    if( poLayer->CreateField( &oField ) != OGRERR_NONE )

    {

        printf( "Creating Name field failed.\n" );

        exit( 1 );

    }

    double x, y;

    char szName[33];

    while( !feof(stdin) 

           && fscanf( stdin, "%lf,%lf,%32s", &x, &y, szName ) == 3 )

    {

        OGRFeature *poFeature;

        poFeature = new OGRFeature( poLayer->GetLayerDefn() );

        poFeature->SetField( "Name", szName );

        OGRPoint *poPoint = new OGRPoint();

        poPoint->setX( x );

        poPoint->setY( y );

        poFeature->SetGeometryDirectly( poPoint );

        if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )

        {

           printf( "Failed to create feature in shapefile.\n" );

           exit( 1 );

        }

        delete poFeature;

    }

    OGRDataSource::DestroyDataSource( poDS );

}

