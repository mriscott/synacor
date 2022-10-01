#include <stdio.h>

FILE * fp;

#define MAX 30050
#define MAXNUM 32767
#define MAXREG 32775
unsigned int program [MAX];
int idx;
unsigned int reg[8];

#define DBG 1
#ifdef DBG
#define DEBUG printf
#else
#define DEBUG //
#endif

int getreg(){
  int num=program[idx];
  idx++;
  if(num>MAXNUM && num <=MAXREG)
    return  num-MAXNUM;
  return -1;
}

unsigned int getnum(){
  int num=program[idx];
  idx++;
  if(num>MAXNUM && num <=MAXREG)
    num = reg[num-MAXNUM];
  if(num>MAXREG)
    num = -1;
  return num;
}



unsigned short readnum(){
	unsigned short code=0;
	if(fread(&code,sizeof(short),1,fp)==0){
	  return -1;
	}
	return code;
}



// return: 0 - ok, 1 - halt, 2 - error
int runcmd(){
	unsigned int a,b,c;
	unsigned int x=getnum();
	//if (x!=19) DEBUG("Command:%d\n",x);
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
	  DEBUG ("Setting %u to %u\n",a,b);
	  reg[a]=b;
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
	case 2:
	  //push: 2 a
	  //push <a> onto the stack
	case 3:
	  //pop: 3 a
	  //remove the top element from the stack and write it into <a>; empty stack = error
	case 4:
	  //eq: 4 a b c
	  //set <a> to 1 if <b> is equal to <c>; set it to 0 otherwise
	case 5:
	  //gt: 5 a b c
	  //set <a> to 1 if <b> is greater than <c>; set it to 0 otherwise
	case 9:
	  //add: 9 a b c
	  //assign into <a> the sum of <b> and <c> (modulo 32768)

	case 10:
	  //mult: 10 a b c
	  //store into <a> the product of <b> and <c> (modulo 32768)
	case 11:
	  //mod: 11 a b c
	  //store into <a> the remainder of <b> divided by <c>
	case 12:
	  //and: 12 a b c
	  //stores into <a> the bitwise and of <b> and <c>
	case 13:
	  //or: 13 a b c
	  //stores into <a> the bitwise or of <b> and <c>
	case 14:
	  //not: 14 a b
	  //stores 15-bit bitwise inverse of <b> in <a>
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
	unsigned int x=0;
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
