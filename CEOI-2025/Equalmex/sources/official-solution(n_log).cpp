// Boaca Andrei - O(N*log(N))

#include <bits/stdc++.h>

using namespace std;
int n,v[600005],q,sol[600005];
struct qrr
{
	int l,r,mex,ind;
};
vector<qrr> myq[600005];
int arb[4*600005];
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
	return getmex(nod*2,st,mij,valmin);
}
int query(int nod,int st,int dr,int a,int b)
{
	if(st>=a&&dr<=b)
		return arb[nod];
	int rez=1e9;
	int mij=(st+dr)/2;
	if(a<=mij)
		rez=min(rez,query(nod*2,st,mij,a,b));
	if(b>mij)
		rez=min(rez,query(nod*2+1,mij+1,dr,a,b));
	return rez;
}
struct date
{
	int cnt,last,mex;
} dp[600005];
int f[600005];
void Solve(int st,int dr,vector<qrr> queries)
{
	if(queries.empty())
		return;
	if(st==dr)
	{
		for(qrr p:queries)
			sol[p.ind]++;
		return;
	}
	int mij=(st+dr)/2;
	for(int i=0;i<=dr-mij;i++)
		f[i]=0;
	int curmex=0;
	for(int i=mij+1;i<=dr;i++)
	{
		 f[v[i]]++;
		 while(f[curmex]>0)
			curmex++;
		dp[i].mex=curmex;
	}
	for(int i=0;i<=dr-mij;i++)
		f[i]=0;
	for(int i=mij+1;i<=dr;i++)
	{
		int l=i;
		int r=i;
		while(r<=dr&&dp[r].mex==dp[i].mex)
			r++;
		r--;
		if(dp[i].mex==0)
		{
			for(int j=l;j<=r;j++)
			{
				dp[j].cnt=j-l+1;
				dp[j].last=l;
			}
			i=r;
			continue;
		}
		int have=0;
		int poz=l;
		for(int j=l;j<=r;j++)
		{
			f[v[j]]++;
			if(f[v[j]]==1&&v[j]<dp[i].mex)
				have++;
			if(have==dp[i].mex)
			{
				while(poz<j)
				{
					if(v[poz]>=dp[i].mex)
					{
						f[v[poz]]--;
						poz++;
						continue;
					}
					if(f[v[poz]]==1)
						break;
					f[v[poz]]--;
					poz++;
				}
				if(poz-1>=l)
				{
					dp[j].cnt=dp[poz-1].cnt+1;
					dp[j].last=dp[poz-1].last;
				}
				else
				{
					dp[j].cnt=0;
					dp[j].last=j+1;
				}
			}
			else
			{
				dp[j].cnt=0;
				dp[j].last=j+1;
			}
		}
		for(int j=l;j<=r;j++)
			f[v[j]]=0;
		i=r;
	}
	for(int i=0;i<=mij-st+1;i++)
		f[i]=0;
	curmex=0;
	for(int i=mij;i>=st;i--)
	{
		f[v[i]]++;
		while(f[curmex]>0)
			curmex++;
		dp[i].mex=curmex;
	}
	for(int i=0;i<=mij-st+1;i++)
		f[i]=0;
	for(int i=mij;i>=st;i--)
	{
		int r=i;
		int l=i;
		while(l>=st&&dp[l].mex==dp[i].mex)
			l--;
		l++;
		if(dp[i].mex==0)
		{
			for(int j=r;j>=l;j--)
			{
				dp[j].cnt=r-j+1;
				dp[j].last=r;
			}
			i=l;
			continue;
		}
		int have=0;
		int poz=r;
		for(int j=r;j>=l;j--)
		{
			f[v[j]]++;
			if(f[v[j]]==1&&v[j]<dp[i].mex)
				have++;
			if(have==dp[i].mex)
			{
				while(poz>j)
				{
					if(v[poz]>=dp[i].mex)
					{
						f[v[poz]]--;
						poz--;
						continue;
					}
					if(f[v[poz]]==1)
						break;
					f[v[poz]]--;
					poz--;
				}
				if(poz+1<=r)
				{
					dp[j].cnt=dp[poz+1].cnt+1;
					dp[j].last=dp[poz+1].last;
				}
				else
				{
					dp[j].cnt=0;
					dp[j].last=j-1;
				}
			}
			else
			{
				dp[j].cnt=0;
				dp[j].last=j-1;
			}
		}
		for(int j=r;j>=l;j--)
			f[v[j]]=0;
		i=l;
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
		int l=p.l,r=p.r,ind=p.ind,mex=p.mex;
		int l2,r2;
		if(dp[l].mex==mex)
		{
			sol[ind]+=dp[l].cnt;
			l2=dp[l].last+1;
		}
		else
			l2=l;
		if(dp[r].mex==mex)
		{
			sol[ind]+=dp[r].cnt;
			r2=dp[r].last-1;
		}
		else
			r2=r;
		if(l2<=r2)
			myq[r2].push_back({l2,r2,mex,ind});
	}
	Solve(st,mij,lft);
	Solve(mij+1,dr,rgt);
}
void leftovers()
{
	build(1,0,n);
	for(int i=1;i<=n;i++)
	{
		update(1,0,n,v[i],i);
		for(qrr p:myq[i])
		{
			int poz=query(1,0,n,0,p.mex-1);
			if(poz>=p.l)
			{
				sol[p.ind]++;
				poz--;
				if(poz>=p.l)
					myq[poz].push_back(p);
			}
		}
		myq[i].clear();
	}
}
vector<int> solve(int N, vector<int>& V, int Q, vector<pair<int,int>>& QUERIES)
{
	n=N;
	q=Q;
	for(int i=1;i<=n;i++)
	{
		//cin>>v[i];
		v[i]=V[i-1];
		assert(v[i]<=400000);
		v[i]--;
	}
	for(int i=1;i<=n;i++)
		if(v[i]>=n)
			v[i]=n;
	for(int i=1;i<=q;i++)
	{
		int l=QUERIES[i-1].first,r=QUERIES[i-1].second;
		l++;
		r++;
		myq[r].push_back({l,r,-1,i});
	}
	vector<qrr> queries;
	build(1,0,n);
	for(int i=1;i<=n;i++)
	{
		update(1,0,n,v[i],i);
		for(qrr &p:myq[i])
		{
			int mex=getmex(1,0,n,p.l)+1;
			p.mex=mex;
			queries.push_back(p);
		}
	}
	for(int i=1;i<=n;i++)
		myq[i].clear();
	Solve(1,n,queries);
	for(int it=1;it<=3;it++)
		leftovers();
	vector<int> res;
	for(int i=1;i<=q;i++)
		res.push_back(sol[i]);
	return res;
}

