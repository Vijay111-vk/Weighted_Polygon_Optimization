# Weighted Polygon Optimization

## Problem Statement

Given a set of points with associated weights (positive and negative), the goal is to:
- Construct a polygon that maximizes the sum of weights of enclosed points
- Ensure the polygon has at most 1000 vertices
- Account for points forming clusters based on adjacency (shared sides)

## Approach

Our solution uses a combination of cluster analysis and dynamic programming:

### Cluster Analysis
1. **Cluster Identification**
   - Identify positive clusters (connected points with positive weights)
   - Identify holes (clusters of negative weight points)
   - Map relationships between positive clusters and holes

2. **Base Solution**
   - Initially include all positive clusters in the polygon
   - Calculate total vertex count from positive clusters
   - Identify if vertex reduction is needed (if count > 1000)

3. **Hole Analysis**
   - Analyze each hole for potential inclusion
   - Including a hole can:
     - Requires at least 2 additional vertices for "tubing" to connect it to the main polygon
     - Reduce total vertex count by merging boundaries 
     - Add negative weight to the total sum
     - Potentially simplify the polygon shape

### Optimization Strategy
- Knapsack-style Dynamic Programming approach to optimize hole selection
- Objective function considers:
  - Reduction in vertex count by including the hole
  - Weight penalty from negative points
  - Impact on overall polygon structure

## Implementation Details

### Dynamic Programming Solution
- **DP State Definition**
  - State: (current_vertices, selected_holes)
  - Transitions: Include/exclude each candidate hole

- **Constraints**
  - Final vertex count ≤ 1000
  - Maximize total weight

### Grid Compression

- Compress original grid with 100:1 ratio
- Preserve relative positions of all points
- Maintain cluster structures while reducing computational complexity
- Map final solution back to original coordinates

The compression step is crucial for making the DP optimization tractable while preserving all the necessary spatial relationships between points.

### Solution Steps
1. Calculate base metrics for positive clusters 
2. Identify candidate holes for inclusion
3. Apply DP to select optimal set of holes
4. Construct final polygon boundary

### Polygon Construction
Our polygon generator takes a compressed Boolean grid as input and constructs a continuous shape by:
- Connecting all points marked as true using narrow tubes
- Ensuring tubes do not occupy any false grid spaces
- Eliminating holes by extending connections to enclose outer boundaries

<img src="https://github.com/user-attachments/assets/dc5bd542-f325-4b72-89c7-18ffb0a3211b" width="400">

### Checker
Validates that:
1. Number of edges ≤ 1000
2. No two edges intersect
3. Exactly 2 edges incident on each vertex
4. All edges are parallel to axes
5. All vertices lie on a single cycle of edges

### Alternative Approach: Column-Based Optimization
- Work directly with original grid values
- Maintain column-wise sums
- Select optimal height for each column
- Choose top 250 columns based on maximum suffix sums

## Time Complexity
- Cluster identification: O(N*N), where N is Size of the compressed gird
- DP solution: O(H * V) where:
  - H = number of holes
  - V = number of vertices to reduce
- Column-based approach: O(N * M) for column sums, O(M log M) for selection, where N and M are without compression

## Results and Benefits
- Achieves vertex count within 1000-vertex limit
- Maximizes total weight of included points
- Optimizes polygon shape for given constraints
- Efficient solution through DP approach

## Running Instructions

To run the solution:

1. Make sure you have Python installed on your system
2. Clone this repository:
   ```
   git clone https://github.com/BrajeshMuwel17/Weighted-Polygon-Optimization.git
   ```

3. Run the Python script:
   ```
   python Run.py [input_file] [output_file]
   ```
   
   Where:
   - `input_file` is the path to the input file containing the point grid
   - `output_file` is the path where the polygon solution will be saved

### Input Format
The input file should contain:
- First line: A single integers N (Number of Positive points)
- Next N lines: 3 space-separated integers x, y, w (0 <= x,y <= 10,000 , w>=0) representing x coordinate, y coordinate, weight at point (x,y)
- Next line : A single integers M  (Number of Negative points)
- Next M lines: 3 space-separated integers x, y, w (0 <= x,y <= 10,000 , w<0) representing x coordinate, y coordinate, weight at point (x,y)
- Sum of N and M combined does not exceed 10,000 ( 1 <= N+M <= 10,000)

### Output Format
The output file will contain:
- First line: A single integer representing the answer (total sum of weights enclosed)
- Next line: Two integers - number of edges (E) and number of vertices (V) in the optimal polygon (1<= V,E <=1000)
- Next V lines: Coordinates of each vertex of polygon (x,y)  (0<= x,y <= 10,000)
- Additionally coordinates of Polygon can be floating point type