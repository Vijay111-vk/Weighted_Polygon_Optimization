#include<bits/stdc++.h>
using namespace std;
#define ll long long


class point {
public:
	int x, y;
	point() : x(0), y(0) {}
	point(int x_, int y_) : x(x_), y(y_) {}
};

struct EdgeTop250 {
    double x1, y1, x2, y2;  // Edge coordinates
    bool isHorizontal;    // Edge orientation
    EdgeTop250(double _x1, double _y1, double _x2, double _y2) 
        : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {
        isHorizontal = (x1 == x2);
    }
};

int Top250ans(vector<point> &P,vector<int> &aVal,vector<point> &Q,vector<int> &bVal,vector<EdgeTop250> &edges){
    vector<pair<int,pair<int,int>>> colSum(1e4+1,{0,{0,0}});  // sum, colNo, height(row).
    vector<vector<pair<int,int>>> colVal(1e4+1); // row, value

    int totPosi=0;
    vector<int> val; // for top 250
    for(int i=0;i<P.size();i++){
        int a,b,c;
        a=P[i].x;
        b=P[i].y;
        c=aVal[i];
        totPosi+=c;
        val.push_back(c);
        if(a==10000) continue;
        colVal[b].push_back({a,c});
    }

    for(int i=0;i<Q.size();i++){
        int a,b,c;
        a=Q[i].x;
        b=Q[i].y;
        c=bVal[i];
        if(a==10000) continue;
        colVal[b].push_back({a,-c});
    }

    // vector<int> height(1e4+1,0);
    for(int i=0; i<1e4+1;i++){
         if(colVal.size() ) {
            sort(colVal[i].rbegin(),colVal[i].rend());
             int val  =  0 ;
             int maxval = 0  ;
             int optHeight = 0;
            for(auto x: colVal[i]){
                val+=x.second;
                if(val>maxval){
                    maxval=val;
                    optHeight=x.first;
                }
            }
            colSum[i].first=maxval;  // sum
            colSum[i].second.first=i; // colNo
            colSum[i].second.second=optHeight; // height(row)
         }
    }

    sort(colSum.begin(),colSum.end(),greater<pair<int,pair<int,int>>>());
    sort(val.begin(),val.end(),greater<int>());
    int totalSum=0;
    int top250=0;

    vector<pair<int,pair<int,int>> > selectedColumns;
    for(int i=0;i<245;i++){
        top250+=val[i];
        selectedColumns.push_back({colSum[i].second.first,{colSum[i].second.second,colSum[i].first}}); // colNo , height, sum
    }
    sort(selectedColumns.begin(),selectedColumns.end());

    
    edges.push_back(EdgeTop250(1e4-0.1, 0.1 , 1e4-0.1, 1e4-0.1));
    edges.push_back(EdgeTop250(1e4-0.2, 0.1 , 1e4-0.1, 0.1));
    edges.push_back(EdgeTop250(1e4-0.1, 1e4-0.1 , 1e4-0.2, 1e4-0.1));

    double x1=(1e4-0.2);
    double y1=0.1;
    // colNo, height, sum
    for(auto i:selectedColumns){
        if(i.second.first==0 || i.second.second<=0) continue;
        if(i.first==0 || i.first ==1e4) continue;
        totalSum+=i.second.second;
        double x2= x1;
        double y2= i.first -0.1;
        edges.push_back(EdgeTop250(x1, y1, x2, y2));
        edges.push_back(EdgeTop250(x2, y2, i.second.first-0.1, y2));
        edges.push_back(EdgeTop250(i.second.first-0.1, y2, i.second.first-0.1, i.first+0.1));
        edges.push_back(EdgeTop250(i.second.first-0.1, i.first+0.1, x1, i.first+0.1));
        y1=i.first+0.1;
        // cout<<i.second.first<<" "<<i.first<<" "<<i.second.second<<endl;
    }

    edges.push_back(EdgeTop250{x1, y1, 1e4-0.2, 1e4-0.1});

    // freopen("./output250.txt","w",stdout);
    // for(auto i:edges){
    //     //  cout<<"(("<<i.x1<<","<<i.y1<<") , "<<"("<<i.x2<<","<<i.y2<<")),\n";
    //      cout<<"{{"<<i.x1<<","<<i.y1<<"} , "<<"{"<<i.x2<<","<<i.y2<<"}},\n";
    //     // cout<<i.x1<<" "<<i.y1<<" "<<i.x2<<" "<<i.y2<<endl;
    // }
    // cout<<endl;
    
    // cout<<"top250 "<<top250<<endl;
    // cout<<"Total Sum "<<totalSum<<endl;
    // cout<<"Tot Positive "<<totPosi<<endl;
    // cout<<"percentage "<<(1.0*totalSum/totPosi)*100<<endl;
    // cout<<"percentage 250 "<<(1.0*top250/totPosi)*100<<endl;
    
    return totalSum;
}

int main(){
ios_base::sync_with_stdio(false);
cin.tie(NULL);
    // freopen("./final_input/input00.txt","r",stdin);
    
    int n, m;
	cin >> n;
	vector<point> P(n);
	vector<int> positive(n);
    long long total_Positive=0;
	for (int i = 0; i < n; i++) {
		cin >> P[i].x >> P[i].y;
		cin >> positive[i];
        total_Positive+=positive[i];
	}
	cin >> m;
	vector<point> Q(m);
	vector<int> negative(m);
	for (int i = 0; i < m; i++) {
		cin >> Q[i].x >> Q[i].y;
		cin >> negative[i];
	}


    // Top 250     
    vector<EdgeTop250> edges250; 
    int top250ans=Top250ans(P,positive,Q,negative,edges250);

    // 250output
    freopen("./top250.txt","w",stdout);
    cout<<top250ans<<endl;
    cout<<edges250.size()<<" "<<edges250.size()<<endl;
    for(auto i:edges250){
         cout<<"("<<i.x1<<","<<i.y1<<"), "<<"("<<i.x2<<","<<i.y2<<")\n";
        //  cout<<"{{"<<i.x1<<","<<i.y1<<"} , "<<"{"<<i.x2<<","<<i.y2<<"}},\n";
        //  cout<<i.x1<<" "<<i.y1<<" "<<i.x2<<" "<<i.y2<<endl;
    }
    cout<<endl;





    /// checkerrrrr 


    return 0;
}