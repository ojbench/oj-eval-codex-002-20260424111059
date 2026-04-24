#include <bits/stdc++.h>
using namespace std;
#include "code.cpp"
int main(){
  sjtu::int2048 a("12345678901234567890"), b("-9876543210");
  cout << (a + b) << "\n"; // 12345669024691345680
  cout << (a - b) << "\n"; // 12345687977777790100
  cout << (a * b) << "\n"; // negative
  sjtu::int2048 c("10"), d("3"), e("-10"), f("-3");
  cout << (c / d) << ' ' << (e / d) << ' ' << (c / f) << ' ' << (e / f) << "\n";
  cout << (c % d) << ' ' << (e % d) << ' ' << (c % f) << ' ' << (e % f) << "\n";
  return 0;
}
