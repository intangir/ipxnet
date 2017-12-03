CC=g++
INSTALL=install
DESTDIR=
PREFIX=/usr/local
TARGET=ipxnet
SRCFILES=main.cpp ipxserver.cpp
LIBS=$(shell pkg-config --libs SDL_net)

all: $(TARGET)

$(TARGET): $(SRCFILES)
	$(CC) -Wall $(CXXFLAGS) $(SRCFILES) $(LIBS) -o $(TARGET)

install: $(TARGET)
	$(INSTALL) -m 755 -D $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

clean:
	rm -f $(TARGET) ipxnet.log
