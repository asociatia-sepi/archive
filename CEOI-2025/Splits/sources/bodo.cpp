#include <iostream> 
#include <vector> 
#include <set> 
  
using namespace std; 
  
const long long mod=998244353; 
  
vector<int> getInv(const vector<int> &p){ 
    int n=p.size() - 1; 
    vector<int> inv(n+1); 
    for(int i=1;i<=n;i++){ 
        inv[p[i]]=i; 
    } 
    return inv; 
} 
  
int splits(const vector<int> &p, const vector<int> &q){ 
    int n = p.size() - 1; 
    vector<int> inv = getInv(p); 
    int cnt = 0; 
    for(int i=1;i<n;i++){ 
        cnt += (inv[q[i]] > inv[q[i+1]]); 
    } 
    return cnt; 
} 
  
int whichSplit(const vector<int> &p, const vector<int> &q){ 
    int n = p.size() - 1; 
    vector<int> inv(n+1); 
    for(int i=1;i<=n;i++){ 
        inv[p[i]]=i; 
    } 
    int cnt = 0; 
    for(int i=1;i<n;i++){ 
        if(inv[q[i]] > inv[q[i+1]]){ 
            return i; 
        } 
    } 
    return 0; 
} 
  
int countCase1(const vector< vector<int> > &p){ 
    int n = p.size() - 1, m=p[0].size() - 1; 
    int ret = 0; 
    for(int i=1;i<=n;i++){ 
        int add = 1; 
        int sum = 0; 
        bool splits1 = false; 
        for(int j=1;j<=n;j++){ 
            int currSplits = splits(p[i], p[j]); 
            if(currSplits>1) 
                add = 0; 
            if(currSplits ==1 && j == 1){ 
                splits1 = true; 
            } 
            sum += currSplits; 
        } 
        if(sum == n && splits1) 
            ret -= 1; 
        ret += add; 
    } 
    return (ret + mod) % mod; 
} 
  
int dp[305][305][305]; 
  
int add(int a, int b){ 
    a+=b; 
    if (a>=mod) a-=mod; 
    return a; 
} 
  
int countCase2(const vector< vector<int> > &p){ 
    set<int> knownSplits; 
    int n = p.size() - 1; 
    int m = p[0].size() - 1; 
    //vector< vector<bool> > have(n+1,vector<bool>(m+1)); 
    vector< vector< int> > invs(n+1); 
    for(int i=1;i<=n;i++) 
        invs[i] = getInv(p[i]); 
    int res = 0; 
    for(int split=1; split<m; split++){ 
        dp[0][0][0] = 1; 
        for(int fromLeft=0; fromLeft<=split; fromLeft++){ 
            for(int fromRight=0;fromRight<=m-split;fromRight++){ 
                int leftInvs = 0, rightInvs = 0; 
                if(fromLeft == split && fromRight == 0){ 
                    /// not a real split, already counted 
                    continue; 
                } 
                for(int perm=2;perm<=n;perm++){ 
                    /// take one more from left side 
                    if(fromLeft < split){ 
                        int pos = invs[perm][p[1][fromLeft+1]]; 
                        if(pos<m){ 
                            int nextInv = invs[1][p[perm][pos+1]]; 
                            leftInvs += ( 
                                (nextInv<=fromLeft || (nextInv>split&&nextInv<=split+fromRight)) 
                            ); 
                        } 
                    }; 
                    /// take one more from right side 
                    if(split + fromRight < m){ 
                        int pos = invs[perm][p[1][split+fromRight+1]]; 
                        if(pos<m){ 
                            int nextInv = invs[1][p[perm][pos+1]]; 
                            rightInvs += ( 
                                (nextInv<=fromLeft || (nextInv>split&&nextInv<=split+fromRight)) 
                            ); 
                        } 
                    }; 
                } 
                for(int currentInversions = 0; currentInversions <= n; currentInversions++){ 
                    if(currentInversions + leftInvs < n && fromLeft < split){ 
                        dp[fromLeft+1][fromRight][currentInversions+leftInvs] = add(dp[fromLeft+1][fromRight][currentInversions+leftInvs], dp[fromLeft][fromRight][currentInversions]); 
                    } 
                    if(currentInversions + rightInvs < n && fromRight <= m - split){ 
                        dp[fromLeft][fromRight+1][currentInversions+rightInvs] = add(dp[fromLeft][fromRight+1][currentInversions+rightInvs], dp[fromLeft][fromRight][currentInversions]); 
                    } 
                } 
            } 
        } 
        res = add(res, dp[split][m-split][n-1]); 
        for(int fromLeft=0; fromLeft<=split; fromLeft++){ 
            for(int fromRight=0;fromRight<=m-split;fromRight++){ 
                for(int currentInversions = 0; currentInversions <= n; currentInversions++){ 
                    dp[fromLeft][fromRight][currentInversions] = 0; 
                } 
                } 
        } 
    } 
    return res; 
} 
  
int Solve(vector< vector<int> > p){ 
    return add(countCase1(p), countCase2(p)); 
} 
int solve(int N,int M,vector<vector<int>>& vec) 
{
    int n, m; 
    n=N;
    m=M;
    swap(n,m);
    vector< vector<int> > p(n+1, vector<int>(m+1)); 
    for(int i=1; i<=n; i++) 
        for(int j=1; j<=m; j++) 
            p[i][j]=vec[i-1][j-1]; 
    return Solve(p); 
} 
