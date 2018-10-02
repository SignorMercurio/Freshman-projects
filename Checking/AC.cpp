#include<bits/stdc++.h>
using namespace std;

const int maxn = 20;
int w[maxn], dp[maxn], n, m, ans;

bool dfs(int x)
{
	if (x > n) return 1;
	for (int i = 1; i <= x && i <= ans; ++i)
		if (dp[i] + w[x] <= m) {
			dp[i] += w[x];
			if(dfs(x+1)) return 1;
			dp[i] -= w[x];
		}
	return 0;
}

int main()
{
	cin >> n >> m;
	for (int i = 1; i <= n; ++i) cin >> w[i];
	sort(w+1, w+n+1, greater<int>());
	for (ans = 1; ans <= n; ++ans) {
		memset(dp, 0, sizeof(dp));
		if (dfs(1)) break;
	}
	cout << ans << endl;
	return 0;
}
