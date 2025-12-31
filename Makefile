# # --- SETTINGS ---
# TARGET = space_invaders_app
# CC = gcc
# CFLAGS = -std=c99 -Wall -Wextra -D_DEFAULT_SOURCE -I./include $(shell pkg-config --cflags sdl3)
# LDFLAGS = -lncurses -lm $(shell pkg-config --libs sdl3)

# # --- SOURCE DISCOVERY ---
# SRCS = $(wildcard src/*.c) \
#        $(wildcard src/model/*.c) \
#        $(wildcard src/view/*.c) \
#        $(wildcard src/controller/*.c)
	   

# OBJS = $(patsubst %.c, build/%.o, $(SRCS))
# # 1. make 
# all: $(TARGET)

# $(TARGET): $(OBJS)
# 	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
# 	@echo "Build SUCCESS!"

# build/%.o: %.c
# 	@mkdir -p $(dir $@)
# 	$(CC) $(CFLAGS) -c $< -o $@

# # 2. make run-ncurses
# run-ncurses: all
# 	./$(TARGET) -text

# # 3. make run-sdl
# run-sdl: all
# 	./$(TARGET) -sdl

# # 4. make clean
# clean:
# 	rm -rf build $(TARGET)
# 	@echo "Cleaned up."

# # 5. make valgrind 
# valgrind: all
# 	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET) -text

# # --- EXTRA ---
# .PHONY: all clean run-ncurses run-sdl valgrind


# --- SETTINGS ---
TARGET = space_invaders_app
CC = gcc

# Thêm -O2 nếu muốn game chạy mượt hơn hoặc -g nếu muốn debug
CFLAGS = -std=c99 -Wall -Wextra -D_DEFAULT_SOURCE \
         -I./include \
         -I./src \
         $(shell pkg-config --cflags sdl3)
LDFLAGS = -lncurses -lm $(shell pkg-config --libs sdl3)

# --- SOURCE DISCOVERY ---
# Tự động tìm tất cả file .c trong src và các thư mục con của src
SRCS = $(shell find src -name "*.c")

# Tạo danh sách file object tương ứng trong thư mục build/
OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

# --- RULES ---
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "--------------------------"
	@echo "  Build SUCCESS: $(TARGET)"
	@echo "--------------------------"

# Biên dịch từng file .c thành .o
build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run-ncurses: all
	./$(TARGET) -text

run-sdl: all
	./$(TARGET) -sdl

clean:
	rm -rf build $(TARGET)
	@echo "Cleaned up."

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET) -text

.PHONY: all clean run-ncurses run-sdl valgrind