/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef ANALOGICSENSORCONFIG_H
#define ANALOGICSENSORCONFIG_H

#include <cstdlib>
#include <iostream>

#include <xml/XMLTools.h>

#include "names.h"
#include "Config.h"

using namespace std;

class AnalogicSensorParameter;

class AnalogicSensorConfig : public Config
{
    public:
        vector<AnalogicSensorParameter*> sensors;

        void from_xml(TiXmlNode *node);
        string to_xml() const;

        virtual AnalogicSensorParameter *create_parameter(TiXmlNode *node);

        AnalogicSensorParameter *find_sensor_parameter(string sensor_name);

        string class_name() const;
};

class AnalogicSensorParameter : public Config
{
    public:
        AnalogicSensorParameter();

        //empty name means that the sensor is deactivated
        string name;
        uint frequency;
        uint can;

        //etalonnage, cf normalize
        double min;
        double max;
        double zero;

        double normalize(double val);

        void from_xml(TiXmlNode * node);
        string to_xml() const;

        string class_name() const {return "AnalogicSensorParameter";}

    private:
        double norm_coef;
};

#endif //ANALOGICSENSORCONFIG_H
