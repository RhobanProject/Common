/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <cstdio>
#include <tests/TestCase.h>
#include <communication/Encodings.h>

using namespace Rhoban;

/**
 * Test pour l'encodage des nombres utilisé sur le réseau
 */
class TestEncoding : public TestCase
{
    public:
        /**
         * Teste l'encodage d'un entier, l'octet le plus signifiant d'abord (MSB)
         */
        void testIntEncoding()
        {
            char buffer[4] = {0x00, 0x00, 0x04, 0xD2};
            char out[4];

            Encodings::encode_int(1234, out);

            assertEqualsSize(buffer, out, 4);
        }

        /**
         * Teste l'encodage d'un entier non signé, l'octet le plus signifiant d'abord (MSB)
         */
        void testUintEncoding()
        {
            char buffer[4] = {0x00, 0x00, 0x04, 0xD2};
            char out[4];

            Encodings::encode_uint(1234, out);

            assertEqualsSize(buffer, out, 4);
        }

        /**
         * Teste l'encodage d'un flottant
         */
        void testFloatEncoding()
        {
            char buffer[4] = {0x3F, 0x9D, 0x70, 0xA4};
            char out[4];

            Encodings::encode_float(1.23, out);

            assertEqualsSize(buffer, out, 4);
        }

        /**
         * Teste le décodage d'un entier
         */
        void testIntDecoding()
        {
            char buffer[4] = {0x00, 0x00, 0x04, 0xD2};

            assertEquals(Encodings::decode_int(buffer), 1234);
        }

        /**
         * Teste le décodage d'un entier non signé
         */
        void testUintDecoding()
        {
            char buffer[4] = {0x00, 0x00, 0x04, 0xD2};

            assertEquals((int)Encodings::decode_uint(buffer), 1234);
        }
        
        /**
         * Teste le décodage d'un flottant
         */
        void testFloatDecoding()
        {
            char buffer[4] = {0x3F, 0x9D, 0x70, 0xA4};

            assertEqualsDelta((float)Encodings::decode_float(buffer), (float)1.23, (float)0.00001);
        }

    protected:
        void _run()
        {
            testUintEncoding();
            testIntEncoding();
            testFloatEncoding();
            testIntDecoding();
            testUintDecoding();
            testFloatDecoding();
        }
};
