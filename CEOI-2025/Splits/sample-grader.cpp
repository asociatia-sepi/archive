#include <bits/stdc++.h>

int solve(int n,int m,std::vector<std::vector<int>>& splits);

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(0);
	int n,m;
	std::cin>>n>>m;
	std::vector<std::vector<int>> vec;
	for(int i=1;i<=m;i++)
	{
		std::vector<int> me;
		for(int j=1;j<=n;j++)
		{
			int x;
			std::cin>>x;
			me.push_back(x);
		}
		vec.push_back(me);
	}
	std::cout<<solve(n,m,vec)<<'\n';
	return 0;	
}
