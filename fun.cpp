#include "fun.h"

void tweet::set_t_id(int tweet_id){
	t_id = tweet_id;
}
			
int tweet::get_t_id(){
	return t_id;
}

void tweet::set_u_id(int user_id){
	u_id = user_id;
}
			
int tweet::get_u_id(){
	return u_id;
}

void tweet::set_total_score(long double t_s){
	total_score = t_s;
}

long double tweet::get_total_score(){
	return total_score;
}
		
void tweet::set_words(vector<string> w){
	words.assign(w.begin(),w.end());
}
			
vector<string> tweet::get_words(){
	return words;
}

void tweet::set_crypto_ref(vector<int> cr){
	crypto_ref.assign(cr.begin(),cr.end());
}

vector<int> tweet::get_crypto_ref(){
	return crypto_ref;
}

void lexicon::set_word(string index){
	word = index;
}

string lexicon::get_word(){
	return word;
}

void lexicon::set_score(double grade){
	score = grade;
}

double lexicon::get_score(){
	return score;
}

user::user(){
	flag=0;
	assigned=0;
	for(int i=0; i<100; i++){
		current.push_back(-INFINITY);
		sent_vec.push_back(make_pair(-INFINITY,0));
	}
}

void user::set_user_id(int ui){
	user_id = ui;
}

int user::get_user_id(){
	return user_id;
}

void user::set_mytweets(vector<tweet> posts){
	mytweets.assign(posts.begin(),posts.end());
}

vector<tweet> user::get_mytweets(){
	return mytweets;
}

void cryptocurrency::set_c_id(int c){
	c_id = c;
}
		
int cryptocurrency::get_c_id(){
	return c_id;
}

void cryptocurrency::set_c_name(vector<string> c){
	c_name.assign(c.begin(),c.end());
}
		
vector<string> cryptocurrency::get_c_name(){
	return c_name;
}

vector<tweet> read_n_store_tweets(string filename){
	ifstream myfile;
	string line;
	vector<string> w;
	vector<tweet> t;
	myfile.open(filename);
	while(getline(myfile,line,'\n')){
		stringstream ss ( line );
		tweet mytweet;
		getline(ss,line,'\t');
		mytweet.set_u_id(atoi(line.c_str()));
		getline(ss,line,'\t');
		mytweet.set_t_id(atoi(line.c_str()));
		while(getline(ss,line,'\t')){
		w.push_back(line);
		}
		mytweet.set_words(w);
		w.clear();
		t.push_back(mytweet);
	}
	myfile.close();
	return t;
}

vector<cryptocurrency> read_n_store_cryptocurrency(string filename){
	ifstream myfile;
	int id=0;
	string line;
	vector<cryptocurrency> c;
	vector<string> names;
	myfile.open(filename);
	while(getline(myfile,line,'\n')){
		stringstream ss ( line );
		cryptocurrency mycrypto;
		mycrypto.set_c_id(id);
		id++;
		while(getline(ss,line,'\t')){
			names.push_back(line);
		}
		mycrypto.set_c_name(names);
		c.push_back(mycrypto);
		names.clear();
	}
	myfile.close();
	return c;
} 

vector<lexicon> read_n_store_lexicon(string filename){
	ifstream myfile;
	string line; 
	string index;
	vector<lexicon> l;
	myfile.open(filename);
	while(getline(myfile,line,'\n')){
		stringstream ss ( line );
		lexicon mylexicon;
		getline(ss,line,'\t');
		mylexicon.set_word(line);
		getline(ss,line,'\t');
		mylexicon.set_score(stold(line));
		l.push_back(mylexicon);
	}
	myfile.close();
	return l;
}

vector<user> make_users(string filename){
	ifstream myfile;
	string line; 
	int id=0,i=2;
	vector<user> u;
	myfile.open(filename);
	getline(myfile,line,'\t');
	id = atoi(line.c_str());
	user myuser;
	myuser.set_user_id(id);
	u.push_back(myuser);
	getline(myfile,line,'\n');
	while(getline(myfile,line,'\t')){
		if(atoi(line.c_str()) != id){
			id = atoi(line.c_str());
			user myuser;
			myuser.set_user_id(id);
			u.push_back(myuser);
		}
		getline(myfile,line,'\n');
	}

	myfile.close();
	return u;
} 

void assign_tweets_to_users(vector<user>& u,vector<tweet>& t){
	vector<tweet> owned;
	for(int i=0; i<u.size(); i++){
		for(int j=0; j<t.size(); j++){
			if(u[i].get_user_id() == t[j].get_u_id()){
				owned.push_back(t[j]);
			}
		}
		u[i].set_mytweets(owned);
		owned.clear();
	}
}

void grades(vector<tweet> &t,vector<lexicon> &l){
	int x = 0;
	long double s=0.0 , sum=0.0, r=0.0;
	const int alpha = 15;
	for(int i=0; i<t.size(); i++){
		for(int j=0; j<t[i].words.size(); j++){
			for(int z=0; z<l.size(); z++){
				x = t[i].words[j].compare(l[z].get_word());
				if(x == 0){
					sum += l[z].get_score();
				}
			}
		}
		r = pow(sum,2) + alpha;
		s = sum /sqrt(r) ;
		t[i].set_total_score(s);
		sum = 0;
		s = 0;
		r = 0;
	}
}

void find_crypto_ref(vector<tweet>& t,vector<cryptocurrency>& c){
	vector<int> coins;
	int comp=0;
	for(int i = 0; i < t.size(); i++){
		for(int j = 0; j < t[i].words.size(); j++){
			for(int z = 0; z < c.size(); z++){
				for(int x = 0; x < c[z].c_name.size(); x++){
					comp = t[i].words[j].compare(c[z].c_name[x]);
					if(comp == 0){
						coins.push_back(c[z].get_c_id());
					}
				}
			}
		}
		t[i].set_crypto_ref(coins);
		coins.clear();
	}

}

void make_sent_vec(vector<user>& u,vector<tweet>& t,vector<cryptocurrency>& c){
	vector<pair<int,long double>> r;
	double score = 0.0;
	int counter =0;
	for(int i=0; i< u.size(); i++){
		for(int j=0; j<u[i].mytweets.size(); j++){
			for(int z=0; z<u[i].mytweets[j].crypto_ref.size(); z++){
					r.push_back(make_pair(u[i].mytweets[j].crypto_ref[z],u[i].mytweets[j].get_total_score())); 
				}
			}
	
		for(int w=0; w<r.size(); w++){
			u[i].sent_vec[r[w].first].first = r[w].second;
			u[i].sent_vec[r[w].first].second = 1;
			if(u[i].current[r[w].first] == -INFINITY){
				u[i].current[r[w].first] = r[w].second;	
				score += r[w].second;
				counter++;
			}else{
				u[i].current[r[w].first] += r[w].second;
				score += r[w].second;
			}
		}
		for(int x=0; x<u[i].current.size(); x++){
			if(u[i].current[x] == -INFINITY){
				u[i].current[x] = score/counter;
			}
		}
		counter = 0;
		score = 0;
		r.clear();
	}

}


//-------------------------------------lsh cosine-----------------------------------------------------

void V_generator(int K,vector<vector <long double> > &V_vectors){
	vector<long double> v;
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator (seed);
	normal_distribution<long double> distribution(0.0,1.0);
	for(int j=0; j<K; j++){
		for(int i=0; i<100; i++){
			v.push_back(distribution(generator));
		} 
		V_vectors.push_back(v);
		v.clear();
	}
}

long double dot_product(vector<long double> v,vector<long double> u,int size){
    long double result = 0.0;
    for (int i = 0; i < size; i++)
        result += v[i]*u[i];
    return result;
}

vector<int> GC_generator(vector<long double> cur,vector<vector <long double> > V_vectors){
	long double dp=0;
	int h;
	vector<int> g;
	for(int i=0; i<V_vectors.size(); i++){
		dp=dot_product(V_vectors[i],cur,100);
		if(dp >= 0){
			h=1;
			g.push_back(h);
		}else{
			h=0;
			g.push_back(h);
		}
	} 

	return g;
}


long long int vec_to_num(vector<int> g){
	reverse(g.begin(),g.end());
   int mul =1,sum=0;
   for(int n:g){
   		sum += n*mul;
   		mul = mul*10;
   }
	return sum;
}


int bin_to_dec(long long int binary){
	int decimal=0 , i=0, remainder;
	while(binary!=0){
		remainder = binary%10;
		binary /= 10;
		decimal += remainder*pow(2,i);
		i++;
	}
	return decimal ;
}

long double euclidean(vector<long double> x,vector<long double> y){
	long double j=0.0,euclidean_distance=0.0,sum=0.0;
	for(int i=0; i<x.size(); i++){
		j = x[i] - y[i];
		sum += pow(j,2);
	}
	euclidean_distance = sqrt(sum);
	return euclidean_distance;
}

void preprocessing_cosine(vector<user>& u,int K,int L){
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
		for(int i=0; i<u.size(); i++){
			g = GC_generator(u[i].current,V_vectors_search[times]);
			converted = vec_to_num(g);
			hash = bin_to_dec(converted);
			g.clear();
			if(vmap[times].count(hash)){
				for(auto x:vmap[times].find(hash)->second){
					if(u[i].get_user_id() != x.get_user_id()){
						u[i].neighbours.push_back(x.get_user_id());
					}
				}
			}
		}

	}
}

long double similarity(vector<long double> x,vector<long double> y){
	long double dp,metro_x,metro_y,similarity;
	dp = dot_product(x,y,100);
	for(int i=0; i<100; i++){
		metro_x += pow(x[i],2);
		metro_y += pow(y[i],2);
	}
	metro_x = sqrt(metro_x);
	metro_y = sqrt(metro_y);
	similarity = dp/(metro_x * metro_y);
	return similarity;
}

void lsh_rec_a(vector<user>& u){
	double new_sent,z,sum_sim,final;
	for(int i=0; i<u.size(); i++){
		for(int j=0; j<u[i].current.size(); j++){
			if(u[i].sent_vec[j].first == -INFINITY){
				for(int z=0; z<u[i].neighbours.size(); z++){
					for(int x=0; x<u.size(); x++){
						if(u[i].neighbours[z] == u[x].get_user_id()){
							sum_sim += abs(similarity(u[i].current,u[x].current));
							new_sent += (similarity(u[i].current,u[x].current) * u[x].current[j]);
						}
					}				
				}
				z = 1/sum_sim;
				final = new_sent * z;
				u[i].current[j] = final;
				new_sent = 0;
				sum_sim = 0;
				final = 0;
				z = 0;
			}
		}
	}

}






			
