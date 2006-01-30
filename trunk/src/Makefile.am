bin_PROGRAMS = unaturalinstincts

AM_CXXFLAGS = -gdwarf-23 -W -Wall -pedantic

noinst_HEADERS = Room.hpp Player.hpp Application.hpp
unaturalinstincts_SOURCES = Application.cpp Room.cpp Player.cpp \
									 Commands.cpp
unaturalinstincts_LDADD   = -ldl
unaturalinstincts_LDFLAGS = -export-dynamic
