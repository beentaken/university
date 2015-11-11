#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

typedef struct Node;

struct Jacket {
	int num;
	vector<int> times;
	Jacket() {};
	Jacket(int n) {
		num = n;
	};
};

struct Node {
	int value;
	vector<int> sequence;

	bool promising;

	vector<Node> children;
};

int calc_cost(vector<Jacket>,vector<int>,int);
int calc_cost_single(Jacket);
int get_lower(vector<Jacket>,vector<int>);

// true if item exists in sequence
bool sequence_contains(vector<int> x, int y) {
	for (int i = 0; i < x.size(); i++) {
		if (x[i] == y)
			return true;
	}
	return false;
}

// get the children for a node in the tree
// this is whatever jacket doesn't appear in the current sequence
vector<Node> get_children(Node node, vector<Jacket> jackets) {
	vector<Node> children;

	for (vector<Jacket>::iterator it = jackets.begin(); 
			it != jackets.end();
			++it) {

		bool ins = !sequence_contains(node.sequence, it->num);

		if (ins) {
			Node newNode;
			newNode.promising = true;
			newNode.sequence.insert(newNode.sequence.begin(), node.sequence.begin(), node.sequence.end());
			newNode.sequence.push_back(it->num);
			newNode.value = get_lower(jackets, newNode.sequence);
			children.push_back(newNode);
		}
	}

	return children;
}

// print a vector of ints seperated by ',''
void printVector(vector<int> i) {
	if (i.size() <= 0) return;
	for (vector<int>::iterator it = i.begin();
		it!=i.end();++it) {
		cout << *it << ", ";
	}
}

// print the node and its children
// testing purposes only
void printNode(Node root) {
	for (int i = 0; i < root.sequence.size(); i++) {
		cout << "\t";
	}
	cout << "Node: ";
	printVector(root.sequence);
	cout << "Value: " << root.value << " | ";
	cout << "Children: (" << root.children.size() << ") | ";
	cout << "Promising " << root.promising << endl;
	if (root.children.size() > 0) {
		for (vector<Node>::iterator child = root.children.begin();
				child != root.children.end();
				++child) {
			printNode(*child);
		}
	}
}

// determine if node exists in tree that could still be promising
bool still_promising(Node node, int upper) {

	if (node.promising)
		return true;
	else if (node.children.size() > 0) {
		for (vector<Node>::iterator child = node.children.begin();
			child != node.children.end();
			++child) {
			if (child->value > upper)
				child->promising = false;
			if (still_promising(*child, upper)) {
				return true;
			}
		}
	}

	return false;
}

void branch(Node &node, vector<Jacket> jackets, int &upper, int &nodes, vector<int> &sequence) {
	// get children
	node.promising = false;
	node.children = get_children(node, jackets);
	nodes += node.children.size();

	Node *best = NULL;
	if (node.children.size() > 0) { // find best from children
		for (vector<Node>::iterator child = node.children.begin();
				child != node.children.end();
				++child) {
			if (child->value > upper) {
				child->promising = false;
			} 

			if (child->promising && (best == NULL || child->value <= best->value)) {
				best = &(*child);
			}
		}
	} else { // end of branch calculate actual sequence value
		int cost = calc_cost(jackets, node.sequence, 3);
		if (cost < upper) {
			upper = cost;
			sequence = node.sequence;
		}
	}

	if (best != NULL) { // if not a dead end, branch again
		branch(*best, jackets, upper, nodes, sequence);
	}
}

// find the next best node from promising
Node* next_best(Node &node) {

	Node *best = &node;
	if (node.children.size() <= 0) {

		return best;

	} else {

		for (vector<Node>::iterator child = node.children.begin();
				child != node.children.end();
				++child) {

			Node *cb = next_best(*child);
			if (best->value < 0 || !best->promising || best->value > cb->value)
				best = cb;

		}

	}

	if (best != NULL && (best->value < node.value || (node.value < 0 || !node.promising)))
		return best;
	return &node;
}

// perform branch and bound algorithm
void do_branch_and_bound(Node root, vector<Jacket> jackets, int upper, int lower) {

	int nodes = 0;
	vector<int> sequence;

	while (still_promising(root, upper)) {
		Node *next = next_best(root);
		branch(*next, jackets, upper, nodes, sequence);
	}

	cout << "Optimum sequence: ";
	printVector(sequence);
	cout << endl << "Optimum time: " << upper << endl;
	cout << "Total nodes created: " << nodes << endl;
}

// main function
int main (int argc, char** argv) {
	int num_jackets = 0;
	vector<Jacket> jackets;

	// read in jacket times
	ifstream fin(argv[1]);
	fin >> num_jackets;

	// 
	for (int i = 0; i < num_jackets; i++) {
		jackets.push_back(Jacket(i+1));
	}

	for (int i = 0; i < 3; i++) {
		int pt = 0;
		for (int j = 0; j < num_jackets; j++) {
			fin >> pt;
			jackets[j].times.push_back(pt);
		}
	}

	// branch and bound to find optimum sequence
	// calc upperbound, 1,2,...,n
	vector<int> default_upper;
	for (int i = 1; i <= num_jackets; i++) 
		default_upper.push_back(i);
	int u_bound = calc_cost(jackets, default_upper, 3);

	/*cout << "Upperbound (";
	for (int i = 1; i <= num_jackets; i++) 
		cout << i;
	cout << "):\t" << u_bound << endl;*/

	// calc lowerbound, highest individual jacket time
	int l_bound = -1;
	l_bound = get_lower(jackets, vector<int>());
	
	/*cout << "Lowerbound:\t\t" << l_bound << endl;
	cout << l_bound << " <= C <= " << u_bound << endl;*/

	// perform branch and bound
	// vector<int> best = start_branch_and_bound(jackets, u_bound, l_bound);
	Node root;
	root.value = -1;
	do_branch_and_bound(root, jackets, u_bound, l_bound);

	return 0;
}

// calculate the true cost of a sequence
// option level exists to limit the calculation to a certain number of machines
int calc_cost(vector<Jacket> jackets, vector<int> sequence, int level) {

	int c_timing = 0;
	vector<vector<int> > timings;

	if (sequence.size() <= 0)
		return 0;
	
	for (vector<int>::iterator it = sequence.begin(); it != sequence.end(); ++it) {

		vector<int> tms;
		Jacket j;
		for (vector<Jacket>::iterator jt = jackets.begin(); jt != jackets.end(); ++jt) {
			if (jt->num == *it)
				j = *jt;
		}

		if (c_timing == 0) {
			tms.push_back(j.times[0]);
			if (level > 1) tms.push_back(j.times[1] + tms[0]);
			if (level > 2) tms.push_back(j.times[2] + tms[1]);
		}
		else {
			tms.push_back(j.times[0] + timings[c_timing-1][0]);

			int bji = j.times[1] + max(timings[c_timing-1][1], tms[0]);
			if (level > 1) tms.push_back(bji);

			int cji = j.times[2] + max(timings[c_timing-1][2], tms[1]);
			if (level > 2) tms.push_back(cji);
		}

		c_timing++;
		timings.push_back(tms);
	}

	return timings.back().back();
}

// calculate the total cost for a single jacket with no waiting time
int calc_cost_single(Jacket jacket) {
	int total = 0;
	for (vector<int>::iterator it = jacket.times.begin();
			it != jacket.times.end();
			++it) {
		total += *it;
	}
	return total;
}

// get the maximum of two integers
int max(int a, int b) {
	if (a > b)
		return a;
	else
		return b;
}

// calculate the lower bound for branchandbound
int get_lower(vector<Jacket> jackets, vector<int> ignore) {
	int l_bound = -1;

	int a=0, b=0, c=0;
	Jacket *final = NULL;

	// calculate the cost of Jr for machine a
	a = calc_cost(jackets, ignore, 1);
	for (vector<Jacket>::iterator jt = jackets.begin(); 
			jt != jackets.end();
			++jt) {
		// get final 
		if (final == NULL || calc_cost_single(*final) > calc_cost_single(*jt)) 
			final = &(*jt);
	}
	a += calc_cost_single(*final);

	// calculate the cost of Jr for machine b
	final = NULL;
	b = calc_cost(jackets, ignore, 2);
	for (vector<Jacket>::iterator jt = jackets.begin();
			jt != jackets.end();
			++jt) {
		// get final 
		if (final == NULL || 
			(calc_cost_single(*final)-final->times[0] > calc_cost_single(*jt)-jt->times[0]))
			final = &(*jt);
	}
	b += calc_cost_single(*final)-final->times[0];

	// calculate the cost of Jr for machine c
	final = NULL;
	c = calc_cost(jackets, ignore, 3);
	for (vector<Jacket>::iterator jt = jackets.begin();
			jt != jackets.end();
			++jt) {
		// get final 
		if (final == NULL || 
			(calc_cost_single(*final)-final->times[1]-final->times[0] > calc_cost_single(*jt)-jt->times[1]-jt->times[0]))
			final = &(*jt);
	}
	c += calc_cost_single(*final)-final->times[1]-final->times[0];

	return max(max(a, b), c);
}