#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>

int w =24;
int r = 1600-256;
int b = 1600;
int c = 256;
int l = 4 ;// log2(w)


uint64_t* arrayFromString(char* s){
    return ((uint64_t*) s);


}
char * stringFromArray(uint64_t* A ){
    return((char*) A);
}
/*
 * z[k]<- z[k-1]
 * */

uint64_t rotateR(uint64_t z){
    //save last bit
    uint64_t temp = (z>>63)&1;
    uint64_t res = (z<<1);
    return	(res|temp);

}
uint64_t setBit(uint64_t source,int value,int index){
    uint64_t mask = ((uint64_t)1)<<index;

    if (value) return (source|mask);
    return source &(~mask);
}
uint64_t getbit(uint64_t source,int index){
    uint64_t mask = ((uint64_t)1)<<(63-index);
    return source&mask;
}
void print55arr(uint64_t a[5][5]){
    for (int i =0; i<5;++i){
        for(int j = 0; j < 5; j++) {
            //printf("0x%x\n",  a[j][i]);
        }
    }
    printf("\n");


}
void theta(uint64_t a[5][5]){
    //uint64_t (* a)[5] = (int (*)[5])A;
    uint64_t C[5]={0};
    uint64_t D[5]={0};
    for (int x=0;x<5;++x){
        C[x]=a[0][x]^a[1][x]^a[2][x]^a[3][x]^a[4][x];

    }
    for (int x=0;x<5;++x){
        D[x]=C[(5+x-1)%5]^rotateR(C[(x+1)%5]);

    }
    for (int x=0;x<5;++x){
        for (int y=0;y<5;++y ){
            a[y][x]=a[y][x]^D[x];
        }
    }
    //return((uint64_t *)a);
}
void rho(uint64_t a[5][5]){
    //uint64_t (* a)[5] = (int (*)[5])A;
    uint64_t  abis[5][5];
    memcpy(abis,a,25*sizeof(uint64_t));
    int x=1;
    int y =0;
    //a[0][0]=abis[0][0];
    for (int t=0;t<24;++t){
        a[y][x]=abis[y][x];
        for (int j=0;j<((t+1)*(t+2)/2)%64;++j){
            a[y][x]=rotateR(a[y][x]);
        }
        int c= x;
        x=y;
        y= (2*c+3*y)%5;

    }

    //return((uint64_t*) abis);
}
void pi(uint64_t a[5][5]){
    //uint64_t (* a)[5] = (int (*)[5])A;
    uint64_t  abis[5][5];
    memcpy(abis,a,25*sizeof(uint64_t));

    for (int x=0;x<5;++x){
        for (int y=0;y<5;++y){
            a[y][x]=abis[x][(x+3*y)%5];
        }

    }
    // return((uint64_t*) abis);
}

void chi(uint64_t a[5][5]){
    //uint64_t (* a)[5] = (int (*)[5])A;
    uint64_t  abis[5][5];
    memcpy(abis,a,25*sizeof(uint64_t));

    for (int x=0;x<5;++x){
        for (int y=0;y<5;++y){
            a[y][x]=abis[y][x]^((abis[y][(x+1)%5]^(~((uint64_t)0)))&abis[y][(x+2)%5]);
        }

    }
    //return((uint64_t*) abis);
}
/*
 * R is q u char r[0]= lastbit
 * l=R[8];
 * mask is 1110001=49
 * */
unsigned char rc(int t){
    unsigned char R=(unsigned char)1;
    unsigned char l =(unsigned char)0;
    if (t%255==0) return R;
    for (int i=1;i<=t%255;++i){
        l=(R>>7)&1; // get last bit
        if(l){
            l=(unsigned char)255; //replicate last bit
        }
        R=R<<1;// add 0 and truncate
        R= R^(l&((unsigned char)113)); //xor on relevant bits
    }
    return (R&((unsigned char)1));

}
int pow2(int n){
    if (n==0) return 1;
    if (n==1) return 2;
    if ((n%2)==0)return pow2(n/2)*pow2(n/2);
    return (2*pow2(n/2)*pow2(n/2));
}
void iota(uint64_t a[5][5],int ir){
    uint64_t RC= (uint64_t)0;
    for (int j=0;j<=6;++j){
        RC=setBit(RC,(int)rc(j+7*ir),pow2(j)-1);

    }
    a[0][0]=a[0][0]^RC;


}
void Rnd(uint64_t a[5][5],int ir){

//	print55arr(a);
    theta(a);
    rho(a);

    // print55arr(a);
    pi(a);
    chi(a);
    iota(a,ir);
}
void keccakp(char * source,char * target,int nr){
    uint64_t a[5][5];
    memcpy(&a,source,25*sizeof(uint64_t));
    //print55arr(a);
    for (int ir=24-nr;ir<=23;++ir){
        Rnd(a,ir);
        //print55arr(a);
    }
    //print55arr(a);
    memcpy(target,&a,25*sizeof(uint64_t));

}
int padLen(int x,int m){
    // warning m is message size in bytes
    return (((-(m*8+4)-2)+x)%x);//first add 1111 to the message
}
void pad(int x, int m,char * target){
    //printf("m: %d\n",m);
//	unsigned char*  fckcast = (unsigned char*) target;
    int l=padLen(x,m);
    if (l==2){
        target[m]=(char)'\x9F';
    }
    else {
        target[m]=(char)'\x1F';
        target[m+(l-6)/8+1]='\x80';
    }
}

int shake128(const char* Psource,char * dest,int filelen,int d){
    char * P =(char *)malloc(filelen);
    memcpy(P,Psource,filelen);
    pad(r,filelen,P);
    int n = (filelen*8+padLen(r,filelen)+6)/r;
    char* s=(char *) calloc (b/8,sizeof(char));
    for (int i=0;i<n;++i){
        for (int k=0;k<r/8;++k){
            		//printf("char %c\n",P[i*r+k]);
            s[k]=P[i*r+k]^s[k];
        }
        keccakp(s,s,24);
    }
    int count =0;
    unsigned char* Z = (unsigned char *)calloc(1+r/8+d,sizeof(char));
    while (count<d*8){

        for (int i=0;i<r/8;++i){
            Z[i+count/8]=s[i];
        }
        count += r;
        //printf("%s\n",Z);
        keccakp(s,s,24);
    }
    Z[d]='\0';
    //printf("%2x\n",Z[1]);
    for (int i=0;i<d;i++){
       //printf("%02hhx",Z[i]);
    }

    if (dest != NULL){
        memcpy(dest,Z,d/8);
    }
    //free(Z);
    //free(s);
    //printf("\n");
    return 0;
}
int shake128File(char* fileName,char* dest,int d) {
    FILE *file;
    char *buffer;
    long filelen;
    file = fopen(fileName, "r");
    if (!file) {
        printf("file not found\n");
        return -1;
    }
    fseek(file, 0, SEEK_END);
    filelen = ftell(file);
    filelen -= 1; // discard the EOF char
    rewind(file);

    buffer = (char *) calloc((filelen + 1), sizeof(char));
    fread(buffer, filelen, 1, file);
    fclose(file);
    char *P = (char *)calloc((filelen + (padLen(r, filelen) + 6) / 8), sizeof(char));

    memcpy(P, buffer, filelen);
    //free(buffer);
    return shake128(P,dest,filelen,d);

}




