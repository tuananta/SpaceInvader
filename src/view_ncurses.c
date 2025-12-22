#include "view.h"
#include <ncurses.h>
#include <stdio.h>

// Tỉ lệ thu nhỏ: Game 800px -> Terminal 80 ký tự (chia 10)
// Game 600px -> Terminal 30 dòng (chia 20)
#define SCALE_X 10
#define SCALE_Y 20

void ncurses_init(void) {
    initscr();              // Bắt đầu Ncurses
    cbreak();               // Nhận phím ngay không cần Enter
    noecho();               // Không hiện phím vừa gõ ra màn hình
    curs_set(0);            // Ẩn con trỏ nhấp nháy
    nodelay(stdscr, TRUE);  // Quan trọng: getch() sẽ KHÔNG chặn chương trình
    keypad(stdscr, TRUE);   // Cho phép bắt phím mũi tên
}

void ncurses_cleanup(void) {
    endwin();               // Trả lại terminal bình thường
}

void ncurses_clear(void) {
    erase();                // Xóa sạch màn hình
}

void ncurses_draw(const GameState *game) {
    // 1. Vẽ khung bao quanh
    box(stdscr, 0, 0);

    // 2. Vẽ thông tin (HUD)
    mvprintw(0, 2, "[ SPACE INVADERS - NCURSES ]");
    mvprintw(1, 2, "Level: %d | Score: %04d | Lives: %d", 
             game->level, game->player.score, game->player.lives);

    // 3. Vẽ Người chơi (Vaisseau)
    // Chuyển đổi tọa độ logic sang tọa độ terminal
    int px = (int)(game->player.base.x / SCALE_X);
    int py = (int)(game->player.base.y / SCALE_Y);
    mvprintw(py, px, "A"); // Chữ A làm phi thuyền

    // 4. Vẽ Kẻ thù (Enemies)
    for (int i = 0; i < TOTAL_ENEMIES; i++) {
        if (game->enemies[i].alive) {
            int ex = (int)(game->enemies[i].base.x / SCALE_X);
            int ey = (int)(game->enemies[i].base.y / SCALE_Y);
            
            // Chọn ký tự dựa trên loại kẻ thù
            char sprite = 'M';
            if (game->enemies[i].type == ENTITY_ENEMY_SQUID) sprite = 'W';
            else if (game->enemies[i].type == ENTITY_ENEMY_CRAB) sprite = 'H';
            
            mvprintw(ey, ex, "%c", sprite);
        }
    }
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (game->projectiles[i].active) {
            int bx = (int)(game->projectiles[i].base.x / SCALE_X);
            int by = (int)(game->projectiles[i].base.y / SCALE_Y);
            
            // Chỉ vẽ nếu đạn nằm trong màn hình
            if (by > 0 && by < 30) {
                mvprintw(by, bx, "|"); // Vẽ đạn bằng ký tự gạch đứng
            }
        }
    }


    refresh(); // Đẩy tất cả thay đổi lên màn hình
}

// Gói các hàm này vào struct để Main sử dụng
GameView view_ncurses = {
    .init = ncurses_init,
    .cleanup = ncurses_cleanup,
    .clear_screen = ncurses_clear,
    .draw = ncurses_draw
};