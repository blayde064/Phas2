using namespace std;
#include <iostream>
#include <algorithm> 
#include <climits>
#include <string>
#include <bitset>
#include <math.h>
#define COUNT 10 

static bool RED = true;
static bool BLACK = false;

template<class keytype, class valuetype>
class Node
{
public:
	Node();
	Node(keytype key, valuetype val);
	~Node();
	void print();
	keytype key;
	valuetype* val;
	Node* left;
	Node* right;
	bool color;	
	int sizeL;
	int sizeR;
	int size;
};

template<class keytype, class valuetype>
class RBTree
{
public:
	RBTree();
	RBTree(keytype k[], valuetype V[], int s);
	RBTree(const RBTree<keytype, valuetype>& other);
	~RBTree();
	valuetype* search(keytype key);
	void insert(keytype key, valuetype value);
	int rank(keytype k);
	keytype select(int pos);
	int remove(keytype k);
	void preorder();
	void postorder();
	void inorder();
	void inorderHelper(Node<keytype, valuetype>* root);
	void postorderHelper(Node<keytype, valuetype>* node);
	void preorderHelper(Node<keytype, valuetype>* node);
	void print2D();

	int size();

	Node<keytype, valuetype>* root;

private:

	void destruct(Node<keytype, valuetype>* h);
	Node<keytype, valuetype>* rotateLeft(Node<keytype, valuetype>* h);
	Node<keytype, valuetype>* rotateRight(Node<keytype, valuetype>* h);
	void colorFlip(Node<keytype, valuetype>* h);
	keytype select(Node<keytype, valuetype>* h, int k);
	Node<keytype, valuetype>* isert(Node<keytype, valuetype>* h, keytype key, valuetype value);
	int rank(keytype k, Node<keytype, valuetype>* x);
	int size(Node<keytype, valuetype>* x);
	int i_size;

	Node<keytype, valuetype>* DeleteRec(Node<keytype, valuetype>* pNode, keytype key);
	Node<keytype, valuetype>* FixUp(Node<keytype, valuetype>* pNode);
	Node<keytype, valuetype>* DeleteMin(Node<keytype, valuetype>* pNode);
	Node<keytype, valuetype>* FindMin(Node<keytype, valuetype>* pNode);
	Node<keytype, valuetype>* MoveRedRight(Node<keytype, valuetype>* pNode);
	Node<keytype, valuetype>* MoveRedLeft(Node<keytype, valuetype>* pNode);

	void copy_helper(Node<keytype, valuetype>* &copy_to, const Node<keytype, valuetype>* copy_from);

	void print2DUtil(Node<keytype, valuetype>* root, int space);
};

template<class keytype, class valuetype>
RBTree<keytype, valuetype>::RBTree()
{
	root = nullptr;
	i_size = 0;
}

template<class keytype, class valuetype>
RBTree<keytype, valuetype>::RBTree(keytype k[], valuetype V[], int s)
{
	i_size = s;
	for (int i = 0; i < s; i++) {
		insert(k[i], V[i]);
	}
}

template<class keytype, class valuetype>
RBTree<keytype, valuetype>::RBTree(const RBTree<keytype, valuetype>& other)
{
	copy_helper(this->root, other.root);
}

template<class keytype, class valuetype>
RBTree<keytype, valuetype>::~RBTree()
{
	destruct(this->root);
}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::destruct(Node<keytype, valuetype>* h)
{
	if (h->left == nullptr && h->right == nullptr) {
		delete h;
		return;
	}
	if(h->left != nullptr) destruct(h->left);
	if (h->right != nullptr) destruct(h->right);

}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::copy_helper(Node<keytype, valuetype>* &copy_to, const Node<keytype, valuetype>* copy_from)
{
	if (copy_from == nullptr) {
		copy_to = nullptr;
	}
	else {
		copy_to = new Node<keytype, valuetype>();
		copy_to->key = copy_from->key;
		copy_to->val = copy_from->val;
		copy_to->color = copy_from->color;
		copy_to->left = copy_from->left;
		copy_to->right = copy_from->right;
		copy_to->size = copy_from->size;
		copy_to->sizeL = copy_from->sizeL;
		copy_to->sizeR = copy_from->sizeR;
		copy_helper(copy_to->left, copy_from->left);
		copy_helper(copy_to->right, copy_from->right);
	}
}

template<class keytype, class valuetype>
valuetype* RBTree<keytype, valuetype>::search(keytype key) {
	Node<keytype, valuetype>* x = root;
	while (x != nullptr)
	{
		if ( key == x->key) return x->val;
		else if (key < x->key) x = x->left;
		else if (key > x->key) x = x->right;
	}
	return nullptr;
}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::insert(keytype key, valuetype value)
{
	i_size++;
	root = isert(root, key, value);
	root->color = BLACK;
}

template<class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::isert(Node<keytype, valuetype>* h, keytype key, valuetype value)
{
	if (h == nullptr) return new Node<keytype, valuetype>(key, value);
	if ((h->left != nullptr && h->left->color) && (h->right != nullptr && h->right->color)) colorFlip(h);

	if (key == h->key) 
		h->val = &value;
	else if (key < h->key)
		h->left = isert(h->left, key, value);
	else 
		h->right = isert(h->right, key, value);

	if ((h->right != nullptr && h->right->color)) h = rotateLeft(h);
	if ((h->left != nullptr && h->left->color) && (h->left->left != nullptr && h->left->left->color)) h = rotateRight(h);

	if (h->left != nullptr) h->sizeL = h->left->size;
	else h->sizeL = 0;

	if (h->right != nullptr) h->sizeR = h->right->size;
	else h->sizeR = 0;

	if (h->right != nullptr && h->left != nullptr) h->size = h->left->size + h->right->size + 1;
	else if (h->right != nullptr)  h->size = h->right->size + 1;
	else if (h->left != nullptr) h->size = h->left->size + 1;
	else h->size = 1;

	return h;

}

#pragma region DELETE


template<class keytype, class valuetype>
int RBTree<keytype, valuetype>::remove(keytype k)
{
	if (search(k) == nullptr) return 0;
	if (root!=nullptr) {
		root = DeleteRec(root, k);
		if (nullptr != root) {
			root->color=BLACK;
		}
	}
	return 1;
}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::preorder()
{
	preorderHelper(root);
	cout << endl;
}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::postorder()
{
	postorderHelper(root);
	cout << endl;
}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::postorderHelper(Node<keytype, valuetype>* node)
{
	if (node == nullptr)
		return;

	// first recur on left subtree 
	postorderHelper(node->left);

	// then recur on right subtree 
	postorderHelper(node->right);

	// now deal with the node 
	cout << node->key << " ";
}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::preorderHelper(Node<keytype, valuetype>* node)
{
	if (node == nullptr)
		return;

	/* first print data of node */
	cout << node->key << " ";

	/* then recur on left sutree */
	preorderHelper(node->left);

	/* now recur on right subtree */
	preorderHelper(node->right);
}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::inorder()
{
	inorderHelper(root);
	cout << endl;
}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::inorderHelper(Node<keytype, valuetype>* root)
{
	if (root == nullptr)
		return;

	inorderHelper(root->left);
	cout << root->key << " ";
	inorderHelper(root->right);
}


template<class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::DeleteRec(Node<keytype, valuetype>* pNode, keytype key)
{
	if (key < pNode->key) {
		if (nullptr != pNode->left) {
			if((pNode->left != nullptr && !pNode->left->color) 
				&& (pNode->left->left != nullptr && !pNode->left->left->color))
			{
				pNode = MoveRedLeft(pNode);
			}

			pNode->left = DeleteRec(pNode->left, key);
		}
	}
	else {
		if (pNode->left != nullptr && pNode->left->color) {
			pNode = rotateRight(pNode);
		}

		if ((key == pNode->key) &&
			(nullptr == pNode->right))
		{
			delete pNode;
			return nullptr;
		}

		if (nullptr != pNode->right) {
			if ((pNode->right != nullptr && !pNode->right->color)
				&& (pNode->right->left != nullptr && !pNode->right->left->color))
			{
				pNode = MoveRedRight(pNode);
			}

			if (key == pNode->key) {
				//print2DUtil(root, 0);

				Node<keytype, valuetype>* temp = FindMin(pNode->right);
				pNode->val= temp->val;
				pNode->key = temp->key;
				//pNode->left = temp->left;
				//pNode->right = temp->right;
				pNode->color=temp->color;
				//print2DUtil(root, 0);
				pNode->right = DeleteMin(pNode->right);
				//fprint2DUtil(root, 0);
			}
			else {
				pNode->right = DeleteRec(pNode->right, key);
			}
		}
	}

	return FixUp(pNode);
}

template<class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::FixUp(Node<keytype, valuetype>* pNode)
{
	// Fix right-leaning red nodes.
	if (pNode->right != nullptr && pNode->right->color) {
		pNode = rotateLeft(pNode);
	}

	// Detect if there is a 4-node that traverses down the left.
	// This is fixed by a right rotation, making both of the red
	// nodes the children of pNode.
	if ((pNode->left != nullptr && pNode->left->color)
		&& (pNode->left->left != nullptr && pNode->left->left->color))
	{
		pNode = rotateRight(pNode);
	}

	// Split 4-nodes.
	if ((pNode->left != nullptr && !pNode->left->color)
		&& (pNode->right != nullptr && !pNode->right->color))
	{
		colorFlip(pNode);
	}
	
	if (pNode->left != nullptr) pNode->sizeL = pNode->left->size;
	else pNode->sizeL = 0;

	if (pNode->right != nullptr) pNode->sizeR = pNode->right->size;
	else pNode->sizeR = 0;

	if (pNode->right != nullptr && pNode->left != nullptr) pNode->size = pNode->left->size + pNode->right->size + 1;
	else if (pNode->right != nullptr)  pNode->size = pNode->right->size + 1;
	else if (pNode->left != nullptr) pNode->size = pNode->left->size + 1;
	else pNode->size = 1;

	return pNode;
}

template<class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::DeleteMin(Node<keytype, valuetype>* pNode)
{
	if (pNode == nullptr) return nullptr;
	
	if (nullptr == pNode->left) {
		return nullptr;
	}

	if ((pNode->left != nullptr && !pNode->left->color)
		&& (pNode->left->left != nullptr && !pNode->left->left->color))
	{
		pNode = MoveRedLeft(pNode);
	}

	pNode->left = DeleteMin(pNode->left);

	return FixUp(pNode);
}

template<class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::FindMin(Node<keytype, valuetype>* pNode)
{
	while (nullptr != pNode->left) {
		pNode = pNode->left;
	}

	return pNode;
}

template<class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::MoveRedRight(Node<keytype, valuetype>* pNode)
{
	colorFlip(pNode);

	if ((nullptr!= pNode->left && nullptr != pNode->left->left) &&
		pNode->left->left->color)
	{
		pNode = rotateRight(pNode);

		colorFlip(pNode);
	}

	return pNode;
}

template<class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::MoveRedLeft(Node<keytype, valuetype>* pNode)
{
	colorFlip(pNode);

	if ((nullptr != pNode->right && nullptr!=pNode->right->left) && pNode->right->left->color)
	{
		pNode->right = rotateRight(pNode->right);
		pNode = rotateLeft(pNode);

		colorFlip(pNode);
	}

	return pNode;
}

#pragma endregion

template<class keytype, class valuetype>
int RBTree<keytype, valuetype>::rank(keytype k)
{
	return 1+rank(k, root);
}

template<class keytype, class valuetype>
int RBTree<keytype, valuetype>::rank(keytype k, Node<keytype, valuetype>* x)
{
	if (x == nullptr) return 0;
	if (k < x->key) return rank(k, x->left);
	if (k > x->key) return 1 + x->left->size+ rank(k, x->right);
	else {
		if (x->left == nullptr) return 0;
		else return x->left->size;
	}
}

template<class keytype, class valuetype>
keytype RBTree<keytype, valuetype>::select(int pos)
{
	return select(root, pos-1);
}


template<class keytype, class valuetype>
keytype RBTree<keytype, valuetype>::select(Node<keytype, valuetype>* h, int k)
{
	//if(h==nullptr) 
	int t = h->sizeL;

	if (t > k) return select(h->left, k);
	else if (t < k) return select(h->right, k - t - 1);
	else return h->key;
}

template<class keytype, class valuetype>
int RBTree<keytype, valuetype>::size(Node<keytype, valuetype>* x)
{
	if (x == nullptr) return 0;
	else return 1 + size(x->left) + size(x->right);
}

template<class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::rotateLeft(Node<keytype, valuetype>* h)
{
	Node<keytype, valuetype>* x = h->right;
	h->right = x->left;
	x->left = h;
	x->color = h->color;
	h->color = RED;
	
	if (x->left != nullptr) x->sizeL = x->left->size;
	else x->sizeL = 0;
	if (x->right != nullptr) x->sizeR = x->right->size;
	else x->sizeR = 0;
	if (x->right != nullptr && x->left != nullptr) x->size = x->left->size + x->right->size + 1;
	else if (x->right != nullptr)  x->size = x->right->size + 1;
	else if (x->left != nullptr) x->size = x->left->size + 1;
	else x->size = 1;

	if (h->left != nullptr) h->sizeL = h->left->size;
	else h->sizeL = 0;
	if (h->right != nullptr) h->sizeR = h->right->size;
	else h->sizeR = 0;
	if (h->right != nullptr && h->left != nullptr) h->size = h->left->size + h->right->size + 1;
	else if (h->right != nullptr)  h->size = h->right->size + 1;
	else if (h->left != nullptr) h->size = h->left->size + 1;
	else h->size = 1;

	return x;
}

template<class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::rotateRight(Node<keytype, valuetype>* h)
{
	Node<keytype, valuetype>* x = h->left;
	h->left = x->right;
	x->right = h;
	x->color = h->color;
	h->color = RED;

	//x->size = h->size;

	if (x->left != nullptr) x->sizeL = x->left->size;
	else x->sizeL = 0;
	if (x->right != nullptr) x->sizeR = x->right->size;
	else x->sizeR = 0;
	if (x->right != nullptr && x->left != nullptr) x->size = x->left->size + x->right->size + 1;
	else if (x->right != nullptr)  x->size = x->right->size + 1;
	else if (x->left != nullptr) x->size = x->left->size + 1;
	else x->size = 1;

	if (h->left != nullptr) h->sizeL = h->left->size;
	else h->sizeL = 0;
	if (h->right != nullptr) h->sizeR = h->right->size;
	else h->sizeR = 0;
	if (h->right != nullptr && h->left != nullptr) h->size = h->left->size + h->right->size + 1;
	else if (h->right != nullptr)  h->size = h->right->size + 1;
	else if (h->left != nullptr) h->size = h->left->size + 1;
	else h->size = 1;

	return x;
}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::colorFlip(Node<keytype, valuetype>* h)
{
	h->color = !h->color;
	h->left->color = !h->left->color;
	h->right->color = !h->right->color;
}


template<class keytype, class valuetype>
int RBTree<keytype, valuetype>::size()
{
	return this->root->size;
}



template<class keytype, class valuetype>
Node<keytype, valuetype>::Node()
{
	left = nullptr;
	right = nullptr;
	color = RED;
}

template<class keytype, class valuetype>
Node<keytype, valuetype>::Node(keytype key, valuetype val)
{
	this->key = key;
	this->val = new valuetype(val);
	this->color = RED;
	this->size = 1;
	this->sizeL = 0;
	this->sizeR = 0;
}

template<class keytype, class valuetype>
Node<keytype, valuetype>::~Node()
{
	delete this->val;
	delete this->left;
	delete this->right;
}
template<class keytype, class valuetype>
void Node<keytype, valuetype>::print()
{
	if (this == nullptr) return;
	cout << "Key: " << this->key << " Value: " << *this->val << " Color:" <<  (this->color ? " Red" : " Black") << " Left: "<<this->left<<" Right: "<<this->right<<endl;
}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::print2D()
{
	print2DUtil(root, 0);
}

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::print2DUtil(Node<keytype, valuetype>* root, int space)
{
	if (root == nullptr)
		return;

	// Increase distance between levels  
	space += COUNT;

	// Process right child first  
	print2DUtil(root->right, space);

	// Print current node after space  
	// count  
	cout << endl;
	for (int i = COUNT; i < space; i++)
		cout << " ";
	cout << root->key <<","<<*root->val<<" L:"<<root->sizeL<<" R:"<<root->sizeR<<" S:"<<root->size<<"\n";

	// Process left child  
	print2DUtil(root->left, space);
}
