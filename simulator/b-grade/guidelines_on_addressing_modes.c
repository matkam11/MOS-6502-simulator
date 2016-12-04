// This is the base for every example bellow
bool carry_flag=1;
uint_8 mem[65536];
uint_8 acc = 0x11110000;
mem[0x00] = 0x01;
mem[0x01] = 0x01;
mem[0x02] = 0x01;
mem[0x03] = 0x01;
mem[0x04] = 0x01;
// =====================================================================
// Non-memory based addressing

// --- Accumulator
//			Perform operations with the accumulator
//			ASL A
			acc = 0x11100001


// --- Immediate
//			Uses myvalue
//			ADC #$myvalue
			acc = acc + myvalue;
//			ADC #$44
			acc = acc + 0x44;
			

// --- Implied
//			Instructions that have implied values like the flags
//			Clear the carry flag
//			CLC
			carry_flag = false;

// ---------------------------------------------------------------------

// This is the base for every example bellow
bool carry_flag=1;
uint_8 mem[65536];
uint_8 acc = b'11110000';
mem[0x00] = 0x01;
mem[0x01] = 0x0A;
mem[0x02] = 0x02;
mem[0x03] = 0x01;
mem[0x04] = b'00000010';
mem[0x2004] = b'00000010';
mem[0x020A] = 0xFF;
mem[0x020B] = 0x00;
pc = 0xc002

// =====================================================================
// Memory - non indexed



// --- Relative
//			offset from current pc counter
//			Branch on Plus
//			BPL offset
			pc = pc + offset


// --- Zero Page
//			Uses the contents of memory pointed by myvalue
//			ADC	$myvalue
			acc = acc + mem[myvalue];

//			ADC	$04
			acc = acc + mem[0x04];
			acc = b'11110010'	


// --- Absolute
//			Same as zero page but with more addresses to look at
//			ADC	$myvalue
			acc = acc + mem[myvalue];

//			ADC	$2004
			acc = acc + mem[0x2004];
			acc = b'11110011'

// --- Indirect
//			Uses the value in pointed by myvalue
//			JMP ($myvalue)
			pc = mem[mem[myvalue]];

//			JMP ($0001)
			pc = mem[mem[0x0001]];
			pc = mem[0x02<<8||0x0A];
			pc = mem[0x020A]; // it loads 2x 8bits =16 bits
			pc = 0x00FF

---------------------------------------------------------------------

// This is the base for every example bellow
bool carry_flag=1;
uint_8 mem[65536];
uint_8 acc = b'11110000';
mem[0x00] = 0x01;
mem[0x01] = 0x0A;
mem[0x02] = 0x02;
mem[0x03] = 0x01;
mem[0x04] = b'00000010';

mem[0x2004] = b'00000010';
mem[0x2005] = b'00000010';
mem[0x2006] = b'00000010';
mem[0x2007] = b'00000010';

mem[0x0020C] = 0xFF;


pc = 0xc002;
X=0x02;
Y=0x02;


//=====================================================================
// Memory - Indexed


// --- Absolute indexed
//			get the value pointed by myvalue+Y
//			LDA $myvalue,Y
			acc = mem[myvalue + Y]

//			LDA $0x2004,Y
			acc = mem[0x2004+0x02];
			acc = mem[0x2006];
			acc = b'00000010';


// --- Zero Page indexed
//			get the value pointed by myvalue+Y but only works on zero page
//			LDA $myvalue,Y
			if(myvalue+Y<256)			
				acc = mem[myvalue + Y];
			else
				acc = mem[(myvalue+ Y)%256];
//			LDA $0x01,Y
			acc = mem[0x01+0x02];
			acc = mem[0x03];
			acc = 0x01;	


// --- Indirect indexed
//			get the value pointed by myvalue+X but only works on zero page
//			THIS MAY HAVE A ZERO PAGE WRAP AROUND
//			LDA ($myvalue,X)
			if(myvalue+Y<256)			
				acc = mem[(mem[myvalue +X+1]<<8)
						|| mem[myvalue +X]];
			else
				acc = mem[(mem[(myvalue +X+1)%256]<<8)
						|| mem[(myvalue +X)  %256]]; // just the remainder


// --- Indexed Indirect 
//			get the value pointed by myvalue, sum Y, get the value
//			pointed by the result

//			LDA ($myvalue),Y
			acc = mem[(mem[myvalue+1]<<8||mem[myvalue]) + Y)];

//			LDA ($01),Y
			acc = mem[(mem[0x02]<<8||mem[0x01])+0x02];
			acc = mem[(0x020A)+0x02];
			acc = mem[0x020A+0x02];
			acc = mem[0x020C];
			acc = 0xFF;

