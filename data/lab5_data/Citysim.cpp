#include <iostream>
#include <iomanip>
#include <istream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <set>
#include <map>
#include <limits>
#include <stack>
using namespace std;

class city {
	public:
		string get_name() { return name; }
		string get_type() { return type; }
		int    get_zone() { return zone; }
		int    get_pop()  { return  pop; }
		float  get_lat()  { return  lat; }
		float  get_lon()  { return  lon; }

		friend istream & operator>>(istream &, city&);
		friend ostream & operator<<(ostream & out, city outcity);
	private:
		string name, type;
		int zone, pop;
		float lat, lon;
};

istream & operator>>(istream & in, city& incity) { 
	
	in >> incity.zone >> incity.name >> incity.type >> incity.lat >> incity.lon >> incity.pop;
	return in;

}

ostream & operator<<(ostream & out, city outcity) { 

	out  << setw(18)  << left << outcity.name  << setw(12) << left << outcity.type
		 << setw(2)   << left << outcity.zone  << setw(10) << right << outcity.pop
		 << setw(8)   << fixed << setprecision(2) << outcity.lat
		 << setw(8)   << fixed << setprecision(2) << outcity.lon   << endl;

	return out;

}

class travelcost {

	public:
		travelcost(vector<city> cities) {
			for (int i = 0; i < (int)cities.size(); i++) {
				for (int j = 0; j <= i; j++) {
					//need to get the latitudes and longitudes of both cities
					//use haversine formula to get the internal angle
					float d_lat = sin((cities[i].get_lat() - cities[j].get_lat()) * M_PI / 360);
					float d_lon = sin((cities[i].get_lon() - cities[j].get_lon()) * M_PI / 360);
					float dist  = 2 * asin( 
							sqrt( d_lat * d_lat + (cos(cities[i].get_lat() * M_PI / 180) * cos(cities[j].get_lat() * M_PI / 180) * d_lon * d_lon)));
					dist = dist * 3982.0;
					dist = 25.0 * round (dist / 25.0);
					table[0].push_back(dist);

					//now that we have the distance between the two cities
					//find the time it takes to get to each of them from each other directly
					//figure out how to push values back first
					if (cities[i].get_type() == "REGIONAL") {
						if (cities[j].get_type() == "REGIONAL") {
							table[1].push_back(dist / 65.0);
						}
						else if (cities[j].get_type() == "GATEWAY") {
							table[1].push_back(dist / 520.0);
						}
					}
					else if (cities[i].get_type() == "GATEWAY") {
						table[1].push_back(dist / 520.0);
					}

				}
			}
		}

		void write_traveldistance(vector<city> &cities) {
			ofstream outfile;
			outfile.open("travel_distance_yours.txt");

			outfile << "DISTANCE TABLE: \n" << endl;
			for (int i = 1; i < (int)cities.size(); i++) {
				for (int j = 0; j < i; j++) {
					string outname = cities[i].get_name() + " to " + cities[j].get_name();
					outfile << setw(3) << setfill(' ') << right << i << " " 
						<< setw(40) << setfill('.') << left << outname 
						<< " " << setprecision(1) << setw(6) << setfill(' ') << right << fixed 
						<< table[0].at(i*(i + 1)/2 + j) << " miles" << endl;
				}
				outfile << endl;
			}
		}

		void write_traveltime(vector<city> & cities) {
			ofstream outfile;
			outfile.open("travel_time_yours.txt");

			for (int i = 1; i < (int)cities.size(); i++) {
				for (int j = 0; j < i; j++) {
					string outname = cities[i].get_name() + " to " + cities[j].get_name();
					outfile << setw(3) << setfill (' ') << right << i << " " 
						<< setw(40) << setfill('.') << left << outname 
						<< " " << setprecision(1) << fixed 
						<< table[1].at(i*(i + 1)/2 + j) << " hours" << endl;
				}
				outfile << endl;
			}
		}

		float &operator () (int mode, int i, int j) {
			if (i < j)
				return table[mode].at(j*(j + 1)/2 + i);
			else
				return table[mode].at(i*(i + 1)/2 + j);
		};
	
	private:
		//distance will be index 0
		//time will be index 1
		vector<float> table[2];

};



//read in the info using getline
//use either peek or stringstream to check if the first character is comment
//or to check if its the newline for emtpy line
void read_cityinfo(vector<city> &v) { 
	
	ifstream fin;
	stringstream ss;
	string input; 
	fin.open("city_list.txt");

	if (!fin.is_open()) {
		cerr << "city_list.txt could not be opened.\n";
		exit(1);
	}

	while (getline(fin, input)) {
		if (input.compare(0, 1, "\0") == 0 || input.compare(0, 1, "#") == 0) {
			//do nothing with the line
		}
		else {
			city incity;
			ss.clear();
			ss.str(input);
			ss >> incity;
			v.push_back(incity);
		}
				
	}

}

void create_citygraph(vector<city> &cities, travelcost &tcost, set< pair<int, int> > &edges) { 

	for (int i = 0; i < (int)cities.size(); i++) {
		float reg_to_gate;
		float reg_to_gate_index = -1;
		map< int, vector<int> > gate_to_gate;
		for (int j = 0; j < (int)cities.size(); j++) {
			
			if (i == j) continue;

			else if(cities[i].get_zone() == cities[j].get_zone()
				&& cities[i].get_type() == "REGIONAL"
				&& cities[j].get_type() == "REGIONAL") {
				//i and j are adjacent to each other
				edges.insert(make_pair(i, j));
				edges.insert(make_pair(j, i));
			}
			else if (cities[i].get_zone() == cities[j].get_zone()
					&& cities[i].get_type() == "REGIONAL"
					&& cities[j].get_type() == "GATEWAY") {
				//closest gateway is adjacent
				if (reg_to_gate_index < 0 || reg_to_gate > tcost(0, i, j)) {
					reg_to_gate = tcost(0, i, j);
					reg_to_gate_index = j;
				}
			}
			else if (cities[i].get_zone() == cities[j].get_zone()
					&& cities[i].get_type() == "GATEWAY"
					&& cities[j].get_type() == "GATEWAY") {
				//i and j are adjacent
				edges.insert(make_pair(i, j));
				edges.insert(make_pair(j, i));
			}
			else if (cities[i].get_zone() != cities[j].get_zone()
					&& cities[i].get_type() == "GATEWAY"
					&& cities[j].get_type() == "GATEWAY"
					&& tcost(0, i, j) < 6000.0) {
				//closest gateway in each zone is adjacent
				gate_to_gate[cities[j].get_zone()].push_back(j);
			}

		}

		//adds closest regional and gateway if there is such a thing
		if (reg_to_gate_index > 0) {
			edges.insert(make_pair(i, reg_to_gate_index));
			edges.insert(make_pair(reg_to_gate_index, i));
		}

		//adds closest gateways and gateway if there is one under 6000 miles away
		for (map< int, vector<int> >::iterator it = gate_to_gate.begin();
				it != gate_to_gate.end(); it++) {
			float gate_to_gate_dist;
			int   gate_to_gate_index = -1;

			for (int k = 0; k < (int)it->second.size(); k++) {
				if (gate_to_gate_index < 0 || gate_to_gate_dist > tcost(0, i, it->second[k])) {
					gate_to_gate_dist  = tcost(0, i, it->second[k]);
					gate_to_gate_index = it->second[k];
				}
			}

			if (gate_to_gate_index >= 0) {
				edges.insert(make_pair(i, gate_to_gate_index));
				edges.insert(make_pair(gate_to_gate_index, i));
			}
		}
	}
}

void write_citygraph(vector<city> &cities, travelcost& tcost, set< pair<int, int> > &edges) {

	ofstream outfile;
	outfile.open("city_graph_yours.txt");
	
	outfile << "CITY GRAPH: \n" << endl;
	for (int i = 0; i < (int)cities.size(); i++) {
		outfile << setw(3) << setfill(' ') << right << i << " " << cities[i].get_name() << '\n';	
		for (set< pair<int, int> >::iterator it = edges.begin(); it != edges.end(); it++) {
			if (it->first == i) {
				outfile << setw(6) << it->second << " " << setw(18) 
					<< left << cities[it->second].get_name() << setw(6) << right
					<< tcost(0, i, it->second) << " miles" << " " << setw(4) 
					<< setprecision(1) << fixed 
					<< tcost(1, i, it->second) << " hours" << endl;
			}
		}
	}

}

void dijkstra_route(int mode, int source, int sink, vector<city> &city_list, set< pair<int, int> > edges, travelcost tcost) {

	vector<bool> visited;
	visited.assign(city_list.size(), false);

	vector<float> vdist;
	vdist.assign(city_list.size(), numeric_limits<float>::max());
	vdist[source] = 0;

	vector<int> vlink;
	vlink.assign(city_list.size(), -1);
	vlink[source] = source;

	while (1) {
		int next_i = -1;
		float mindist = numeric_limits<float>::max();

		for (int i = 0; i < (int)visited.size(); i++) {
			if (visited[i] == false && mindist > vdist[i]) {
				next_i = i;
				mindist = vdist[i];
			}
		}

		int i = next_i;
		if (i == -1)
			return;

		visited[i] = true;

		if (i == sink)
			break;

		//not very effiecient but here we are...
		if (mode == 0) {
		for (set< pair<int, int> >::iterator it = edges.begin(); it != edges.end(); it++) {
			if (it->first == i) {
				int z = it->second;
				float tmp = vdist[i] + tcost(0, i, z);
				if (tmp < vdist[it->second]) {				
					vdist[it->second] = tmp;
					vlink[it->second] = i;
				}
			}
		}
		}
		else if (mode == 1) {
		for (set< pair<int, int> >::iterator it = edges.begin(); it != edges.end(); it++) {
			if (it->first == i) {
				int z = it->second;
				float tmp = vdist[i] + tcost(1, i, z);
				if (tmp < vdist[it->second]) {				
					vdist[it->second] = tmp;
					vlink[it->second] = i;
				}
			}
		}
		}


	}

	stack<int> S;
	for (int i = sink; i != source; i = vlink[i]) {
		S.push(i);
	}
	
	S.push(source);
	float int_dist = 0.0;
	float tot_hour = 0.0;
	int prev = 0;
	if (mode == 0) {
	while (!S.empty()) {
		int i = S.top();
		S.pop();
		if (i == source) {
			int_dist = 0.0;
			cout << setw(7) << setprecision(1) << fixed << int_dist << " miles"
				<< setw(6) << int_dist << " hours" << " : " << setw(2) << right << i << " "
				<< setw(18) << left <<  city_list[i].get_name() << endl;
		}
		else {
			int_dist = vdist[i] - vdist[prev];
			tot_hour += tcost(1, i, prev);

		cout << setw(7) << right << vdist[i]  << " miles " 
			<< setw(5) << tot_hour << " hours"
			<< " :" << setw(3) << right << i << " "
			<< setw(18) << left << city_list[i].get_name() << " "
			<< setw(6) << setprecision(1) << fixed << right << int_dist << " miles " 
			<< setw(5) << right << tcost(1, i, prev) << " hours" << '\n';
		}
		prev = i;
	}
	}
	if (mode == 1) {
	while (!S.empty()) {
		int i = S.top();
		S.pop();
		if (i == source) {
			int_dist = 0.0;
			cout << setw(7) << setprecision(1) << fixed << int_dist << " miles"
			<< setw(6) << int_dist << " hours" << " : " << setw(2) << right << i << " "
			<< setw(18) << left <<  city_list[i].get_name() << endl;
		}
		else {
			int_dist = vdist[i] - vdist[prev];
			tot_hour += tcost(0, i, prev);

		cout << setw(7) << right << tot_hour << " miles " 
			<< setw(5) << vdist[i]<< " hours"
			<< " :" << setw(3) << right << i << " "
			<< setw(18) << left << city_list[i].get_name() << " "
			<< setw(6) << setprecision(1) << fixed << right << tcost(0, i, prev) << " miles " 
			<< setw(5) << right << int_dist << " hours" << '\n';
		}
		prev = i;
	}
	}


}

int main(int argc, char *argv[])
{
  //option decoding

  if (argc != 2) {
	  cerr << "usage: " << argv[0] << " -distance|time\n";
	  return 1;
  }
  string option = argv[1];
  if ((option != "-time" && option != "-distance")) {
	cerr << "usage: " << argv[0] << " -distance|time\n";
	return 1;
  }

  int mode;

  if (option == "-time")
	  mode = 1;
  else if (option == "-distance")
	  mode = 0;
  //object declarations

  vector<city> city_list;
  read_cityinfo(city_list);

  //set up travelcosts
  travelcost tcost(city_list);
  tcost.write_traveldistance(city_list);
  tcost.write_traveltime(city_list);
  
  set< pair<int, int> > connections;
  create_citygraph(city_list, tcost, connections);
  write_citygraph(city_list, tcost, connections);
  //create_citygraph()

  cout << "Enter> ";
  string source, dest;

  map<string, int> names_ids;
  for (int i = 0; i < (int)city_list.size(); i++)
	  names_ids.insert(make_pair(city_list[i].get_name(), i));

  while (cin >> source >> dest) {
	//ask for form, to cities
	int source_id, dest_id;
	if (names_ids.lower_bound(source) == names_ids.end()) {
		cerr << source << " unkown city\n";
		continue;
	}
	else { source_id = names_ids.lower_bound(source)->second; }
	if (names_ids.lower_bound(source) == names_ids.end()) {
		cerr << source << " unknown city\n";
		continue;
	}
	else { dest_id = names_ids.lower_bound(dest)->second; }

	dijkstra_route(mode, source_id, dest_id, city_list, connections, tcost); 
	cout << endl;
	cout << "Enter> ";
  }
}
