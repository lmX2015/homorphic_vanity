#include <iostream>
#include <gmp.h>
#define LAMBA 8
#define RHO 8
#define ETHA 65
#define GAMMA 256
#define TAU 308
#include "keccak.cpp"
#include <string>
using namespace std;

gmp_randstate_t state;
void init(){
	gmp_randinit_mt (state);
}


void generate_private_key(mpz_t private_key){
    //generate a number in 0,2**n-1
    mpz_urandomb(private_key,state,ETHA-1);

    //add 2**n-1
    mpz_t acc;
    mpz_init(acc);
    mpz_set_ui(acc,2);
    mpz_pow_ui(acc,acc,ETHA-1);
    mpz_add(private_key,private_key,acc);

    //gmp_printf("%Zd\n",private_key);
    gmp_printf ("private_key : mpz %Zd\n", private_key);

    FILE * fp=fopen ("private_key.txt","w");
    mpz_out_str (fp,16,private_key);
    fprintf(fp,"\n");
    fclose(fp);
}
void load_key(mpz_t key,char* filename){
    FILE * fp=fopen (filename,"r");
    mpz_inp_str (key,fp,16);
    fclose(fp);

}
void encrypt_bit(mpz_t msg,int bit,mpz_t private_key){
    mpz_t bruit,mask,max_size;//,acc;
    mpz_inits(bruit,mask,max_size,NULL);

    mpz_set_ui(max_size,2);
    mpz_pow_ui(max_size,max_size,GAMMA);
    mpz_cdiv_q (max_size,max_size,private_key);

    mpz_urandomb(bruit,state,RHO);
    mpz_urandomm (mask,state,max_size);

    mpz_mul_ui(bruit,bruit,2);
    mpz_mul(mask,mask,private_key);
    mpz_add(msg,mask,private_key);
    mpz_add_ui(msg,msg,bit);
   // gmp_printf ("encrypted message : mpz %Zd\n", msg);

}
int decrypt_bit(mpz_t msg,mpz_t private_key){
    mpz_t output;
    mpz_init(output);

    mpz_mod(output,msg,private_key);
    mpz_mod_ui(output,output,2);
    return mpz_sgn(output);

}
void generate_public_key(mpz_t public_key[TAU],mpz_t private_key){
    FILE * fp = fopen ("public_key.txt","w");
    //FILE * f2 = fopen ("key_compact.txt","w");
    for (int i=0;i<TAU;++i){
        encrypt_bit(public_key[i],0,private_key);
        mpz_out_str (fp,62,public_key[i]);
        //mpz_out_str (f2,62,public_key[i]);

        fprintf(fp,"\n");


    }
    fclose(fp);
    //fclose(f2);

}
void load_public_key(mpz_t public_key[TAU],char* filename){

    FILE * fp = fopen (filename,"r");
    for (int i=0;i<TAU;++i){
        mpz_inp_str (public_key[i],fp,62);


    }
    fclose(fp);
}
// we define the adress as the sum of all the public_keys mod 2**etha
char * generate_address(mpz_t public_key[TAU]){
    FILE * fp=fopen ("address.txt","w");

    mpz_t temp,max_size;
    mpz_inits(temp,max_size,NULL);
    mpz_set_ui(max_size,2);
    mpz_pow_ui(max_size,max_size,ETHA-1);



    for (int i=0;i<TAU;++i){
        mpz_add(temp,temp,public_key[i]);
        mpz_mod(temp,temp,max_size);

    }
    //char * output = mpz_get_str (NULL,62,temp);
    mpz_out_str (fp,62,temp);
    fclose(fp);
    char* hash = (char *)malloc(128/8);
    shake128File("address.txt",hash,128);
    free(hash);
    mpz_import (temp, 128/8, 1, sizeof(char), 0, 0, hash);
    return mpz_get_str(NULL,62,temp);


}
int test_pattern(char * address, string pattern){
    for (int i =0;i<pattern.length();i++){
        if (tolower(address[i])!=tolower(pattern[i])) return 0;

    }
    return 1;
}
// we only generate noise on first 64 keys to save time
void generate_vanity(mpz_t public_key[TAU],string pattern){
    mpz_t bruit;
    mpz_init(bruit);

    mpz_urandomb(bruit,state,RHO);
    mpz_add(public_key[0],public_key[0],bruit);

    char * address= generate_address(public_key);


    if (pattern.length()> strlen(address)){
        cout << "error pattern too long"<<endl;
    }
    while (!test_pattern(address,pattern)){
        for(int i=0;i<64;i++){
            mpz_sub(public_key[i],public_key[i],bruit);
            mpz_urandomb(bruit,state,RHO);
            mpz_add(public_key[i],public_key[i],bruit);

        }
        address= generate_address(public_key);
        cout << "trying "<<address<< endl;
    }



    cout << address<< endl;
}
int main() {
    //std::cout << "Hello, World!" << std::endl;
    init();
	
    mpz_t  a,b;
    mpz_inits(a,b,NULL);
    //generate_private_key(a);
    load_key(a,(char *)"private_key.txt");
    encrypt_bit(b,1,a);
    cout <<decrypt_bit(b,a)<< endl;
    mpz_t  pk[TAU];
    for (int i=0;i<TAU;++i){
        mpz_init(pk[i]);
    }
    load_public_key(pk,(char *)"public_key.txt");
    //char* address = generate_address(pk);
    //printf("\n address : %s\n",address);
    generate_vanity(pk, "po");
    return 0;
}
