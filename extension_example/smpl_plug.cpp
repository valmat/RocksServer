// g++ -std=c++11 -shared -fPIC -o smpl_plug.so smpl_plug.cpp

// g++ -std=c++11 -fPIC -I../deps/rocksdb -c smpl_plug.cpp
// g++ -std=c++11 -shared smpl_plug.o -o smpl_plug.so

 // g++ -std=c++11 -fPIC -I../deps/rocksdb/include -c smpl_plug.cpp

// nm smpl_plug.so

#include "../src/api.h"
#include "RequestHelloWorld.h"



struct TestClass
{
    TestClass()
    {
        std::cout << "TestClass()" << std::endl;
    }

    ~TestClass()
    {
        std::cout << "~TestClass()" << std::endl;
    }

    operator int()
    {
        return 5678;
    }
};

static TestClass ret_val;



using namespace RocksServer;

//#define PLUGIN(extension) extern "C" int get_plugin(RocksServer::Extension *(extension))


 
//Наша функция, которую мы будем запускать из нашего приложения.
//extern "C" int get_plugin() 
//PLUGIN(extension)
void plugin(const RocksDBWrapper& rdb, const IniConfigs& cfg, Extension *extension)
{
    // create extension
    //static RocksServer::Extension extension();
    //ret_val = 177;

    extension->add("/test", new RequestHelloWorld());

    std::cout << "This message from get_plugin.so function get_plugin \t"<< (int)ret_val << std::endl;
}