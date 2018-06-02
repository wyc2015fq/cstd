package org.opencv.test.imgproc;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.CvMat;
import org.opencv.core.MatOfFloat;
import org.opencv.core.MatOfInt;
import org.opencv.core.MatOfInt4;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.RotatedRect;
import org.opencv.core.Scalar;
import org.opencv.core.CvSize;
import org.opencv.core.TermCriteria;
import org.opencv.imgproc.Imgproc;
import org.opencv.test.OpenCVTestCase;

public class ImgprocTest extends OpenCVTestCase {

    Point anchorPoint;
    private int imgprocSz;
    CvSize size;

    @Override
    protected void setUp() throws Exception {
        super.setUp();

        imgprocSz = 2;
        anchorPoint = new Point(2, 2);
        size = new CvSize(3, 3);
    }

    public void testAccumulateMatMat() {
        CvMat src = getMat(CvType.CC_64F, 2);
        CvMat dst = getMat(CvType.CC_64F, 0);
        CvMat dst2 = src.clone();

        Imgproc.accumulate(src, dst);
        Imgproc.accumulate(src, dst2);

        assertMatEqual(src, dst, EPS);
        assertMatEqual(getMat(CvType.CC_64F, 4), dst2, EPS);
    }

    public void testAccumulateMatMatMat() {
        CvMat src = getMat(CvType.CC_64F, 2);
        CvMat mask = makeMask(getMat(CvType.CC_8U, 1));
        CvMat dst = getMat(CvType.CC_64F, 0);
        CvMat dst2 = src.clone();

        Imgproc.accumulate(src, dst, mask);
        Imgproc.accumulate(src, dst2, mask);

        assertMatEqual(makeMask(getMat(CvType.CC_64F, 2)), dst, EPS);
        assertMatEqual(makeMask(getMat(CvType.CC_64F, 4), 2), dst2, EPS);
    }

    public void testAccumulateProductMatMatMat() {
        CvMat src = getMat(CvType.CC_64F, 2);
        CvMat dst = getMat(CvType.CC_64F, 0);
        CvMat dst2 = src.clone();

        Imgproc.accumulateProduct(src, src, dst);
        Imgproc.accumulateProduct(src, dst, dst2);

        assertMatEqual(getMat(CvType.CC_64F, 4), dst, EPS);
        assertMatEqual(getMat(CvType.CC_64F, 10), dst2, EPS);
    }

    public void testAccumulateProductMatMatMatMat() {
        CvMat src = getMat(CvType.CC_64F, 2);
        CvMat mask = makeMask(getMat(CvType.CC_8U, 1));
        CvMat dst = getMat(CvType.CC_64F, 0);
        CvMat dst2 = src.clone();

        Imgproc.accumulateProduct(src, src, dst, mask);
        Imgproc.accumulateProduct(src, dst, dst2, mask);

        assertMatEqual(makeMask(getMat(CvType.CC_64F, 4)), dst, EPS);
        assertMatEqual(makeMask(getMat(CvType.CC_64F, 10), 2), dst2, EPS);
    }

    public void testAccumulateSquareMatMat() {
        CvMat src = getMat(CvType.CC_64F, 2);
        CvMat dst = getMat(CvType.CC_64F, 0);
        CvMat dst2 = src.clone();

        Imgproc.accumulateSquare(src, dst);
        Imgproc.accumulateSquare(src, dst2);

        assertMatEqual(getMat(CvType.CC_64F, 4), dst, EPS);
        assertMatEqual(getMat(CvType.CC_64F, 6), dst2, EPS);
    }

    public void testAccumulateSquareMatMatMat() {
        CvMat src = getMat(CvType.CC_64F, 2);
        CvMat mask = makeMask(getMat(CvType.CC_8U, 1));
        CvMat dst = getMat(CvType.CC_64F, 0);
        CvMat dst2 = src.clone();

        Imgproc.accumulateSquare(src, dst, mask);
        Imgproc.accumulateSquare(src, dst2, mask);

        assertMatEqual(makeMask(getMat(CvType.CC_64F, 4)), dst, EPS);
        assertMatEqual(makeMask(getMat(CvType.CC_64F, 6), 2), dst2, EPS);
    }

    public void testAccumulateWeightedMatMatDouble() {
        CvMat src = getMat(CvType.CC_64F, 2);
        CvMat dst = getMat(CvType.CC_64F, 4);
        CvMat dst2 = src.clone();

        Imgproc.accumulateWeighted(src, dst, 0.5);
        Imgproc.accumulateWeighted(src, dst2, 2);

        assertMatEqual(getMat(CvType.CC_64F, 3), dst, EPS);
        assertMatEqual(getMat(CvType.CC_64F, 2), dst2, EPS);
    }

    public void testAccumulateWeightedMatMatDoubleMat() {
        CvMat src = getMat(CvType.CC_64F, 2);
        CvMat mask = makeMask(getMat(CvType.CC_8U, 1));
        CvMat dst = getMat(CvType.CC_64F, 4);
        CvMat dst2 = src.clone();

        Imgproc.accumulateWeighted(src, dst, 0.5, mask);
        Imgproc.accumulateWeighted(src, dst2, 2, mask);

        assertMatEqual(makeMask(getMat(CvType.CC_64F, 3), 4), dst, EPS);
        assertMatEqual(getMat(CvType.CC_64F, 2), dst2, EPS);
    }

    public void testAdaptiveThreshold() {
        CvMat src = makeMask(getMat(CvType.CC_8U, 50), 20);
        CvMat dst = new CvMat();

        Imgproc.adaptiveThreshold(src, dst, 1, Imgproc.ADAPTIVE_THRESH_MEAN_C, Imgproc.THRESH_BINARY, 3, 0);

        assertEquals(src.rows(), Core.countNonZero(dst));
    }

    public void testApproxPolyDP() {
        MatOfPoint2f curve = new MatOfPoint2f(new Point(1, 3), new Point(2, 4), new Point(3, 5), new Point(4, 4), new Point(5, 3));

        MatOfPoint2f approxCurve = new MatOfPoint2f();

        Imgproc.approxPolyDP(curve, approxCurve, EPS, true);

        List<Point> approxCurveGold =  new ArrayList<Point>(3);
        approxCurveGold.add(new Point(1, 3));
        approxCurveGold.add(new Point(3, 5));
        approxCurveGold.add(new Point(5, 3));

        assertListPointEquals(approxCurve.toList(), approxCurveGold, EPS);
    }

    public void testArcLength() {
        MatOfPoint2f curve = new MatOfPoint2f(new Point(1, 3), new Point(2, 4), new Point(3, 5), new Point(4, 4), new Point(5, 3));

        double arcLength = Imgproc.arcLength(curve, false);

        assertEquals(5.656854249, arcLength, 0.000001);
    }

    public void testBilateralFilterMatMatIntDoubleDouble() {
        Imgproc.bilateralFilter(gray255, dst, 5, 10, 5);

        assertMatEqual(gray255, dst);
        // TODO_: write better test
    }

    public void testBilateralFilterMatMatIntDoubleDoubleInt() {
        Imgproc.bilateralFilter(gray255, dst, 5, 10, 5, Core.BORDER_REFLECT);

        assertMatEqual(gray255, dst);
        // TODO_: write better test
    }

    public void testBlurMatMatSize() {
        Imgproc.blur(gray0, dst, size);
        assertMatEqual(gray0, dst);

        Imgproc.blur(gray255, dst, size);
        assertMatEqual(gray255, dst);
        // TODO_: write better test
    }

    public void testBlurMatMatSizePoint() {
        Imgproc.blur(gray0, dst, size, anchorPoint);
        assertMatEqual(gray0, dst);
        // TODO_: write better test
    }

    public void testBlurMatMatSizePointInt() {
        Imgproc.blur(gray0, dst, size, anchorPoint, Core.BORDER_REFLECT);
        assertMatEqual(gray0, dst);
        // TODO_: write better test
    }

    public void testBoundingRect() {
        MatOfPoint points = new MatOfPoint(new Point(0, 0), new Point(0, 4), new Point(4, 0), new Point(4, 4));
        Point p1 = new Point(1, 1);
        Point p2 = new Point(-5, -2);

        Rect bbox = Imgproc.boundingRect(points);

        assertTrue(bbox.contains(p1));
        assertFalse(bbox.contains(p2));
    }

    public void testBoxFilterMatMatIntSize() {
        CvSize size = new CvSize(3, 3);
        Imgproc.boxFilter(gray0, dst, 8, size);
        assertMatEqual(gray0, dst);
        // TODO_: write better test
    }

    public void testBoxFilterMatMatIntSizePointBoolean() {
        Imgproc.boxFilter(gray255, dst, 8, size, anchorPoint, false);
        assertMatEqual(gray255, dst);
        // TODO_: write better test
    }

    public void testBoxFilterMatMatIntSizePointBooleanInt() {
        Imgproc.boxFilter(gray255, dst, 8, size, anchorPoint, false, Core.BORDER_REFLECT);
        assertMatEqual(gray255, dst);
        // TODO_: write better test
    }

    public void testCalcBackProject() {
        List<CvMat> images = Arrays.asList(grayChess);
        MatOfInt channels = new MatOfInt(0);
        MatOfInt histSize = new MatOfInt(10);
        MatOfFloat ranges = new MatOfFloat(0f, 256f);

        CvMat hist = new CvMat();
        Imgproc.calcHist(images, channels, new CvMat(), hist, histSize, ranges);
        Core.normalize(hist, hist);

        Imgproc.calcBackProject(images, channels, hist, dst, ranges, 255);

        assertEquals(grayChess.size(), dst.size());
        assertEquals(grayChess.depth(), dst.depth());
        assertFalse(0 == Core.countNonZero(dst));
    }

    public void testCalcHistListOfMatListOfIntegerMatMatListOfIntegerListOfFloat() {
        List<CvMat> images = Arrays.asList(gray128);
        MatOfInt channels = new MatOfInt(0);
        MatOfInt histSize = new MatOfInt(10);
        MatOfFloat ranges = new MatOfFloat(0f, 256f);
        CvMat hist = new CvMat();

        Imgproc.calcHist(images, channels, new CvMat(), hist, histSize, ranges);

        truth = new CvMat(10, 1, CvType.CC_32F, Scalar.all(0)) {
            {
                put(5, 0, 100);
            }
        };
        assertMatEqual(truth, hist, EPS);
    }

    public void testCalcHistListOfMatListOfIntegerMatMatListOfIntegerListOfFloat2D() {
        List<CvMat> images = Arrays.asList(gray255, gray128);
        MatOfInt channels = new MatOfInt(0, 1);
        MatOfInt histSize = new MatOfInt(10, 10);
        MatOfFloat ranges = new MatOfFloat(0f, 256f, 0f, 256f);
        CvMat hist = new CvMat();

        Imgproc.calcHist(images, channels, new CvMat(), hist, histSize, ranges);

        truth = new CvMat(10, 10, CvType.CC_32F, Scalar.all(0)) {
            {
                put(9, 5, 100);
            }
        };
        assertMatEqual(truth, hist, EPS);
    }

    public void testCalcHistListOfMatListOfIntegerMatMatListOfIntegerListOfFloat3D() {
        List<CvMat> images = Arrays.asList(rgbLena);

        CvMat hist3D = new CvMat();
        List<CvMat> histList = Arrays.asList( new CvMat[] {new CvMat(), new CvMat(), new CvMat()} );

        MatOfInt histSize = new MatOfInt(10);
        MatOfFloat ranges = new MatOfFloat(0f, 256f);

        for(int i=0; i<rgbLena.channels(); i++)
        {
            Imgproc.calcHist(images, new MatOfInt(i), new CvMat(), histList.get(i), histSize, ranges);

            assertEquals(10, histList.get(i).checkVector(1));
        }

        Core.merge(histList, hist3D);

        assertEquals(CvType.CC_32FC3, hist3D->tid);
        assertEquals(10, hist3D.checkVector(3));

        CvMat truth = new CvMat(10, 1, CvType.CC_32FC3);
        truth.put(0, 0,
                 0, 24870, 0,
                 1863, 31926, 1,
                 56682, 37677, 2260,
                 77278, 44751, 32436,
                 69397, 41343, 18526,
                 27180, 40407, 18658,
                 21101, 15993, 32042,
                 8343, 18585, 47786,
                 300, 6567, 80988,
                 0, 25, 29447
                );

        assertMatEqual(truth, hist3D, EPS);
    }

    public void testCalcHistListOfMatListOfIntegerMatMatListOfIntegerListOfFloatBoolean() {
        List<CvMat> images = Arrays.asList(gray255, gray128);
        MatOfInt channels = new MatOfInt(0, 1);
        MatOfInt histSize = new MatOfInt(10, 10);
        MatOfFloat ranges = new MatOfFloat(0f, 256f, 0f, 256f);
        CvMat hist = new CvMat();

        Imgproc.calcHist(images, channels, new CvMat(), hist, histSize, ranges, true);

        truth = new CvMat(10, 10, CvType.CC_32F, Scalar.all(0)) {
            {
                put(9, 5, 100);
            }
        };
        assertMatEqual(truth, hist, EPS);
    }

    public void testCannyMatMatDoubleDouble() {
        Imgproc.Canny(gray255, dst, 5, 10);
        assertMatEqual(gray0, dst);
        // TODO_: write better test
    }

    public void testCannyMatMatDoubleDoubleIntBoolean() {
        Imgproc.Canny(gray0, dst, 5, 10, 5, true);
        assertMatEqual(gray0, dst);
        // TODO_: write better test
    }

    public void testCompareHist() {
        CvMat H1 = new CvMat(3, 1, CvType.CC_32F);
        CvMat H2 = new CvMat(3, 1, CvType.CC_32F);
        H1.put(0, 0, 1, 2, 3);
        H2.put(0, 0, 4, 5, 6);

        double distance = Imgproc.compareHist(H1, H2, Imgproc.CC_COMP_CORREL);

        assertEquals(1., distance, EPS);
    }

    public void testContourAreaMat() {
        CvMat contour = new CvMat(1, 4, CvType.CC_32FC2);
        contour.put(0, 0, 0, 0, 10, 0, 10, 10, 5, 4);

        double area = Imgproc.contourArea(contour);

        assertEquals(45., area, EPS);
    }

    public void testContourAreaMatBoolean() {
        CvMat contour = new CvMat(1, 4, CvType.CC_32FC2);
        contour.put(0, 0, 0, 0, 10, 0, 10, 10, 5, 4);

        double area = Imgproc.contourArea(contour, true);

        assertEquals(45., area, EPS);
        // TODO_: write better test
    }

    public void testConvertMapsMatMatMatMatInt() {
        CvMat map1 = new CvMat(1, 4, CvType.CC_32FC1, new Scalar(1));
        CvMat map2 = new CvMat(1, 4, CvType.CC_32FC1, new Scalar(2));
        CvMat dstmap1 = new CvMat(1, 4, CvType.CC_16SC2);
        CvMat dstmap2 = new CvMat(1, 4, CvType.CC_16UC1);

        Imgproc.convertMaps(map1, map2, dstmap1, dstmap2, CvType.CC_16SC2);

        CvMat truthMap1 = new CvMat(1, 4, CvType.CC_16SC2);
        truthMap1.put(0, 0, 1, 2, 1, 2, 1, 2, 1, 2);
        assertMatEqual(truthMap1, dstmap1);
        CvMat truthMap2 = new CvMat(1, 4, CvType.CC_16UC1, new Scalar(0));
        assertMatEqual(truthMap2, dstmap2);
    }

    public void testConvertMapsMatMatMatMatIntBoolean() {
        CvMat map1 = new CvMat(1, 3, CvType.CC_32FC1, new Scalar(2));
        CvMat map2 = new CvMat(1, 3, CvType.CC_32FC1, new Scalar(4));
        CvMat dstmap1 = new CvMat(1, 3, CvType.CC_16SC2);
        CvMat dstmap2 = new CvMat(1, 3, CvType.CC_16UC1);

        Imgproc.convertMaps(map1, map2, dstmap1, dstmap2, CvType.CC_16SC2, false);
        // TODO_: write better test (last param == true)

        CvMat truthMap1 = new CvMat(1, 3, CvType.CC_16SC2);
        truthMap1.put(0, 0, 2, 4, 2, 4, 2, 4);
        assertMatEqual(truthMap1, dstmap1);
        CvMat truthMap2 = new CvMat(1, 3, CvType.CC_16UC1, new Scalar(0));
        assertMatEqual(truthMap2, dstmap2);
    }

    public void testConvexHullMatMat() {
        MatOfPoint points = new MatOfPoint(
                new Point(20, 0),
                new Point(40, 0),
                new Point(30, 20),
                new Point(0,  20),
                new Point(20, 10),
                new Point(30, 10)
        );

        MatOfInt hull = new MatOfInt();

        Imgproc.convexHull(points, hull);

        MatOfInt expHull = new MatOfInt(
                1, 2, 3, 0
        );
        assertMatEqual(expHull, hull, EPS);
    }

    public void testConvexHullMatMatBooleanBoolean() {
        MatOfPoint points = new MatOfPoint(
                new Point(2, 0),
                new Point(4, 0),
                new Point(3, 2),
                new Point(0, 2),
                new Point(2, 1),
                new Point(3, 1)
        );

        MatOfInt hull = new MatOfInt();

        Imgproc.convexHull(points, hull, true);

        MatOfInt expHull = new MatOfInt(
                3, 2, 1, 0
        );
        assertMatEqual(expHull, hull, EPS);
    }

    public void testConvexityDefects() {
        MatOfPoint points = new MatOfPoint(
                new Point(20, 0),
                new Point(40, 0),
                new Point(30, 20),
                new Point(0,  20),
                new Point(20, 10),
                new Point(30, 10)
        );

        MatOfInt hull = new MatOfInt();
        Imgproc.convexHull(points, hull);

        MatOfInt4 convexityDefects = new MatOfInt4();
        Imgproc.convexityDefects(points, hull, convexityDefects);

        assertMatEqual(new MatOfInt4(3, 0, 5, 3620), convexityDefects);
    }

    public void testCornerEigenValsAndVecsMatMatIntInt() {
        fail("Not yet implemented");
        // TODO: write better test
        CvMat src = new CvMat(imgprocSz, imgprocSz, CvType.CC_32FC1);
        src.put(0, 0, 1, 2);
        src.put(1, 0, 4, 2);

        int blockSize = 3;
        int ksize = 5;

        // TODO: eigen vals and vectors returned = 0 for most src matrices
        Imgproc.cornerEigenValsAndVecs(src, dst, blockSize, ksize);
        truth = new CvMat(imgprocSz, imgprocSz, CvType.CC_32FC(6), new Scalar(0));
        assertMatEqual(truth, dst, EPS);
    }

    public void testCornerEigenValsAndVecsMatMatIntIntInt() {
        fail("Not yet implemented");
        // TODO: write better test
        CvMat src = new CvMat(4, 4, CvType.CC_32FC1, new Scalar(128));

        int blockSize = 3;
        int ksize = 5;

        truth = new CvMat(4, 4, CvType.CC_32FC(6), new Scalar(0));

        Imgproc.cornerEigenValsAndVecs(src, dst, blockSize, ksize, Core.BORDER_REFLECT);
        assertMatEqual(truth, dst, EPS);
    }

    public void testCornerHarrisMatMatIntIntDouble() {
        fail("Not yet implemented");
        // TODO: write better test

        truth = new CvMat(matSize, matSize, CvType.CC_32FC1, new Scalar(0));
        int blockSize = 5;
        int ksize = 7;
        double k = 0.1;
        Imgproc.cornerHarris(gray128, dst, blockSize, ksize, k);
        assertMatEqual(truth, dst, EPS);
    }

    public void testCornerHarrisMatMatIntIntDoubleInt() {
        fail("Not yet implemented");
        // TODO: write better test

        truth = new CvMat(matSize, matSize, CvType.CC_32FC1, new Scalar(0));
        int blockSize = 5;
        int ksize = 7;
        double k = 0.1;
        Imgproc.cornerHarris(gray255, dst, blockSize, ksize, k, Core.BORDER_REFLECT);
        assertMatEqual(truth, dst, EPS);
    }

    public void testCornerMinEigenValMatMatInt() {
        fail("Not yet implemented");
        // TODO: write better test

        CvMat src = new CvMat(imgprocSz, imgprocSz, CvType.CC_32FC1);
        src.put(0, 0, 1, 2);
        src.put(1, 0, 2, 1);
        int blockSize = 5;

        Imgproc.cornerMinEigenVal(src, dst, blockSize);

        truth = new CvMat(imgprocSz, imgprocSz, CvType.CC_32FC1, new Scalar(0));
        assertMatEqual(truth, dst, EPS);

        Imgproc.cornerMinEigenVal(gray255, dst, blockSize);

        truth = new CvMat(matSize, matSize, CvType.CC_32FC1, new Scalar(0));
        assertMatEqual(truth, dst, EPS);
    }

    public void testCornerMinEigenValMatMatIntInt() {
        CvMat src = CvMat.eye(3, 3, CvType.CC_32FC1);
        int blockSize = 3;
        int ksize = 5;

        Imgproc.cornerMinEigenVal(src, dst, blockSize, ksize);

        truth = new CvMat(3, 3, CvType.CC_32FC1) {
            {
                put(0, 0, 1. / 18, 1. / 36, 1. / 18);
                put(1, 0, 1. / 36, 1. / 18, 1. / 36);
                put(2, 0, 1. / 18, 1. / 36, 1. / 18);
            }
        };
        assertMatEqual(truth, dst, EPS);
    }

    public void testCornerMinEigenValMatMatIntIntInt() {
        CvMat src = CvMat.eye(3, 3, CvType.CC_32FC1);
        int blockSize = 3;
        int ksize = 5;

        Imgproc.cornerMinEigenVal(src, dst, blockSize, ksize, Core.BORDER_REFLECT);

        truth = new CvMat(3, 3, CvType.CC_32FC1) {
            {
                put(0, 0, 0.68055558, 0.92708349, 0.5868057);
                put(1, 0, 0.92708343, 0.92708343, 0.92708343);
                put(2, 0, 0.58680564, 0.92708343, 0.68055564);
            }
        };
        assertMatEqual(truth, dst, EPS);
    }

    public void testCornerSubPix() {
        CvMat img = new CvMat(matSize, matSize, CvType.CC_8U, new Scalar(128));
        Point truthPosition = new Point(img.cols() / 2, img.rows() / 2);

        Rect r = new Rect(new Point(0, 0), truthPosition);
        Imgproc.rectangle(img, r.tl(), r.br(), new Scalar(0), Core.FILLED);
        MatOfPoint2f corners = new MatOfPoint2f(new Point(truthPosition.x + 1, truthPosition.y + 1));
        CvSize winSize = new CvSize(2, 2);
        CvSize zeroZone = new CvSize(-1, -1);
        TermCriteria criteria = new TermCriteria(TermCriteria.EPS, 0, 0.01);

        Imgproc.cornerSubPix(img, corners, winSize, zeroZone, criteria);

        assertPointEquals(truthPosition, corners.toList().get(0), weakEPS);
    }

    public void testCvtColorMatMatInt() {
        fail("Not yet implemented");
    }

    public void testCvtColorMatMatIntInt() {
        fail("Not yet implemented");
    }

    public void testDilateMatMatMat() {
        CvMat kernel = new CvMat();

        Imgproc.dilate(gray255, dst, kernel);

        assertMatEqual(gray255, dst);

        Imgproc.dilate(gray1, dst, kernel);

        assertMatEqual(gray1, dst);
        // TODO_: write better test
    }

    public void testDilateMatMatMatPoint() {
        fail("Not yet implemented");
    }

    public void testDilateMatMatMatPointInt() {
        fail("Not yet implemented");
    }

    public void testDilateMatMatMatPointIntInt() {
        fail("Not yet implemented");
    }

    public void testDilateMatMatMatPointIntIntScalar() {
        fail("Not yet implemented");
    }

    public void testDistanceTransformWithLabels() {
        CvMat dstLables = getMat(CvType.CC_32SC1, 0);
        CvMat labels = new CvMat();

        Imgproc.distanceTransformWithLabels(gray128, dst, labels, Imgproc.CC_DIST_L2, 3);

        assertMatEqual(dstLables, labels);
        assertMatEqual(getMat(CvType.CC_32FC1, 8192), dst, EPS);
    }

    public void testDrawContoursMatListOfMatIntScalar() {
        Imgproc.rectangle(gray0, new Point(1, 2), new Point(7, 8), new Scalar(100));
        List<MatOfPoint> contours = new ArrayList<MatOfPoint>();
        Imgproc.findContours(gray0, contours, new CvMat(), Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);

        Imgproc.drawContours(gray0, contours, -1, new Scalar(0));

        assertEquals(0, Core.countNonZero(gray0));
    }

    public void testDrawContoursMatListOfMatIntScalarInt() {
        Imgproc.rectangle(gray0, new Point(1, 2), new Point(7, 8), new Scalar(100));
        List<MatOfPoint> contours = new ArrayList<MatOfPoint>();
        Imgproc.findContours(gray0, contours, new CvMat(), Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);

        Imgproc.drawContours(gray0, contours, -1, new Scalar(0), Core.FILLED);

        assertEquals(0, Core.countNonZero(gray0));
    }


    public void testDrawContoursMatListOfMatIntScalarIntIntMatIntPoint() {
        fail("Not yet implemented");
    }

    public void testEqualizeHist() {
        Imgproc.equalizeHist(gray0, dst);
        assertMatEqual(gray0, dst);

        Imgproc.equalizeHist(gray255, dst);
        assertMatEqual(gray255, dst);
        // TODO_: write better test
    }

    public void testErodeMatMatMat() {
        CvMat kernel = new CvMat();

        Imgproc.erode(gray128, dst, kernel);

        assertMatEqual(gray128, dst);
    }

    public void testErodeMatMatMatPointInt() {
        CvMat src = new CvMat(3, 3, CvType.CC_8U) {
            {
                put(0, 0, 15, 9, 10);
                put(1, 0, 10, 8, 12);
                put(2, 0, 12, 20, 25);
            }
        };
        CvMat kernel = new CvMat();

        Imgproc.erode(src, dst, kernel, anchorPoint, 10);

        truth = new CvMat(3, 3, CvType.CC_8U, new Scalar(8));
        assertMatEqual(truth, dst);
    }

    public void testErodeMatMatMatPointIntIntScalar() {
        CvMat src = new CvMat(3, 3, CvType.CC_8U) {
            {
                put(0, 0, 15, 9, 10);
                put(1, 0, 10, 8, 12);
                put(2, 0, 12, 20, 25);
            }
        };
        CvMat kernel = new CvMat();
        Scalar sc = new Scalar(3, 3);

        Imgproc.erode(src, dst, kernel, anchorPoint, 10, Core.BORDER_REFLECT, sc);

        truth = new CvMat(3, 3, CvType.CC_8U, new Scalar(8));
        assertMatEqual(truth, dst);
    }

    public void testFilter2DMatMatIntMat() {
        CvMat src = CvMat.eye(4, 4, CvType.CC_32F);
        CvMat kernel = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F, new Scalar(1));

        Imgproc.filter2D(src, dst, -1, kernel);

        truth = new CvMat(4, 4, CvType.CC_32F) {
            {
                put(0, 0, 2, 2, 1, 0);
                put(1, 0, 2, 2, 1, 0);
                put(2, 0, 1, 1, 2, 1);
                put(3, 0, 0, 0, 1, 2);
            }
        };
        assertMatEqual(truth, dst, EPS);
    }

    public void testFilter2DMatMatIntMatPointDouble() {
        fail("Not yet implemented");
    }

    public void testFilter2DMatMatIntMatPointDoubleInt() {
        CvMat kernel = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F, new Scalar(0));
        Point point = new Point(0, 0);

        Imgproc.filter2D(gray128, dst, -1, kernel, point, 2, Core.BORDER_CONSTANT);

        assertMatEqual(gray2, dst);
    }

    public void testFindContoursMatListOfMatMatIntInt() {
        CvMat img = new CvMat(50, 50, CvType.CC_8UC1, new Scalar(0));
        List<MatOfPoint> contours = new ArrayList<MatOfPoint>(5);
        CvMat hierarchy = new CvMat();

        Imgproc.findContours(img, contours, hierarchy, Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);

        // no contours on empty image
        assertEquals(contours.size(), 0);
        assertEquals(contours.size(), hierarchy.total());

        Imgproc.rectangle(img, new Point(10, 20), new Point(20, 30), new Scalar(100), 3, Imgproc.LINE_AA, 0);
        Imgproc.rectangle(img, new Point(30, 35), new Point(40, 45), new Scalar(200));

        Imgproc.findContours(img, contours, hierarchy, Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);

        // two contours of two rectangles
        assertEquals(contours.size(), 2);
        assertEquals(contours.size(), hierarchy.total());
    }

    public void testFindContoursMatListOfMatMatIntIntPoint() {
        CvMat img = new CvMat(50, 50, CvType.CC_8UC1, new Scalar(0));
        CvMat img2 = img.submat(5, 50, 3, 50);
        List<MatOfPoint> contours = new ArrayList<MatOfPoint>();
        List<MatOfPoint> contours2 = new ArrayList<MatOfPoint>();
        CvMat hierarchy = new CvMat();

        Imgproc.rectangle(img, new Point(10, 20), new Point(20, 30), new Scalar(100), 3, Imgproc.LINE_AA, 0);
        Imgproc.rectangle(img, new Point(30, 35), new Point(40, 45), new Scalar(200));

        Imgproc.findContours(img, contours, hierarchy, Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);
        Imgproc.findContours(img2, contours2, hierarchy, Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE, new Point(3, 5));

        assertEquals(contours.size(), contours2.size());
        assertMatEqual(contours.get(0), contours2.get(0));
        /*
        Log.d("findContours", "hierarchy=" + hierarchy);
        int iBuff[] = new int[ (int) (hierarchy.total() * hierarchy.channels()) ]; // [ Contour0 (next sibling num, previous sibling num, 1st child num, parent num), Contour1(...), ...
        hierarchy.get(0, 0, iBuff);
        Log.d("findContours", Arrays.toString(iBuff));
        */
    }

    public void testFitEllipse() {
        MatOfPoint2f points = new MatOfPoint2f(new Point(0, 0), new Point(-1, 1), new Point(1, 1), new Point(1, -1), new Point(-1, -1));
        RotatedRect rrect = new RotatedRect();

        rrect = Imgproc.fitEllipse(points);

        assertPointEquals(new Point(0, 0), rrect.center, EPS);
        assertEquals(2.828, rrect.size.width, EPS);
        assertEquals(2.828, rrect.size.height, EPS);
    }

    public void testFitLine() {
        CvMat points = new CvMat(1, 4, CvType.CC_32FC2);
        points.put(0, 0, 0, 0, 2, 3, 3, 4, 5, 8);

        CvMat linePoints = new CvMat(4, 1, CvType.CC_32FC1);
        linePoints.put(0, 0, 0.53196341, 0.84676737, 2.496531, 3.7467217);

        Imgproc.fitLine(points, dst, Imgproc.CC_DIST_L12, 0, 0.01, 0.01);

        assertMatEqual(linePoints, dst, EPS);
    }

    public void testFloodFillMatMatPointScalar() {
        CvMat mask = new CvMat(matSize + 2, matSize + 2, CvType.CC_8U, new Scalar(0));
        CvMat img = gray0;
        Imgproc.circle(mask, new Point(matSize / 2 + 1, matSize / 2 + 1), 3, new Scalar(2));

        int retval = Imgproc.floodFill(img, mask, new Point(matSize / 2, matSize / 2), new Scalar(1));

        assertEquals(Core.countNonZero(img), retval);
        Imgproc.circle(mask, new Point(matSize / 2 + 1, matSize / 2 + 1), 3, new Scalar(0));
        assertEquals(retval + 4 * (matSize + 1), Core.countNonZero(mask));
        assertMatEqual(mask.submat(1, matSize + 1, 1, matSize + 1), img);
    }

    public void testFloodFillMatMatPointScalar_WithoutMask() {
        CvMat img = gray0;
        Imgproc.circle(img, new Point(matSize / 2, matSize / 2), 3, new Scalar(2));

        // TODO: ideally we should pass null instead of "new CvMat()"
        int retval = Imgproc.floodFill(img, new CvMat(), new Point(matSize / 2, matSize / 2), new Scalar(1));

        Imgproc.circle(img, new Point(matSize / 2, matSize / 2), 3, new Scalar(0));
        assertEquals(Core.countNonZero(img), retval);
    }

    public void testFloodFillMatMatPointScalarRect() {
        fail("Not yet implemented");
    }

    public void testFloodFillMatMatPointScalarRectScalar() {
        fail("Not yet implemented");
    }

    public void testFloodFillMatMatPointScalarRectScalarScalar() {
        fail("Not yet implemented");
    }

    public void testFloodFillMatMatPointScalarRectScalarScalarInt() {
        fail("Not yet implemented");
    }

    public void testGaussianBlurMatMatSizeDouble() {
        Imgproc.GaussianBlur(gray0, dst, size, 1);
        assertMatEqual(gray0, dst);

        Imgproc.GaussianBlur(gray2, dst, size, 1);
        assertMatEqual(gray2, dst);
    }

    public void testGaussianBlurMatMatSizeDoubleDouble() {
        Imgproc.GaussianBlur(gray2, dst, size, 0, 0);

        assertMatEqual(gray2, dst);
        // TODO_: write better test
    }

    public void testGaussianBlurMatMatSizeDoubleDoubleInt() {
        Imgproc.GaussianBlur(gray2, dst, size, 1, 3, Core.BORDER_REFLECT);

        assertMatEqual(gray2, dst);
        // TODO_: write better test
    }

    public void testGetAffineTransform() {
        MatOfPoint2f src = new MatOfPoint2f(new Point(2, 3), new Point(3, 1), new Point(1, 4));
        MatOfPoint2f dst = new MatOfPoint2f(new Point(3, 3), new Point(7, 4), new Point(5, 6));

        CvMat transform = Imgproc.getAffineTransform(src, dst);

        CvMat truth = new CvMat(2, 3, CvType.CC_64FC1) {
            {
                put(0, 0, -8, -6, 37);
                put(1, 0, -7, -4, 29);
            }
        };
        assertMatEqual(truth, transform, EPS);
    }

    public void testGetDefaultNewCameraMatrixMat() {
        CvMat mtx = Imgproc.getDefaultNewCameraMatrix(gray0);

        assertFalse(mtx.empty());
        assertEquals(0, Core.countNonZero(mtx));
    }

    public void testGetDefaultNewCameraMatrixMatSizeBoolean() {
        CvMat mtx = Imgproc.getDefaultNewCameraMatrix(gray0, size, true);

        assertFalse(mtx.empty());
        assertFalse(0 == Core.countNonZero(mtx));
        // TODO_: write better test
    }

    public void testGetDerivKernelsMatMatIntIntInt() {
        CvMat kx = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F);
        CvMat ky = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F);
        CvMat expKx = new CvMat(3, 1, CvType.CC_32F);
        CvMat expKy = new CvMat(3, 1, CvType.CC_32F);
        kx.put(0, 0, 1, 1);
        kx.put(1, 0, 1, 1);
        ky.put(0, 0, 2, 2);
        ky.put(1, 0, 2, 2);
        expKx.put(0, 0, 1, -2, 1);
        expKy.put(0, 0, 1, -2, 1);

        Imgproc.getDerivKernels(kx, ky, 2, 2, 3);

        assertMatEqual(expKx, kx, EPS);
        assertMatEqual(expKy, ky, EPS);
    }

    public void testGetDerivKernelsMatMatIntIntIntBooleanInt() {
        CvMat kx = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F);
        CvMat ky = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F);
        CvMat expKx = new CvMat(3, 1, CvType.CC_32F);
        CvMat expKy = new CvMat(3, 1, CvType.CC_32F);
        kx.put(0, 0, 1, 1);
        kx.put(1, 0, 1, 1);
        ky.put(0, 0, 2, 2);
        ky.put(1, 0, 2, 2);
        expKx.put(0, 0, 1, -2, 1);
        expKy.put(0, 0, 1, -2, 1);

        Imgproc.getDerivKernels(kx, ky, 2, 2, 3, true, CvType.CC_32F);

        assertMatEqual(expKx, kx, EPS);
        assertMatEqual(expKy, ky, EPS);
        // TODO_: write better test
    }

    public void testGetGaussianKernelIntDouble() {
        dst = Imgproc.getGaussianKernel(1, 0.5);

        truth = new CvMat(1, 1, CvType.CC_64FC1, new Scalar(1));
        assertMatEqual(truth, dst, EPS);
    }

    public void testGetGaussianKernelIntDoubleInt() {
        dst = Imgproc.getGaussianKernel(3, 0.8, CvType.CC_32F);

        truth = new CvMat(3, 1, CvType.CC_32F);
        truth.put(0, 0, 0.23899426, 0.52201146, 0.23899426);
        assertMatEqual(truth, dst, EPS);
    }

    public void testGetPerspectiveTransform() {
        fail("Not yet implemented");
    }

    public void testGetRectSubPixMatSizePointMat() {
        CvSize size = new CvSize(3, 3);
        Point center = new Point(gray255.cols() / 2, gray255.rows() / 2);

        Imgproc.getRectSubPix(gray255, size, center, dst);

        truth = new CvMat(3, 3, CvType.CC_8U, new Scalar(255));
        assertMatEqual(truth, dst);
    }

    public void testGetRectSubPixMatSizePointMatInt() {
        CvMat src = new CvMat(10, 10, CvType.CC_32F, new Scalar(2));
        CvSize patchSize = new CvSize(5, 5);
        Point center = new Point(src.cols() / 2, src.rows() / 2);

        Imgproc.getRectSubPix(src, patchSize, center, dst);

        truth = new CvMat(5, 5, CvType.CC_32F, new Scalar(2));
        assertMatEqual(truth, dst, EPS);
    }

    public void testGetRotationMatrix2D() {
        Point center = new Point(0, 0);

        dst = Imgproc.getRotationMatrix2D(center, 0, 1);

        truth = new CvMat(2, 3, CvType.CC_64F) {
            {
                put(0, 0, 1, 0, 0);
                put(1, 0, 0, 1, 0);
            }
        };

        assertMatEqual(truth, dst, EPS);
    }

    public void testGetStructuringElementIntSize() {
        dst = Imgproc.getStructuringElement(Imgproc.MORPH_RECT, size);

        truth = new CvMat(3, 3, CvType.CC_8UC1, new Scalar(1));
        assertMatEqual(truth, dst);
    }

    public void testGetStructuringElementIntSizePoint() {
        dst = Imgproc.getStructuringElement(Imgproc.MORPH_CROSS, size, anchorPoint);

        truth = new CvMat(3, 3, CvType.CC_8UC1) {
            {
                put(0, 0, 0, 0, 1);
                put(1, 0, 0, 0, 1);
                put(2, 0, 1, 1, 1);
            }
        };
        assertMatEqual(truth, dst);
    }

    public void testGoodFeaturesToTrackMatListOfPointIntDoubleDouble() {
        CvMat src = gray0;
        Imgproc.rectangle(src, new Point(2, 2), new Point(8, 8), new Scalar(100), -1);
        MatOfPoint lp = new MatOfPoint();

        Imgproc.goodFeaturesToTrack(src, lp, 100, 0.01, 3);

        assertEquals(4, lp.total());
    }

    public void testGoodFeaturesToTrackMatListOfPointIntDoubleDoubleMatIntBooleanDouble() {
        CvMat src = gray0;
        Imgproc.rectangle(src, new Point(2, 2), new Point(8, 8), new Scalar(100), -1);
        MatOfPoint lp = new MatOfPoint();

        Imgproc.goodFeaturesToTrack(src, lp, 100, 0.01, 3, gray1, 4, true, 0);

        assertEquals(4, lp.total());
    }

    public void testGrabCutMatMatRectMatMatInt() {
        fail("Not yet implemented");
    }

    public void testGrabCutMatMatRectMatMatIntInt() {
        fail("Not yet implemented");
    }

    public void testHoughCirclesMatMatIntDoubleDouble() {
        int sz = 512;
        CvMat img = new CvMat(sz, sz, CvType.CC_8U, new Scalar(128));
        CvMat circles = new CvMat();

        Imgproc.HoughCircles(img, circles, Imgproc.CC_HOUGH_GRADIENT, 2, img.rows() / 4);

        assertEquals(0, circles.cols());
    }

    public void testHoughCirclesMatMatIntDoubleDouble1() {
        int sz = 512;
        CvMat img = new CvMat(sz, sz, CvType.CC_8U, new Scalar(128));
        CvMat circles = new CvMat();

        Point center = new Point(img.cols() / 2, img.rows() / 2);
        int radius = Math.min(img.cols() / 4, img.rows() / 4);
        Imgproc.circle(img, center, radius, colorBlack, 3);

        Imgproc.HoughCircles(img, circles, Imgproc.CC_HOUGH_GRADIENT, 2, img.rows() / 4);

        assertEquals(1, circles.cols());
    }

    public void testHoughCirclesMatMatIntDoubleDoubleDoubleDoubleIntInt() {
        fail("Not yet implemented");
    }

    public void testHoughLinesMatMatDoubleDoubleInt() {
        int sz = 512;
        CvMat img = new CvMat(sz, sz, CvType.CC_8U, new Scalar(0));
        Point point1 = new Point(50, 50);
        Point point2 = new Point(img.cols() / 2, img.rows() / 2);
        Imgproc.line(img, point1, point2, colorWhite, 1);
        CvMat lines = new CvMat();

        Imgproc.HoughLines(img, lines, 1, 3.1415926/180, 100);

        assertEquals(1, lines.cols());

        /*
        Log.d("HoughLines", "lines=" + lines);
        int num = (int)lines.total();
        int buff[] = new int[num*4]; //[ (x1, y1, x2, y2), (...), ...]
        lines.get(0, 0, buff);
        Log.d("HoughLines", "lines=" + Arrays.toString(buff));
        */
    }

    public void testHoughLinesMatMatDoubleDoubleIntDouble() {
        fail("Not yet implemented");
    }

    public void testHoughLinesMatMatDoubleDoubleIntDoubleDouble() {
        fail("Not yet implemented");
    }

    public void testHoughLinesPMatMatDoubleDoubleInt() {
        int sz = 512;
        CvMat img = new CvMat(sz, sz, CvType.CC_8U, new Scalar(0));
        Point point1 = new Point(0, 0);
        Point point2 = new Point(sz, sz);
        Point point3 = new Point(sz, 0);
        Point point4 = new Point(2*sz/3, sz/3);
        Imgproc.line(img, point1, point2, Scalar.all(255), 1);
        Imgproc.line(img, point3, point4, Scalar.all(255), 1);
        CvMat lines = new CvMat();

        Imgproc.HoughLinesP(img, lines, 1, 3.1415926/180, 100);

        assertEquals(2, lines.rows());

        /*
        Log.d("HoughLinesP", "lines=" + lines);
        int num = (int)lines.cols();
        int buff[] = new int[num*4]; // CC_32SC4 as [ (x1, y1, x2, y2), (...), ...]
        lines.get(0, 0, buff);
        Log.d("HoughLinesP", "lines=" + Arrays.toString(buff));
        */
    }

    public void testHoughLinesPMatMatDoubleDoubleIntDouble() {
        fail("Not yet implemented");
    }

    public void testHoughLinesPMatMatDoubleDoubleIntDoubleDouble() {
        fail("Not yet implemented");
    }

    public void testHuMoments() {
        fail("Not yet implemented");
    }

    public void testInitUndistortRectifyMap() {
        fail("Not yet implemented");
        CvMat cameraMatrix = new CvMat(3, 3, CvType.CC_32F);
        cameraMatrix.put(0, 0, 1, 0, 1);
        cameraMatrix.put(1, 0, 0, 1, 1);
        cameraMatrix.put(2, 0, 0, 0, 1);

        CvMat R = new CvMat(3, 3, CvType.CC_32F, new Scalar(2));
        CvMat newCameraMatrix = new CvMat(3, 3, CvType.CC_32F, new Scalar(3));

        CvMat distCoeffs = new CvMat();
        CvMat map1 = new CvMat();
        CvMat map2 = new CvMat();

        // TODO: complete this test
        Imgproc.initUndistortRectifyMap(cameraMatrix, distCoeffs, R, newCameraMatrix, size, CvType.CC_32F, map1, map2);
    }

    public void testInitWideAngleProjMapMatMatSizeIntIntMatMat() {
        fail("Not yet implemented");
        CvMat cameraMatrix = new CvMat(3, 3, CvType.CC_32F);
        CvMat distCoeffs = new CvMat(1, 4, CvType.CC_32F);
        // CvSize imageSize = new CvSize(2, 2);

        cameraMatrix.put(0, 0, 1, 0, 1);
        cameraMatrix.put(1, 0, 0, 1, 2);
        cameraMatrix.put(2, 0, 0, 0, 1);

        distCoeffs.put(0, 0, 1, 3, 2, 4);
        truth = new CvMat(3, 3, CvType.CC_32F);
        truth.put(0, 0, 0, 0, 0);
        truth.put(1, 0, 0, 0, 0);
        truth.put(2, 0, 0, 3, 0);
        // TODO: No documentation for this function
        // Imgproc.initWideAngleProjMap(cameraMatrix, distCoeffs, imageSize,
        // 5, m1type, truthput1, truthput2);
    }

    public void testInitWideAngleProjMapMatMatSizeIntIntMatMatInt() {
        fail("Not yet implemented");
    }

    public void testInitWideAngleProjMapMatMatSizeIntIntMatMatIntDouble() {
        fail("Not yet implemented");
    }

    public void testIntegral2MatMatMat() {
        CvMat src = new CvMat(3, 3, CvType.CC_32F, new Scalar(3));
        CvMat expSum = new CvMat(4, 4, CvType.CC_64F);
        CvMat expSqsum = new CvMat(4, 4, CvType.CC_64F);
        CvMat sum = new CvMat();
        CvMat sqsum = new CvMat();

        expSum.put(0, 0, 0, 0, 0, 0);
        expSum.put(1, 0, 0, 3, 6, 9);
        expSum.put(2, 0, 0, 6, 12, 18);
        expSum.put(3, 0, 0, 9, 18, 27);

        expSqsum.put(0, 0, 0, 0, 0, 0);
        expSqsum.put(1, 0, 0, 9, 18, 27);
        expSqsum.put(2, 0, 0, 18, 36, 54);
        expSqsum.put(3, 0, 0, 27, 54, 81);

        Imgproc.integral2(src, sum, sqsum);

        assertMatEqual(expSum, sum, EPS);
        assertMatEqual(expSqsum, sqsum, EPS);
    }

    public void testIntegral2MatMatMatInt() {
        CvMat src = new CvMat(3, 3, CvType.CC_32F, new Scalar(3));
        CvMat expSum = new CvMat(4, 4, CvType.CC_64F);
        CvMat expSqsum = new CvMat(4, 4, CvType.CC_64F);
        CvMat sum = new CvMat();
        CvMat sqsum = new CvMat();

        expSum.put(0, 0, 0, 0, 0, 0);
        expSum.put(1, 0, 0, 3, 6, 9);
        expSum.put(2, 0, 0, 6, 12, 18);
        expSum.put(3, 0, 0, 9, 18, 27);

        expSqsum.put(0, 0, 0, 0, 0, 0);
        expSqsum.put(1, 0, 0, 9, 18, 27);
        expSqsum.put(2, 0, 0, 18, 36, 54);
        expSqsum.put(3, 0, 0, 27, 54, 81);

        Imgproc.integral2(src, sum, sqsum, CvType.CC_64F, CvType.CC_64F);

        assertMatEqual(expSum, sum, EPS);
        assertMatEqual(expSqsum, sqsum, EPS);
    }

    public void testIntegral3MatMatMatMat() {
        CvMat src = new CvMat(1, 1, CvType.CC_32F, new Scalar(1));
        CvMat expSum = new CvMat(imgprocSz, imgprocSz, CvType.CC_64F);
        CvMat expSqsum = new CvMat(imgprocSz, imgprocSz, CvType.CC_64F);
        CvMat expTilted = new CvMat(imgprocSz, imgprocSz, CvType.CC_64F);
        CvMat sum = new CvMat();
        CvMat sqsum = new CvMat();
        CvMat tilted = new CvMat();

        expSum.put(0, 0, 0, 0);
        expSum.put(1, 0, 0, 1);

        expSqsum.put(0, 0, 0, 0);
        expSqsum.put(1, 0, 0, 1);

        expTilted.put(0, 0, 0, 0);
        expTilted.put(1, 0, 0, 1);

        Imgproc.integral3(src, sum, sqsum, tilted);

        assertMatEqual(expSum, sum, EPS);
        assertMatEqual(expSqsum, sqsum, EPS);
        assertMatEqual(expTilted, tilted, EPS);
    }

    public void testIntegral3MatMatMatMatInt() {
        CvMat src = new CvMat(1, 1, CvType.CC_32F, new Scalar(1));
        CvMat expSum = new CvMat(imgprocSz, imgprocSz, CvType.CC_64F);
        CvMat expSqsum = new CvMat(imgprocSz, imgprocSz, CvType.CC_64F);
        CvMat expTilted = new CvMat(imgprocSz, imgprocSz, CvType.CC_64F);
        CvMat sum = new CvMat();
        CvMat sqsum = new CvMat();
        CvMat tilted = new CvMat();

        expSum.put(0, 0, 0, 0);
        expSum.put(1, 0, 0, 1);

        expSqsum.put(0, 0, 0, 0);
        expSqsum.put(1, 0, 0, 1);

        expTilted.put(0, 0, 0, 0);
        expTilted.put(1, 0, 0, 1);

        Imgproc.integral3(src, sum, sqsum, tilted, CvType.CC_64F, CvType.CC_64F);

        assertMatEqual(expSum, sum, EPS);
        assertMatEqual(expSqsum, sqsum, EPS);
        assertMatEqual(expTilted, tilted, EPS);
    }

    public void testIntegralMatMat() {
        CvMat src = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F, new Scalar(2));

        Imgproc.integral(src, dst);

        truth = new CvMat(3, 3, CvType.CC_64F) {
            {
                put(0, 0, 0, 0, 0);
                put(1, 0, 0, 2, 4);
                put(2, 0, 0, 4, 8);
            }
        };
        assertMatEqual(truth, dst, EPS);
    }

    public void testIntegralMatMatInt() {
        CvMat src = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F, new Scalar(2));

        Imgproc.integral(src, dst, CvType.CC_64F);

        truth = new CvMat(3, 3, CvType.CC_64F) {
            {
                put(0, 0, 0, 0, 0);
                put(1, 0, 0, 2, 4);
                put(2, 0, 0, 4, 8);
            }
        };
        assertMatEqual(truth, dst, EPS);
    }

    public void testInvertAffineTransform() {
        CvMat src = new CvMat(2, 3, CvType.CC_64F, new Scalar(1));

        Imgproc.invertAffineTransform(src, dst);

        truth = new CvMat(2, 3, CvType.CC_64F, new Scalar(0));
        assertMatEqual(truth, dst, EPS);
    }

    public void testIsContourConvex() {
        MatOfPoint contour1 = new MatOfPoint(new Point(0, 0), new Point(10, 0), new Point(10, 10), new Point(5, 4));

        assertFalse(Imgproc.isContourConvex(contour1));

        MatOfPoint contour2 = new MatOfPoint(new Point(0, 0), new Point(10, 0), new Point(10, 10), new Point(5, 6));

        assertTrue(Imgproc.isContourConvex(contour2));
    }

    public void testLaplacianMatMatInt() {
        Imgproc.Laplacian(gray0, dst, CvType.CC_8U);

        assertMatEqual(gray0, dst);
    }

    public void testLaplacianMatMatIntIntDoubleDouble() {
        CvMat src = CvMat.eye(imgprocSz, imgprocSz, CvType.CC_32F);

        Imgproc.Laplacian(src, dst, CvType.CC_32F, 1, 2, EPS);

        truth = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F) {
            {
                put(0, 0, -7.9990001, 8.0009995);
                put(1, 0, 8.0009995, -7.9990001);
            }
        };
        assertMatEqual(truth, dst, EPS);
    }

    public void testLaplacianMatMatIntIntDoubleDoubleInt() {
        CvMat src = new CvMat(3, 3, CvType.CC_32F, new Scalar(2));

        Imgproc.Laplacian(src, dst, CvType.CC_32F, 1, 2, EPS, Core.BORDER_REFLECT);

        truth = new CvMat(3, 3, CvType.CC_32F, new Scalar(0.00099945068));
        assertMatEqual(truth, dst, EPS);
    }

    public void testMatchShapes() {
        CvMat contour1 = new CvMat(1, 4, CvType.CC_32FC2);
        CvMat contour2 = new CvMat(1, 4, CvType.CC_32FC2);
        contour1.put(0, 0, 1, 1, 5, 1, 4, 3, 6, 2);
        contour2.put(0, 0, 1, 1, 6, 1, 4, 1, 2, 5);

        double distance = Imgproc.matchShapes(contour1, contour2, Imgproc.CC_CONTOURS_MATCH_I1, 1);

        assertEquals(2.81109697365334, distance, EPS);
    }

    public void testMatchTemplate() {
        CvMat image = new CvMat(imgprocSz, imgprocSz, CvType.CC_8U);
        CvMat templ = new CvMat(imgprocSz, imgprocSz, CvType.CC_8U);
        image.put(0, 0, 1, 2, 3, 4);
        templ.put(0, 0, 5, 6, 7, 8);

        Imgproc.matchTemplate(image, templ, dst, Imgproc.TM_CCORR);

        truth = new CvMat(1, 1, CvType.CC_32F, new Scalar(70));
        assertMatEqual(truth, dst, EPS);

        Imgproc.matchTemplate(gray255, gray0, dst, Imgproc.TM_CCORR);

        truth = new CvMat(1, 1, CvType.CC_32F, new Scalar(0));
        assertMatEqual(truth, dst, EPS);
    }

    public void testMedianBlur() {
        Imgproc.medianBlur(gray255, dst, 5);
        assertMatEqual(gray255, dst);

        Imgproc.medianBlur(gray2, dst, 3);
        assertMatEqual(gray2, dst);
        // TODO_: write better test
    }

    public void testMinAreaRect() {
        MatOfPoint2f points = new MatOfPoint2f(new Point(1, 1), new Point(5, 1), new Point(4, 3), new Point(6, 2));

        RotatedRect rrect = Imgproc.minAreaRect(points);

        assertEquals(new CvSize(2, 5), rrect.size);
        assertEquals(-90., rrect.angle);
        assertEquals(new Point(3.5, 2), rrect.center);
    }

    public void testMinEnclosingCircle() {
        MatOfPoint2f points = new MatOfPoint2f(new Point(0, 0), new Point(-100, 0), new Point(0, -100), new Point(100, 0), new Point(0, 100));
        Point actualCenter = new Point();
        float[] radius = new float[1];

        Imgproc.minEnclosingCircle(points, actualCenter, radius);

        assertEquals(new Point(0, 0), actualCenter);
        assertEquals(100.0f, radius[0], 1.0);
    }

    public void testMomentsMat() {
        fail("Not yet implemented");
    }

    public void testMomentsMatBoolean() {
        fail("Not yet implemented");
    }

    public void testMorphologyExMatMatIntMat() {
        Imgproc.morphologyEx(gray255, dst, Imgproc.MORPH_GRADIENT, gray0);

        assertMatEqual(gray0, dst);
        // TODO_: write better test
    }

    public void testMorphologyExMatMatIntMatPointInt() {
        CvMat src = CvMat.eye(imgprocSz, imgprocSz, CvType.CC_8U);

        CvMat kernel = new CvMat(imgprocSz, imgprocSz, CvType.CC_8U, new Scalar(0));
        Point point = new Point(0, 0);

        Imgproc.morphologyEx(src, dst, Imgproc.MORPH_CLOSE, kernel, point, 10);

        truth = CvMat.eye(imgprocSz, imgprocSz, CvType.CC_8U);
        assertMatEqual(truth, dst);
        // TODO_: write better test
    }


    public void testMorphologyExMatMatIntMatPointIntIntScalar() {
        CvMat src = new CvMat(imgprocSz, imgprocSz, CvType.CC_8U);
        src.put(0, 0, 2, 1);
        src.put(1, 0, 2, 1);

        CvMat kernel = new CvMat(imgprocSz, imgprocSz, CvType.CC_8U, new Scalar(1));
        Point point = new Point(1, 1);
        Scalar sc = new Scalar(3, 3);

        Imgproc.morphologyEx(src, dst, Imgproc.MORPH_TOPHAT, kernel, point, 10, Core.BORDER_REFLECT, sc);
        truth = new CvMat(imgprocSz, imgprocSz, CvType.CC_8U) {
            {
                put(0, 0, 1, 0);
                put(1, 0, 1, 0);
            }
        };
        assertMatEqual(truth, dst);
        // TODO_: write better test
    }

    public void testPointPolygonTest() {
        MatOfPoint2f contour = new MatOfPoint2f(new Point(0, 0), new Point(1, 3), new Point(3, 4), new Point(4, 3), new Point(2, 1));
        double sign1 = Imgproc.pointPolygonTest(contour, new Point(2, 2), false);
        assertEquals(1.0, sign1);

        double sign2 = Imgproc.pointPolygonTest(contour, new Point(4, 4), true);
        assertEquals(-Math.sqrt(0.5), sign2);
    }

    public void testPreCornerDetectMatMatInt() {
        CvMat src = new CvMat(4, 4, CvType.CC_32F, new Scalar(1));
        int ksize = 3;

        Imgproc.preCornerDetect(src, dst, ksize);

        truth = new CvMat(4, 4, CvType.CC_32F, new Scalar(0));
        assertMatEqual(truth, dst, EPS);
    }

    public void testPreCornerDetectMatMatIntInt() {
        CvMat src = new CvMat(4, 4, CvType.CC_32F, new Scalar(1));
        int ksize = 3;

        Imgproc.preCornerDetect(src, dst, ksize, Core.BORDER_REFLECT);

        truth = new CvMat(4, 4, CvType.CC_32F, new Scalar(0));
        assertMatEqual(truth, dst, EPS);
        // TODO_: write better test
    }

    public void testPyrDownMatMat() {
        CvMat src = new CvMat(4, 4, CvType.CC_32F) {
            {
                put(0, 0, 2, 1, 4, 2);
                put(1, 0, 3, 2, 6, 8);
                put(2, 0, 4, 6, 8, 10);
                put(3, 0, 12, 32, 6, 18);
            }
        };

        Imgproc.pyrDown(src, dst);

        truth = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F) {
            {
                put(0, 0, 2.78125, 4.609375);
                put(1, 0, 8.546875, 8.8515625);
            }
        };
        assertMatEqual(truth, dst, EPS);
    }

    public void testPyrDownMatMatSize() {
        CvMat src = new CvMat(4, 4, CvType.CC_32F) {
            {
                put(0, 0, 2, 1, 4, 2);
                put(1, 0, 3, 2, 6, 8);
                put(2, 0, 4, 6, 8, 10);
                put(3, 0, 12, 32, 6, 18);
            }
        };
        CvSize dstSize = new CvSize(2, 2);

        Imgproc.pyrDown(src, dst, dstSize);

        truth = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F) {
            {
                put(0, 0, 2.78125, 4.609375);
                put(1, 0, 8.546875, 8.8515625);
            }
        };
        assertMatEqual(truth, dst, EPS);
        // TODO_: write better test
    }

    public void testPyrMeanShiftFilteringMatMatDoubleDouble() {
        CvMat src = new CvMat(matSize, matSize, CvType.CC_8UC3, new Scalar(0));

        Imgproc.pyrMeanShiftFiltering(src, dst, 10, 50);

        assertMatEqual(src, dst);
        // TODO_: write better test
    }

    public void testPyrMeanShiftFilteringMatMatDoubleDoubleInt() {
        fail("Not yet implemented");
    }

    public void testPyrMeanShiftFilteringMatMatDoubleDoubleIntTermCriteria() {
        fail("Not yet implemented");
    }

    public void testPyrUpMatMat() {
        CvMat src = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F);
        src.put(0, 0, 2, 1);
        src.put(1, 0, 3, 2);

        Imgproc.pyrUp(src, dst);

        truth = new CvMat(4, 4, CvType.CC_32F) {
            {
                put(0, 0, 2,     1.75,  1.375, 1.25);
                put(1, 0, 2.25,  2,     1.625, 1.5);
                put(2, 0, 2.625, 2.375, 2,     1.875);
                put(3, 0, 2.75,  2.5,   2.125, 2);
            }
        };
        assertMatEqual(truth, dst, EPS);
    }

    public void testPyrUpMatMatSize() {
        fail("Not yet implemented");
    }

    public void testRemapMatMatMatMatInt() {
        fail("Not yet implemented");
        // this test does something weird
        CvMat src = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F, new Scalar(2));
        CvMat map1 = new CvMat(1, 3, CvType.CC_32FC1);
        CvMat map2 = new CvMat(1, 3, CvType.CC_32FC1);

        map1.put(0, 0, 3, 6, 5);
        map2.put(0, 0, 4, 8, 12);

        Imgproc.remap(src, dst, map1, map2, Imgproc.INTER_LINEAR);

        truth = new CvMat(1, 3, CvType.CC_32F, new Scalar(0));
        assertMatEqual(truth, dst, EPS);
    }

    public void testRemapMatMatMatMatIntIntScalar() {
        fail("Not yet implemented");
        // this test does something weird
        CvMat src = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F, new Scalar(2));
        CvMat map1 = new CvMat(1, 3, CvType.CC_32FC1);
        CvMat map2 = new CvMat(1, 3, CvType.CC_32FC1);

        Scalar sc = new Scalar(0);

        map1.put(0, 0, 3, 6, 5, 0);
        map2.put(0, 0, 4, 8, 12);

        truth = new CvMat(1, 3, CvType.CC_32F, new Scalar(2));

        Imgproc.remap(src, dst, map1, map2, Imgproc.INTER_LINEAR, Core.BORDER_REFLECT, sc);
        assertMatEqual(truth, dst, EPS);
    }

    public void testResizeMatMatSize() {
        CvMat src = new CvMat(imgprocSz, imgprocSz, CvType.CC_8UC1, new Scalar(1));
        CvSize dsize = new CvSize(1, 1);

        Imgproc.resize(src, dst, dsize);

        truth = new CvMat(1, 1, CvType.CC_8UC1, new Scalar(1));
        assertMatEqual(truth, dst);
    }

    public void testResizeMatMatSizeDoubleDoubleInt() {
        Imgproc.resize(gray255, dst, new CvSize(2, 2), 0, 0, Imgproc.INTER_AREA);

        truth = new CvMat(2, 2, CvType.CC_8UC1, new Scalar(255));
        assertMatEqual(truth, dst);
        // TODO_: write better test
    }

    public void testScharrMatMatIntIntInt() {
        CvMat src = CvMat.eye(imgprocSz, imgprocSz, CvType.CC_32F);

        Imgproc.Scharr(src, dst, CvType.CC_32F, 1, 0);

        truth = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F, new Scalar(0));
        assertMatEqual(truth, dst, EPS);
    }

    public void testScharrMatMatIntIntIntDoubleDouble() {
        CvMat src = CvMat.eye(imgprocSz, imgprocSz, CvType.CC_32F);

        Imgproc.Scharr(src, dst, CvType.CC_32F, 1, 0, 1.5, 0.001);

        truth = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F, new Scalar(0.001));
        assertMatEqual(truth, dst, EPS);
    }

    public void testScharrMatMatIntIntIntDoubleDoubleInt() {
        CvMat src = CvMat.eye(3, 3, CvType.CC_32F);

        Imgproc.Scharr(src, dst, CvType.CC_32F, 1, 0, 1.5, 0, Core.BORDER_REFLECT);

        truth = new CvMat(3, 3, CvType.CC_32F) {
            {
                put(0, 0, -15, -19.5, -4.5);
                put(1, 0, 10.5, 0, -10.5);
                put(2, 0, 4.5, 19.5, 15);
            }
        };
        assertMatEqual(truth, dst, EPS);
    }

    public void testSepFilter2DMatMatIntMatMat() {
        CvMat src = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F, new Scalar(2));
        CvMat kernelX = new CvMat(1, 3, CvType.CC_32FC1);
        CvMat kernelY = new CvMat(1, 3, CvType.CC_32FC1);
        kernelX.put(0, 0, 4, 3, 7);
        kernelY.put(0, 0, 9, 4, 2);

        Imgproc.sepFilter2D(src, dst, CvType.CC_32F, kernelX, kernelY);

        truth = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F, new Scalar(420));
        assertMatEqual(truth, dst, EPS);
    }

    public void testSepFilter2DMatMatIntMatMatPointDouble() {
        CvMat src = new CvMat(imgprocSz, imgprocSz, CvType.CC_32FC1, new Scalar(2));
        CvMat kernelX = new CvMat(1, 3, CvType.CC_32FC1);
        kernelX.put(0, 0, 2, 2, 2);
        CvMat kernelY = new CvMat(1, 3, CvType.CC_32FC1);
        kernelY.put(0, 0, 1, 1, 1);

        Imgproc.sepFilter2D(src, dst, CvType.CC_32F, kernelX, kernelY, anchorPoint, weakEPS);

        truth = new CvMat(imgprocSz, imgprocSz, CvType.CC_32F, new Scalar(36 + weakEPS));
        assertMatEqual(truth, dst, EPS);
    }

    public void testSepFilter2DMatMatIntMatMatPointDoubleInt() {
        CvMat kernelX = new CvMat(1, 3, CvType.CC_32FC1);
        kernelX.put(0, 0, 2, 2, 2);

        CvMat kernelY = new CvMat(1, 3, CvType.CC_32FC1);
        kernelY.put(0, 0, 1, 1, 1);

        Imgproc.sepFilter2D(gray0, dst, CvType.CC_32F, kernelX, kernelY, anchorPoint, weakEPS, Core.BORDER_REFLECT);

        truth = new CvMat(10, 10, CvType.CC_32F, new Scalar(weakEPS));
        assertMatEqual(truth, dst, EPS);
        // TODO_: write better test
    }

    public void testSobelMatMatIntIntInt() {
        Imgproc.Sobel(gray255, dst, CvType.CC_8U, 1, 0);

        assertMatEqual(gray0, dst);
    }

    public void testSobelMatMatIntIntIntIntDoubleDouble() {
        Imgproc.Sobel(gray255, dst, CvType.CC_8U, 1, 0, 3, 2, 0.001);
        assertMatEqual(gray0, dst);
        // TODO_: write better test
    }

    public void testSobelMatMatIntIntIntIntDoubleDoubleInt() {
        CvMat src = new CvMat(3, 3, CvType.CC_32F) {
            {
                put(0, 0, 2, 0, 1);
                put(1, 0, 6, 4, 3);
                put(2, 0, 1, 0, 2);
            }
        };

        Imgproc.Sobel(src, dst, CvType.CC_32F, 1, 0, 3, 2, 0, Core.BORDER_REPLICATE);

        truth = new CvMat(3, 3, CvType.CC_32F) {
            {
                put(0, 0, -16, -12, 4);
                put(1, 0, -14, -12, 2);
                put(2, 0, -10, 0, 10);
            }
        };
        assertMatEqual(truth, dst, EPS);
    }

    public void testThreshold() {
        Imgproc.threshold(makeMask(gray0.clone(), 10), dst, 5, 255, Imgproc.THRESH_TRUNC);
        assertMatEqual(makeMask(gray0.clone(), 5), dst);

        Imgproc.threshold(makeMask(gray2.clone(), 10), dst, 1, 255, Imgproc.THRESH_BINARY);
        assertMatEqual(gray255, dst);

        Imgproc.threshold(makeMask(gray2.clone(), 10), dst, 3, 255, Imgproc.THRESH_BINARY_INV);
        assertMatEqual(makeMask(gray255.clone(), 0), dst);
    }

    public void testUndistortMatMatMatMat() {
        CvMat src = new CvMat(3, 3, CvType.CC_32F, new Scalar(3));
        CvMat cameraMatrix = new CvMat(3, 3, CvType.CC_32F) {
            {
                put(0, 0, 1, 0, 1);
                put(1, 0, 0, 1, 2);
                put(2, 0, 0, 0, 1);
            }
        };
        CvMat distCoeffs = new CvMat(1, 4, CvType.CC_32F) {
            {
                put(0, 0, 1, 3, 2, 4);
            }
        };

        Imgproc.undistort(src, dst, cameraMatrix, distCoeffs);

        truth = new CvMat(3, 3, CvType.CC_32F) {
            {
                put(0, 0, 0, 0, 0);
                put(1, 0, 0, 0, 0);
                put(2, 0, 0, 3, 0);
            }
        };
        assertMatEqual(truth, dst, EPS);
    }

    public void testUndistortMatMatMatMatMat() {
        CvMat src = new CvMat(3, 3, CvType.CC_32F, new Scalar(3));
        CvMat cameraMatrix = new CvMat(3, 3, CvType.CC_32F) {
            {
                put(0, 0, 1, 0, 1);
                put(1, 0, 0, 1, 2);
                put(2, 0, 0, 0, 1);
            }
        };
        CvMat distCoeffs = new CvMat(1, 4, CvType.CC_32F) {
            {
                put(0, 0, 2, 1, 4, 5);
            }
        };
        CvMat newCameraMatrix = new CvMat(3, 3, CvType.CC_32F, new Scalar(1));

        Imgproc.undistort(src, dst, cameraMatrix, distCoeffs, newCameraMatrix);

        truth = new CvMat(3, 3, CvType.CC_32F, new Scalar(3));
        assertMatEqual(truth, dst, EPS);
    }

    //undistortPoints(List<Point> src, List<Point> dst, CvMat cameraMatrix, CvMat distCoeffs)
    public void testUndistortPointsListOfPointListOfPointMatMat() {
        MatOfPoint2f src = new MatOfPoint2f(new Point(1, 2), new Point(3, 4), new Point(-1, -1));
        MatOfPoint2f dst = new MatOfPoint2f();
        CvMat cameraMatrix = CvMat.eye(3, 3, CvType.CC_64FC1);
        CvMat distCoeffs = new CvMat(8, 1, CvType.CC_64FC1, new Scalar(0));

        Imgproc.undistortPoints(src, dst, cameraMatrix, distCoeffs);

        assertEquals(src.size(), dst.size());
        for(int i=0; i<src.toList().size(); i++) {
            //Log.d("UndistortPoints", "s="+src.get(i)+", d="+dst.get(i));
            assertTrue(src.toList().get(i).equals(dst.toList().get(i)));
        }
    }


    public void testWarpAffineMatMatMatSize() {
        CvMat src = new CvMat(3, 3, CvType.CC_32F) {
            {
                put(0, 0, 2, 0, 1);
                put(1, 0, 6, 4, 3);
                put(2, 0, 1, 0, 2);
            }
        };
        CvMat M = new CvMat(2, 3, CvType.CC_32F) {
            {
                put(0, 0, 1, 0, 1);
                put(1, 0, 0, 1, 1);
            }
        };

        Imgproc.warpAffine(src, dst, M, new CvSize(3, 3));

        truth = new CvMat(3, 3, CvType.CC_32F) {
            {
                put(0, 0, 0, 0, 0);
                put(1, 0, 0, 2, 0);
                put(2, 0, 0, 6, 4);
            }
        };
        assertMatEqual(truth, dst, EPS);
    }

    public void testWarpAffineMatMatMatSizeInt() {
        CvMat src = new CvMat(3, 3, CvType.CC_32F) {
            {
                put(0, 0, 2, 4, 1);
                put(1, 0, 6, 4, 3);
                put(2, 0, 0, 2, 2);
            }
        };
        CvMat M = new CvMat(2, 3, CvType.CC_32F) {
            {
                put(0, 0, 1, 0, 0);
                put(1, 0, 0, 0, 1);
            }
        };

        Imgproc.warpAffine(src, dst, M, new CvSize(2, 2), Imgproc.WARP_INVERSE_MAP);

        truth = new CvMat(2, 2, CvType.CC_32F) {
            {
                put(0, 0, 6, 4);
                put(1, 0, 6, 4);
            }
        };
        assertMatEqual(truth, dst, EPS);
    }

    public void testWarpAffineMatMatMatSizeIntInt() {
        fail("Not yet implemented");
    }

    public void testWarpAffineMatMatMatSizeIntIntScalar() {
        fail("Not yet implemented");
    }

    public void testWarpPerspectiveMatMatMatSize() {
        CvMat src = new CvMat(3, 3, CvType.CC_32F) {
            {
                put(0, 0, 2, 4, 1);
                put(1, 0, 0, 4, 5);
                put(2, 0, 1, 2, 2);
            }
        };
        CvMat M = new CvMat(3, 3, CvType.CC_32F) {
            {
                put(0, 0, 1, 0, 1);
                put(1, 0, 0, 1, 1);
                put(2, 0, 0, 0, 1);
            }
        };

        Imgproc.warpPerspective(src, dst, M, new CvSize(3, 3));

        truth = new CvMat(3, 3, CvType.CC_32F) {
            {
                put(0, 0, 0, 0, 0);
                put(1, 0, 0, 2, 4);
                put(2, 0, 0, 0, 4);
            }
        };
        assertMatEqual(truth, dst, EPS);
    }

    public void testWarpPerspectiveMatMatMatSizeInt() {
        fail("Not yet implemented");
    }

    public void testWarpPerspectiveMatMatMatSizeIntInt() {
        fail("Not yet implemented");
    }

    public void testWarpPerspectiveMatMatMatSizeIntIntScalar() {
        fail("Not yet implemented");
    }

    public void testWatershed() {
        CvMat image = CvMat.eye(4, 4, CvType.CC_8UC(3));
        CvMat markers = new CvMat(4, 4, CvType.CC_32SC1, new Scalar(0));

        Imgproc.watershed(image, markers);

        truth = new CvMat(4, 4, CvType.CC_32SC1) {
            {
                put(0, 0, -1, -1, -1, -1);
                put(1, 0, -1, 0, 0, -1);
                put(2, 0, -1, 0, 0, -1);
                put(3, 0, -1, -1, -1, -1);
            }
        };
        assertMatEqual(truth, markers);
    }

    public void testGetTextSize() {
        String text = "Android all the way";
        double fontScale = 2;
        int thickness = 3;
        int baseLine[] = new int[1];

        Imgproc.getTextSize(text, Core.FONT_HERSHEY_SCRIPT_SIMPLEX, fontScale, thickness, null);
        CvSize res = Imgproc.getTextSize(text, Core.FONT_HERSHEY_SCRIPT_SIMPLEX, fontScale, thickness, baseLine);

        assertEquals(543.0, res.width);
        assertEquals(44.0, res.height);
        assertEquals(20, baseLine[0]);
    }

    public void testCircleMatPointIntScalar() {
        Point center = new Point(gray0.cols() / 2, gray0.rows() / 2);
        int radius = Math.min(gray0.cols() / 4, gray0.rows() / 4);
        Scalar color = new Scalar(128);

        Imgproc.circle(gray0, center, radius, color);

        assertTrue(0 != Core.countNonZero(gray0));
    }

    public void testCircleMatPointIntScalarInt() {
        Point center = new Point(gray0.cols() / 2, gray0.rows() / 2);
        int radius = Math.min(gray0.cols() / 4, gray0.rows() / 4);
        Scalar color = new Scalar(128);

        Imgproc.circle(gray0, center, radius, color, Core.FILLED);

        assertTrue(0 != Core.countNonZero(gray0));
    }

    public void testCircleMatPointIntScalarIntIntInt() {
        Point center = new Point(gray0.cols() / 2, gray0.rows() / 2);
        Point center2 = new Point(gray0.cols(), gray0.rows());
        int radius = Math.min(gray0.cols() / 4, gray0.rows() / 4);
        Scalar color128 = new Scalar(128);
        Scalar color0 = new Scalar(0);

        Imgproc.circle(gray0, center2, radius * 2, color128, 2, Imgproc.LINE_4, 1/*
                                                                            * Number
                                                                            * of
                                                                            * fractional
                                                                            * bits
                                                                            */);
        assertFalse(0 == Core.countNonZero(gray0));

        Imgproc.circle(gray0, center, radius, color0, 2, Imgproc.LINE_4, 0);

        assertTrue(0 == Core.countNonZero(gray0));
    }

    public void testClipLine() {
        Rect r = new Rect(10, 10, 10, 10);
        Point pt1 = new Point(5.0, 15.0);
        Point pt2 = new Point(25.0, 15.0);

        assertTrue(Imgproc.clipLine(r, pt1, pt2));

        Point pt1Clipped = new Point(10.0, 15.0);
        Point pt2Clipped = new Point(19.0, 15.0);
        assertEquals(pt1Clipped, pt1);
        assertEquals(pt2Clipped, pt2);

        pt1 = new Point(5.0, 5.0);
        pt2 = new Point(25.0, 5.0);
        pt1Clipped = new Point(5.0, 5.0);
        pt2Clipped = new Point(25.0, 5.0);

        assertFalse(Imgproc.clipLine(r, pt1, pt2));

        assertEquals(pt1Clipped, pt1);
        assertEquals(pt2Clipped, pt2);
    }

    public void testEllipse2Poly() {
        Point center = new Point(4, 4);
        CvSize axes = new CvSize(2, 2);
        int angle = 30;
        int arcStart = 30;
        int arcEnd = 60;
        int delta = 2;
        MatOfPoint pts = new MatOfPoint();

        Imgproc.ellipse2Poly(center, axes, angle, arcStart, arcEnd, delta, pts);

        Point truth[] = {
                new Point(5, 6),
                new Point(4, 6)
        };
        assertArrayPointsEquals(truth, pts.toArray(), EPS);
    }

    public void testEllipseMatPointSizeDoubleDoubleDoubleScalar() {
        Point center = new Point(gray0.cols() / 2, gray0.rows() / 2);
        CvSize axes = new CvSize(2, 2);
        double angle = 30, startAngle = 60, endAngle = 90;

        Imgproc.ellipse(gray0, center, axes, angle, startAngle, endAngle, colorWhite);

        assertTrue(0 != Core.countNonZero(gray0));
    }

    public void testEllipseMatPointSizeDoubleDoubleDoubleScalarInt() {
        Point center = new Point(gray0.cols() / 2, gray0.rows() / 2);
        CvSize axes = new CvSize(2, 2);
        double angle = 30, startAngle = 60, endAngle = 90;

        Imgproc.ellipse(gray0, center, axes, angle, startAngle, endAngle, colorWhite, Core.FILLED);

        assertTrue(0 != Core.countNonZero(gray0));
    }

    public void testEllipseMatPointSizeDoubleDoubleDoubleScalarIntIntInt() {
        Point center = new Point(gray0.cols() / 2, gray0.rows() / 2);
        CvSize axes = new CvSize(2, 2);
        Point center2 = new Point(gray0.cols(), gray0.rows());
        CvSize axes2 = new CvSize(4, 4);
        double angle = 30, startAngle = 0, endAngle = 30;

        Imgproc.ellipse(gray0, center, axes, angle, startAngle, endAngle, colorWhite, Core.FILLED, Imgproc.LINE_4, 0);

        assertTrue(0 != Core.countNonZero(gray0));

        Imgproc.ellipse(gray0, center2, axes2, angle, startAngle, endAngle, colorBlack, Core.FILLED, Imgproc.LINE_4, 1);

        assertEquals(0, Core.countNonZero(gray0));
    }

    public void testEllipseMatRotatedRectScalar() {
        int matSize = 10;
        CvMat gray0 = CvMat.zeros(matSize, matSize, CvType.CC_8U);
        Point center = new Point(matSize / 2, matSize / 2);
        CvSize size = new CvSize(matSize / 4, matSize / 2);
        RotatedRect box = new RotatedRect(center, size, 45);

        Imgproc.ellipse(gray0, box, new Scalar(1));

        final byte[] truth = new byte[] {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
                0, 0, 0, 0, 1, 1, 0, 1, 0, 0,
                0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
                0, 0, 0, 1, 0, 1, 1, 0, 0, 0,
                0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

        assertMatEqual(new CvMat(matSize, matSize, CvType.CC_8U) {
            {
                put(0, 0, truth);
            }
        }, gray0);
    }

    public void testEllipseMatRotatedRectScalarInt() {
        Point center = new Point(matSize / 2, matSize / 2);
        CvSize size = new CvSize(matSize / 4, matSize / 2);
        RotatedRect box = new RotatedRect(center, size, 45);

        Imgproc.ellipse(gray0, box, new Scalar(1), Core.FILLED);
        Imgproc.ellipse(gray0, box, new Scalar(0));

        assertTrue(0 < Core.countNonZero(gray0));
    }

    public void testEllipseMatRotatedRectScalarIntInt() {
        Point center = new Point(matSize / 2, matSize / 2);
        CvSize size = new CvSize(2, matSize * 2 / 3);
        RotatedRect box = new RotatedRect(center, size, 20);

        Imgproc.ellipse(gray0, box, new Scalar(9), 1, Imgproc.LINE_AA);
        Imgproc.ellipse(gray0, box, new Scalar(0), 1, Imgproc.LINE_4);

        assertTrue(0 < Core.countNonZero(gray0));
    }

    public void testPolylinesMatListOfListOfPointBooleanScalar() {
        CvMat img = gray0;
        List<MatOfPoint> polyline = new ArrayList<MatOfPoint>();
        polyline.add(new MatOfPoint(new Point(1, 1), new Point(7, 1), new Point(7, 6), new Point(1, 6)));

        Imgproc.polylines(img, polyline, true, new Scalar(100));

        assertEquals(22, Core.countNonZero(img));

        Imgproc.polylines(img, polyline, false, new Scalar(0));

        assertEquals(4, Core.countNonZero(img));
    }

    public void testPolylinesMatListOfListOfPointBooleanScalarInt() {
        CvMat img = gray0;
        List<MatOfPoint> polyline = new ArrayList<MatOfPoint>();
        polyline.add(new MatOfPoint(new Point(1, 1), new Point(7, 1), new Point(7, 6), new Point(1, 6)));

        Imgproc.polylines(img, polyline, true, new Scalar(100), 2);

        assertEquals(62, Core.countNonZero(img));
    }

    public void testPolylinesMatListOfListOfPointBooleanScalarIntIntInt() {
        CvMat img = gray0;
        List<MatOfPoint> polyline1 = new ArrayList<MatOfPoint>();
        polyline1.add(new MatOfPoint(new Point(1, 1), new Point(7, 1), new Point(7, 6), new Point(1, 6)));
        List<MatOfPoint> polyline2 = new ArrayList<MatOfPoint>();
        polyline2.add(new MatOfPoint(new Point(2, 2), new Point(14, 2), new Point(14, 12), new Point(2, 12)));

        Imgproc.polylines(img, polyline1, true, new Scalar(100), 2, Imgproc.LINE_8, 0);

        assertTrue(Core.countNonZero(img) > 0);

        Imgproc.polylines(img, polyline2, true, new Scalar(0), 2, Imgproc.LINE_8, 1);

        assertEquals(0, Core.countNonZero(img));
    }

    public void testPutTextMatStringPointIntDoubleScalar() {
        String text = "Hello World";
        CvSize labelSize = new CvSize(175, 22);
        CvMat img = new CvMat(20 + (int) labelSize.height, 20 + (int) labelSize.width, CvType.CC_8U, colorBlack);
        Point origin = new Point(10, labelSize.height + 10);

        Imgproc.putText(img, text, origin, Core.FONT_HERSHEY_SIMPLEX, 1.0, colorWhite);

        assertTrue(Core.countNonZero(img) > 0);
        // check that border is not corrupted
        Imgproc.rectangle(img, new Point(11, 11), new Point(labelSize.width + 10, labelSize.height + 10), colorBlack, Core.FILLED);
        assertEquals(0, Core.countNonZero(img));
    }

    public void testPutTextMatStringPointIntDoubleScalarInt() {
        String text = "Hello World";
        CvSize labelSize = new CvSize(176, 22);
        CvMat img = new CvMat(20 + (int) labelSize.height, 20 + (int) labelSize.width, CvType.CC_8U, colorBlack);
        Point origin = new Point(10, labelSize.height + 10);

        Imgproc.putText(img, text, origin, Core.FONT_HERSHEY_SIMPLEX, 1.0, colorWhite, 2);

        assertTrue(Core.countNonZero(img) > 0);
        // check that border is not corrupted
        Imgproc.rectangle(img, new Point(10, 10), new Point(labelSize.width + 10 + 1, labelSize.height + 10 + 1), colorBlack, Core.FILLED);
        assertEquals(0, Core.countNonZero(img));
    }

    public void testPutTextMatStringPointIntDoubleScalarIntIntBoolean() {
        String text = "Hello World";
        CvSize labelSize = new CvSize(175, 22);

        CvMat img = new CvMat(20 + (int) labelSize.height, 20 + (int) labelSize.width, CvType.CC_8U, colorBlack);
        Point origin = new Point(10, 10);

        Imgproc.putText(img, text, origin, Core.FONT_HERSHEY_SIMPLEX, 1.0, colorWhite, 1, Imgproc.LINE_8, true);

        assertTrue(Core.countNonZero(img) > 0);
        // check that border is not corrupted
        Imgproc.rectangle(img, new Point(10, 10), new Point(labelSize.width + 9, labelSize.height + 9), colorBlack, Core.FILLED);
        assertEquals(0, Core.countNonZero(img));
    }
}
