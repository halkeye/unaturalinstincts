#include "config.h"

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

#include <stdarg.h>
#include <dlfcn.h>

#include "Application.hpp"
#include "Player.hpp"
#include "Room.hpp"

#include "Commands.hpp"
#include "AnsiColors.hpp"
	
// http://pueblo.sourceforge.net/doc/manual/ansi_color_codes.html
const char * AnsiColors::RESET        = "\033[0m";
const char * AnsiColors::BLACK        = "\033[30m";
const char * AnsiColors::DARK_RED     = "\033[31m";
const char * AnsiColors::DARK_GREEN   = "\033[32m";
const char * AnsiColors::BROWN        = "\033[33m";
const char * AnsiColors::DARK_BLUE    = "\033[34m";
const char * AnsiColors::PURPLE       = "\033[35m";
const char * AnsiColors::CYAN         = "\033[36m";
const char * AnsiColors::GREY         = "\033[37m";
const char * AnsiColors::DEFAULT      = "\033[39m";
const char * AnsiColors::RED          = "\033[1;31m";
const char * AnsiColors::GREEN        = "\033[1;32m";
const char * AnsiColors::YELLOW       = "\033[1;33m";
const char * AnsiColors::BLUE         = "\033[1;34m";
const char * AnsiColors::PINK         = "\033[1;35m";
const char * AnsiColors::LIGHT_BLUE   = "\033[1;36m";
const char * AnsiColors::WHITE        = "\033[1;37m";

Application * app = NULL;
Application * getApplication() {
   return app;
}

std::string Application::NEWLINE = "\n\r";

Room * room1, * room2;

int main(int argc, char ** argv) {
   int port = 0;

   if (argc > 1) {
      port = atoi(argv[1]);
   }
      
   if (port == 0) { 
      port = 10000;
   }

   debug("Pre New Application");

   room1 = new Room();
   room2 = new Room();
   room1->addExit(EXIT_NORTH,room2,true);

   app = new Application(port);
   Command::initCommands();

   debug("Post New Application");
   app->gameLoop();
   return 0;
}

void status(const char * msg, ...) {
   va_list   param;
   printf("%s[%sSTATUS%s]%s ",
         AnsiColors::RED, AnsiColors::BLUE,
         AnsiColors::RED, AnsiColors::RESET);

   va_start(param, msg);
   vprintf(msg, param);
   va_end(param);
   printf("\n");
}

void error(const char * msg, ...) {
   va_list   param;
   printf("%s[%sERROR%s]%s ",
         AnsiColors::RED, AnsiColors::BLUE,
         AnsiColors::RED, AnsiColors::RESET);

   va_start(param, msg);
   vprintf(msg, param);
   va_end(param);
   printf("\n");
}

void debug(const char * msg, ...) {
   va_list   param;
   printf("%s[%sDEBUG%s]%s ",
         AnsiColors::RED, AnsiColors::BLUE,
         AnsiColors::RED, AnsiColors::RESET);

   va_start(param, msg);
   vprintf(msg, param);
   va_end(param);
   printf("\n");

}
   


void Application::gameLoop() {
   fd_set readFds;
   int ret;
   time_t tTime = 0;
   struct timeval tv;
   Player * player;
   PLAYERLIST::iterator iter;

   shutdown = false;
   FD_ZERO(&readFds);
   FD_SET(_control,&_masterFds);

   while (!shutdown) {
      if (!closed.empty()) {
         for (iter = closed.begin(); iter != closed.end(); iter++) {
            player = *iter;
            players.remove(player);
            FD_CLR(player->fd(),&_masterFds);
            close(player->fd());
            delete player;
         }
         closed.clear();
      }

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
   //unsigned int iSaSize=sizeof(stSa);
   socklen_t iSaSize=sizeof(stSa);

   new_fd = accept(_control, (struct sockaddr*)&stSa, &iSaSize);
   if (new_fd < 0) {
      perror("accept()");
      throw("accept");
   }
   setNonBlocking(new_fd);
   FD_SET(new_fd, &_masterFds); 
   _maxFds = MAX_RANGE(_maxFds, new_fd+1);
   Player * p = new Player(new_fd);
	players.push_back(p);
   p->onConnect();
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

   /* open the needed object */
   _dlHandle = dlopen(NULL, RTLD_NOW);

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
   status("Listening using fd %d on %d", _control, port);
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
   static int resetPulse = PULSE_RESET;
   if (resetPulse-- <= 0 ) {
      std::string output("This is a reset Message\n");
      Player * p;
      PLAYERLIST::iterator iter;
      for (iter = players.begin(); iter != players.end(); iter++) {
         p = *iter;
         p->send(output);
      }
      resetPulse = PULSE_RESET;
   }
}

DO_FUN * Application::getCommand(const char *name)
{
   //void * funHandle;
   DO_FUN * funHandle;

   const char *error;

   funHandle = (DO_FUN *) dlsym(_dlHandle, name);

   if ((error = dlerror()) != NULL)
   {
      debug("Error locating %s in symbol table. %s\n\r", name, error);
      return (DO_FUN *) cmdNotFound;
   }
   return (DO_FUN *) funHandle;
}

bool Application::queueRemove(Player * p) {
	closed.push_back(p);
   return true;
}
