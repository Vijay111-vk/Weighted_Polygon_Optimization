#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class F>
struct Point {
    F x, y;
    Point(F x = 0, F y = 0) : x(x), y(y) {}
    
    bool operator<(const Point& other) const { 
        return tie(x, y) < tie(other.x, other.y); 
    }

    bool operator==(const Point& other) const { 
        return x == other.x && y == other.y; 
    }
};

template <class F> using Polygon = vector<Point<F>>;

inline int prev(int i, int n) { return i == 0 ? n-1 : i-1; }
inline int next(int i, int n) { return i == n-1 ? 0 : i+1; }

template <class F>
F area(const Polygon<F>& poly) {
    int n = static_cast<int>(poly.size());
    F area = F(0);
    for (int i = 0; i < n; ++i)
        area += poly[i].x * (poly[next(i, n)].y - poly[prev(i, n)].y);
    return area;
}

template <class F>
bool edge_inter(pair<Point<F>, Point<F>> e1, pair<Point<F>, Point<F>> e2) {
    // auto [p1, p2] = e1; auto [p3, p4] = e2;
    auto p1=e1.first;
    auto p2=e1.second;
    auto p3=e2.first;
    auto p4=e2.second;
    F x = p1.x, y = p3.y;
    return (min(p1.y, p2.y) < y && y < max(p1.y, p2.y) &&
            min(p3.x, p4.x) < x && x < max(p3.x, p4.x));
}

template <class F>
void dfs_for_check(Point<F> p, map<Point<F>, Point<F>>& hori, map<Point<F>, Point<F>>& veri, 
                   map<Point<F>, bool>& vis, vector<Point<F>>& vertices) {
    vis[p] = true;
    vertices.push_back(p);
    if (!vis[hori[p]]) dfs_for_check(hori[p], hori, veri, vis, vertices);
    if (!vis[veri[p]]) dfs_for_check(veri[p], hori, veri, vis, vertices);
}

template <class F>
bool checker(vector<pair<Point<F>, Point<F>>>& edges, vector<Point<F>>& vertices) {
    map<Point<F>, Point<F>> hori, veri;
    map<Point<F>, int> deg;
    
    for (auto& edge : edges) {
        auto p1=edge.first;
        auto p2=edge.second;
        if (p1 == p2) {
            //cout << p1.x << " " << p1.y << " "<<p2.x << " "<<p2.y << endl;
            cerr << "INVALID! Edge of 0 length\n";
            return false;
        }
        if (p1.x != p2.x && p1.y != p2.y) {
            cerr << "INVALID! Edge not parallel to axes\n";
            return false;
        }
        if (p1.x == p2.x) { hori[p1] = p2; hori[p2] = p1; }
        else { veri[p1] = p2; veri[p2] = p1; }
        deg[p1]++; deg[p2]++;
    }

    for (auto& e1 : edges) {
        auto p1=e1.first;
        auto p2=e1.second;
        for (auto& e2 : edges) {
            auto p3=e2.first;
            auto p4=e2.second;
            if (p1 == p3 && p2 == p4) continue;
            bool hori1 = (p1.y == p2.y), hori2 = (p3.y == p4.y);
            if (hori1 && hori2) {
                if (p1.y != p3.y || (max(p1.x, p2.x) < min(p3.x, p4.x)) ||
                    (max(p3.x, p4.x) < min(p1.x, p2.x))) {}
                else {
                    cerr << "INVALID! Edge intersect or is making 180 degrees at a vertex\n";
                    return false;
                }
            } 
            else if(!hori1&&hori2) {
                if(edge_inter(pair<Point<double>, Point<double>>{p1, p2}, pair<Point<double>, Point<double>>{p3, p4})) {cerr << "INVALID! Edge intersect\n";
                return false;}
            }
            else if(hori1&&!hori2) {
                if(edge_inter(pair<Point<double>, Point<double>>{p3, p4}, pair<Point<double>, Point<double>>{p1, p2})) {cerr << "INVALID! Edge intersect\n";
                return false;}
            }
            else {
                if(((p1.x != p3.x) || (max(p1.y, p2.y) < min(p3.y, p4.y)) ||
                       (max(p3.y, p4.y) < min(p1.y, p2.y)))) {}
                else {
                    cerr << "INVALID! Edge intersect or is making 180 degrees at a vertex\n";
                    return false;
                }
            }
        }
    }

    for (auto& d1 : deg) {
        auto x=d1.second;
        if (x != 2) {
            cerr << "INVALID! Some vertex doesn't have a degree of 2\n";
            return false;
        }
    }

    map<Point<F>, bool> vis;
    dfs_for_check(edges[0].first, hori, veri, vis, vertices);
    
    for (auto& d1 : deg) {
        auto p=d1.first;
        if (!vis[p]) {
            cerr << "INVALID! There are holes!\n";
            return false;
        }
    }
    
    return true;
}

template <class F>
F ccw(const Point<F>& A, const Point<F>& B, const Point<F>& C) {
    return ((B.x-A.x)*(C.y-A.y)-(B.y-A.y)*(C.x-A.x)) ;
}

template <class F1, class F2>
int pointVsPolygon(const Point<F1>& point, const Polygon<F2>& poly) {
  int n = static_cast<int>(poly.size()), windingNumber = 0;
  for (int i = 0; i < n; ++i) {
    if (point == poly[i]) return 0;
    int j = next(i, n);
    if (poly[i].y == point.y && poly[j].y == point.y) {
      if (min(poly[i].x, poly[j].x) <= point.x &&
          point.x <= max(poly[i].x, poly[j].x)) return 0;
    } else {
      bool below = poly[i].y < point.y;
      if (below != (poly[j].y < point.y)) {
        auto orientation = ccw(point, poly[j], poly[i]);
        if (orientation == 0) return 0;
        if (below == (orientation > 0)) windingNumber += below ? 1 : -1;
      }
    }
  }
  return windingNumber == 0 ? 1 : -1;
}

template <class F>
ll score_calc(vector<pair<Point<F>, Point<F>>>& edges, vector<pair<Point<F>,int>>& given_shells_mines) {
    vector<Point<F>> vertices;
    if(checker(edges,vertices)) {
        if(area(vertices)<0) {reverse(vertices.begin(),vertices.end());}
        ll res = 0;
        for(auto& entry:given_shells_mines) {
            auto point=entry.first;
            auto val=entry.second;
            if(pointVsPolygon(point,vertices)!=1) res+=val;
        }
        return res;
    }
    else return -1;
}

// Function to generate a random double in the given range
double randDouble(double low, double high) {
    return low + (high - low) * (rand() / (double)RAND_MAX);
}

// Function to generate a random set of edges
vector<pair<Point<double>, Point<double>>> generateRandomEdges(int numEdges, double rangeX, double rangeY) {
    vector<pair<Point<double>, Point<double>>> edges;
    set<pair<Point<double>, Point<double>>> edgeSet; // Ensure uniqueness

    while ((int)edges.size() < numEdges) {
        double x1 = randDouble(0, rangeX);
        double y1 = randDouble(0, rangeY);
        bool horizontal = rand() % 2;
        
        double x2 = horizontal ? randDouble(x1, rangeX) : x1;
        double y2 = horizontal ? y1 : randDouble(y1, rangeY);

        if (x1 == x2 && y1 == y2) continue; // Skip zero-length edges

        pair<Point<double>, Point<double>> edge = {{x1, y1}, {x2, y2}};
        if (edgeSet.count(edge) == 0) {
            edges.push_back(edge);
            edgeSet.insert(edge);
        }
    }

    return edges;
}

// Function to generate random shells and mines
vector<pair<Point<double>, int>> generateRandomShellsMines(int numItems, double rangeX, double rangeY, int minVal, int maxVal) {
    vector<pair<Point<double>, int>> items;

    for (int i = 0; i < numItems; ++i) {
        double x = randDouble(0, rangeX);
        double y = randDouble(0, rangeY);
        int value = rand() % (maxVal - minVal + 1) + minVal;
        items.push_back({{x, y}, value});
    }

    return items;
}

int main() {

    //  if (argc < 3) {  // expecting two file names, for example
    //     cerr << "Usage: " << argv[0] << " <edges_file> <input_file>" << endl;
    //     return 1;
    // }

    // ifstream file1(argv[1]);
    // if (!file1) {
    //     cerr << "Error opening file " << argv[1] << endl;
    //     return 1;
    // }
    srand(time(0)); // Seed for randomness
    //put edges in this file
    //freopen("input09.txt","r",stdin);
    vector<pair<Point<double>, Point<double>>> edges;
    // ifstream file1("Edges.txt");
    int num_edges; cin>>num_edges;
    cin>>num_edges;
    for(int i=0; i<num_edges; i++) {
        double a,b,c,d; cin>>a>>b>>c>>d;
        edges.push_back({{a,b},{c,d}});
    }
    // file1.close();

    // ifstream file2(argv[2]);
    // if (!file2) {
    //     cerr << "Error opening file " << argv[2] << endl;
    //     return 1;
    // }

    vector<pair<Point<double>, int>> given_shells_mines;
    // ifstream file2("./input/input00.txt");

    int n; cin>>n;
    for(int i=0; i<n; i++) {
        double x,y; cin>>x>>y;
        int val; cin>>val;
        given_shells_mines.push_back({{x,y},val});
    }
    int m; cin>>m;
    for(int i=0; i<m; i++) {
        double x,y; cin>>x>>y;
        int val; cin>>val;
        given_shells_mines.push_back({{x,y},-val});
    }
    // file2.close();
    freopen("output.txt","w",stdout);
    int score=score_calc(edges,given_shells_mines);
    if(score<=0 || num_edges > 1000 || num_edges==0){
        cout<<-1<<endl;
    }
    else{
        cout<<score<<endl;
        cout<<num_edges<<" "<<num_edges<<endl;
        for(auto edge: edges){
            cout<<"("<<edge.first.x<<", "<<edge.first.y<<"), "<<"("<<edge.second.x<<", "<<edge.second.y<<")\n";
            // cout<<"(("<<edge.first.x<<","<<edge.first.y << "),(" << edge.second.x << ","<< edge.second.y <<")),\n";

        }
    }
    
    return 0;  
}