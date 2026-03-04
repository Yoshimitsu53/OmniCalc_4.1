/* ========= MAIN INTERFACE ========= */
int main() {
    int tus, adet, l1, l2; char devam, input[256]; double sayi, sonuc, x, y;
    
    
    printf("  ____  __  __ _   _ ___  ____    _    _     ____ \n");
    printf(" / __ \\|  \\/  | \\ | |_ _|/ ___|  / \\  | |   / ___|\n");
    printf("| |  | | |\\/| |  \\| || || |     / _ \\ | |  | |    \n");
    printf("| |__| | |  | | |\\  || || |___ / ___ \\| |__| |___ \n");
    printf(" \\____/|_|  |_|_| \\_|___|\\____/_/   \\_\\_____\\____|\n");
    printf(" >> OmniCalc v4.1 | Engineering Hybrid Engine <<\n\n");
    
    printf("----- KULLANIM KLAVUZU -----\n");
    printf("-> Kare/Us: ^ (Orn: x^2)\n");
    printf("-> Trigonometri: sin^2(3x)\n");
    printf("-> Carpma: * (Veya bitisik: 3x)\n");
    printf("-> Bolme: /\n");
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