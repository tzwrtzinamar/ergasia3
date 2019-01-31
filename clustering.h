#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <unordered_map>
#include <algorithm>
#include <unordered_map>
#include <boost/timer.hpp>
#include "fun.h"
using namespace std;

class point{
	private:
		int id,flag,assigned,belong;

	public:
		vector<long double> coordinates;
		void setID(int p_id);
		int getID();
		void setFlag(int F);
		int getFlag();
		void setassigned(int a);
		int getassigned();
		void setbelong(int b);
		int getbelong();
		void setCoordinates(vector<long double> p);
		vector<long double> getCoordinates();
};

class cluster{
	private:
		point centroid;
		long double cluster_silhouette;
	public:
		vector<point> cluster_points;
		void setCentroid(point mypoint);
		point getCentroid();
		void setCluster_points(point v_point);
		vector<point> getCluster_points();
};

void setID(int p_id);

int getID();

void setFlag(int F);

int getFlag();

void setassigned(int a);

int getassigned();

void setbelong(int b);

int getbelong();

void setCoordinates(vector<long double> p);

vector<long double> getCoordinates();

void setCentroid(point mypoint);

point getCentroid();

void setCluster_points(point v_point);

vector<point> getCluster_points();

vector<point> read_n_store(string filename);

long double euclidean(vector<long double> x,vector<long double> y);

long double cosine(vector<long double> x,vector<long double> y);

vector<point> random_init(int number_of_clusters,vector<point> &v);

vector<point> kmeans_init(int number_of_clusters,vector<point>& v,int metric_function);

vector<cluster> make_clusters(vector<point>& centroids);

void Lloyds(vector<point>& v,vector<cluster>& c,int metric_function);

void cluster_mean(vector<point>& v,vector<cluster>& c);

void update_Lloyds(vector<point>& v,vector<cluster>& c,int metric_function);

vector<user> make_iconic_users(vector<cluster>& c,vector<tweet>& t);

vector<point> convert_users_to_points(vector<user>& u);

long double similarity_euclidean(vector<long double> x,vector<long double> y);

void cluster_rec_a(vector<point>& u,vector<cluster>& c);

void preprocessing_cosine_b(vector<user>& u,vector<user>& f,int K,int L);
