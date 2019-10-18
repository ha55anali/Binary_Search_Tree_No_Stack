#pragma once

#include <iostream>
using namespace std;

template<class T>
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

template<class T>
class SortedStacklessBST {

private:
	Node<T>* root;
	bool IsSuccessor;
};
