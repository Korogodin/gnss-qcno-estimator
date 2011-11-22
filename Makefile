NAME = front-end-corr
# C source names
CSRCS = front-end-corr.c PowerMeasure.c

COBJS = $(CSRCS:%.c=%.o)
ASOBJS = $(ASRCS:%.S=%.o)

OBJS= $(COBJS) $(ASOBJS)

LIBS = 

CC = gcc

CFLAGS = -Wall -g3

all: clean note $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS) 
	rm $(OBJS)
	@echo "******************** SUCCESS **********************"
	
dis: note $(OBJS)
	$(CC) $(CFLAGS)  -o $(NAME).elf $(OBJS) $(LIBS)
	$(OBJDUMP) -D $(NAME).elf > $(NAME).dis
	$(PREFIX)/$(TOOLPREFIX)-strip -s -R .comment $(NAME)
	@echo "******************** SUCCESS DIS **********************"

note:
	@echo "********************************************************"

clean:
	rm -f *.o $(OBJS) $(NAME).dis $(NAME).bin



%.o : %.c *.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o : %.S #*.h
	$(CC) $(CFLAGS) -c $< -o $@


