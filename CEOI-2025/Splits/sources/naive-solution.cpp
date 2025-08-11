#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
const ll mod=998244353;
ll n,m,poz[305];
vector<ll> perm[305];
bool good(vector<ll> a,vector<ll> b)
{
	for(int i=0;i<a.size();i++)
		poz[a[i]]=i;
	int cnt=0;
	for(int i=1;i<b.size();i++)
	{
		if(poz[b[i-1]]>poz[b[i]])
			cnt++;
		if(cnt>1)
			return 0;
	}
	return 1;
}
int solve(int N,int M,vector<vector<int>>& splits)
{
	n=N;
	m=M;
	for(int i=1;i<=m;i++)
		for(int j=1;j<=n;j++)
		{
			ll x=splits[i-1][j-1];
			perm[i].push_back(x);
		}
	vector<ll> v;
	for(int i=1;i<=n;i++)
		v.push_back(i);
	ll ans=0;
	do
	{
		bool ok=1;
		for(int i=1;i<=m;i++)
			if(!good(v,perm[i]))	
			{
				ok=0;
				break;
			}
		ans+=ok;
	}while(next_permutation(v.begin(),v.end()));
	return ans;
}
