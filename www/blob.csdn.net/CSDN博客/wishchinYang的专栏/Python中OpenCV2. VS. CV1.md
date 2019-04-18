# Python中OpenCV2. VS. CV1 - wishchinYang的专栏 - CSDN博客
2014年02月25日 14:18:37[wishchin](https://me.csdn.net/wishchin)阅读数：7181
OpenCV的2.4.7.版本生成了python的CV2模块，可以直接载入；
有兴趣的可以参考这个教程：[http://blog.csdn.net/sunny2038/article/details/9080047](http://blog.csdn.net/sunny2038/article/details/9080047)  不过可惜的是，这个教程只是针对CV2 的；
**Tips1：** 关于两种载入方法的区别：
```python
import numpy as np
import cv2 as cv2
Vs.
import cv2.cv as cv
```
**（一）：引入CV2使用：**
第一种方法的cv函数使用方法几乎等同于matlab，其中可以使用的函数范围也较少；
例如：利用help(cv2)命令；
查看函数列表如下：
```python
ANN_MLP([layerSizes[, activateFunc[, fparam1[, fparam2]]]]) -> <ANN_MLP object>
 Algorithm__create(name) -> retval
 Algorithm_getList() -> algorithms
 BFMatcher([, normType[, crossCheck]]) -> <BFMatcher object>
 BRISK([, thresh[, octaves[, patternScale]]]) -> <BRISK object>  or  BRISK(radiusList, numberList[, dMax[, dMin[, indexChange]]]) -> <BRISK object>
 BackgroundSubtractorMOG([history, nmixtures, backgroundRatio[, noiseSigma]]) -> <BackgroundSubtractorMOG object>
 Boost([trainData, tflag, responses[, varIdx[, sampleIdx[, varType[, missingDataMask[, params]]]]]]) -> <Boost object>
 CamShift(probImage, window, criteria) -> retval, window
 Canny(image, threshold1, threshold2[, edges[, apertureSize[, L2gradient]]]) -> edges
 CascadeClassifier([filename]) -> <CascadeClassifier object>
 DMatch() -> <DMatch object>  or  DMatch(_queryIdx, _trainIdx, _distance) -> <DMatch object>  or  DMatch(_queryIdx, _trainIdx, _imgIdx, _distance) -> <DMatch object>
 DTree() -> <DTree object>
DescriptorExtractor_create(descriptorExtractorType) -> retval
 DescriptorMatcher_create(descriptorMatcherType) -> retval
 EM([, nclusters[, covMatType[, termCrit]]]) -> <EM object>
 ERTrees() -> <ERTrees object>
 FastFeatureDetector([, threshold[, nonmaxSuppression]]) -> <FastFeatureDetector object>
Feature2D_create(name) -> retval
 FeatureDetector_create(detectorType) -> retval
 FileNode() -> <FileNode object>
FileStorage([source, flags[, encoding]]) -> <FileStorage object>
 FlannBasedMatcher([, indexParams[, searchParams]]) -> <FlannBasedMatcher object>
 GBTrees([trainData, tflag, responses[, varIdx[, sampleIdx[, varType[, missingDataMask[, params]]]]]]) -> <GBTrees object>
 GFTTDetector([, maxCorners[, qualityLevel[, minDistance[, blockSize[, useHarrisDetector[, k]]]]]]) -> <GFTTDetector object>
 GaussianBlur(src, ksize, sigmaX[, dst[, sigmaY[, borderType]]]) -> dst
 GridAdaptedFeatureDetector([, detector[, maxTotalKeypoints[, gridRows[, gridCols]]]]) -> <GridAdaptedFeatureDetector object>
 HOGDescriptor() -> <HOGDescriptor object>  or  HOGDescriptor(_winSize, _blockSize, _blockStride, _cellSize, _nbins[, _derivAperture[, _winSigma[, _histogramNormType[, _L2HysThreshold[, _gammaCorrection[, _nlevels]]]]]]) -> <HOGDescriptor object>  or  HOGDescriptor(filename) -> <HOGDescriptor object>
    
 HOGDescriptor_getDaimlerPeopleDetector() -> retval
 HOGDescriptor_getDefaultPeopleDetector() -> retval
 HoughCircles(image, method, dp, minDist[, circles[, param1[, param2[, minRadius[, maxRadius]]]]]) -> circles
 HoughLines(image, rho, theta, threshold[, lines[, srn[, stn]]]) -> lines
 HoughLinesP(image, rho, theta, threshold[, lines[, minLineLength[, maxLineGap]]]) -> lines
 HuMoments(m[, hu]) -> hu
    
KDTree() -> <KDTree object>  or  KDTree(points[, copyAndReorderPoints]) -> <KDTree object>  or  KDTree(points, _labels[, copyAndReorderPoints]) -> <KDTree object> KNearest([trainData, responses[, sampleIdx[, isRegression[, max_k]]]]) -> <KNearest object>
KalmanFilter([dynamParams, measureParams[, controlParams[, type]]]) -> <KalmanFilter object>
KeyPoint([x, y, _size[, _angle[, _response[, _octave[, _class_id]]]]]) -> <KeyPoint object>
LUT(src, lut[, dst[, interpolation]]) -> dst
Laplacian(src, ddepth[, dst[, ksize[, scale[, delta[, borderType]]]]]) -> dst
MSER([, _delta[, _min_area[, _max_area[, _max_variation[, _min_diversity[, _max_evolution[, _area_threshold[, _min_margin[, _edge_blur_size]]]]]]]]]) -> <MSER object>
    
 Mahalanobis(v1, v2, icovar) -> retval
 NormalBayesClassifier([trainData, responses[, varIdx[, sampleIdx]]]) -> <NormalBayesClassifier object>
 ORB([, nfeatures[, scaleFactor[, nlevels[, edgeThreshold[, firstLevel[, WTA_K[, scoreType[, patchSize]]]]]]]]) -> <ORB object>
 PCABackProject(data, mean, eigenvectors[, result]) -> result
 PCACompute(data[, mean[, eigenvectors[, maxComponents]]]) -> mean, eigenvectors
 PCAComputeVar(data, retainedVariance[, mean[, eigenvectors]]) -> mean, eigenvectors
 PCAProject(data, mean, eigenvectors[, result]) -> result
    
 PSNR(src1, src2) -> retval
 PyramidAdaptedFeatureDetector(detector[, maxLevel]) -> <PyramidAdaptedFeatureDetector object>
 RQDecomp3x3(src[, mtxR[, mtxQ[, Qx[, Qy[, Qz]]]]]) -> retval, mtxR, mtxQ, Qx, Qy, Qz
 RTrees() -> <RTrees object>
 Rodrigues(src[, dst[, jacobian]]) -> dst, jacobian
 SIFT([, nfeatures[, nOctaveLayers[, contrastThreshold[, edgeThreshold[, sigma]]]]]) -> <SIFT object>
SURF([hessianThreshold[, nOctaves[, nOctaveLayers[, extended[, upright]]]]]) -> <SURF object>
 SVBackSubst(w, u, vt, rhs[, dst]) -> dst
 SVDecomp(src[, w[, u[, vt[, flags]]]]) -> w, u, vt
SVM([trainData, responses[, varIdx[, sampleIdx[, params]]]]) -> <SVM object>
    
 Scharr(src, ddepth, dx, dy[, dst[, scale[, delta[, borderType]]]]) -> dst
 SimpleBlobDetector([, parameters]) -> <SimpleBlobDetector object>
SimpleBlobDetector_Params() -> <SimpleBlobDetector_Params object>
 Sobel(src, ddepth, dx, dy[, dst[, ksize[, scale[, delta[, borderType]]]]]) -> dst
 StarDetector([, _maxSize[, _responseThreshold[, _lineThresholdProjected[, _lineThresholdBinarized[, _suppressNonmaxSize]]]]]) -> <StarDetector object>
 
StereoBM([preset[, ndisparities[, SADWindowSize]]]) -> <StereoBM object>
StereoSGBM([minDisparity, numDisparities, SADWindowSize[, P1[, P2[, disp12MaxDiff[, preFilterCap[, uniquenessRatio[, speckleWindowSize[, speckleRange[, fullDP]]]]]]]]]) -> <StereoSGBM object>
StereoVar([levels, pyrScale, nIt, minDisp, maxDisp, poly_n, poly_sigma, fi, lambda, penalization, cycle, flags]) -> <StereoVar object>
    
 Subdiv2D([rect]) -> <Subdiv2D object>
 VideoCapture() -> <VideoCapture object>  or  VideoCapture(filename) -> <VideoCapture object>  or  VideoCapture(device) -> <VideoCapture object>
 VideoWriter([filename, fourcc, fps, frameSize[, isColor]]) -> <VideoWriter object>
absdiff(src1, src2[, dst]) -> dst
accumulate(src, dst[, mask]) -> None
accumulateProduct(src1, src2, dst[, mask]) -> None
accumulateSquare(src, dst[, mask]) -> None
accumulateWeighted(src, dst, alpha[, mask]) -> None
adaptiveBilateralFilter(src, ksize, sigmaSpace[, dst[, maxSigmaColor[, anchor[, borderType]]]]) -> dst
adaptiveThreshold(src, maxValue, adaptiveMethod, thresholdType, blockSize, C[, dst]) -> dst
add(src1, src2[, dst[, mask[, dtype]]]) -> dst
addWeighted(src1, alpha, src2, beta, gamma[, dst[, dtype]]) -> dst
applyColorMap(src, colormap[, dst]) -> dst
    
 approxPolyDP(curve, epsilon, closed[, approxCurve]) -> approxCurve
arcLength(curve, closed) -> retval
batchDistance(src1, src2, dtype[, dist[, nidx[, normType[, K[, mask[, update[, crosscheck]]]]]]]) -> dist, nidx
bilateralFilter(src, d, sigmaColor, sigmaSpace[, dst[, borderType]]) -> dst
bitwise_and(src1, src2[, dst[, mask]]) -> dst
bitwise_not(src[, dst[, mask]]) -> dst
bitwise_or(src1, src2[, dst[, mask]]) -> dst
bitwise_xor(src1, src2[, dst[, mask]]) -> dst
blur(src, ksize[, dst[, anchor[, borderType]]]) -> dst
borderInterpolate(p, len, borderType) -> retval
boundingRect(points) -> retval
boxFilter(src, ddepth, ksize[, dst[, anchor[, normalize[, borderType]]]]) -> dst
buildOpticalFlowPyramid(img, winSize, maxLevel[, pyramid[, withDerivatives[, pyrBorder[, derivBorder[, tryReuseInputImage]]]]]) -> retval, pyramid
calcBackProject(images, channels, hist, ranges, scale[, dst]) -> dst
calcCovarMatrix(samples, flags[, covar[, mean[, ctype]]]) -> covar, mean
calcGlobalOrientation(orientation, mask, mhi, timestamp, duration) -> retval
calcHist(images, channels, mask, histSize, ranges[, hist[, accumulate]]) -> hist
calcMotionGradient(mhi, delta1, delta2[, mask[, orientation[, apertureSize]]]) -> mask, orientation
calcOpticalFlowFarneback(prev, next, pyr_scale, levels, winsize, iterations, poly_n, poly_sigma, flags[, flow]) -> flow
calcOpticalFlowPyrLK(prevImg, nextImg, prevPts[, nextPts[, status[, err[, winSize[, maxLevel[, criteria[, flags[, minEigThreshold]]]]]]]]) -> nextPts, status, err
calcOpticalFlowSF(from, to, flow, layers, averaging_block_size, max_flow) -> None  or  calcOpticalFlowSF(from, to, flow, layers, averaging_block_size, max_flow,
sigma_dist, sigma_color, postprocess_window, sigma_dist_fix, sigma_color_fix, occ_thr, upscale_averaging_radius, upscale_sigma_dist, upscale_sigma_color, speed_up_thr) -> None
    
 calibrateCamera(objectPoints, imagePoints, imageSize[, cameraMatrix[, distCoeffs[, rvecs[, tvecs[, flags[, criteria]]]]]]) -> retval, cameraMatrix, distCoeffs, rvecs, tvecs
calibrationMatrixValues(cameraMatrix, imageSize, apertureWidth, apertureHeight) -> fovx, fovy, focalLength, principalPoint, aspectRatio
cartToPolar(x, y[, magnitude[, angle[, angleInDegrees]]]) -> magnitude, angle
chamerMatching(img, templ[, templScale[, maxMatches[, minMatchDistance[, padX[, padY[, scales[, minScale[, maxScale[, orientationWeight[, truncate]]]]]]]]]]) -> retval, results, cost
    
checkHardwareSupport(feature) -> retval
checkRange(a[, quiet[, minVal[, maxVal]]]) -> retval, pos
circle(img, center, radius, color[, thickness[, lineType[, shift]]]) -> None
clipLine(imgRect, pt1, pt2) -> retval, pt1, pt2
compare(src1, src2, cmpop[, dst]) -> dst
compareHist(H1, H2, method) -> retval
completeSymm(mtx[, lowerToUpper]) -> None
composeRT(rvec1, tvec1, rvec2, tvec2[, rvec3[, tvec3[, dr3dr1[, dr3dt1[, dr3dr2[, dr3dt2[, dt3dr1[, dt3dt1[, dt3dr2[, dt3dt2]]]]]]]]]]) -> rvec3, tvec3, dr3dr1, dr3dt1, dr3dr2, dr3dt2, dt3dr1, dt3dt1, dt3dr2, dt3dt2
computeCorrespondEpilines(points, whichImage, F[, lines]) -> lines
contourArea(contour[, oriented]) -> retval
convertMaps(map1, map2, dstmap1type[, dstmap1[, dstmap2[, nninterpolation]]]) -> dstmap1, dstmap2
convertPointsFromHomogeneous(src[, dst]) -> dst
convertPointsToHomogeneous(src[, dst]) -> dst
convertScaleAbs(src[, dst[, alpha[, beta]]]) -> dst
    
convexHull(points[, hull[, clockwise[, returnPoints]]]) -> hull
convexityDefects(contour, convexhull[, convexityDefects]) -> convexityDefects
copyMakeBorder(src, top, bottom, left, right, borderType[, dst[, value]]) -> dst
cornerEigenValsAndVecs(src, blockSize, ksize[, dst[, borderType]]) -> dst
cornerHarris(src, blockSize, ksize, k[, dst[, borderType]]) -> dst
cornerMinEigenVal(src, blockSize[, dst[, ksize[, borderType]]]) -> dst
    
cornerSubPix(image, corners, winSize, zeroZone, criteria) -> None
correctMatches(F, points1, points2[, newPoints1[, newPoints2]]) -> newPoints1, newPoints2
countNonZero(src) -> retval
    
createCLAHE([, clipLimit[, tileGridSize]]) -> retval
createEigenFaceRecognizer([, num_components[, threshold]]) -> retval
createFisherFaceRecognizer([, num_components[, threshold]]) -> retval
createHanningWindow(winSize, type[, dst]) -> dst
createLBPHFaceRecognizer([, radius[, neighbors[, grid_x[, grid_y[, threshold]]]]]) -> retval
    
createTrackbar(trackbarName, windowName, value, count, onChange) -> None
cubeRoot(val) -> retval
cvtColor(src, code[, dst[, dstCn]]) -> dst
dct(src[, dst[, flags]]) -> dst
    
decomposeProjectionMatrix(projMatrix[, cameraMatrix[, rotMatrix[, transVect[, rotMatrixX[, rotMatrixY[, rotMatrixZ[, eulerAngles]]]]]]]) -> cameraMatrix, rotMatrix, transVect, rotMatrixX, rotMatrixY, rotMatrixZ, eulerAngles
    
destroyAllWindows() -> None
destroyWindow(winname) -> None
determinant(mtx) -> retval
    
dft(src[, dst[, flags[, nonzeroRows]]]) -> dst
dilate(src, kernel[, dst[, anchor[, iterations[, borderType[, borderValue]]]]]) -> dst
distanceTransform(src, distanceType, maskSize[, dst]) -> dst
distanceTransformWithLabels(src, distanceType, maskSize[, dst[, labels[, labelType]]]) -> dst, labels
divide(src1, src2[, dst[, scale[, dtype]]]) -> dst  or  divide(scale, src2[, dst[, dtype]]) -> dst
drawChessboardCorners(image, patternSize, corners, patternWasFound) -> None
drawContours(image, contours, contourIdx, color[, thickness[, lineType[, hierarchy[, maxLevel[, offset]]]]]) -> None
    
drawDataMatrixCodes(image, codes, corners) -> None
drawKeypoints(image, keypoints[, outImage[, color[, flags]]]) -> outImage
eigen(src, computeEigenvectors[, eigenvalues[, eigenvectors]]) -> retval, eigenvalues, eigenvectors
ellipse(img, center, axes, angle, startAngle, endAngle, color[, thickness[, lineType[, shift]]]) -> None  or  ellipse(img, box, color[, thickness[, lineType]]) -> None
ellipse2Poly(center, axes, angle, arcStart, arcEnd, delta) -> pts
equalizeHist(src[, dst]) -> dst
erode(src, kernel[, dst[, anchor[, iterations[, borderType[, borderValue]]]]]) -> dst
estimateAffine3D(src, dst[, out[, inliers[, ransacThreshold[, confidence]]]]) -> retval, out, inliers
estimateRigidTransform(src, dst, fullAffine) -> retval
exp(src[, dst]) -> dst
extractChannel(src, coi[, dst]) -> dst
fastAtan2(y, x) -> retval
fastNlMeansDenoising(src[, dst[, h[, templateWindowSize[, searchWindowSize]]]]) -> dst
fastNlMeansDenoisingColored(src[, dst[, h[, hColor[, templateWindowSize[, searchWindowSize]]]]]) -> dst
fastNlMeansDenoisingColoredMulti(srcImgs, imgToDenoiseIndex, temporalWindowSize[, dst[, h[, hColor[, templateWindowSize[, searchWindowSize]]]]]) -> dst
fastNlMeansDenoisingMulti(srcImgs, imgToDenoiseIndex, temporalWindowSize[, dst[, h[, templateWindowSize[, searchWindowSize]]]]) -> dst
fillConvexPoly(img, points, color[, lineType[, shift]]) -> None
fillPoly(img, pts, color[, lineType[, shift[, offset]]]) -> None
    
filter2D(src, ddepth, kernel[, dst[, anchor[, delta[, borderType]]]]) -> dst
filterSpeckles(img, newVal, maxSpeckleSize, maxDiff[, buf]) -> None
findChessboardCorners(image, patternSize[, corners[, flags]]) -> retval, corners
findCirclesGrid(image, patternSize[, centers[, flags[, blobDetector]]]) -> retval, centers
findCirclesGridDefault(image, patternSize[, centers[, flags]]) -> retval, centers
findContours(image, mode, method[, contours[, hierarchy[, offset]]]) -> contours, hierarchy
    
findDataMatrix(image[, corners[, dmtx]]) -> codes, corners, dmtx
findFundamentalMat(points1, points2[, method[, param1[, param2[, mask]]]]) -> retval, mask
findHomography(srcPoints, dstPoints[, method[, ransacReprojThreshold[, mask]]]) -> retval, mask
findNonZero(src[, idx]) -> idx
fitEllipse(points) -> retval
fitLine(points, distType, param, reps, aeps[, line]) -> line
    
flann_Index([features, params[, distType]]) -> <flann_Index object>
flip(src, flipCode[, dst]) -> dst
floodFill(image, mask, seedPoint, newVal[, loDiff[, upDiff[, flags]]]) -> retval, rect
gemm(src1, src2, alpha, src3, gamma[, dst[, flags]]) -> dst
getAffineTransform(src, dst) -> retval
getBuildInformation() -> retval
getCPUTickCount() -> retval
getDefaultNewCameraMatrix(cameraMatrix[, imgsize[, centerPrincipalPoint]]) -> retval
    
getDerivKernels(dx, dy, ksize[, kx[, ky[, normalize[, ktype]]]]) -> kx, ky
getGaborKernel(ksize, sigma, theta, lambd, gamma[, psi[, ktype]]) -> retval
getGaussianKernel(ksize, sigma[, ktype]) -> retval
getNumberOfCPUs() -> retval
getOptimalDFTSize(vecsize) -> retval
getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, alpha[, newImgSize[, centerPrincipalPoint]]) -> retval, validPixROI
getPerspectiveTransform(src, dst) -> retval
getRectSubPix(image, patchSize, center[, patch[, patchType]]) -> patch
    
getRotationMatrix2D(center, angle, scale) -> retval
getStructuringElement(shape, ksize[, anchor]) -> retval
getTextSize(text, fontFace, fontScale, thickness) -> retval, baseLine
getTickCount() -> retval
getTickFrequency() -> retval
getTrackbarPos(trackbarname, winname) -> retval
getValidDisparityROI(roi1, roi2, minDisparity, numberOfDisparities, SADWindowSize) -> retval
getWindowProperty(winname, prop_id) -> retval
    
goodFeaturesToTrack(image, maxCorners, qualityLevel, minDistance[, corners[, mask[, blockSize[, useHarrisDetector[, k]]]]]) -> corners
grabCut(img, mask, rect, bgdModel, fgdModel, iterCount[, mode]) -> None
groupRectangles(rectList, groupThreshold[, eps]) -> rectList, weights
hconcat(src[, dst]) -> dst
    
idct(src[, dst[, flags]]) -> dst
idft(src[, dst[, flags[, nonzeroRows]]]) -> dst
imdecode(buf, flags) -> retval
imencode(ext, img[, params]) -> retval, buf
    
imread(filename[, flags]) -> retval
imshow(winname, mat) -> None
imwrite(filename, img[, params]) -> retval
inRange(src, lowerb, upperb[, dst]) -> dst
initCameraMatrix2D(objectPoints, imagePoints, imageSize[, aspectRatio]) -> retval
initModule_nonfree() -> retval
initUndistortRectifyMap(cameraMatrix, distCoeffs, R, newCameraMatrix, size, m1type[, map1[, map2]]) -> map1, map2
initWideAngleProjMap(cameraMatrix, distCoeffs, imageSize, destImageWidth, m1type[, map1[, map2[, projType[, alpha]]]]) -> retval, map1, map2
    
inpaint(src, inpaintMask, inpaintRadius, flags[, dst]) -> dst
insertChannel(src, dst, coi) -> None
integral(src[, sum[, sdepth]]) -> sum
integral2(src[, sum[, sqsum[, sdepth]]]) -> sum, sqsum
integral3(src[, sum[, sqsum[, tilted[, sdepth]]]]) -> sum, sqsum, tilted
    
intersectConvexConvex(_p1, _p2[, _p12[, handleNested]]) -> retval, _p12
invert(src[, dst[, flags]]) -> retval, dst
invertAffineTransform(M[, iM]) -> iM
isContourConvex(contour) -> retval
    
kmeans(data, K, criteria, attempts, flags[, bestLabels[, centers]]) -> retval, bestLabels, centers
line(img, pt1, pt2, color[, thickness[, lineType[, shift]]]) -> None
log(src[, dst]) -> dst
magnitude(x, y[, magnitude]) -> magnitude
matMulDeriv(A, B[, dABdA[, dABdB]]) -> dABdA, dABdB
    
matchShapes(contour1, contour2, method, parameter) -> retval
matchTemplate(image, templ, method[, result]) -> result
max(src1, src2[, dst]) -> dst
mean(src[, mask]) -> retval
meanShift(probImage, window, criteria) -> retval, window
meanStdDev(src[, mean[, stddev[, mask]]]) -> mean, stddev
medianBlur(src, ksize[, dst]) -> dst
    
merge(mv[, dst]) -> dst
min(src1, src2[, dst]) -> dst
minAreaRect(points) -> retval
minEnclosingCircle(points) -> center, radius
minMaxLoc(src[, mask]) -> minVal, maxVal, minLoc, maxLoc
mixChannels(src, dst, fromTo) -> None
    
moments(array[, binaryImage]) -> retval
morphologyEx(src, op, kernel[, dst[, anchor[, iterations[, borderType[, borderValue]]]]]) -> dst
moveWindow(winname, x, y) -> None
mulSpectrums(a, b, flags[, c[, conjB]]) -> c
mulTransposed(src, aTa[, dst[, delta[, scale[, dtype]]]]) -> dst
multiply(src1, src2[, dst[, scale[, dtype]]]) -> dst
namedWindow(winname[, flags]) -> None
    
norm(src1[, normType[, mask]]) -> retval  or  norm(src1, src2[, normType[, mask]]) -> retval
normalize(src[, dst[, alpha[, beta[, norm_type[, dtype[, mask]]]]]]) -> dst
patchNaNs(a[, val]) -> None
perspectiveTransform(src, m[, dst]) -> dst
phase(x, y[, angle[, angleInDegrees]]) -> angle
phaseCorrelate(src1, src2[, window]) -> retval
phaseCorrelateRes(src1, src2, window) -> retval, response
    
pointPolygonTest(contour, pt, measureDist) -> retval
polarToCart(magnitude, angle[, x[, y[, angleInDegrees]]]) -> x, y
polylines(img, pts, isClosed, color[, thickness[, lineType[, shift]]]) -> None
pow(src, power[, dst]) -> dst
preCornerDetect(src, ksize[, dst[, borderType]]) -> dst
projectPoints(objectPoints, rvec, tvec, cameraMatrix, distCoeffs[, imagePoints[, jacobian[, aspectRatio]]]) -> imagePoints, jacobian
    
putText(img, text, org, fontFace, fontScale, color[, thickness[, lineType[, bottomLeftOrigin]]]) -> None
pyrDown(src[, dst[, dstsize[, borderType]]]) -> dst
pyrMeanShiftFiltering(src, sp, sr[, dst[, maxLevel[, termcrit]]]) -> dst
pyrUp(src[, dst[, dstsize[, borderType]]]) -> dst
randShuffle(dst[, iterFactor]) -> None
randn(dst, mean, stddev) -> None
randu(dst, low, high) -> None
rectangle(img, pt1, pt2, color[, thickness[, lineType[, shift]]]) -> None
    
rectify3Collinear(cameraMatrix1, distCoeffs1, cameraMatrix2, distCoeffs2, cameraMatrix3, distCoeffs3, imgpt1, imgpt3, imageSize, R12, T12, R13, T13, alpha, newImgSize, flags[, R1[, R2[, R3[, P1[, P2[, P3[, Q]]]]]]]) -> retval, R1, R2, R3, P1, P2, P3, Q, roi1, roi2
reduce(src, dim, rtype[, dst[, dtype]]) -> dst
remap(src, map1, map2, interpolation[, dst[, borderMode[, borderValue]]]) -> dst
repeat(src, ny, nx[, dst]) -> dst
reprojectImageTo3D(disparity, Q[, _3dImage[, handleMissingValues[, ddepth]]]) -> _3dImage
    
resize(src, dsize[, dst[, fx[, fy[, interpolation]]]]) -> dst
resizeWindow(winname, width, height) -> None
scaleAdd(src1, alpha, src2[, dst]) -> dst
segmentMotion(mhi, timestamp, segThresh[, segmask]) -> segmask, boundingRects
sepFilter2D(src, ddepth, kernelX, kernelY[, dst[, anchor[, delta[, borderType]]]]) -> dst
setIdentity(mtx[, s]) -> None
setMouseCallback(windowName, onMouse [, param]) -> None
setTrackbarPos(trackbarname, winname, pos) -> None
setUseOptimized(onoff) -> None
setWindowProperty(winname, prop_id, prop_value) -> None
    
solve(src1, src2[, dst[, flags]]) -> retval, dst
solveCubic(coeffs[, roots]) -> retval, roots
solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs[, rvec[, tvec[, useExtrinsicGuess[, flags]]]]) -> retval, rvec, tvec
    
solvePnPRansac(objectPoints, imagePoints, cameraMatrix, distCoeffs[, rvec[, tvec[, useExtrinsicGuess[, iterationsCount[, reprojectionError[, minInliersCount[, inliers[,      flags]]]]]]]]) -> rvec, tvec, inliers
    
solvePoly(coeffs[, roots[, maxIters]]) -> retval, roots
sort(src, flags[, dst]) -> dst
sortIdx(src, flags[, dst]) -> dst
split(m[, mv]) -> mv
sqrt(src[, dst]) -> dst
startWindowThread() -> retval
stereoCalibrate(objectPoints, imagePoints1, imagePoints2, imageSize[, cameraMatrix1[, distCoeffs1[, cameraMatrix2[, distCoeffs2[, R[, T[, E[, F[, criteria[, flags]]]]]]]]]]) -> retval, cameraMatrix1, distCoeffs1, cameraMatrix2, distCoeffs2, R, T, E, F
    
stereoRectify(cameraMatrix1, distCoeffs1, cameraMatrix2, distCoeffs2, imageSize, R, T[, R1[, R2[, P1[, P2[, Q[, flags[, alpha[, newImageSize]]]]]]]]) -> R1, R2, P1, P2, Q, validPixROI1, validPixROI2
    
stereoRectifyUncalibrated(points1, points2, F, imgSize[, H1[, H2[, threshold]]]) -> retval, H1, H2
subtract(src1, src2[, dst[, mask[, dtype]]]) -> dst
sumElems(src) -> retval
threshold(src, thresh, maxval, type[, dst]) -> retval, dst
trace(mtx) -> retval
transform(src, m[, dst]) -> dst
    
transpose(src[, dst]) -> dst
triangulatePoints(projMatr1, projMatr2, projPoints1, projPoints2[, points4D]) -> points4D
undistort(src, cameraMatrix, distCoeffs[, dst[, newCameraMatrix]]) -> dst
undistortPoints(src, cameraMatrix, distCoeffs[, dst[, R[, P]]]) -> dst
updateMotionHistory(silhouette, mhi, timestamp, duration) -> None
    
useOptimized() -> retval
validateDisparity(disparity, cost, minDisparity, numberOfDisparities[, disp12MaxDisp]) -> None
vconcat(src[, dst]) -> dst
    
waitKey([, delay]) -> retval
warpAffine(src, M, dsize[, dst[, flags[, borderMode[, borderValue]]]]) -> dst
warpPerspective(src, M, dsize[, dst[, flags[, borderMode[, borderValue]]]]) -> dst
watershed(image, markers) -> None
```
总结：
对于CV2的使用，其主要类型是mat型，可以直接下标索引：如 Image [idxY] [idxX] = 65552
  其中有一个疑惑的问题是：
对于resize的使用，  Image  =cv.resize(image,(w,h))    此句函数的使用会 导致：自动把Image 转化为3通道；这直接让我没有办法使用cv2了。
**（二）：引入Cv的使用方法：**
引入cv2.cv 后，使用cv便可以找到几乎在cv2refman里面的所有函数了，此处不再列出，举个例子说明。
```python
path ="D:/Develope/EclipseWorks/SLICSeg/Recog/ViewX_2.094395 ViewY_0.000000 ViewZ_6.141592.pcd_comb.pcd_label.txtNo_SLIC.png"
Image      = cv.LoadImageM(path,2)  # 原始图像载入，参数为1 会转化为 三通道，8bit
cv.ShowImage("name", Image)
cv.WaitKey(0)
```
```python
ImageHeight= Image.height  #Image.shape[0]  #Iplimage 没有shape属性！
ImageWidth = Image.width
cv.Size = (Image.width, Image.height)
cv.Size = cv.GetSize(Image)
ImagePatch = cv.CreateImage(cv.Size,16, 1)
cv.Copy(Image, ImagePatch)#copy大小必须一致！
cv.SetImageROI(ImagePatch,(0,0,Image.width/2,Image.height/2))
path ="Patch2/"+ "path_"+ “76”+"_" + "S_posX" +"_Image.png" 
cv.SaveImage(path,ImagePatch)
cv.ShowImage("name", ImagePatch)
cv.WaitKey(0)
ImagePatch = cv.CreateImage((320,240), 16, 1)
cv.Resize(Image,  ImagePatch, cv.CV_INTER_LINEAR)
rows  = Image.height
cols   = Image.width
Mat    = cv.CreateMat(rows, cols, cv.CV_16UC1)
cv.Convert(Image, Mat)
Image[24,25] =0  #对于单通道图像
ImagePatch = cv.CreateImage(cv.Size,8, 3)
ImagePatch[24,25][2] =255#对于三通道图像
```
至此，基本语法讲解告一段落！
