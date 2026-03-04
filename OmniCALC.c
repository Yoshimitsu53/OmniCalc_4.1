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

/* ========= OMNIPARSER  ========= */
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
        NodeType type = (strncmp(p, "sin", 3) == 0) ? NODE_SIN : 
                        (strncmp(p, "cos", 3) == 0) ? NODE_COS : 
                        (strncmp(p, "tan", 3) == 0) ? NODE_TAN : NODE_COT;
        p += 3;
        
        
        double expo = 1.0;
        if (*p == '^') {
            p++; char* end; expo = strtod(p, &end); p = end;
        }

        if (*p == '(') p++;
        Node* inner = parse_expr();
        if (*p == ')') p++;
        
        n = new_node(type, inner, NULL, 0);
        if (expo != 1.0) n = new_node(NODE_POW, n, NULL, expo);
    }
    
    else if (*p == 'x') { 
        p++; 
        n = new_node(NODE_X, NULL, NULL, 0); 
    }
   
    else if (*p == '(') { 
        p++; 
        n = parse_expr(); 
        if (*p == ')') p++; 
    }

    
    while (*p == '^') {
        p++; char* end; double exp = strtod(p, &end); p = end;
        n = new_node(NODE_POW, n, NULL, exp);
    }

    while (isspace(*p)) p++;
    // Implicit Multiplication 
    if (n && (*p == 'x' || isalpha(*p) || *p == '(')) {
        n = new_node(NODE_MUL, n, parse_factor(), 0);
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

/* ========= DERIVATIVE ENGINE ========= */
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
        case NODE_CONST: printf("%.2f", n->value); break;
        case NODE_X:     printf("x"); break;
        case NODE_MUL:   printf("("); print_math(n->left); printf("*"); print_math(n->right); printf(")"); break;
        case NODE_POW:   
            print_math(n->left); printf("^%.0f", n->value);
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

/* ========= MAIN INTERFACE ========= */
int main() {
    int tus, adet, l1, l2; char devam, input[256]; double sayi, sonuc, x, y;
    
    printf("  ____  __  __ _   _ ___  ____    _    _     ____ \n");
    printf(" / __ \\|  \\/  | \\ | |_ _|/ ___|  / \\  | |   / ___|\n");
    printf("| |  | | |\\/| |  \\| || || |     / _ \\ | |  | |    \n");
    printf("| |__| | |  | | |\\  || || |___ / ___ \\| |__| |___ \n");
    printf(" \\____/|_|  |_|_| \\_|___|\\____/_/   \\_\\_____\\____|\n");
    printf(" >> OmniCalc v4.2 | Engineering Hybrid Engine <<\n\n");
    
    printf("----- KULLANIM KLAVUZU -----\n");
    printf("-> Kare/Us: ^ (Orn: x^2)\n");
    printf("-> Trigonometri: sin^2(3x)\n");
    printf("-> Carpma: * (Veya bitisik: 3x)\n");
    printf("-> Bolme: /\n");
    printf("-> Kesirli islem: . \n");
    printf("----------------------------\n");

    do {
        printf("\n==========================================================\n");
        printf("%-15s %-15s %-15s %-15s\n", "1: TOPLAMA", "2: CIKARMA", "3: CARPMA", "4: BOLME");
        printf("%-15s %-15s %-15s %-15s\n", "5: US ALMA", "6: MOD ALMA", "7: SINUS", "8: COSINUS");
        printf("%-15s %-15s %-15s %-15s\n", "9: TANJANT", "10: COTANJANT", "11: TUREV MODU", "0: CIKIS");
        printf("==========================================================\nSECIM: ");
        if (scanf("%d", &tus) != 1) { while (getchar() != '\n'); continue; }
        if (tus == 0) break;
        
        switch (tus) {
            case 1: case 2: case 3: case 4:
                printf("KAC SAYI ILE ISLEM YAPILACAK: "); scanf("%d", &adet);
                if (tus == 1) sonuc = 0; else { printf("1. SAYI: "); scanf("%lf", &sonuc); adet--; }
                for(int i=0; i<adet; i++) {
                    printf("%s SAYI: ", (tus==1?"":"SONRAKI")); scanf("%lf", &sayi);
                    if (tus==1) sonuc += sayi; else if (tus==2) sonuc -= sayi;
                    else if (tus==3) sonuc *= sayi; else if (tus==4 && sayi!=0) sonuc /= sayi;
                }
                printf("\n>>> SONUC: %f\n", sonuc); break;
            case 5: printf("Taban ve Us: "); scanf("%lf %lf", &x, &y); printf(">>> SONUC: %f\n", pow(x, y)); break;
            case 6: printf("Sayi ve Mod: "); scanf("%d %d", &l1, &l2); printf(">>> SONUC: %d\n", l1 % l2); break;
            case 7: case 8: case 9: case 10:
                printf("Derece: "); scanf("%lf", &x); x = x * M_PI / 180;
                if(tus==7) printf(">>> SONUC: %f\n", sin(x)); else if(tus==8) printf(">>> SONUC: %f\n", cos(x));
                else if(tus==9) printf(">>> SONUC: %f\n", tan(x)); else printf(">>> SONUC: %f\n", 1.0/tan(x)); break;
            case 11:
                printf("DENKLEM (orn: tan^3(2x)*sin(x)): ");
                while (getchar() != '\n'); fgets(input, 256, stdin);
                input[strcspn(input, "\n")] = 0; p = input;
                Node* ast = parse_expr();
                if (ast) {
                    Node* d = derivative(ast);
                    for(int i=0; i<5; i++) d = simplify(d);
                    printf(">>> TUREV SONUCU: "); print_math(d); printf("\n");
                } break;
        }
        printf("\nDevam etmek istiyor musunuz? (e/h): "); scanf(" %c", &devam);
    } while (devam == 'e' || devam == 'E');
    
    return 0;
}
