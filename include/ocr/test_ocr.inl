

int test_ocr()
{
	// Try to load the jpeg image
	int width;
	int height;
	int depth;
	vector<uint8_t> original;

	const char* fn = NULL;

	if (!loadJpeg(fn, width, height, depth, original))
		return -1;

	// Create a grayscale image
	vector<uint8_t> grayscale(width * height);

	for (int i = 0; i < width * height; ++i) {
		int sum = 0;

		for (int j = 0; j < depth; ++j)
			sum += static_cast<int>(original[i * depth + j]);

		grayscale[i] = sum / depth;
	}

	// Extract MSER
	clock_t start = clock();

	MSER mser8(2, 0.0005, 0.1, 0.5, 0.5, true);
	MSER mser4(2, 0.0005, 0.1, 0.5, 0.5, false);

	vector<MSER::Region> regions[2];

	mser8(&grayscale[0], width, height, regions[0]);

	// Invert the pixel values
	for (int i = 0; i < width * height; ++i)
		grayscale[i] = ~grayscale[i];

	mser4(&grayscale[0], width, height, regions[1]);

	clock_t stop = clock();

	cout << "Extracted " << (regions[0].size() + regions[1].size()) << " regions from " << argv[1]
		<< " (" << width << 'x' << height << ") in "
		<< (static_cast<double>(stop - start) / CLOCKS_PER_SEC) << "s." << endl;

	// Draw ellipses in the original image
	const uint8_t colors[2][3] = { { 127, 127, 127 },{ 255, 255, 255 } };

	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < regions[i].size(); ++j)
			drawEllipse(regions[i][j], width, height, depth, original, colors[i]);

	// Save the original image
	saveJpeg(argv[2], width, height, depth, original);
	return 0;
}
