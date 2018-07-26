
typedef _jsfeat::matrix_t matrix_t;
typedef _jsfeat::data_t data_t;

struct _objectdetect {
    /**
		 * Converts from a 4-channel RGBA source image to a 1-channel grayscale
		 * image. Corresponds to the CV_RGB2GRAY OpenCV color space conversion.
		 * 
		 * @param {Array} src   4-channel 8-bit source image
		 * @param {Array} [dst] 1-channel 32-bit destination image
		 * 
		 * @return {Array} 1-channel 32-bit destination image
		 */
		void convertRgbaToGrayscale(int srcLength, const uchar* src, uchar* dst) {
			//int srcLength = src.size;
			//if (!dst) dst = new Uint32Array(srcLength >> 2);
			
			for (int i = 0; i < srcLength; i += 4) {
				dst[i >> 2] = (src[i] * 4899 + src[i + 1] * 9617 + src[i + 2] * 1868 + 8192) >> 14;
			}
		}

		/**
		 * Reduces the size of a given image by the given factor. Does NOT 
		 * perform interpolation. If interpolation is required, prefer using
		 * the drawImage() method of the <canvas> element.
		 * 
		 * @param {Array}  src       1-channel source image
		 * @param {Number} srcWidth	 Width of the source image
		 * @param {Number} srcHeight Height of the source image
		 * @param {Number} factor    Scaling down factor (> 1.0)
		 * @param {Array}  [dst]     1-channel destination image
		 * 
		 * @return {Array} 1-channel destination image
		 */
		void rescaleImage(const matrix_t& src, int srcWidth, int srcHeight, double factor, matrix_t& dst) {
			int dstWidth = (int)(srcWidth / factor),
				dstHeight = (int)(srcHeight / factor);
			dst.resize(dstWidth, dstHeight, 1, CC_8U);
			jsfeat.imgproc.resample(src, dst, dstWidth, dstHeight);
#if 0
			int srcLength = srcHeight * srcWidth;
			
			dst.resize(dstWidth, srcHeight);
			
			for (int x = 0; x < dstWidth; ++x) {
				int dstIndex = x;
				for (int srcIndex = (int)(x * factor), srcEnd = srcIndex + srcLength; srcIndex < srcEnd; srcIndex += srcWidth) {
					dst[dstIndex] = src[srcIndex];
					dstIndex += dstWidth;
				}
			}
			
			int dstIndex = 0;
			for (double y = 0, yEnd = dstHeight * factor; y < yEnd; y += factor) {
				for (int srcIndex = ((int)y)* dstWidth, srcEnd=srcIndex + dstWidth; srcIndex < srcEnd; ++srcIndex) {
					dst[dstIndex] = dst[srcIndex];
					++dstIndex;
				}
			}
#endif
		}

#if 0
		/**
		 * Horizontally mirrors a 1-channel source image.
		 * 
		 * @param {Array}  src       1-channel source image
		 * @param {Number} srcWidth  Width of the source image
		 * @param {Number} srcHeight Height of the source image
		 * @param {Array} [dst]      1-channel destination image
		 * 
		 * @return {Array} 1-channel destination image
		 */
		void mirrorImage(src, srcWidth, srcHeight, dst) {
			if (!dst) dst = new src.constructor(src.length);
			
			var index = 0;
			for (var y = 0; y < srcHeight; ++y) {
				for (var x = (srcWidth >> 1); x >= 0; --x) {
					var swap = src[index + x];
					dst[index + x] = src[index + srcWidth - 1 - x];
					dst[index + srcWidth - 1 - x] = swap;
				}
				index += srcWidth;
			}
			return dst;
		}

		/**
		 * Computes the gradient magnitude using a sobel filter after
		 * applying gaussian smoothing (5x5 filter size). Useful for canny
		 * pruning.
		 * 
		 * @param {Array}  src      1-channel source image
		 * @param {Number} srcWidth Width of the source image
		 * @param {Number} srcWidth Height of the source image
		 * @param {Array}  [dst]    1-channel destination image
		 * 
		 * @return {Array} 1-channel destination image
		 */
		computeCanny = function(src, srcWidth, srcHeight, dst) {
			var srcLength = src.length;
			if (!dst) dst = new src.constructor(srcLength);
			var buffer1 = dst === src ? new src.constructor(srcLength) : dst;
			var buffer2 = new src.constructor(srcLength);
			
			// Gaussian filter with size=5, sigma=sqrt(2) horizontal pass:
			for (var x = 2; x < srcWidth - 2; ++x) {
				var index = x;
				for (var y = 0; y < srcHeight; ++y) {
					buffer1[index] =
						0.1117 * src[index - 2] +
						0.2365 * src[index - 1] +
						0.3036 * src[index] +
						0.2365 * src[index + 1] +
						0.1117 * src[index + 2];
					index += srcWidth;
				}
			}
			
			// Gaussian filter with size=5, sigma=sqrt(2) vertical pass:
			for (var x = 0; x < srcWidth; ++x) {
				var index = x + srcWidth;
				for (var y = 2; y < srcHeight - 2; ++y) {
					index += srcWidth;
					buffer2[index] =
						0.1117 * buffer1[index - srcWidth - srcWidth] +
						0.2365 * buffer1[index - srcWidth] +
						0.3036 * buffer1[index] +
						0.2365 * buffer1[index + srcWidth] +
						0.1117 * buffer1[index + srcWidth + srcWidth];
				}
			}
			
			// Compute gradient:
			var abs = Math.abs;
			for (var x = 2; x < srcWidth - 2; ++x) {
				var index = x + srcWidth;
				for (var y = 2; y < srcHeight - 2; ++y) {
					index += srcWidth;
					
					dst[index] = 
						abs(-     buffer2[index - 1 - srcWidth]
							+     buffer2[index + 1 - srcWidth]
							- 2 * buffer2[index - 1]
							+ 2 * buffer2[index + 1]
							-     buffer2[index - 1 + srcWidth]
							+     buffer2[index + 1 + srcWidth]) +
						
						abs(      buffer2[index - 1 - srcWidth]
							+ 2 * buffer2[index - srcWidth]
							+     buffer2[index + 1 - srcWidth]
							-     buffer2[index - 1 + srcWidth]
							- 2 * buffer2[index + srcWidth]
							-     buffer2[index + 1 + srcWidth]);
				}
			}
			return dst;
		}
#endif

		/**
		 * Computes the integral image of a 1-channel image. Arithmetic
		 * overflow may occur if the integral exceeds the limits for the
		 * destination image values ([0, 2^32-1] for an unsigned 32-bit image).
		 * The integral image is 1 pixel wider both in vertical and horizontal
		 * dimension compared to the source image.
		 * 
		 * SAT = Summed Area Table.
		 * 
		 * @param {Array}       src       1-channel source image
		 * @param {Number}      srcWidth  Width of the source image
		 * @param {Number}      srcHeight Height of the source image
		 * @param {Uint32Array} [dst]     1-channel destination image
		 * 
		 * @return {Uint32Array} 1-channel destination image
		 */
		matrix_t&  computeSat(matrix_t& src, int srcWidth, int srcHeight, matrix_t& _dst) {
			int dstWidth = srcWidth + 1;
			_dst.resize(dstWidth, srcHeight + 1, 1, CC_32U);
			uint32* dst = _dst.u32;
			
			for (int i = srcHeight * dstWidth; i >= 0; i -= dstWidth)
				dst[i] = 0;
			
			for (int x = 1; x <= srcWidth; ++x) {
				int column_sum = 0;
				int index = x;
				dst[x] = 0;
				
				for (int y = 1; y <= srcHeight; ++y) {
					column_sum += src.u8[index - y];
					index += dstWidth;
					dst[index] = dst[index - 1] + column_sum;
				}
			}
			return _dst;
		}
		
		/**
		 * Computes the squared integral image of a 1-channel image.
		 * @see computeSat()
		 * 
		 * @param {Array}       src       1-channel source image
		 * @param {Number}      srcWidth  Width of the source image
		 * @param {Number}      srcHeight Height of the source image
		 * @param {Uint32Array} [dst]     1-channel destination image
		 * 
		 * @return {Uint32Array} 1-channel destination image
		 */
		matrix_t& computeSquaredSat(const matrix_t& src, int srcWidth, int srcHeight, matrix_t& _dst) {
			int dstWidth = srcWidth + 1;
			_dst.resize(srcWidth + 1, srcHeight + 1, 1, CC_32U);
			uint32* dst = _dst.u32;
			int i, x, y;
			
			for (i = srcHeight * dstWidth; i >= 0; i -= dstWidth)
				dst[i] = 0;
			
			for (x = 1; x <= srcWidth; ++x) {
				int column_sum = 0;
				int index = x;
				dst[x] = 0;
				for (y = 1; y <= srcHeight; ++y) {
					int val = src.u8[index - y];
					column_sum += val * val;
					index += dstWidth;
					dst[index] = dst[index - 1] + column_sum;
				}
			}
			return _dst;
		}

		/**
		 * Computes the rotated / tilted integral image of a 1-channel image.
		 * @see computeSat()
		 * 
		 * @param {Array}       src       1-channel source image
		 * @param {Number}      srcWidth  Width of the source image
		 * @param {Number}      srcHeight Height of the source image
		 * @param {Uint32Array} [dst]     1-channel destination image
		 * 
		 * @return {Uint32Array} 1-channel destination image
		 */
		matrix_t& computeRsat(const matrix_t& src, int srcWidth, int srcHeight, matrix_t& _dst) {
			int dstWidth = srcWidth + 1, srcHeightTimesDstWidth = srcHeight * dstWidth;
			int i, x, y;
			_dst.resize(srcWidth + 1, srcHeight + 1, 1, CC_32U);
			uint32* dst = _dst.u32;

			for (i = srcHeightTimesDstWidth; i >= 0; i -= dstWidth)
				dst[i] = 0;
			
			for (i = dstWidth - 1; i >= 0; --i)
				dst[i] = 0;
			
			int index = 0;
			for (y = 0; y < srcHeight; ++y) {
				for (x = 0; x < srcWidth; ++x) {
					dst[index + dstWidth + 1] = src.u8[index - y] + dst[index];
					++index;
				}
				dst[index + dstWidth] += dst[index];
				index++;
			}
			
			for (x = srcWidth - 1; x > 0; --x) {
				int index = x + srcHeightTimesDstWidth;
				for (y = srcHeight; y > 0; --y) {
					index -= dstWidth;
					dst[index + dstWidth] += dst[index] + dst[index + 1];
				}
			}
			
			return _dst;
		}

		/**
		 * Equalizes the histogram of an unsigned 1-channel image with integer 
		 * values in [0, 255]. Corresponds to the equalizeHist OpenCV function.
		 * 
		 * @param {Array}  src   1-channel integer source image
		 * @param {Number} step  Sampling stepsize, increase for performance
		 * @param {Array}  [dst] 1-channel destination image
		 * 
		 * @return {Array} 1-channel destination image
		 */
		matrix_t& equalizeHistogram(const matrix_t& src, int step, matrix_t& _dst) {
			int srcLength = src.length();
			_dst.resize(src.w, src.h, 1, src.tid);
			uint8* dst = _dst.u8;
			int i;
			if (step<1) step = 5;
			
			// Compute histogram and histogram sum:
			int hist[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0 };
			
			for (i = 0; i < srcLength; i += step) {
				++hist[src.u8[i]];
			}
			
			// Compute integral histogram:
			int norm = 255 * step / srcLength, prev = 0;
			for (i = 0; i < 256; ++i) {
				int h = hist[i];
				prev = h += prev;
				hist[i] = h * norm; // For non-integer src: ~~(h * norm + 0.5);
			}
			
			// Equalize image:
			for (i = 0; i < srcLength; ++i) {
				dst[i] = hist[src.u8[i]];
			}
			return _dst;
		}

		/**
		 * Horizontally mirrors a cascase classifier. Useful to detect mirrored
		 * objects such as opposite hands.
		 * 
		 * @param {Array} dst Cascade classifier
		 * 
		 * @return {Array} Mirrored cascade classifier
		 */
#if 0
		matrix_t& mirrorClassifier(const int32* src, matrix_t& dst) {
			_dst.resize(src.w, src.h, 1, src.tid);
			int windowWidth  = src[0];
			
			for (var i = 1, iEnd = src.length - 1; i < iEnd; ) {
				++i;
				for (var j = 0, jEnd = src[++i]; j < jEnd; ++j) {
					if (src[++i]) {		
						// Simple classifier is tilted:
						for (var kEnd = i + src[++i] * 5; i < kEnd; ) {
							dst[i + 1] = windowWidth - src[i + 1];
							var width = src[i + 3];
							dst[i + 3] = src[i + 4];
							dst[i + 4] = width;
							i += 5;
						}
					} else {
						// Simple classifier is not tilted:
						for (var kEnd = i + src[++i] * 5; i < kEnd; ) {
							dst[i + 1] = windowWidth - src[i + 1] - src[i + 3];
							i += 5;
						}
					}
					i += 3;
				}
			}
			return dst;
		}
#endif
		/**
		 * Compiles a cascade classifier to be applicable to images
		 * of given dimensions. Speeds-up the actual detection process later on.
		 * 
		 * @param {Array}        src    Cascade classifier
		 * @param {Number}       width  Width of the source image
		 * @param {Number}       height Height of the source image
		 * @param {Float32Array} [dst]  Compiled cascade classifier
		 * 
		 * @return {Float32Array} Compiled cascade classifier
		 */
		int compileClassifier(const float* _src, int _src_length, int width, int height, float* dst) {
			width += 1;
			height += 1;
			const float* src = _src;
			int32* dstUint32 = (int32*)dst;
			
			dstUint32[0] = src[0];
			dstUint32[1] = src[1];
			int dstIndex = 1;
			for (int srcIndex = 1, iEnd = _src_length - 1; srcIndex < iEnd; ) {
				dst[++dstIndex] = src[++srcIndex];
				
				int numComplexClassifiers = dstUint32[++dstIndex] = src[++srcIndex];
				for (int j = 0, jEnd = numComplexClassifiers; j < jEnd; ++j) {
					int tilted = dst[++dstIndex] = src[++srcIndex];
					int numFeaturesTimes2 = dstUint32[++dstIndex] = src[++srcIndex] * 3;
					if (tilted) {
						for (int kEnd = dstIndex + numFeaturesTimes2; dstIndex < kEnd; ) {						
							int featureOffset = src[srcIndex + 1] + src[srcIndex + 2] * width,
								featureWidthTimesWidth = src[srcIndex + 3] * (width + 1),
								featureHeightTimesWidth = src[srcIndex + 4] * (width - 1);
							
							dstUint32[++dstIndex] = featureOffset;
							dstUint32[++dstIndex] = featureWidthTimesWidth + (featureHeightTimesWidth << 16);
							
							dst[++dstIndex] = src[srcIndex + 5];
							srcIndex += 5;
						}
					} else {
						for (int kEnd = dstIndex + numFeaturesTimes2; dstIndex < kEnd; ) {
							int featureOffset = src[srcIndex + 1] + src[srcIndex + 2] * width,
								featureWidth = src[srcIndex + 3],
								featureHeightTimesWidth = src[srcIndex + 4] * width;

							dstUint32[++dstIndex] = featureOffset;
							dstUint32[++dstIndex] = featureWidth + (featureHeightTimesWidth << 16);
							dst[++dstIndex] = src[srcIndex + 5];
							srcIndex += 5;
						}
					}
					int classifierThreshold = src[++srcIndex];
					for (int k = 0; k < numFeaturesTimes2;) {
						dst[dstIndex - k] /= classifierThreshold;
						k += 3;
					}

					if ((classifierThreshold < 0)) {
						dst[dstIndex + 2] = src[++srcIndex];
						dst[dstIndex + 1] = src[++srcIndex];
						dstIndex += 2;
					} else {
						dst[++dstIndex] = src[++srcIndex];
						dst[++dstIndex] = src[++srcIndex];
					}
				}
			}
			//dstData.size = (dstIndex + 1) * sizeof(float);
			return dstIndex + 1;
		}
		
		/**
		 * Evaluates a compiled cascade classifier. Sliding window approach.
		 * 
		 * @param {Uint32Array}  sat        SAT of the source image
		 * @param {Uint32Array}  rsat       Rotated SAT of the source image
		 * @param {Uint32Array}  ssat       Squared SAT of the source image
		 * @param {Uint32Array}  [cannySat] SAT of the canny source image
		 * @param {Number}       width      Width of the source image
		 * @param {Number}       height     Height of the source image
		 * @param {Number}       step       Stepsize, increase for performance
		 * @param {Float32Array} classifier Compiled cascade classifier
		 * 
		 * @return {Array} Rectangles representing detected objects
		 */
		int detect_(const uint32* sat, const uint32* rsat, const uint32* ssat, const uint32* cannySat, int width, int height, int step, const float* classifier, int classifier_length, int* rects, int len, int maxlen) {
			width  += 1;
			height += 1;

			//int classifier_length = _classifier.size >> 2;
			//const float* classifier = (_classifier.f32);
			const int32* classifierUint32 = (const int32*)(classifier);
			int windowWidth = classifierUint32[0],
				windowHeight = classifierUint32[1],
				windowHeightTimesWidth = windowHeight * width,
				area = windowWidth * windowHeight,
				widthTimesStep = width * step;
			double inverseArea = 1 / area;
			for (int x = 0; x + windowWidth < width && len<maxlen; x += step) {
				int satIndex = x;
				for (int y = 0; y + windowHeight < height && len<maxlen; y += step) {
					int satIndex1 = satIndex + windowWidth,
						satIndex2 = satIndex + windowHeightTimesWidth,
						satIndex3 = satIndex2 + windowWidth,
						canny = false;

					// Canny test:
					if (cannySat) {
						double edgesDensity = (cannySat[satIndex] -
											cannySat[satIndex1] -
											cannySat[satIndex2] +
											cannySat[satIndex3]) * inverseArea;
						if (edgesDensity < 60 || edgesDensity > 200) {
							canny = true;
							satIndex += widthTimesStep;
							continue;
						}
					}
					
					// Normalize mean and variance of window area:
					double mean = (sat[satIndex] -
							    sat[satIndex1] -
						        sat[satIndex2] +
						        sat[satIndex3]),
						
						variance = (ssat[satIndex] -
						            ssat[satIndex1] -
						            ssat[satIndex2] +
						            ssat[satIndex3]) * area - mean * mean,
						            
						std = variance > 1 ? Math.sqrt(variance) : 1,
						found = true;
					
					// Evaluate cascade classifier aka 'stages':
					for (int i = 1, iEnd = classifier_length - 1; i < iEnd; ) {
						float complexClassifierThreshold = classifier[++i];
						// Evaluate complex classifiers aka 'trees':
						double complexClassifierSum = 0;
						for (int j = 0, jEnd = classifierUint32[++i]; j < jEnd; ++j) {
							// Evaluate simple classifiers aka 'nodes':
							double simpleClassifierSum = 0;

							if (classifierUint32[++i]) {
								// Simple classifier is tilted:
								for (int kEnd = i + classifierUint32[++i]; i < kEnd; ) {
									int f1 = satIndex + classifierUint32[++i],
										packed = classifierUint32[++i],
										f2 = f1 + (packed & 0xFFFF),
										f3 = f1 + (packed >> 16 & 0xFFFF);
									
									simpleClassifierSum += classifier[++i] *
										(rsat[f1] - rsat[f2] - rsat[f3] + rsat[f2 + f3 - f1]);
								}
							} else {
								// Simple classifier is not tilted:
								for (int kEnd = i + classifierUint32[++i]; i < kEnd; ) {
									int f1 = satIndex + classifierUint32[++i],
										packed = classifierUint32[++i],
										f2 = f1 + (packed & 0xFFFF),
										f3 = f1 + (packed >> 16 & 0xFFFF);
									
									simpleClassifierSum += classifier[++i] *
										(sat[f1] - sat[f2] - sat[f3] + sat[f2 + f3 - f1]);
								}
							}
							complexClassifierSum += classifier[i + (simpleClassifierSum > std ? 2 : 1)];
							i += 2;
						}
						if (complexClassifierSum < complexClassifierThreshold) {
							found = false;
							break;
						}
					}
					if (found && len<maxlen) {
						int* r = rects + 4 * len;
						r[0] = x;
						r[1] = y;
						r[2] = windowWidth;
						r[3] = windowHeight;
						++len;
					}
					satIndex += widthTimesStep;
				}
			}
			return len;
		}

		/**
		 * Groups rectangles together using a rectilinear distance metric. For
		 * each group of related rectangles, a representative mean rectangle
		 * is returned.
		 * 
		 * @param {Array}  rects        Rectangles (Arrays of 4 floats)
		 * @param {Number} minNeighbors
		 *  
		 * @return {Array} Mean rectangles (Arrays of 4 floats)
		 */
		int groupRectangles(const int* rects, int rectsLength, int minNeighbors, double confluence, int* filteredGroups) {
			if (confluence<1) confluence = 1.0;
			
	    	// Partition rects into similarity classes:
	    	int i, j, numClasses = 0;
			int* labels = NULL;
			MYREALLOC(labels, rectsLength);
			for (i = 0; i < rectsLength; ++i) {
				labels[i] = 0;
			}
			
			for (i = 0; i < rectsLength; ++i) {
				int found = false;
				for (j = 0; j < i; ++j) {
					
					// Determine similarity:
					const int* rect1 = rects + 4 * i;
					const int* rect2 = rects + 4 * j;
			        double delta = confluence * (Math_min(rect1[2], rect2[2]) + Math_min(rect1[3], rect2[3]));
			        if (Math.abs(rect1[0] - rect2[0]) <= delta &&
			        	Math.abs(rect1[1] - rect2[1]) <= delta &&
			        	Math.abs(rect1[0] + rect1[2] - rect2[0] - rect2[2]) <= delta &&
			        	Math.abs(rect1[1] + rect1[3] - rect2[1] - rect2[3]) <= delta) {
						
						labels[i] = labels[j];
						found = true;
						break;
					}
				}
				if (!found) {
					labels[i] = numClasses++;
				}
			}
			
			// Compute average rectangle (group) for each cluster:
			int* groups = NULL;
			MYREALLOC(groups, 5 * numClasses);

			for (i = 0; i < 5*numClasses; ++i) {
				groups[i] = 0;
			}
			
			for (i = 0; i < rectsLength; ++i) {
				const int* rect = rects + 4 * i;
				int* group = groups + 5*labels[i];
				group[0] += rect[0];
				group[1] += rect[1];
				group[2] += rect[2];
				group[3] += rect[3];
				++group[4];
			}

			int numGroups = 0;
			for (i = 0; i < numClasses; ++i) {
				int* group = groups + 5 * i;
				int numNeighbors = group[4];
				if (numNeighbors >= minNeighbors) {
					int* out = groups + numGroups *5;
					numGroups++;
					out[0] = group[0] / numNeighbors;
					out[1] = group[1] / numNeighbors;
					out[2] = group[2] / numNeighbors;
					out[3] = group[3] / numNeighbors;
					out[4] = numNeighbors;
				}
			}
			numClasses = numGroups;
			numGroups = 0;
			// Filter out small rectangles inside larger rectangles:
			for (i = 0; i < numClasses; ++i) {
		        int* r1 = groups+ 5*i;
		        
		        for (j = 0; j < numClasses; ++j) {
		        	if (i == j) continue;
		            int* r2 = groups + 5*j;
		            int dx = r2[2] * 0.2;
					int dy = r2[3] * 0.2;
		            
		            if (r1[0] >= r2[0] - dx &&
		                r1[1] >= r2[1] - dy &&
		                r1[0] + r1[2] <= r2[0] + r2[2] + dx &&
		                r1[1] + r1[3] <= r2[1] + r2[3] + dy) {
		            	break;
		            }
		        }
		        
		        if (j == numClasses) {
					int* out = filteredGroups + 4*numGroups;;
					out[0] = r1[0];
					out[1] = r1[1];
					out[2] = r1[2];
					out[4] = r1[3];
		        }
		    }
			FREE(labels);
			FREE(groups);
			return numGroups;
		}


		/**
		 * Creates a new detector - basically a convenient wrapper class around
		 * the js-objectdetect functions and hides away the technical details
		 * of multi-scale object detection on image, video or canvas elements.
		 * 
		 * @param width       Width of the detector
		 * @param height      Height of the detector
		 * @param scaleFactor Scaling factor for multi-scale detection
		 * @param classifier  Compiled cascade classifier
		 */

		/**
		 * Multi-scale object detection on image, video or canvas elements. 
		 * 
		 * @param image          HTML image, video or canvas element
		 * @param [group]        Detection results will be grouped by proximity
		 * @param [stepSize]     Increase for performance
		 * @param [Roi]          Region of interest, i.e. search window
		 * 
		 * @return Grouped rectangles
		 */
		static int cmpfunc(const void * a, const void * b)
		{
			return (((int*)a)[2] - ((int*)b)[2]);
		}
		int detect(const matrix_t& image, int group, int stepSize, double scaleFactor, const float* classifier, int classifier_length, int* rects, int maxlen) {
			if (!stepSize) stepSize = 1;
			int len = 0;
			matrix_t sat;
			matrix_t rsat;
			matrix_t ssat;
			matrix_t scaledGray;
			int width = image.width;
			int height = image.height;
			int i, j;
			float* compiledClassifiers = NULL;
			--classifier_length;
			int tilted = (int)*classifier++;
			int numScales = (int)(Math.log(Math_min(width / classifier[0], height / classifier[1])) / Math.log(scaleFactor));
			MYREALLOC(compiledClassifiers, classifier_length);
			//this->scaledGray.resize(width, height, 1, CC_8U);
			//drawImage(image, this->context, 0, 0, width, height);

			double scale = 1;
			for (i = 0; i < numScales; ++i) {
				int scaledWidth = (int)(width / scale);
				int scaledHeight = (int)(height / scale);
				int compiledClassifiersLens;
				compiledClassifiersLens  = compileClassifier(classifier, classifier_length, scaledWidth, scaledHeight, compiledClassifiers);
				rescaleImage(image, width, height, scale, scaledGray);
				computeSat(scaledGray, scaledWidth, scaledHeight, sat);
				computeSquaredSat(scaledGray, scaledWidth, scaledHeight, ssat);
				if (tilted) computeRsat(scaledGray, scaledWidth, scaledHeight, rsat);
				//this->cannysat = NULL;
				uint32* cannysat = NULL;
				j = len;
				len = detect_(sat.u32, rsat.u32, ssat.u32, cannysat, scaledWidth, scaledHeight, stepSize, compiledClassifiers, compiledClassifiersLens, rects, len, maxlen);
				for (; j < len; ++j) {
					int* r = rects + 4 * j;
					r[0] *= width / scaledWidth;
					r[1] *= height / scaledHeight;
					r[2] *= width / scaledWidth;
					r[3] *= height / scaledHeight;
				}

				scale *= scaleFactor;
			}
			FREE(compiledClassifiers);
			len = (group ? groupRectangles(rects, len, group, 0, rects) : len);
			//sort(function(r1, r2) { return r2[4] - r1[4]; });
			qsort(rects, len, 4*sizeof(int), cmpfunc);
			return len;
		}
};
