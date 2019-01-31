#include "clustering.h"


void point::setID(int p_id){
	id = p_id;
}

int point::getID(){
	return id;
}

void point::setFlag(int F){
	flag = F;
}

int point::getFlag(){
	return flag;
}

void point::setassigned(int a){
	assigned = a;
}

int point::getassigned(){
	return assigned;
}

void point::setbelong(int b){
	belong = b;
}

int point::getbelong(){
	return belong;
}

void point::setCoordinates(vector<long double> p){
	coordinates.assign(p.begin(),p.end());
}

vector<long double> point::getCoordinates(){
	return coordinates;
}

//class cluster:functions
void cluster::setCentroid(point mypoint){
	centroid = mypoint;
}

point cluster::getCentroid(){
	return centroid;
}

void cluster::setCluster_points(point v_point){
	cluster_points.push_back(v_point);
}

vector<point> cluster::getCluster_points(){
	return cluster_points;
}

vector<point> read_n_store(string filename){
	ifstream myfile;
	string line;
	string::size_type sz; 
	vector<long double> p;
	vector<point> v;
	myfile.open(filename);
	while(getline(myfile,line,'\n')){
		stringstream ss ( line );
		point mypoint;
		mypoint.setFlag(0);
		mypoint.setassigned(0);
		getline(ss,line,',');
		mypoint.setID(atoi(line.c_str()));
		while(getline(ss,line,',')){
			p.push_back(stold(line,&sz));
		}
		mypoint.setCoordinates(p);
		p.clear();
		v.push_back(mypoint);
	}
	myfile.close();
	return v;
}

long double cosine(vector<long double> x,vector<long double> y){
	long double multiply=0.0,metro_x=0.0,metro_y=0.0,cosine=0.0;
	for(int i=0; i<x.size(); i++){
		multiply += x[i]*y[i];
		metro_x += pow(x[i],2);
		metro_y += pow(y[i],2);
	}

	metro_x = sqrt(metro_x);
	metro_y = sqrt(metro_y);
	cosine = 1 - (multiply/(metro_x*metro_y));
	return cosine ; 
}


vector<point> random_init(int number_of_clusters,vector<point>& v){
	vector<point> centroids;
	int temp=0;
	random_device rd; 
    mt19937 eng(rd()); 
    uniform_int_distribution<> distr(1,v.size());
	 for(int n=0; n<number_of_clusters; ++n){
	 	temp = distr(eng);
  		v[temp].setFlag(1);
  		centroids.push_back(v[temp]);
  	}

    return centroids;
}

vector<cluster> make_clusters(vector<point>& centroids){
	vector<cluster> c;
	for(int i=0; i<centroids.size(); i++){
		cluster mycluster;
		mycluster.setCentroid(centroids[i]);
		c.push_back(mycluster);
	}
	return c;
}

void Lloyds(vector<point>& v,vector<cluster>& c,int metric_function){
	int id=0,x,counter=0;
	long double distance = 0.0 , min = 0.0;
	vector<long double> min_distance;
	for(int i=0; i<v.size(); i++){
		if(v[i].getFlag() != 1){
			for(int j=0; j<c.size(); j++){
				if(metric_function == 0){
					distance = euclidean(v[i].getCoordinates(),c[j].getCentroid().getCoordinates());
					min_distance.push_back(distance);
				}else{
					distance = cosine(v[i].getCoordinates(),c[j].getCentroid().getCoordinates());
					min_distance.push_back(distance);
				} }
			min = min_distance[0];
			for(x=0; x<min_distance.size(); x++){
				if(min_distance[x] <= min){
					min = min_distance[x];
					id = x;	// id einai i thesi tou kentrou sto vector me ta kentra
				}
			}
			v[i].setassigned(1);
			v[i].setbelong(id);
			c[id].setCluster_points(v[i]);
			min_distance.clear();

		}
	}

}

void cluster_mean(vector<point>& v,vector<cluster>& c){
	vector<point> points;
	vector<long double> x;
	vector<long double> mean;
	long double sum=0.0;
	for(int i=0; i<c.size(); i++){
		points = c[i].getCluster_points();
		for(int k=0; k<203; k++){
			for(int j=0; j<points.size(); j++){
				x = points[j].getCoordinates();
				sum += x[k];
			}
			sum = sum/points.size();
			mean.push_back(sum);
			sum = 0;
		}
		point mypoint;
		mypoint.setFlag(1);
		mypoint.setCoordinates(mean);
		c[i].setCentroid(mypoint);
		mean.clear();
		points.clear();
	}

}

void update_Lloyds(vector<point>& v,vector<cluster>& c,int metric_function){
	for(int i=0; i<v.size(); i++){
			v[i].setFlag(0);
			v[i].setassigned(0);
			v[i].setbelong(0);
	}
	for(int i=0; i<2; i++){
		cluster_mean(v,c);
		for(int j=0; j<c.size(); j++){
			c[j].cluster_points.clear();
		}
		Lloyds(v,c,metric_function);
	}
}

vector<user> make_iconic_users(vector<cluster>& c,vector<tweet>& t){
	vector<user> iconic;
	for(int i=0; i<c.size(); i++){
		user myuser;
		myuser.set_user_id(i);
		for(int j=0; j<c[i].cluster_points.size(); j++){
			for(int y=0; y<t.size(); y++){
				if(c[i].cluster_points[j].getID() == t[y].get_t_id()){
					myuser.mytweets.push_back(t[y]);
				}
			}
		}
		iconic.push_back(myuser);
	}
	return iconic;
}

void preprocessing_cosine_b(vector<user>& u,vector<user>& f,int K,int L){
	long long int converted=0;
	int hash=0;
	long double e=0.0;
	vector<vector <long double> > V_vectors;
	vector<int> g;
	unordered_map < int,vector<user> > umap;
	vector<unordered_map < int,vector<user> > >vmap;
	vector<vector<vector <long double> > > V_vectors_search;
	
	for(int times=0; times<L; times++){
		V_generator(K,V_vectors);
		V_vectors_search.push_back(V_vectors);
		for(int i=0; i<u.size(); i++){
			g = GC_generator(u[i].current,V_vectors);
			converted = vec_to_num(g);
			hash = bin_to_dec(converted);
			umap[hash].push_back(u[i]);
			g.clear();
		}
		V_vectors.clear();
		vmap.push_back(umap);
		umap.clear();
	}

	for(int times = 0; times<L; times++){
		for(int i=0; i<f.size(); i++){
			g = GC_generator(f[i].current,V_vectors_search[times]);
			converted = vec_to_num(g);
			hash = bin_to_dec(converted);
			g.clear();
			if(vmap[times].count(hash)){
				for(auto x:vmap[times].find(hash)->second){
					if(f[i].get_user_id() != x.get_user_id()){
						f[i].neighbours.push_back(x.get_user_id());
					}
				}
			}else{
				cout << "no neighbours" << endl;
			}
		}

	}
}

//Clustering Recommendation

vector<point> convert_users_to_points(vector<user>& u){
	vector<point> users;
	for(int i=0; i<u.size(); i++){
		point another_point;
		another_point.setID(u[i].get_user_id());
		another_point.setCoordinates(u[i].current);
		another_point.setFlag(0);
		another_point.setassigned(0);
		users.push_back(another_point);
	}
	return users;
}

long double similarity_euclidean(vector<long double> x,vector<long double> y){
	long double similarity_euclidean,sum =0.0;
	for(int i=0; i<x.size(); i++){
		for(int j=0; j<y.size(); j++){
			sum += pow(x[i]-y[j],2);
		}
	}
	similarity_euclidean = 1/(1+sqrt(sum));
	return similarity_euclidean;
}

void cluster_rec_a(vector<point>& u,vector<cluster>& c){
	double new_sent,z,sum_sim,final;
	for(int i=0; i<u.size(); i++){
		for(int j=0; j<u[i].coordinates.size(); j++){
			if(u[i].coordinates[j] == -INFINITY){
				for(int z=0; z<c[u[i].getbelong()].cluster_points.size(); z++){
					sum_sim += abs(similarity_euclidean(u[i].coordinates,c[u[i].getbelong()].cluster_points[z].coordinates));
					new_sent += (similarity_euclidean(u[i].coordinates,c[u[i].getbelong()].cluster_points[z].coordinates) * c[u[i].getbelong()].cluster_points[z].coordinates[j]);
				}
				z = 1/sum_sim;
				final = new_sent * z;
				u[i].coordinates[j] = final;
				new_sent = 0;
				sum_sim = 0;
				final = 0;
				z = 0;
			}
		}
	}

}
