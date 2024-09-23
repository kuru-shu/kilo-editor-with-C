#include <termios.h>
#include <unistd.h>
void enableRawMode() {
  struct termios raw;
  // 現在の画面環境を取得
  tcgetattr(STDIN_FILENO, &raw);
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
