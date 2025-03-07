#include <algorithm>
#include <bits/stdc++.h>
#define endl "\n"
#define YES "yes\n"
#define NO "no\n"
#define all(a) (a).begin(), (a).end()

using namespace std;
using ll = long long;

void print() { cout << endl; }

template <class T, class... Args> void print(T head, Args... rest) {
  cout << head << ' ';
  print(rest...);
}

template <class T, class... Args> void dbg(T head, Args... rest) {
#ifdef DEBUG
  cout << "[DBG] ";
  print(head, rest...);
#endif
}

void cases() {
#ifdef DEBUG
  cout << "==========================\n";
#endif
}

void solve() {}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int t = 1;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
