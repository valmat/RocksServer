// g++ -shared -fPIC -o smpl_plug.so smpl_plug.cpp

// g++ -fPIC -c smpl_plug.cpp
// g++ -shared smpl_plug.o -o smpl_plug.so

// nm smpl_plug.so


#include <iostream>
#include "../src/api.h"

 
//Наша функция, которую мы будем запускать из нашего приложения.
//extern "C" int myplug() 
PLUGIN()
{

    std::cout << "This message from myplug.so function myplug" << std::endl;
    return 5;
}