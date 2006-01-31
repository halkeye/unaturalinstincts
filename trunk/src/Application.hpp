#ifndef __UNRATURALINSTINCTS_APPLICATION_HPP_
#define __UNRATURALINSTINCTS_APPLICATION_HPP_

#include <string>
#include <set>

/* Can't use MAX as its used elesewhere */
#ifndef MAX_RANGE
#define MAX_RANGE(a,b) ((a)>(b)?(a):(b))
#endif

#if !defined(__cplusplus)
#define CMDF void
#else
#define CMDF extern "C" void
#endif

#define PULSE_RESET           3600

class Player;

typedef void DO_FUN(Player * ch, std::string argument);

CMDF cmdNotFound(Player * p, std::string arguments);

class Application {
   public:
      Application(int port);

   private:
      int _control; /* Main server connection */
      fd_set _masterFds;
      std::set<Player *> players;
      int _maxFds;
      bool shutdown;
      int _port;
      void *_dlHandle;

   public:
      void gameLoop();
      void onRead();
      void onPulse();
      int makeSocket(int socktype);
      bool setNonBlocking(int fd);

      int port() { return _port; }
      int port(int nport) { _port = nport; return _port; }

      DO_FUN *getCommand(char *name);

};

extern Application * getApplication();

#endif
