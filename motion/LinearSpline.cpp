/*
 * Spline.cpp
 *
 *  Created on: 11 janv. 2013
 *      Author: Hugo
 */


#include <algorithm>
#include "util.h"

#include "LinearSpline.h"

namespace Rhoban
{

bool my_sort(LinearSplineSequence::SplinePoint a, LinearSplineSequence::SplinePoint b)
{
    return a.x < b.x;
}

void LinearSplineSequence::importRawData(vector<float> raw_data, bool use_tangents)
{
    points.resize(raw_data.size() / (use_tangents ? 4 : 2) );
    uint ind = 0;
    for(uint i = 0 ; i< points.size(); i++)
    {
        SplinePoint * point = &points[i];
        point->x = raw_data[ind++];
        point->y = raw_data[ind++];
        if(use_tangents)
        {
            point->left_tangent = raw_data[ind++];
            point->right_tangent = raw_data[ind++];
        }
    }
    sort( points.begin() , points.end(), my_sort);
}

void LinearSplineSequence::exportToRawData(	vector<float> & result, bool use_tangents) const
{
    result.resize( (use_tangents ? 4 : 2)  * points.size());
    uint ind = 0;
    for(uint i = 0 ; i< points.size(); i++)
    {
        const SplinePoint * point = &points[i];
        result[ind++] = point->x;
        result[ind++] = point->y;
        if(use_tangents)
        {
            result[ind++] = point->left_tangent;
            result[ind++] = point->right_tangent;
        }
    }
}

void LinearSpline::importRawData(vector < vector<float> > raw_data)
{
    if(sequences.size() != raw_data.size())
        throw std::runtime_error("Sequence number and raw data size do not match for spline "+ name);
    for(uint i = 0; i < sequences.size(); i++)
    {
        LinearSplineSequence * ss = &sequences[i];
        ss->importRawData(raw_data[i], (ss->type == 1));
    }
}

void LinearSpline::exportToRawData(vector < vector<float> > & result) const
{
    result.resize(sequences.size());
    for(uint i = 0; i < sequences.size(); i++)
        sequences[i].exportToRawData(result[i]);
}


void LinearSpline::from_xml(TiXmlNode* node)
{
    name = XMLTools::get_string_element(node,"name");

    //length = XMLTools::get_double_element(node,"length");
    shift = XMLTools::get_double_element(node,"shift");
    speed_factor = XMLTools::get_double_element(node,"speed_factor");

    for ( TiXmlNode* child = node->FirstChild();
            child != 0;
            child = child->NextSibling())
    {
        if (!strcmp(child->Value(),"sequences")) {
            for ( TiXmlNode* sequencenode = child->FirstChild();
                    sequencenode != 0;
                    sequencenode = sequencenode->NextSibling())
            {
                if (!strcmp(sequencenode->Value(),"SplineSequence"))
                {
                    LinearSplineSequence seq(sequencenode);
                    sequences.push_back(seq);
                }
            }
        }
    }

    if(sequences.size()==0)
        xml_parse_error("Spline defined with no sequence!")

}


LinearSplineSequence::LinearSplineSequence(TiXmlNode * sequencenode)
{
    TiXmlNode * pointsnode = sequencenode->FirstChild();

    name = XMLTools::get_string_element(sequencenode,"name");
    type = XMLTools::get_int_element(sequencenode,"type");
    cyclic = XMLTools::get_int_element(sequencenode,"cyclic");
    bounded = XMLTools::get_int_element(sequencenode,"bounded");

    if(pointsnode)
    {
        for ( TiXmlNode* pointnode = pointsnode->FirstChild();
                pointnode != 0;
                pointnode = pointnode->NextSibling())
        {
            if (!strcmp(pointnode->Value(),"SplinePoint"))
            {
                SplinePoint p(XMLTools::get_float_element(pointnode, "x"),
                        XMLTools::get_float_element(pointnode, "y"),
                        XMLTools::get_float_element(pointnode, "left_tangent"),
                        XMLTools::get_float_element(pointnode, "right_tangent"));
                points.push_back(p);
            }
        }
    }
}


}


