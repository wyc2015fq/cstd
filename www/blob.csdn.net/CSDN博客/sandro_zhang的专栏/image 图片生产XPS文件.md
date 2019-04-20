# image 图片生产XPS文件 - sandro_zhang的专栏 - CSDN博客
2011年10月27日 08:59:55[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：1999
由图片转为XPS文件，需要调用windows 版本为6.0后的API创建XPS，然后用图片刷将图片刷上去即可。
并且如需加入print ticket可以用同样的方法加载资源，并载入，以下为我昨天参考MSDN上面的例子写好的源码，共享之。
```cpp
#include <iostream>
#include <windows.h>
#include <comdef.h>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <comdef.h>
#include <XpsObjectModel.h>
#include <StrSafe.h>
#include <shlobj.h>
using namespace std;
HRESULT 
CreateRectanglePath(
    __in  IXpsOMObjectFactory   *xpsFactory,
    __in  const XPS_RECT        *rect,
    __out IXpsOMPath            **rectPath
)
{
   
    HRESULT hr = S_OK;
    IXpsOMGeometryFigure           *rectFigure;
    IXpsOMGeometry                 *imageRectGeometry;
    IXpsOMGeometryFigureCollection *geomFigureCollection;
    // Define start point and three of the four sides of the rectangle.
    //  The fourth side is implied by setting the path type to CLOSED.
    XPS_POINT            startPoint = {rect->x, rect->y};
    XPS_SEGMENT_TYPE     segmentTypes[3] = {
        XPS_SEGMENT_TYPE_LINE, 
        XPS_SEGMENT_TYPE_LINE, 
        XPS_SEGMENT_TYPE_LINE
    };
    FLOAT segmentData[6] = {
        rect->x,              rect->y+rect->height, 
        rect->x+rect->width,  rect->y+rect->height, 
        rect->x+rect->width,  rect->y 
    };
    BOOL segmentStrokes[3] = {
        TRUE, TRUE, TRUE
    };
    // Create a closed geometry figure using the three 
    //  segments defined above.
    hr = xpsFactory->CreateGeometryFigure( &startPoint, &rectFigure );
    hr = rectFigure->SetIsClosed( TRUE );
    hr = rectFigure->SetIsFilled( TRUE );
    hr = rectFigure->SetSegments( 3, 6, 
            segmentTypes, segmentData, segmentStrokes );
    // Create a geometry that consists of the figure created above.
    hr = xpsFactory->CreateGeometry( &imageRectGeometry );
    hr = imageRectGeometry->GetFigures( &geomFigureCollection );
    hr = geomFigureCollection->Append( rectFigure );
    // Create the path that consists of the geometry created above
    //  and return the pointer in the parameter passed in to the function.
    hr = xpsFactory->CreatePath( reinterpret_cast<IXpsOMPath**>(rectPath) );
    hr = (*rectPath)->SetGeometryLocal( imageRectGeometry );
    // The calling method will release these interfaces when 
    //  it is done with them.
    return hr;
}
int main( void ){
    //XPS size (A4)
    float width = 793.76f;
    float height = 1122.56f;
    //color/mono
    //portrait
    //image source
    LPCWSTR imageFileName = L"1.jpg";
    float image_width = 768.0f;
    float image_height = 1024.0f;
    /* full fill page
        XPS_RECT    rect = {0.0f, 0.0f, width, height}; // set to image size
        XPS_RECT    viewPort = {0.0,0.0, width,height};
        XPS_RECT    viewBox = {0.0,0.0, image_width, image_height};
    */
    /* adapt page
    */
    XPS_RECT    rect        = {0.0f, 0.0f, width, height};//page size
    XPS_RECT    viewPort    = {0.0f, 0.0f, 0.0f, 0.0f};
    XPS_RECT    viewBox     = {0.0f, 0.0f, image_width, image_height}; //view all image
    if( (image_width > image_height) && (image_width > width ) ){
        //adapt width
        float resize_height = image_height * ( width/image_width);
        float padding_top = (height - resize_height)/2 ;
        viewPort.y      = padding_top;
        viewPort.width  = width;
        viewPort.height = resize_height;    
    } else if( (image_height > image_width) && ( image_height >  height) ){
        float resize_width = image_width * (height/image_height);
        float padding_left = (width - resize_width) / 2;
        viewPort.x      = padding_left;
        viewPort.width  = resize_width;
        viewPort.height = height;
    } else {
        //position center
        float padding_left = (width - image_width)/2;
        float padding_top  = (height - image_height)/2;
        viewPort.x      = padding_left;
        viewPort.y      = padding_top;
        viewPort.width  = image_width;
        viewPort.height = image_height;
    }
    LPCWSTR XPSFileName = L"a.xps";
    IXpsOMObjectFactory *xpsFactory;
    HRESULT hr = S_OK;
    // Init COM for this thread if it hasn't 
    //  been initialized, yet.
    hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    hr = CoCreateInstance(
        __uuidof(XpsOMObjectFactory),
        NULL, 
        CLSCTX_INPROC_SERVER,
        __uuidof(IXpsOMObjectFactory),
        reinterpret_cast<LPVOID*>(&xpsFactory));
    if (SUCCEEDED(hr))
    {
        IOpcPartUri                   *opcPartUri = NULL;
        IXpsOMPackage                 *xpsPackage = NULL;
        IXpsOMDocumentSequence        *xpsFDS = NULL;
        IXpsOMDocumentCollection      *fixedDocuments = NULL;
        IXpsOMDocument                *xpsFD = NULL;
        IXpsOMPage                    *xpsPage = NULL;
        IXpsOMPageReferenceCollection *pageRefs = NULL;
        IXpsOMPageReference           *xpsPageRef = NULL;
        XPS_SIZE pageSize = {width, height}; 
    
        // Create the package.
        hr = xpsFactory->CreatePackage( &xpsPackage );
        // Create the URI for the fixed document sequence part and then  
        //  create the fixed document sequence
        hr = xpsFactory->CreatePartUri( 
            L"/FixedDocumentSequence.fdseq", &opcPartUri );
        hr = xpsFactory->CreateDocumentSequence( opcPartUri, &xpsFDS );
        // Release this URI to reuse the interface pointer.
        if (NULL != opcPartUri) {opcPartUri->Release(); opcPartUri = NULL;}
        // Create the URI for the document part and then create the document.
        hr = xpsFactory->CreatePartUri( 
            L"/Documents/1/FixedDocument.fdoc", &opcPartUri );
        hr = xpsFactory->CreateDocument( opcPartUri, &xpsFD );
        // Release this URI to reuse the interface pointer.
        if (NULL != opcPartUri) {opcPartUri->Release(); opcPartUri = NULL;}
        // Create a blank page.
        hr = xpsFactory->CreatePartUri( 
            L"/Documents/1/Pages/1.fpage", &opcPartUri );
        hr = xpsFactory->CreatePage(
            &pageSize,                  // Page size
            L"en-US",                   // Page language
            opcPartUri,                 // Page part name
            &xpsPage);                
        // Release this URI to reuse the interface pointer.
        if (NULL != opcPartUri) {opcPartUri->Release(); opcPartUri = NULL;}
    //add image
        IStreamPtr             imageStream; // the resulting image stream
        IOpcPartUri            *imagePartUri;
        IXpsOMImageResource    *imageResource;
        hr = xpsFactory->CreateReadOnlyStreamOnFile ( 
                imageFileName, &imageStream );
        hr = xpsFactory->CreatePartUri( imageFileName, &imagePartUri );
        /*    imageType; // set to type of image being read in
    XPS_IMAGE_TYPE_JPEG
    A JPEG (Joint Photographic Experts Group) image.
    XPS_IMAGE_TYPE_PNG
    A PNG (Portable Network Graphics) image.
    XPS_IMAGE_TYPE_TIFF
    A TIFF (Tagged Image File Format) image.
    XPS_IMAGE_TYPE_WDP
    An HD Photo (formerly Windows Media Photo) image.
        */
        hr = xpsFactory->CreateImageResource ( 
            imageStream,
            XPS_IMAGE_TYPE_JPEG,
            imagePartUri,
            &imageResource);
        imagePartUri->Release();
        imageStream->Release();
        // imageResource can now be used by other parts in the XPS OM.
        // These variables are initialized outside of this code example,
        //  for example, as the parameters of a method or from some 
        //  preceding program code.
        // dimensions of the image in pixels
        XPS_SIZE    bmpDim = {0,0} ; 
        // DPI resolution values obtained from image
        FLOAT        dpiX = 96.0f;
        FLOAT        dpiY = 96.0f;
        // These are part of this code example.
        IXpsOMPath *imageRectPath;
        IXpsOMImageBrush *imageBrush;
        IXpsOMVisualCollection *pageVisuals;
        // Describe image source dimensions and set viewbox to be the 
        // entire image DIP width of image. 
        //  Example: 
        //    600 image pixels, 300 dpi -> 2 inches -> 2 * 96 = 192 DIP width
        //viewBox.width = FLOAT((double)bmpDim.width * 96.0 / dpiX); 
        //viewBox.height = FLOAT((double)bmpDim.height * 96.0 / dpiY);
        // Create the image brush.
        hr = xpsFactory->CreateImageBrush(imageResource, &viewBox, &viewPort, 
            reinterpret_cast<IXpsOMImageBrush**>(&imageBrush));
        // Create the path that describes the outline of the image on the page.
        //  This step uses the function described in the next code example.
        hr = CreateRectanglePath(xpsFactory, &rect,
            reinterpret_cast<IXpsOMPath**>(&imageRectPath));
        // Set the accessibility description for the path object as required.
        LPCWSTR shortDescText = L"short description text.";
        hr = imageRectPath->SetAccessibilityShortDescription( shortDescText );
        // Set the image brush to be the fill brush for this path.
        hr = imageRectPath->SetFillBrushLocal( imageBrush );
        // Get the list of visuals for this page...
        hr = xpsPage->GetVisuals( &pageVisuals );
        // ...and add the completed path to the list.
        hr = pageVisuals->Append( imageRectPath );
        // Release locally created interfaces.
        if (NULL != pageVisuals) pageVisuals->Release();
        if (NULL != imageRectPath) imageRectPath->Release();
        if (NULL != imageBrush) imageBrush->Release();
        //create pt
        /*
        color:  psk:Monochrome/psk:Color
        */
        ofstream ofpt("Job_PT.xml");
        ofpt << "<?xml version=\"1.0\" encoding=\"UTF-8\"?> \
            <psf:PrintTicket xmlns:psf=\"http://schemas.microsoft.com/windows/2003/08/printing/printschemaframework\" xmlns:psk=\"http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" version=\"1\">\
              <psf:Feature name=\"psk:PageOutputColor\">\
            <psf:Option name=\"psk:Monochrome\">\
            <psf:ScoredProperty name=\"psk:DeviceBitsPerPixel\">\
            <psf:Value xsi:type=\"xsd:integer\">24</psf:Value>\
            </psf:ScoredProperty>\
            <psf:ScoredProperty name=\"psk:DriverBitsPerPixel\">\
            <psf:Value xsi:type=\"xsd:integer\">24</psf:Value>\
            </psf:ScoredProperty>\
            </psf:Option>\
            </psf:Feature>\
            </psf:PrintTicket>";
        ofpt.close();
	    IStream						*ptStream = NULL;
        LPCWSTR printTicketFileName = L"Job_PT.xml";
	    hr = xpsFactory->CreateReadOnlyStreamOnFile (
		    printTicketFileName,
		    &ptStream);
        LPCWSTR printTicketPartName = L"Metadata/Job_PT.xml";
	    hr = xpsFactory->CreatePartUri( 
			    printTicketPartName, &opcPartUri );
        IXpsOMPrintTicketResource* printTicketResource;
        xpsFactory->CreatePrintTicketResource( ptStream, opcPartUri, &printTicketResource);
        opcPartUri->Release();
        if (NULL != ptStream) ptStream->Release();
        // Create a page reference for the page.
        hr = xpsFactory->CreatePageReference( &pageSize, &xpsPageRef );
        // Add the fixed document sequence to the package.
        hr = xpsPackage->SetDocumentSequence( xpsFDS );
        xpsFDS->SetPrintTicketResource( printTicketResource);
        // Get the document collection of the fixed document sequence
        //  and then add the document to the collection.
        hr = xpsFDS->GetDocuments( &fixedDocuments );
        hr = fixedDocuments->Append( xpsFD );
        // Get the page reference collection from the document
        //  and add the page reference and blank page.
        hr = xpsFD->GetPageReferences( &pageRefs );
        hr = pageRefs->Append( xpsPageRef );
        hr = xpsPageRef->SetPage( xpsPage );
        xpsPackage->WriteToFile( XPSFileName, NULL, FILE_ATTRIBUTE_NORMAL, FALSE);
        // Release interface pointer
        if (NULL != xpsPage) xpsPage->Release();
        if (NULL != pageRefs) pageRefs->Release();
        if (NULL != fixedDocuments) fixedDocuments->Release();
        if (NULL != xpsPageRef) xpsPageRef->Release();
        if (NULL != xpsFD) xpsFD->Release();
        if (NULL != xpsFDS) xpsFDS->Release();
        if (NULL != xpsPackage) xpsPackage->Release();
            // ... and release when done
            xpsFactory->Release();
    }
    // Uninitialize COM when finished
    CoUninitialize();
    return 0;
}
```
感谢：[http://msdn.microsoft.com/en-us/library/windows/desktop/dd372970(v=VS.85).aspx](http://msdn.microsoft.com/en-us/library/windows/desktop/dd372970(v=VS.85).aspx)
