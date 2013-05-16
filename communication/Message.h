/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include "types.h"
#include "Buffer.h"
#include "Header.h"

using namespace std;

namespace Rhoban
{
  class Message : public Header, public Buffer
  {
  public:

    /**
     * Construit un message vide
     */
    Message();

    /**
     * Copy constructor
     */
    Message(const Message &);

    //void operator=(const Message& o);

    /**
     * Construit un message à partir d'un buffer existant
     */
    Message(ui32 size, char *buffer = NULL);

    /**
     * Destruction
     */
    virtual ~Message();

    /**
     * Obtiens les données brutes du message avec les en-têtes et la bonne
     * taille 
     */
    char* getRaw();

    /**
     * Efface le message
     */
    void clear();

    /**
     * Initialise le message
     */
    void init();
    void init_cursor();

    /**
     * Ecris les en-têtes dans le buffer
     */
    void write_header();

    /**
     * Lis un entier signé ou non signé
     */
    ui32 read_uint();
    void read(ui32 &value);
    int read_int();
    void read(int &value);

    /* Lit un booléen */
    bool read_bool();
    void read(bool &value);
    void read(vector<bool>::reference value);

    /**
     * Lis un nombre flottant depuis les données
     */
    float read_float();
    void read(float &value);
    double read_double();
    void read(double &value);

    /**
     * Lis une chaîne de caractère
     */
    string read_string(void);
    void read(string &value);

    /**
     * Lis un tableau de caractères
     */
    vector<ui8> read_array(void);
    void read(vector<ui8> &value);

    /**
     * Lis un tableau de tablean de caractères
     */
    vector< vector<ui8> > read_array_array(void);
    void read(vector< vector<ui8> > &value);

    /**
     * Il est possible de lire des vecteurs d'un type à l'aide de la méthode
     * read_TYPE_array, ou TYPE est un type lisible
     */
#define _READ_ARRAY_HEADER(TYPE, FUNCTION)	\
    vector< TYPE > FUNCTION##_array (); \
    void read(vector< TYPE > &value);

#define READ_ARRAY_HEADER(TYPE, FUNCTION)			\
    _READ_ARRAY_HEADER(TYPE, FUNCTION )				\
    _READ_ARRAY_HEADER(vector< TYPE >, FUNCTION ## _array ) \
    _READ_ARRAY_HEADER(vector< vector< TYPE > >, FUNCTION ## _array_array )

    READ_ARRAY_HEADER(string, read_string);
    READ_ARRAY_HEADER(ui32, read_uint);
    READ_ARRAY_HEADER(int, read_int);
    READ_ARRAY_HEADER(float, read_float);
    READ_ARRAY_HEADER(double, read_double);
    READ_ARRAY_HEADER(bool, read_bool);

    /**
     * Ecris un entier
     */
    void append(ui8 value);
    void append(bool value);
    void append(ui32 value);
    void append(int value);

    /**
     * Ecris un nombre flottant
     */
    void append(float value);
    void append(double value);

    /**
     * Ajoute un bloc arbitraire de données
     */
    void append(const char * data, ui32 siz);

    /**
     * Ajoute une chaîne de caractère
     */
    void append(const string &value);
    void append(const char *str);

    /**
     * Ajoute des données en utilisant l'opérateur de stream
     */
    friend Message & operator<< (Message & msg , ui8 &val);
    friend Message & operator<< (Message & msg , ui32 &val);
    friend Message & operator<< (Message & msg , int &val);
    friend Message & operator<< (Message & msg , float &val);
    friend Message & operator<< (Message & msg , double &val);
    friend Message & operator<< (Message & msg , string &val);
    friend Message & operator<< (Message & msg , vector<double> &val);
    friend Message & operator<< (Message & msg , vector<float> &val);
    friend Message & operator<< (Message & msg , vector<int> &val);
    friend Message & operator<< (Message & msg , vector<unsigned int> &val);

    /**
     * Il est possible d'ajouter des éléments de type vector<TYPE>, ou
     * TYPE est un type d'élément ajoutable.
     */
#define _APPEND_VECTOR_HEADER(TYPE)		\
    void append(const vector< TYPE > &values);

#define APPEND_VECTOR_HEADER(TYPE)			\
    _APPEND_VECTOR_HEADER(TYPE)				\
    _APPEND_VECTOR_HEADER(vector< TYPE >)		\
    _APPEND_VECTOR_HEADER(vector< vector< TYPE > >) \
    _APPEND_VECTOR_HEADER(vector < vector< vector< TYPE > > >)

    APPEND_VECTOR_HEADER(ui8);
    APPEND_VECTOR_HEADER(string);
    APPEND_VECTOR_HEADER(ui32);
    APPEND_VECTOR_HEADER(int);
    APPEND_VECTOR_HEADER(double);
    APPEND_VECTOR_HEADER(float);
    APPEND_VECTOR_HEADER(bool);

    /**
     * Affiche le message (peut servir pour débugger)
     */
    void print();

    /**
     * Dump le message brut (affichage héxadécimal)
     */
    void rawPrint();

  private:

    /**
     * Curseur interne utilisé pour lire et écrire le message
     */
    int cursor;
  };
}

#endif // _MESSAGE_H
