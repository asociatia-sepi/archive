#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
const ll mod=998244353;
ll penalty[305][305],poz[305];
ll n,m;
vector<ll> perm[305];
ll good(vector<ll> a,vector<ll> b)
{
	for(int i=0;i<a.size();i++)
		poz[a[i]]=i;
	int cnt=0;
	for(int i=1;i<b.size();i++)
	{
		if(poz[b[i-1]]>poz[b[i]])
			cnt++;
	}
	return cnt;
}
ll dp[(1<<20)][25];
ll allpen[(1<<20)][25];
int solve(int N, int M, vector<vector<int>>& splits)
{
	n=N;
	m=M;
	for(int i=1;i<=m;i++)
		for(int j=1;j<=n;j++)
		{
			ll x=splits[i-1][j-1];
			perm[i].push_back(x);
		}
	for(int z=1;z<=m;z++)
		for(int i=1;i<n;i++)
			penalty[perm[z][i]][perm[z][i-1]]++;
	ll ans=0;
	for(int i=1;i<=m;i++)
	{
		bool ok=1;
		int suma=0;
		for(int j=1;j<=m;j++)
		{
			ll cnt=good(perm[i],perm[j]);
			suma+=cnt;
			if(cnt>1)
				ok=0;
		}
		if(ok)
			ans++;
		else if(suma==m)
			ans--;
	}
	if(ans<0)
		ans+=mod;
	dp[0][0]=1;
	for(int mask=0;mask<(1<<n);mask++)
		for(int nxt=1;nxt<=n;nxt++)
			if(!((mask>>(nxt-1)&1)))
			{
				for(int i=1;i<=n;i++)
					if((mask>>(i-1))&1)
						allpen[mask][nxt]+=penalty[i][nxt];
			}
	for(int mask=0;mask<(1<<n);mask++)
		for(int cnt=0;cnt<=m;cnt++)
			if(dp[mask][cnt]!=0)
			{
				for(int nxt=1;nxt<=n;nxt++)
					if(!((mask>>(nxt-1))&1))
					{
						int newcnt=cnt+allpen[mask][nxt];
						if(newcnt<=m)
						{
							dp[mask+(1<<(nxt-1))][newcnt]+=dp[mask][cnt];
							dp[mask+(1<<(nxt-1))][newcnt]%=mod;
						}
					}
			}
	ans=(ans+dp[(1<<n)-1][m])%mod;
	return ans;
}
