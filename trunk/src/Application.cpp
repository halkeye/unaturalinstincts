/* According to POSIX 1003.1-2001 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/* socket stuff */
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>

#include <set>

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

#include "Player.hpp"

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

   public:
      void gameLoop();
      void onRead();
      void onPulse();
      int makeSocket(int socktype);
      bool setNonBlocking(int fd);

      int port() { return _port; }
      int port(int nport) { _port = nport; return _port; }

};

void Application::gameLoop() {
   fd_set readFds;
   int ret;
   time_t tTime = 0;
   struct timeval tv;
   Player * player;
   std::set<Player *>::iterator iter;

   shutdown = false;
   FD_ZERO(&readFds);
   FD_SET(_control,&_masterFds);

   while (!shutdown) {
      memcpy(&readFds,&_masterFds,sizeof(readFds));

      tv.tv_sec = 1;
      tv.tv_usec = 0;
        
      /* Update pulse called every second */
      if ( (time(0) - tTime) >= 1 /* second */ ) {
         this->onPulse();
         tTime = time(0);
      }

      ret = select(_maxFds+10, &readFds, NULL, NULL, &tv);

      if (ret == -1) {
         perror("gameLoop::select()");
         throw "gameLoop::select()";
         break;
      }

      if (ret == 0) {
         continue;
      }

      if (FD_ISSET(_control, &readFds)) {
         this->onRead();
      }

      for (iter = players.begin(); iter != players.end(); iter++) {
         player = (Player *) *iter;
         if (FD_ISSET(player->fd(), &readFds)) {
            player->onRead();
         }
      }
   }
   return;
}

void Application::onRead() {
   int new_fd;
   struct sockaddr_in stSa;
   unsigned int iSaSize=sizeof(stSa);

   new_fd = accept(_control, (struct sockaddr*)&stSa, (socklen_t *)iSaSize);
   if (new_fd < 0) {
      perror("accept()");
      throw("accept");
   }
   setNonBlocking(new_fd);
   FD_SET(new_fd, &_masterFds); 
   _maxFds = MAX(_maxFds, new_fd+1);
   Player * p = new Player(new_fd);
	players.insert(p);
}

void Connection::close() {
   /* Remove me from Appliaction->fds */
}

int main(int argc, char ** argv) {
   int port = 0;

   if (argc > 1) {
      port = atoi(argv[1]);
   }
      
   if (port == 0) { 
      port = 10000;
   }
   Application * app = new Application(port);
   app->gameLoop();
   return 0;
}

int Application::makeSocket(int socktype) {
   int ret = socket(PF_INET, socktype, 0);
   if (ret < 0) {
      return -1;
   }
   return ret;
}

Application::Application(int port) {
   struct sockaddr_in name;
   int yes = 1;
   _port = port;
   _maxFds = 1;

   _control = makeSocket(SOCK_STREAM);
   if (!_control) {
      perror("makeSocket");
      throw("Unable to make a socket");
   }
   if (setsockopt(_control, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
   {
      perror("setsockopt");
      exit(1);
   }


   name.sin_family=AF_INET;
   name.sin_port=htons(port);
   name.sin_addr.s_addr=htonl(INADDR_ANY);

   if (bind(_control, (sockaddr *)&name, sizeof(name))<0) {
      perror("bind");
      throw("bind");
   }
   setNonBlocking(_control);
   if (listen(_control, SOMAXCONN) == -1)
   {
      perror("listen");
      exit(1);
   }

   FD_ZERO(&_masterFds);
   FD_SET(_control, &_masterFds);
}

bool Application::setNonBlocking(int fd)
{
	int oldflags=fcntl(fd, F_GETFL, 0);
	if (oldflags == -1)
		return true;
   oldflags |= O_NONBLOCK;
	return fcntl(fd, F_SETFL, oldflags);
}

void Application::onPulse() 
{
   Player * p;
   std::set<Player *>::iterator iter;
   for (iter = players.begin(); iter != players.end(); iter++) {
      p = *iter;
      write(p->fd(), "This is a reset Message\n", 24);
   }
}

