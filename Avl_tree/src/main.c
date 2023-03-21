#include <stdio.h>
#include <stdlib.h>
#define HERE fprintf(stderr, "HERE %i\n", __LINE__);
typedef struct Node Node;
struct Node {
    int value;
    int height;
    Node *left;
    Node *right;
};

void lab6();
Node* Destroy(Node *root);
Node* Insert(Node *root, Node* newRoot, int d);
Node* TurnLeft(Node *root);
Node* TurnRight(Node *root);
Node* SetBalance(Node *root);
short BFactor(Node* p);
int GetHeight(Node *root);
Node* NewNode(Node* node, Node* newRoot, int key);
int Max(int a, int b);

int main() {
	lab6();
	return 0;
}

void lab6() {
	int n;
	Node *tree = NULL;
	if (!scanf("%d", &n)) return;
	if (n == 0) {
		printf("%d", 0);
		return;
	}
	tree = (Node*)calloc(n, sizeof(Node));
	int value;
	if (!scanf("%d", &value)) {
		free(tree);
		return;
	}
	tree[0] = (Node){value, 1, NULL, NULL};
	Node* root = &tree[0];
	for (int i = 1; i < n; i++) {
		if (!scanf("%d", &value)) return;
		root = Insert(root, &tree[i], value);
	}
	printf("%d\n", root->height);
	free(tree);
}

void FixHeight(Node *root) {
	root->height = Max(GetHeight(root->left), GetHeight(root->right))+1;
}

Node* Insert(Node *root, Node* newRoot, int value) {
	if (!root) {
		Node *NewRoot = newRoot;
		NewRoot->value = value;
		NewRoot->height = 1;
		return NewRoot;
	}
	if (value > root->value) {
		root->right = Insert(root->right, newRoot, value);
	} else {
		root->left = Insert(root->left, newRoot, value);
	}
	
	if (BFactor(root) > 1) {
		if (BFactor(root->right) < 0) {
			root->right = TurnRight(root->right);
		}
		return TurnLeft(root);
	} else if (BFactor(root) < -1) {
		if (BFactor(root->left) > 0) {
			root->left = TurnLeft(root->left);
		}
		return TurnRight(root);
	} else {
		FixHeight(root);
	}
	return root;
}


int Max(int a, int b) {
    return (a > b) ? a : b;
}

int GetHeight(Node *root) {
	if (!root) return 0;
	return root->height;
}

short BFactor(Node* p) {
	return GetHeight(p->right) - GetHeight(p->left);
}

Node* TurnRight(Node *root) {
    // in:   (A L B) root (R)
    // out:  (A) L (B root R)
    Node *L = root->left;
    Node *B = L->right;
    L->right = root;
    root->left = B;
    FixHeight(root);
    FixHeight(L);
    return L;
}

Node* TurnLeft(Node *root) {
	// in:    (L) root (A R B)
    // out:   (L root A) R (B)
    Node *R = root->right;
    Node *A = R->left;
    R->left = root;
    root->right = A;
    FixHeight(root);
    FixHeight(R);
    return R;
}

