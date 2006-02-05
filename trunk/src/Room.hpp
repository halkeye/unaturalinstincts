#ifndef __UNRATURALINSTINCTS_ROOM_HPP_
#define __UNRATURALINSTINCTS_ROOM_HPP_
#include <string>
#include "Types.hpp"

enum eExitTypes {
   EXIT_NORTHWEST, EXIT_NORTH, EXIT_NORTHEAST, EXIT_EAST,
   EXIT_SOUTHEAST, EXIT_SOUTH, EXIT_SOUTHWEST, EXIT_WEST,
   EXIT_OTHER, EXIT_NONE
};

extern const char * sExitTypes[EXIT_NONE];

class Room;

class Exit {
   public:
      Room * _room;

   public:
      Exit();
      const std::string roomName() const ; 
      Room * room(Room * to = NULL) { if (to) _room = to; return _room; }

   /* Static Functions */
      static eExitTypes getOpposite(eExitTypes pos);

};

class Room {
   private:
      std::string _roomName;
      std::string _roomDescription;
      Exit *      _exits[EXIT_NONE];
      /**
       * Characters that are in the room
       */
      PLAYERLIST  _charactersInRoom;

   public:
      Room(std::string name = "");

      const std::string name(std::string newName = "") { 
         if (!newName.empty()) { _roomName = newName; }
         return _roomName; 
      }

      const std::string description(std::string newDescription = "") { 
         if (!newDescription.empty()) { _roomDescription = newDescription; }
         return _roomDescription; 
      }

      void addExit(eExitTypes spot, Exit * e);
      void addExit(eExitTypes spot, Room * r, bool biDirectional);
      Exit * getExit(eExitTypes pos) const;
      void addPlayer(Player * p, bool silent=false) ;
      void removePlayer(Player * p, bool silent=false) ;
      void echo(std::string str, const Player * playerToSkip = NULL);
      void showToPlayer( Player * p) const;
};

#endif

