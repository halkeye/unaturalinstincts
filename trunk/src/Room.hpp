#ifndef __UNRATURALINSTINCTS_ROOM_HPP_
#define __UNRATURALINSTINCTS_ROOM_HPP_
#include <string>

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
      std::string roomName();
      Room * room(Room * to = NULL) { if (to) _room = to; return _room; }

   /* Static Functions */
      static eExitTypes getOpposite(eExitTypes pos);

};

class Room {
   private:
      std::string _roomName;
      std::string _roomDescription;
      Exit *      _exits[EXIT_NONE];

   public:
      Room(std::string name = "");

      std::string name(std::string newName = "") { 
         if (!newName.empty()) { _roomName = newName; }
         return _roomName; 
      }

      std::string description(std::string newDescription = "") { 
         if (!newDescription.empty()) { _roomDescription = newDescription; }
         return _roomDescription; 
      }

      void addExit(eExitTypes spot, Exit * e);
      void addExit(eExitTypes spot, Room * r, bool biDirectional);
      Exit * getExit(eExitTypes pos);
};

#endif

