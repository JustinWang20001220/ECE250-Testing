#ifndef DAGH
#define DAGH

#include <string>
#include <vector>
#include <list>
#include <queue>

class DAG {
public:
    class Node;
    class Iterator;

    DAG();
    ~DAG();
    
    void add_file(std::string filename, std::vector<std::string> &includes);

    Iterator begin(); // return the first node in the topological sorting of the DAG
    Iterator end();

    std::vector<Node *> nodes;
    std::queue<Node *> independent_nodes;
};


class DAG::Node {
public:
    Node(std::string fn = "", int id = 0);
    
    std::string filename;
    int in_degree;
    std::list<Node *> neighbours;
};


class DAG::Iterator {
public:
    Iterator(DAG *graph, Node *starting_node);
    Iterator &operator++();

    bool operator!=(Iterator const &rhs) const;
    std::string operator*() const;

    DAG *containing_graph;
    Node *current_node;
};

#endif