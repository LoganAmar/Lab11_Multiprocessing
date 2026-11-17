CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=-ljpeg -lm

# mandel and mandelmovie each get their own sources, objects, and executable
MANDEL_SOURCES= mandel.c jpegrw.c
MOVIE_SOURCES= mandelmovie.c
SOURCES= $(MANDEL_SOURCES) $(MOVIE_SOURCES)

MANDEL_OBJECTS=$(MANDEL_SOURCES:.c=.o)
MOVIE_OBJECTS=$(MOVIE_SOURCES:.c=.o)
OBJECTS=$(MANDEL_OBJECTS) $(MOVIE_OBJECTS)

MANDEL_EXE=mandel
MOVIE_EXE=mandelmovie

all: $(SOURCES) $(MANDEL_EXE) $(MOVIE_EXE)

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

$(MANDEL_EXE): $(MANDEL_OBJECTS)
	$(CC) $(MANDEL_OBJECTS) $(LDFLAGS) -o $@

# mandelmovie only forks and execs mandel, so it needs no libraries
$(MOVIE_EXE): $(MOVIE_OBJECTS)
	$(CC) $(MOVIE_OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > $*.d

clean:
	rm -rf $(OBJECTS) $(MANDEL_EXE) $(MOVIE_EXE) *.d