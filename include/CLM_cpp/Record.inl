

// Record.cpp : A useful function for quick recording from a webcam for test purposes


#define INFO_STREAM( stream ) \
  std::cout << stream << std::endl

#define WARN_STREAM( stream ) \
  std::cout << "Warning: " << stream << std::endl

#define ERROR_STREAM( stream ) \
  std::cout << "Error: " << stream << std::endl

static void printErrorAndAbort(const std::string& error)
{
  std::cout << error << std::endl;
  abort();
}

#define FATAL_STREAM( stream ) \
  printErrorAndAbort( std::string( "Fatal error: " ) + stream )

using namespace std;
using namespace cv;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime()
{
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  localtime_s(&tstruct, &now);
  // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
  // for more information about date/time format
  strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%M", &tstruct);

  return buf;
}

vector<string> get_arguments(int argc, char** argv)
{

  vector<string> arguments;

  for (int i = 1; i < argc; ++i) {
    arguments.push_back(string(argv[i]));
  }

  return arguments;
}

int main(int argc, char** argv)
{

  vector<string> arguments = get_arguments(argc, argv);

  // Some initial parameters that can be overriden from command line
  string outroot, outfile;

  TCHAR NPath[200];
  GetCurrentDirectory(200, NPath);

  // By default write to same directory
  outroot = NPath;
  outroot = outroot + "/recording/";
  outfile = currentDateTime() + ".avi";

  // By default try webcam
  int device = 0;

  for (size_t i = 0; i < arguments.size(); i++) {
    if (strcmp(arguments[i].c_str(), "--help") == 0 || strcmp(arguments[i].c_str(), "-h") == 0) {
      INFO_STREAM("Usage is [ -r <root dir> | -dev <dev num> ] -of <out file>\n");   // Inform the user of how to use the program
      exit(0);
    }
    else if (strcmp(arguments[i].c_str(), "-dev") == 0) {
      std::stringstream ss;
      ss << arguments[i + 1].c_str();
      ss >> device;
    }
    else if (strcmp(arguments[i].c_str(), "-r") == 0) {
      outroot = arguments[i + 1];
    }
    else if (strcmp(arguments[i].c_str(), "-of") == 0) {
      outroot = arguments[i + 1];
    }
    else {
      WARN_STREAM("invalid argument");
    }

    i++;
  }

  // Do some grabbing
  VideoCapture vCap;
  INFO_STREAM("Attempting to capture from device: " << device);
  vCap = VideoCapture(device);

  if (!vCap.isOpened()) {
    FATAL_STREAM("Failed to open video source");
  }

  Mat img;
  vCap >> img;

  boost::filesystem::path dir(outroot);
  boost::filesystem::create_directory(dir);

  string out_file = outroot + outfile;
  // saving the videos
  VideoWriter video_writer(out_file, CV_FOURCC('D', 'I', 'V', 'X'), 30, img.size(), true);

  ofstream outlog;
  outlog.open((outroot + outfile + ".log").c_str(), ios_base::out);
  outlog << "frame, time(ms)" << endl;

  double freq = getTickFrequency();

  double init_time = getTickCount();

  int frameProc = 0;

  while (!img.empty()) {

    namedWindow("rec", 1);

    vCap >> img;
    double curr_time = (getTickCount() - init_time) / freq;
    curr_time *= 1000;

    video_writer << img;

    outlog << frameProc + 1 << " " << curr_time;
    outlog << endl;


    imshow("rec", img);

    frameProc++;

    // detect key presses
    char c = waitKey(1);

    // quit the application
    if (c == 'q') {
      outlog.close();

      return(0);
    }


  }

  return 0;
}

