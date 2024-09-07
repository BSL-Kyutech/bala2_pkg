#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>



using namespace std;

void enableRawMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);  // 現在のターミナル設定を取得
    term.c_lflag &= ~(ICANON | ECHO); // カノニカルモードとエコーを無効化
    tcsetattr(STDIN_FILENO, TCSANOW, &term); // 設定を適用
}

void disableRawMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);  // 現在のターミナル設定を取得
    term.c_lflag |= (ICANON | ECHO); // カノニカルモードとエコーを有効化
    tcsetattr(STDIN_FILENO, TCSANOW, &term); // 設定を適用
}

void setNonBlockingMode() {
  struct termios term;
  tcgetattr(STDIN_FILENO, &term);  
  term.c_lflag &= ~(ICANON | ECHO); 
  tcsetattr(STDIN_FILENO, TCSANOW, &term); 

  int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK); // set non-blocking mode
}

void resetTerminalMode() {
  struct termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag |= (ICANON | ECHO); 
  tcsetattr(STDIN_FILENO, TCSANOW, &term);

  int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK); // set blocking mode again
}
  
int main() {
    enableRawMode();  // Rawモードを有効にする

    std::cout << "Press arrow keys (ESC to quit):\n";

    while (true) {
        char c;
        read(STDIN_FILENO, &c, 1);  // キー入力を1バイト読み取る

        if (c == 27) {  // ESCシーケンスの開始（カーソルキーやESCキー）
            char seq[2];
            read(STDIN_FILENO, &seq[0], 1);
            read(STDIN_FILENO, &seq[1], 1);

            if (seq[0] == '[') {  // カーソルキーのシーケンス
                switch (seq[1]) {
                    case 'A':
                        std::cout << "Up arrow key pressed\n";
                        break;
                    case 'B':
                        std::cout << "Down arrow key pressed\n";
                        break;
                    case 'C':
                        std::cout << "Right arrow key pressed\n";
                        break;
                    case 'D':
                        std::cout << "Left arrow key pressed\n";
                        break;
                }
            }
        } else if (c == 'q') {  // 'q'キーで終了
            break;
        } else {
            std::cout << "Other key pressed: " << c << "\n";
        }
    }

    disableRawMode();  // Rawモードを無効にする
    return 0;
}