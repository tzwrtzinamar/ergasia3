#include "clustering.h"

int main(int argc,char* argv[]){
	string input_file = "";
	string output_file = "";
	int rec_type,number_of_clusters=20,K=4,L=2;
	vector<tweet> tweets;
	vector<lexicon> lexiko;
	vector<user> real_users;
	vector<user> iconic_users;
	vector<cryptocurrency> crypto;
	vector<point> points;
	vector<point> centroids;
	vector<point> centroids_a;
	vector<point> centroids_b;
	vector<cluster> systades;
	vector<cluster> systades_a;
	vector<cluster> systades_b;
	vector<point> converted_real;
	vector<point> converted_iconic;
	vector<point> total;

	for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-d") {
            input_file =  argv[i+1];
        } else if (string(argv[i]) == "-o") {
        	output_file = argv[i+1];
        }
    }

    tweets = read_n_store_tweets(input_file);
    lexiko = read_n_store_lexicon("vader_lexicon.csv");
    grades(tweets,lexiko);
    crypto = read_n_store_cryptocurrency("coins_queries.csv");
    find_crypto_ref(tweets,crypto);
    real_users =  make_users(input_file);
    assign_tweets_to_users(real_users,tweets);
    make_sent_vec(real_users,tweets,crypto);
 	points = read_n_store("small.csv");
	centroids = random_init(number_of_clusters, points);
	systades =  make_clusters(centroids);
	Lloyds(points,systades,0);
	cluster_mean(points,systades);
	update_Lloyds(points,systades,0);
	iconic_users = make_iconic_users(systades,tweets);
    make_sent_vec(iconic_users,tweets,crypto);


    cout << " Which type of recommendation do you want? " << endl;
	cin >> rec_type;
	if(rec_type == 1){
    	preprocessing_cosine(real_users,K,L);
    	lsh_rec_a(real_users);
    	preprocessing_cosine_b(iconic_users,real_users,K,L);
    	lsh_rec_a(iconic_users);

	}else{
		converted_real = convert_users_to_points(real_users);
		centroids_a = random_init(number_of_clusters, converted_real);
		systades_a =  make_clusters(centroids_a);
		Lloyds(converted_real,systades,0);
		cluster_mean(converted_real,systades_a);
		update_Lloyds(converted_real,systades_a,0);
		cluster_rec_a(converted_real,systades_a);
		converted_iconic = convert_users_to_points(iconic_users);
		for(int i=0; i<converted_real.size(); i++){
			total.push_back(converted_real[i]);
		}
		for(int j=0; j<converted_iconic.size(); j++){
			total.push_back(converted_iconic[j]);
		}
		centroids_b = random_init(number_of_clusters, total);
		systades_b =  make_clusters(centroids_b);
		Lloyds(total,systades_b,0);
		cluster_mean(total,systades_b);
		update_Lloyds(total,systades_b,0);
		cluster_rec_a(total,systades_b);
	}
	return 0;
}
