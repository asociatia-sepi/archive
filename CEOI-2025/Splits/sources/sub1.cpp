#include <bits/stdc++.h>

using namespace std;
const long long mod=998244353;
long long pw(long long a,long long b)
{
	long long rez=1;
	while(b)
	{
		if(b&1)
			rez=(rez*a)%mod;
		b/=2;
		a=(a*a)%mod;
	}
	return rez;
}
int solve(int n,int m,vector<vector<int>>& splits)
{
	long long ans=pw(2,n);
	ans=(ans-n+mod)%mod;
	return ans;
}
