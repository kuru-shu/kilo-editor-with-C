#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;
void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {

  // 現在の画面環境を取得
  tcgetattr(STDIN_FILENO, &orig_termios);
  // プログラム終了時に呼び出す
  atexit(disableRawMode);

  struct termios raw = orig_termios;
  // 現在の環境でechoをオフにする
  raw.c_lflag &= ~(ECHO);
  // 現在の画面環境を更新
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
int main() {
  enableRawMode();
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  return 0;
}
