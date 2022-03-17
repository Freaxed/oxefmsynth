// adds "main" to the exported symbols

#include "public.sdk/source/h/aeffectx.h"	// Version 2.x 'C' Extensions and Structures

extern "C"
{
    extern AEffect * VSTPluginMain(audioMasterCallback audioMaster);
    extern "C" __declspec(dllexport) AEffect * main_plugin (audioMasterCallback audioMaster);
#define main main_plugin
    AEffect * main(audioMasterCallback audioMaster)
    {
        return VSTPluginMain(audioMaster);
    }
}

