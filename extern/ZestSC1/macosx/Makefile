
TARGET = Lib/libZestSC1.a
ARFLAGS = r
OBJS = Main.o Error.o Data.o Config.o
CC = gcc
CFLAGS = -c -O2 -I../Inc -I/sw/include

all: $(TARGET)

clean:
	rm $(TARGET)
	rm $(OBJS)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)

