#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int prime(int n, int n_comp) {
    
    if (n <= 1) return 1;
    if (n_comp == 2) return 1;
    if (n%(n_comp-1) != 0) return prime(n, n_comp-1);
    return 0;
}

int mdc(int a, int b){
    if (b == 0) return a;
    return mdc(b, a % b);
}

int inverse (int e_decrypt, int phy_decrypt)
{
    int s = 1, t = 0, c = 0, d = 1, e, f;

    int euclides_a, euclides_b, temp_b;

    // Organizando pra q o a seja sempre maior q b (euclides_a > euclides_b).
    euclides_a = (e_decrypt > phy_decrypt)? e_decrypt : phy_decrypt;
    euclides_b = (e_decrypt < phy_decrypt)? e_decrypt : phy_decrypt;
    //

    // Encontra o mdc através do euclides e pega o s e t, no final o s (ou o t) é o inverso do e_decrypt.
    if (euclides_a % euclides_b != 0){
        do
        {
            if((euclides_a % euclides_b) != 0)
            {
                e = s;
                f = t;
                s = s * (euclides_a / euclides_b);
                t = t * (euclides_a / euclides_b);
                s = s + c;
                t = t + d;
                c = e;
                d = f;
           }
            if(euclides_a % euclides_b != 0)
            {
                temp_b = euclides_b;
                euclides_b = (euclides_a % euclides_b);
                euclides_a = temp_b;
            }

        } while (euclides_a % euclides_b > 0);

    }
    else
    {
        t = 0;
        s = 1;
    }

    // dá pro s e t os sinais certos

    if (e_decrypt > phy_decrypt)
    {
        if ((e_decrypt * t + phy_decrypt * -s) == euclides_b)s = -s;
        else t = -t;

        if( t < 0 ) t += phy_decrypt;

        if (t > phy_decrypt) t = t % phy_decrypt;

        return t;
    }

    else
    {
        if ((e_decrypt * -s + phy_decrypt * t) == euclides_b) s = -s;
        else t = -t;

        if( s < 0 ) s += phy_decrypt;

        if (s > phy_decrypt) s = s % phy_decrypt;

        return s;
    }
}

int power(long long x, unsigned int y, int p)
{
    int res = 1;     // Initialize result
 
    x = x % p; // Update x if it is more than or
                // equal to p
  
    if (x == 0) return 0; // In case x is divisible by p;
 
    while (y > 0)
    {
        // If y is odd, multiply x with result
        if (y & 1)
            res = (res*x) % p;
 
        // y must be even now
        y = y>>1; // y = y/2
        x = (x*x) % p;
    }
    return res;
}

void encoding(char name_function[],int map_encoded[], int encoded[], unsigned int i) {

    if (i == strlen(name_function) - 1) return;

    encoded[i] = map_encoded[name_function[i]];
    
    encoding(name_function, map_encoded, encoded, i + 1);
    }

void encrypt() {
    
    int map[256], encoded_out[280], key_n, key_e; 
    char message[280];
    FILE *encrypted;

   // [ ! ] STRING TO NUMBER

    system("cls");
    printf("\n< ------ ENCRYPT ----- >\n\nType your message. Maximum of 280 characters (spaces count).\n>>>  ");
    fgets(message, sizeof(message), stdin);

    for(int i = 'A'; i <= 'Z'; i++) map[i] = i - 'A' + 2;    
    map[' '] = 28;
    for(int i = 'a'; i <= 'z'; i++) map[i] = i - 'a' + 2;

    encoding(message, map, encoded_out, 0);

    // [ ! ] NUMBER TO NEW NUMBER

    printf("Insert your N key and E key respecively.\n>>>   ");
    scanf("%i %i", &key_n, &key_e);
    getchar();
    
    int x = 0;
    encrypted = fopen("ENCRYPTED.txt", "w+");

    while(x+1 < strlen(message)){ 

        encoded_out[x] = (unsigned long long) power(encoded_out[x], key_e, key_n);

        fprintf(encrypted, " %i", encoded_out[x]);  //PRINT TO FILE 
        x++;
	}

    fclose(encrypted);

    printf("[ ! ] Ecryption sucessful.\n");

    interstitial();
    
    EXIT_SUCCESS;}

void decrypt() {
    
    int p, q, e, x = 0;
    char decoded[280];
    int message_in[280], message_out[280];
    FILE *decrypted;
    
    // [ ! ] INPUTS

    system("cls");

    printf("\n< ------ DECRYPT ----- >\n\nInform your P, Q and E respectively.\n>>>  ");
    scanf("%i %i %i", &p, &q, &e);
    getchar();

    printf("Inform your encrypted message\n>>>  ");

    int array_size = 1;
    while (x < 280) {

        char c;

        scanf("%d%c", &message_in[x], &c);
        if (c == '\n') break;

        x++;
        array_size++;

    }

    x = 0;
    
    while(x < array_size){ 

        printf("%i ", message_in[x]);
        x++;
	}

    int phi = ((p-1)*(q-1));
    double d = inverse(e, phi);

    x = 0;

    // [ ! ] CONVERSIONS

	while(x < array_size){  // ARRAY > [ FORMULA ] > ORIGINAL NUMBERS

            int y = message_in[x];
            int N = p*q;
            message_out[x] = power(message_in[x], d, N);

        	x++;
	}

    // [ ! ] NUMBER TO STRING

    x = 0;
    while(x < array_size){ 

        if(message_out[x] != 28)  decoded[x] = message_out[x] + ('A' - 2);
        else if(message_out[x] == 28) decoded[x] = ' ';
		x++;

	}
    
    // [ ! ] DOCUMENT GENERATION BELOW 

    x = 0;

    decrypted = fopen("DECRYPTED.txt", "w+");

    while(x < array_size){ 

        fprintf(decrypted, "%c", decoded[x]);  //PRINT TO FILE 
        x++;

	}

    fclose(decrypted);

    printf("\n[ ! ] Decryption sucessful.\n");

    interstitial();

    return;}

void generate_key(){

    int p, q, e_key, n_key;
    FILE *key;

    // [ ! ] MENU E VALIDATION
    
    system("cls");
    printf("\n< ------ KEYGEN ----- >\n\nInput two prime numbers separated by a space.\n>>>  ");
    scanf("%i %i", &p, &q);
	getchar();

    while (prime(p, p) == 0 && prime(q, q) == 0) {

        printf("[ ! ] Something went wrong. Check if both inputs are prime numbers and try again.\n");
        printf("Input two prime numbers separated by a space.\n>>>  ");
        scanf("%i %i", &p, &q);

    }

    printf("Input your exponent. It needs to be coprime of %i.\n>>>  ", (p-1)*(q-1));
    scanf("%i", &e_key);

    int n = (p-1)*(q-1);
    
    while (mdc(n, e_key) != 1){

        printf("[ ! ] Something went wrong. Check if input is coprime to %i and try again.\n", (p-1)*(q-1));
        printf("Input your exponent. It needs to be coprime of %i,\n>>>  ", p-1*q-1);
        scanf("%i", &e_key);
		getchar();

	}

    // [ ! ] KEY GENERATION AND REGISTERING STARTS HERE
    
    n_key = p*q;
    key = fopen("PUBLIC_KEY.txt", "w+");
    fprintf(key, "< ------ RSA KEYS ----- >\nN KEY: %i\nE KEY: %i", n_key, e_key);
    fclose(key);
    printf("[ ! ] Key registered sucessfully. ;)\n");

    interstitial();

    EXIT_SUCCESS;
}

void welcome()
{
    printf("8888888b.   .d8888b.        d8888 \n888   Y88b d88P  Y88b      d88888\n888    888 Y88b.          d88P888\n");
    printf("888   d88P  'Y888b.      d88P 888\n8888888P'      'Y88b.   d88P  888\n888 T88b         '888  d88P   888\n" );
    printf("888  T88b  Y88b  d88P d8888888888\n888   T88b  'Y8888P' d88P     888 \n\n" );
    printf("      [ Gustavo & Isadora ]\n  >> Press ENTER to continue. <<\n");


    getchar();
    system("cls");
}

void menu()
{
    int x;
    
    while (x != 1 && x != 2 && x != 3) {

        printf("\n< ------ RSA MENU ----- >\n\nPick an option below.\n\n1 - Generate public key\n2 - Encrypt\n3 - Decrypt\n\nInput the number referring to your option.\n\n>>>  ");
        scanf("%i", &x);
        getchar();

        if (x == 1) generate_key();
        else if (x == 2) encrypt();
        else if (x == 3) decrypt();
        else {
            printf("\nERROR [ ! ] Your input does not match any of the options. Press ENTER to try again.\n\n");
            getchar();
            system("cls");
        }
    }
}

void interstitial()
{
    printf("Go back to menu? [ Y / N ]\n");
    char c;
    c = getchar();
    if(c == 'Y' || c == 'y')
    {
        system("cls");
        menu();
    }
    EXIT_SUCCESS;
}

int main()
{
    welcome();
    menu();

    EXIT_SUCCESS;
}