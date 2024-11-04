#include <ctype.h>
#include <stdio.h>
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
  // 現在の環境でフラグをオフにする
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | ISIG);

  // リアルタイム入力受付
  // VMIN: 受付文字数
  // VTIME: タイムアウト
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  // 現在の画面環境を更新
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
int main() {
  enableRawMode();
  while (1) {
    char c = '\0';
    read(STDIN_FILENO, &c, 1);
    // 制御文字かどうか
    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }
    if (c == 'q') break;
  }

  return 0;
}
