

#ifndef __GDI_MATRIX_HPP__
#define __GDI_MATRIX_HPP__

#include "matrix_c.h"

struct Matrix
{
	REAL nativeMatrix[6];
    
    // Default constructor is set to identity matrix.

    Matrix()
    {
		GdipCreateMatrix(nativeMatrix);
       }

    Matrix(IN REAL m11, 
           IN REAL m12,
           IN REAL m21, 
           IN REAL m22,
           IN REAL dx, 
           IN REAL dy)
    {
		CError lastResult = GdipCreateMatrix2(m11, m12, m21, m22, dx, dy, nativeMatrix);
    }
    
    Matrix(IN const RectF& rect, 
           IN const PointF* dstplg)
    {
        CError lastResult = GdipCreateMatrix3(&rect, dstplg,nativeMatrix);
    }

    Matrix(IN const Rect& rect, 
           IN const Point* dstplg)
    {
        CError lastResult = GdipCreateMatrix3I(&rect, dstplg,nativeMatrix);
    }

    CError GetElements(OUT REAL *m) const 
    {
        return SetStatus(GdipCopyMatrix(nativeMatrix, m));
    }
    
    CError SetElements(IN REAL m11, 
                       IN REAL m12, 
                       IN REAL m21, 
                       IN REAL m22, 
                       IN REAL dx, 
                       IN REAL dy)
    {
        return SetStatus(GdipSetMatrixElements(nativeMatrix,
                            m11, m12, m21, m22, dx, dy));
    }

    REAL OffsetX() const
    {
        REAL elements[6];

        if (GetElements(&elements[0]) == Ok)
            return elements[4];
        else 
            return 0.0f;
    }

    REAL OffsetY() const
    {
       REAL elements[6];

       if (GetElements(&elements[0]) == Ok)
           return elements[5];
       else 
           return 0.0f;
    }

    CError Reset()
    {
        // set identity matrix elements 
        return SetStatus(GdipSetMatrixElements(nativeMatrix,
                                             1.0, 0.0, 0.0, 1.0, 0.0, 0.0));
    }

    CError Multiply(IN const Matrix& matrix,
                    IN MatrixOrder order = MatrixOrderPrepend)
    {
        return SetStatus(GdipMultiplyMatrix(nativeMatrix, 
                                          matrix.nativeMatrix,
                                          order));
    }

    CError Translate(IN REAL offsetX, 
                     IN REAL offsetY, 
                     IN MatrixOrder order = MatrixOrderPrepend)
    {
        return SetStatus(GdipTranslateMatrix(nativeMatrix, offsetX,
                                                         offsetY, order));
    }

    CError Scale(IN REAL scaleX, 
                 IN REAL scaleY, 
                 IN MatrixOrder order = MatrixOrderPrepend)
    {
        return SetStatus(GdipScaleMatrix(nativeMatrix, scaleX, 
                                                     scaleY, order));
    }

    CError Rotate(IN REAL angle, 
                  IN MatrixOrder order = MatrixOrderPrepend)
    {
        return SetStatus(GdipRotateMatrix(nativeMatrix, angle, 
                                                      order));
    }
    
    CError RotateAt(IN REAL angle, 
                    IN const PointF& center, 
                    IN MatrixOrder order = MatrixOrderPrepend)
    {
        if(order == MatrixOrderPrepend)
        {
            SetStatus(GdipTranslateMatrix(nativeMatrix, center.X,
                                                      center.Y, order));
            SetStatus(GdipRotateMatrix(nativeMatrix, angle, 
                                                   order));
            return SetStatus(GdipTranslateMatrix(nativeMatrix,
                                                             -center.X, 
                                                             -center.Y, 
                                                             order));
        }
        else
        {
            SetStatus(GdipTranslateMatrix(nativeMatrix, 
                                                      - center.X, 
                                                      - center.Y, 
                                                      order));
            SetStatus(GdipRotateMatrix(nativeMatrix, angle, 
                                                   order));
            return SetStatus(GdipTranslateMatrix(nativeMatrix, 
                                                             center.X, 
                                                             center.Y, 
                                                             order));
        }
    }

    CError Shear(IN REAL shearX, 
                 IN REAL shearY,
                 IN MatrixOrder order = MatrixOrderPrepend)
    {
        return SetStatus(GdipShearMatrix(nativeMatrix, shearX, 
                                                     shearY, order));
    }

    CError Invert()
    {
        return SetStatus(GdipInvertMatrix(nativeMatrix));
    }

    // float version
    CError TransformPoints(IN OUT PointF* pts, 
                           IN INT count = 1) const
    {
        return SetStatus(GdipTransformMatrixPoints(nativeMatrix, 
                                                               pts, count));
    }
    
    CError TransformPoints(IN OUT Point* pts, 
                           IN INT count = 1) const
    {
        return SetStatus(GdipTransformMatrixPointsI(nativeMatrix, 
                                                                pts, 
                                                                count));
    }

    CError TransformVectors(IN OUT PointF* pts, 
                            IN INT count = 1) const
    { 
        return SetStatus(GdipVectorTransformMatrixPoints(
                                        nativeMatrix, pts, count));
    }

    CError TransformVectors(IN OUT Point* pts, 
                            IN INT count = 1) const
    { 
       return SetStatus(GdipVectorTransformMatrixPointsI(
                                        nativeMatrix, 
                                        pts, 
                                        count));
    }
    
    BOOL IsInvertible() const
    {
        BOOL result = FALSE;

        SetStatus(GdipIsMatrixInvertible(nativeMatrix, &result));
    
        return result;
    }

    BOOL IsIdentity() const
    {
       BOOL result = FALSE;

       SetStatus(GdipIsMatrixIdentity(nativeMatrix, &result));
    
       return result;
    }

    BOOL Equals(IN const Matrix& matrix) const
    {
       BOOL result = FALSE;

       SetStatus(GdipIsMatrixEqual(nativeMatrix,
                                               matrix.nativeMatrix, 
                                               &result));
   
       return result;
    }
    
    CError SetStatus(CError status) const
    {
		return status;
    }

};


#endif // __GDI_MATRIX_HPP__
