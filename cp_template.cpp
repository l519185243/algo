#include <bits/stdc++.h>
#include <numeric>
#define endl "\n"
#define YES "yes\n"
#define NO "no\n"
#define all(a) (a).begin(), (a).end()
#define rall(a) (a).rbegin(), (a).rend()
#define fn(fc) auto fc = [&]

using namespace std;
using ll = long long;

template <typename... Args> void dbg(Args &&...args) {
#ifdef DEBUG
  ((cout << args << ' '), ...) << endl;
#endif
}

void cases(int n) {
#ifdef DEBUG
  cout << "=============" << n << "============\n";
#endif
}

struct MI {
  int x = 0, M = 998244353;
  MI() {}
  MI(int _x) { x = (_x % M + M) % M; }

  MI &operator+=(MI o) {
    add(o);
    return *this;
  }
  MI operator+(MI o) { return o += x; }
  MI &operator-=(MI o) {
    *this += -o.x;
    return *this;
  }
  MI operator-(MI o) { return MI(x) -= o; }
  MI &operator*=(MI o) {
    mul(o);
    return *this;
  }
  MI operator*(MI o) { return o *= x; }
  MI &operator/=(MI o) {
    *this *= o.inv();
    return *this;
  }
  MI operator/(MI o) { return MI(x) /= o; }
  MI inv() { return pow(M - 2); }
  MI pow(int y) {
    MI t(1), b(x);
    for (; y; y >>= 1, b *= b)
      if (y & 1)
        t *= b;
    return t;
  }
  friend MI operator+(int lhs, const MI &rhs) { return MI(lhs) + rhs; }
  friend MI operator-(int lhs, const MI &rhs) { return MI(lhs) - rhs; }
  friend MI operator*(int lhs, const MI &rhs) { return MI(lhs) * rhs; }
  friend MI operator/(int lhs, const MI &rhs) { return MI(lhs) / rhs; }

private:
  void add(MI y) { x = (x % M + y.x % M) % M; }
  void mul(MI y) { x = 1ll * x * y.x % M; }
};

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
