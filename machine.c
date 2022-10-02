#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

FILE * fp;

#define MAX 30050
#define MAXNUM 32767
#define MAXREG 32775
uint16_t program [MAX];
uint16_t stack[MAX];
uint16_t pushpop;
uint16_t idx;
uint16_t reg[8];

#ifdef DBG
#define DEBUG printf
#else
#define DEBUG //
#endif

void fail(char * str){
  printf("ABORTING - %s\n",str);
  exit(1);
}

uint16_t getreg(){
  uint16_t num=program[idx];
  idx++;
  if(num>MAXNUM && num <=MAXREG)
    return  num-MAXNUM-1;
  fail("Invalid reg\n");
  return 0;
}

void push(uint16_t i){
  stack[pushpop]=i;
  pushpop++;
}

uint16_t pop(){
  if(pushpop==0){
    fail("Fallen off the bottom of the stack");
  }
  pushpop--;
  return stack[pushpop];
}

uint16_t getnum(){
  uint16_t num=program[idx];
  idx++;
  if(num>MAXNUM && num <=MAXREG){
    DEBUG("Reg %u\n",(num-MAXNUM-1));
    num = reg[num-MAXNUM-1];
   }	
  if(num>MAXREG){
    printf("Got number %u from index %u\n",num,idx);
    fail("Number out of range\n");
    num = -1;
   }
  return num;
}



 uint16_t  readnum(){
	uint16_t code=0;
	if(fread(&code,sizeof(uint16_t),1,fp)==0){
	  fail("Read past end of file");
	}
	return code;
}



// return: 0 - ok, 1 - halt, 2 - error
uint16_t runcmd(){
	uint16_t a,b,c,d;
	uint16_t x=getnum();
	if (x!=19) DEBUG("Command:%d\n",x);
	switch (x){
	case 0:
	  printf("Halt\n");
	  return 1;
	  break;

	case 1:
	  //set: 1 a b
	  //  set register <a> to the value of <b>
	  a=getreg();
	  b=getnum();
	  DEBUG ("Set %u  %u\n",a,b);
	  reg[a]=b;
	  break;
	case 2:
	  //push: 2 a
	  //push <a> onto the stack
	  push(getnum());
	  break;
	case 3:
	  //pop: 3 a
	  //remove the top element from the stack and write it into <a>; empty stack = error
	  a=getreg();
	  reg[a]=pop();
	  break;
	case 4:
	  //eq: 4 a b c
	  //set <a> to 1 if <b> is equal to <c>; set it to 0 otherwise
	  a=getreg();
	  b=getnum();
	  c=getnum();
	  if(b==c) reg[a]=1;
	  else reg[a]=0;
	  break;
	case 5:
	  //gt: 5 a b c
	  //set <a> to 1 if <b> is greater than <c>; set it to 0 otherwise
	  a=getreg();
	  b=getnum();
	  c=getnum();
	  reg[a]=b>c?1:0;
	  break;

	case 6:
	  //jmp: 6 a
	  //jump to <a>
	DEBUG("idx:%d\n",idx);
	  idx=getnum();
	DEBUG("jmp:%d\n",idx);
	  break;
	
	case 7:
	  //jt: 7 a b
	  //if <a> is nonzero, jump to <b>
	DEBUG("idx:%d\n",idx);
	  a=getnum();
	  b=getnum();
	  if (a!=0) idx=b;
	DEBUG("jt:%u,%u\n",a,b);
	  break;

	case 8:
	  //jf: 8 a b
	  //if <a> is zero, jump to <b>
	DEBUG("idx:%d\n",idx);
	  a=getnum();
	  b=getnum();
	  if (a==0) idx=b;
	DEBUG("jf:%u,%u\n",a,b);
	  break;

	case 9:
	  //add: 9 a b c
	  //assign into <a> the sum of <b> and <c> (modulo 32768)
	  a=getreg();
	  b=getnum();
	  c=getnum();
	  d=b+c;
	  d=d%32768;
	  DEBUG("add:%u,%u,%u : %u->%u\n",a,b,c,a,d);
	  reg[a]=d;
	  break;

	case 12:
	  //and: 12 a b c
	  //stores into <a> the bitwise and of <b> and <c>
	  a=getreg();
	  b=getnum();
	  c=getnum();
	  reg[a]=b&c;
	  break;
	case 13:
	  //or: 13 a b c
	  //stores into <a> the bitwise or of <b> and <c>
	  a=getreg();
	  b=getnum();
	  c=getnum();
	  reg[a]=b|c;
	  break;
	case 14:
	  //not: 14 a b
	  //stores 15-bit bitwise inverse of <b> in <a>
	  a=getreg();
	  b=getnum();
	  reg[a]=~b;
	  break;
	  

	case 19:
	  //out: 19 a
	  //write the character represented by ascii code <a> to the terminal
	  a=getnum();
	  printf("%c",a);
	  break;
	
	case 21:
	  //noop
	  DEBUG("noop\n");
	  break;

	  // unimplemented codes here


	case 10:
	  //mult: 10 a b c
	  //store into <a> the product of <b> and <c> (modulo 32768)
	case 11:
	  //mod: 11 a b c
	  //store into <a> the remainder of <b> divided by <c>
	case 15:
	  //rmem: 15 a b
	  //read memory at address <b> and write it to <a>
	case 16:
	  //wmem: 16 a b
	  //write the value from <b> into memory at address <a>
	case 17:
	  //call: 17 a
	  //write the address of the next instruction to the stack and jump to <a>
	case 18:
	  //ret: 18
	  //remove the top element from the stack and jump to it; empty stack = halt
	case 20:
	  //in: 20 a
	  //read a character from the terminal and write its ascii code to <a>; it can be assumed that once input starts, it will continue until a newline is encountered; this means that you can safely read whole lines from the keyboard and trust that they will be fully read

	  // unimplemented codes fall thru
	default:
	  printf("Error - unimplemented code %d\n",x);
	  return 2;

	  

	}

	return 0;
	
}



int main(int c, char ** v){
	printf("Starting ... \n");
	printf("Reading into array\n");
	fp=fopen("challenge.bin","rb");
	for (int i=0;i<8;i++){
		reg[i]=0;
	}
	uint16_t x=0;
	for (int i=0;i<MAX;i++)
	{
	  x=readnum();
	  if (x==-1){
	    printf("Bad read of byte %d\n",i);
	    return 1;
	  }
	  program[i]=x;
	}
	fclose(fp);
	printf("Read complete\n");
	idx=0;
	while(runcmd()==0); // keep running till we exit
	
	
	return 0;
}
