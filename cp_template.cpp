#include <bits/stdc++.h>
#include <numeric>
#define endl "\n"
#define YES "yes\n"
#define NO "no\n"
#define all(a) (a).begin(), (a).end()
#define rall(a) (a).rbegin(), (a).rend()

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

void cases(int n) {
#ifdef DEBUG
  cout << "=============" << n << "============\n";
#endif
}

void solve() {}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int t = 1;
  cin >> t;
  for (int i = 1; i <= t; ++i) {
    cases(i);
    solve();
  }
  return 0;
}
