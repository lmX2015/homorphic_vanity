#include "encryption.cpp"

int main() {
    //std::cout << "Hello, World!" << std::endl;
    init();

    mpz_t  a,b;
    mpz_inits(a,b,NULL);
    generate_private_key(a);
    mpz_t  pk[TAU];
    for (int i=0;i<TAU;++i){
        mpz_init(pk[i]);
    }

    generate_public_key(pk,a);
    return 0;
}
