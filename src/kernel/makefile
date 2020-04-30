INCLUDES := ../include/
OBJS := $(patsubst %.c, %.o, $(wildcard *.c))
OBJS += $(patsubst %.s, %.o, $(wildcard *.s))

.PHONY: all clean

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDES) -o $@ $<

%.o: %.s
	$(ASM) $(ASMFLAGS) $<

clean:
	rm -rf *.o
