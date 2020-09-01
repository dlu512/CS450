// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - DAVID LU
//  twinprime.c
//
//
//  Created by David Lu on 9/22/18.
//
#include <stdlib.h>
#include <stdio.h>
#define BITSPERSEG (8*256*sizeof(int)) //8192


typedef struct _seg{ //seg structure
    int bits[256];
    struct _seg *next;
}seg;

typedef struct{ //coordinate structure
    seg *segpt;
    int intnum;
    int bitnum;
}coordinate;
void setBitstoZero(seg *m){ //sets all bits in each seg to zero
    int i;
    for (i=0; i<256; i++){
        m->bits[i] = 0;
    }
}
seg *head;
seg *whichseg(int j){//given an int, return the segment the int is in
    int segnum = (j + BITSPERSEG - 1)/BITSPERSEG;
    if (segnum<2){
        return head;
    }
    else{
        int c;
        seg *temp = head;
        for (c=1; c<segnum; c++){
            temp = temp->next;
        }
        return temp;
    }
}
int whichint (int j){   //returns the int part of the coordinate for int j
    int segnum = (j + BITSPERSEG - 1)/BITSPERSEG;
    return (j + 31 - BITSPERSEG*(segnum-1))/32-1;
    
}
int whichbit (int j){ // returns the bit part of the coordinate for int j
    int segnum = (j + BITSPERSEG - 1)/BITSPERSEG;
    int which = whichint(j);
    int whichbit = j - (BITSPERSEG * (segnum-1)) - (32*(which));
    return whichbit-1;
    
}
coordinate getcoord(int j){ //returns the coordinate for int j
    coordinate newcoord;
    newcoord.segpt = whichseg(j);
    newcoord.intnum = whichint(j);
    newcoord.bitnum = whichbit(j);
    return newcoord;
}
void markcoord (coordinate c){ //marks the coordinate given
    (c.segpt->bits[c.intnum]) |= 1 << (c.bitnum);
}

int testcoord (coordinate c ){ //tests if the coordinate is a prime
    int bit = ((c.segpt->bits[c.intnum]) >> c.bitnum) & 1;
    return bit;
}
void marknonprime (int j){ //marks the coordinate cooresponding to int j as non prime
    markcoord(getcoord(j));
}
int testprime (int j){ //tests if j is prime
    return testcoord(getcoord(j));
}
coordinate incrcoord (coordinate c,int inc){ //increments the coordinates by int inc
    int tempbitnum = c.bitnum + inc; //31 + 4
    int intcarrycount = 0;
    while (tempbitnum>31){ //35-32=3
        tempbitnum = tempbitnum - 32;
        intcarrycount++;
    }
    int tempintnum = c.intnum + intcarrycount;
    int segcarrycount = 0;
    while (tempintnum>255){
        tempintnum = tempintnum - 256;
        segcarrycount++;
    }
    int i;
    seg *tempseg = c.segpt;
    for (i=0; i<segcarrycount; i++){
        tempseg = tempseg->next;
    }
    coordinate newcoord;
    newcoord.bitnum = tempbitnum;
    newcoord.intnum = tempintnum;
    newcoord.segpt = tempseg;
    return newcoord;
}
int whichnum (coordinate c){ //returns the int given a coordinate c
    int tempint = c.intnum;
    int tempbit = c.bitnum + 1;
    seg *tempseg = c.segpt;
    seg *pt = head;
    int count = 0;
    while (pt->bits!=tempseg->bits){
        pt = pt->next;
        count++;
    }
    return BITSPERSEG*count + 32 * tempint + tempbit;
    
}
int main(int argc, char* argv[]){
    if (argc==1){
        printf("\nEnter a number N as command line argument");
    }
    int N = atoi(argv[1]);
    int M = N;
    seg *pt;
    int i;
    N = (N + BITSPERSEG - 1)/BITSPERSEG;
    head = (seg* ) malloc(sizeof (seg));        //allocates memory for the initial segment
    setBitstoZero(head);
    pt = head;
    for (i=1; i<N; i++){                        //creates additional segments if needed and allocates memory
        pt->next = (seg*) malloc(sizeof(seg));
        setBitstoZero(pt);
        pt = pt->next;
    }
    //begin sieve
    int x = 2;
    marknonprime(1); //marks 1 as non-prime
    while (x<=M){
        if (testprime(x)==0){
            int tester;
            for (tester=x+1; tester<=M; tester++){
                if (tester%x==0){
                    marknonprime(tester);
                }
            }
        }
        x++;
    }
    //counts the number of primes and prints it out
    int count = 0;
    for (i=1; i<=M; i++){
        if (testprime(i)==0){
            count++;
        }
    }
    printf("Number of primes: %d" , count);
    printf("\n Enter an even number: ");
    int k;
    scanf("%d", &k);
    while (k%2!=0){
        printf("Please enter an even number: \n");
        scanf("%d", &k);
    }
    while (1){
        if (k == EOF){
            break;
        }
        //twinprimes!
        int i;
        int twincount = 0;
        int largesttwinOne = 0;
        int largesttwinTwo = 0;
        for (i=1; i<=M-k; i++){
            coordinate newcoord = getcoord(i);
            coordinate comparecoord = incrcoord(newcoord, k);
            if (testcoord(newcoord)==0){
                if (testcoord(comparecoord)==0){
                    twincount++;
                    largesttwinOne = i;
                    largesttwinTwo = i+k;
                }
            }
        }
        printf("There are %d twin primes with interval %d." , twincount, k);
        printf("The largest Twin Primes are: %d and %d \n" , largesttwinOne, largesttwinTwo);
        scanf("%d", &k);
        while (k%2!=0){
            printf("Please enter an even number: \n");
            scanf("%d", &k);
        }
    }
    
    return 0;
    
    
}

