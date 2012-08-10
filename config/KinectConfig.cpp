/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <sstream>

#include "KinectConfig.h"
#include "LowLevelConfig.h"

void KinectConfig::from_xml(TiXmlNode * node)
{
    kinects.clear();
    TiXmlNode * kinects_node = node->FirstChild( "kinects" );
    if(kinects_node)
        for ( TiXmlNode* child = kinects_node->FirstChild(); child != 0; child = child->NextSibling())
        {
            kinects.push_back(create_parameter(child));
        }
    else
        CLL_CAUTION_MSG("No node with name kinects");
}
        
KinectSensorParameter *KinectConfig::create_parameter(TiXmlNode *node)
{
    KinectSensorParameter *ksp = new KinectSensorParameter();
    ksp->from_xml(node);
    return ksp;
}

string KinectConfig::to_xml() const
{
    string result;
    result += "<kinects>";
    for(uint i =0; i< kinects.size(); i++)
    {
        const KinectSensorParameter * ksp = kinects[i];
        result += "<" + ksp->class_name() + ">" + ksp->to_xml() + "</" + ksp->class_name() + ">";
    }
    result += "</kinects>";
    return result;
}


string KinectConfig::class_name() const
{
    return KINECT_CONFIG_FILENAME;
}

KinectSensorParameter * KinectConfig::find_kinect_parameter(string sensor_name)
{
    for(uint i = 0; i< kinects.size(); i++)
    {
        KinectSensorParameter * ksp = kinects[i];
        if(ksp->name == sensor_name )
        {
            CLL_MSG("Found kinect parameters with name " << sensor_name);
            return ksp;
        }
    }
    CLL_MSG("Found no kinect parameter with name " << sensor_name << " among " << kinects.size() << " params");
    return NULL;
}

KinectSensorParameter::KinectSensorParameter()
{
    name="";
    for(int i = 0; i< KINECT_JOINTS_NB; i++)
    {
        min[i] = -1;
        max[i] = 1;
        zero[i] = 0;
    }
}

void KinectSensorParameter::from_xml(TiXmlNode * node)
{
    name = XMLTools::get_string_element(node,"name");

#ifndef NO_RHOBANMATH
    Matrix bound;

    bound = XMLTools::extract_double_array(node,"min");
    for(uint i = 0; (i< KINECT_JOINTS_NB) && (i< bound.size()); i++)
        min[i] = bound[i];

    bound = XMLTools::extract_double_array(node,"max");
    for(uint i = 0; (i< KINECT_JOINTS_NB) && (i< bound.size()); i++)
        max[i] = bound[i];

    bound = XMLTools::extract_double_array(node,"zero");
    for(uint i = 0; (i< KINECT_JOINTS_NB) && (i< bound.size()); i++)
        zero[i] = bound[i];

    for(uint i = 0; (i< KINECT_JOINTS_NB) && (i< bound.size()); i++)
        norm_coef[i] = 2.0 / (max[i] - min[i]);
#endif
}

string KinectSensorParameter::to_xml() const
{
    ostringstream result;
    result << "<name>" << name << "</name>";

    vector<double> vec(KINECT_JOINTS_NB);

    for(int i=0; i< KINECT_JOINTS_NB; i++)
        vec[i] = min[i];
    result << "<min>" << XMLTools::serialize_double_array(vec) << "</min>";

    for(int i=0; i< KINECT_JOINTS_NB; i++)
        vec[i] = max[i];
    result << "<max>" << XMLTools::serialize_double_array(vec) << "</max>";

    for(int i=0; i< KINECT_JOINTS_NB; i++)
        vec[i] = zero[i];
    result << "<zero>" << XMLTools::serialize_double_array(vec) << "</zero>";

    return result.str();
}

double KinectSensorParameter::normalize(float val, uint axis_id)
{
    if(axis_id >= KINECT_JOINTS_NB)
        throw string("KinectSensorParameter: normalize: axis_id too large");
    //return norm_coef[axis_id] * (val - zero[axis_id] ) ;
    //cout << "Kinect angle " << val << " (zero " << zero[axis_id] << ") for axis "<< axis_id << endl;
    return val  - zero[axis_id];
}
