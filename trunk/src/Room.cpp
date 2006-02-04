#include "config.h"

#include <string>

#include "Room.hpp"
#include "Player.hpp"
#include "AnsiColors.hpp"

const char * sExitTypes[EXIT_NONE] = {
   "northwest", "north", "northeast", "east",
   "southeast", "south", "southwet",  "west",
   "other"
};

/*
   _____      _ _   
  | ____|_  _(_) |_ 
  |  _| \ \/ / | __|
  | |___ >  <| | |_ 
  |_____/_/\_\_|\__|
                    
*/

Exit::Exit() {
   _room = NULL;
}

const std::string Exit::roomName() const { 
   if (!_room) { throw("A exit always have to have a room"); }
   return _room->name();
}

/*
   ____                       
  |  _ \ ___   ___  _ __ ___  
  | |_) / _ \ / _ \| '_ ` _ \ 
  |  _ < (_) | (_) | | | | | |
  |_| \_\___/ \___/|_| |_| |_|
                              
*/

Room::Room(std::string name) 
{

   for (int i = EXIT_NORTHWEST; i < EXIT_NONE; i++) {
      _exits[i] = NULL;
   }

   if (_roomName.empty()) {
      _roomName = "Unamed Room";
   }
   else {
      _roomName = name;
   }
   
   if (_roomDescription.empty()) {
      _roomDescription = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Sed aliquet. Curabitur nonummy, velit id dignissim posuere, ligula arcu ultrices erat, et interdum mi sem interdum lectus. Duis sodales, tortor congue tincidunt vehicula, ante orci varius pede, at fermentum nulla nulla pellentesque velit. Aliquam mi. Duis eros tortor, mollis vel, eleifend et, rhoncus ac, leo. Aenean vestibulum orci ac odio. Suspendisse a massa.";
   }
}
      
void Room::addExit(eExitTypes spot, Exit * e) {
   if (_exits[spot] != NULL) {
      delete _exits[spot];
      _exits[spot] = NULL;
   }
   _exits[spot] = e;
}

void Room::addExit(eExitTypes spot, Room * r, bool biDirectional) {
   Exit * e = new Exit();
   e->room(r);
   addExit(spot, e);

   if (biDirectional) {
      Exit * e2 = new Exit();
      e2->room(this);
      r->addExit(Exit::getOpposite(spot), e2);
   }
}

      
Exit * Room::getExit(eExitTypes pos) const {
   return _exits[pos];
}
   
eExitTypes Exit::getOpposite(eExitTypes pos) {
   switch (pos) {
      case EXIT_NORTHWEST:
         return EXIT_SOUTHEAST;
      case EXIT_NORTH:
         return EXIT_SOUTH;
      case EXIT_NORTHEAST:
         return EXIT_SOUTHWEST;
      case EXIT_EAST:
         return EXIT_WEST;

      case EXIT_SOUTHEAST:
         return EXIT_NORTHWEST;
      case EXIT_SOUTH:
         return EXIT_NORTH;
      case EXIT_SOUTHWEST:
         return EXIT_NORTHEAST;
      case EXIT_WEST:
         return EXIT_EAST;
      case EXIT_OTHER:
         return EXIT_OTHER;
      default:
         throw("Unknown exit type");
         return EXIT_NONE;
   }
}

void Room::addPlayer(Player * p, bool silent) {
   _charactersInRoom.push_back(p);
   if (!silent) {
      std::string leavestr;
      /* Fix me.. direction? */
      leavestr = p->name() + " has entered the room\n";
      this->echo(leavestr,p);
   }
}
void Room::removePlayer(Player * p, bool silent) {
   _charactersInRoom.remove(p);
   if (!silent) {
      std::string leavestr;
      /* Fix me.. direction? */
      leavestr = p->name() + " has left the room\n";
      this->echo(leavestr,p);
   }
}

void Room::echo(std::string str, Player * playerToSkip) {
   Player * player;
   PLAYERLIST::const_iterator iter;
   for (iter = _charactersInRoom.begin(); iter != _charactersInRoom.end(); iter++) {
      player = (Player *) *iter;
      if (playerToSkip != NULL && player != playerToSkip) {
         player->send(str);
      }
   }
}

void Room::showToPlayer(Player * p) const {

   std::string output = AnsiColors::RESET;

   output += AnsiColors::RED + _roomName + AnsiColors::RESET + "\n\r";
   output += AnsiColors::BLUE;
   for (unsigned int i = 0; i < _roomName.length(); i++) {
      output += "=";
   }
   output += AnsiColors::RESET;

   output += "\n\r";
   output += _roomDescription;
   output += "\n\r\n\r";
   output += "Exits\n\r";
   output += AnsiColors::BLUE;
   output += "=====";
   output += AnsiColors::RESET;
   output += "\n\r";

   for (int i = EXIT_NORTHWEST; i < EXIT_NONE; i++) {
      const Exit * e = this->getExit((eExitTypes)i);
      if (!e) {
         continue;
      }
      output += "" + std::string(sExitTypes[i]) + " --- " + e->roomName() + "\n\r";
   }

   output += AnsiColors::RESET;

   PLAYERLIST::const_iterator iter = _charactersInRoom.begin();
   Player * player;
   for ( ; iter != _charactersInRoom.end(); iter++) {
      player = *iter;
      if (player == p) { continue; }
      output += player->name() + " is here\n\r";
   }
   p->send(output);
}

