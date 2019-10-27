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

	//default constructor
	Node() {
		left = nullptr;
		right = nullptr;
		nextInOrder = nullptr;
	}

	//parametric constructor
	Node(T d, Node<T>* r = 0, Node<T>* l = 0, Node<T>* next = 0) {
		data = d;
		left = l;
		right = r;
		nextInOrder = next;
	}
};

template<class T>
class SortedStacklessBST {
public:
	SortedStacklessBST();
	~SortedStacklessBST();
	SortedStacklessBST(SortedStacklessBST const&);
	//bool insert(T d);
	bool insertStackless(T d);

	bool remove(T d);

	void print();
	void print(T const& high, T const& low);

	bool search(T const& d);
	//returns 1 if the tree is perfectly balanced
	bool isBalanced();

	//toggles nextInOrder storing successors or predecessors
	void reverseOrder();

	SortedStacklessBST<T>& operator=(SortedStacklessBST<T> const&);
private:
	Node<T>* root;
	bool IsSuccessor;

	//recursive function for toggling nextInOrder storing successors or predecessors
	Node<T>* reverseOrder(Node<T>* r);

	//deletes the whole tree
	void delTree();
	
	//recursive definition for copying source node and its subtree to this
	void copy( Node<T>* source);

	//recursive definition of checking balance of the passed node
	bool Balance(Node<T>* r,int currDept, int& maxDepth);

	//recursive definition of removing node with data d
	bool remove(T d, Node<T>*& r, Node<T>* prev);
	//detaches node with data d and returns it
	Node<T>* detach(T d, Node<T>*& r, Node<T>* prev);
	//detaches leaf with data d and returns it
	void DislocateLeaf(T d, Node<T>*& r, Node<T>* prev);

	//remove node r from the linked list of successor/predecessors
	void updateNextInOrder(Node<T>*& r, Node<T>* prev);

	//searches for successor of passed node
	Node<T>* getChildSuccessor(Node<T>* n);
};

template<class T>
SortedStacklessBST<T>::SortedStacklessBST()
{
	root = nullptr;
	IsSuccessor = 0;
}

template<class T>
SortedStacklessBST<T>::~SortedStacklessBST()
{
	delTree();
}

template<class T>
SortedStacklessBST<T>::SortedStacklessBST(SortedStacklessBST const& obj)
{
	copy(obj.root);
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
	Node<T>* prev = nullptr; //stores previous node in the linked list

	//find location of the new node
	while (trav != nullptr) {

		//right subtree
		if (d > trav->data) {
			if (IsSuccessor == 1) {

				//if at the end of the linked list, insert at end
				if (trav->nextInOrder == nullptr) {
					prev = trav;
				}
				//if r->data < data < next->data insert node here
				else {
					if (d < trav->nextInOrder->data) {
						prev = trav;
					}
				}
			}
			else if (IsSuccessor == 0) {
				//update predecessor of new node
				newNode->nextInOrder = trav;
			}
			trail = trav;
			trav = trav->right;
		}

		//left subtree
		else if (d < trav->data) {
			if (IsSuccessor == 0) {
				//if at end of linked list, insert node at end
				if (trav->nextInOrder == nullptr) {
					prev = trav;
				}
				else {
					// if next < data < current insert node here
					if (d > trav->nextInOrder->data) {
						prev = trav;
					}
				}
			}
			else if (IsSuccessor == 0) {
				//update new node successor
				newNode->nextInOrder = trav;
			}
			trail = trav;
			trav = trav->left;
		}

		//if duplicate found
		else {
			delete newNode;
			return 0;
		}
	}

	//update previous
	if (prev!=nullptr)
		prev->nextInOrder = newNode;

	//insert node in tree
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
	if (root == nullptr) {
		cout << "tree is empty";
		return;
	}

	Node<T>* temp = root;

	//find start of linked list
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

	//print till end of linked list
	while (temp) {
		cout << temp->data << " ";
		temp = temp->nextInOrder;
	}

}

template<class T>
void SortedStacklessBST<T>::print(T const& high, T const& low)
{
	Node<T>* temp = root;

	//find start of linked list
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

	//print list
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
bool SortedStacklessBST<T>::isBalanced()
{
	int maxDepth = -1;
	return Balance(root,0,maxDepth);
}

template<class T>
void SortedStacklessBST<T>::reverseOrder()
{
	if (root == nullptr)
		return;

	//find start of linked list
	Node<T>* temp = root;
	if (IsSuccessor == 1) {
		while (temp->left != nullptr)
			temp = temp->left;
	}
	else {
		while (temp->right != nullptr)
			temp = temp->right;
	}

	//invert bool variable
	IsSuccessor = !IsSuccessor;

	Node<T>* tempRet = reverseOrder(temp);
	tempRet->nextInOrder = nullptr;
}

template<class T>
SortedStacklessBST<T>& SortedStacklessBST<T>::operator=(SortedStacklessBST<T> const& s)
{
	//delete current data
	delTree();

	copy(s.root);

	return *this;
}

template<class T>
Node<T>* SortedStacklessBST<T>::reverseOrder(Node<T>* r)
{
	if (r->nextInOrder==nullptr)
		return r;

	//uses system stack to store the linked list
	//when unwinding stack, insert stored data at the end

	Node<T>* temp = r;
	Node<T>* tempret = reverseOrder(r->nextInOrder);
	tempret->nextInOrder = r;
	return r;
}

template<class T>
void SortedStacklessBST<T>::delTree()
{
	//if tree empty
	if (root == nullptr)
		return;


	Node<T> * trav = root;

	//get start of linked list element
	if (IsSuccessor == 1) {
		while (trav->left != nullptr)
			trav = trav->left;
	}
	else {
		while (trav->right != nullptr)
			trav = trav->right;
	}

	Node<T>* trail = root;

	while (trav != nullptr) {
		trail = trav;
		trav = trav->nextInOrder;
		delete trail;
	}
}

template<class T>
void SortedStacklessBST<T>::copy(Node<T>* source)
{
	if (source == nullptr)
		return;

	insertStackless(source->data);
	copy(source->left);
	copy(source->right);
}

template<class T>
bool SortedStacklessBST<T>::Balance(Node<T>* r,int currDepth, int& maxDepth)
{
	if (r == nullptr)
		return 1;
	//base case, leaf found
	if (r->left == nullptr && r->right == nullptr) {
		//if maxDepth not set, set maxDepth
		if (maxDepth == -1) {
			maxDepth = currDepth;
			return 1;
		}
		//if leaf not at maxDepth then tree is not balanced
		else {
			if (currDepth != maxDepth)
				return 0;
			else
				return 1;
		}
	}

	//return 1 if both right and left subtree are balanced
	if (Balance((r->left), currDepth + 1, maxDepth))
		if (Balance((r->right), currDepth + 1, maxDepth))
			return 1;
	return 0;
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

	//get the closest previous node in the children of r
	IsSuccessor = !IsSuccessor;
	Node<T>* prevChild = getChildSuccessor(r);
	IsSuccessor = !IsSuccessor;

	//if no previous node, change nothing
	if (prev == nullptr && prevChild == nullptr)
		return;

	//if previous node in the parent
	if (prev != nullptr && prevChild==nullptr) {
		prev->nextInOrder = r->nextInOrder;
		return;
	}

	//if previous node in the child
	if (prevChild != nullptr && prev==nullptr) {
		prevChild->nextInOrder = r->nextInOrder;
		return;
	}

	//if both previous nodes, update the one closest to the node
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
