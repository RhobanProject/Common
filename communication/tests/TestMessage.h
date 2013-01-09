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
#include <communication/Message.h>

using namespace Rhoban;

/**
 * Test sur la création de messages
 */
class TestMessage : public TestCase
{
    public:
        /**
         * Teste l'initialisation du message
         */
        void testInit()
        {
            Message msg;
            msg.clear();

            assertEquals((int)msg.getSize(), (int)MSG_HEADER_SIZE);
        }

        /**
         * Tests la création d'en-têtes
         */
        void testHeader()
        {
            Message msg;
            char header[] = {0x00, 0x00, 0x00, 0x7b,
                0x00, 0x00, 0x01, 0xc8,
                0x00, 0x00, 0x03, 0x15,
                0x00, 0x00, 0x03, 0xf3};

            msg.clear();
            msg.setUid(123);
            msg.setDestination(456);
            msg.setCommand(789);
            msg.setSize(1011 + MSG_HEADER_SIZE);

            assertEqualsSize(msg.getRaw(), header, 13);//sizeof(header));
        }

        /**
         * Test d'écriture le contenu d'un message
         */
        void testWriteContents()
        {
            Message msg;
            char contents[] = {
                0x00, 0x00, 0x00, 0x7b,
                0x00, 0x00, 0x00, 0x05,
                'H', 'e', 'l', 'l', 'o'
            };
            char *raw;

            msg.clear();
            msg.append((ui32)123);
            msg.append("Hello");
            raw = msg.getRaw();

            assertEquals(msg.getLength(), 13);
            assertEqualsSize(raw+MSG_HEADER_SIZE, contents, sizeof(contents));
        }

        /**
         * Test de lecture le contenu d'un message
         */
        void testReadContents()
        {
            char contents[] = {
                0x00, 0x00, 0x00, 0x7b, // 123
                0x00, 0x00, 0x00, 0x03, // 3
                0x00, 0x00, 0x00, 0x02, // 2
                0x00, 0x00, 0x00, 0x09, // 9
                0x00, 0x00, 0x00, 0x01, // 1
                0x00, 0x00, 0x00, 0x05, // 5
                'H', 'e', 'l', 'l', 'o'
            };

            Message msg(sizeof(contents), contents);
            msg.read_header(msg.getBuffer());
            
            assertEquals(msg.getUid(), 123);
            assertEquals(msg.getDestination(), 3);
            assertEquals(msg.getCommand(), 2);
            assertEquals(msg.getLength(), 9);
            assertEquals(msg.read_int(), 1);
            assertEquals(msg.read_string(), "Hello");
        }

        /**
         * Test de l'ajout d'un vector
         */
        void testAppendVector()
        {
            Message msg;
            vector<int> ints, back;

            ints.push_back(123);
            ints.push_back(456);
            ints.push_back(789);

            msg.append(ints);
            back = msg.read_int_array();

            assertEquals((int)back.size(), (int)ints.size());
            assertEquals(back[0], ints[0]);
            assertEquals(back[1], ints[1]);
            assertEquals(back[2], ints[2]);
        }

        /**
         * Test de la génération des UIDs
         */
        void testUidGeneration()
        {
            Message msg1, msg2, msg3;

            assertNotEquals((int)msg1.getUid(), (int)msg2.getUid());
            assertNotEquals((int)msg1.getUid(), (int)msg3.getUid());
            assertNotEquals((int)msg2.getUid(), (int)msg3.getUid());
        }

    protected:
        void _run()
        {
            testInit();
            testHeader();
            testWriteContents();
            testReadContents();
            testAppendVector();
            testUidGeneration();
        }
};
