/*** includes ***/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** data ***/

struct termios orig_termios;

/*** terminal ***/

void die(const char *s) {
  // 投げられたエラー(errno)を見てエラーメッセージを表示
  perror(s);
  exit(1);
}


void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}

void enableRawMode() {

  // 現在の画面環境を取得
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
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
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/*** init ***/

int main() {
  enableRawMode();
  while (1) {
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
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
