AM_CXXFLAGS = --pedantic -Wall -O2 -I ../src
AM_LDFLAGS = 

# deal with bug with generating header file from flex.
 
BUILT_SOURCES = json.cpp lex.cpp json.h lex.h 
CLEANFILES = lex.h
lex.h : lex.lpp
	$(LEX) --header-file=$@ -o /dev/null $<
AM_YFLAGS = -d


pkginclude_HEADERS = jsonapi.h jsonobj.h context.h jsonparse.h
pkglib_LTLIBRARIES = libjsonapi.la 

libjsonapi_la_SOURCES = json.ypp lex.lpp context.cpp context.h \
                          jsonapi.cpp jsonapi.h \
                          jsonobj.cpp jsonobj.h \
                          jsonparse.cpp jsonparse.h \
                          yyerror.cpp utf8.c
