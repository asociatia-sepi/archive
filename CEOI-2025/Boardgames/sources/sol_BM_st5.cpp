#include "boardgames.h"

#define MAXN 100000

int aa, bb;
std::vector<int> edge[MAXN];
int t[MAXN];

struct node
{
    int a, b, l, mini, pos;
    node *leftC, *rightC;
};
node *root;

int val(node *p)
{
    return p->mini + p->l;
}

void upd(node *p)
{
    int lv = val(p->leftC), rv = val(p->rightC);
    p->mini = std::min(lv, rv);
    if (rv <= lv)
    {
        p->pos = (p->rightC)->pos;
    }
    else
    {
        p->pos = (p->leftC)->pos;
    }
}

node *build(int x, int y)
{
    node *p = new node;
    node *lc = nullptr;
    node *rc = nullptr;

    p->a = x;
    p->b = y;
    p->l = 0;
    if (x == y)
    {
        p->mini = t[x - 1];
        p->pos = x - 1;
        p->leftC = lc;
        p->rightC = rc;
    }
    else
    {
        lc = build(x, (x + y) / 2);
        rc = build((x + y) / 2 + 1, y);
        p->leftC = lc;
        p->rightC = rc;
        upd(p);
    }
    return p;
}

void lazy(node *p)
{
    p->leftC->l += p->l;
    p->rightC->l += p->l;
    p->l = 0;
}

void inc(node *p)
{
    if (aa <= p->a)
    {
        p->l++;
        return;
    }
    lazy(p);
    if (aa <= (p->a + p->b) / 2)
        inc(p->leftC);
    inc(p->rightC);
    upd(p);
}

node *findMin(node *p)
{
    if (aa <= p->a)
        return p;

    lazy(p);
    node *r = findMin(p->rightC);
    node *l = (aa <= (p->a + p->b) / 2 ? findMin(p->leftC) : r);
    upd(p);

    if (val(r) <= val(l))
        return r;
    return l;
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    std::vector<int> ans;
    ans.clear();

    for (int i = 0; i < M; i++)
    {
        edge[X[i]].push_back(Y[i]);
        t[Y[i]]--;
    }
    t[0] = 1;
    for (int i = 1; i < N; i++)
        t[i] += t[i - 1] + 1;

    root = build(1, N);

    int a = 0;
    while (a < N)
    {
        aa = a + 1;
        int pos = findMin(root)->pos;
        ans.push_back(pos - a + 1);
        for (int i = a; i <= pos; i++)
        {
            for (int j : edge[i])
            {
                aa = j + 1;
                inc(root);
            }
        }
        a = pos + 1;
    }

    return ans;
}
