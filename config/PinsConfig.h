#ifndef PINSCONFIG_H
#define PINSCONFIG_H

#include <cstdlib>
#include <iostream>

#include "names.h"
#include "Config.h"

class PinDeviceParameter;

class PinsConfig : public Config
{
    public:
        vector<PinDeviceParameter> pins;

        void from_xml(TiXmlNode *node);
        string to_xml() const;

        string class_name() const;

        PinDeviceParameter create_parameter(TiXmlNode *node);

        PinDeviceParameter *find_pindevice_parameter(string pin_name);
};

class PinDeviceParameter : public Config
{
    public:
        PinDeviceParameter():frequency(0) {name=""; pin="";};

        string name;
        uint frequency;
        string pin;
        bool on;

        typedef enum { reader, writer } PinDeviceType;
        PinDeviceType type;

        void from_xml(TiXmlNode * node);
        string to_xml() const;
        string class_name() const {return "PinDeviceParameter";}

};

#endif // PINSCONFIG_H
