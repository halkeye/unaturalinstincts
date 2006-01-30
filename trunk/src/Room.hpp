#ifndef __UNRATURALINSTINCTS_ROOM_HPP_
#define __UNRATURALINSTINCTS_ROOM_HPP_

#include "Player.hpp"

class Room {
   private:
      std::string _roomName;
   public:
      std::string name(std::string newName = "") { 
         if (!newName.empty()) { _roomName = newName; }
         return _roomName; 
      }
      void print(Player * p);
};

#endif

