
OBJS := Main.o Error.o Data.o DataMac.o Config.o
ARCH := -arch i386 -arch x86_64 -arch ppc
CC := gcc
CFLAGS := -c -O2 -fPIC $(ARCH) -I../Inc -I/sw/include -I/opt/local/include
ARFLAGS := r
LFLAGS := -L/sw/lib -L/opt/local/lib -lusb -dynamiclib

default: static

all: static dynamic

static: Lib/libZestSC1.a

dynamic: Lib/libZestSC1.dylib

clean:
	rm -f $(OBJS) Lib/libZestSC1.a Lib/libZestSC1.dylib

Lib/libZestSC1.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
	ranlib $@

Lib/libZestSC1.dylib: $(OBJS)
	$(CC) $(LFLAGS) -o $@ $^

.PHONY: default all static dynamic clean

