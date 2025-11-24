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
  INFO apply(INFO &rt, int l, int r, int v, OP &op) {
    if (rt.inner(l, r) and op.inner_hit(rt, v)) {
      op.inner_op(rt, v);
      return rt;
    }
    if (rt.outer(l, r) and op.outer_hit(rt, v)) {
      op.outer_op(rt, v);
      return INFO();
    }
    rt.down();
    INFO L = apply(*rt.L, l, r, v, op);
    INFO R = apply(*rt.R, l, r, v, op);
    rt.up();
    return L.merge(R);
  }
  // For teaching only, all followings can be replaced by `apply`
  void add(int l, int r, ll v) { add(A[0], l, r, v); }
  void chmax(int l, int r, ll v) { chmax(A[0], l, r, v); }
  INFO qry(int l, int r) { return qry(A[0], l, r); }
  void add(INFO &rt, int l, int r, ll v) {
    if (rt.inner(l, r)) {
      rt.chadd(v);
      return;
    }
    if (rt.outer(l, r))
      return;
    rt.down();
    add(*rt.L, l, r, v);
    add(*rt.R, l, r, v);
    rt.up();
  }
  void chmax(INFO &rt, int l, int r, ll v) {
    if (rt.inner(l, r) and rt.mn < v and v < rt.se_mn) {
      rt.chmax(v);
      return;
    }
    if (rt.outer(l, r) or v <= rt.mn)
      return;
    rt.down();
    chmax(*rt.L, l, r, v);
    chmax(*rt.R, l, r, v);
    rt.up();
  }
  INFO qry(INFO &rt, int l, int r) {
    if (rt.inner(l, r))
      return rt;
    if (rt.outer(l, r))
      return INFO();
    rt.down();
    INFO L = qry(*rt.L, l, r);
    INFO R = qry(*rt.R, l, r);
    rt.up();
    return L.merge(R);
  }
};

OP SEG::NONE;
OP SEG::ADD = {.inner_hit = OP::TRUE,
               .outer_hit = OP::TRUE,
               .inner_op = [](INFO &rt, ll v) { rt.chadd(v); },
               .outer_op = OP::NONE};

OP SEG::CHMAX = {
    .inner_hit = [](INFO &rt, ll v) { return rt.mn < v < rt.se_mn; },
    .outer_hit = [](INFO &rt, ll v) { return v <= rt.mn; },
    .inner_op = [](INFO &rt, ll v) { rt.chmax(v); },
    .outer_op = OP::NONE};

OP SEG::CHMIN = {
    .inner_hit = [](INFO &rt, ll v) { return rt.se_mx < v < rt.mx; },
    .outer_hit = [](INFO &rt, ll v) { return v >= rt.mx; },
    .inner_op = [](INFO &rt, ll v) { rt.chmin(v); },
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
