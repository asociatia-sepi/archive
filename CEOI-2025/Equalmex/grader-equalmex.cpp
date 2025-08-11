#include <bits/stdc++.h>

std::vector<int> solve(int n, std::vector<int>& v, int q, std::vector<std::pair<int,int>>& queries);

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(0);
	int n,q;
	std::cin>>n>>q;
	std::vector<int> v;
	for(int i=0;i<n;i++)
	{
		int x;
		std::cin>>x;
		v.push_back(x);
	}
	std::vector<std::pair<int,int>> queries;
	for(int i=0;i<q;i++)
	{
		int l,r;
		std::cin>>l>>r;
		l--;
		r--;
		queries.push_back(std::make_pair(l,r));
	}
	std::vector<int> res=solve(n,v,q,queries);
	for(int i=0;i<(int)res.size();i++)
		std::cout<<res[i]<<'\n';
	return 0;
}
