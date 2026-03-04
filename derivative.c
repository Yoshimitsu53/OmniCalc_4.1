#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/* ========= OMNIPARSER ========= */
const char *p;
Node* parse_expr();

Node* parse_factor() {
    while (isspace(*p)) p++;
    Node* n = NULL;

    if (*p == '-') { 
        p++; 
        n = new_node(NODE_NEG, parse_factor(), NULL, 0); 
    }
    
    else if (isdigit(*p) || *p == '.') { 
        char* end; 
        double v = strtod(p, &end); 
        p = end; 
        n = new_node(NODE_CONST, NULL, NULL, v); 
    }
    else if (strncmp(p, "sin", 3) == 0 || strncmp(p, "cos", 3) == 0 || 
             strncmp(p, "tan", 3) == 0 || strncmp(p, "cot", 3) == 0) {
        
    }
   
    return n;
}

Node* parse_term() {
    Node* n = parse_factor();
    while (*p == '*' || *p == '/') { 
        char op = *p++; Node* r = parse_factor();
        n = new_node(op == '*' ? NODE_MUL : NODE_DIV, n, r, 0); 
    }
    return n;
}

Node* parse_expr() {
    Node* n = parse_term();
    while (*p == '+' || *p == '-') { 
        char op = *p++; Node* r = parse_term();
        n = new_node(op == '+' ? NODE_ADD : NODE_SUB, n, r, 0); 
    }
    return n;
}

/* ========= TUREV MOTORU ========= */
Node* derivative(Node* n) {
    if (!n) return NULL;
    switch (n->type) {
        case NODE_CONST: return new_node(NODE_CONST, NULL, NULL, 0);
        case NODE_X:     return new_node(NODE_CONST, NULL, NULL, 1);
        case NODE_ADD:   return new_node(NODE_ADD, derivative(n->left), derivative(n->right), 0);
        case NODE_SUB:   return new_node(NODE_SUB, derivative(n->left), derivative(n->right), 0);
        case NODE_MUL:   return new_node(NODE_ADD, new_node(NODE_MUL, derivative(n->left), n->right, 0), new_node(NODE_MUL, n->left, derivative(n->right), 0), 0);
        case NODE_DIV:   return new_node(NODE_DIV, new_node(NODE_SUB, new_node(NODE_MUL, derivative(n->left), n->right, 0), new_node(NODE_MUL, n->left, derivative(n->right), 0), 0), new_node(NODE_MUL, n->right, n->right, 0), 0);
        case NODE_POW:   return new_node(NODE_MUL, new_node(NODE_CONST, NULL, NULL, n->value), new_node(NODE_MUL, new_node(NODE_POW, n->left, NULL, n->value - 1), derivative(n->left), 0), 0);
        case NODE_SIN:   return new_node(NODE_MUL, new_node(NODE_COS, n->left, NULL, 0), derivative(n->left), 0);
        case NODE_COS:   return new_node(NODE_NEG, new_node(NODE_MUL, new_node(NODE_SIN, n->left, NULL, 0), derivative(n->left), 0), NULL, 0);
        case NODE_TAN:   return new_node(NODE_DIV, derivative(n->left), new_node(NODE_MUL, new_node(NODE_COS, n->left, NULL, 0), new_node(NODE_COS, n->left, NULL, 0), 0), 0);
        case NODE_COT:   return new_node(NODE_NEG, new_node(NODE_DIV, derivative(n->left), new_node(NODE_MUL, new_node(NODE_SIN, n->left, NULL, 0), new_node(NODE_SIN, n->left, NULL, 0), 0), 0), NULL, 0);
        case NODE_NEG:   return new_node(NODE_NEG, derivative(n->left), NULL, 0);
        default: return NULL;
    }
}

void print_math(Node* n) {
    if (!n) return;
    switch (n->type) {
        case NODE_CONST: printf("%.0f", n->value); break;
        case NODE_X:     printf("x"); break;
        case NODE_MUL:   printf("("); print_math(n->left); printf("*"); print_math(n->right); printf(")"); break;
        case NODE_POW:   
            if(n->left && n->left->type >= NODE_SIN) { print_math(n->left); printf("^%.0f", n->value); }
            else { printf("("); print_math(n->left); printf("^%.0f)", n->value); }
            break;
        case NODE_ADD:   printf("("); print_math(n->left); printf("+"); print_math(n->right); printf(")"); break;
        case NODE_SUB:   printf("("); print_math(n->left); printf("-"); print_math(n->right); printf(")"); break;
        case NODE_DIV:   printf("("); print_math(n->left); printf("/"); print_math(n->right); printf(")"); break;
        case NODE_SIN:   printf("sin("); print_math(n->left); printf(")"); break;
        case NODE_COS:   printf("cos("); print_math(n->left); printf(")"); break;
        case NODE_TAN:   printf("tan("); print_math(n->left); printf(")"); break;
        case NODE_COT:   printf("cot("); print_math(n->left); printf(")"); break;
        case NODE_NEG:   printf("-"); print_math(n->left); break;
    }
}