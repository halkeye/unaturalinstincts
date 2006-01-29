#ifndef __UNRATURALINSTINCTS_PLAYER_HPP_
#define __UNRATURALINSTINCTS_PLAYER_HPP_

class Room;

class Connection {
   public:
      void close();
};

class Player : public Connection {
   private:
      int _fd;
      Room * _room;

   public:
      Player(int nfd);
      int fd(int nfd = 0) { if (nfd > 0) { _fd = nfd; } return _fd; }
      void onRead();
      void printRoom();
};

#endif
