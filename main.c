#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct node {
	char inf[4];
	struct node *left;
	struct node *right;
}Node;

typedef Node* Tree;

typedef struct elemq {
	Node *inf;
	struct elemq *next;
}elemQ;

typedef struct {
	elemQ *first;
	elemQ *last;
}Queue;

typedef struct elem {
	char inf;
	struct elem *next;
}Elem;

typedef Elem* Stack;

typedef struct elemV {
	double inf;
	struct elemV *next;
}elemV;

typedef elemV* stackV;

typedef struct elemt {
	Node *inf;
	int visit;
	struct elemt *next;
}elemT;

typedef elemT* stackT;

int queueEmpty(Queue Q) {
	if (Q.first == NULL)
		return 1;
	return 0;
}

void insert(Queue *Q, Node *n) {
	elemQ *q = malloc(sizeof(elemQ));
	q->inf = n;
	q->next = NULL;
	if (Q->last == NULL)
		Q->first = q;
	else
		Q->last->next = q;
	Q->last = q;
}

Node* delete(Queue *Q) {
	elemQ *old;
	Node *n;
	if (Q->first == NULL)
		return NULL;
	n = Q->first->inf;
	old = Q->first;
	Q->first = Q->first->next;
	free(old);
	if (Q->first == NULL)
		Q->last = NULL;
	return n;
}

int stackEmpty(Stack s) {
	if (s == NULL)
		return 1;
	return 0;
}

void push(Stack *s, char c) {
	Elem *node = malloc(sizeof(Elem));
	node->inf = c;
	node->next = *s;
	*s = node;
}

char pop(Stack *s) {
	if (stackEmpty(*s))
		return 'x';
	Elem *t = *s;
	char x = (*s)->inf;
	*s = (*s)->next;
	free(t);
	return x;
}

char top(Stack S) {
	if (!stackEmpty(S))
		return S->inf;
	else
		return 'x';
}

int stackVEmpty(stackV s) {
	if (s == NULL)
		return 1;
	return 0;
}

void pushV(stackV *s, double v) {
	elemV *node = malloc(sizeof(elemV));
	node->inf = v;
	node->next = *s;
	*s = node;
}

double popV(stackV *s) {
	if (stackVEmpty(*s))
		return -1;
	elemV *t = *s;
	double x = (*s)->inf;
	*s = (*s)->next;
	free(t);
	return x;
}

int stackTEmpty(stackT s) {
	if (s == NULL)
		return 1;
	return 0;
}

void pushT(stackT *s, Node *n, int v) {
	elemT *node = malloc(sizeof(elemT));
	node->inf = n;
	node->visit = v;
	node->next = *s;
	*s = node;
}

Node *popT(stackT *s) {
	if (stackTEmpty(*s))
		return NULL;
	elemT *t = *s;
	Node *x = (*s)->inf;
	*s = (*s)->next;
	free(t);
	return x;
}

int topTV(stackT s) {
	if (!stackTEmpty(s))
		return s->visit;
}

int ipr(char c) {
	switch (c) {
	case '+': case '-': return 2;
	case '*': case '/': return 3;
	case '^': return 5;
	case '(': return 8;
	case ')': return 1;
	case 'l': case 's': case 'c': case '_': return 7;
	}
	return -1;
}

int spr(char c) {
	switch (c) {
	case '+': case '-': return 2;
	case '*': case '/': return 3;
	case '^': return 4;
	case '(': return 0;
	case ')': return -1;
	case 'l': case 's': case 'c': case '_': return 6;
	}
	return -1;
}

char *in2post(char *infix) {
	Stack S = NULL;
	int i = 0;
	char *next = infix, *postfix = NULL, c;
	while (*next) {
		if (*next >= 'A' && *next <= 'Z') {
			postfix = realloc(postfix, (i + 2) * sizeof(char));
			postfix[i++] = *next;
		}
		else {
			while (!stackEmpty(S) && (ipr(*next) <= spr(top(S)))) {
				c = pop(&S);
				postfix = realloc(postfix, (i + 2) * sizeof(char));
				postfix[i++] = c;
			}
			if (*next != ')') {
				if (*next == '-' && *(next - 1) == '(')
					push(&S, '_');
				else
				    push(&S, *next);
			}
			else
				c = pop(&S);
		}
		if (*next == 'l')
			next += 2;
		else if (*next == 's' || *next == 'c')
			next += 3;
		else
			next++;
	}
	while (!stackEmpty(S)) {
		c = pop(&S);
		if (c == '-' && top(S) == '(') {
			postfix = realloc(postfix, (i + 2) * sizeof(char));
			postfix[i++] = '_';
		}
		else {
			postfix = realloc(postfix, (i + 2) * sizeof(char));
			postfix[i++] = c;
		}
	}
	postfix[i] = '\0';
	return postfix;
}

Node *formNode(char *op, Node *l, Node *r) {
	Node *x = (Node*)malloc(sizeof(Node));
	char *s = op;
	int i = 0;
	while (*s) {
		x->inf[i++] = *s;
		s++;
	}
	x->inf[i] = '\0';
	x->left = l;
	x->right = r;
	return x;
}

Node *formNodeL(char *op, Node *l) {
	Node *x = (Node*)malloc(sizeof(Node));
	char *s = op;
	int i = 0;
	while (*s) {
		x->inf[i++] = *s;
		s++;
	}
	x->inf[i] = '\0';
	x->left = l;
	x->right = NULL;
	return x;
}

Node *formNodeN(char *var) {
	Node *x = (Node*)malloc(sizeof(Node));
	char *s = var;
	int i = 0;
	while (*s) {
		x->inf[i++] = *s;
		s++;
	}
	x->inf[i] = '\0';
	x->left = NULL;
	x->right = NULL;
	return x;
}

Node *copyNode(Node *n) {
	stackT S1 = NULL, S2 = NULL;
	Node *next = n, *l, *r, *x;
	int v;
	while (next != NULL) {
		pushT(&S1, next, 1);
		next = next->left;
	}
	while (!stackTEmpty(S1)) {
		v = topTV(S1);
		next = popT(&S1);
		if (v > 0) {
			pushT(&S1, next, -1);
			next = next->right;
			while (next != NULL) {
				pushT(&S1, next, 1);
				next = next->left;
			}
		}
		else {
			if ((next->inf[0] >= 'A' && next->inf[0] <= 'Z') || (next->inf[0] >= '0' && next->inf[0] <= '9')) {
				l = formNodeN(next->inf);
				pushT(&S2, l, 1);
			}
			else if (next->inf[0] == '+' || (next->inf[0] == '-' && next->right != NULL) || next->inf[0] == '*' || next->inf[0] == '/' || next->inf[0] == '^') {
				r = popT(&S2);
				l = popT(&S2);
				x = formNode(next->inf, l, r);
				pushT(&S2, x, 1);
			}
			else if (next->inf[0] == 'l' || next->inf[0] == 's' || next->inf[0] == 'c' || (next->inf[0] == '-' && next->right == NULL)) {
				l = popT(&S2);
				x = formNodeL(next->inf, l);
				pushT(&S2, x, 1);
			}
		}
	}
	return popT(&S2);
}

int equal(Node *n1, Node *n2) {
	stackT S1 = NULL, S2 = NULL;
	Node *next1, *next2;
	pushT(&S1, n1, 1);
	pushT(&S2, n2, 1);
	while (1) {
		if (stackTEmpty(S1) && !stackTEmpty(S2)) {
			while (!stackTEmpty(S2))
				popT(&S2);
			return 0;
		}
		if (!stackTEmpty(S1) && stackTEmpty(S2)) {
			while (!stackTEmpty(S1))
				popT(&S1);
			return 0;
		}
		if (stackTEmpty(S1) && stackTEmpty(S2))
			break;
		next1 = popT(&S1);
		next2 = popT(&S2);
		while (1) {
			if ((next1 == NULL && next2 != NULL) || (next1 != NULL && next2 == NULL)) {
				while (!stackTEmpty(S1))
					popT(&S1);
				while (!stackTEmpty(S2))
					popT(&S2);
				return 0;
			}
			if (next1 == NULL && next2 == NULL)
				break;
			if (strcmp(next1->inf, next2->inf)) {
				while (!stackTEmpty(S1))
					popT(&S1);
				while (!stackTEmpty(S2))
					popT(&S2);
				return 0;
			}
			if (next1->right != NULL)
				pushT(&S1, next1->right, 1);
			if (next2->right != NULL)
				pushT(&S2, next2->right, 1);
			next1 = next1->left;
			next2 = next2->left;
		}
	}
	return 1;
}

Tree formTree(char *postfix) {
	char *next = postfix, s[4];
	Node *l, *r, *x;
	stackT S = NULL;
	while (*next) {
		if (*next >= 'A' && *next <= 'Z') {
			s[0] = *next;
			s[1] = '\0';
			x = formNodeN(s);
			pushT(&S, x, 1);
		}
		else if (*next == '+' || *next == '-' || *next == '*' || *next == '/' || *next == '^') {
			r = popT(&S);
			l = popT(&S);
			s[0] = *next;
			s[1] = '\0';
			x = formNode(s, l, r);
			pushT(&S, x, 1);
		}
		else if (*next == 'l') {
			l = popT(&S);
			s[0] = 'l';
			s[1] = 'n';
			s[2] = '\0';
			x = formNodeL(s, l);
			pushT(&S, x, 1);
		}
		else if (*next == 's') {
			l = popT(&S);
			s[0] = 's';
			s[1] = 'i';
			s[2] = 'n';
			s[3] = '\0';
			x = formNodeL(s, l);
			pushT(&S, x, 1);
		}
		else if (*next == 'c') {
			l = popT(&S);
			s[0] = 'c';
			s[1] = 'o';
			s[2] = 's';
			s[3] = '\0';
			x = formNodeL(s, l);
			pushT(&S, x, 1);
		}
		else if (*next == '_') {
			l = popT(&S);
			s[0] = '-';
			s[1] = '\0';
			x = formNodeL(s, l);
			pushT(&S, x, 1);
		}
		next++;
	}
	return popT(&S);
}

void preorder(Tree root) {
	stackT S = NULL;
	Node *next;
	pushT(&S, root, 1);
	while (!stackTEmpty(S)) {
		next = popT(&S);
		while (next != NULL) {
			printf("%s", next->inf);
			if (next->right != NULL)
				pushT(&S, next->right, 1);
			next = next->left;
		}
	}
	printf("\n");
}

double value(Tree root, double *values) {
	stackT sT = NULL;
	stackV sV = NULL;
	Node *next = root;
	int v;
	double x, rez, op1, op2;
	while (next != NULL) {
		pushT(&sT, next, 1);
		next = next->left;
	}
	while (!stackTEmpty(sT)) {
		v = topTV(sT);
		next = popT(&sT);
		if (v > 0) {
			pushT(&sT, next, -1);
			next = next->right;
			while (next != NULL) {
				pushT(&sT, next, 1);
				next = next->left;
			}
		}
		else {
			if (next->inf[0] >= '0' && next->inf[0] <= '9') {
				x = (double)atoi(next->inf);
				pushV(&sV, x);
			}
			else if (next->inf[0] >= 'A' && next->inf[0] <= 'Z') {
				x = values[next->inf[0] - 'A'];
				pushV(&sV, x);
			}
			else if (next->inf[0] == '+') {
				op2 = popV(&sV);
				op1 = popV(&sV);
				rez = op1 + op2;
				pushV(&sV, rez);
			}
			else if (next->inf[0] == '-') {
				op2 = popV(&sV);
				if (next->right != NULL) {
					op1 = popV(&sV);
					rez = op1 - op2;
				}
				else {
					rez = 0 - op2;
				}
				pushV(&sV, rez);
			}
			else if (next->inf[0] == '*') {
				op2 = popV(&sV);
				op1 = popV(&sV);
				rez = op1 * op2;
				pushV(&sV, rez);
			}
			else if (next->inf[0] == '/') {
				op2 = popV(&sV);
				op1 = popV(&sV);
				rez = op1 / op2;
				pushV(&sV, rez);
			}
			else if (next->inf[0] == '^') {
				op2 = popV(&sV);
				op1 = popV(&sV);
				rez = pow(op1, op2);
				pushV(&sV, rez);
			}
			else if (next->inf[0] == 'l') {
				op1 = popV(&sV);
				rez = log(op1);
				pushV(&sV, rez);
			}
			else if (next->inf[0] == 's') {
				op1 = popV(&sV);
				rez = sin(op1);
				pushV(&sV, rez);
			}
			else if (next->inf[0] == 'c') {
				op1 = popV(&sV);
				rez = cos(op1);
				pushV(&sV, rez);
			}
		}
	}
	return popV(&sV);
}

int maxDepth(Tree root) {
	int currw = 1, max = 0, nextw;
	Node *next;
	Queue Q;
	Q.first = Q.last = NULL;
	insert(&Q, root);
	while (!queueEmpty(Q)) {
		nextw = 0;
		while (currw > 0) {
			currw--;
			next = delete(&Q);
			if (next->left != NULL) {
				insert(&Q, next->left);
				nextw++;
			}
			if (next->right != NULL) {
				insert(&Q, next->right);
				nextw++;
			}
		}
		currw = nextw;
		max++;
	}
	return max;
}

int distance(Tree root, Node *n) {
	int currw = 1, d = 0, nextw;
	Node *next;
	Queue Q;
	Q.first = Q.last = NULL;
	insert(&Q, root);
	while (!queueEmpty(Q)) {
		nextw = 0;
		while (currw > 0) {
			currw--;
			next = delete(&Q);
			if (next == n) {
				while (!queueEmpty(Q))
					delete(&Q);
				return d;
			}
			if (next->left != NULL) {
				insert(&Q, next->left);
				nextw++;
			}
			if (next->right != NULL) {
				insert(&Q, next->right);
				nextw++;
			}
		}
		currw = nextw;
		d++;
	}
	return -1;
}

void treePrint(Tree root) {
	stackT S = NULL;
	Node *next;
	int max = maxDepth(root), d, l, i;
	pushT(&S, root, 1);
	while (!stackTEmpty(S)) {
		next = popT(&S);
		while (next != NULL) {
			d = distance(root, next);
			l = max - distance(root, next);
			for (i = 0; i < d; i++) {
				printf(" ");
			}
			printf("%s", next->inf);
			for (i = 0; i < 4 - strlen(next->inf); i++) {
				printf(" ");
			}
			for (i = 0; i < l; i++) {
				printf("-");
			}
			printf("\n");
			if (next->right != NULL)
				pushT(&S, next->right, 1);
			next = next->left;
		}
	}
}


Tree derrivate(Tree root, char x) {
	Tree T = NULL;
	stackT S1 = NULL, S2 = NULL;
	Node *next = root, *n, *l = NULL, *r, *x1, *x2, *x3, *x4, *x5, *cl, *cr;
	int v;
	char s[4];
	while (next != NULL) {
		pushT(&S1, next, 1);
		next = next->left;
	}
	while (!stackTEmpty(S1)) {
		v = topTV(S1);
		next = popT(&S1);
		if (v > 0) {
			pushT(&S1, next, -1);
			next = next->right;
			while (next != NULL) {
				pushT(&S1, next, 1);
				next = next->left;
			}
		}
		else {
			if (next->inf[0] >= '0' && next->inf[0] <= '9') {
				s[0] = '0';
				s[1] = '\0';
				n = formNodeN(s);
				pushT(&S2, n, 1);
			}
			else if (next->inf[0] >= 'A' && next->inf[0] <= 'Z') {
				if (next->inf[0] == x) {
					s[0] = '1';
					s[1] = '\0';
					n = formNodeN(s);
					pushT(&S2, n, 1);
				}
				else {
					s[0] = '0';
					s[1] = '\0';
					n = formNodeN(s);
					pushT(&S2, n, 1);
				}
			}
			else if (next->inf[0] == '+') {
				r = popT(&S2);
				l = popT(&S2);
				if (l->inf[0] != '0' && r->inf[0] != '0') {
					if (l->inf[0] == '1' && r->inf[0] == '1') {
						s[0] = '2';
						s[1] = '\0';
						n = formNodeN(s);
						pushT(&S2, n, 1);
					}
					else {
						s[0] = '+';
						s[1] = '\0';
						n = formNode(s, l, r);
						pushT(&S2, n, 1);
					}
				}
				else if (l->inf[0] != '0' && r->inf[0] == '0') {
					pushT(&S2, l, 1);
				}
				else if (l->inf[0] == '0' && r->inf[0] != '0') {
					pushT(&S2, r, 1);
				}
				else {
					s[0] = '0';
					s[1] = '\0';
					n = formNodeN(s);
					pushT(&S2, n, 1);
				}
			}
			else if (next->inf[0] == '-' && next->right != NULL) {
				r = popT(&S2);
				l = popT(&S2);
				if (l->inf[0] != '0' && r->inf[0] != '0') {
					if (equal(l, r)) {
						s[0] = '0';
						s[1] = '\0';
						n = formNodeN(s);
						pushT(&S2, n, 1);
					}
					else {
						s[0] = '-';
						s[1] = '\0';
						n = formNode(s, l, r);
						pushT(&S2, n, 1);
					}
				}
				else if (l->inf[0] != '0' && r->inf[0] == '0') {
					pushT(&S2, l, 1);
				}
				else if (l->inf[0] == '0' && r->inf[0] != '0') {
					s[0] = '-';
					n = formNodeL(s, r);
					pushT(&S2, n, 1);
				}
				else {
					s[0] = '0';
					s[1] = '\0';
					n = formNodeN(s);
					pushT(&S2, n, 1);
				}
			}
			else if (next->inf[0] == '*') {
				x2 = popT(&S2);
				x1 = popT(&S2);
				cl = copyNode(next->left);
				cr = copyNode(next->right);
				if (x1->inf[0] == '0' && x2->inf[0] == '0') {
					s[0] = '0';
					s[1] = '\0';
					n = formNodeN(s);
					pushT(&S2, n, 1);
				}
				else if (x1->inf[0] != '0' && x2->inf[0] == '0') {
					if (x1->inf[0] == '1')
						pushT(&S2, cr, 1);
					else {
						s[0] = '*';
						s[1] = '\0';
						n = formNode(s, x1, cr);
						pushT(&S2, n, 1);
					}
				}
				else if (x1->inf[0] == '0' && x2->inf[0] != '0') {
					if (x2->inf[0] == '1')
						pushT(&S2, cl, 1);
					else {
						s[0] = '*';
						s[1] = '\0';
						n = formNode(s, cl, x2);
						pushT(&S2, n, 1);
					}
				}
				else {
					if (x1->inf[0] == '1' && x2->inf[0] == '1') {
						s[0] = '+';
						s[1] = '\0';
						n = formNode(s, cr, cl);
						pushT(&S2, n, 1);
					}
					else if (x1->inf[0] != '1' && x2->inf[0] == '1') {
						s[0] = '*';
						s[1] = '\0';
						l = formNode(s, x1, cr);
						s[0] = '+';
						n = formNode(s, l, cl);
						pushT(&S2, n, 1);
					}
					else if (x1->inf[0] == '1' && x2->inf[0] != '1') {
						s[0] = '*';
						s[1] = '\0';
						r = formNode(s, cl, x2);
						s[0] = '+';
						n = formNode(s, cr, r);
						pushT(&S2, n, 1);
					}
					else {
						s[0] = '*';
						s[1] = '\0';
						l = formNode(s, x1, cr);
						r = formNode(s, cl, x2);
						s[0] = '+';
						n = formNode(s, l, r);
						pushT(&S2, n, 1);
					}
				}
			}
			else if (next->inf[0] == '/') {
				x2 = popT(&S2);
				x1 = popT(&S2);
				cl = copyNode(next->left);
				cr = copyNode(next->right);
				if (x1->inf[0] == '0' && x2->inf[0] == '0') {
					s[0] = '0';
					s[1] = '\0';
					l = formNodeN(s);
				}
				else if (x1->inf[0] != '0' && x2->inf[0] == '0') {
					if (x1->inf[0] == '1')
						l = cr;
					else {
						s[0] = '*';
						s[1] = '\0';
						l = formNode(s, x1, cr);
					}
				}
				else if (x1->inf[0] == '0' && x2->inf[0] != '0') {
					if (x2->inf[0] == '1') {
						s[0] = '-';
						s[1] = '\0';
						l = formNodeL(s, cl);
					}
					else {
						s[0] = '*';
						s[1] = '\0';
						x3 = formNode(s, cl, x2);
						s[0] = '-';
						l = formNodeL(s, x3);
					}
				}
				else {
					if (x1->inf[0] == '1' && x2->inf[0] == '1') {
						if (equal(cl, cr)) {
							s[0] = '0';
							s[1] = '\0';
							l = formNodeN(s);
						}
						else {
							s[0] = '-';
							s[1] = '\0';
							l = formNode(s, cr, cl);
						}
					}
					else if (x1->inf[0] != '1' && x2->inf[0] == '1') {
						s[0] = '*';
						s[1] = '\0';
						x3 = formNode(s, x1, cr);
						if (equal(cl, x3)) {
							s[0] = '0';
							s[1] = '\0';
							l = formNodeN(s);
						}
						else {
						s[0] = '-';
						l = formNode(s, x3, cl);
					    }
					}
					else if (x1->inf[0] == '1' && x2->inf[0] != '1') {
						s[0] = '*';
						s[1] = '\0';
						x3 = formNode(s, cl, x2);
						if (equal(x3, cr)) {
							s[0] = '0';
							s[1] = '\0';
							l = formNodeN(s);
						}
						else {
							s[0] = '-';
							l = formNode(s, cr, x3);
						}
					}
					else {
						s[0] = '*';
						s[1] = '\0';
						x3 = formNode(s, x1, cr);
						x4 = formNode(s, cl, x2);
						if (equal(x3, x4)) {
							s[0] = '0';
							s[1] = '\0';
							l = formNodeN(s);
						}
						else {
							s[0] = '-';
							l = formNode(s, x3, x4);
						}
					}
				}
				s[0] = '*';
				cl = copyNode(cr);
				cr = copyNode(cr);
				r = formNode(s, cl, cr);
				if (l->inf[0]=='0') {
					s[0] = '0';
					s[1] = '\0';
					n = formNodeN(s);
					pushT(&S2, n, 1);
				}
				else {
					s[0] = '/';
					n = formNode(s, l, r);
					pushT(&S2, n, 1);
				}
			}
			else if (next->inf[0] == '^') {
				x2 = popT(&S2);
				x1 = popT(&S2);
				cl = copyNode(next->left);
				cr = copyNode(next->right);
				if (x1->inf[0] != '0' && x2->inf[0] == '0') {
					s[0] = '1';
					s[1] = '\0';
					x5 = formNodeN(s);
					s[0] = '-';
					x4 = formNode(s, cr, x5);
					s[0] = '^';
					x3 = formNode(s, cl, x4);
					s[0] = '*';
					l = formNode(s, copyNode(cr), x3);
					if (x1->inf[0] == '1')
						pushT(&S2, l, 1);
					else {
						n = formNode(s, l, x1);
						pushT(&S2, n, 1);
					}
				}
				else if (x1->inf[0] == '0' && x2->inf[0] != '0') {
					s[0] = 'l';
					s[1] = 'n';
					s[2] = '\0';
					x3 = formNodeL(s, cl);
					s[0] = '*';
					s[1] = '\0';
					x4 = copyNode(next);
					l = formNode(s, x4, x3);
					if (x2->inf[0] == '1')
						pushT(&S2, l, 1);
					else {
						n = formNode(s, l, x2);
						pushT(&S2, n, 1);
					}
				}
				else if (x1->inf[0] == '0' && x2->inf[0] == '0') {
					s[0] = '0';
					s[1] = '\0';
					n = formNodeN(s);
					pushT(&S2, n, 1);
				}
			}
			else if (next->inf[0] == 'l') {
			    x1 = popT(&S2);
				cl = copyNode(next->left);
				if (x1->inf[0] == '0') {
					s[0] = '0';
					s[1] = '\0';
					n = formNodeN(s);
					pushT(&S2, n, 1);
				}
				else if (x1->inf[0] == '1') {
					s[0] = '1';
					s[1] = '\0';
					l = formNodeN(s);
					s[0] = '/';
					n = formNode(s, l, cl);
					pushT(&S2, n, 1);
				}
				else {
					s[0] = '/';
					s[1] = '\0';
					n = formNode(s, x1, cl);
					pushT(&S2, n, 1);
				}
			}
			else if (next->inf[0] == 's') {
			    x1 = popT(&S2);
				cl = copyNode(next->left);
			    if (x1->inf[0] == '0') {
				    s[0] = '0';
				    s[1] = '\0';
				    n = formNodeN(s);
				    pushT(&S2, n, 1);
			    }
			    else if (x1->inf[0] == '1') {
				    s[0] = 'c';
				    s[1] = 'o';
				    s[2] = 's';
				    s[3] = '\0';
				    n = formNodeL(s, cl);
				    pushT(&S2, n, 1);
			    }
			    else {
				    s[0] = 'c';
				    s[1] = 'o';
				    s[2] = 's';
				    s[3] = '\0';
				    l = formNodeL(s, cl);
				    s[0] = '*';
				    s[1] = '\0';
				    n = formNode(s, l, x1);
				    pushT(&S2, n, 1);
			    }
			}
			else if (next->inf[0] == 'c') {
				x1 = popT(&S2);
				cl = copyNode(next->left);
				if (x1->inf[0] == '0') {
					s[0] = '0';
					s[1] = '\0';
					n = formNodeN(s);
					pushT(&S2, n, 1);
				}
				else if (x1->inf[0] == '1') {
					s[0] = 's';
					s[1] = 'i';
					s[2] = 'n';
					s[3] = '\0';
					l = formNodeL(s, cl);
					s[0] = '-';
					s[1] = '\0';
					n = formNodeL(s, l);
					pushT(&S2, n, 1);
				}
				else {
					s[0] = 's';
					s[1] = 'i';
					s[2] = 'n';
					s[3] = '\0';
					x2 = formNodeL(s, cl);
					s[0] = '*';
					s[1] = '\0';
					l = formNode(s, x2, x1);
					s[0] = '-';
					n = formNodeL(s, l);
					pushT(&S2, n, 1);
				}
			}
			else if (next->inf[0] == '-' && next->right == NULL) {
			    x1 = popT(&S2);
				if (x1->inf[0] == '0') {
					s[0] = '0';
					s[1] = '\0';
					n = formNodeN(s);
					pushT(&S2, n, 1);
				}
				else if (x1->inf[0] == '1') {
					s[0] = '1';
					s[1] = '\0';
					l = formNodeN(s);
					s[0] = '-';
					n = formNodeL(s, l);
					pushT(&S2, n, 1);
				}
				else {
					s[0] = '-';
					s[1] = '\0';
					n = formNodeL(s, x1);
					pushT(&S2, n, 1);
				}
			}
		}
	}
	return popT(&S2);
}

void treeDelete(Tree *root) {
	Node *next = *root;
	stackT S = NULL;
	int v;
	while (next != NULL) {
		pushT(&S, next, 1);
		next = next->left;
	}
	while (!stackTEmpty(S)) {
		v = topTV(S);
		next = popT(&S);
		if (v > 0) {
			pushT(&S, next, -1);
			next = next->right;
			while (next != NULL) {
				pushT(&S, next, 1);
				next = next->left;
			}
		}
		else {
			free(next);
		}
	}
}

int main()
{
	Tree root = NULL, derr = NULL, oderr = NULL;
	char *infix = NULL, *ninfix, *postfix = NULL, c, *s;
	int i = 0, in[26], input, end = 1, n, k;
	double values[26], v;
	for (i = 0; i < 26; i++) {
		in[i] = 0;
	}
	i = 0;
	while (end) {
		printf("\n1. Unos izraza\n"
			"2. Formiranje binarnog stabla\n"
			"3. Unos vrednosti promenljivih\n"
			"4. Promena vrednosti promenljive\n"
			"5. Ispis binarnog stabla\n"
			"6. Ispis unetog izraza u prefiksnoj notaciji\n"
			"7. Izracunavanje izraza za zadate vrednosti promenljivih\n"
			"8. Izvod izraza\n"
			"9. Ispis trazenog izvoda u prefiksnoj notaciji\n"
			"10. Vrednost trazenog izvoda\n"
			"11. Brisanje stabla\n"
			"0. Zavrsetak rada\n\n"
			"Vas izbor? ");
		scanf("%d", &input);
		switch (input) {
		case 1: getchar();
			printf("Unesite izraz: ");
			while ((c = getchar()) != '\n') {
				if (i % 15 == 0) {
					ninfix = realloc(infix, (i + 15) * sizeof(char));
					if (ninfix == NULL) {
						printf("Losa alokacija!!!");
						exit(1);
					}
					else
						infix = ninfix;
				}
				infix[i++] = c;
			}
			ninfix = realloc(infix, (i + 1) * sizeof(char));
			if (ninfix == NULL) {
				printf("Losa alokacija!!!");
				i--;
			}
			else
				infix[i] = '\0';
			break;
		case 2: if (infix == NULL) {
			printf("Nema izraza!!!\n");
			break;
		    }
			postfix = in2post(infix);
			root = formTree(postfix);
			break;
		case 3: if (root == NULL) {
			printf("Nema stabla!!!\n");
			break;
		    }
			printf("Unesite vrednosti promenljivih:\n");
			s = postfix;
			while (*s) {
				if (*s >= 'A' && *s <= 'Z') {
					if (in[*s - 'A'] == 0) {
						printf("%c = ", *s);
						scanf("%lf", &values[*s - 'A']);
						in[*s - 'A'] = 1;
					}
				}
				s++;
			}
			break;
		case 4: getchar();
			if (root == NULL) {
			    printf("Nema stabla!!!\n");
			    break;
		    }
			printf("Unesite promenljivu i novu vrednost: ");
			scanf("%c %lf", &c, &v);
			if (in[c - 'A'] == 0) {
				printf("Nepostojeca promenljiva!!!\n");
			}
			else {
				values[c - 'A'] = v;
				in[c - 'A'] = 1;
			}
			break;
		case 5: if (root == NULL) {
			printf("Nema stabla!!!\n");
			break;
		    }
			printf("Izgled binarnog stabla izraza:\n");
			treePrint(root);
			break;
		case 6: if (root == NULL) {
			printf("Nema stabla!!!\n");
			break;
		    }   
			printf("Prefiksna notacija izraza: ");
			preorder(root);
			break;
		case 7: if (root == NULL) {
			printf("Nema stabla!!!\n");
			break;
		    }
			k = 0;
			for (i = 0; i < 26; i++) {
				if (in[i] == 1)
					k = 1;
			}
			if (k == 0) {
				printf("Nisu unete vrednosti!!!\n");
				break;
			}
			printf("Vrednost izraza: %lf\n", value(root, values));
			break;
		case 8: getchar();
			if (root == NULL) {
				printf("Nema stabla!!!\n");
				break;
			}
			printf("Unesite red izvoda i promenljivu: ");
			scanf("%d %c", &n, &c);
			if (n < 1) {
				printf("Los unos!!!\n");
				break;
			}
			if (derr != NULL) {
				treeDelete(&derr);
				derr = NULL;
			}
			derr = derrivate(root, c);
			for (i = 0; i < n - 1; i++) {
				oderr = derr;
				derr = derrivate(oderr, c);
				treeDelete(&oderr);
				oderr = NULL;
			}
			printf("Izgled binarnog stabla izvoda:\n");
			treePrint(derr);
			break;
		case 9: if (derr == NULL) {
			printf("Nema izvoda!!!\n");
			break;
		    }
			printf("Prefiksna notacija izvoda: ");
			preorder(derr);
			break;
		case 10: if (derr == NULL) {
			printf("Nema izvoda!!!\n");
			break;
		    }
			k = 0;
			for (i = 0; i < 26; i++) {
			    if (in[i] == 1)
				    k = 1;
			}
			if (k == 0) {
			    printf("Nisu unete vrednosti!!!\n");
			    break;
			}
			printf("Vrednost trazenog izvoda: %lf\n", value(derr, values));
			break;
		case 11: if (infix == NULL) {
				printf("Nema izraza!!!\n");
				break;
			}
			free(infix);
			infix = NULL;
			if (postfix == NULL) {
				printf("Nema izraza!!!\n");
				break;
			}
			free(postfix);
			postfix = NULL;
			for (i = 0; i < 26; i++) {
				in[i] = 0;
			}
			i = 0;
			if (root == NULL) {
				printf("Nema stabla!!!\n");
				break;
			}
			treeDelete(&root);
			root = NULL;
			if (derr == NULL) {
				printf("Nema izvoda!!!\n");
				break;
			}
			treeDelete(&derr);
			derr = NULL;
			break;
		case 0: printf("Kraj.\n");
			end = 0;
			break;
		default: printf("Nepostojeci izbor!!!\n");
			break;
		}
	}
	return 0;
}