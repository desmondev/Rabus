/*
   Kenneth L Moore
   11.16.2003
   Dijkstra Algorithm via the STL
   Many thanks to iCarnegie(http://www3.carnegietech.org/) 
   for this brillant approach.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <queue>
#include <vector>

using namespace std;

class Edge; 

class Node {
public:
	string    name;
	list<Edge*> neighbors;
	bool      visited;
	double       cost;
	Node      *back_pointer;

	Node(string const &n): name(n),visited(false),cost(0){}
};

class Edge 
{
public:
	double weight;
	Node * dest;

	Edge(double c,Node *d = NULL):weight(c),dest(d){}

	~Edge(){if(dest)delete dest;}
};

class NodeMap{
public:
	map<string,Node*> nodemap;

	Node* find_in_nodemap(string const &name){
		Node* result = nodemap[name];
		if ( result == 0 )
			result = nodemap[name] = new Node(name);
		return result;
	}
	friend ostream& operator<<(ostream& o, NodeMap nm){
		map<string,Node*>::iterator im;
		for(im = nm.nodemap.begin(); im != nm.nodemap.end(); im++){
			//pair<string,Node*> p = *im;
			//o << p.second->name << endl;
			o << (*im).second->name << endl;
	        list<Edge*> neighbors = (*im).second->neighbors;
			list<Edge*>::iterator e;
			for(e = neighbors.begin(); e != neighbors.end(); e++){
				cout << "   -> " << (*e)->dest->name << " weight " << (*e)->weight <<endl;
			}

		}
		return o;
	}
};

// is this correct?
struct compare{
   
   bool operator()(Node * &a,Node * &b)const{
      // least to greatest
      return b->cost < a->cost;      
   }
};

// for each solution, reset node information
void reset_nodes(NodeMap &nm){
	map<string,Node*>::iterator im;
	for(im = nm.nodemap.begin(); im != nm.nodemap.end(); im++){
		(*im).second->cost = 0; 
		(*im).second->back_pointer = 0;
		(*im).second->visited = false;
	}
}

void dijkstra(string s,string t,NodeMap &nodes){

	// check and report or abort
	Node* source = nodes.nodemap[s];
	if(source==0){cout << s << " not in map " << endl; return;}
	else cout << s << " in map " << endl;
	Node* target = nodes.nodemap[t];
	if(target==0){cout << t << " not in map " << endl; return;}
	else cout << t << " in map " << endl;

	reset_nodes(nodes);

	// put the source into pq and loop until empty
	priority_queue<Node *, deque<Node*>, compare>pq;
	pq.push(source);
	while(!pq.empty()){

		// process least cost node.
		Node* curr = pq.top(); 
		pq.pop();
		curr->visited = true;

		// process neighbors
		list<Edge*>::iterator edge;
		for(edge = curr->neighbors.begin(); edge != curr->neighbors.end(); edge++){
			Node *ne = (*edge)->dest;
			if(!ne->visited){
				ne->cost += (*edge)->weight + curr->cost;
				ne->visited = true;
				ne->back_pointer = curr;
				cout << " pushing " << ne->name << " cost " << ne->cost << endl;;
				pq.push(ne);
			}
			else{
				// see if this can cost less
			}
		}
	}
}

void get_graph(string const &filename, NodeMap &node_map) 
{
	ifstream inf(filename.c_str());
	string from, to;
	double weight;
	while ( inf.good() ) 
	{
		inf >> from >> to >> weight;
		cout << from << " " << to << " " << weight << endl;
		if ( inf.good() ) 
		{
			Node *Target = node_map.find_in_nodemap(to);
			Node *Source = node_map.find_in_nodemap(from);
			Edge *connector = new Edge(weight,Target);
			Source->neighbors.push_back(connector);
		}
	}
}
void main(){
	NodeMap nodes;
	get_graph("graph.txt", nodes);

	cout << "after read graph" << endl;
	cout << nodes;
	string s,t;
	do{
		cout << " Please enter a source" << endl;
		cin >> s;
		cout << " Please enter a target" << endl;
		cin >> t;
		dijkstra(s,t,nodes);
		cout << "Enter y to continue " << endl;
		cin >> s;
	}while(s == "y");

	Node* target = nodes.find_in_nodemap(t);
	Node* previous = target->back_pointer;
	while(previous != NULL){
		cout << previous->name;
		previous = previous->back_pointer;

	}

}