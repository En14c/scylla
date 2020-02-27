INCLUDES := ../include/
OBJS := $(patsubst %.c, %.o, $(wildcard *.c))

.PHONY: all clean

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDES) -o $@ $<

clean:
	rm -rf *.o
