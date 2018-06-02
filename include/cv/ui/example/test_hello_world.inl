
#include "cv/cv.inl"

int test_hello_world(int argc, const char *argv[])
{
const char* WINDOW_NAME = "CVUI Hello World!";
	int count = 0;
  img_t frame[1] = {0};
  cvSetMat(frame, 200, 500, CC_8U, 3);

	// Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
	cvui_init(WINDOW_NAME);

	while (true) {
		// Fill the frame with a nice color
		cvSet(frame, cScalar(49, 52, 49));

		// Buttons will return true if they were clicked, which makes
		// handling clicks a breeze.
		if (cvui_button(frame, 110, 80, "Hello, world!")) {
			// The button was clicked, so let's increment our counter.
			count++;
		}

		// Sometimes you want to show text that is not that simple, e.g. strings + numbers.
		// You can use cvui_printf for that. It accepts a variable number of parameter, pretty
		// much like printf does.
		// Let's show how many times the button has been clicked.
		cvui_printf(frame, 250, 90, 0.4, 0xff0000, "Button click count: %d", count);

		// Update cvui stuff and show everything on the screen
		cvui_imshow(WINDOW_NAME, frame);

		// Check if ESC key was pressed
		if (waitKey(20) == 27) {
			break;
		}
	}

	return 0;
}