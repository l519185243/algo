#include <bits/stdc++.h>

namespace VEC_T {

using namespace std;

template <int n> struct VEC {
  array<double, n> A;
  VEC &operator+=(const VEC &o) {
    for (int i = 0; i < n; ++i)
      A[i] += o.A[i];
    return *this;
  }
  VEC &operator-=(const VEC &o) {
    for (int i = 0; i < n; ++i)
      A[i] -= o.A[i];
    return *this;
  }
  VEC &operator*=(double x) {
    for (auto &i : A)
      i *= x;
    return *this;
  }
  VEC operator-(const VEC &o) {
    VEC t = *this;
    return t -= o;
  }
  VEC operator+(const VEC &o) {
    VEC t = *this;
    return t += o;
  }
  VEC operator*(double x) {
    VEC t = *this;
    return t *= x;
  }
  double E_dis(const VEC &o) {
    double res = 0;
    for (int i = 0; i < n; ++i)
      res += (A[i] - o.A[i]) * (A[i] - o.A[i]);
    return res;
  }
  double norm() {
    double res = 0;
    for (auto &i : A)
      res += i * i;
    return sqrtl(res);
  }
  friend istream &operator>>(istream &is, VEC &v) {
    for (auto &i : v.A)
      is >> i;
    return is;
  }
  friend ostream &operator<<(ostream &os, const VEC &v) {
    for (auto &i : v.A)
      os << i << ' ';
    os << endl;
    return os;
  }
};

} // namespace VEC_T
using VEC_T::VEC;

namespace VEC_T_DEMO {

// problem link: https://atcoder.jp/contests/abc426/tasks/abc426_e

#define fn(f) auto(f) = [&]
using namespace std;

void demo() {
  using VEC = VEC<2>;
  VEC sa, sb, ta, tb;
  cin >> sa >> ta >> sb >> tb;
  VEC da = ta - sa, db = tb - sb;
  double norm_mn = min(da.norm(), db.norm());
  da *= norm_mn / da.norm(), db *= norm_mn / db.norm();
  double res = 1e10;
  fn(calc)(VEC & da, VEC & db, double x) {
    VEC pa = sa + da * x;
    VEC pb = sb + db * x;
    return (pa - pb).norm();
  };
  fn(tri_search)(VEC & da, VEC & db) {
    double l = 0, r = 1;
    for (int i = 0; i < 100; ++i) {
      double d = (r - l) / 3;
      double pl = l + d, pr = r - d;
      if (calc(da, db, pl) > calc(da, db, pr))
        l = pl;
      else
        r = pr;
    }
    res = min(res, calc(da, db, l));
  };
  tri_search(da, db);
  da += sa, db += sb;
  sa = da, sb = db;
  da = ta - da, db = tb - db;
  tri_search(da, db);
  cout << fixed << setprecision(15) << res << endl;
}

} // namespace VEC_T_DEMO
