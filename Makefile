# Tên file chạy (Output)
TARGET = space_invaders_app

# Trình biên dịch
CC = gcc

# Cờ biên dịch (Lấy header từ SDL3 và thư mục include)
CFLAGS = -std=c99 -Wall -Wextra -I./include $(shell pkg-config --cflags sdl3)

# Cờ liên kết (Ncurses + Math + SDL3)
LDFLAGS = -lncurses -lm $(shell pkg-config --libs sdl3)

# TỰ ĐỘNG tìm tất cả file .c trong thư mục src/
SRCS = $(wildcard src/*.c)
# Đổi đuôi .c thành .o để tạo danh sách object
OBJS = $(SRCS:.c=.o)

# --- QUY TẮC BIÊN DỊCH ---

all: $(TARGET)

# Link các file .o thành file chạy
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile từng file .c thành .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dọn dẹp file rác
clean:
	rm -f src/*.o $(TARGET)

# Chạy game
run: all
	./$(TARGET)