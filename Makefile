NAME :=	miniRT
CC := cc
CFLAGS := -Wall -Wextra -Werror
LIBMLX := ./MLX42

HEADERS := -I ./headers -I $(LIBMLX)/include -I libft/headers

LIBS := $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm \
		./libft/libft.a

SRCS := src/main.c src/errors.c src/files.c src/ray.c src/rtfile.c \
		src/utils.c src/vector.c

OBJS := $(addprefix obj/, $(notdir $(SRCS:.c=.o)))

all: libmlx libft $(NAME)

clone_mlx42:
	if [ ! -d "$(LIBMLX)" ]; then \
		git clone https://github.com/codam-coding-college/MLX42.git $(LIBMLX); \
		fi

libmlx: clone_mlx42
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

libft:
	make -C ./libft

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $< && printf "Compiling: $(notdir $<)\n"

clean:
	@rm -rf $(OBJS)
	@rm -rf $(LIBMLX)/build
	make -C ./libft clean

fclean: clean
	@rm -rf $(NAME)
	make -C ./libft fclean

re: fclean all

.PHONY: all clean fclean re libmlx libft clone_mlx42