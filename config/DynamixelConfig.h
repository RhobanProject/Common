/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef DYNAMIXELCONFIG_H
#define DYNAMIXELCONFIG_H

#include <cstdlib>
#include <iostream>

#include <xml/XMLTools.h>

#include "names.h"
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
