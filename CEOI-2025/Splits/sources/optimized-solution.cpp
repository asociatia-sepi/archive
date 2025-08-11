// Boaca Andrei - O(N^3*M)

#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
const ll mod=998244353;
ll n,m,penalty[305][305];
vector<ll> perm[305];
ll poz[305];
ll asuf[305][305],bsuf[305][305];
ll good(vector<ll> a,vector<ll> b) // numarul minim de split-uri pe care trb sa le dau in a ca sa obtin b
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
//ll dp[305][305][305];
map<int,ll> dp[305][305];
int solve(int N, int M, vector<vector<int>>& vec)
{
	n=N;
	m=M;
	for(int i=1;i<=m;i++)
		for(int j=1;j<=n;j++)
		{
			ll x=vec[i-1][j-1];
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
		int in1=0;
		for(int j=1;j<=m;j++)
		{
			ll cnt=good(perm[i],perm[j]);
			suma+=cnt;
			if(j==1)
				in1=cnt;
			if(cnt>1)
				ok=0;
		}
		if(ok)
			ans++;
		else if(suma==m&&in1==1)
			ans--;
	}
	if(ans<0)
		ans+=mod;
	for(int p=1;p<n;p++)
	{
		vector<ll> a,b;
		a.push_back(0);
		b.push_back(0);
		for(int j=0;j<p;j++)
			a.push_back(perm[1][j]);
		for(int j=p;j<n;j++)
			b.push_back(perm[1][j]);
		for(int x=1;x<=n;x++)
		{
			asuf[x][(int)a.size()]=0;
			for(int i=(int)a.size()-1;i>=1;i--)
				asuf[x][i]=asuf[x][i+1]+penalty[x][a[i]];
			bsuf[x][(int)b.size()]=0;
			for(int i=(int)b.size()-1;i>=1;i--)
				bsuf[x][i]=bsuf[x][i+1]+penalty[x][b[i]];
		}
		for(int i=0;i<a.size();i++)
			for(int j=0;j<b.size();j++)
			{
				/*for(int cnt=0;cnt<=m;cnt++)
					dp[i][j][cnt]=0;*/
				dp[i][j].clear();
			}
		dp[0][0][0]=1;
		for(int i=0;i<a.size();i++)
			for(int j=0;j<b.size();j++)
			{
				for(auto [cnt,val]:dp[i][j])
				{
					if(val!=0)
					{
						if(i+1<a.size()&&(j!=0||i+2!=a.size()))
						{
							ll add=asuf[a[i+1]][i+2]+bsuf[a[i+1]][j+1];
							ll newcnt=add+cnt;
							if(newcnt<=m)
							{
								dp[i+1][j][newcnt]+=val;
								dp[i+1][j][newcnt]%=mod;
							}
						}
						if(j+1<b.size())
						{
							ll add=asuf[b[j+1]][i+1]+bsuf[b[j+1]][j+2];
							ll newcnt=add+cnt;
							if(newcnt<=m)
							{
								dp[i][j+1][newcnt]+=val;
								dp[i][j+1][newcnt]%=mod;
							}
						}
					}
				}
				}
		ans+=dp[(int)a.size()-1][(int)b.size()-1][m];
		ans%=mod;
	}
	return ans;
}
