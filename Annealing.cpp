#include <cmath> 
#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <bits/stdc++.h>
#include <thread>
using namespace std;

double get_clock() {
	return double(clock()) / CLOCKS_PER_SEC;
}


mt19937 mt(time(nullptr));  

int totalRecSum = 0 ;
const int L = 10000;
const int Y = 101;  
const int Z = (L+Y-1) / Y;
vector<vector<int> > A(Z, vector<int>(Z,0));
vector<vector<bool>> B(Z, vector<bool>(Z, false));
int no_of_subrows = 2;
int no_of_subcols = 2; 
static int totalVertices  = no_of_subcols*no_of_subrows*4; 
int recSize_inrow=(Z+no_of_subrows-1)/no_of_subrows;
int recSize_incol = (Z+no_of_subcols-1)/no_of_subcols;
vector<vector<vector<vector<bool>>>> newB(no_of_subrows,vector<vector<vector<bool>>>(no_of_subcols,vector<vector<bool>>
                                        (recSize_inrow,vector<bool> (recSize_incol,false))));

vector<vector<long long>> curScore(no_of_subrows,vector<long long>(no_of_subcols,0));
vector<vector<long long>> bestScore(no_of_subrows,vector<long long>(no_of_subcols,0));
vector<vector<long long>> edgeCount(no_of_subrows,vector<long long>(no_of_subcols,4));
vector<vector<long long>> Best_B_edgeCount(no_of_subrows,vector<long long>(no_of_subcols,4));
int get_max_rec_sum(int row, int col); 
vector<vector<vector<vector<bool> >>> best_B ;

int isHori=0;
int isVer=0;

int vertex_limit = (970) / (no_of_subrows * no_of_subcols) ; 

namespace myrandom {
	uint64_t seed = 1234567891234567891;
	uint64_t xorshift64() {
		seed ^= seed << 13;
		seed ^= seed >> 7;
		seed ^= seed << 17;
		return seed;
	}
	int64_t next_int(int64_t l, int64_t r) {
		assert(l < r);
        return l + mt() % (r - l);
		// return l + xorshift64() % (r - l);

	}
	double next_double(double l, double r) {
		double z = double(xorshift64()) / double(uint64_t(-1));
		return l + z * (r - l);
	}
}

class point {
public:
	int x, y;
	point() : x(0), y(0) {}
	point(int x_, int y_) : x(x_), y(y_) {}
};

struct Edge {
    int x1, y1, x2, y2;  // Edge coordinates
    bool isHorizontal;    // Edge orientation
    Edge(int _x1, int _y1, int _x2, int _y2) 
        : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {
        isHorizontal = (x1 == x2);
    }
};

// /////////// check for loop of zeros /////////////////////////////////////// ( need to be done)
bool check_shape(const vector<vector<bool> >& B) {
    int H=B.size();
    int W=B[0].size();
	const vector<int> dx = { 0, 1, 0, -1 };
	const vector<int> dy = { 1, 0, -1, 0 };
	vector<vector<bool> > vis(H, vector<bool>(W, false));
	int cnt = 0;
	auto dfs = [&](auto& self, int x, int y, bool f) -> void {
		vis[x][y] = true;
		cnt++;
		for (int i = 0; i < 4; i++) {
			int tx = x + dx[i];
			int ty = y + dy[i];
			if (0 <= tx && tx < H && 0 <= ty && ty < W && !vis[tx][ty] && B[tx][ty] == f) {
				self(self, tx, ty, f);
			}
		}
	};
	int sx = -1, sy = -1;
	for (int i = 0; i < H && sx == -1; i++) {
		for (int j = 0; j < W && sx == -1; j++) {
			if (B[i][j]) {
				sx = i;
				sy = j;
			}
		}   
	}
	if (sx == -1 && sy == -1) {
		return false;
	}
	dfs(dfs, sx, sy, true);
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if ((i == 0 || i == H - 1 || j == 0 || j == W - 1) && !B[i][j] && !vis[i][j]) {
				dfs(dfs, i, j, false);
			}
		}
	}
	return (cnt == H * W);
}

////////////////// giving collinear points also (need to be corrected) ////////////////////////////
vector<point> get_polygon(int H, int W, const vector<vector<bool> >& B) {
	const vector<int> dx = { 0, 1, 0, -1 };
	const vector<int> dy = { 1, 0, -1, 0 };
	auto inside = [&](int x, int y) -> bool {
		return (0 <= x && x < H && 0 <= y && y < W && B[x][y]);
	};
	int sx = -1, sy = -1;
	for (int i = 0; i < H && sx == -1; i++) {
		for (int j = 0; j < W && sx == -1; j++) {
			if (!inside(i - 1, j - 1) && !inside(i - 1, j) && !inside(i, j - 1) && inside(i, j)) {
				sx = i;
				sy = j;
			}
		}
	}


	vector<point> res;
	int x = sx, y = sy, dir = 1;
	do {
		res.push_back(point(x, y));
		x += dx[dir];
		y += dy[dir];
		if (dir != 3 && !inside(x, y - 1) && inside(x, y)) {
			dir = 1;
		} else if (dir != 0 && !inside(x - 1, y - 1) && inside(x, y - 1)) {
			dir = 2;
		} else if (dir != 1 && !inside(x - 1, y) && inside(x - 1, y - 1)) {
			dir = 3;
		} else {
			assert(dir != 2 && !inside(x, y) && inside(x - 1, y));
			dir = 0;
		}
	} while (x != sx || y != sy);
	return res;

}

Edge get_edge(vector <vector <bool>>& B, int &edgeCount){
    int H = B.size() ; int W = B[0].size() ;
    vector <int> row[H] ;
    vector <int> col[W] ;

    for(int j = 0 ; j < H ; j ++){
        for(int k = 0 ; k < W ; k++){
            if(B[j][k] == 1){
                if(j == 0  or j == H-1 ){
                    row[j].push_back(k) ; 
                }
                else if(j > 0 and B[j - 1][k] == 0){
                    row[j].push_back(k) ; 
                }
                else if(j < H - 1 and B[j + 1][k] == 0){
                    row[j].push_back(k) ; 
                }
            }
        }
    }
    for(int k = 0 ; k < W ; k++){
        for(int j = 0 ; j < H ; j++){
            if(B[j][k] == 1){
                if(k == 0  or k == W-1){
                    col[k].push_back(j) ; 
                }
                else if(k > 0 and B[j][k - 1] == 0){
                    col[k].push_back(j) ; 
                }
                else if(k < W - 1 and B[j][k + 1] == 0){
                    col[k].push_back(j) ; 
                }
            }
        }
    }
    
    vector<Edge> edges ; 

    for(int j = 0 ; j < H ; j++){
        if(row[j].size()){
           int prev = row[j][0]  ; 
           int start = prev; 
           for(int i=1; i<row[j].size(); i++) {
                if(row[j][i] != prev + 1){
                     edges.push_back({j, start, j, prev }) ; 
                     start = row[j][i] ;
                     
                }
                prev = row[j][i] ; 
           }
            edges.push_back({j, start, j, prev }) ;
        }
    }

    for(int k = 0 ; k < W ; k++){
        if(col[k].size()){
           int prev = col[k][0]  ; 
           int start = prev; 
           for(int i=1; i<col[k].size(); i++) {
                if(col[k][i] != prev + 1){
                        edges.push_back({start, k, prev, k }) ;
                     start = col[k][i] ;
                     
                }
                prev = col[k][i] ; 
           }
            edges.push_back({start, k, prev, k }) ;
        }
    }

    edgeCount = edges.size() ;
    int randomNum = myrandom::next_int(0, edges.size());

    return edges[randomNum] ;

}

int checkValidRectangle (vector<vector<int>> &A,  bool ishorizontal, bool dir, Edge& edge , vector<vector<bool>> &B,int startx,int starty) {
    int endx = B.size();
    int endy= B[0].size();

    if (ishorizontal) {                 ////// ??????
        int maxHeight =0 ;  
        bool valid = true; 
        int curSum = 0 ; 
        int optHeight = 0 ; 
        int optSum = 0; 
        if (dir) {
            int x = edge.x1 ;
            int y1 = edge.y1;
            int y2 = edge.y2;
           
            while(valid && x+maxHeight+1 < endx) { 
                int tempSum = 0 ; 
                for (int i = y1; i <= y2; i++) {
                    if (B[x+maxHeight+1][i] == 0) {
                        valid = false;
                        break;
                    }
                }
                if(valid) {
                    maxHeight++;
                }
            }

            if(maxHeight <= 1) return -1; 

            for (int i = y1; i <= y2; i++) {
                curSum += A[x+startx][i+starty];
            }
            optSum = curSum;
            optHeight = 1;

            for (int i = 1; i+1 < maxHeight; i++) {
                for (int j = y1; j <= y2; j++) {
                    curSum += A[x+i+startx][j+starty];
                }
                
                if (curSum < optSum) {
                    optSum = curSum;
                    optHeight = i + 1;
                }

            }
            if(optSum>0 ) return -1;
            return optHeight;
        } 
        else {
            int x = edge.x2 ;
            int y1 = edge.y1;
            int y2 = edge.y2;
           
            while(valid && x-maxHeight-1 >= 0) { 
                int tempSum = 0 ; 
                for (int i = y1; i <= y2; i++) {
                    if (B[x-maxHeight-1][i] == 0) {
                        valid = false;
                        break;
                    }
                }
                if(valid) {
                    maxHeight++;
                }
            }

            if(maxHeight <= 1) return -1; 

            for (int i = y1; i <= y2; i++) {
                curSum += A[x+startx][i+starty];
            }
            optSum = curSum;
            optHeight = 1;

            for (int i = 1; i+1 < maxHeight; i++) {
                for (int j = y1; j <= y2; j++) {
                    curSum += A[x-i+startx][j+starty];
                }
                
                if (curSum < optSum) {
                    optSum = curSum;
                    optHeight = i - 1;
                }

            }
            if(optSum>0 ) return -1;

            return optHeight;
        }
    } else {
        int maxHeight =0 ;  
        bool valid = true; 
        int curSum = 0 ; 
        int optHeight = 0 ; 
        int optSum = 0; 
        if (dir) {
            int y = edge.y1 ;
            int x1 = edge.x1;
            int x2 = edge.x2;
            while(valid && y+maxHeight+1 < endy) { 
                for (int i = x1; i <= x2; i++) {
                    if (B[i][y+maxHeight+1] == 0) {
                        valid = false;
                        break;
                    }
                }
                if(valid) {
                    maxHeight++;
                }
            }

            if(maxHeight <= 1) return -4; 

            for (int i = x1; i <= x2; i++) {
                curSum += A[i+startx][y+starty];
            }
            optSum = curSum;
            optHeight = 1;

            for (int i = 1; i+1 < maxHeight; i++) {
                for (int j = x1; j <= x2; j++) {
                    curSum += A[j+startx][y+i+starty];
                }
                
                if (curSum < optSum) {
                    optSum = curSum;
                    optHeight = i + 1;
                }

            }
            if(optSum>0 ) return -1;

            return optHeight;
        } 
        else {
            int y = edge.y2 ;
            int x1 = edge.x1;
            int x2 = edge.x2;
           
            while(valid && y-maxHeight-1 >= 0) { 
                int tempSum = 0 ; 
                for (int i = x1; i <= x2; i++) {
                    if (B[i][y-maxHeight-1] == 0) {
                        valid = false;
                        break;
                    }
                }
                if(valid) {
                    maxHeight++;
                }
            }

            if(maxHeight <= 1) return -1; 

            for (int i = x1; i <= x2; i++) {
                curSum += A[i+startx][y+starty];
            }
            optSum = curSum;
            optHeight = 1;

            for (int i = 1; i+1 < maxHeight; i++) {
                for (int j = x1; j <= x2; j++) {
                    curSum += A[j+startx][y-i+starty];
                }
                if(curSum < optSum) {
                    optSum = curSum;
                    optHeight = i - 1;
                }
            }
            if(optSum>0 ) return -1;

            return optHeight;
        }
    }

  
}


void annealing (int row,int col){
    
    const double TIME_LIMIT = 5;
	double start_time = get_clock();
	double cur_time = 0.0;
    double temp_time = get_clock(); 
	uint64_t loops = 0;
    deque<int> prevScores ; 
    int counterOfStalls =0  ;

    // int allowedVertices = (((double) get_max_rec_sum(row,col))/ ((double) totalRecSum)) * (1100.00) ; 

    while (true) {
		loops++;
        // check time limit
		if ((loops & 31) == 0) {
			cur_time = get_clock() - start_time;
			if(cur_time > TIME_LIMIT) {
				break;
			}
		}

        int startx= row*recSize_inrow;
        int starty= col*recSize_incol;
        int endx=min(Z,startx+recSize_inrow);
        int endy=min(Z,starty+recSize_incol);

        auto restartAnnealing = [&]() {
                for(int i=0;i<recSize_inrow;i++){
                    for(int j=0;j<recSize_incol;j++){
                        newB[row][col][i][j]= best_B[row][col][i][j];
                    }
                }
                curScore[row][col]=0;


                 for(int i=startx;i<endx;i++){
                    for(int j=starty;j<endy;j++){
                        if(newB[row][col][i-startx][j-starty]) curScore[row][col] += A[i][j];
                    }
                }
                // int temptemp; 
                // get_edge(best_B[row][col], temptemp); 
                // edgeCount[row][col] = temptemp;
                edgeCount[row][col]=Best_B_edgeCount[row][col];

                // temp_time = get_clock();
        } ; 


        int curEdge=edgeCount[row][col];
        // if(curEdge > vertex_limit) break;
       
        Edge edge = get_edge(newB[row][col] , curEdge);


        vector<pair<int, int>> points_to_flip;
        if (edge.isHorizontal) {
            // continue ; 
            isHori++;
            int x = edge.x1 ; 
            bool dir = 0;  // 0 means UP, 1 means DOWN
            int dif = edge.y2 - edge.y1;
            int y1 = edge.y1 + (myrandom::next_int(0, dif+1));
            int y2 = edge.y1 + (myrandom::next_int(0, dif+1));
            if(y1>y2) swap(y1,y2); 
            // cout << "Hello .............." << row << " " << col << " " << edge.x1 << " " << y1 << " " << edge.x2 << " " <<  y2 << endl ; 
            if(edge.x1+1 < recSize_inrow && newB[row][col][edge.x1+1][edge.y1] == 1) {
                dir = 1;
            }
            int height = checkValidRectangle(A, true, dir, edge, newB[row][col],startx,starty); /// checkkkk
            if(height <= 0) continue;

            if(dir==1) {
                for (int i = x; i < min(x+height , recSize_inrow); i++) {
                    for(int j=y1; j<=y2; j++) {
                        newB[row][col][i][j] = !newB[row][col][i][j];
                        points_to_flip.push_back({i, j});
                    }
                }
            }
            else {
                for (int i = x; i >= max(0,x-height+1); i--) {
                    for(int j=y1; j<=y2; j++) {
                        newB[row][col][i][j] = !newB[row][col][i][j];
                        points_to_flip.push_back({i, j});
                    }
                }
            }
        } 
        else {
            isVer++;
            continue;
            int y = edge.y1 ; 
            bool dir = 0;  // 0 means LEFT, 1 means RIGHT
            int dif = edge.x2 - edge.x1;
            int x1 = edge.x1 + (myrandom::next_int(0, dif+1));
            int x2 = edge.x1 + (myrandom::next_int(0, dif+1));
            if(x1>x2) swap(x1,x2); 


            if(edge.y1+1 < recSize_incol && newB[row][col][edge.x1][edge.y1+1] == 1) {
                dir = 1;
            }

            int height = checkValidRectangle(A, false, dir, edge, newB[row][col],startx,starty);  /// checkkkk

            if(height <= 0) continue;

            if(dir==1) {
                for (int i = x1; i <= x2; i++) {
                        for(int j=y; j<min(y+height,recSize_incol); j++) {
                            newB[row][col][i][j] = !newB[row][col][i][j];
                            points_to_flip.push_back({i, j});
                        }
                    }
            }

            else {
                    for (int i = x1; i <= x2; i++) {
                        for(int j=y; j>=max(0,y-height+1); j--) {
                            newB[row][col][i][j] = !newB[row][col][i][j];
                            points_to_flip.push_back({i, j});
                        }
                    }
            }
            
        }
        if(totalVertices> 1200) {
            // restartAnnealing(); 
            // continue;
            break;
        }
        int next_score = 0;

        for(int i=startx;i<endx;i++){
            for(int j=starty;j<endy;j++){
                next_score += int(newB[row][col][i-startx][j-starty] ? A[i][j] : 0);
            }
        }

        auto revertBack = [&] () {
            for (auto p : points_to_flip) {
                newB[row][col][p.first][p.second] = !newB[row][col][p.first][p.second];
            }
        }; 
        bool valid = true;
        valid = check_shape(newB[row][col]); /// checkkkk
      
  
  
        if (!valid) {
            revertBack(); 
            counterOfStalls ++ ;
        } else {
            // cout << edge.x1 << " " << edge.y1 << " " << edge.x2 << " " << edge.y2 << endl;
            double ratio = (get_clock()-temp_time) / TIME_LIMIT;
            double temp = 40 * (1.0 - ratio);
            if (curScore[row][col] <= next_score || myrandom::next_double(0.0, 1.0) < exp((next_score - curScore[row][col]) / temp) ) {
                if (bestScore[row][col] < next_score) {
                    cout << "Loop #" << loops <<" "<<row<<" "<<col<<" " << ": score = " << next_score << endl;
                    bestScore[row][col] = next_score;
                    best_B[row][col] = newB[row][col];
                    totalVertices -= Best_B_edgeCount[row][col];
                    totalVertices += curEdge  ; 
                    Best_B_edgeCount[row][col]=curEdge;
                }
                curScore[row][col] = next_score;
                cout << loops << " " <<row<<" "<<col<<" "<< curScore[row][col] << endl;
                counterOfStalls = 0 ;
                edgeCount[row][col]=curEdge;
             
            } 
            // else if( (prevScores.size() >= 400 && ( abs( prevScores.front() - prevScores.back()) <= (0.03*((double) bestScore[row][col] )) ))) {
            //      // restart annealing
            //    restartAnnealing(); 
            //    counterOfStalls = 0 ; 
            // }
            // else if (counterOfStalls > 100) {
            //      restartAnnealing ; 
            //      counterOfStalls = 0 ; 
            // }
            else {
                revertBack() ;
                counterOfStalls ++ ;
            }
        }

        // prevScores.push_back(bestScore[row][col]);
        // if(prevScores.size() > 400) {
        //         prevScores.pop_front();
        // }

    }
}

void annealing_thread(int row, int col) {
    // Call the annealing function for the given row and col
    annealing(row, col);
}




int get_max_rec_sum (int row, int col) {
    int max_sum = INT_MIN;
     int startx= row*recSize_inrow;
     int starty= col*recSize_incol;
    int best_r1 =startx, best_c1 = starty,   best_r2 = startx,  best_c2 = starty;
    int endx = min(startx+recSize_inrow, Z); 
    int endy = min(starty + recSize_incol, Z); 

    for (int r1 = startx; r1 < endx ; r1++) {
        vector<int> temp(Z, 0);
        for (int r2 = r1; r2 < endx; r2++) {
            for (int c = starty ; c < endy; c++) {
                temp[c] += A[r2][c];
            }
            int current_sum = 0, start_col = 0;

            for (int c = starty ; c < endy; c++) {
                current_sum += temp[c];
                if (current_sum > max_sum) {
                    max_sum = current_sum;
                    best_r1 = r1;
                    best_c1 = start_col;
                    best_r2 = r2;
                    best_c2 = c;
                }
                if (current_sum < 0) {
                    current_sum = 0;
                    start_col = c + 1;
                }
            }
        }
    }

    return max_sum ; 

}


int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
    cout.tie(nullptr);
    // freopen("./input/input05.txt", "r", stdin);
	int n, m;
	cin >> n;
	vector<point> P(n);
	vector<int> a(n);
    long long total_Positive=0;
	for (int i = 0; i < n; i++) {
		cin >> P[i].x >> P[i].y;
		cin >> a[i];
        total_Positive+=a[i];
	}
	cin >> m;
	vector<point> Q(m);
	vector<int> b(m);
	for (int i = 0; i < m; i++) {
		cin >> Q[i].x >> Q[i].y;
		cin >> b[i];
	}


	for (int i = 0; i < n; i++) {
		int r = min(P[i].x / Y, Z - 1);
		int c = min(P[i].y / Y, Z - 1);
		A[r][c] += a[i];
	}
	for (int i = 0; i < m; i++) {
		int r = min(Q[i].x / Y, Z - 1);
		int c = min(Q[i].y / Y, Z - 1);
		A[r][c] -= b[i];
	}

    cout<<Z<<" "<<recSize_inrow<< " " << recSize_incol<< endl;
    // for(int i=0; i<Z; i++){ 
    //     for(int j=0; j<Z; j++){
    //         cout<<"{ "<<A[i][j]<<" "<<i<<" "<<j<<"} ";
    //     }
    //     cout<<"\n";
    // }
    // cout<<"ok\n";

    for(int i=0;i<no_of_subrows;i++){
        for(int j=0;j<no_of_subcols;j++){
            int startx= i*recSize_inrow;
            int starty= j*recSize_incol;
            int endx=min(Z,startx+recSize_inrow);
            int endy=min(Z,starty+recSize_incol);
            // cout<<startx<<" "<<starty<<" "<<endx<<" "<<endy<<endl;
            for(int k=startx;k<endx;k++){
                for(int l=starty;l<endy;l++){
                    curScore[i][j]+=A[k][l];
                    newB[i][j][k-startx][l-starty]=1;
                }
            }
        }
    }

    int Total_sum_of_grid = 0;


    for(int i=0; i<Z; i++){ 
        for(int j=0; j<Z; j++){
            Total_sum_of_grid += A[i][j];
        }
    }
    cout <<"Total sum of grid "<< Total_sum_of_grid << endl;
    int ttsum=0; // sum of all subrectangles

    // bestScore intialisation
    for(int i=0;i<no_of_subrows;i++){
        for(int j=0;j<no_of_subcols;j++){
            bestScore[i][j]=curScore[i][j];
        }
    }
    cout<<endl;

    for(int i=0;i<no_of_subrows;i++){
        for(int j=0;j<no_of_subcols;j++){
            cout<<bestScore[i][j]<<" ";
            ttsum+=bestScore[i][j];
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<"ttsum "<<ttsum<<endl;
    // cout<<"ok";
    
    best_B.assign(no_of_subrows,vector<vector<vector<bool>>>(no_of_subcols,vector<vector<bool>>
                                        (recSize_inrow,vector<bool> (recSize_incol,true))));
	// int cur_score = max_sum; 
	// int best_score = cur_score;

    // for(int row = 0; row < no_of_subrows; row++) {       //////// kyu?????
    //     for(int col = 0; col < no_of_subcols; col++) {
    //         totalRecSum += get_max_rec_sum(row,col); 
    //     }
    // }


    // thread creation
    std::vector<std :: thread> threads;
    for(int row = 0; row < no_of_subrows; row++) {
        for(int col = 0; col < no_of_subcols; col++) {
            threads.push_back(std::thread(annealing_thread, row, col));
        }
    }
    for(auto& th : threads) {
        th.join();
    }


    // cout<<"Total Score: "<<totScore<<endl;

    for(int i=0;i<no_of_subrows;i++){
        for(int j=0;j<no_of_subcols;j++){
            // bestScore[i][j]=curScore[i][j];                  //// best_B  ya Curr_B ?????
            int startx= i*recSize_inrow;
            int starty= j*recSize_incol;
            int endx = startx+recSize_inrow;
            int endy = starty+recSize_incol;
            for(int k=startx;k<endx;k++){
                for(int l=starty;l<endy;l++){
                    // cout<<newB[i][j][k-startx][l-starty]<<" ";
                    if(newB[i][j][k-startx][l-starty]) cout<<"#";
                    else cout<<".";
                }
                cout<<endl;
            }
            cout<<endl;
            cout<<endl;
            // break;
        }
        // break;
        cout<<endl;
        cout<<endl;
    }
    cout<<endl;
    // return 0; 


    freopen("./Boolean_B_Annealing.txt", "w", stdout);

	for(int i=0; i<Z; i++) {
         for(int j=0; j<Z; j++) { 
             int aa = i/((Z+no_of_subrows-1)/no_of_subrows); 
             int bb = j/((Z+no_of_subcols-1)/no_of_subcols); 
             int cc = i%((Z+no_of_subrows-1)/no_of_subrows); 
             int dd = j%((Z+no_of_subcols-1)/no_of_subcols); 
             cout << best_B[aa][bb][cc][dd] << " ";
        }
        cout << endl; 
    }
 
	fclose(stdout) ;

    return 0 ; 

	

    
}