using namespace std;
#include <iostream>
#include <algorithm> 
#include <climits>
#include <string>
#include <bitset>
#include <math.h>

static bool RED = true;
static bool BLACK = false;

template<class keytype, class valuetype>
class Node
{
public:
	Node(keytype key, valuetype val)
	{
		this.key = key;
		this.val = val;
		this.color = RED;
	};
private:
	keytype key;
	valuetype val;
	Node left, right;
	bool color;
};

template<class keytype, class valuetype>
class RBTree
{
public:
	valuetype* search(keytype key);
	void insert(keytype key, valuetype value);
	Node<keytype, valuetype> isert(Node<keytype, valuetype> h, keytype key, valuetype value);
	/*int remove(keytype k);
	int rank(keytype k);
	keytype select(int pos);
	void split(keytype k, RBTree<keytype, valuetype>& T1, RBTree<keytype, vlauetype>& T2);
	int size();
	void preorder();
	void inorder();
	void postorder();*/
	void inorder();

private:
	Node<keytype, valuetype> root;
	Node<keytype, valuetype> rotateLeft(Node<keytype, valuetype> h);
	Node<keytype, valuetype> rotateRight(Node<keytype, valuetype> h);
	void colorFlip(Node<keytype, valuetype> h);
};

template<class keytype, class valuetype>
valuetype* RBTree<keytype, valuetype>::search(keytype key) {
	Node x = root;
	while (x != NULL)
	{
		if ( key == x.key) return x.val;
		else if (key < x.key) x = x.left;
		else if (key > x.key) x = x.right;
	}
	return NULL;
}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::insert(keytype key, valuetype value)
{
	root = insert(root, key, value);
	root.color = BLACK;
}

template<class keytype, class valuetype>
Node<keytype, valuetype> RBTree<keytype, valuetype>::isert(Node<keytype, valuetype> h, keytype key, valuetype value)
{
	if (h == NULL) return new Node(key, value);
	if (h.left.color && h.right.color) colorFlip(h);

	if (key == h.key) h.val = value;
	else if (key < h.key) h.left = insert(h.left, key, value);
	else h.right = insert(h.right, key, value);

	if (h.right.color && !h.left.color) h = rotateLeft(h);
	if (h.left.color && h.left.left.color) h = rotateRight(h);
	return h;
}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::inorder()
{
	if (root == NULL)
		return;

	inorderHelper(root.left);
	cout << root.value << "  ";
	inorderHelper(root.right);
}

template<class keytype, class valuetype>
Node<keytype, valuetype> RBTree<keytype, valuetype>::rotateLeft(Node<keytype, valuetype> h)
{
	Node x = h.right;
	h.right = x.left;
	x.left = h;
	x.color = h.color;
	h.color = RED;
	return x;
}

template<class keytype, class valuetype>
Node<keytype, valuetype> RBTree<keytype, valuetype>::rotateRight(Node<keytype, valuetype> h)
{
	Node x = h.left;
	h.left = x.right;
	x.right = h;
	x.color = h.color;
	h.color = RED;
	return x;
}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::colorFlip(Node<keytype, valuetype> h)
{
	h.color = !h.color;
	h.left.color = !h.left.color;
	h.right.color = !h.right.color;
}
