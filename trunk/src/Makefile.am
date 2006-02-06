bin_PROGRAMS = unaturalinstincts

#AM_CXXFLAGS = -gdwarf-23 -W -Wall -pedantic -fno-inline
AM_CXXFLAGS = -g3 -W -Wall -pedantic -fno-inline @LIBXML2_CFLAGS@
LIBS += @LIBXML2_LIBS@
@WITH_CRYPT@ LIBS += -lcrypt

noinst_HEADERS = Room.hpp Player.hpp Application.hpp AnsiColors.hpp Types.hpp
unaturalinstincts_SOURCES = Application.cpp Room.cpp Player.cpp \
			    Commands.cpp

#unaturalinstincts_LDADD   = -ldl
if PLATFORM_WIN32 
unaturalinstincts_LDFLAGS = -export-all
else
unaturalinstincts_LDFLAGS = -export-dynamic
endif
