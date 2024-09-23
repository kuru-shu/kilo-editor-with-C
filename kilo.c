#include <unistd.h>
int main() {
  char c;
  // 標準入力のバイト数の読み込み
  // 1バイトであれば続ける
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  return 0;
}
