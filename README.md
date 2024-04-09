# 线性基
## 板题 P3812 【模板】线性基
### 思路
首先得会[高斯消元](https://github.com/yabnto/Gaussian-elimination/tree/main)，然后我们从高斯消元的阴影中走出来，顺便带走我们的增广矩阵，然后把 $b$ 砍了（$b$ 是什么？建议你看看[高斯消元](https://github.com/yabnto/Gaussian-elimination/tree/main)），此时我们会得到一个矩阵，然后把它放到异或意义上，每个数就只会是 $0$ 和 $1$ 了，而我们要求的是异或最大值，所以我们可以用一个一个的数的二进制来写出一个矩阵，此时把行变换也扯出来，因为我们也还要用，同样的，我们还是要将其削成一个每一行的主元为 $1$ 的矩阵，因为我们如果两个同一列都为 $1$ 异或起来后就没意义了，但是每一行主元后面的不用动，因为我们又不求解，削出来后，我们就成功地得到了答案，因为此时每一行的该有的主元都有了啊，那么异或起来能保证是最大值，这个理解不了的可以重学二进制。

那到底该怎么做呢？首先因为一个数不会太大，所以我们没有必要用一个数组去存，直接有一个数来存即可（额，好像本来就是数，其实就是装压），然后我们需要一个桶，第 $i$ 行表示二进制里第 $i$ 位为 $1$ 的数。

如何将其塞入基里呢？我们可以打个比喻，一个数要塞入基里，就相当于你要从缅北南部跑往北部，再跑回中国，你会依此经过每一个地点，而每一个地点上有一个恶霸，他是第一个来并且拥有腰子的（这里就是二进制上的一位为 $1$ 的数，注意是第一个），那么他他就会噶了你的腰子，但如果你连腰子都没有，那他就会算了，放你过去，然后你是第一个有脑子的并且第一个到了一的地方的人，那么你就会成为这里的恶霸，~~然后专吃脑子~~，如果你成功的跑回了中国，但是你却已经一无所有了，这是很正常的，这只能证明你与缅北的恶霸是线性相关的（什么，线性相关都不知道，你要不去看看[高斯消元](https://github.com/yabnto/Gaussian-elimination/tree/main)吧，里面有解释的链接）。因为二进制是位权越大，贡献就越大，所以我们可以从位权最大的开始枚举，然后一点一点往里塞。

### code
```cpp
#include <iostream>

using namespace std;

const int MaxN = 64;

long long a[MaxN], N_M[MaxN], n, ans; // N_M 就是缅北的英文首字母

int main() {
  cin >> n;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
    for (long long j = 63; ~j; j--) {
      if (!(a[i] & (1ll << j))) {
        continue;
      }
      if (N_M[j] & (1ll << j)) {
        a[i] ^= N_M[j];
      } else {
        N_M[j] = a[i];  // 古希腊掌管腰子的神
        break;
      }
    }
  }
  for (int i = 63; ~i; i--) {
    ans = max(ans, (ans ^ N_M[i]));
  }
  cout << ans << endl;
  return 0;
}
```
## P3292 [SCOI2016] 幸运数字
### 思路
至此，你已经迈出了坚实的一小步，然后此时我们又要引入一些新东西了，现在我们将上一题改一下：不仅要你求异或最大值，还要你求一个方案。

所以除了我们现有的 $N_M$（接下来改名为 $b$），我们需要一个数组来维护我们原来的数，因为 $b$ 里所存的都是被削了的数，现在又要引入一个新东西，我们需要让两个基合并该怎么办呢？很显然会有一个方法就是从一个基中取出所有的数然后插入到另一个基中，然后你会觉得很傻逼，所以你想到了一个很睿智的做法，启发式合并，可是却完全没必要，因为基又不大，而且小的基插入大的基会被小的次数还会更多，所以其实直接插就行了，至于为啥不能直接合并，很简单，你存的都是被削了的，你如果直接插入，那么你还会有一堆与其有关系的数要改，不如直接插入。

同理我们会发现我们做不了删除，可是如果我们硬是要插入，我们可以悄无声息的给它踹下来，此时我们有需要一个新数组 $s$（嗯，就是 $abs$），这个指的是对于它相对应的 $b$，是由哪些 $a$ 异或成的，因为你再替换的过程中，你需要将与你替换掉的数相关的所有数都进行一个变换，如果枚举到一个数，还要去枚举所有与其有关的数，太不划算了，不如在再插入时就解决掉，现在假设我们要替换的数为 $v$，与其相关的数我们就需要枚举一边基然后算出来，这里就直接把它弄出来吧：$a_i,a_j,a_k$，好，我们可以通过一些规则指定一个倒霉蛋，然后把它踹下来，这里就指定 $a_i$，那替换对基有什么影响呢？首先原来 $a_i$ 的位置一定要被换，可这是最后一步，我们先需要找到所有的与 $a_i$ 有关的基里的数，因为 $a_i$ 马上就要消失了，因为是替换，所以 $b$ 不会受影响，所以只要考虑 $s$，然后我们知道：

$v = a_i \ xor \  a_j  \ xor  \ a_k$

所以

$a_i = v \ xor  \ a_j  \ xor  \ a_k$

那么现在要将 $a_i$ 消了，所以先将与其有关的 $s$ 异或上第 $i$ 位的位权（这个有问题的话，建议重学二进制），然后将 $a_i$ 的新的表示方式给加入，所以将新的表示方式给与其有关的 $s$ 异或上即可（当然，同上）。

这里给出打包好了的基代码（$p$ 就是用来辅助倒霉规则的）：

```cpp
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
```

线性基已经就绪，我们可以来考虑这道题了，这道题由于是树，所以两点只有一条路径，所以直接暴力找路径然后线性基维护即可，这样子的话大概会多一个 $log$ ~~可大丈夫怎么能被一个小小的 $log$ 给阻挡呢？~~，所以我们考虑先预处理，我们可以把每个点到根节点的线性基求出，然后再询问时 $LCA$ 求最近公共祖先，把最近公共祖先上面的权值删了即可，最后合并求值，注意，我在维护每个点到根节点的线性基时，对于每个会被削完的权，我们需要替换，而被替换的得是深度最低的（从根节点开始），这样就能保证，再删最近公共祖先后还是最大的。(整体还是线性基)

