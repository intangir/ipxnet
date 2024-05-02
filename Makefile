CC=g++
TARGET=ipxnet
SRCFILES=main.cpp ipxserver.cpp
LIBS=-lSDL_net -lSDL -lX11 -lpthread

all: $(TARGET)

$(TARGET): $(SRCFILES)
	$(CC) -Wall $(CXXFLAGS) $(SRCFILES) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET) ipxnet.log
