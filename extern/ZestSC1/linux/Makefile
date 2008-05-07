
TARGET = Lib/libZestSC1.a
ARFLAGS = r
OBJS = Main.o Error.o Data.o DataLinux.o Config.o
CC = gcc
# Production values:
#CFLAGS = -c -O2 -W -Wall -I../Inc
# Debugging values:
CFLAGS = -c -g -O -W -Wall -fno-inline -I../Inc

all: $(TARGET)

clean:
	rm $(TARGET)
	rm $(OBJS)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)

