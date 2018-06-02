



namespace cv
{

Algorithm::Algorithm()
{
}

Algorithm::~Algorithm()
{
}

void Algorithm::save(const String& filename) const
{
    FileStorage fs(filename, FileStorage::WRITE);
    fs << getDefaultName() << "{";
    write(fs);
    fs << "}";
}

String Algorithm::getDefaultName() const
{
    return String("my_object");
}

void Algorithm::writeFormat(FileStorage& fs) const
{
    fs << "format" << (int)3;
}

}


