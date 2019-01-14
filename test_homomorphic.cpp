#include "encryption.cpp"

int main(int argc, char *argv[]) {
    init();
    mpz_t  p,a,b;
    mpz_inits(p,a,b,NULL);
    load_key(p,(char *)"private_key.txt");
    encrypt_bit(a,1,p);
    encrypt_bit(b,0,p);
    mpz_add(a,a,b);
    cout<<decrypt_bit(a,p)<<endl;
    

    return 0;
}
