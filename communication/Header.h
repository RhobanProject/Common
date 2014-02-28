/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef _HEADER_H
#define _HEADER_H

#include <string>
#include <vector>
#include "types.h"
#include "Buffer.h"

using namespace std;

/**
 * Taille des en-têtes d'un message
 */
#define MSG_HEADER_SIZE 4*sizeof(ui32)

/**
 * Taille minimum d'un message
 */
#define MSG_MIN_SIZE MSG_HEADER_SIZE + 32

/**
 * Taille maximale d'un message
 */
#define MSG_MAX_SIZE 20000000

namespace Rhoban
{
  class Header
  {
  public:
    /**
     * Unique ID, identifie le message de manière unique
     */
    ui32 uid;
            
    /**
      * Source du message
      */
     ui16 source;

     /**
     * Destination du message (composant cible)
     */
    ui16 destination;

    /**
     * Commande du message
     */
    ui32 command;

    /**
     * Taille des données du message
     */
    ui32 length;

    /**
     * Is the message an answer ?
     */
    bool answer;

    /**
     * Is the message incoming ?
     */
    bool incoming;

    Header();

    void clear();

    /**
     * Lis les en-têtes à partir d'un buffer brut
     */
    void read_header(const char * raw_header);

    /**
     * Ecris les en-têtes dans un buffer brut
     */ 
    void write_header(char * raw_header);

    ui32 getUid();
    ui32 getDestination();
    ui32 getCommand();
    ui32 getLength();

    void setUid(ui32 uid);
    void setDestination(ui16 destination);
    void setCommand(ui32 command);
    void setLength(ui32 length);

    /**
     * Offset des éléments à l'intérieur d'un message
     */
    static const ui32 uid_offset         = 0;
    static const ui32 source_offset 	 = 4;
    static const ui32 destination_offset = 6;
    static const ui32 command_offset     = 8;
    static const ui32 length_offset      = 12;

  protected:
    /**
     * Compteur interne utilisé pour générer les identifiants uniques
     */
    static ui32 _uid; 
  };
}

#endif // _HEADER_H
