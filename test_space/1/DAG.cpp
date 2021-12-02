#include "DAG.h"
#include <queue>

void DAG::add_file(std::string filename, std::vector<std::string> &includes) {
    
    Node *current_node = nullptr;

    for(Node *n:nodes) {
        if(n->filename == filename) {
            current_node = n;
            current_node->in_degree = includes.size();
        }
    }

    if(!current_node){
        current_node = new Node(filename, includes.size());
        nodes.push_back(current_node);
    }
        
    // add the neighbours
    for(std::string include:includes) {
        bool is_found = false;
        for(Node *n:nodes) {
            if(n->filename == include) {
                n->neighbours.push_back(current_node);  // Assumption: no two files with the same name
                is_found = true;
                break;
            }
        }

        // Add a new node if the included file is not found
        if(!is_found) {
            Node *new_include = new Node(include);
            new_include->neighbours.push_back(current_node);
            nodes.push_back(new_include);
        }
    }
}


DAG::DAG():
nodes(),
independent_nodes()
{

}


DAG::~DAG() {
    for(Node *n:nodes)
        delete n;
}


DAG::Iterator DAG::begin() {
    for(Node *n:nodes) {
        if(n->in_degree == 0)
            independent_nodes.push(n);
    }

    if(independent_nodes.empty()) {
        return Iterator(this, nullptr);
    }

    Node *current_node = independent_nodes.front();
    independent_nodes.pop();

    current_node->in_degree = -1;
    for(Node *neighbour:current_node->neighbours) {
        neighbour->in_degree -= 1;
        if(neighbour->in_degree == 0)
            independent_nodes.push(neighbour);
    }

    return Iterator(this, current_node);
}


DAG::Iterator DAG::end() {
    return Iterator(this, nullptr);
}


//************************* Node Class Methods ****************************//
DAG::Node::Node(std::string fn, int id):
filename(fn),
in_degree(id)
{

}


//************************* Iterator Class Methods **************************//
DAG::Iterator::Iterator(DAG *graph, Node *starting_node):
containing_graph(graph),
current_node(starting_node)
{

}


DAG::Iterator &DAG::Iterator::operator++() {
    if(containing_graph->independent_nodes.empty()){
        current_node = nullptr;
        return *this;
    }
    
    current_node = containing_graph->independent_nodes.front();
    containing_graph->independent_nodes.pop();

    current_node->in_degree = -1;
    for(Node *neighbour:current_node->neighbours) {
        neighbour->in_degree -= 1;
        if(neighbour->in_degree == 0)
            containing_graph->independent_nodes.push(neighbour);
    }

    return *this;
}


bool DAG::Iterator::operator!=(Iterator const &rhs) const {
    return current_node != rhs.current_node;
}


std::string DAG::Iterator::operator*() const {
    return current_node->filename;
}