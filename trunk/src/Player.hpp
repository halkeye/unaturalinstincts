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
      int fd() const { return _fd; }

      const Room * room(Room * r = NULL);
      void onRead();
      void onConnect();
      
      void close();
      void send(std::string &str);

      const std::string name() { std::string name("Player"); return name; }
};

#endif
