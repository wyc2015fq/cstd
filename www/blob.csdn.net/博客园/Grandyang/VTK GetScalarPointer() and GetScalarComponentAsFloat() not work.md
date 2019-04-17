# VTK GetScalarPointer() and GetScalarComponentAsFloat() not work - Grandyang - 博客园







# [VTK GetScalarPointer() and GetScalarComponentAsFloat() not work](https://www.cnblogs.com/grandyang/p/5331386.html)







I am using VTK 5.10.1 with VS 2010, and the following example does not work on my machine:

[http://www.vtk.org/Wiki/VTK/Examples/Cxx/ImageData/IterateImageData](http://www.vtk.org/Wiki/VTK/Examples/Cxx/ImageData/IterateImageData)

The erros said:

vtkImageData [009B92A8]: Bad component index 1302176011

I am not sure why this happened, and it should work since it is from the official document. However, the reality is that it does not work on my machine, neither GetScalarComponentAsFloat() or GetScalarComponentAsDouble() will work.

So if we want to get the pixel data from vtkImageData, we need to find another way to do it. And we can get the data by accessing vtkDataArray, please see the following example code:

```
#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include "vtkBMPReader.h"

int main(int, char *[])
{
    char * filename = "img.bmp";
    if( !filename  || strlen(filename) == 0 )
    {
        return -1;
    }

    vtkBMPReader * reader = vtkBMPReader::New();
    reader->SetFileName(filename);
    reader->Update();

    vtkSmartPointer<vtkImageData> image_data = reader->GetOutput();

    int* dims = image_data->GetDimensions();
    std::cout << "Dims: " << " x: " << dims[0] << " y: " << dims[1] << " z: " << dims[2] << std::endl;
    std::cout << "Number of points: " << image_data->GetNumberOfPoints() << std::endl;
    std::cout << "Number of cells: " << image_data->GetNumberOfCells() << std::endl;
    std::cout << "Number of scalar components: " << image_data->GetNumberOfScalarComponents() << std::endl;

    vtkDataArray *arr = image_data->GetPointData()->GetArray(0);

    // Retrieve the entries from the image data and print them to the screen
    for (int z = 0; z < dims[2]; z++)
    {
        for (int y = 0; y < dims[1]; y++)
        {
            for (int x = 0; x < dims[0]; x++)
            {
                /* Change this
                double* pixel = static_cast<double*>(imageData->GetScalarPointer(x,y,z));
                // do something with v
                std::cout << pixel[0] << " ";
                */
                double d[3];
                arr->GetTuple(y * dims[0] + x, d);
                std::cout << d[0] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
```














