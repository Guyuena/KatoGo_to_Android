#include <sstream>
#include "main.h"

#include <android/log.h>
#include "core/os.h"
#include "core/mainargs.h"

#ifdef NO_GIT_REVISION
#define GIT_REVISION "<omitted>"
#else
#include "program/gitinfotemplate.h"
#include <jni.h>


#include <unistd.h>
#include <fcntl.h>

#endif


using namespace std;


//#include <sstream>

//------------------------
//#include "../core/using.h"
//------------------------


static void printHelp(const vector<string>& args) {

  cout << endl;


  if(args.size() >= 1)
    cout << "Usage: " << args[0] << " SUBCOMMAND ";
  else
    cout << "Usage: " << "./katago" << " SUBCOMMAND ";
  cout << endl;


  cout << R"%%(
---Common subcommands------------------

gtp : Runs GTP engine that can be plugged into any standard Go GUI for play/analysis.
benchmark : Test speed with different numbers of search threads.
genconfig : User-friendly interface to generate a config with rules and automatic performance tuning.

contribute : Connect to online distributed KataGo training and run perpetually contributing selfplay games.

match : Run self-play match games based on a config, more efficient than gtp due to batching.
version : Print version and exit.

analysis : Runs an engine designed to analyze entire games in parallel.
tuner : (OpenCL only) Run tuning to find and optimize parameters that work on your GPU.

---Selfplay training subcommands---------

selfplay : Play selfplay games and generate training data.
gatekeeper : Poll directory for new nets and match them against the latest net so far.

---Testing/debugging subcommands-------------
evalsgf : Utility/debug tool, analyze a single position of a game from an SGF file.

runtests : Test important board algorithms and datastructures
runnnlayertests : Test a few subcomponents of the current neural net backend

runnnontinyboardtest : Run neural net on a tiny board and dump result to stdout
runnnsymmetriestest : Run neural net on a hardcoded rectangle board and dump symmetries result
runownershiptests : Run neural net search on some hardcoded positions and print avg ownership

runoutputtests : Run a bunch of things and dump details to stdout
runsearchtests : Run a bunch of things using a neural net and dump details to stdout
runsearchtestsv3 : Run a bunch more things using a neural net and dump details to stdout
runsearchtestsv8 : Run a bunch more things using a neural net and dump details to stdout
runselfplayinittests : Run some tests involving selfplay training init using a neural net and dump details to stdout
runsekitrainwritetests : Run some tests involving seki train output

gtp: 运行可以插入任何标准Go GUI进行对弈和分析的GTP引擎。
benchmark: 使用不同数量的搜索线程测试速度。
genconfig: 提供用户友好的接口，用于生成包含规则和自动性能调优的配置文件。

contribute: 连接到在线分布式KataGo训练，永久运行自对弈游戏以贡献训练数据。

match: 基于配置文件运行自对弈对局，比起gtp更高效，因为可以进行批量处理。
version: 打印版本信息并退出。

analysis: 运行一个设计用于并行分析整个棋局的引擎。
tuner: （仅限于OpenCL）运行参数调优，以找到并优化适用于您的GPU的参数。

---自对弈训练子命令---------

selfplay: 进行自对弈游戏并生成训练数据。
gatekeeper: 监视目录以获取新的神经网络，并与迄今为止的最新网络进行对局。

---测试/调试子命令-------------
evalsgf: 实用工具/调试工具，从SGF文件中分析单个棋局的位置。

runtests: 测试重要的棋盘算法和数据结构。
runnnlayertests: 测试当前神经网络后端的一些子组件。

runnnontinyboardtest: 在一个小棋盘上运行神经网络，并将结果输出到标准输出。
runnnsymmetriestest: 在一个预定义的矩形棋盘上运行神经网络，并输出对称性结果。
runownershiptests: 在一些预定义的位置上运行神经网络搜索，并打印平均所有权。

runoutputtests: 运行多个功能并将详细信息输出到标准输出。
runsearchtests: 使用神经网络运行多个功能并将详细信息输出到标准输出。
runsearchtestsv3: 使用神经网络运行更多功能并将详细信息输出到标准输出。
runsearchtestsv8: 使用神经网络运行更多功能并将详细信息输出到标准输出。
runselfplayinittests: 运行涉及自对弈训练初始化的一些测试，并将详细信息输出到标准输出。
runsekitrainwritetests: 运行涉及seki训练输出的一些测试，并将详细信息输出到标准输出。
)%%" << endl;
}

static int handleSubcommand(const string& subcommand, const vector<string>& args) {
  vector<string> subArgs(args.begin()+1,args.end());
  if(subcommand == "analysis")
    return MainCmds::analysis(subArgs);
  if(subcommand == "benchmark")
    return MainCmds::benchmark(subArgs);
  if(subcommand == "contribute")
    return MainCmds::contribute(subArgs);
  if(subcommand == "evalsgf")
    return MainCmds::evalsgf(subArgs);
  else if(subcommand == "gatekeeper")
    return MainCmds::gatekeeper(subArgs);
  else if(subcommand == "genconfig")
    return MainCmds::genconfig(subArgs,args[0]);
  else if(subcommand == "gtp")
    return MainCmds::gtp(subArgs);
  else if(subcommand == "tuner")
    return MainCmds::tuner(subArgs);
  else if(subcommand == "match")
    return MainCmds::match(subArgs);
  else if(subcommand == "matchauto")
    return MainCmds::matchauto(subArgs);
  else if(subcommand == "selfplay")
    return MainCmds::selfplay(subArgs);
  else if(subcommand == "runtests")
    return MainCmds::runtests(subArgs);
  else if(subcommand == "runnnlayertests")
    return MainCmds::runnnlayertests(subArgs);
  else if(subcommand == "runnnontinyboardtest")
    return MainCmds::runnnontinyboardtest(subArgs);
  else if(subcommand == "runnnsymmetriestest")
    return MainCmds::runnnsymmetriestest(subArgs);
  else if(subcommand == "runownershiptests")
    return MainCmds::runownershiptests(subArgs);
  else if(subcommand == "runoutputtests")
    return MainCmds::runoutputtests(subArgs);
  else if(subcommand == "runsearchtests")
    return MainCmds::runsearchtests(subArgs);
  else if(subcommand == "runsearchtestsv3")
    return MainCmds::runsearchtestsv3(subArgs);
  else if(subcommand == "runsearchtestsv8")
    return MainCmds::runsearchtestsv8(subArgs);
  else if(subcommand == "runselfplayinittests")
    return MainCmds::runselfplayinittests(subArgs);
  else if(subcommand == "runselfplayinitstattests")
    return MainCmds::runselfplayinitstattests(subArgs);
  else if(subcommand == "runsekitrainwritetests")
    return MainCmds::runsekitrainwritetests(subArgs);
  else if(subcommand == "runnnonmanyposestest")
    return MainCmds::runnnonmanyposestest(subArgs);
  else if(subcommand == "runnnbatchingtest")
    return MainCmds::runnnbatchingtest(subArgs);
  else if(subcommand == "runtinynntests")
    return MainCmds::runtinynntests(subArgs);
  else if(subcommand == "runnnevalcanarytests")
    return MainCmds::runnnevalcanarytests(subArgs);
  else if(subcommand == "samplesgfs")
    return MainCmds::samplesgfs(subArgs);
  else if(subcommand == "dataminesgfs")
    return MainCmds::dataminesgfs(subArgs);
  else if(subcommand == "genbook")
    return MainCmds::genbook(subArgs);
  else if(subcommand == "trystartposes")
    return MainCmds::trystartposes(subArgs);
  else if(subcommand == "viewstartposes")
    return MainCmds::viewstartposes(subArgs);
  else if(subcommand == "demoplay")
    return MainCmds::demoplay(subArgs);
  else if(subcommand == "sampleinitializations")
    return MainCmds::sampleinitializations(subArgs);
  else if(subcommand == "runbeginsearchspeedtest")
    return MainCmds::runbeginsearchspeedtest(subArgs);
  else if(subcommand == "runsleeptest")
    return MainCmds::runsleeptest(subArgs);
  else if(subcommand == "printclockinfo")
    return MainCmds::printclockinfo(subArgs);
  else if(subcommand == "sandbox")
    return MainCmds::sandbox();
  else if(subcommand == "version") {
    cout << Version::getKataGoVersionFullInfo() << std::flush;
    return 0;
  }
  else {
    cout << "Unknown subcommand: " << subcommand << endl;
    printHelp(args);
    return 1;
  }
  return 0;
}


int main(int argc, const char* const* argv) {

  vector<string> args = MainArgs::getCommandLineArgsUTF8(argc,argv);

  if(args.size() < 2) {
    printHelp(args);
    return 0;
  }

  string cmdArg = string(args[1]);
  if(cmdArg == "-h" || cmdArg == "--h" || cmdArg == "-help" || cmdArg == "--help" || cmdArg == "help") {
    printHelp(args);
    return 0;
  }

#if defined(OS_IS_WINDOWS)
  //On windows, uncaught exceptions reaching toplevel don't normally get printed out,
  //so explicitly catch everything and print
  int result;
  try {
    result = handleSubcommand(cmdArg, args);
  }
  catch(std::exception& e) {
    cerr << "Uncaught exception: " << e.what() << endl;
    return 1;
  }
  catch(...) {
    cerr << "Uncaught exception that is not a std::exception... exiting due to unknown error" << endl;
    return 1;
  }
  return result;
#else
  return handleSubcommand(cmdArg, args);
#endif
}


string Version::getKataGoVersion() {
  return string("1.10.0");
}

string Version::getKataGoVersionForHelp() {
  return string("KataGo v1.10.0");
}

string Version::getKataGoVersionFullInfo() {
  ostringstream out;
  out << Version::getKataGoVersionForHelp() << endl;
  out << "Git revision: " << Version::getGitRevision() << endl;
  out << "Compile Time: " << __DATE__ << " " << __TIME__ << endl;
#if defined(USE_CUDA_BACKEND)
  out << "Using CUDA backend" << endl;
#if defined(CUDA_TARGET_VERSION)
#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
  out << "Compiled with CUDA version " << STRINGIFY2(CUDA_TARGET_VERSION) << endl;
#endif
#elif defined(USE_TENSORRT_BACKEND)
  out << "Using TensorRT backend" << endl;
#elif defined(USE_OPENCL_BACKEND)
  out << "Using OpenCL backend" << endl;
#elif defined(USE_EIGEN_BACKEND)
  out << "Using Eigen(CPU) backend" << endl;
#else
  out << "Using dummy backend" << endl;
#endif

#if defined(USE_AVX2)
  out << "Compiled with AVX2 and FMA instructions" << endl;
#endif
#if defined(COMPILE_MAX_BOARD_LEN)
  out << "Compiled to allow boards of size up to " << COMPILE_MAX_BOARD_LEN << endl;
#endif
#if defined(BUILD_DISTRIBUTED)
  out << "Compiled to support contributing to online distributed selfplay" << endl;
#endif

  return out.str();
}

string Version::getGitRevision() {
  return string(GIT_REVISION);
}

string Version::getGitRevisionWithBackend() {
  string s = string(GIT_REVISION);

#if defined(USE_CUDA_BACKEND)
  s += "-cuda";
#elif defined(USE_TENSORRT_BACKEND)
  s += "-trt";
#elif defined(USE_OPENCL_BACKEND)
  s += "-opencl";
#elif defined(USE_EIGEN_BACKEND)
  s += "-eigen";
#else
  s += "-dummy";
#endif
  return s;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_katago_Native_testMain(JNIEnv *env, jclass clazz) {

    stringstream buffer;
    //将cout的输出流重定向到自定义buffer
    streambuf* old = cout.rdbuf(buffer.rdbuf());

    handleSubcommand("version",{""});
//    int res = handleSubcommand("benchmark",{"-model","kata1-b18c384nbt-s7342816768-d3618704537.bin.gz"});
    try {
//      int res = handleSubcommand("gtp",{"-config","configs/gtp_example.cfg","-model","tests/models/g170-b6c96-s175395328-d26788732.bin.gz"});
//      int res = handleSubcommand("gtp",{" "," ","-config","configs/gtp_example.cfg","-model","tests/models/g170-b6c96-s175395328-d26788732.bin.gz"});
//      __android_log_print(ANDROID_LOG_ERROR, "NativeLog", "%d", res);
    }catch (const exception& e){
        __android_log_print(ANDROID_LOG_ERROR, "NativeLog", "%s", e.what());
    }

    cout.rdbuf(old);

    __android_log_print(ANDROID_LOG_ERROR, "NativeLog", "%s", buffer.str().c_str());

}