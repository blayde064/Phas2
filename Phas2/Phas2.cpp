#include <iostream>
using namespace std;
#include <algorithm> 
#include <chrono> 
#include "RBTree.cpp"

int main()
{
	string K[10] = { "A","B","C","D","E","F","H","I","J","K" };
	int V[10] = { 10,9,8,7,6,5,4,3,2,1 };

	RBTree<string, int> T1, T2(K, V, 10);

	for (int i = 0; i < 10; i++) {
		T1.insert(K[i], V[i]);
	}
	// T1 and T2 should be identical trees

	cout << *(T2.search("C")) << endl;
	// Should output 8

	cout << T1.rank("C") << endl;
	//Should output 3

	cout << T1.select(3) << endl;
	//Should output C 

	T2.preorder();
	//Should output D B A C I F E H K J

	T2.inorder();
	//Should output	A B C D E F H I J K

	cout << T2.remove("G") << endl;
	//Should output 0

	cout << T2.remove("B") << endl;
	//Should output 1

	cout << T2.rank("J") << endl;
	//Should output 

	T2.inorder();
	//Should output	A C D E F H I J K

	//RBTree<string, int> L, R;
	//T1.split("G", L, R);
	//cout << R.rank("K") << endl;
	// Should output 4


	auto start1 = chrono::high_resolution_clock::now();
	RBTree<int, int> X1;
	for (int i = 1; i < 1000000; i++) X1.insert(i, i);
	for (int i = 1; i < 1000000; i++) {
		if (X1.rank(i) != i) cout << "Rank error" << endl;
		if (X1.select(i) != i) cout << "Select error: " << X1.select(i) << " instead of " << i << endl;
		if (*(X1.search(i)) != i) cout << "Search error" << endl;
	}
	auto stop1 = chrono::high_resolution_clock::now();
	auto duration1 = chrono::duration_cast<chrono::microseconds>(stop1 - start1);

	cout << "Time taken by function: "
		<< duration1.count()/1000000.0 << " seconds" << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
