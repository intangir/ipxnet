CC=g++
TARGET=ipxnet
SRCFILES=main.cpp ipxserver.cpp
LIBS=$(shell pkg-config --libs SDL_net)

all: $(TARGET)

$(TARGET): $(SRCFILES)
	$(CC) -Wall $(CXXFLAGS) $(SRCFILES) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET) ipxnet.log
