#include <bits/stdc++.h>

using namespace std;
typedef pair<int,int> pii;
int n,q,v[1000005],sol[1000005];
struct qrr
{
	int l,r,mex,ind;
};
struct date
{
	int l,r,ind;
};
vector<pii> bymex[1000005];
vector<qrr> qmex[1000005];
vector<int> intervs[1000005];
vector<qrr> myq[1000005];
vector<date> inter;
vector<qrr> queries;
int arb[4*1000005];
void buildmin(int nod,int st,int dr)
{
	arb[nod]=0;
	if(st==dr)
		return;
	int mij=(st+dr)/2;
	buildmin(nod*2,st,mij);
	buildmin(nod*2+1,mij+1,dr);
}
void updmin(int nod,int st,int dr,int poz,int val)
{
	if(st==dr)
	{
		arb[nod]=val;
		return;
	}
	int mij=(st+dr)/2;
	if(poz<=mij)
		updmin(nod*2,st,mij,poz,val);
	else
		updmin(nod*2+1,mij+1,dr,poz,val);
	arb[nod]=min(arb[nod*2],arb[nod*2+1]);
}
int querymin(int nod,int st,int dr,int a,int b)
{
	if(st>=a&&dr<=b)
		return arb[nod];
	int rez=n+1;
	int mij=(st+dr)/2;
	if(a<=mij)
		rez=min(rez,querymin(nod*2,st,mij,a,b));
	if(b>mij)
		rez=min(rez,querymin(nod*2+1,mij+1,dr,a,b));
	return rez;
}

void buildmax(int nod,int st,int dr)
{
	arb[nod]=n+1;
	if(st==dr)
		return;
	int mij=(st+dr)/2;
	buildmax(nod*2,st,mij);
	buildmax(nod*2+1,mij+1,dr);
}
void updmax(int nod,int st,int dr,int poz,int val)
{
	if(st==dr)
	{
		arb[nod]=val;
		return;
	}
	int mij=(st+dr)/2;
	if(poz<=mij)
		updmax(nod*2,st,mij,poz,val);
	else
		updmax(nod*2+1,mij+1,dr,poz,val);
	arb[nod]=max(arb[nod*2],arb[nod*2+1]);
}
int querymax(int nod,int st,int dr,int a,int b)
{
	if(st>=a&&dr<=b)
		return arb[nod];
	int rez=0;
	int mij=(st+dr)/2;
	if(a<=mij)
		rez=max(rez,querymax(nod*2,st,mij,a,b));
	if(b>mij)
		rez=max(rez,querymax(nod*2+1,mij+1,dr,a,b));
	return rez;
}
int getmex(int nod,int st,int dr,int valmin)
{
	if(arb[nod]>=valmin)
		return dr+1;
	if(st==dr)
		return 0;
	int mij=(st+dr)/2;
	if(arb[nod*2]>=valmin)
		return max(mij+1,getmex(nod*2+1,mij+1,dr,valmin));
	return getmex(nod*2,st,mij,valmin);
}
bool bysec(pii a, pii b)
{
	return a.second<b.second;
}
bool byl(date a, date b)
{
	return make_pair(a.l,a.r)<make_pair(b.l,b.r);
}
int lmax[1000005];
pii what[1000005];
int dp[25][1000005];
int best[1000005],prv[1000005];
void Solve()
{
	vector<int> mypoz;
	for(date me:inter)
	{
		mypoz.push_back(me.l);
		mypoz.push_back(me.r);
	}
	sort(mypoz.begin(),mypoz.end());
	mypoz.erase(unique(mypoz.begin(),mypoz.end()),mypoz.end());
	int p=0;
	for(int i:mypoz)
	{
		best[i]=inter.size()-1;
		prv[i]=p;
		p=i;
	}
	for(date me:inter)
	{
		what[me.ind]={me.l,me.r};
		best[prv[me.l]]=min(best[prv[me.l]],me.ind);
	}
	for(int i=(int)mypoz.size()-2;i>=0;i--)
		best[mypoz[i]]=min(best[mypoz[i]],best[mypoz[i+1]]);
	for(date me:inter)
	{
		dp[0][me.ind]=best[me.r];
		//assert(inter[best[me.r]].l>me.r);
		/*if(inter[best[me.r]].l<=me.r)
			cerr<<best[me.r]<<' '<<inter[best[me.r]].l<<' '<<me.r<<'\n';*/
	}
	for(int j=1;j<=20;j++)
		for(date me:inter)
		{
			dp[j][me.ind]=dp[j-1][dp[j-1][me.ind]];
		}
	lmax[0]=inter[0].l;
	for(int i=1;i<inter.size();i++)
		lmax[i]=max(lmax[i-1],inter[i].l);
	for(qrr me:queries)
	{
		int l=me.l;
		int r=me.r;
		int ind=me.ind;
		int poz=inter.size();
		int st=0;
		int dr=inter.size();
		dr--;
		while(st<=dr)
		{
			int mij=(st+dr)/2;
			if(lmax[mij]>=l)
			{
				poz=inter[mij].ind;
				dr=mij-1;
			}
			else
				st=mij+1;
		}  
		sol[ind]=1;
		for(int i=20;i>=0;i--)
			if(dp[i][poz]<inter.size()&&what[dp[i][poz]].second<=r)
			{
				//if(ind==3)
				//	cerr<<i<<' '<<dp[i][poz]<<' '<<what[dp[i][poz]].second<<'\n';
				sol[ind]+=(1<<i);
				poz=dp[i][poz];
			}
	}
}
vector<int> solve(int N, vector<int>& V, int Q, vector<pair<int,int>>& QUERIES)
{
	n=N;
	q=Q;
	for(int i=1;i<=n;i++)
		v[i]=V[i-1];
	for(int i=1;i<=q;i++)
	{
		int l=QUERIES[i-1].first,r=QUERIES[i-1].second;
		l++;
		r++;
		myq[r].push_back({l,r,0,i});
	}
	for(int i=1;i<=n;i++)
	{
		v[i]--;
		if(v[i]>=n)
			v[i]=n;
	}
	buildmin(1,0,n);
	for(int i=1;i<=n;i++)
	{
		updmin(1,0,n,v[i],i);
		int poz=querymin(1,0,n,0,v[i]);
		if(poz!=0)
			intervs[i].push_back(poz);
	}
	buildmax(1,0,n);
	for(int i=n;i>=1;i--)
	{
		updmax(1,0,n,v[i],i);
		int poz=querymax(1,0,n,0,v[i]);
		if(poz<=n)
			intervs[poz].push_back(i);
	}
	buildmin(1,0,n);
	for(int i=1;i<=n;i++)
	{
		updmin(1,0,n,v[i],i);
		for(int l:intervs[i])
		{
			int val=getmex(1,0,n,l);
			bymex[val].push_back({l,i});
		}
		for(qrr &me:myq[i])
		{
			int val=getmex(1,0,n,me.l);
			me.mex=val;
			if(val>0)
				qmex[val].push_back(me);
			else
				sol[me.ind]=me.r-me.l+1;
		}
	}
	for(int i=1;i<=n;i++)
		if(!qmex[i].empty())
		{
			inter.clear();
			queries=qmex[i];
			sort(bymex[i].begin(),bymex[i].end(),bysec);
			bymex[i].erase(unique(bymex[i].begin(),bymex[i].end()),bymex[i].end());
			bymex[i].push_back({n+1,n+1});
			int nr=0;
			for(pii p:bymex[i])
			{
				inter.push_back({p.first,p.second,nr});
				nr++;
			}
			Solve();
		}
	vector<int> res;
	for(int i=1;i<=q;i++)
		res.push_back(sol[i]);
	return res;
}
