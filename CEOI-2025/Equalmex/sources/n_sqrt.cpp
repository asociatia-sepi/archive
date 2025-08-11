// Boaca Andrei - O(N*sqrt(N))

#include <bits/stdc++.h>

using namespace std;
int n,q,v[1000005],sol[1000005];
int LIM,bucksize;
struct qrr
{
	int l,r,mex,ind;
};
vector<qrr> small[10005];
vector<qrr> myq[1000005];
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
	return getmex(nod*2,st,mij,valmin);
}
vector<int> app[1000005];
int batog[1000005],prefmax[1000005],prefbuck[1000005];
int getbuck(int poz)
{
	return poz/bucksize;
}
int buckmax(int b)
{
	int poz=min(n,bucksize*(b+1)-1);
	return prefmax[poz];
}
void batog_init()
{	
	for(int i=0;i<=n;i++)
	{
		batog[i]=n+1;
		prefmax[i]=n+1;
		prefbuck[i]=n+1;
		app[i].push_back(n+1);
	}
}
void batog_update(int poz,int val)
{
	batog[poz]=val;
	int b=getbuck(poz);
	for(int i=bucksize*b;i<bucksize*(b+1)&&i<=n;i++)
	{
		if(i==bucksize*b)
			prefmax[i]=batog[i];
		else
			prefmax[i]=max(prefmax[i-1],batog[i]);
	}
	for(int i=0;i<=getbuck(n);i++)
	{
		int x=buckmax(i);
		if(i==0)
			prefbuck[i]=x;
		else
			prefbuck[i]=max(prefbuck[i-1],x);
	}
}
int batog_query(int poz)
{
	int b=getbuck(poz);
	int val=prefmax[poz];
	if(b>0)
		val=max(val,prefbuck[b-1]);
	return val;
}
void bigmex()
{
	batog_init();
	for(int i=n;i>=1;i--)
	{
		app[v[i]].push_back(i);
		batog_update(v[i],i);
	}
	for(int i=1;i<=n;i++)
	{
		for(qrr p:myq[i])
		{
			int poz=batog_query(p.mex-1);
			if(poz<=p.r)
			{
				sol[p.ind]++;
				if(poz+1<=p.r)
					myq[poz+1].push_back(p);
			}
		}
		myq[i].clear();
		app[v[i]].pop_back();
		batog_update(v[i],app[v[i]].back());
	}
}
vector<int> muchii[1000005];
int par[1000005],radpar[1000005];
int f[1000005];
vector<int> stiva;
void dfs(int nod)
{
	stiva.push_back(nod);
	int lg=stiva.size();
	int poz=lg-1-bucksize;
	if(poz>=0)
		radpar[nod]=stiva[poz];
	else
		radpar[nod]=n+2;
	for(int i:muchii[nod])
		dfs(i);
	stiva.pop_back();
}
void Solve(int mex)
{
	for(int i=1;i<=n+2;i++)	
	{
		par[i]=0;
		muchii[i].clear();
	}
	par[n+2]=n+2;
	par[n+1]=n+2;
	muchii[n+2].push_back(n+1);
	for(int i=0;i<=n;i++)
		f[i]=0;
	int have=0;
	int j=0;
	for(int i=1;i<=n;i++)
	{
		f[v[i]]++;
		if(v[i]<mex&&f[v[i]]==1)
			have++;
		if(have==mex)
		{
			j=i;
			break;
		}
	}
	for(int i=1;i<=n;i++)
	{
		par[i]=j+1;
		muchii[j+1].push_back(i);
		f[v[i]]--;
		if(f[v[i]]==0&&v[i]<mex)
		{
			while(j<n&&f[v[i]]==0)
			{
				j++;
				f[v[j]]++;
			}
			if(f[v[i]]==0)
				j=n+1;
		}
	}
	dfs(n+2);
	for(qrr p:small[mex])
	{
		int l=p.l,r=p.r,ind=p.ind;
		while(l<=n)
		{
			int poz=radpar[l];
			if(poz<=r+1)
			{
				sol[ind]+=bucksize;
				l=poz;
			}
			else
				break;
		}
		while(par[l]<=r+1)
		{
			sol[ind]++;
			l=par[l];
		}
	}
}
vector<int> solve(int N, vector<int>& V, int Q, vector<pair<int,int>>& QUERIES)
{
	//cin>>n>>q;
	n=N;
	q=Q;
	for(int i=1;i<=n;i++)
	{
		//cin>>v[i];
		v[i]=V[i-1];
		v[i]--;
	}
	for(int i=1;i<=n;i++)
		if(v[i]>n)
			v[i]=n;
	LIM=sqrt(n);
	bucksize=LIM;
	for(int i=1;i<=q;i++)
	{
		int l=QUERIES[i-1].first,r=QUERIES[i-1].second;
		//cin>>l>>r;
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
			if(mex==0)
				sol[p.ind]=p.r-p.l+1;
			else
				queries.push_back(p);
		}
		myq[i].clear();
	}
	for(qrr p:queries)
	{
		if(p.mex>LIM)
			myq[p.l].push_back(p);
		else
			small[p.mex].push_back(p);	
	}
	bigmex();
	for(int i=1;i<=LIM;i++)
		Solve(i);
	vector<int> res;
	for(int i=1;i<=q;i++)
		res.push_back(sol[i]);
	return res;
}	
