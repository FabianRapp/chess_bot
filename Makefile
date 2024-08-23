CC=cc
CFLAG=-fsanitize=address -g
SRC_PARENT = parent.c
OBJ_PARENT = $(SRC_PARENT:%.c=%.o)
PARENT = parent

SRC_CHILD = child.c
OBJ_CHILD = $(SRC_CHILD:%.c=%.o)
CHILD = child

LIBFT = libft.a

all: $(CHILD) $(PARENT)

$(PARENT): $(LIBFT) $(OBJ_PARENT)
	$(CC) $(CFLAGS) $(OBJ_PARENT) -o $(PARENT)

$(CHILD): $(LIBFT) $(OBJ_CHILD)
	$(CC) $(CFLAGS) $(OBJ_CHILD) -o $(CHILD)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	cd libft && make;
	cp ./libft/libft.a $(LIBFT)

clean:
	rm -f $(OBJ_PARENT) $(OBJ_CHILD)

fclean: clean
	rm -f $(LIBFT) $(CHILD) $(PARENT)
