#ifndef CONFIG_H_
#define CONFIG_H_

#include <cstdlib>
#include <iostream>
#include "XMLTools.h"

#define CLL_CAUTION_MSG(msg) cout << msg << endl;
#define CLL_MSG(msg) cout << msg << endl;

using namespace std;

class Config : public Serializable
{
    public:
        /**
         * Loads the configuration
         */
        void from_xml(TiXmlNode *node);

        /**
         * Saves the configuration
         */
        string to_xml() const;

        /**
         * Class name
         */
        virtual string class_name() const = 0;

        /**
         * Loads the configuration
         */
        void load_config();
};

#endif // CONFIG_H_
