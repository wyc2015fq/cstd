
#include <string>

struct FileStorage : public CFileStorage {
  typedef FileStorage self;
  self& operator <<(const char* s) {
    return *this;
  }
  self& operator <<(int i) {
    return *this;
  }
  self& operator <<(double d) {
    return *this;
  }
  self& operator <<(const std::string& d) {
    return *this;
  }
};

struct FileNode : public CFileNode {
  typedef FileNode self;
  typedef const FileNode cself;
  cself& get(const char* name, int& x) const {
    return *this;
  }
  cself& get(const char* name, double& x) const {
    return *this;
  }
  cself& get(const char* name, float& x) const {
    return *this;
  }
  cself& get(const char* name, std::string& x) const {
    return *this;
  }
};


class MyData
{
public:
    MyData() : A(0), X(0), id()
    {}
    explicit MyData(int) : A(97), X(CC_PI), id("mydata1234") // explicit to avoid implicit conversion
    {}
    void write(FileStorage& fs) const                        //Write serialization for this class
    {
        fs<<"{"<<"A"<<A<<"X"<<X<<"id"<<id<<"}";
    }
    void read(const FileNode& node)                          //Read serialization for this class
    {
        node.get("A", A);
        node.get("X", X);
        node.get("id", id);
    }
public:   // Data Members
    int A;
    double X;
    std::string id;
};

#if 0
//These write and read functions must be defined for the serialization in FileStorage to work
static void write(FileStorage& fs, const std::string&, const MyData& x)
{
    x.write(fs);
}
static void read(const FileNode& node, MyData& x, const MyData& default_value = MyData()){
    if(node.empty())
        x = default_value;
    else
        x.read(node);
}

// This function will print our custom class to the console
static ostream& operator<<(ostream& out, const MyData& m)
{
    out<<"{ id = "<<m.id<<", ";
    out<<"X = "<<m.X<<", ";
    out<<"A = "<<m.A<<"}";
    return out;
}
#endif

int file_input_output(int ac, char** av)
{
    if (ac != 2)
    {

      printf("\n"
        "%s shows the usage of the OpenCV serialization functionality.\n"
        "usage: \n"
        "%s outputfile.yml.gz\n"
        "The output file may be either XML (xml) or YAML (yml/yaml). You can even compress it by "
        "specifying this in its extension like xml.gz yaml.gz etc... \n"
        "With FileStorage you can serialize objects in OpenCV by using the<<and >> operators\n"
        "For example: - create a class and have it serialized\n"
        "             - use it to read and write matrices.\n",
      av[0], av[0]);

        return 1;
    }
    const char* filename = av[1];
        MyData m11(1);
#if 0
    { //write
        CvMat R = Mat_<uchar>::eye(3, 3),
            T = Mat_<double>::zeros(3, 1);
        MyData m(1);

        FileStorage fs(filename, FileStorage::WRITE);

        fs<<"iterationNr"<<100;
        fs<<"strings"<<"[";                              // text - string sequence
        fs<<"image1.jpg"<<"Awesomeness"<<"../data/baboon.jpg";
        fs<<"]";                                           // close sequence

        fs<<"Mapping";                              // text - mapping
        fs<<"{"<<"One"<<1;
        fs<<       "Two"<<2<<"}";

        fs<<"R"<<R;                                      // CvMat
        fs<<"T"<<T;

        fs<<"MyData"<<m;                                // your own data structures

        fs.release();                                       // explicit close
        cout<<"Write Done.\n";
    }

    {//read
        cout<<endl<<"Reading: \n";
        FileStorage fs;
        fs.open(filename, FileStorage::READ);

        int itNr;
        //fs["iterationNr"] >> itNr;
        itNr = (int) fs["iterationNr"];
        cout<<itNr;
        if (!fs.isOpened())
        {
            cerr<<"Failed to open "<<filename<<endl;
            help(av);
            return 1;
        }

        FileNode n = fs["strings"];                         // Read string sequence - Get node
        if (n->tid != FileNode::SEQ)
        {
            cerr<<"strings is not a sequence! FAIL\n";
            return 1;
        }

        FileNodeIterator it = n.begin(), it_end = n.end(); // Go through the node
        for (; it != it_end; ++it)
            cout<<(string)*it<<endl;


        n = fs["Mapping"];                                // Read mappings from a sequence
        cout<<"Two  "<<(int)(n["Two"])<<"; ";
        cout<<"One  "<<(int)(n["One"])<<endl<<endl;


        MyData m;
        CvMat R, T;

        fs["R"] >> R;                                      // Read CvMat
        fs["T"] >> T;
        fs["MyData"] >> m;                                 // Read your own structure_

        cout<<endl
<<"R = "<<R<<endl;
        cout<<"T = "<<T<<endl<<endl;
        cout<<"MyData = \n"<<m<<endl<<endl;

        //Show default behavior for non existing nodes
        cout<<"Attempt to read NonExisting (should initialize the data structure with its default).";
        fs["NonExisting"] >> m;
        cout<<endl<<"NonExisting = \n"<<m<<endl;
    }

    cout<<endl
<<"Tip: Open up "<<filename<<" with a text editor to see the serialized data.\n";

#endif

    return 0;
}

