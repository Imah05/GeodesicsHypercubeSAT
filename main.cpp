#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

unsigned int factorial(unsigned int n) {
    unsigned int result = 1;
    for (unsigned int i = 2; i <= n; ++i)
        result *= i;
    return result;
}

int edgeToVar(const vector<int>& vertex, int direction) {
    int result = 0;
    int n = vertex.size();
    int N = 1 << (n-1);

    for (int i = n-1; i >=0; --i) {
        if(i != direction)
        result = (result << 1) | vertex[i];
    }
    return N*direction + result + 1;
}

vector<int> geodesicToPreClause(const vector<int>& start, const vector<int>& perm){
    vector<int> preClause;
    vector<int> vertex = start;

    for(int dir : perm) {
        preClause.push_back(edgeToVar(vertex, dir));
        vertex[dir] = (vertex[dir] + 1) % 2;
    }
    return preClause;
}

void generateSAT(int n) {
    int N = 1 << (n-1);
    vector<int> startingVertex(n, 0);
    vector<int> perm(n, 0);

    ofstream outfile("SAT" + to_string(n) + ".txt");
    
    if (!outfile) 
    cerr << "Error opening file!\n";
    
    outfile << "p cnf " << N*n << " " << 2*n*N*factorial(n) << endl;

    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < n; j++)
            perm[j] = j;
        do {
            vector<int> preClause = geodesicToPreClause(startingVertex, perm);
            for (int k = 0; k <= n; ++k) {
                for (int m = 0; m < n; ++m)
                outfile << ((m < k) ? -preClause[m] : preClause[m]) << ' ';

                outfile << "0\n";

                if (k != 0 && k != n) {
                    for (int m = 0; m < n; ++m)
                    outfile << ((m < k) ? preClause[m] : -preClause[m]) << ' ';

                    outfile << "0\n";
                }
            }
        }
        while (next_permutation(perm.begin(), perm.end()));

        int ind = 0;
        while(startingVertex[ind] == 1) {
            startingVertex[ind] = 0;
            ++ind;
        }
        startingVertex[ind] = 1;
    }
    
    outfile.close();
    cout << "File SAT" + to_string(n) + ".txt has been created and written successfully!" << endl;
}

int main() {
    generateSAT(6);
    generateSAT(7);
}
