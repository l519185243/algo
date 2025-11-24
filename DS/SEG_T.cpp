#include <bits/stdc++.h>

namespace SEG_T {

using namespace std;
using ll = long long;
struct INFO {
  static const ll INF = 1e18;
  ll i, l, r, mx, se_mx, mx_c, mn, se_mn, mn_c, sum, add;
  INFO *L, *R;

  INFO() { init(); }
  INFO(int _l, int _r, INFO *_L = nullptr, INFO *_R = nullptr) {
    l = _l;
    r = _r;
    L = _L;
    R = _R;
    init();
  }

  void init() {
    sum = mx_c = mn_c = add = 0;
    mx = se_mx = -INF;
    mn = se_mn = INF;
  }

  void set(ll v) {
    mx = mn = sum = v;
    mx_c = mn_c = 1;
  }

  void chadd(ll v) {
    if (!v)
      return;
    sum += (r - l + 1) * v;
    add += v;
    mx += v, mn += v;
    if (se_mn != INF)
      se_mn += v;
    if (se_mx != -INF)
      se_mx += v;
  }

  // ensure mn < v < se_mn
  void chmax(ll v) {
    if (v <= mn)
      return;
    sum += mn_c * (v - mn);
    mn = v;
    // change mn can only change se_mx
    se_mx = max(se_mx, v);
  }

  // ensure se_mx < v < mx
  void chmin(ll v) {
    if (v >= mx)
      return;
    sum += mx_c * (v - mx);
    mx = v;
    // change mx can only change se_mn
    se_mn = min(se_mn, v);
  }

  void chall(ll _mn, ll _mx, ll _add) {
    chadd(_add);
    chmax(_mn);
    chmin(_mx);
  }

  void down() {
    assert(L and R);
    L->chall(mn, mx, add);
    R->chall(mn, mx, add);
    add = 0;
  }

  void up() {
    assert(L and R);
    INFO T = *L;
    T = T.merge(*R);
    mx = T.mx, se_mx = T.se_mx, mx_c = T.mx_c;
    mn = T.mn, se_mn = T.se_mn, mn_c = T.mn_c;
    sum = T.sum;
  }

  bool inner(int tl, int tr) { return tl <= l and r <= tr; }

  bool outer(int tl, int tr) { return tr < l or r < tl; }

  INFO merge(INFO &o) {
    INFO res = *this;
    if (res.mx == o.mx) {
      res.mx_c += o.mx_c;
      res.se_mx = max(res.se_mx, o.se_mx);
    } else if (res.mx > o.mx) {
      res.se_mx = max(res.se_mx, o.mx);
    } else {
      res.se_mx = max(res.mx, o.se_mx);
      res.mx = o.mx;
      res.mx_c = o.mx_c;
    }
    if (res.mn == o.mn) {
      res.mn_c += o.mn_c;
      res.se_mn = min(res.se_mn, o.se_mn);
    } else if (res.mn < o.mn) {
      res.se_mn = min(res.se_mn, o.mn);
    } else {
      res.se_mn = min(res.mn, o.se_mn);
      res.mn = o.mn;
      res.mn_c = o.mn_c;
    }
    res.sum += o.sum;
    return res;
  }
};

struct OP {
  using BF = function<bool(INFO &, ll)>;
  using VF = function<void(INFO &, ll)>;

  static BF TRUE;
  static BF FALSE;
  static VF NONE;

  BF inner_hit = TRUE, outer_hit = TRUE;
  VF inner_op = NONE, outer_op = NONE;
};

OP::BF OP::TRUE = [](INFO &_, ll __) { return true; };
OP::BF OP::FALSE = [](INFO &_, ll __) { return false; };
OP::VF OP::NONE = [](INFO &_, ll __) {};

struct SEG {
  static OP ADD, CHMIN, CHMAX, MIN, MAX, NONE;
  vector<INFO> A;
  int ptr = 1;
  SEG(vector<ll> &_A, int cap) {
    int n = _A.size();
    A.resize(cap);
    A[0] = build(0, n - 1, _A);
  }
  SEG(vector<ll> &_A) {
    int n = _A.size();
    A.resize(n << 2);
    A[0] = build(0, n - 1, _A);
  }
  INFO &alloc(int l, int r, INFO *L = nullptr, INFO *R = nullptr) {
    assert(ptr < A.size());
    A[ptr] = INFO(l, r, L, R);
    return A[ptr++];
  }
  INFO &build(int l, int r, vector<ll> &_A) {
    if (l == r) {
      INFO &T = alloc(l, r);
      T.set(_A[l]);
      return T;
    }
    int m = l + r >> 1;
    INFO &L = build(l, m, _A);
    INFO &R = build(m + 1, r, _A);
    INFO &T = alloc(l, r, &L, &R);
    T.up();
    return T;
  }
  INFO apply(int l, int r, ll v = 0, OP &op = NONE) {
    return apply(A[0], l, r, v, op);
  }
  INFO apply(INFO &RT, int l, int r, int v, OP &op) {
    if (RT.inner(l, r) and op.inner_hit(RT, v)) {
      op.inner_op(RT, v);
      return RT;
    }
    if (RT.outer(l, r) and op.outer_hit(RT, v)) {
      op.outer_op(RT, v);
      return INFO();
    }
    RT.down();
    INFO L = apply(*RT.L, l, r, v, op);
    INFO R = apply(*RT.R, l, r, v, op);
    RT.up();
    return L.merge(R);
  }
  // For teaching only, all followings can be replaced by `apply`
  void add(int l, int r, ll v) { add(A[0], l, r, v); }
  void chmax(int l, int r, ll v) { chmax(A[0], l, r, v); }
  INFO qry(int l, int r) { return qry(A[0], l, r); }
  void add(INFO &RT, int l, int r, ll v) {
    if (RT.inner(l, r)) {
      RT.chadd(v);
      return;
    }
    if (RT.outer(l, r))
      return;
    RT.down();
    add(*RT.L, l, r, v);
    add(*RT.R, l, r, v);
    RT.up();
  }
  void chmax(INFO &RT, int l, int r, ll v) {
    if (RT.inner(l, r) and RT.mn < v and v < RT.se_mn) {
      RT.chmax(v);
      return;
    }
    if (RT.outer(l, r) or v <= RT.mn)
      return;
    RT.down();
    chmax(*RT.L, l, r, v);
    chmax(*RT.R, l, r, v);
    RT.up();
  }
  INFO qry(INFO &RT, int l, int r) {
    if (RT.inner(l, r))
      return RT;
    if (RT.outer(l, r))
      return INFO();
    RT.down();
    INFO L = qry(*RT.L, l, r);
    INFO R = qry(*RT.R, l, r);
    RT.up();
    return L.merge(R);
  }
};

OP SEG::NONE;
OP SEG::ADD = {.inner_hit = OP::TRUE,
               .outer_hit = OP::TRUE,
               .inner_op = [](INFO &info, ll v) { info.chadd(v); },
               .outer_op = OP::NONE};

OP SEG::CHMAX = {
    .inner_hit = [](INFO &info, ll v) { return info.mn < v < info.se_mn; },
    .outer_hit = [](INFO &info, ll v) { return v <= info.mn; },
    .inner_op = [](INFO &info, ll v) { info.chmax(v); },
    .outer_op = OP::NONE};

OP SEG::CHMIN = {
    .inner_hit = [](INFO &info, ll v) { return info.se_mx < v < info.mx; },
    .outer_hit = [](INFO &info, ll v) { return v >= info.mx; },
    .inner_op = [](INFO &info, ll v) { info.chmin(v); },
    .outer_op = OP::NONE};

} // namespace SEG_T
using SEG_T::SEG;

namespace SEG_T_DEMO {
// problem link: https://atcoder.jp/contests/abc426/tasks/abc426_f
using namespace std;
using ll = long long;
void demo() {
  int N;
  cin >> N;
  vector<ll> A(N);
  for (auto &i : A)
    cin >> i;
  int Q;
  cin >> Q;
  SEG tree(A);
  for (int l, r, x; Q; --Q) {
    cin >> l >> r >> x;
    l--, r--;
    // Alternative:
    // ll ans = tree.qry(l, r).sum;
    // tree.add(l, r, -x);
    // tree.chmax(l, r, 0);
    // ans -= tree.qry(l, r).sum;
    ll ans = tree.apply(l, r).sum;
    tree.apply(l, r, -x, SEG::ADD);
    ans -= tree.apply(l, r, 0, SEG::CHMAX).sum;
    cout << ans << endl;
  }
}

} // namespace SEG_T_DEMO
