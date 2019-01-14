#include "encryption.cpp"
int test_pattern(char * address, string pattern){
    for (int i =0;i<pattern.length();i++){
        if (tolower(address[i])!=tolower(pattern[i])) return 0;

    }
    return 1;
}
// we only generate noise on first 64 keys to save time
void generate_vanity(mpz_t public_key[TAU],string pattern){
    mpz_t original_public_key[64];
    mpz_t bruit;
    mpz_init(bruit);
    for(int i=0;i<64;i++){
        mpz_init(original_public_key[i]);
        mpz_set(original_public_key[i],public_key[i]);
        mpz_urandomb(bruit,state,RHO);
        mpz_add(public_key[i],original_public_key[i],bruit);

    }

    char * address= generate_address(public_key);


    if (pattern.length()> strlen(address)){
        cout << "error pattern too long"<<endl;
    }
    while (!test_pattern(address,pattern)){
        for(int i=0;i<64;i++){
            mpz_urandomb(bruit,state,RHO);
            mpz_add(public_key[i],original_public_key[i],bruit);

        }
        address= generate_address(public_key);
        cout << "trying "<<address<< endl;
    }



    cout <<"Found  :"<< address<< endl;
    //save public key
    FILE * fp = fopen ("public_key.txt","w");
    for (int i=0;i<TAU;++i){
        mpz_out_str (fp,62,public_key[i]);
        //mpz_out_str (f2,62,public_key[i]);

        fprintf(fp,"\n");


    }
    fclose(fp);

}
int main(int argc, char *argv[]) {
    init();
    mpz_t  a,b;
    mpz_inits(a,b,NULL);
    load_key(a,(char *)"private_key.txt");
    mpz_t  pk[TAU];
    for (int i=0;i<TAU;++i){
        mpz_init(pk[i]);
    }
    load_public_key(pk,(char *)"public_key.txt");
    
    if (argc<2){

        generate_vanity(pk, string("po"));

    }
    else{

        generate_vanity(pk, string(argv[1]));

    }


    return 0;
}



