#include <bits/stdc++.h>
using namespace std;






vector<pair<pair<float, float>, pair<float, float>>> give_edges(vector<vector<bool>> &B)
{
    int sz = 100;

    typedef struct
    {
        pair<float, float> a, b;
    } Edge;

    typedef struct
    {
        array<float, 4> v;
    } state;

    typedef struct
    {
        vector<Edge> edges;
        pair<float, float> botleft;

        int index;
    } cluster;

    function<float(double, double)> mini = [&](double a, double b) -> float
    {
        float aa = (float)a;
        float bb = (float)b;
        return (aa < bb ? aa : bb);
    };

    function<float(double, double)> maxi = [&](double a, double b) -> float
    {
        float aa = (float)a;
        float bb = (float)b;
        return (aa > bb ? aa : bb);
    };

    function<pair<float, float>(pair<float, float>, pair<float, float>)> minin = [&](pair<float, float> p1, pair<float, float> p2)
    {
        if (p1.first < p2.first)
            return p1;
        else if (p1.first > p2.first)
            return p2;
        if (p1.second > p2.second)
            return p1;
        return p2;
    };

    function<bool(pair<float, float> &, pair<float, float> &)> cmp2 = [&](pair<float, float> &a, pair<float, float> &b) -> bool
    {
        if (a.first == b.first)
            return a.second < b.second;
        else
            return a.first < b.first;
    };

    function<bool(pair<float, float> &, pair<float, float> &)> cmp = [&](pair<float, float> &a, pair<float, float> &b) -> bool
    {
        if (a.second == b.second)
            return a.first < b.first;
        else
            return a.second < b.second;
    };

    for(int i=0; i<sz; i++)
    {
        for(int j=0; j<sz; j++)
        {
            if(j == sz-1) B[i][j] = 0;
        }
    }

    state bb = {0, 0, 0, 0};
    vector<vector<state>> s(sz, vector<state>(sz, bb));
    vector<vector<float>> A;
    float Z = sz - 1;
    vector<vector<bool>> vis(sz, vector<bool>(sz, 0));
    vector<vector<bool>> vis2(sz, vector<bool>(sz, 0));
    vector<state> valid;

    valid.push_back({0, 0, 0, 0});
    valid.push_back({0, 1, 0, 1});
    valid.push_back({1, 0, 1, 0});
    valid.push_back({0, 1, 1, 0});
    valid.push_back({1, 0, 0, 1});
    valid.push_back({1, 0, 0, 0});
    valid.push_back({0, 1, 0, 0});
    valid.push_back({0, 0, 1, 0});
    valid.push_back({0, 0, 0, 1});

    function<void(float, float)> dfs = [&](float i, float j) -> void
    {
        if (vis[i][j])
            return;
        vis[i][j] = 1;

        if (j - 1 >= 0 && B[i][j - 1])
        {
            s[i][j].v[2] = 1;
            dfs(i, j - 1);
        }
        if (j + 1 <= Z && B[i][j + 1])
        {
            s[i][j].v[3] = 1;
            dfs(i, j + 1);
        }
        if (i - 1 >= 0 && B[i - 1][j])
        {
            s[i][j].v[0] = 1;
            dfs(i - 1, j);
        }
        if (i + 1 <= Z && B[i + 1][j])
        {
            s[i][j].v[1] = 1;
            dfs(i + 1, j);
        }
    };
    vector<pair<float, float>> vertices;
    function<void(float, float)> dfs2 = [&](float i, float j) -> void
    {
        if (vis2[i][j])
            return;

        vis2[i][j] = 1;
        for (float l = 0; l < 9; l++)
        {
            int check = 1;
            for (float k = 0; k < 4; k++)
            {
                if (valid[l].v[k] != s[i][j].v[k])
                {
                    check = 0;
                    break;
                }
            }
            if (check)
            {

                if (l == 0)
                {
                    vertices.push_back({maxi(i, (float)0), maxi(j, (float)0)});
                    vertices.push_back({maxi(i, (float)0), mini(j + 0.998, (float)Z + 1)});
                    vertices.push_back({mini(i + 0.998, (float)Z + 1), maxi(j, (float)0)});
                    vertices.push_back({mini(i + 0.998, (float)Z + 1), mini(j + 0.998, (float)Z + 1)});
                }
                if (l == 1)
                {
                    vertices.push_back({maxi(i, (float)0), maxi(j, (float)0)});
                }
                if (l == 2)
                {
                    vertices.push_back({mini(i + 0.998, (float)Z + 1), mini(j + 0.998, (float)Z + 1)});
                }
                if (l == 3)
                {
                    vertices.push_back({maxi(i, (float)0), mini(j + 0.998, (float)Z + 1)});
                }
                if (l == 4)
                {
                    vertices.push_back({mini(i + 0.998, (float)Z + 1), maxi(j, (float)0)});
                }
                if (l == 5)
                {
                    vertices.push_back({mini(i + 0.998, (float)Z + 1), maxi(j, (float)0)});
                    vertices.push_back({mini(i + 0.998, (float)Z + 1), mini(j + 0.998, (float)Z + 1)});
                }
                if (l == 6)
                {
                    vertices.push_back({maxi(i, (float)0), maxi(j, (float)0)});
                    vertices.push_back({maxi(i, (float)0), mini(j + 0.998, (float)Z + 1)});
                }
                if (l == 7)
                {
                    vertices.push_back({maxi(i, (float)0), mini(j + 0.998, (float)Z + 1)});
                    vertices.push_back({mini(i + 0.998, (float)Z + 1), mini(j + 0.998, (float)Z + 1)});
                }
                if (l == 8)
                {
                    vertices.push_back({maxi(i, (float)0), maxi(j, (float)0)});
                    vertices.push_back({mini(i + 0.998, (float)Z + 1), maxi(j, (float)0)});
                }
            }
        }
        if (i - 1 >= 0 && j - 1 >= 0 && B[i - 1][j] && B[i][j - 1] && !B[i - 1][j - 1])
            vertices.push_back({maxi(i - 0, (float)0), maxi(j - 0, (float)0)});
        if (i - 1 >= 0 && j + 1 <= Z && B[i - 1][j] && B[i][j + 1] && !B[i - 1][j + 1])
            vertices.push_back({maxi(i - 0, (float)0), mini(j + 0.998, (float)Z + 1)});
        if (i + 1 <= Z && j - 1 >= 0 && B[i + 1][j] && B[i][j - 1] && !B[i + 1][j - 1])
            vertices.push_back({mini(i + 0.998, (float)Z + 1), maxi(j - 0, (float)0)});
        if (i + 1 <= Z && j + 1 <= Z && B[i + 1][j] && B[i][j + 1] && !B[i + 1][j + 1])
            vertices.push_back({mini(i + 0.998, (float)Z + 1), mini(j + 0.998, (float)Z + 1)});
        if (j - 1 >= 0 && B[i][j - 1])
            dfs2(i, j - 1);
        if (j + 1 <= Z && B[i][j + 1])
            dfs2(i, j + 1);
        if (i - 1 >= 0 && B[i - 1][j])
            dfs2(i - 1, j);
        if (i + 1 <= Z && B[i + 1][j])
            dfs2(i + 1, j);
    };
    map<pair<float, float>, float> mpp;
    vector<vector<Edge>> clusters;

    function<void(vector<vector<bool>> &)> connector = [&](vector<vector<bool>> &B) -> void
    {
        for (float i = 0; i <= Z; i++)
        {
            for (float j = 0; j <= Z; j++)
            {
                if (B[i][j])
                {
                    dfs(i, j);
                    dfs2(i, j);
                }
                sort(vertices.begin(), vertices.end());
                float n = vertices.size();
                if (n == 0)
                    continue;

                vector<Edge> temp;
                for (float k = 0; k < n; k += 2)
                {
                    temp.push_back({vertices[k], vertices[k + 1]});
                }
                sort(vertices.begin(), vertices.end(), cmp);
                for (float k = 0; k < n; k += 2)
                {
                    temp.push_back({vertices[k], vertices[k + 1]});
                }
                clusters.push_back(temp);
                vertices.clear();
            }
        }
    };
    connector(B);

    vector<cluster> group;

    map<int, cluster> m;
    set<pair<float, float>> bottomleft;
    set<pair<float, float>> bottomright;
    set<pair<float, float>> vert;
    float ymaxi = -1;
    float xmaxi = -1;
    float xmini = 1e5;
    for (float i = 0; i < clusters.size(); i++)
    {

        cluster c;
        c.index = i;
        float minix = 1e5;
        float cory = -1;
        float ymaxiy = -1;
        for (float j = 0; j < clusters[i].size(); j++)
        {
            c.edges.push_back(clusters[i][j]);

            vert.insert(clusters[i][j].a);
            vert.insert(clusters[i][j].b);

            if (clusters[i][j].a.second > ymaxiy)
                ymaxiy = clusters[i][j].a.second;
            if (clusters[i][j].b.second > ymaxiy)
                ymaxiy = clusters[i][j].b.second;

            if (clusters[i][j].a.first < minix)
            {
                minix = clusters[i][j].a.first;
                cory = clusters[i][j].a.second;
            }
            else if (clusters[i][j].a.first == minix)
            {
                if (cory < clusters[i][j].a.second)
                {
                    cory = clusters[i][j].a.second;
                }
            }
            if (clusters[i][j].b.first < minix)
            {
                minix = clusters[i][j].b.first;
                cory = clusters[i][j].b.second;
            }
            else if (clusters[i][j].b.first == minix)
            {
                if (cory < clusters[i][j].b.second)
                {
                    cory = clusters[i][j].b.second;
                }
            }
        }
        // cout<<minix<<" "<<cory<<endl;
        c.botleft.first = minix;
        c.botleft.second = cory;
        bottomleft.insert(c.botleft);
        mpp[c.botleft] = ymaxiy;
        group.push_back(c);
        m[i] = c;
    }

    for (auto botleft : bottomleft)
    {

        float boty = botleft.second;
        float botx = botleft.first;

        if (botx < xmini)
            xmini = botx;
        if (boty > ymaxi)
            ymaxi = boty;
        if (botx > xmaxi)
            xmaxi = botx;
    }

    float sf = 100;

    float tub = 0.002;
    ymaxi = Z;
    vert.insert({0, Z + 1});
    vert.insert({0, Z + 1 - tub});
    vert.insert({Z + 1, Z + 1});
    vert.insert({Z + 1, Z + 1 - tub});

    for (auto botleft : bottomleft)
    {

        float boty = botleft.second;

        int yy = (boty + 1);
        float botx = botleft.first;
        int xx = (botx + 0.998);
        int check = 0;
        for (int j = yy; j <= Z; j++)
        {
            if (B[xx][j] == 1)
            {

                check = 1;
                float y = j;

                if (vert.find({botx, y}) == vert.end())
                {
                    vert.insert({botx, y});
                }
                else
                    vert.erase(vert.find({botx, y}));
                if (vert.find({botx + tub, y}) == vert.end())
                {
                    vert.insert({botx + tub, y});
                }
                else
                    vert.erase(vert.find({botx + tub, y}));
                vert.erase({botx, boty});
                vert.insert({botx + tub, boty});
                break;
            }
        }

        if (!check)
        {

            vert.erase({botx, boty});

            vert.insert({mini(botx + tub, Z + 1), boty});

            if (vert.find({mini(botx + tub, Z + 1), Z + 1 - tub}) == vert.end())
            {
                vert.insert({mini(botx + tub, Z + 1), Z + 1 - tub});
            }
            else
                vert.erase({mini(botx + tub, Z + 1), Z + 1 - tub});

            if (vert.find({mini(botx, Z + 1), Z + 1 - tub}) == vert.end())
            {
                vert.insert({mini(botx, Z + 1), Z + 1 - tub});
            }
            else
                vert.erase({mini(botx, Z + 1), Z + 1 - tub});
        }
    }

    vis.assign(Z + 1, vector<bool>(Z + 1, 0));
    function<void(int, int)> dfs3 = [&](int i, int j) -> void
    {
        if (i < 0 || i > Z || j < 0 || j > Z)
            return;
        if (B[i][j] || vis[i][j])
            return;
        vis[i][j] = 1;
        dfs3(i - 1, j);
        dfs3(i - 1, j - 1);
        dfs3(i, j + 1);
        dfs3(i + 1, j + 1);
        dfs3(i + 1, j);
        dfs3(i + 1, j - 1);
        dfs3(i, j - 1);
        dfs3(i - 1, j + 1);
    };
    function<pair<float, float>(int, int)> dfs4 = [&](int i, int j) -> pair<float, float>
    {
        if (i == -1 || i == Z + 1 || j == -1 || j == Z + 1 || B[i][j] || vis[i][j])
            return {INT_MAX, INT_MIN};
        vis[i][j] = 1;
        pair<int, int> p = {i, j};
        p = minin(dfs4(i - 1, j), p);
        p = minin(dfs4(i, j + 1), p);
        p = minin(dfs4(i + 1, j), p);
        p = minin(dfs4(i, j - 1), p);
        p = minin(dfs4(i - 1, j - 1), p);
        p = minin(dfs4(i + 1, j + 1), p);
        p = minin(dfs4(i + 1, j - 1), p);
        p = minin(dfs4(i - 1, j + 1), p);

        return p;
    };

    for (int i = 0; i <= Z; i++)
    {
        for (int j = 0; j <= Z; j++)
        {
            if (i == 0 || i == Z || j == 0 || j == Z)
                dfs3(i, j);
        }
    }

    int holes = 1;

    for (int i = 0; i <= Z; i++)
    {
        for (int j = 0; j <= Z; j++)
        {
            pair<float, float> p = dfs4(i, j);

            if (p.first != INT_MAX || p.second != INT_MIN)
            {

                int check = 0;
                for (int k = p.second + 1; k <= Z; k++)
                {
                    if (!B[i][k] || !B[maxi(i - 1, 0)][k])
                    {
                        check = 1;
                        if (vert.find({p.first - 0.002, p.second + 1}) != vert.end())
                            vert.erase({p.first - 0.002, p.second + 1});

                        if (vert.find({p.first - 0.002, (float)k - 0.002}) == vert.end())
                            vert.insert({p.first - 0.002, (float)k - 0.002});
                        else
                            vert.erase({p.first - 0.002, (float)k - 0.002});

                        if (vert.find({p.first - 0.002 + tub, p.second + 1}) == vert.end())
                            vert.insert({p.first - 0.002 + tub, p.second + 1});
                        else
                            vert.erase({p.first - 0.002 + tub, p.second + 1});

                        if (vert.find({p.first - 0.002 + tub, k - 0.002}) == vert.end())
                            vert.insert({p.first - 0.002 + tub, k - 0.002});
                        else
                            vert.erase({p.first - 0.002 + tub, k - 0.002});
                        break;
                    }
                }
                if (!check)
                {

                    vert.erase({p.first - 0.002, p.second + 1});
                    if (vert.find({p.first - 0.002, Z + 1}) == vert.end())
                        vert.insert({p.first - 0.002, Z + 1});
                    else
                        vert.erase({p.first - 0.002, Z + 1});
                    if (vert.find({p.first - 0.002 + tub, p.second + 1}) == vert.end())
                        vert.insert({p.first - 0.002 + tub, p.second + 1});
                    else
                        vert.erase({p.first - 0.002 + tub, p.second + 1});
                    if (vert.find({p.first - 0.002 + tub, Z + 1}) == vert.end())
                        vert.insert({p.first - 0.002 + tub, Z + 1});
                    else
                        vert.erase({p.first - 0.002 + tub, Z + 1});
                }
            }
        }
    }

    vector<pair<float, float>> answer;

    for (auto vv : vert)
    {

        answer.push_back({vv.first * 100, vv.second * 100});
    }

    int cnt = 0;
    pair<float, float> prev;

    sort(answer.begin(), answer.end(), cmp2);

    int gg = vert.size();

    vector<pair<pair<float, float>, pair<float, float>>> edges;

    for (int i = 0; i < gg; i += 2)
    {
        
        edges.push_back({{answer[i].first, answer[i].second}, {answer[i + 1].first, answer[i + 1].second}});
    }
    sort(answer.begin(), answer.end(), cmp);
    for (int i = 0; i < gg; i += 2)
    {
       
        edges.push_back({{answer[i].first, answer[i].second}, {answer[i + 1].first, answer[i + 1].second}});
    }

    return edges;
}


int main()
{
    // freopen("Boolean_B.txt", "r", stdin);
    freopen("Edges.txt","w",stdout);
    vector<vector<bool>>B(100,vector<bool>(100,0));
    for(int j=0;j<100;j++){
        for(int i=0;i<100;i++){
            int x;
            cin>>x;
            B[j][i]=x;
        }
    }
    auto ans=give_edges(B);
    cout<<ans.size()<<" "<<ans.size()<<endl;
    for(auto answer:ans){
        // cout<<"("<<answer.first.first<<", "<<answer.first.second<<"),"<<"("<<answer.second.first<<","<<answer.second.second<<"),\n";
        // cout<<"{{"<<answer.first.first<<", "<<answer.first.second<<"},"<<"{"<<answer.second.first<<","<<answer.second.second<<"}},\n";
        cout<<answer.first.first<<" "<<answer.first.second<<" "<<answer.second.first<<" "<<answer.second.second<<"\n";
    }
    fclose(stdout) ; 

    return 0 ; 
}