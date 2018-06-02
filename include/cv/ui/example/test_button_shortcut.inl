/*
This is a demo application to showcase keyboard shortcuts. 
Author: Pascal Thomet
*/


int test_button_shortcut(int argc, const char *argv[])
{
const char* WINDOW_NAME = "Button shortcut";
  img_t frame[1] = {0};
  cvSetMat(frame, 150, 650, CC_8U, 3);

	// Init cvui and tell it to use a value of 20 for cwaitKey()
	// because we want to enable keyboard shortcut for
	// all components, e.g. button with label "&Quit".
	// If cvui has a value for waitKey, it will call
	// waitKey() automatically for us within cvui_update().
	cvui_init(WINDOW_NAME, 20);

	while (true) {
    //cvSet(frame, cScalar(49, 52, 49));

		cvui_text(frame, 40, 40, "To exit this app click the button below or press Q (shortcut for the button below).");

		// Exit the application if the quit button was pressed.
		// It can be pressed because of a mouse click or because 
		// the user pressed the "q" key on the keyboard, which is
		// marked as a shortcut in the button label ("&Quit").
		if (cvui_button(frame, 300, 80, "&Quit")) {
			break;
		}

		// Since cvui_init() received a param regarding waitKey,
		// there is no need to call cwaitKey() anymore. cvui_update()
		// will do it automatically.
		cvui_update();
		
		imshow2(WINDOW_NAME, frame);
	}

	return 0;
}
