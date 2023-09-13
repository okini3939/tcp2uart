NAME  = tcp2uart
SRCS  = main.c tcp.c uart.c
OBJS  = $(SRCS:%.c=%.o)
CC    = cc
CFLAG = 

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

all: $(NAME)

.c.o:
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJS)

tgz:
	tar cfz $(NAME).tgz Makefile *.c *.h

