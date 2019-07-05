#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// data type for avl tree nodes
typedef struct AVLTreeNode {
	int key; //key of this item
	int  value;  //value (int) of this item 
	int height; //height of the subtree rooted at this node
	struct AVLTreeNode *parent; //pointer to parent
	struct AVLTreeNode *left; //pointer to left child
	struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;
//data type for AVL trees
typedef struct AVLTree{
	int  size;      // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;
typedef struct queue{
	int rear, front;
	AVLTreeNode *Q[50];
}queue;

AVLTreeNode *Search(AVLTree *, int, int);
AVLTreeNode *newAVLTreeNode(int, int);
int balance(AVLTreeNode *);
int MAX(AVLTreeNode *, AVLTreeNode *);
AVLTreeNode * LL(AVLTreeNode *);
AVLTreeNode * RR(AVLTreeNode *);
AVLTreeNode * LR(AVLTreeNode *);
AVLTreeNode * RL(AVLTreeNode *);
AVLTree *newAVLTree();
AVLTree *CreateAVLTree(const char *);
AVLTree *CloneAVLTree(AVLTree *);
AVLTree *AVLTreesUnion(AVLTree *, AVLTree *);
AVLTree *AVLTreesIntersection(AVLTree *, AVLTree *);
int TreeDeep(AVLTreeNode*);
int InsertNode(AVLTree *, int, int);
int DeleteNode(AVLTree *, int, int);
void FreeAVLTree(AVLTree *);
void PrintAVLTree(AVLTreeNode *);
void FreeAVLTreeNode(AVLTreeNode*);



// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v)
{
	AVLTreeNode *new;
	new = malloc(sizeof(AVLTreeNode));
	assert(new != NULL);
	new->key = k;
	new->value = v;
	new->height = 0; // height of this new node is set to 0
	new->left = NULL; // this node has no child
	new->right = NULL;
	new->parent = NULL; // no parent
	return new;
}


int balance(AVLTreeNode * A)
{
	if (A->left == NULL&&A->right != NULL)
		return (-1 - A->right->height);
	else if (A->right == NULL&&A->left != NULL)
		return(A->left->height + 1);
	else if (A->left != NULL&&A->right != NULL)
		return(A->left->height - A->right->height);
}
int MAX(AVLTreeNode * A, AVLTreeNode * B)
{
	if (A == NULL&&B == NULL)
		return 0;
	else if (A == NULL&&B != NULL)
		return B->height + 1;
	else if (A != NULL&&B == NULL)
		return A->height + 1;
	else
	{
		if (A->height > B->height)
			return A->height + 1;
		else
		{
			return B->height + 1;
		}
	}
}
//LL
AVLTreeNode * LL(AVLTreeNode * A) {

	AVLTreeNode  *B, *BR;

	B = A->left;
	BR = B->right;

	A->parent = B;
	B->right = A;
	if (BR != NULL)
		BR->parent = A;
	A->left = BR;
	B->parent = NULL;

	A->height = MAX(A->left, A->right);
	B->height = MAX(B->left, B->right);
	return B;
}

//RR
AVLTreeNode * RR(AVLTreeNode * A) {
	AVLTreeNode *BL, *B;

	B = A->right;
	BL = B->left;
	A->parent = B;
	B->left = A;
	if (BL != NULL)
		BL->parent = A;
	A->right = BL;
	B->parent = NULL;
	A->height = MAX(A->left, A->right);
	B->height = MAX(B->left, B->right);
	return B;
}

//LR
AVLTreeNode * LR(AVLTreeNode * A) {
	AVLTreeNode *B, *C;
	B = A->left;

	A->left = RR(B);
	C = LL(A);

	return C;
}

//RL
AVLTreeNode * RL(AVLTreeNode *A) {

	AVLTreeNode *B, *C;
	B = A->right;
	A->right = LL(B);
	C = RR(A);
	return C;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
	AVLTree *T;
	T = malloc(sizeof (AVLTree));
	assert(T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}

// put your time complexity analysis of CreateAVLTree() here    O(log n)
AVLTree *CreateAVLTree(const char *filename)
{
	// put your code here
	AVLTree *tree = newAVLTree();
	char szTest[1000] = { 0 }, sz_temp[1000] = { 0 };
	int point[1000] = { 0 };
	int len = 0, x = 0;
	int sig = 0;

	if (filename == "stdin")
	{
		printf("请输入键值对，输入空行结束输入\n");
		char szTest2[1000];
		char ch = 'T';
		gets(szTest2);
		while (szTest2 != '\n')
		{
			if (szTest2[0] == '\0')
				break;
			strcat(szTest, szTest2);
			gets(szTest2);
		}

		//gets(szTest);
		for (int i = 0; i <= strlen(szTest); i++)
		{
			if (szTest[i] >= '0'&&szTest[i] <= '9' || szTest[i] == '-')
			{
				sz_temp[0] = szTest[i];
				for (int j = i + 1;; j++)
				{
					if (szTest[j] >= '0'&&szTest[j] <= '9')
					{
						sig = 1;
						int len = strlen(sz_temp);
						sz_temp[len] = szTest[j];
						sz_temp[len + 1] = 0;
						i++;
					}
					else
					{
						if (sig == 0)
							sz_temp[0] = szTest[i];
						break;
					}

				}
				sig = 0;
				point[x] = atoi(sz_temp);
				x++;
				memset(sz_temp, 0, sizeof(sz_temp));
			}
		}
	}
	else
	{
		FILE *fp = fopen(filename, "r");
		if (fp == NULL)
		{
			printf("failed to open dos.txt\n");
			exit(0);
		}
		while (!feof(fp))
		{
			memset(szTest, 0, sizeof(szTest));
			fgets(szTest, sizeof(szTest)-1, fp); // 包含了换行符 
			for (int i = 0; i <= strlen(szTest); i++)
			{
				if (szTest[i] >= '0'&&szTest[i] <= '9' || szTest[i] == '-')
				{
					sz_temp[0] = szTest[i];
					for (int j = i + 1;; j++)
					{
						if (szTest[j] >= '0'&&szTest[j] <= '9')
						{
							sig = 1;
							int len = strlen(sz_temp);
							sz_temp[len] = szTest[j];
							sz_temp[len + 1] = 0;
							i++;
						}
						else
						{
							if (sig == 0)
								sz_temp[0] = szTest[i];
							break;
						}

					}
					sig = 0;
					point[x] = atoi(sz_temp);
					x++;
					memset(sz_temp, 0, sizeof(sz_temp));
				}
			}
		}
		fclose(fp);
	}

	for (int i = 0; i < x-1; i += 2)
	{
		InsertNode(tree, point[i], point[i + 1]);
	}

	return tree;
}

// put your time complexity analysis for CloneAVLTree() here   O(n)
AVLTree *CloneAVLTree(AVLTree *T)
{
	AVLTreeNode *r = T->root;
	AVLTree *tree = newAVLTree();
	queue que;
	que.front = que.rear = 1;
	if (r == NULL)
		return NULL;
	que.Q[++que.rear] = r;
	while (que.front != que.rear){
		AVLTreeNode *q;
		q = que.Q[++que.front];
		InsertNode(tree, q->key, q->value);
		if (q->left != NULL)que.Q[++que.rear] = q->left;
		if (q->right != NULL)que.Q[++que.rear] = q->right;
	}
	return tree;
}

// put your time complexity for ALVTreesUNion() here    O(n)
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
	AVLTree *tree = newAVLTree();

	AVLTreeNode *r1 = T1->root, *r2 = T2->root;
	//AVLTree *tree = newAVLTree();
	queue que;
	que.front = que.rear = 1;

	que.Q[++que.rear] = r1;
	while (que.front != que.rear){
		AVLTreeNode *q;
		q = que.Q[++que.front];
		InsertNode(tree, q->key, q->value);
		if (q->left != NULL)que.Q[++que.rear] = q->left;
		if (q->right != NULL)que.Q[++que.rear] = q->right;
	}
	queue q;
	q.front = q.rear = 1;

	que.Q[++que.rear] = r2;
	while (que.front != que.rear){
		AVLTreeNode *q;
		q = que.Q[++que.front];
		InsertNode(tree, q->key, q->value);
		if (q->left != NULL)que.Q[++que.rear] = q->left;
		if (q->right != NULL)que.Q[++que.rear] = q->right;
	}
	return tree;
}

// put your time complexity for ALVTreesIntersection() here      O(n)
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
	AVLTree *tree = newAVLTree();
	AVLTreeNode *r1 = T1->root, *r2 = T2->root;
	//AVLTree *tree = newAVLTree();
	queue que;
	que.front = que.rear = 1;

	que.Q[++que.rear] = r1;
	while (que.front != que.rear){
		AVLTreeNode *q;
		q = que.Q[++que.front];
		if (Search(T2, q->key, q->value) != NULL)
			InsertNode(tree, q->key, q->value);
		if (q->left != NULL)que.Q[++que.rear] = q->left;
		if (q->right != NULL)que.Q[++que.rear] = q->right;
	}
	return tree;
}

int TreeDeep(AVLTreeNode* T)
{
	int deep = 0;
	if (T)
	{
		int leftdeep = TreeDeep(T->left);
		int rightdeep = TreeDeep(T->right);
		deep = leftdeep >= rightdeep ? leftdeep + 1 : rightdeep + 1;
		T->height = deep - 1;
	}
	return deep;
}


// put the time complexity analysis for InsertNode() here    O(log n)   
int InsertNode(AVLTree *T, int k, int v)
{

	if (T->root == NULL){
		T->root = newAVLTreeNode(k, v);
		T->size++;
	}
	else
	{
		AVLTreeNode *r = T->root, *temp;
		temp = newAVLTreeNode(k, v);
		while (1){
			if (k > r->key){
				if (r->right != NULL)
					r = r->right;
				else{
					temp->parent = r;
					r->right = temp;
					break;
				}
			}
			else if (k < r->key){
				if (r->left != NULL)
					r = r->left;
				else{
					temp->parent = r;
					r->left = temp;
					break;
				}
			}
			else{
				if (v>r->value){
					if (r->right)
						r = r->right;
					else{
						temp->parent = r;
						r->right = temp;
						break;
					}
				}
				else if (v<r->value){
					if (r->left)
						r = r->left;
					else{
						temp->parent = r;
						r->left = temp;
						break;
					}
				}
				else
					return 0;
			}
		}
		T->size++;
		r = T->root;
		TreeDeep(r);

		temp = temp->parent;
		while (temp->parent != NULL)
		{
			temp = temp->parent;
			AVLTreeNode *P = temp->parent, *Q;
			if (balance(temp) == 2)
			{
				if (balance(temp->left) == 1){
					if (P != NULL)
					{
						Q = LL(temp);
						Q->parent = P;
						if (P->left != NULL&&P->left->key == temp->key&&P->left->value == temp->value)
							P->left = Q;
						else if (P->right != NULL&&P->right->key == temp->key&&P->right->value == temp->value)
							P->right = Q;
					}
					else
						T->root = LL(temp);
				}
				else if (balance(temp->left) == -1){
					if (P != NULL)
					{
						Q = LR(temp);
						Q->parent = P;
						if (P->left != NULL&&P->left->key == temp->key&&P->left->value == temp->value)
							P->left = Q;
						else if (P->right != NULL&&P->right->key == temp->key&&P->right->value == temp->value)
							P->right = Q;
					}
					else
						T->root = LR(temp);
				}
				break;
			}
			else if (balance(temp) == -2)
			{
				if (balance(temp->right) == 1){
					if (P != NULL)
					{
						Q = RL(temp);
						Q->parent = P;
						if (P->left != NULL&&P->left->key == temp->key&&P->left->value == temp->value)
							P->left = Q;
						else if (P->right != NULL&&P->right->key == temp->key&&P->right->value == temp->value)
							P->right = Q;
					}
					else
						T->root = RL(temp);
				}
				else if (balance(temp->right) == -1){
					if (P != NULL)
					{
						Q = RR(temp);
						Q->parent = P;
						if (P->left != NULL&&P->left->key == temp->key&&P->left->value == temp->value)
							P->left = Q;
						else if (P->right != NULL&&P->right->key == temp->key&&P->right->value == temp->value)
							P->right = Q;
					}
					else
						T->root = RR(temp);
				}
				break;
			}
		}

	}
	return 1;
}

// put your time complexity for DeleteNode() here     O(log n)
int DeleteNode(AVLTree *T, int k, int v)
{
	AVLTreeNode *node = Search(T, k, v);

	if (node == NULL)
		return 0;
	else
	{

		AVLTreeNode *r = T->root;
		AVLTree *tree = newAVLTree();
		queue que;
		que.front = que.rear = 1;

		que.Q[++que.rear] = r;
		while (que.front != que.rear){
			AVLTreeNode *q;
			q = que.Q[++que.front];
			if (q->key != k&&q->value != v)
				InsertNode(tree, q->key, q->value);
			if (q->left != NULL)que.Q[++que.rear] = q->left;
			if (q->right != NULL)que.Q[++que.rear] = q->right;
		}
		T->root = tree->root;
		T->size--;
		return 1;
	}
}

// put your time complexity analysis for Search() here     O(log n)
AVLTreeNode *Search(AVLTree *T, int k, int v)
{
	AVLTreeNode *R = T->root;
	while (R)
	{
		if (k > R->key)
		{
			R = R->right;
		}
		else if (k < R->key)
		{
			R = R->left;
		}
		else
		{
			if (v>R->value)
				R = R->right;
			else if (v < R->value)
				R = R->left;
			else
				return R;
		}
	}
	return NULL;
}

// put your time complexity analysis for freeAVLTree() here   O(n)
void FreeAVLTree(AVLTree *T)
{
	FreeAVLTreeNode(T->root);
	free(T);
}

void FreeAVLTreeNode(AVLTreeNode *R)
{
	if (R != NULL)
	{

		FreeAVLTreeNode(R->left);
		FreeAVLTreeNode(R->right);

		free(R);

	}
}

// put your time complexity analysis for PrintAVLTree() here    O(n)
void PrintAVLTree(AVLTreeNode *R)
{
	if (R != NULL)
	{
		
		PrintAVLTree(R->left);
		printf("(%d,%d) %d\n", R->key, R->value, R->height);
		PrintAVLTree(R->right);
	}
}

int main() //sample main for testing 
{
	int i, j;
	AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
	AVLTreeNode *node1;

	tree1 = CreateAVLTree("stdin");
	PrintAVLTree(tree1->root);
	FreeAVLTree(tree1);
	//you need to create the text file file1.txt
	//to store a set of items without duplicate items
	
	tree2 = CreateAVLTree("File1.txt");
	PrintAVLTree(tree2->root);
	tree3 = CloneAVLTree(tree2);
	PrintAVLTree(tree3->root);
	FreeAVLTree(tree2);
	FreeAVLTree(tree3);

	//Create tree4 
	tree4 = newAVLTree();
	j = InsertNode(tree4, 10, 10);
	for (i = 0; i<15; i++)
	{
		j = InsertNode(tree4, i, i);
		if (j == 0) printf("(%d, %d) already exists\n", i, i);
	}
	PrintAVLTree(tree4->root);
	node1 = Search(tree4, 20, 20);
	if (node1 != NULL)
		printf("key= %d value= %d\n", node1->key, node1->value);
	else
		printf("Key 20 does not exist\n");

	for (i = 17; i>0; i--)
	{
		j = DeleteNode(tree4, i, i);
		if (j == 0)
			printf("Key %d does not exist\n", i);
		PrintAVLTree(tree4->root);
	}
	FreeAVLTree(tree4);

	//Create tree5
	tree5 = newAVLTree();
	j = InsertNode(tree5, 6, 25);
	j = InsertNode(tree5, 6, 10);
	j = InsertNode(tree5, 6, 12);
	j = InsertNode(tree5, 6, 20);
	j = InsertNode(tree5, 9, 25);
	j = InsertNode(tree5, 10, 25);
	PrintAVLTree(tree5->root);

	//Create tree6
	tree6 = newAVLTree();
	j = InsertNode(tree6, 6, 25);
	j = InsertNode(tree6, 5, 10);
	j = InsertNode(tree6, 6, 12);
	j = InsertNode(tree6, 6, 20);
	j = InsertNode(tree6, 8, 35);
	j = InsertNode(tree6, 10, 25);
	PrintAVLTree(tree6->root);

	tree7 = AVLTreesIntersection(tree5, tree6);
	tree8 = AVLTreesUnion(tree5, tree6);
	PrintAVLTree(tree7->root);
	PrintAVLTree(tree8->root);
	system("pause");
	return 0;
}
