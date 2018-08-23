
#include "flags.hpp"
#include "auto_buffer.hpp"
#include "logging.hpp"
#include "test.hpp"


static const char* const keys =
  "{ h help    |       | print help }"
  "{ i info    | false | print info }"
  "{ t true    | true  | true value }"
  "{ n unused  |       | dummy }"
  ;

TEST(CommandLineParser, testFailure)
{
  const char* argv[] = {"<bin>", "-q"};
  const int argc = 2;
  CommandLineParser parser(argc, argv, keys);
  EXPECT_FALSE(parser.has("q"));
  EXPECT_FALSE(parser.get<bool>("q"));
  EXPECT_FALSE(parser.get<bool>(0));
}

TEST(CommandLineParser, testHas_noValues)
{
  const char* argv[] = {"<bin>", "-h", "--info"};
  const int argc = 3;
  CommandLineParser parser(argc, argv, keys);
  EXPECT_TRUE(parser.has("help"));
  EXPECT_TRUE(parser.has("h"));
  EXPECT_TRUE(parser.get<bool>("help"));
  EXPECT_TRUE(parser.get<bool>("h"));
  EXPECT_TRUE(parser.has("info"));
  EXPECT_TRUE(parser.has("i"));
  EXPECT_TRUE(parser.get<bool>("info"));
  EXPECT_TRUE(parser.get<bool>("i"));
  EXPECT_TRUE(parser.get<bool>("true"));
  EXPECT_TRUE(parser.get<bool>("t"));
  EXPECT_FALSE(parser.has("n"));
  EXPECT_FALSE(parser.has("unused"));
}

TEST(CommandLineParser, testHas_TrueValues)
{
  const char* argv[] = {"<bin>", "-h=TRUE", "--info=true"};
  const int argc = 3;
  CommandLineParser parser(argc, argv, keys);
  EXPECT_TRUE(parser.has("help"));
  EXPECT_TRUE(parser.has("h"));
  EXPECT_TRUE(parser.get<bool>("help"));
  EXPECT_TRUE(parser.get<bool>("h"));
  EXPECT_TRUE(parser.has("info"));
  EXPECT_TRUE(parser.has("i"));
  EXPECT_TRUE(parser.get<bool>("info"));
  EXPECT_TRUE(parser.get<bool>("i"));
  EXPECT_TRUE(parser.get<bool>("true"));
  EXPECT_TRUE(parser.get<bool>("t"));
  EXPECT_FALSE(parser.has("n"));
  EXPECT_FALSE(parser.has("unused"));
}
TEST(CommandLineParser, testHas_TrueValues1)
{
  const char* argv[] = {"<bin>", "-h=1", "--info=1"};
  const int argc = 3;
  CommandLineParser parser(argc, argv, keys);
  EXPECT_TRUE(parser.has("help"));
  EXPECT_TRUE(parser.has("h"));
  EXPECT_TRUE(parser.get<bool>("help"));
  EXPECT_TRUE(parser.get<bool>("h"));
  EXPECT_TRUE(parser.has("info"));
  EXPECT_TRUE(parser.has("i"));
  EXPECT_TRUE(parser.get<bool>("info"));
  EXPECT_TRUE(parser.get<bool>("i"));
  EXPECT_TRUE(parser.get<bool>("true"));
  EXPECT_TRUE(parser.get<bool>("t"));
  EXPECT_FALSE(parser.has("n"));
  EXPECT_FALSE(parser.has("unused"));
}
TEST(CommandLineParser, testHas_FalseValues0)
{
  const char* argv[] = {"<bin>", "-h=0", "--info=0"};
  const int argc = 3;
  CommandLineParser parser(argc, argv, keys);
  EXPECT_TRUE(parser.has("help"));
  EXPECT_TRUE(parser.has("h"));
  EXPECT_FALSE(parser.get<bool>("help"));
  EXPECT_FALSE(parser.get<bool>("h"));
  EXPECT_TRUE(parser.has("info"));
  EXPECT_TRUE(parser.has("i"));
  EXPECT_FALSE(parser.get<bool>("info"));
  EXPECT_FALSE(parser.get<bool>("i"));
  EXPECT_TRUE(parser.get<bool>("true"));
  EXPECT_TRUE(parser.get<bool>("t"));
  EXPECT_FALSE(parser.has("n"));
  EXPECT_FALSE(parser.has("unused"));
}

TEST(CommandLineParser, testBoolOption_noArgs)
{
  const char* argv[] = {"<bin>"};
  const int argc = 1;
  CommandLineParser parser(argc, argv, keys);
  EXPECT_FALSE(parser.get<bool>("help"));
  EXPECT_FALSE(parser.get<bool>("h"));
  EXPECT_FALSE(parser.get<bool>("info"));
  EXPECT_FALSE(parser.get<bool>("i"));
  EXPECT_TRUE(parser.get<bool>("true")); // default is true
  EXPECT_TRUE(parser.get<bool>("t"));
}

TEST(CommandLineParser, testBoolOption_noValues)
{
  const char* argv[] = {"<bin>", "-h", "--info"};
  const int argc = 3;
  CommandLineParser parser(argc, argv, keys);
  EXPECT_TRUE(parser.get<bool>("help"));
  EXPECT_TRUE(parser.get<bool>("h"));
  EXPECT_TRUE(parser.get<bool>("info"));
  EXPECT_TRUE(parser.get<bool>("i"));
  EXPECT_TRUE(parser.get<bool>("true"));
  EXPECT_TRUE(parser.get<bool>("t"));
}

TEST(CommandLineParser, testBoolOption_TrueValues)
{
  const char* argv[] = {"<bin>", "-h=TrUe", "-t=1", "--info=true", "-n=truE"};
  const int argc = 5;
  CommandLineParser parser(argc, argv, keys);
  EXPECT_TRUE(parser.get<bool>("help"));
  EXPECT_TRUE(parser.get<bool>("h"));
  EXPECT_TRUE(parser.get<bool>("info"));
  EXPECT_TRUE(parser.get<bool>("i"));
  EXPECT_TRUE(parser.get<bool>("true"));
  EXPECT_TRUE(parser.get<bool>("t"));
  EXPECT_TRUE(parser.get<bool>("unused"));
  EXPECT_TRUE(parser.get<bool>("n"));
}

TEST(CommandLineParser, testBoolOption_FalseValues)
{
  const char* argv[] = {"<bin>", "--help=FALSE", "-t=FaLsE", "-i=false", "-n=0"};
  const int argc = 5;
  CommandLineParser parser(argc, argv, keys);
  EXPECT_FALSE(parser.get<bool>("help"));
  EXPECT_FALSE(parser.get<bool>("h"));
  EXPECT_FALSE(parser.get<bool>("info"));
  EXPECT_FALSE(parser.get<bool>("i"));
  EXPECT_FALSE(parser.get<bool>("true"));
  EXPECT_FALSE(parser.get<bool>("t"));
  EXPECT_FALSE(parser.get<bool>("unused"));
  EXPECT_FALSE(parser.get<bool>("n"));
}


static const char* const keys2 =
  "{ h help    |          | print help }"
  "{ @arg1     | default1 | param1 }"
  "{ @arg2     |          | param2 }"
  "{ n unused  |          | dummy }"
  ;

TEST(CommandLineParser, testPositional_noArgs)
{
  const char* argv[] = {"<bin>"};
  const int argc = 1;
  CommandLineParser parser(argc, argv, keys2);
  EXPECT_TRUE(parser.has("@arg1"));
  EXPECT_FALSE(parser.has("@arg2"));
  EXPECT_EQ("default1", parser.get<String>("@arg1"));
  EXPECT_EQ("default1", parser.get<String>(0));
  EXPECT_EQ("", parser.get<String>("@arg2"));
  EXPECT_EQ("", parser.get<String>(1));
}

TEST(CommandLineParser, testPositional_default)
{
  const char* argv[] = {"<bin>", "test1", "test2"};
  const int argc = 3;
  CommandLineParser parser(argc, argv, keys2);
  EXPECT_TRUE(parser.has("@arg1"));
  EXPECT_TRUE(parser.has("@arg2"));
  EXPECT_EQ("test1", parser.get<String>("@arg1"));
  EXPECT_EQ("test2", parser.get<String>("@arg2"));
  EXPECT_EQ("test1", parser.get<String>(0));
  EXPECT_EQ("test2", parser.get<String>(1));
}

TEST(CommandLineParser, testPositional_withFlagsBefore)
{
  const char* argv[] = {"<bin>", "-h", "test1", "test2"};
  const int argc = 4;
  CommandLineParser parser(argc, argv, keys2);
  EXPECT_TRUE(parser.has("@arg1"));
  EXPECT_TRUE(parser.has("@arg2"));
  EXPECT_EQ("test1", parser.get<String>("@arg1"));
  EXPECT_EQ("test2", parser.get<String>("@arg2"));
  EXPECT_EQ("test1", parser.get<String>(0));
  EXPECT_EQ("test2", parser.get<String>(1));
}

TEST(CommandLineParser, testPositional_withFlagsAfter)
{
  const char* argv[] = {"<bin>", "test1", "test2", "-h"};
  const int argc = 4;
  CommandLineParser parser(argc, argv, keys2);
  EXPECT_TRUE(parser.has("@arg1"));
  EXPECT_TRUE(parser.has("@arg2"));
  EXPECT_EQ("test1", parser.get<String>("@arg1"));
  EXPECT_EQ("test2", parser.get<String>("@arg2"));
  EXPECT_EQ("test1", parser.get<String>(0));
  EXPECT_EQ("test2", parser.get<String>(1));
}

TEST(CommandLineParser, testEmptyStringValue)
{
  static const char* const keys3 =
    "{ @pos0 |        | empty default value }"
    "{ @pos1 | <none> | forbid empty default value }";
  const char* argv[] = {"<bin>"};
  const int argc = 1;
  CommandLineParser parser(argc, argv, keys3);
  // EXPECT_TRUE(parser.has("@pos0"));
  EXPECT_EQ("", parser.get<String>("@pos0"));
  EXPECT_FALSE(parser.has("@pos1"));
}

TEST(CommandLineParser, positional_regression_5074_equal_sign)
{
  static const char* const keys3 =
    "{ @eq0 |  | }"
    "{ eq1  |  | }";
  const char* argv[] = {"<bin>", "1=0", "--eq1=1=0"};
  const int argc = 3;
  CommandLineParser parser(argc, argv, keys3);
  EXPECT_EQ("1=0", parser.get<String>("@eq0"));
  EXPECT_EQ("1=0", parser.get<String>(0));
  EXPECT_EQ("1=0", parser.get<String>("eq1"));
}


TEST(AutoBuffer, allocate_test)
{
  AutoBuffer<int, 5> abuf(2);
  EXPECT_EQ(2u, abuf.size());
  abuf.allocate(4);
  EXPECT_EQ(4u, abuf.size());
  abuf.allocate(6);
  EXPECT_EQ(6u, abuf.size());
}

#if 0
TEST(CommandLineParser, testScalar)
{
  static const char* const keys3 =
    "{ s0 | 3 4 5 | default scalar }"
    "{ s1 |       | single value scalar }"
    "{ s2 |       | two values scalar (default with zeros) }"
    "{ s3 |       | three values scalar }"
    "{ s4 |       | four values scalar }"
    "{ s5 |       | five values scalar }";
  const char* argv[] = {"<bin>", "--s1=1.1", "--s3=1.1 2.2 3",
                        "--s4=-4.2 1 0 3", "--s5=5 -4 3 2 1"
                       };
  const int argc = 5;
  CommandLineParser parser(argc, argv, keys3);
  EXPECT_EQ(parser.get<Scalar>("s0"), Scalar(3, 4, 5));
  EXPECT_EQ(parser.get<Scalar>("s1"), Scalar(1.1));
  EXPECT_EQ(parser.get<Scalar>("s2"), Scalar(0));
  EXPECT_EQ(parser.get<Scalar>("s3"), Scalar(1.1, 2.2, 3));
  EXPECT_EQ(parser.get<Scalar>("s4"), Scalar(-4.2, 1, 0, 3));
  EXPECT_EQ(parser.get<Scalar>("s5"), Scalar(5, -4, 3, 2));
}
#endif

DEFINE_string(gpu, "",
              "Optional; run in GPU mode on given device IDs separated by ','."
              "Use '-gpu all' to run on all available GPUs. The effective training "
              "batch size is multiplied by the number of devices.");
DEFINE_string(solver, "", "The solver definition protocol buffer text file.");
DEFINE_string(model, "", "The model definition protocol buffer text file.");
DEFINE_string(phase, "", "Optional; network phase (TRAIN or TEST). Only used for 'time'.");
DEFINE_int32(level, 0, "Optional; network level.");
DEFINE_string(stage, "", "Optional; network stages (not to be confused with phase), separated by ','.");
DEFINE_string(snapshot, "", "Optional; the snapshot solver state to resume training.");
DEFINE_string(weights, "",
              "Optional; the pretrained weights to initialize finetuning, "
              "separated by ','. Cannot be set simultaneously with snapshot.");
DEFINE_int32(iterations, 50, "The number of iterations to run.");
DEFINE_string(sigint_effect, "stop",
              "Optional; action to take when a SIGINT signal is received: "
              "snapshot, stop or none.");
DEFINE_string(sighup_effect, "snapshot",
              "Optional; action to take when a SIGHUP signal is received: "
              "snapshot, stop or none.");


TEST(CommandLineParser, testScalar)
{
  const char* argv[] = { "<bin>", "--gpu=cpu", "--solver=lenet_solver.prototxt",
                         "--level=1", "--iterations=51"
                       };
  const int argc = 5;
  ::gflags::ParseCommandLineFlags(argc, argv, true);
  EXPECT_EQ(FLAGS_gpu, "cpu");
  EXPECT_EQ(FLAGS_solver, "lenet_solver.prototxt");
  EXPECT_EQ(FLAGS_level, 1);
  EXPECT_EQ(FLAGS_iterations, 51);
}