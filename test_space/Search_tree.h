/*****************************************
 * UW User ID:  uwuserid
 * Submitted for ECE 250
 * Semester of Submission:  (Winter|Spring|Fall) 20NN
 *
 * By  submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#ifndef RPOVDGBQN9TIEO3P
#define RPOVDGBQN9TIEO3P

#include "Exception.h"
#include "ece250.h"
#include <cassert>
#include <cmath>

template <typename Type>
class Search_tree {
	public:
		class Iterator;

	public:
		class Node {
			public:
				Type node_value;
				int tree_height;

				// The left and right sub-trees
				Node *left_tree;
				Node *right_tree;

				// Hint as to how you can create your iterator
				// Point to the previous and next nodes in linear order
				Node *previous_node;
				Node *next_node;

				// Member functions
				Node( Type const & = Type() );

				void update_height();

				int height() const;
				bool is_leaf() const;
				Node *front();
				Node *back();
				Node *find( Type const &obj );

				void clear();
				bool insert( Type const &obj, Node *&to_this );
				bool erase( Type const &obj, Node *&to_this );

			public:
				// check if the sub-tree rooted at the current node is balanced
				bool is_balanced() const;
				// left-left or right-right balancing
				void balance(bool flag, Node *b, Node *BLR, Node *&to_this);
				// left-right or right-left balancing					
				void balance(bool flag, Node *b, Node *d, Node* BL, Node* BR, Node *&to_this);
				// left balancing
				void left_balance(Node *&to_this);
				// right balancing
				void right_balance(Node *&to_this);
		};


		Node *root_node;
		int tree_size;

		// Hint as to how to start your linked list of the nodes in order 
		Node *front_sentinel;
		Node *back_sentinel;

	public:
		class Iterator {
			public:
				Search_tree *containing_tree;
				Node *current_node;
				bool is_end;

				// The constructor is public so that only the search tree can create an iterator
				Iterator( Search_tree *tree, Node *starting_node );

			public:
				// DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
				Type operator*() const;
				Iterator &operator++();
				Iterator &operator--();
				bool operator==( Iterator const &rhs ) const;
				bool operator!=( Iterator const &rhs ) const;

			// Make the search tree a friend so that it can call the constructor
			friend class Search_tree;
		};

		// DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
		Search_tree();
		~Search_tree();

		bool empty() const;
		int size() const;
		int height() const;

		Type front() const;
		Type back() const;

		Iterator begin();
		Iterator end();
		Iterator rbegin();
		Iterator rend();
		Iterator find( Type const & );

		void clear();
		bool insert( Type const & );
		bool erase( Type const & );

	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Search_tree<T> const & );
};

//////////////////////////////////////////////////////////////////////
//                Search Tree Public Member Functions               //
//////////////////////////////////////////////////////////////////////

// The initialization of the front and back sentinels is a hint
template <typename Type>
Search_tree<Type>::Search_tree():
root_node( nullptr ),
tree_size( 0 ),
front_sentinel( new Search_tree::Node( Type() ) ),
back_sentinel( new Search_tree::Node( Type() ) ) {
	front_sentinel->next_node = back_sentinel;
	back_sentinel->previous_node = front_sentinel;
}

template <typename Type>
Search_tree<Type>::~Search_tree() {
	clear();  // might as well use it...
	delete front_sentinel;
	delete back_sentinel;
}

template <typename Type>
bool Search_tree<Type>::empty() const {
	return ( root_node == nullptr );
}

template <typename Type>
int Search_tree<Type>::size() const {
	return tree_size;
}

template <typename Type>
int Search_tree<Type>::height() const {
	return root_node->height();
}

template <typename Type>
Type Search_tree<Type>::front() const {
	if ( empty() ) {
		throw underflow();
	}

	return root_node->front()->node_value;
}

template <typename Type>
Type Search_tree<Type>::back() const {
	if ( empty() ) {
		throw underflow();
	}

	return root_node->back()->node_value;
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::begin() {
	return empty() ? Iterator( this, back_sentinel ) : Iterator( this, root_node->front() );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::end() {
	return Iterator( this, back_sentinel );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::rbegin() {
	return empty() ? Iterator( this, front_sentinel ) : Iterator( this, root_node->back() );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::rend() {
	return Iterator( this, front_sentinel );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::find( Type const &obj ) {
	if ( empty() ) {
		return Iterator( this, back_sentinel );
	}

	typename Search_tree<Type>::Node *search_result = root_node->find( obj );

	if ( search_result == nullptr ) {
		return Iterator( this, back_sentinel );
	} else {
		return Iterator( this, search_result );
	}
}

template <typename Type>
void Search_tree<Type>::clear() {
	if ( !empty() ) {
		root_node->clear();
		root_node = nullptr;
		tree_size = 0;
	}

	// Reinitialize the sentinels
	front_sentinel->next_node = back_sentinel;
	back_sentinel->previous_node = front_sentinel;
}

template <typename Type>
bool Search_tree<Type>::insert( Type const &obj ) {
	if ( empty() ) {
		root_node = new Search_tree::Node( obj );
		tree_size = 1;
		front_sentinel->next_node = root_node;
		root_node->previous_node = front_sentinel;
		root_node->next_node = back_sentinel;
		back_sentinel->previous_node = root_node;
		return true;
	} else if ( root_node->insert( obj, root_node ) ) {
		++tree_size;
		return true;
	} else {
		return false;
	}
}

template <typename Type>
bool Search_tree<Type>::erase( Type const &obj ) {
	if ( !empty() && root_node->erase( obj, root_node ) ) {
		--tree_size;
		return true;
	} else {
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
//                   Node Public Member Functions                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Search_tree<Type>::Node::Node( Type const &obj ):
node_value( obj ),
left_tree( nullptr ),
right_tree( nullptr ),
next_node( nullptr ),
previous_node( nullptr ),
tree_height( 0 ) {
	// does nothing
}

template <typename Type>
void Search_tree<Type>::Node::update_height() {
	tree_height = std::max( left_tree->height(), right_tree->height() ) + 1;
}

template <typename Type>
int Search_tree<Type>::Node::height() const {
	return ( this == nullptr ) ? -1 : tree_height;
}

// Return true if the current node is a leaf node, false otherwise
template <typename Type>
bool Search_tree<Type>::Node::is_leaf() const {
	return ( (left_tree == nullptr) && (right_tree == nullptr) );
}

// Return a pointer to the front node
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::front() {
	return ( left_tree == nullptr ) ? this : left_tree->front();
}

// Return a pointer to the back node
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::back() {
	return ( right_tree == nullptr ) ? this : right_tree->back();
}

template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::find( Type const &obj ) {
	if ( obj == node_value ) {
		return this;
	} else if ( obj < node_value ) {
		return (left_tree == nullptr) ? nullptr : left_tree->find( obj );
	} else {
		return ( right_tree == nullptr ) ? nullptr : right_tree->find( obj );
	}
}

// Recursively clear the tree
template <typename Type>
void Search_tree<Type>::Node::clear() {
	if ( left_tree != nullptr ) {
		left_tree->clear();
	}

	if ( right_tree != nullptr ) {
		right_tree->clear();
	}

	delete this;
}

// Insert, rebalance, and rearrange the linked list
template <typename Type>
bool Search_tree<Type>::Node::insert( Type const &obj, Search_tree<Type>::Node *&to_this ) {
	if ( obj < node_value ) {
		if ( left_tree == nullptr ) {
			left_tree = new Search_tree<Type>::Node( obj );
			update_height();

			// maintain linkedlist
			Search_tree<Type>::Node *pnode = previous_node;
			pnode->next_node = left_tree;
			left_tree->previous_node = pnode;
			left_tree->next_node = this;
			previous_node = left_tree;

			return true;
		} else {
			if ( left_tree->insert( obj, left_tree ) ) {
				if (!is_balanced())
					left_balance(to_this);
				update_height();
				return true;
			} else {
				return false;
			}
		}
	} else if ( obj > node_value ) {
		if ( right_tree == nullptr ) {
			right_tree = new Search_tree<Type>::Node( obj );
			update_height();

			// maintain linkedlist
			Search_tree<Type>::Node *nnode = next_node;
			next_node = right_tree;
			right_tree->previous_node = this;
			right_tree->next_node = nnode;
			nnode->previous_node = right_tree;

			return true;
		} else {
			if ( right_tree->insert( obj, right_tree ) ) {
				if (!is_balanced())
					right_balance(to_this);
				update_height();
				return true;
			} else {
				return false;
			}
		}
	} else {
		return false;
	}
}

template <typename Type>
bool Search_tree<Type>::Node::erase( Type const &obj, Search_tree<Type>::Node *&to_this ) {
	if ( obj < node_value ) {
		if ( left_tree == nullptr ) {
			return false;
		} else {
			if ( left_tree->erase( obj, left_tree ) ) {
				if (!is_balanced())
					right_balance(to_this);
				update_height();
				return true;
			}
			return false;
		}
	} else if ( obj > node_value ) {
		if ( right_tree == nullptr ) {
			return false;
		} else {
			if ( right_tree->erase( obj, right_tree ) ) {
				if (!is_balanced())
					left_balance(to_this);
				update_height();
				return true;
			}
			return false;
		}
	} else {
		assert( obj == node_value );
		previous_node->next_node = next_node;	// maintain linkedlist
		if ( is_leaf() ) {
			to_this = nullptr;
			delete this;
		} else if ( left_tree == nullptr ) {
			to_this = right_tree;
			delete this;
		} else if ( right_tree == nullptr ) {
			to_this = left_tree;
			delete this;
		} else {
			node_value = right_tree->front()->node_value;
			previous_node->next_node = this;	// maintain linkedlist
			next_node = next_node->next_node;	// maintain linkedlist
			right_tree->erase( node_value, right_tree );
			if (!is_balanced())
				left_balance(to_this);
			update_height();
		}

		return true;
	}
}

template <typename Type>
bool Search_tree<Type>::Node::is_balanced() const {
	return std::abs(left_tree->height() - right_tree->height()) <= 1;
}

// left balancing
template <typename Type>
void Search_tree<Type>::Node::left_balance(Node *&to_this) {
	if (left_tree->left_tree->height() >= left_tree->right_tree->height()) {
		// left-left
		balance(true, left_tree, left_tree->right_tree, to_this);
	} else {
		// left-right
		balance(true, left_tree, left_tree->right_tree,
		left_tree->right_tree->left_tree,
		left_tree->right_tree->right_tree,
		to_this);
	}
}

// right balancing
template <typename Type>
void Search_tree<Type>::Node::right_balance(Node *&to_this) {
	if (right_tree->right_tree->height() >= right_tree->left_tree->height()) {
		// right-right
		balance(false, right_tree, right_tree->left_tree, to_this);
	} else {
		// right-left
		balance(false, right_tree, right_tree->left_tree,
				right_tree->left_tree->left_tree,
				right_tree->left_tree->right_tree,
				to_this);
	}
}

// left-left or right-right balances
template <typename Type>
void Search_tree<Type>::Node::balance(bool flag, Node *b, Node *BLR, Node *&to_this) {
	// flag: true for left-left and false for right-right
	if (flag) {
		to_this = b;
		b->right_tree = this;
		left_tree = BLR;
	} else {
		to_this = b;
		b->left_tree = this;
		right_tree = BLR;
	}
	update_height();
	b->update_height();
}

// left-right or right-left balances
template <typename Type>
void Search_tree<Type>::Node::balance(bool flag, Node *b, Node *d, Node* DL, Node* DR, Node *&to_this) {
	// flag: true for left-right and false for right-left
	if (flag) {
		to_this = d;
		d->left_tree = b;
		d->right_tree = this;
		b->right_tree = DL;
		left_tree = DR;
	} else {
		to_this = d;
		d->left_tree = this;
		d->right_tree = b;
		b->left_tree = DR;
		right_tree = DL;
	}
	update_height();
	b->update_height();
	d->update_height();
}

//////////////////////////////////////////////////////////////////////
//                   Iterator Private Constructor                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Search_tree<Type>::Iterator::Iterator( Search_tree<Type> *tree, typename Search_tree<Type>::Node *starting_node ):
containing_tree( tree ),
current_node( starting_node ) {
	// This is done for you...
	// Does nothing...
}

//////////////////////////////////////////////////////////////////////
//                 Iterator Public Member Functions                 //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Type Search_tree<Type>::Iterator::operator*() const {
	// This is done for you...
	return current_node->node_value;
}

template <typename Type>
typename Search_tree<Type>::Iterator &Search_tree<Type>::Iterator::operator++() {
	// Update the current node to the node containing the next higher value
	// If we are already at end do nothing

	// Your implementation here, do not change the return value
	if (current_node != containing_tree->back_sentinel)
		current_node = current_node->next_node;

	return *this;
}

template <typename Type>
typename Search_tree<Type>::Iterator &Search_tree<Type>::Iterator::operator--() {
	// Update the current node to the node containing the next smaller value
	// If we are already at either rend, do nothing

	// Your implementation here, do not change the return value
	if (current_node != containing_tree->front_sentinel)
		current_node = current_node->previous_node;

	return *this;
}

template <typename Type>
bool Search_tree<Type>::Iterator::operator==( typename Search_tree<Type>::Iterator const &rhs ) const {
	// This is done for you...
	return ( current_node == rhs.current_node );
}

template <typename Type>
bool Search_tree<Type>::Iterator::operator!=( typename Search_tree<Type>::Iterator const &rhs ) const {
	// This is done for you...
	return ( current_node != rhs.current_node );
}

//////////////////////////////////////////////////////////////////////
//                            Friends                               //
//////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<( std::ostream &out, Search_tree<T> const &list ) {
	out << "not yet implemented";

	return out;
}

#endif
