#pragma once
#include<stack>

#include <iostream>
using namespace std;

template<typename T>
struct Node {
	T data;
	Node<T>* left;
	Node<T>* right;
	Node<T>* nextInOrder;

	Node() {
		left = nullptr;
		right = nullptr;
		nextInOrder = nullptr;
	}

	Node(T d, Node<T>* r = 0, Node<T>* l = 0, Node<T>* next = 0) {
		data = d;
		left = l;
		right = r;
		nextInOrder = next;
	}
};

//stores parent of current node and its direction relative to parent
template<typename T>
struct PushNode{
	enum ChildDirection { left = 0, right = 1 };

	Node<T> * nod;
	ChildDirection direction;
	PushNode(Node<T>* n, ChildDirection dir) : nod(n), direction(dir) {};	
};

template<class T>
class SortedStacklessBST {
public:
	SortedStacklessBST();
	bool insert(T d);
private:
	Node<T>* root;
	bool IsSuccessor;

	//updates successor of parents of new node and finds successor of new node
	void updateSuccessor(stack<PushNode<T>> traceTree, Node<T>* newNode);
	//searches for successor of passed node
	Node<T>* getChildSuccessor(Node<T>* n);
};

template<class T>
SortedStacklessBST<T>::SortedStacklessBST()
{
	root = nullptr;
	IsSuccessor = 1;
}

template<typename T>
bool SortedStacklessBST<T>::insert(T d)
{
	//list empty
	if (root == nullptr) {
		root = new Node<T>(d);
		return 1;
	}

	//traverse the tree
	Node<T>* trav = root;
	//store previous nodes while tracing tree
	stack<PushNode<T>> traceTree;

	//traverse to find the right location
	while (trav != nullptr) {
		if (d > trav->data) {
			traceTree.push(PushNode<T>(trav, PushNode<T>::ChildDirection::right));
			trav = trav->right;
		}
		else if (d < trav->data) {
			traceTree.push(PushNode<T>(trav, PushNode<T>::ChildDirection::left));
			trav = trav->left;
		}
		else
			return 0; //data is already in stack
	}

	//add new node
	trav = new Node<T>(d);
	PushNode<T> temp = traceTree.top();
	if (temp.direction == temp.left)
		temp.nod->left =trav;
	else
		temp.nod->right =trav;

	//update the successors of parent nodes
	//find successor of new node
	updateSuccessor(traceTree, trav);
}

//update the successors of parent nodes
//find successor of new node
template<typename T>
void SortedStacklessBST<T>::updateSuccessor(stack<PushNode<T>> traceTree, Node<T>* newNode)
{
	//Rules for successor / predecessor
	//	1. Successor
	//		a.Possibility 1
	//			i.Goto right child
	//			ii.Get min node of right tree
	//		b.Possibility 2
	//			i.Go to parent
	//			ii.Stop when find a rightwards connection
	//		c.Get max of p1 and p2
	//	2. Predecessor
	//		a.Possibility 1
	//			i.Goto left child
	//			ii.Get max of left tree
	//		b.Possibilty 2
	//			i.Goto parent
	//			ii.Stop when find left connection

	while (!traceTree.empty()) {
		PushNode<T> tempParent = traceTree.top();
		traceTree.pop();

		//find successor of newNode
		if (IsSuccessor == 1 && tempParent.direction == tempParent.left 
			&& newNode->nextInOrder == nullptr) {

			newNode->nextInOrder = tempParent.nod;
		}
		if (IsSuccessor == 0 && tempParent.direction == tempParent.right
			&& newNode->nextInOrder == nullptr) {

			newNode->nextInOrder = tempParent.nod;
		}
		
		//update successor of current parent
		if (IsSuccessor == 1 && tempParent.direction == tempParent.right
			|| IsSuccessor == 0 && tempParent.direction == tempParent.left) {
			
			Node<T>* tempSucc = getChildSuccessor(tempParent.nod);
			if (tempSucc!=nullptr && tempSucc->data > tempParent.nod->data) {
				tempParent.nod->nextInOrder = tempSucc;
			}
		}
	}
}

template<class T>
Node<T>* SortedStacklessBST<T>::getChildSuccessor(Node<T>* n)
{
	if (IsSuccessor == 1) {
		//goto right subtree and get the smallest node
		Node<T>* trav = n->right;
		while (trav->left != nullptr) {
			trav = trav->left;
		}
		return trav;
	}
	else {
		//goto left subtree and get biggest node
		Node<T>* trav = n->left;
		while (trav->right != nullptr) {
			trav = trav->right;
		}
		return trav;
	}
}
