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
	bool insertStackless(T d);

	bool remove(T d);

	void print();
	void print(T const& high, T const& low);

	bool search(T const& d);
private:
	Node<T>* root;
	bool IsSuccessor;
	

	bool remove(T d, Node<T>*& r, Node<T>* prev);
	Node<T>* detach(T d, Node<T>*& r, Node<T>* prev);
	void DislocateLeaf(T d, Node<T>*& r, Node<T>* prev);

	void updateNextInOrder(Node<T>*& r, Node<T>* prev);

	//updates successor of parents of new node and finds successor of new node
	void updateSuccessor(stack<PushNode<T>> traceTree, Node<T>* newNode);
	//searches for successor of passed node
	Node<T>* getChildSuccessor(Node<T>* n);
};

template<class T>
SortedStacklessBST<T>::SortedStacklessBST()
{
	root = nullptr;
	IsSuccessor = 0;
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

template<class T>
bool SortedStacklessBST<T>::insertStackless(T d)
{
	//empty tree
	if (root == nullptr) {
		root = new Node<T>(d);
		return 1;
	}

	Node<T>* newNode = new Node<T>(d);
	Node<T>* trav = root;
	Node<T>* trail = root;

	while (trav != nullptr) {

		//right subtree
		if (d > trav->data) {
			if (IsSuccessor == 1) {

				//update trav successor
				if (trav->nextInOrder == nullptr) {
					trav->nextInOrder = newNode;
				}
				else {
					if (d < trav->nextInOrder->data) {
						trav->nextInOrder = newNode;
					}
				}
			}
			else{
				//update new node successor
				newNode->nextInOrder = trav;
			}
			trail = trav;
			trav = trav->right;
		}

		//left subtree
		else if (d < trav->data) {
			if (IsSuccessor == 0) {
				//update trav successor
				if (trav->nextInOrder == nullptr) {
					trav->nextInOrder = newNode;
				}
				else {
					if (d > trav->nextInOrder->data) {
						trav->nextInOrder = newNode;
					}
				}
			}
			else  {
				//update new node successor
				newNode->nextInOrder = trav;
			}
			trail = trav;
			trav = trav->left;
		}

		else {
			delete newNode;
			return 0;
		}
	}

	if (trail->data > d) {
		trail->left = newNode;
	}
	else {
		trail->right = newNode;
	}
	return 1;
}

template<class T>
bool SortedStacklessBST<T>::remove(T d)
{
	return remove(d, root, nullptr);
}

template<class T>
void SortedStacklessBST<T>::print()
{
	Node<T>* temp = root;

	if (IsSuccessor == 1) {
		while (temp != nullptr && temp->left) {
			temp = temp->left;
		}
	}
	else {
		while (temp != nullptr && temp->right) {
			temp = temp->right;
		}
	}

	while (temp) {
		cout << temp->data << " ";
		temp = temp->nextInOrder;
	}

}

template<class T>
void SortedStacklessBST<T>::print(T const& high, T const& low)
{
	Node<T>* temp = root;

	if (IsSuccessor == 1) {
		while (temp != nullptr && temp->left) {
			temp = temp->left;
		}
	}
	else {
		while (temp != nullptr && temp->right) {
			temp = temp->right;
		}
	}

	while (temp && temp->data<high) {
		cout << temp->data << " ";
		temp = temp->nextInOrder;
	}
}

template<class T>
bool SortedStacklessBST<T>::search(T const& d)
{
	Node<T>* trav = root;

	while (trav != nullptr) {
		//right
		if (d > trav->data) {
			trav = trav->right;
		}
		//left
		else if (d < trav->data) {
			trav = trav->left;
		}
		else if (d == trav->data) {
			break;
		}
	}

	if (trav == nullptr)
		return 0;
	else
		return 1;
}

template<class T>
bool SortedStacklessBST<T>::remove(T d, Node<T>*& r, Node<T>* prev)
{
	Node<T>* temp = detach(d, r, prev);
	if (temp == nullptr)
		return 0;
	
	delete temp;
	return 1;
}

template<class T>
 Node<T>* SortedStacklessBST<T>::detach(T d, Node<T>*& r, Node<T>* prev)
{
	 //data not found
	 if (r == nullptr)
		 return nullptr;
	 //right
	 if (d > r->data) {
		 if (IsSuccessor == 1)
			 return detach(d, r->right, r);
		 else
			 return detach(d, r->right, prev);
	 }
	 //left
	 else if (d < r->data) {
		 if (IsSuccessor == 1)
			 return detach(d, r->left, prev);
		 else
			 return detach(d, r->left, r);
	 }
	 else
	 { //node found
		 //finds the node whose nextInOrder points to r and updates that
		 updateNextInOrder(r, prev);

		 //delete node
		 if (r->left == nullptr && r->right == nullptr) {
			 //case 1
			 Node<T>* temp = r;
			 r = nullptr;
			 return temp;
		 }
		 else if (r->left == nullptr) {
			 //case 2
			 Node<T>* temp = r;
			 r = r->right;
			 return temp;
		 }
		 else if (r->right == nullptr) {
			 //case 2
			 Node<T>* temp = r;
			 r = r->left;
			 return temp;
		 }
		 else
		 {
			 //case 3

			 //find and detach successor
			 Node<T>* tempSuc = getChildSuccessor(r);
			 if (IsSuccessor == 1) {
				 DislocateLeaf(tempSuc->data, r->right, r);
			 }
			 else
				 DislocateLeaf(tempSuc->data, r->left, r);
			
			 //replace successor with r
			 Node<T>* temp = r;
			 r = tempSuc;
			 r->left = temp->left;
			 r->right = temp->right;
			
			 return temp;
		 }

	 }
}

 template<class T>
 void SortedStacklessBST<T>::DislocateLeaf(T d, Node<T>*& r, Node<T>* prev)
 {
	 //data not found
	 if (r == nullptr)
		 return;
	 //right
	 if (d > r->data) {
		 if (IsSuccessor == 1)
			 DislocateLeaf(d, r->right, r);
		 else
			 DislocateLeaf(d, r->right, prev);
	 }
	 //left
	 else if (d < r->data) {
		 if (IsSuccessor == 1)
			 DislocateLeaf(d, r->left, prev);
		 else
			 DislocateLeaf(d, r->left, r);
	 }
	 else
	 { //node found
		 if (r->left == nullptr && r->right == nullptr) {
			 //case 1
			 Node<T>* temp = r;
			 r = nullptr;
		 }
	 }
 }

template<typename T>
void SortedStacklessBST<T>::updateNextInOrder(Node<T>*& r,Node<T>* prev) {
	//update nextInOrder of previous node
	IsSuccessor = !IsSuccessor;
	Node<T>* prevChild = getChildSuccessor(r);
	IsSuccessor = !IsSuccessor;

	if (prev == nullptr && prevChild == nullptr)
		return;

	if (prev != nullptr && prevChild==nullptr) {
		prev->nextInOrder = r->nextInOrder;
		return;
	}

	if (prevChild != nullptr && prev==nullptr) {
		prevChild->nextInOrder = r->nextInOrder;
		return;
	}

	if (IsSuccessor == 1) {

		if (prev->data < prevChild->data)
			prev = prevChild;
	}
	else {
		if (prev->data > prevChild->data)
			prev = prevChild;
	}

	prev->nextInOrder = r->nextInOrder;
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
		while (trav!=nullptr && trav->left != nullptr) {
			trav = trav->left;
		}
		return trav;
	}
	else {
		//goto left subtree and get biggest node
		Node<T>* trav = n->left;
		while (trav != nullptr && trav->right != nullptr) {
			trav = trav->right;
		}
		return trav;
	}
}
