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
using namespace std;
#define P 20

class tweet{
		private:
			int t_id,u_id;
			long double total_score;
		public:
			vector<string> words;
			vector<int> crypto_ref;
			void set_t_id(int tweet_id);
			int get_t_id();
			void set_u_id(int user_id);
			int get_u_id();
			void set_total_score(long double t_s);
			long double get_total_score();
			void set_words(vector<string> w);
			vector<string> get_words();
			void set_crypto_ref(vector<int> cr);
			vector<int> get_crypto_ref();
};

class lexicon{
	private:
		double score;
		string word;
	public:
		void set_word(string index);
		string get_word();
		void set_score(double grade);
		double get_score();
};

class user{
	private:
		int user_id,flag,assigned,belong;
	
	public:
		user();
		vector<tweet> mytweets;
		vector<long double> current;
		vector<pair<long double,int>> sent_vec; //size = #cryptocurrencies	
		void set_user_id(int ui);
		vector<int> neighbours;
		int get_user_id();
		void set_mytweets(vector<tweet> posts);
		vector<tweet> get_mytweets();
};

class cryptocurrency{
	private:
		int c_id;
			
	public:
		vector<string> c_name;
		void set_c_id(int c);
		int get_c_id();
		void set_c_name(vector<string> c);
		vector<string> get_c_name();
};



void set_t_id(int tweet_id);
			
int get_t_id();

void set_u_id(int user_id);
			
int get_u_id();

void set_total_score(long double t_s);

long double get_total_score();
		
void set_words(vector<string> w);
			
vector<string> get_words();

void set_crypto_ref(vector<int> cr);

vector<int> get_crypto_ref();

void set_word(string index);
		
string get_word();

void set_score(double grade);

double get_score();

void set_user_id(int ui);
		
int get_user_id();
	
void set_mytweets(vector<tweet> posts);

vector<tweet> get_mytweets();

void set_c_id(int c);
		
int get_c_id();
		
void set_c_name(vector<string> c);
		
vector<string> get_c_name();

void set_owner(int o);

int get_owner();

vector<tweet> read_n_store_tweets(string filename);

vector<lexicon> read_n_store_lexicon(string filename);

vector<cryptocurrency> read_n_store_cryptocurrency(string filename);

void grades(vector<tweet> &t,vector<lexicon> &l);

vector<user> make_users(string filename);

void assign_tweets_to_users(vector<user>& u,vector<tweet>& t);

void find_crypto_ref(vector<tweet>& t,vector<cryptocurrency>& c);

void make_sent_vec(vector<user>& u,vector<tweet>& t,vector<cryptocurrency>& c);

void V_generator(int K,vector<vector <long double> > &V_vectors);

long double dot_product(vector<long double> v,vector<long double> u,int size);

vector<int> GC_generator(vector<long double> coor,vector<vector <long double> > V_vectors);

long long int vec_to_num(vector<int> g);

int bin_to_dec(long long int binary);

long double euclidean(vector<long double> x,vector<long double> y);

void preprocessing_cosine(vector<user>& u,int K,int L);

long double similarity(vector<double> x,vector<double> y);

void lsh_rec_a(vector<user>& u);
