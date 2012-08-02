#ifndef KINECTCONFIG_H
#define KINECTCONFIG_H

#include <cstdlib>
#include <iostream>

#include "names.h"
#include "Config.h"

using namespace std;

class KinectSensorParameter;

class KinectConfig : public Config
{
    public:
        vector<KinectSensorParameter> kinects;

        void from_xml(TiXmlNode *node);
        string to_xml() const;

        KinectSensorParameter create_parameter(TiXmlNode *node);

        KinectSensorParameter * find_kinect_parameter(string sensor_name);

        string class_name() const;
};

#ifndef KINECT_JOINTS_NB
#define KINECT_JOINTS_NB 16
#endif

class KinectSensorParameter : public Config
{
public:
	KinectSensorParameter();

	//empty name means that the sensor is deactivated
	string name;

	//etalonnage, cf normalize
	double min[KINECT_JOINTS_NB];
	double max[KINECT_JOINTS_NB];
	double zero[KINECT_JOINTS_NB];

	double normalize(float val, uint axis_id);

	//loads a configuration file
	void from_xml(TiXmlNode * node);
	string to_xml() const;

	string class_name() const {return "KinectSensorParameter";}

private:
	double norm_coef[KINECT_JOINTS_NB];

};

#endif // KINECTCONFIG_H
