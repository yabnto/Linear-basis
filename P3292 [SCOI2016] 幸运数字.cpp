#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

const int MaxN = 2e4 + 10, MaxK = 65, MaxM = 16;

struct N_M {
  ll a[MaxK], p[MaxK], b[MaxK], s[MaxK], tot;

  N_M() {
    tot = -1;
    for (ll j = 63; ~j; j--) {
      a[j] = p[j] = b[j] = s[j] = 0;
    }
  }

  ll insert(ll x, ll k) {
    ll rs = 0, ux = x;
    for (ll j = 63; ~j; j--) {
      if (!(x & (1ll << j))) {
        continue;
      }
      if (b[j] & (1ll << j)) {
        x ^= b[j], rs ^= s[j];
      } else {
        b[j] = x, p[++tot] = k, a[tot] = ux, s[j] = (rs ^ (1ll << tot));
        return x;
      }
    }
    return 0;
  }

  void change(ll x, ll k) {
    ll ux = x, rs = 0;
    for (ll j = 63; ~j; j--) {
      if (!(x & (1ll << j))) {
        continue;
      }
      if (b[j] & (1ll << j)) {
        x ^= b[j], rs ^= s[j];
      }
    }
    ll mini = 64, minx = 1e9;
    for (ll j = 63; ~j; j--) {
      if (rs & (1ll << j)) {
        if (minx >= p[j]) {
          minx = p[j], mini = j;
        }
      }
    }
    if (mini != 64) {
      rs ^= (1ll << mini);
      for (ll j = 63; ~j; j--) {
        if (s[j] & (1ll << mini)) {
          s[j] ^= rs;
        }
      }
      a[mini] = ux, p[mini] = k;
    }
  }

  void rebuild(int k) {
    N_M res;
    for (ll i = tot; i >= 0; i--) {
      if (p[i] >= k) {
        res.insert(a[i], p[i]);
      }
    }
    swap(a, res.a), swap(b, res.b), swap(s, res.s), swap(p, res.p), tot = res.tot;
  }

  N_M merge(N_M x, N_M y) {
    N_M c;
    for (ll j = 63; ~j; j--) {
      if (x.a[j]) {
        c.insert(x.a[j], x.p[j]);
      }
      if (y.a[j]) {
        c.insert(y.a[j], y.p[j]);
      }
    }
    return c;
  }
} e[MaxN];

ll f[MaxN][MaxM], a[MaxN], d[MaxN], n, q;
vector<int> g[MaxN];

void DFS(int x, int fa, int sum, N_M cnt) {
  f[x][0] = (~fa ? fa : x);
  for (int i = 1; i < MaxM; i++) {
    f[x][i] = f[f[x][i - 1]][i - 1];
  }
  d[x] = sum;
  if (!cnt.insert(a[x], d[x])) {
    cnt.change(a[x], d[x]);
  }
  e[x] = cnt;
  for (int i : g[x]) {
    if (i == fa) continue;
    DFS(i, x, sum + 1, cnt);
  }
}

int Q(int u, int k) {
  for (int i = 0; i < MaxM; i++) {
    if (k & (1 << i)) {
      u = f[u][i];
    }
  }
  return u;
}

int find(int x, int y) {
  int md = min(d[x], d[y]);
  x = Q(x, d[x] - md), y = Q(y, d[y] - md);
  if (x == y) {
    return x;
  }
  for (int i = MaxM - 1; i >= 0; i--) {
    if (f[x][i] != f[y][i]) {
      x = f[x][i], y = f[y][i];
    }
  }
  return f[x][0];
}

int main() {
  ios::sync_with_stdio(0), cin.tie(0);
  cin >> n >> q;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  for (int i = 1, x, y; i < n; i++) {
    cin >> x >> y;
    g[x].push_back(y);
    g[y].push_back(x);
  }
  DFS(1, -1, 1, N_M());
  for (ll x, y; q; q--) {
    cin >> x >> y;
    if (x == y) {
      cout << a[x] << '\n';
      continue;
    }
    ll k = find(x, y), ans = 0;
    N_M xd = e[x], yd = e[y];
    xd.rebuild(d[k]), yd.rebuild(d[k]);
    N_M c = c.merge(xd, yd);
    for (ll j = 63; ~j; j--) {
      ans = max(ans, ans ^ c.b[j]);
    }
    cout << ans << '\n';
  }
  return 0;
}
