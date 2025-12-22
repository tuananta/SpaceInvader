#ifndef VIEW_H
#define VIEW_H

#include "model.h"

// Cấu trúc chứa các con trỏ hàm (Interface)
// Giúp Controller điều khiển việc vẽ mà không cần biết đang dùng thư viện gì
typedef struct {
    void (*init)(void);                  // Hàm khởi tạo (mở cửa sổ)
    void (*cleanup)(void);               // Hàm dọn dẹp (đóng cửa sổ)
    void (*clear_screen)(void);                 // Xóa màn hình cũ
    void (*draw)(const GameState *game); // Vẽ màn hình mới
} GameView;

// Khai báo biến toàn cục để các file khác có thể gọi được
// (Được định nghĩa chi tiết trong view_ncurses.c)
extern GameView view_ncurses;
extern GameView view_sdl;

#endif