#include <bits/stdc++.h>
#pragma GCC optimize ("Ofast")
#pragma GCC optimize ("unroll-loops")
using namespace std;
int n,q,v[1000005];
struct qrr
{
	int l,r,mex,ind;
} safe[1000005];
vector<qrr> aux;
vector<qrr> myq[1000005];
int sol[1000005];
int arb[4*1000005];
void build(int nod,int st,int dr)
{
	arb[nod]=0;
	if(st==dr)
		return;
	int mij=(st+dr)/2;
	build(nod*2,st,mij);
	build(nod*2+1,mij+1,dr);
}
void update(int nod,int st,int dr,int poz,int val)
{
	if(st==dr)
	{
		arb[nod]=val;
		return;
	}
	int mij=(st+dr)/2;
	if(poz<=mij)
		update(nod*2,st,mij,poz,val);
	else
		update(nod*2+1,mij+1,dr,poz,val);
	arb[nod]=min(arb[nod*2],arb[nod*2+1]);
}
int getmex(int nod,int st,int dr,int valmin)
{
	if(arb[nod]>=valmin)
		return dr;
	if(st==dr)
		return -1;
	int mij=(st+dr)/2;
	if(arb[nod*2]>=valmin)
		return max(mij,getmex(nod*2+1,mij+1,dr,valmin));
	else
		return getmex(nod*2,st,mij,valmin);
}
struct date
{
	int mex,jump,cnt,last;
} dp[1000005];
int last[1000005];
void Solve(int st,int dr,vector<qrr> queries)
{
	if(queries.empty())
		return;
	if(st==dr)
	{
		for(qrr i:queries)
			sol[i.ind]=1;
		return;
	}
	int mij=(st+dr)/2;
	for(int i=0;i<=dr-mij;i++)
		last[i]=1e9;
	set<int> setik;
	int curmex=0;
	for(int i=mij+1;i<=dr;i++)
	{
		if(v[i]<curmex)
		{
			setik.erase(last[v[i]]);
			last[v[i]]=i;
			setik.insert(last[v[i]]);
		}
		else
		{
			last[v[i]]=i;
			while(last[curmex]<=i)
			{
				setik.insert(last[curmex]);
				curmex++;
			}
		}
		dp[i].mex=curmex;
		if(curmex==0)
			dp[i].jump=i;
		else
			dp[i].jump=*setik.begin();
		int prv=dp[i].jump-1;
		if(prv<mij+1||dp[prv].mex!=curmex)
		{
			dp[i].cnt=1;
			dp[i].last=dp[i].jump;
		}
		else
		{
			dp[i].cnt=dp[prv].cnt+1;
			dp[i].last=dp[prv].last;
		}
	}
	for(int i=0;i<=mij-st+1;i++)
		last[i]=0;
	setik.clear();
	curmex=0;
	for(int i=mij;i>=st;i--)
	{
		if(v[i]<curmex)
		{
			setik.erase(last[v[i]]);
			last[v[i]]=i;
			setik.insert(last[v[i]]);
		}
		else
		{
			last[v[i]]=i;
			while(last[curmex]>=i)
			{
				setik.insert(last[curmex]);
				curmex++;
			}
		}
		dp[i].mex=curmex;
		if(curmex==0)
			dp[i].jump=i;
		else
			dp[i].jump=*prev(setik.end());
		int prv=dp[i].jump+1;
		if(prv>mij||dp[prv].mex!=curmex)
		{
			dp[i].cnt=1;
			dp[i].last=dp[i].jump;
		}
		else
		{
			dp[i].cnt=dp[prv].cnt+1;
			dp[i].last=dp[prv].last;
		}
	}
	vector<qrr> lft,rgt;
	for(qrr p:queries)
	{
		if(p.r<=mij)
		{
			lft.push_back(p);
			continue;
		}
		if(p.l>mij)
		{
			rgt.push_back(p);
			continue;
		}
		int l=p.l,r=p.r;
		int l1,r1;
		if(dp[l].mex!=p.mex&&dp[r].mex!=p.mex)
		{
			sol[p.ind]=1;
			continue;
		}
		if(dp[l].mex==p.mex)
		{
			sol[p.ind]+=dp[l].cnt;
			l1=dp[l].last+1;
		}
		else
			l1=l;
		if(dp[r].mex==p.mex)
		{
			sol[p.ind]+=dp[r].cnt;
			r1=dp[r].last-1;
		}
		else
			r1=r;
		if(l1<=r1)
			aux.push_back({l1,r1,p.mex,p.ind});
	}
	Solve(st,mij,lft);
	Solve(mij+1,dr,rgt);
}
vector<int> solve(int N, vector<int>& V, int Q, vector<pair<int,int>>& QUERIES)
{
	n=N;
	q=Q;
	for(int i=1;i<=n;i++)
	{
		//cin>>v[i];
		v[i]=V[i-1];
		v[i]--;
	}
	for(int i=1;i<=n;i++)
		if(v[i]>=n)
			v[i]=n;
	for(int i=1;i<=q;i++)
	{
		int l,r;
		l=QUERIES[i-1].first,r=QUERIES[i-1].second;
		l++;
		r++;
		myq[r].push_back({l,r,-1,i});
	}
	build(1,0,n);
	vector<qrr> queries;
	for(int i=1;i<=n;i++)
	{
		update(1,0,n,v[i],i);
		for(qrr& p:myq[i])
		{
			p.mex=getmex(1,0,n,p.l)+1;
			queries.push_back(p);
			safe[p.ind]=p;
		}
	}
	Solve(1,n,queries);
	for(int i=1;i<=n;i++)
		myq[i].clear();
	for(qrr p:aux)
		myq[p.r].push_back(p);
	build(1,0,n);
	for(int i=1;i<=n;i++)
	{
		update(1,0,n,v[i],i);
		for(qrr p:myq[i])
		{
			int mex=getmex(1,0,n,p.l)+1;
			if(mex==p.mex)
				sol[p.ind]++;
		}
	}
	vector<int> res;
	for(int i=1;i<=q;i++)
		res.push_back(sol[i]);
	return res;
}
