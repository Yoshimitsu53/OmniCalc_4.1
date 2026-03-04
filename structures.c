#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ========= OMNICALC CORE STRUCTURES ========= */
typedef enum {
    NODE_CONST, NODE_X, NODE_ADD, NODE_SUB,
    NODE_MUL, NODE_DIV, NODE_NEG, NODE_POW,
    NODE_SIN, NODE_COS, NODE_TAN, NODE_COT
} NodeType;

typedef struct Node {
    NodeType type;
    double value;
    struct Node *left, *right;
} Node;

Node* new_node(NodeType t, Node* l, Node* r, double v) {
    Node* n = (Node*)malloc(sizeof(Node));
    if (!n) return NULL;
    n->type = t; n->left = l; n->right = r; n->value = v;
    return n;
}

/* ========= SİMPLİFYİNG ENGİNE ========= */
Node* simplify(Node* n) {
    if (!n) return NULL;
    if (n->left) n->left = simplify(n->left);
    if (n->right) n->right = simplify(n->right);

    if (n->type == NODE_MUL && n->left && n->right) {
        if ((n->left->type == NODE_CONST && n->left->value == 0) || (n->right->type == NODE_CONST && n->right->value == 0)) 
            return new_node(NODE_CONST, NULL, NULL, 0);
        if (n->left->type == NODE_CONST && n->left->value == 1) return n->right;
        if (n->right->type == NODE_CONST && n->right->value == 1) return n->left;
    }
    if (n->type == NODE_ADD && n->left && n->right) {
        if (n->left->type == NODE_CONST && n->left->value == 0) return n->right;
        if (n->right->type == NODE_CONST && n->right->value == 0) return n->left;
    }
    return n;
}
