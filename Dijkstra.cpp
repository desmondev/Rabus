
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <queue>
#include <vector>
#include <stack>

using namespace std;


class Edge;


class Node {
public:
	string    name;
	list<Edge*> neighbors;
	bool      visited;
	double       distance;
	Node      *back_pointer;
	list<Node*> previous;

	Node(string const &n) : name(n), visited(false), distance(0){}
};

class Edge
{
public:
	double weight;
	Node * dest;

	Edge(double c, Node *d = nullptr) :weight(c), dest(d){}

	~Edge(){ if (dest)delete dest; }
};

class NodeMap{
public:
	map<string, Node*> nodemap;

	Node* find_in_nodemap(string const &name){
		Node* result = nodemap[name];
		if (result == nullptr)
			result = nodemap[name] = new Node(name);
		return result;
	}
	friend ostream& operator<<(ostream& o, NodeMap nm){
		map<string, Node*>::iterator im;
		for (im = nm.nodemap.begin(); im != nm.nodemap.end(); ++im){
			o << (*im).second->name << endl;
			list<Edge*> neighbors = (*im).second->neighbors;
			list<Edge*>::iterator e;
			for (e = neighbors.begin(); e != neighbors.end(); ++e){
				cout << "   -> " << (*e)->dest->name << " weight " << (*e)->weight << endl;
			}

		}
		return o;
	}
};

typedef std::vector<Node*> Path;
typedef std::vector<Path> PathSet;
typedef std::vector<Path>::const_iterator Paths;
PathSet path_set;

struct compare{

	bool operator()(Node * &a, Node * &b)const{
		// least to greatest
		return b->distance < a->distance;
	}
};

// for each solution, reset node information
void reset_nodes(NodeMap &nm){
	map<string, Node*>::iterator im;
	for (im = nm.nodemap.begin(); im != nm.nodemap.end(); ++im){
		(*im).second->distance = 0;
		(*im).second->back_pointer = nullptr;
		(*im).second->visited = false;
	}
}
void reset_visited_nodes(NodeMap &nm){
	map<string, Node*>::iterator im;
	for (im = nm.nodemap.begin(); im != nm.nodemap.end(); ++im){
		(*im).second->visited = false;
	}
}

void dijkstra(string s, string t, NodeMap &nodes){

	// check and report or abort
	Node* source = nodes.nodemap[s];
	if (source == nullptr){ cout << s << " not in map " << endl; return; }
	else cout << s << " in map " << endl;
	Node* target = nodes.nodemap[t];
	if (target == nullptr){ cout << t << " not in map " << endl; return; }
	else cout << t << " in map " << endl;

	reset_nodes(nodes);

	// put the source into pq and loop until empty
	priority_queue<Node *, deque<Node*>, compare>pq;
	pq.push(source);
	while (!pq.empty()){

		// process least distance node.
		Node* current = pq.top();
		pq.pop();
		current->visited = true;

		// process neighbors
		list<Edge*>::iterator neighbour;
		for (neighbour = current->neighbors.begin(); neighbour != current->neighbors.end(); ++neighbour){
			Node *ne = (*neighbour)->dest;
			if (!ne->visited){
				ne->distance += (*neighbour)->weight + current->distance;
				ne->visited = true;
				ne->back_pointer = current;

				ne->previous.push_back(current);

				cout << " pushing " << ne->name << " distance " << ne->distance << endl;
				pq.push(ne);
			}
			else{
				if (ne->distance >= (*neighbour)->weight + current->distance)
				{
					if (ne->distance == (*neighbour)->weight + current->distance)
					{
						ne->previous.push_back(current);
					}
					else
					{
						ne->previous.clear();
						ne->previous.push_back(current);
					}

					ne->distance = (*neighbour)->weight + current->distance;
					ne->back_pointer = current;
					cout << " pushing " << ne->name << " distance " << ne->distance << endl;
				}

			}
		}
	}
}

void get_graph(string const &filename, NodeMap &node_map)
{
	ifstream inf(filename.c_str());
	string from, to;
	double weight;
	while (inf.good())
	{
		inf >> from >> to >> weight;
		cout << from << " " << to << " " << weight << endl;
		if (inf.good())
		{
			Node *Target = node_map.find_in_nodemap(to);
			Node *Source = node_map.find_in_nodemap(from);
			Edge *connector = new Edge(weight, Target);
			Source->neighbors.push_back(connector);
		}
	}
}
Path path;
void dfs(NodeMap np, Node* n, Node* target)
{
	
	path.push_back(n);
	if (n == target)
	{
		path_set.push_back(Path(path));
		
		return;
	}
	list<Node*>::iterator neighbour;
	for (neighbour = n->previous.begin(); neighbour != n->previous.end(); ++neighbour){
			dfs(np, (*neighbour),target);
			(*neighbour)->visited = false;
			std::cout << (*neighbour)->name;
			path.pop_back();
	}
	std::cout << endl;
}

void dfs_iter(NodeMap np, Node* n)
{
	stack<Node*> stack;
	stack.push(n);
	while (!stack.empty())
	{
		Node* u = stack.top();
		stack.pop();
		if (!u->visited)
		{
			u->visited = true;
			std::cout << u->name;
			list<Node*>::iterator neighbour;
			for (neighbour = u->previous.begin(); neighbour != u->previous.end(); ++neighbour){
				stack.push(*neighbour);
			}

		}
	}
}

void myfunction(Node* n) {  // function:
	std::cout << ' ' << n->name;
}


void main(){
	NodeMap nodes;
	get_graph("graph.txt", nodes);

	cout << "after read graph" << endl;
	cout << nodes;
	string s, t,e;
	do{
		cout << " Please enter a source" << endl;
		cin >> s;
		cout << " Please enter a target" << endl;
		cin >> t;
		dijkstra(s, t, nodes);
		cout << "Enter y to continue " << endl;
		cin >> e;
	} while (e == "y");

	// wersja z tylko jednym poprzednikiem
	vector<Node*> path;
	Node* source = nodes.find_in_nodemap(s);
	Node* target = nodes.find_in_nodemap(t);
	Node* prev = target->previous.front();
	path.push_back(target);
	while (!prev->previous.empty()){
		path.push_back(prev);
		prev = prev->previous.front();

	}
	std::reverse(path.begin(), path.end());
	std::for_each(path.begin(), path.end(), myfunction);

	reset_visited_nodes(nodes);
	dfs(nodes, target, source);
// wzor uzycia lambdy
//	std::for_each(nums.begin(), nums.end(), [](int &n){ n++; });
	std::for_each(path_set.begin(), path_set.end(), [](Path &i){std::reverse(i.begin(), i.end()); });
	std::for_each(path_set.begin(), path_set.end(), [](Path &i){std::for_each(i.begin(), i.end(), [](Node* n){ std::cout << n->name << " "; }); std::cout << endl; });


	int i;
	cin >> i;
}