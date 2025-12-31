TARGET = space_invaders_app
CC = gcc

# Use -O2 for better performance or -g for debugging purposes
CFLAGS = -std=c99 -Wall -Wextra -D_DEFAULT_SOURCE \
         -I./include \
         -I./src \
         $(shell pkg-config --cflags sdl3)
LDFLAGS = -lncurses -lm $(shell pkg-config --libs sdl3)

# --- SOURCE DISCOVERY ---
# Automatically find all .c files in src and its subdirectories
SRCS = $(shell find src -name "*.c")

# Generate the list of corresponding object files in the build/ directory
OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

# --- RULES ---
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "--------------------------"
	@echo "  Build SUCCESS: $(TARGET)"
	@echo "--------------------------"

# Compile each .c file into its corresponding .o object file
build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Run the game in Terminal/Text mode
run-ncurses: all
	./$(TARGET) -text

# Run the game in SDL3 Graphical mode
run-sdl: all
	./$(TARGET) -sdl

# Remove object files and the final executable
clean:
	rm -rf build $(TARGET)
	@echo "Cleaned up."

# Run memory leak detection using Valgrind
valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET) -text

.PHONY: all clean run-ncurses run-sdl valgrind