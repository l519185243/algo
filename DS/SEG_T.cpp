#include <bits/stdc++.h>

namespace SEG_T {

using namespace std;
using ll = long long;
struct INFO {
  static const ll INF = 1e18;
  ll i, l, r, mx, se_mx, mx_c, mn, se_mn, mn_c, sum, add;
  INFO *cl, *cr;

  INFO() { init(); }
  INFO(int _l, int _r, INFO *_cl = nullptr, INFO *_cr = nullptr) {
    l = _l;
    r = _r;
    cl = _cl;
    cr = _cr;
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
    assert(cl and cr);
    cl->chall(mn, mx, add);
    cr->chall(mn, mx, add);
    add = 0;
  }

  void up() {
    assert(cl and cr);
    INFO T = *cl;
    T = T.merge(*cr);
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
  SEG(vector<ll> &_A, int cap) { build(_A, cap); }
  SEG(vector<ll> &_A) { build(_A, int(_A.size()) << 1); }
  void build(vector<ll> _A, int cap) {
    A.resize(cap);
    int n = _A.size();
    A[0] = build(0, n - 1, _A);
  }
  INFO &alloc(int l, int r, INFO *L = nullptr, INFO *R = nullptr) {
    assert(ptr < A.size());
    A[ptr] = INFO(l, r, L, R);
    return A[ptr++];
  }
  INFO &build(int l, int r, vector<ll> &_A) {
    if (l == r) {
      INFO &rt = alloc(l, r);
      rt.set(_A[l]);
      return rt;
    }
    int m = l + r >> 1;
    INFO &cl = build(l, m, _A);
    INFO &cr = build(m + 1, r, _A);
    INFO &rt = alloc(l, r, &cl, &cr);
    rt.up();
    return rt;
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
    INFO cl = apply(*rt.cl, l, r, v, op);
    INFO cr = apply(*rt.cr, l, r, v, op);
    rt.up();
    return cl.merge(cr);
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
    add(*rt.cl, l, r, v);
    add(*rt.cr, l, r, v);
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
    chmax(*rt.cl, l, r, v);
    chmax(*rt.cr, l, r, v);
    rt.up();
  }
  INFO qry(INFO &rt, int l, int r) {
    if (rt.inner(l, r))
      return rt;
    if (rt.outer(l, r))
      return INFO();
    rt.down();
    INFO cl = qry(*rt.cl, l, r);
    INFO cr = qry(*rt.cr, l, r);
    rt.up();
    return cl.merge(cr);
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
