#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

using namespace std;

void enableRawMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term); 
}

void disableRawMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);  
    term.c_lflag |= (ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSANOW, &term); 
}
  
int main() {
    // ============ Setup part ============
    enableRawMode();  // enable Raw input mode

    std::cout << "Press arrow keys (ESC to quit):\n";

    
    // ============ Loop part ============
    while (true) {
        char key;
        read(STDIN_FILENO, &key, 1);  // read key input

        switch (key){
            // -------- for control --------
            case 'w':
                std::cout << "w key pressed\n";
                break;
            case 's':
                std::cout << "s key pressed\n";
                break;
            case 'a':
                std::cout << "a key pressed\n";
                break;
            case 'd':
                std::cout << "d key pressed\n";
                break;
            // --------------------------------
            case 'q':   // quit process
                std::cout << "quit process ..." << std::endl;
                disableRawMode();  // disable raw input mode
                return 0;
        }        
    }

    return 0;
}