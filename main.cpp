#include"StacklessBST.h"
#include <string>

bool isNumber(string const& str) {
	for (auto it = str.begin(); it != str.end(); ++it) {
		if (*it<'0' || *it>'9')
			return 0;
	}

	return 1;
}

int InputNum() {
	string temp;
	cin >> temp;

	while (!isNumber(temp) || temp == "" ) {
		cout << "Enter a valid number: ";
		cin.ignore();
		getline(cin, temp);
	}

	return stoi(temp);
}

void showMenu() {
	cout << "1. print trees"<<endl;
	cout << "2. print tree between values"<<endl;
	cout << "3. add element to the tree"<<endl;
	cout << "4. remove element"<<endl;
	cout << "5. search element"<<endl;
	cout << "6. check if the tree is balance"<<endl;
	cout << "7. reverse order of tree"<<endl;
	cout << "8. copy tree 1 to tree 2"<<endl;
	cout << "9. Quit" << endl;

}

void programLoop(SortedStacklessBST<int>* b1,SortedStacklessBST<int>* b2) {
	int temp;
	int temp2;

	bool exit = 0;

	while (!exit) {
		system("cls");
		showMenu();

		cout << endl << "tree 1: ";
		b1->print();
		cout << endl;

		cout << "Enter menu option: ";
		int choice = InputNum();

		switch (choice)
		{
		case 1:
			cout << endl << "tree 1: ";
			b1->print();
			cout << endl << "tree 2: ";
			b2->print();
			break;

		case 2:
			cout << "Enter lower bound: ";
			temp = InputNum();
			cout << "Enter upper bound: ";
			temp2 = InputNum();

			try {
				b1->print(temp2, temp);
			}
			catch (invalid_argument & e) {
				cout << e.what();
			}

			break;
		case 3:
			//add element
			cout << "enter integer to add to the tree: ";
			temp = InputNum();
			b1->insertStackless(temp);
			break;

		case 4:
			cout << "Enter element to remove: ";
			temp = InputNum();

			if (b1->remove(temp))
				cout << "removed";
			else
				cout << "not found";

			break;

		case 5:
			cout << "Enter element to search: ";
			temp = InputNum();
			if (b1->search(temp))
				cout << "Found";
			else
				cout << "Not found";
			break;

		case 6:
			if (b1->isBalanced())
				cout << "balanced";
			else
				cout << "not balanced";
			break;

		case 7:
			b1->reverseOrder();
			break;

		case 8:
			*b2 = *b1;
			break;
		
		case 9:
			exit = 1;
			break;

		default:
			cout << "Invalid choice";
			break;
		}
	
		cout << endl;
		system("pause");
	}
}

int main() {

	SortedStacklessBST<int>* b1 = new SortedStacklessBST<int>;
	SortedStacklessBST<int>* b2 = new SortedStacklessBST<int>;

	programLoop(b1, b2);

	/*
	cout << "sdg";
	int temp = InputNum();
	system("pause");*/

	//functions to implement
	//two lists, copy first to second
	//add elements
	//remove elements
	//search
	//print
	//balance
	//reverse order

	//showMenu();

	//SortedStacklessBST<int>* b=new SortedStacklessBST<int>;
	//SortedStacklessBST<int>* b2 = new SortedStacklessBST<int>;

	//for (int c = 0; c < 100; ++c) {
	//	b->insertStackless(c);
	//	b->reverseOrder();
	//}

	//b->print();

	//SortedStacklessBST<int>* b2 = new SortedStacklessBST<int>(*b);
	//delete b;
	//b2->print();
	//cout << endl;

	//for (int c = 100; c > 0; --c) {
	//	cout << b2->search(c);
	//	b2->remove(c);
	//}

	//cout << endl;
	//b2->print();

	//b->insertStackless(100);
	//b->insertStackless(50);
	//b->insertStackless(10);
	//b->insertStackless(70);
	//b->insertStackless(200);
	////b.insertStackless(150);
	////b.insertStackless(120);
	//b->insertStackless(170);
	//b->insertStackless(300);

	//b->print();
	//cout << endl;

	//SortedStacklessBST<int> b2;
	//b2 = *b;
	//delete b;

	//b2.print();
	//b2.reverseOrder();
	//cout << endl;
	//b2.print();
}