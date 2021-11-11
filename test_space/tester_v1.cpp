#include <iostream>
#include "Search_tree.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <cmath>


int check_tree_balance(Search_tree<int>::Node &node){
    int l_height = node.left_tree != nullptr ? node.left_tree->height() : -1;
    int r_height = node.right_tree != nullptr ? node.right_tree->height() : -1;
    if (std::abs(l_height - r_height) >= 2){
        std::cout << "Failed" << std::endl;
        std::cout << "Nodes are not balanced" << std::endl;
    }
    if (node.left_tree != nullptr){
        if (node.node_value < node.left_tree->node_value){
            std::cout << "Failed" << std::endl;
            std::cout << "Left Subtree Node Value is Larger Than Parent Node" << std::endl;
        }
    } 
    if (node.right_tree != nullptr){
        if (node.node_value > node.right_tree->node_value){
            std::cout << "Failed" << std::endl;
            std::cout << "Right Subtree Node Value is Smaller Than Parent Node" << std::endl;
        }
    } 
    return node.node_value;
}

bool test_iterator(std::vector<int> &v, Search_tree<int> &tree){
    // Test the Iterator of the Search Tree. Tests both forwards and backwards.
    // Will check if the iterators are properly ordered, size of iterator is correct, and if the iterator matches elements inserted.
    // vector v contains a list of all the unique elements that are currently in the Search Tree
    // Search_tree tree is the tree in which the iterator are tested for.
    
    std::vector<int> tree_it;
    int prev = 0;
    std::sort(v.begin(), v.end()); //Sort the vector ascending
    for ( Search_tree<int>::Iterator itr = tree.begin(); itr != tree.end(); ++itr ) {
		tree_it.push_back(*itr);
        if (itr != tree.begin()){
            // Check if sorted, from front to back, value should strictly increasing
            if (*itr < prev) {
                 std::cout << "Failed" << std::endl;
                std::cout << "An error was found when trying to iterate from front to back" << std::endl;
                std::cout << *itr << " is > " << prev << "  - The iterator is not sorted correctly" << std::endl;
                return false;
            }
        }
        prev = *itr;
	}
    if (tree_it.size() != v.size()){
        std::cout << "Failed" << std::endl;
        std::cout << std::endl << "An error was found when trying to iterate from front to back" << std::endl;
        std::cout << "The iterator contains " << tree_it.size() << " elements but the tree should contain " << v.size() << " elements" << std::endl;
        return false;
    }else if (tree_it != v){
        std::cout << "Failed" << std::endl;
        std::cout << std::endl << "An error was found when trying to iterate from front to back" << std::endl;
        std::cout << "The elements in the iterator do not match the elements inserted into the tree" << std::endl;
        return false;
    }


	tree_it.clear(); 
    std::sort(v.begin(), v.end(), std::greater<int>()); //Sort the vector decending
    for ( Search_tree<int>::Iterator itr = tree.rbegin(); itr != tree.rend(); --itr ) {
		tree_it.push_back(*itr);
        if (itr != tree.rbegin()){
            // Check if sorted, from back to front, value should strictly decreasing
            if (*itr > prev) {
                std::cout << "Failed" << std::endl;
                std::cout << "An error was found when trying to iterate from back to front" << std::endl;
                std::cout << *itr << " is < " << prev << " - The iterator is not sorted correctly" << std::endl;
                return false;
            }
        }
        prev = *itr;
	}
    if (tree_it.size() != v.size()){
        std::cout << "Failed" << std::endl;
        std::cout << std::endl << "An error was found when trying to iterate from back to front" << std::endl;
        std::cout << "The iterator contains " << tree_it.size() << " elements but the tree should contain " << v.size() << " elements" << std::endl;
        return false;
    }else if (tree_it != v){
        std::cout << "Failed" << std::endl;
        std::cout << std::endl << "An error was found when trying to iterate from back to front" << std::endl;
        std::cout << "The elements in the iterator do not match the elements inserted into the tree" << std::endl;
        return false;
    }

    return true; // All tests passed!
}

int main() {
    // Initialize Everything
	Search_tree<int> tree;
	srand(time(NULL));
	int randomNum;

    // Test Basic Insertion 
    std::cout << "Basic Tree Insertion -> ";
	for ( int i = 1; i <= 30; ++i ) {
		if (!tree.insert( (83*i) % 30 )){
            std::cout << "Failed" << std::endl;
            std::cout << "Element " << (83*i) % 30 << " should have been inserted, but was not" << std::endl;
            return 1;
        }
	}
	for ( int i = 1; i <= 30; ++i ) {
		if(tree.insert( (97*i) % 30 )){
            std::cout << "Failed" << std::endl;
            std::cout << "Insertion of a duplicate element " << (97*i) % 30 << " succeeded when it should have failed" << std::endl;
            return 1;
        }
	}
    if (tree.size()!=30){
        std::cout << "Failed" << std::endl;
        std::cout << "The size of the tree should be 30 but instead is " << tree.size() << std::endl;
        return 1;
    }
    std::cout << "Passed!" << std::endl;

    // Test Basic Eraser 
    std::cout << "Basic Tree Deletion -> ";
	for ( int i = 1; i <= 30; ++i ) {
		if (!tree.erase( (83*i) % 30 )){
            std::cout << "Failed" << std::endl;
            std::cout << "Element " << (83*i) % 30 << " should have been erased, but was not" << std::endl;
            return 1;
        }
	}
	for ( int i = 1; i <= 30; ++i ) {
		if(tree.erase( (97*i) % 30 )){
            std::cout << "Erasing non-existant node " << (97*i) % 30 << " succeeded when it should have failed, ending test" << std::endl;
            return 1;
        }
	}
    if (tree.size()!=0){
        std::cout << "Failed" << std::endl;
        std::cout << "The size of the tree should be 0 but instead is " << tree.size() << std::endl;
        return 1;
    }
    std::cout << "Passed!" << std::endl; 
    

    // Test the Iterator Insertion from Empty
    std::cout << "Iterator Test (Insertion from Empty) -> ";
    int insertions = rand()%5000+5000;
    std::vector<int> v;
    for (int i = 0; i<insertions; ++i){
        randomNum = rand()%50000+1;
		if(tree.insert(randomNum)){
            v.push_back(randomNum);
        }
    }
    // Make sure Tree is correct size.
    if (tree.size()!=v.size()){
        std::cout << "The size of the tree should be " << v.size() << " but instead is " << tree.size() << std::endl;
        std::cout << "Ending the Test" << std::endl;
        return 1;
    }
    assert(test_iterator(v, tree));
    std::cout << "Passed!" << std::endl;

    // Test Iterator Deletion after Insertion
    std::cout << "Test Iterator (Deletion after Insertion) -> ";
    int deletions = rand()%5000+5000;
    for (int i = 0; i<deletions; ++i){
        randomNum = rand()%50000+1;
		if(tree.erase(randomNum)){
            v.erase(std::remove(v.begin(), v.end(), randomNum), v.end());
        }
    }
    
    // Make sure Tree is correct size.
    if (tree.size()!=v.size()){
        std::cout << "The size of the tree should be " << v.size() << " but instead is " << tree.size() << std::endl;
        std::cout << "Ending the Test" << std::endl;
        return 1;
    }
    assert(test_iterator(v, tree));
    std::cout << "Passed!" << std::endl;

    // Test Iterator Insertion After Deletion
    std::cout << "Test Iterator (Insertion After Deletion) -> ";
    insertions = rand()%5000+5000;
    for (int i = 0; i<insertions; ++i){
        randomNum = rand()%50000+1;
		if(tree.insert(randomNum)){
            v.push_back(randomNum);
        }
    }
    // Make sure that tree is correct size
    if (tree.size()!=v.size()){
        std::cout << "The size of the tree should be " << v.size() << " but instead is " << tree.size() << std::endl;
        std::cout << "Ending the Test" << std::endl;
        return 1;
    }
    assert(test_iterator(v, tree));
    std::cout << "Passed!" << std::endl;

    // Test if AVL Tree is properly rebalancing
    // Please note that this requires the tree nodes to be public.
    
    // Test LL Inbalance 
    std::cout << "Test AVL Tree LL Rebalancing -> ";
    tree.clear();
    tree.insert(30);
    tree.insert(20);
    tree.insert(10);
    check_tree_balance(*(tree.root_node));

    tree.insert(8);
    tree.insert(15);
    tree.insert(7);
    check_tree_balance(*(tree.root_node));
    std::cout << "Passed!" << std::endl;

    // Test RR Inbalance 
    std::cout << "Test AVL Tree RR Rebalancing -> ";
    tree.clear();
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    check_tree_balance(*(tree.root_node));

    tree.insert(25);
    tree.insert(35);
    tree.insert(33);
    check_tree_balance(*(tree.root_node));
    std::cout << "Passed!" << std::endl;

    // Test LR Inbalance 
    std::cout << "Test AVL Tree LR Rebalancing -> ";
    tree.clear();
    tree.insert(20);
    tree.insert(4);
    tree.insert(26);
    tree.insert(3);
    tree.insert(9);
    tree.insert(21);
    tree.insert(30);
    tree.insert(2);
    tree.insert(7);
    tree.insert(11);
    tree.insert(15);
    check_tree_balance(*(tree.root_node));

    tree.clear();
    tree.insert(20);
    tree.insert(4);
    tree.insert(26);
    tree.insert(3);
    tree.insert(9);
    tree.insert(21);
    tree.insert(30);
    tree.insert(2);
    tree.insert(7);
    tree.insert(11);
    tree.insert(8);
    check_tree_balance(*(tree.root_node));

    std::cout << "Passed!" << std::endl;

    // Test RL Inbalance 
    std::cout << "Test AVL Tree RL Rebalancing -> ";
    tree.clear();
    tree.insert(20);
    tree.insert(4);
    tree.insert(26);
    tree.insert(3);
    tree.insert(9);
    tree.insert(23);
    tree.insert(30);
    tree.insert(22);
    tree.insert(25);
    tree.insert(35);
    tree.insert(21);
    check_tree_balance(*(tree.root_node));

    tree.clear();
    tree.insert(20);
    tree.insert(4);
    tree.insert(26);
    tree.insert(3);
    tree.insert(9);
    tree.insert(23);
    tree.insert(30);
    tree.insert(22);
    tree.insert(25);
    tree.insert(35);
    tree.insert(24);
    check_tree_balance(*(tree.root_node));
    std::cout << "Passed!" << std::endl;

    std::cout << "Test AVL Tree Deletion Rebalancing 1 -> ";
    tree.clear();
    tree.insert(2);
    tree.insert(1);
    tree.insert(4);
    tree.insert(3);
    tree.insert(5);
    tree.erase(1);
    check_tree_balance(*(tree.root_node));
    std::cout << "Passed!" << std::endl;

    std::cout << "Test AVL Tree Deletion Rebalancing 2 -> ";
    tree.clear();
    tree.insert(5);
    tree.insert(2);
    tree.insert(8);
    tree.insert(1);
    tree.insert(3);
    tree.insert(7);
    tree.insert(15);
    tree.insert(4);
    tree.insert(6);
    tree.insert(9);
    tree.insert(16);
    tree.insert(17);
    tree.erase(1);
    check_tree_balance(*(tree.root_node));
    std::cout << "Passed!" << std::endl;

	return 0;
}
