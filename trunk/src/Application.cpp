/* According to POSIX 1003.1-2001 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <set>

class Connection {
   public:
      void close();
};

class Player : public Connection {
   public:
      int fd;
};

class Application {
   private:
      fd_set masterFds;
      set<Player *> connections;
      int maxFds;
      bool shutdown;

   public:
      void gameLoop();
      void onRead();
};

void Application::gameLoop() {
   fd_set readFds;
   int ret;
   struct timeval tv;
   Player * player;
   set<Player *>::iterator iter;

   shutdown = false;

   while (!shutdown) {
      tv.tv_sec = 1;
      tv.tv_sec = 0;
      ret = select(maxFds, &readFds, NULL, NULL, &tv);

      if (ret == -1) {
         perror("gameLoop::select()");
         throw "gameLoop::select()";
         break;
      }

      if (ret == 0) {
         continue;
      }

      if (FD_ISSET(control, &readFds)) {
         this->onRead();
      }

      for (iter = players.begin(); iter != players.end(); iter++) {
         player = (Player *) *iter;
         if (FD_ISSET(player->fd, &readFds)) {
            player->onRead();
         }
      }
   }
   return 0;
}

void Connection::close() {
   /* Remove me from Appliaction->fds */
}
