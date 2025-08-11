#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
const ll mod=998244353;
ll n,m,penalty[305][305];
vector<ll> perm[305];
ll poz[305],poz1[305];
struct date
{
	ll nra,nrb,cnt;
};
vector<date> states[305];
ll asuf[305][305],bsuf[305][305];
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
struct node
{
	vector<pii> kids;
} emp;
vector<node> t;
vector<int> pref;
void add(vector<ll> v)
{
	int me=0;
	for(int x:v)
	{
		if(t[me].kids.empty()||t[me].kids.back().first!=x)
		{
			t[me].kids.push_back({x,(int)t.size()});
			t.push_back(emp);
		}
		me=t[me].kids.back().second;
	}
}
bool have[305];
void dfs(int nod,int who,int nra,int nrb,int cnt)
{
	vector<int> appear;
	for(int i=0;i<=n;i++)
		appear.push_back(0);
	for(pii p:t[nod].kids)
	{
		int x=p.first;
		appear[x]=1;
		int fiu=p.second;
		if(have[x])
			continue;
		if(who!=0)
		{
			if(nra<who&&x==perm[1][nra])
			{
				int newcnt=cnt;
				for(int j=1;j<=n;j++)
					if(!have[j])
						newcnt+=penalty[x][j];
				if(newcnt<=m)
				{
					pref.push_back(x);
					have[x]=1;
					dfs(fiu,who,nra+1,nrb,newcnt);
					have[x]=0;
					pref.pop_back();
				}
			}
			if(nrb<n-who&&x==perm[1][nrb+who])
			{
				int newcnt=cnt;
				for(int j=1;j<=n;j++)
					if(!have[j])
						newcnt+=penalty[x][j];
				if(newcnt<=m)
				{
					pref.push_back(x);
					have[x]=1;
					dfs(fiu,who,nra,nrb+1,newcnt);
					have[x]=0;
					pref.pop_back();
				}
			}
		}
		else
		{
			if(x==perm[1][pref.size()])
			{
				int newcnt=cnt;
				for(int j=1;j<=n;j++)
					if(!have[j])
						newcnt+=penalty[x][j];
				if(newcnt<=m)
				{
					pref.push_back(x);
					have[x]=1;
					dfs(fiu,0,0,0,newcnt);
					have[x]=0;
					pref.pop_back();
				}
			}
			else
			{
				int guesswho=poz1[x];
				int lga=pref.size();
				int lgb=1;
				int newcnt=cnt;
				for(int j=1;j<=n;j++)
					if(!have[j])
						newcnt+=penalty[x][j];
				if(newcnt<=m)
				{
					pref.push_back(x);
					have[x]=1;
					dfs(fiu,guesswho,lga,lgb,newcnt);
					have[x]=0;
					pref.pop_back();
				}
			}
		}
	}
	for(int x=1;x<=n;x++)
		if(!appear[x])
		{
			if(have[x])
				continue;
			if(who!=0)
			{
				if(nra<who&&x==perm[1][nra])
				{
					int newcnt=cnt;
					for(int j=1;j<=n;j++)
						if(!have[j])
							newcnt+=penalty[x][j];
					if(newcnt<=m)
						states[who].push_back({nra+1,nrb,newcnt});
				}
				if(nrb<n-who&&x==perm[1][nrb+who])
				{
					int newcnt=cnt;
					for(int j=1;j<=n;j++)
						if(!have[j])
							newcnt+=penalty[x][j];
					if(newcnt<=m)
						states[who].push_back({nra,nrb+1,newcnt});
				}
			}
			else
			{
				int guesswho=poz1[x];
				int lga=pref.size();
				int lgb=1;
				int newcnt=cnt;
				for(int j=1;j<=n;j++)
					if(!have[j])
						newcnt+=penalty[x][j];
				if(newcnt<=m)
					states[guesswho].push_back({lga,lgb,newcnt});
			}
		}
}
ll dp[305][305];
int solve(int N, int M, vector<vector<int>>& splits)
{
	t.push_back(emp);
	n=N;
	m=M;
	for(int i=1;i<=m;i++)
		for(int j=1;j<=n;j++)
		{
			ll x=splits[i-1][j-1];
			perm[i].push_back(x);
		}
	sort(perm+1,perm+m+1);
	for(int z=1;z<=m;z++)
		for(int i=1;i<n;i++)
			penalty[perm[z][i]][perm[z][i-1]]++;
	for(int i=0;i<n;i++)
		poz1[perm[1][i]]=i;
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
	}
	for(int z=1;z<=m;z++)
		add(perm[z]);
	dfs(0,0,0,0,0);
	
	for(int p=1;p<n;p++)
	{
		vector<ll> a,b;
		a.push_back(0);
		b.push_back(0);
		for(int i=0;i<p;i++)
			a.push_back(perm[1][i]);
		for(int i=p;i<n;i++)
			b.push_back(perm[1][i]);
		for(int x=1;x<=n;x++)
		{
			asuf[x][(int)a.size()]=0;
			for(int i=(int)a.size()-1;i>=1;i--)
				asuf[x][i]=asuf[x][i+1]+penalty[x][a[i]];
			bsuf[x][(int)b.size()]=0;
			for(int i=(int)b.size()-1;i>=1;i--)
				bsuf[x][i]=bsuf[x][i+1]+penalty[x][b[i]];
		}
		for(int i=0;i<=a.size();i++)
			for(int j=0;j<=b.size();j++)
				dp[i][j]=0;
		dp[a.size()][b.size()]=1;
		for(int i=a.size();i>=1;i--)
			for(int j=b.size();j>=1;j--)
			{
				if(i<a.size()&&asuf[a[i]][i+1]==0&&bsuf[a[i]][j]==0)
				{
					dp[i][j]+=dp[i+1][j];
					dp[i][j]%=mod;
				}
				if(j<b.size()&&asuf[b[j]][i]==0&&bsuf[b[j]][j+1]==0)
				{
					dp[i][j]+=dp[i][j+1];
					dp[i][j]%=mod;
				}
			}
		for(date me:states[p])
		{ 
			ll nra=me.nra,nrb=me.nrb;
			ans=(ans+dp[nra+1][nrb+1])%mod;
		}
	}
	return ans;
}
