#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <pthread.h>
#include <sys/time.h>

using namespace std;

vector<double> sums;
vector<double> sums_sq;
double beta = 1;


void InitializeSums(vector<double>&);
void InitializeSumsSq(vector<double>&);
double GetMean(int, int);
double GetSumSq(int, int);
double GetSegmentCost(int, int);
double FindMin(vector<double>&, vector<int>&, int*);
int FindOptimalSegments(vector<double>&);

vector<double> data; //To store data points
vector<double> dp; //To store optimal costs
vector<string> dp_change; //To store segment end points



int main(int argc, char** argv){
	int num_points;
	
	if(argc != 2){
		cout << "Usage: <#num of data points>" << endl;
		exit(1);
	}
	
	num_points = atoi(argv[1]);
	
	for(int i = 0; i < num_points; i++){
		data.push_back(i + 1);
	} 
    
	int n = data.size();
    
	for(int i = 0; i < n + 1; i++){
    	dp.push_back(0);
    	dp_change.push_back("");
    	
    	sums.push_back(0);
    	sums_sq.push_back(0);
	}    
    
	InitializeSums(data);
	InitializeSumsSq(data);
	
	double wait_time = 0;
	double elapsed_time;
	struct timeval before, after;
	gettimeofday(&before,NULL);
	
  	FindOptimalSegments(data);
	
	cout << "Optimal cost values:" << endl;
	for(int i = 1; i <= n; i++){
		cout << dp[i] << " ";
	}
	
	gettimeofday(&after,NULL);
	elapsed_time = ( 1000000.0*(after.tv_sec - before.tv_sec) + (after.tv_usec - before.tv_usec))/1000000.0;
	wait_time += elapsed_time;
	
	cout << "\n\nelapsed time: " << wait_time << endl;
	
}

int FindOptimalSegments(vector<double>& data_points){
	
	vector<double> tempList;
	vector<int> tempPos;
	int position;
	
	dp[0] = beta;
	
	for(int i = 1; i < data_points.size() + 1; i++){
		for(int j = 0; j < i; j++){
			double val = dp[j] + GetSegmentCost(j + 1, i) + beta;
			tempList.push_back(val);
			tempPos.push_back(j + 1);
		}
		dp[i] = FindMin(tempList, tempPos, &position);
		ostringstream str1; 
		str1 << i;
		dp_change[i] = dp_change[position - 1] == "" ? str1.str() : (dp_change[position - 1] + " " + str1.str());
		tempList.clear();
		tempPos.clear();
	}
	
	return 0;
}

void InitializeSums(vector<double>& data_points){
	double total = 0;
	for(int x = 0; x < data_points.size() + 1; x++){
		if(x == 0){
			continue;
		}
		total += data_points[x - 1];
		sums[x] = total;
	}
}

void InitializeSumsSq(vector<double>& data_points){
	double total = 0;
	for(int x = 0; x < data_points.size() + 1; x++){
		if(x == 0){
			continue;
		}
		total += data_points[x - 1] * data_points[x - 1];
		sums_sq[x] = total;
	}
}

double GetMean(int initial, int final){
	double mean = 0;

	int length = final - initial + 1;    
	if(initial == 0)
        mean = sums[final] / length;
    else
        mean = (sums[final] - sums[initial - 1]) / length;
    
	return mean;	
}

double GetSumSq(int initial, int final){
	if ((initial == final) || (initial == 0))
        return sums_sq[final];
    else
        return sums_sq[final] - sums_sq[initial - 1];	
}

double GetSegmentCost(int initial, int final){
	double mu = GetMean(initial, final);
    int length = final - initial + 1;
    double total = mu * length;

    return ((-2 * mu * total) + (length * mu * mu));
}

double FindMin(vector<double>& data_points, vector<int>& positions, int* position){
	if (data_points.size() == 0)
        return -1 * beta;
        
    double least = data_points[0];
    *position = positions[0];
	    
	for(int i = 0; i < data_points.size(); i++){
        if(data_points[i] <= least){
        	least = data_points[i];
            *position = positions[i];            
		}
	}
	return least;
}

