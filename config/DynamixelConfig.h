#ifndef DYNAMIXELCONFIG_H
#define DYNAMIXELCONFIG_H

#include <cstdlib>
#include <iostream>

#include "names.h"
#include "XMLTools.h"
#include "Config.h"

using namespace std;

class DynamixelConfig : public Config
{
    public:
        //empty string means deactivated
        string device_name;
        uint frequency;
        uint write_frequency;
        uint usart_id;

        DynamixelConfig() {device_name = "Dynamixel";}

        void from_xml(TiXmlNode *node);
        string to_xml() const;

        string class_name() const;

};

#endif // DYNAMIXELCONFIG_H
