bin_PROGRAMS = unaturalinstincts

#AM_CXXFLAGS = -gdwarf-23 -W -Wall -pedantic -fno-inline
AM_CXXFLAGS = -g3 -W -Wall -pedantic -fno-inline

@WITH_CRYPT@ LIBS += -lcrypt

noinst_HEADERS = Room.hpp Player.hpp Application.hpp AnsiColors.hpp Types.hpp
unaturalinstincts_SOURCES = Application.cpp Room.cpp Player.cpp \
			    Commands.cpp

#unaturalinstincts_LDADD   = -ldl
unaturalinstincts_LDFLAGS = -export-all
#unaturalinstincts_LDFLAGS = -export-dynamic
