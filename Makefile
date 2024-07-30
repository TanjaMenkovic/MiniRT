NAME := miniRT
CC := cc
CFLAGS := -Wall -Wextra -Werror
LIBMLX := ./MLX42

HEADERS := -I ./headers -I $(LIBMLX)/include -I libft/headers

LIBS_LINUX := $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm ./libft/libft.a
LIBS_MAC := $(LIBMLX)/build/libmlx42.a -Iinclude -lglfw \
            -L"/opt/homebrew/Cellar/glfw/3.3.9/lib/" -framework Cocoa \
            -framework OpenGL -framework IOKit ./libft/libft.a

SRCS := src/main.c src/errors.c src/files.c src/ray.c src/rtfile.c \
        src/utils.c src/vector.c

OBJS := $(addprefix obj/, $(notdir $(SRCS:.c=.o)))

all: libmlx libft $(NAME)

mac: CFLAGS += -D__MACOS__
mac: libmlx libft $(NAME)_mac

clone_mlx42:
	if [ ! -d "$(LIBMLX)" ]; then \
		git clone https://github.com/codam-coding-college/MLX42.git $(LIBMLX); \
	fi

libmlx:	clone_mlx42
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

libft:
	make -C ./libft

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBS_LINUX) -o $(NAME)

$(NAME)_mac: $(OBJS)
	$(CC) $(OBJS) $(LIBS_MAC) -o $(NAME)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $< && printf "Compiling: $(notdir $<)\n"

clean:
	@rm -rf $(OBJS)
	@rm -rf $(LIBMLX)/build
	make -C ./libft clean

fclean:	clean
	@rm -rf $(NAME)
	@rm -rf $(LIBMLX)
	make -C ./libft fclean

re:	fclean all

.PHONY: all clean fclean re libmlx libft clone_mlx42 mac
