#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <sstream>
#include <unistd.h>
#include<string.h>
#include<conio.h>
#include<windows.h>



using namespace std;

/* Unsupervised Learning utilized with a heuristic algorithm to extract features from raw data
   Data was classified by creating labels that fit bounds of data based on gait kinematics.
   All labels are generated in real time with no future data points being referenced and timing out
   based on the millisecond time data given */
vector <pair <vector <int>, string> > gaitHeuristic(vector < int > ts, vector < int > ax, vector < int > az, vector < int > gy){
	vector <pair <vector <int>, string> > trained_data; //Stores ax, az, gy and label data
	for (int i = 0 ; i < ts.size() ; i++){
		if(i > 1){ //Prevents index out of range
			//Heel-Strike Case - Signals the start of a gait cycle
			if (abs(ax[i]) > 6500 && (az[i] < 3500 || az[i] > 10000) && (gy[i] - gy[i-1]) > 0){
				cout <<"Frame (ms): " << ts[i] << "  Gait Cycle Position: " << "Heel Strike" << endl;
				vector<int> data = {ax[i],az[i],gy[i]};
				pair<vector<int>,string> p(data,"Heel-Strike");
				trained_data.push_back(p);
			}
			//Heel-Off Case
			else if (abs(ax[i] - ax[i-1]) < 1000 && abs(ax[i] - ax[i-1]) > 50 && abs(az[i] - az[i-1]) < 2000 && abs(az[i] - az[i-1]) > 0 && gy[i] < 8000 && gy[i] > 500){
				cout << "Time (ms): " << ts[i] << "  Gait Cycle Position: " << "Heel Off" << endl;
				vector<int> data = {ax[i],az[i],gy[i]};
				pair<vector<int>,string> p(data,"Heel-Off");
				trained_data.push_back(p);
			}

			//Rest Case
			else if (abs(ax[i] - ax[i-1]) < 700 && ax[i] < -2650 && abs(az[i] - az[i-1]) < 360 && abs(gy[i] - gy[i-1]) < 225 && gy[i] < 800){
				cout << "Time (ms): " << ts[i] << "  Gait Cycle Position: " << "Rest" << endl;
				vector<int> data = {ax[i],az[i],gy[i]};
				pair<vector<int>,string> p(data,"Rest");
				trained_data.push_back(p);
			}
			//Foot - Flat
			else if (ax[i] < -1000 && abs(ax[i] - ax[i-1]) < 2000 && abs(ax[i] - ax[i-1]) > 350 && abs(az[i] - az[i-1]) < 3800 && abs(gy[i] - gy[i-1]) < 500 && abs(gy[i]) < 3500){
				cout << "Time (ms): " << ts[i] << "  Gait Cycle Position: " << "Flat Foot" << endl;
				vector<int> data = {ax[i],az[i],gy[i]};
				pair<vector<int>,string> p(data,"Flat-Foot");
				trained_data.push_back(p);
			}
			// Mid-Swing
			else if ((ax[i] > 1000 || ax[i] < -200) && az[i] < 33000  && abs(az[i] - az[i-1]) > 125 && abs(az[i] - az[i-1]) < 20000 && abs(gy[i]) < 11000){
				cout << "Time (ms): " << ts[i] << "  Gait Cycle Position: " << "Mid Swing" << endl;
				vector<int> data = {ax[i],az[i],gy[i]};
				pair<vector<int>,string> p(data,"Mid-Swing");
				trained_data.push_back(p);
			}
			// Toe-Off Case
			else if ((ax[i] > -1000 || ax[i] < -3000) && az[i] > 5500 && az[i] < 18000 && gy[i] > 8000){
				cout << "Time (ms): " << ts[i] << "  Gait Cycle Position: " << "Toe Off" << endl;
				vector<int> data = {ax[i],az[i],gy[i]};
				pair<vector<int>,string> p(data,"Toe-Off");
				trained_data.push_back(p);
			}
		}

		//Real time sampling
		if(i > 0){
			usleep((ts[i]-ts[i-1])*1000);
		}
		else{
			usleep(ts[i]*1000);
		}
	}
	return trained_data;
}
/* Euclidean Distance Formula*/
double euc_dist(int ax, int az, int gy, int ax_t, int az_t, int gy_t){
	return sqrt(pow(ax_t - ax, 2) + pow(az_t - az, 2) + pow(gy_t - gy, 2));
}

/* 1-nearest neighbour classifier (k=1) which allows an easy classification of data from the
   training data of walkData1 and any other walking data set given to it (i.e don't have to
   optimize the heuristic algorithm for every dataset being tested) */
vector <string> kNN (vector <pair <vector <int>, string> > trained_data ,vector < int > ts, vector < int > ax, vector < int > az, vector < int > gy){
	int min_dist_index = 0;
	double min_dist = 0;
	vector <string> labels;
	for (int i = 0; i < ts.size() ; i++){

		for(int j = 0; j < trained_data.size(); j++){
			if (euc_dist(ax[i],az[i],gy[i],trained_data[j].first[0],trained_data[j].first[1],trained_data[j].first[2]) < min_dist || min_dist == 0){
				min_dist_index = j;
				min_dist = euc_dist(ax[i],az[i],gy[i],trained_data[j].first[0],trained_data[j].first[1],trained_data[j].first[2]);
			}
		}

		cout << "Frame: " << ts[i] << "  Gait Cyle Position: " << trained_data[min_dist_index].second << endl;
		labels.push_back(trained_data[min_dist_index].second);

		if(i > 0){
			usleep((ts[i]-ts[i-1])*1000);
		}
		else{
			usleep(ts[i]*1000);
		}

	}
	return labels;
}

int main(int argc, char** argv) {
	ifstream fin1("D:/Downloads/Desktop/data_gait.csv");
	ifstream fin2("D:/Downloads/Desktop/data2_gait.csv");
	fin1.ignore();
	fin2.ignore();
	string line1;
	string line2;

	vector < int > arr1;
	vector < int > arr2;

	vector < int > ts1;
	vector < int > ax1;
	vector < int > az1;
	vector < int > gy1;

	vector < int > ts2;
	vector < int > ax2;
	vector < int > az2;
	vector < int > gy2;

	cout << "Heuristic Classification for walkData1:" << endl;

	while(getline(fin1,line1)){
		char* csv_val;

		char * writable = new char[line1.size() + 1];
		std::copy(line1.begin(), line1.end(), writable);
		writable[line1.size()] = '\0';


		csv_val = strtok(writable, ",");
		while (csv_val != NULL){
		    arr1.push_back(atof(csv_val));
		    csv_val = strtok (NULL, ",");
		}
	}
	fin1.close();
	while(getline(fin2,line2)){
		char* csv_val;

		char * writable = new char[line2.size() + 1];
		std::copy(line2.begin(), line2.end(), writable);
		writable[line2.size()] = '\0';


		csv_val = strtok(writable, ",");
		while (csv_val != NULL){
		    arr2.push_back(atof(csv_val));
		    csv_val = strtok (NULL, ",");
		}
	}
	fin2.close();

	for(int i = 22; i < arr1.size(); i+=20){
		if((i-22) % 20 == 0){
			ts1.push_back(arr1[i]);
			ax1.push_back(arr1[i+1]);
			az1.push_back(arr1[i+3]);
			gy1.push_back(arr1[i+5]);
		}
	}
	for(int i = 22; i < arr2.size(); i+=20){
		if((i-22) % 20 == 0){
			ts2.push_back(arr1[i]);
			ax2.push_back(arr1[i+1]);
			az2.push_back(arr1[i+3]);
			gy2.push_back(arr1[i+5]);
		}
	}
	vector <pair <vector <int>, string> > trained_data = gaitHeuristic(ts1,ax1,az1,gy1);
	cout << "\nkNN Classification results for walkData2: " << "\n\n";
	vector <string> labels = kNN(trained_data, ts2, ax2, az2, gy2);

	/* Data output to be used with gnuplot to show Heel-stike positions (including all positions overlaps
	   too much with a lot of labels */
	ofstream ofs ("data.dat",ofstream::out);
	for(int i = 0; i < labels.size(); i++){
		if(labels[i] == "Heel-Strike"){
			ofs << labels[i] << "	" << ts2[i] << "	" << gy2[i] << "\n";
		}
		else{
			ofs << "	" << ts2[i] << "	" << gy2[i] << "\n";
		}
	}
    getch();
	return true;

}
