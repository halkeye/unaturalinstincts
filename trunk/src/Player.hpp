#ifndef __UNRATURALINSTINCTS_PLAYER_HPP_
#define __UNRATURALINSTINCTS_PLAYER_HPP_

#include <string>

class Room;


enum eConnectionStates
{
   CONN_CONNECTING, CONN_GETNAME, CONN_PLAYING,

   CONN_MAX
};				/* ----------  end of enum eConnectionStates  ---------- */

typedef enum eConnectionStates CONNECTIONSTATE;

class Player {
   private:
      std::string _inbuf;
      int _fd;
      Room * _room;
      CONNECTIONSTATE _state;

   public:
      Player(int nfd);
      int fd(int nfd = 0) { if (nfd > 0) { _fd = nfd; } return _fd; }

      Room * room(Room * r = NULL) { if (r != NULL) _room = r; return _room; } 
      void onRead();
      void onConnect();
      void printRoom();
      
      void close();
      void send(std::string &str);
};

#endif
