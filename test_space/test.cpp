#include <iostream>
#include "Search_tree.h"

int main() {
	Search_tree<int> tree;

	for ( int i = 1; i <= 30; ++i ) {
		int x = (83*i)%30;
		std::cout << x << " ";
		tree.insert( x );

	}

	std::cout << std::endl;

	for ( int i = 1; i <= 30; ++i ) {
		std::cout << tree.insert( (97*i) % 30 );
	}

	std::cout << std::endl;

	std::cout << "Front:   " << tree.front()   << std::endl;
	std::cout << "Back:    " << tree.back()    << std::endl;
	std::cout << "Height:  " << tree.height()  << " <- this will change for an AVL tree" << std::endl;

	std::cout << "--> ";

	for ( Search_tree<int>::Iterator itr = tree.begin(); itr != tree.end(); ++itr ) {
		std::cout << *itr << " ";
	}

	std::cout << std::endl;

	std::cout << "--> ";

	for ( Search_tree<int>::Iterator itr = tree.rbegin(); itr != tree.rend(); --itr ) {
		std::cout << *itr << " ";
	}

	std::cout << std::endl;

	tree.clear();

	std::cout << "--> ";
	

	for ( Search_tree<int>::Iterator itr = tree.begin(); itr != tree.end(); ++itr ) {
		std::cout << *itr << " ";
	}

	std::cout << std::endl;

	return 0;
}
