#include<stdio.h>

unsigned short a,d,ram[0x7fff],prog[0x7fff],pc;

FILE*infile;

char init(char*fname){
	for(int i=0;i<0x7fff;i++)ram[i]=0;
	FILE*f=fopen(fname,"r");
	if(!f){return 1;}
	pc=0;
	printf("     |   |   |   |   |\n");
	printf("         *admu01zsleg\n");
	while(1){
		a=0;
		for(int j=0;j<16;j++)
			a=(a<<1)|(getc(f)=='1');
		if(feof(f))break;
		printf("%04x %016b (%4x)\n",pc,a,a);
		prog[pc++]=a;
		while(getc(f)!=10 && !feof(f));
	}
	fclose(f);
	a=d=pc=0;
	return 0;
}

char loop(){
	unsigned short inst=prog[pc++],l,r;
	printf("command is %04x\n",inst);
	if(inst==0xffff){printf("EOF\n");return 0;}
	if(!(inst&0x8000)){printf("A cmd\n");a=inst;return 1;}

	const char*jnames[]={"nil","jgt","jeq","jge","jlt","jne","jle","jmp"},
	*cmdnames[]={"and","or","xor","not","add","sub","inc","dec"};

	printf("C cmd (%c%c%c%c, cmd:%s zx:%c sw:%c jmp:%s)",
		inst&0x800?'*':' ',
		inst&0x400?'a':' ',
		inst&0x200?'d':' ',
		inst&0x100?'m':' ',
		cmdnames[(inst>>5)&7],
		inst&0x10?'y':'n',
		inst&0x08?'y':'n',
		jnames[inst&7]
	);

	l=a,r=d;
	if(inst&0x800){
		if(a==0x7fff && infile)l=getc(infile);
		else l=ram[a];
	}
	if(inst&0x08)r=l,l=d;
	if(inst&0x10)l=0;
	switch((inst>>5)&7){
		case 0:l&=r;break;
		case 1:l|=r;break;
		case 2:l^=r;break;
		case 3:l=!l;break;

		case 4:l+=r;break;
		case 5:l-=r;break;
		case 6:l+=1;break;
		case 7:l-=1;break;
	}

	//bits 5-3 tell us where to store output (a d *a)
	if(inst&0x400)a=l;
	if(inst&0x200)d=l;
	if(inst&0x100)ram[a]=l;

	//bits 2-0 tell us whether to jump on lt/eq/gt (result to 0)
	r=pc-1;
	if(inst&4 && l>0x8000)pc=a;
	if(inst&2 && l==0)pc=a;
	if(inst&1 && l>0 && l<0x8000)pc=a;
	if(pc==r){printf(" terminating\n");return 0;}
	if(pc==a)printf(" jumped");

	putchar(10);return 1;
}

void print(){
	printf("pc: %2x (%04x)  a:%04x  d:%04x  m:%04x\n",pc,prog[pc],a,d,ram[a]);
	for(int i=0;i<8;i++)printf("%2x:% 4x | ",i,ram[i]);
	printf("\n");
}

void main(int argc,char**argv){
	if(argc<2)return;
	if(init(argv[1]))return;
	infile=argc>2?fopen(argv[2],"r"):0;
	print();putchar(10);
	//for(int i=0;i<100;i++)loop(),print(),putchar(10);
	while(loop())print(),putchar(10);

}
