#include "int.h"
#include <string>
#include <cstring>

#include <stdlib.h>

//#define TEST_NUM 1000000000
//#define TEST_NUM 30000000
#define TEST_NUM 30000000

unsigned int total = 0;
int total_add = 0;
int total_sub = 0;
int total_div = 0;
int total_mul = 0;
int total_mod = 0;
int total_cmp = 0;
int total_log = 0;
int total_shi = 0;

void zero_clear (unsigned long &x, unsigned int width)
{
	unsigned long mask = 0;
	mask = ~mask;
	mask = mask << width;
	mask = ~mask;

	x &= mask;
}

void mask_stat (unsigned long &x, unsigned int width)
{
	unsigned long mask = 0;
	mask = ~mask;
	mask = mask << width;
	mask = ~mask;

	x &= mask;
}

int is_neg(unsigned long x, unsigned int width)
{
	unsigned long msb = 1;

	x = (x >> (width-1)) & 0x1;

	return x;
}

unsigned long sign_ext(unsigned long x, unsigned int width)
{

	if (x == 0) { return x; }

	if ((x >> (width - 1)) && 0x1) {
		unsigned int tmp = width % BIGINT_BITS_ONE;
		unsigned int wrd = width / BIGINT_BITS_ONE;
  	
		UNIT_TYPE mask = 0;
		if (tmp > 0) {
			mask = ~mask;
			mask = mask << tmp;
		}
  	
		unsigned long l_mask;
		l_mask = mask;
		l_mask = l_mask << 8*sizeof(UNIT_TYPE)*wrd;
  	
		x = x | l_mask;
	}

	return x;
}

void full_sign_ext(unsigned long &x, unsigned int width)
{

	if (x == 0) { return; }

	if ((x >> (width - 1)) && 0x1) {
		unsigned int tmp = width % 64;
  	
		unsigned long mask = 0;

		if (tmp > 0) {
			mask = ~mask;
			mask = mask << tmp;
		}
  	
		x = x | mask;
	}

	return;
}

int main () {

#ifdef BIGINT_TEST
	unsigned long test1, test2, test3;
	unsigned long test1_tmp, test2_tmp;

	UNIT_TYPE v1, v2;
	unsigned int s1, s2, d1, d2;
	unsigned int n1, n2;

	unsigned int unit_size = 8 * sizeof(UNIT_TYPE);

	unsigned int wn = 0;

	unsigned long sign_mask = 0;

	std::string sop1, sop2, sop3;

	BigInt test;
	test = "0xabc";

	for (auto t = 0; t < TEST_NUM; t++) {

    total++;

		unsigned int rwn1 = rand() % 4 + 1;//((sizeof(unsigned long)/sizeof(UNIT_TYPE))-1)+1;
		unsigned int rwn2 = rand() % 4 + 1;//((sizeof(unsigned long)/sizeof(UNIT_TYPE))-1)+1;
		unsigned int resw = 0;

		BigInt op1(8*rwn1, 0, 0);
		BigInt tmp1(8*rwn1, 0, 0);
		BigInt op2(8*rwn2, 0, 0);
		BigInt tmp2(8*rwn2, 0, 0);
		BigInt op3;

		int cmp_op = 0;
		UNIT_TYPE shift_op = 0;

		test1 = 0;
		test2 = 0;

		test1_tmp = 0;
		test2_tmp = 0;

		for (auto wn = rwn1; wn > 0; wn--) {
			v1 = rand() % 255;
			test1 = (test1 << BIGINT_BITS_ONE) | v1;
			test1_tmp = (test1_tmp << BIGINT_BITS_ONE) | v1;
			op1.SetV(wn-1, v1);
			tmp1.SetV(wn-1, v1);
		}
		for (auto wn = rwn2; wn > 0; wn--) {
			v2 = rand() % 255;
			test2 = (test2 << BIGINT_BITS_ONE) | v2;
			test2_tmp = (test2_tmp << BIGINT_BITS_ONE) | v2;
			op2.SetV(wn-1, v2);
			tmp2.SetV(wn-1, v2);
		}

		s1 = rand() % 2;
		s2 = rand() % 2;

		d1 = rand() % 2;
		d2 = rand() % 2;
/*
		if (s1 == 1) {
			op1.toSigned();
			tmp1.toSigned();
			test1 = (long)test1;
			test1_tmp = (long)test1_tmp;
		} else {
			op1.toUnsigned();
			tmp1.toUnsigned();
			test1 = (unsigned long)test1;
			test1_tmp = (unsigned long)test1_tmp;
		}
		if (s2 == 1) {
			op2.toSigned();
			tmp2.toSigned();
			test2 = (long)test2;
			test2_tmp = (long)test2_tmp;
		} else {
			op2.toUnsigned();
			tmp2.toUnsigned();
			test2 = (unsigned long)test2;
			test2_tmp = (unsigned long)test2_tmp;
		}
*/
		if (s1 == 1 && s2 == 1) {
			op1.toSigned();
			tmp1.toSigned();
			op2.toSigned();
			tmp2.toSigned();
			test1 = (long)test1;
			test1_tmp = (long)test1_tmp;
			test2 = (long)test2;
			test2_tmp = (long)test2_tmp;
			test3 = (long)test3; 
		} else {
			op1.toUnsigned();
			tmp1.toUnsigned();
			op2.toUnsigned();
			tmp2.toUnsigned();
			test1 = (unsigned long)test1;
			test1_tmp = (unsigned long)test1_tmp;
			test2 = (unsigned long)test2;
			test2_tmp = (unsigned long)test2_tmp;
			test3 = (unsigned long) test3; 
		}

		sign_mask = 0;
		if (s1 && (rwn1 < rwn2)) {
			if (s1 && s2 && op1.isNegative()) {
				sign_mask = ~sign_mask;
				sign_mask = sign_mask >> (8*(sizeof(unsigned long) - (rwn2-rwn1)));
				sign_mask = sign_mask << (rwn1*BIGINT_BITS_ONE);
				test1 |= sign_mask;
				test1_tmp |= sign_mask;
			}
		} else if (s2 && (rwn2 < rwn1)) {
			if (s1 && s2 && op2.isNegative()) {
				sign_mask = ~sign_mask;
				sign_mask = sign_mask >> (8*(sizeof(unsigned long) - (rwn1-rwn2)));
				sign_mask = sign_mask << (rwn2*BIGINT_BITS_ONE);
				test2 |= sign_mask;
				test2_tmp |= sign_mask;
			}
		}

		if (d1 && (rwn1 < sizeof(unsigned long))) {
			op1.toDynamic();
			tmp1.toDynamic();
		} else {
			op1.toStatic();
			tmp1.toStatic();
		}
		if (d2 && (rwn2 < sizeof(unsigned long))) {
			op2.toDynamic();
			tmp2.toDynamic();
		} else {
			op2.toStatic();
			tmp2.toStatic();
		}

		/*
				ADDITION TEST
		*/
		test3 = test1 + test2;
		op3 = op1 + op2;

		if (s1 && s2) {	test3 = sign_ext(test3, op3.getWidth()); }
		if (d1 == 0) { mask_stat (test3, op3.getWidth()); }
		if (op3 == test3) {
		} else {
      total_add++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "ADD FAILED:\n%lx + %lx = %lx\n", test1, test2, test3);
			sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
			fprintf(stdout, "%s + %s = %s(%i)\n", sop1.c_str(), sop2.c_str(), sop3.c_str(), op3.getWidth());
		}

		/*
				SUBTRACTION TEST
		*/
		op1 = tmp1;
		op2 = tmp2;
    
		test3 = test1 - test2;
		op3 = op1 - op2;

		if (s1 && s2) {	test3 = sign_ext(test3, op3.getWidth()); }
		if (d1 == 0) { mask_stat (test3, op3.getWidth()); }
		if (op3 == test3) {
		} else {
      total_sub++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "SUB FAILED:\n%lx - %lx = %lx\n", test1, test2, test3);
			sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
			fprintf(stdout, "%s - %s = %s\n", sop1.c_str(), sop2.c_str(), sop3.c_str());
		}

		/*
				DIVISION TEST
		*/
		if (test2 != 0) {
			op1 = tmp1;
			op2 = tmp2;
    	
			zero_clear(test1, op1.getWidth());
			zero_clear(test2, op2.getWidth());
			if (s1 == 1 && s2 == 1 && op1.isNegative()) { test1 = (-test1); }
			if (s1 == 1 && s2 == 1 && op2.isNegative()) { test2 = (-test2); }
    	
			zero_clear(test1, op1.getWidth());
			zero_clear(test2, op2.getWidth());
	  	
			op3 = op1 / op2;
			test3 = test1 / test2;
    	
			if (s1 == 1 && s2 == 1 && (tmp1.isNegative() != tmp2.isNegative())) { 
				test3 = (-test3);
			}
			
			//if (op3.getWidth() != 64) {
			//	mask_stat(test3, op3.getWidth());
			//}

			if (s1 && s2) {	test3 = sign_ext(test3, op3.getWidth()); }
			if (op3 == test3) {
			} else {
        total_div++;
				fprintf(stdout, "===========================\n");
				fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
				fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
				fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
				fprintf(stdout, "DIV FAILED:\n%lx / %lx = %lx\n", test1, test2, test3);
				sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
				fprintf(stdout, "%s / %s = %s\n", sop1.c_str(), sop2.c_str(), sop3.c_str());
			}
		}

		/*
				MODULO TEST
		*/
		test1 = test1_tmp;
		test2 = test2_tmp;

		if (test2 != 0) {
			op1 = tmp1;
			op2 = tmp2;

			zero_clear(test1, op1.getWidth());
			zero_clear(test2, op2.getWidth());
			if (s1 == 1 && s2 == 1 && op1.isNegative()) { test1 = (-test1); }
			if (s1 == 1 && s2 == 1 && op2.isNegative()) { test2 = (-test2); }
    	
			zero_clear(test1, op1.getWidth());
			zero_clear(test2, op2.getWidth());
	  	
			op3 = op1 % op2;
			test3 = test1 % test2;
    	
			if (s1 == 1 && s2 == 1 && (tmp1.isNegative() != tmp2.isNegative())) { 
				test3 = (-test3);
			}
			//if (op3.getWidth() != 64) {
			//	mask_stat(test3, op3.getWidth());
			//}
  
			if (s1 && s2) {	test3 = sign_ext(test3, op3.getWidth()); }
			if (op3 == test3) {
			} else {
        total_mod++;
				fprintf(stdout, "===========================\n");
				fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
				fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
				fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
				fprintf(stdout, "MOD FAILED:\n%lx %% %lx = %lx\n", test1, test2, test3);
				sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
				fprintf(stdout, "%s %% %s = %s\n", sop1.c_str(), sop2.c_str(), sop3.c_str());
			}
		}

		/*
				MULTIPLICATION TEST
		*/
		op1 = tmp1;
		op2 = tmp2;
		test1 = test1_tmp;
		test2 = test2_tmp;

		if (rwn1 > 4) {
			op1.setWidth(32);
			test1 &= 0x00000000FFFFFFFF;
		}
		if (rwn2 > 4) {
			op2.setWidth(32);
			test2 &= 0x00000000FFFFFFFF;
		}

		if (s1 == 1 && s2 == 1 && op1.isNegative()) { test1 = (-test1); }
		if (s1 == 1 && s2 == 1 && op2.isNegative()) { test2 = (-test2); }

		zero_clear(test1, op1.getWidth());
		zero_clear(test2, op2.getWidth());

		test3 = test1 * test2;
		op3 = op1 * op2;

		if (s1 == 1 && s2 == 1 && (op1.isNegative() != op2.isNegative())) { 
			test3 = (-test3);
		}
		//if (op3.getWidth() != 64) {
		//	mask_stat(test3, op3.getWidth());
		//}

		if (op3 == test3) {
		} else {
      total_mul++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "MUL FAILED:\n%lx * %lx = %ld\n", test1, test2, test3);
			sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
			fprintf(stdout, "%s * %s = %s\n", sop1.c_str(), sop2.c_str(), sop3.c_str());
		}

		/*
				LOGIC TEST
		*/

		//The result is of the A length
		//Expect zero/sign extension of the B operand.

		op1 = tmp1;
		op2 = tmp2;
		test1 = test1_tmp;
		test2 = test2_tmp;

		test3 = test1 & test2;
		op3 = op1 & op2;

		if (op3 != test3) {
      total_log++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "AND FAILED:\n%lx & %lx = %lx\n", test1, test2, test3);
			sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
			fprintf(stdout, "%s & %s = %s\n", sop1.c_str(), sop2.c_str(), sop3.c_str());
		}
	
		op1 = tmp1;
		op2 = tmp2;

		test3 = test1 | test2;
		op3 = op1 | op2;

		if (op3 != test3) {
      total_log++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "OR FAILED:\n%lx | %lx = %lx\n", test1, test2, test3);
			sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
			fprintf(stdout, "%s | %s = %s\n", sop1.c_str(), sop2.c_str(), sop3.c_str());
		}

		op1 = tmp1;
		op2 = tmp2;

		test3 = test1 ^ test2;
		op3 = op1 ^ op2;

		if (op3 != test3) {
      total_log++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "xor FAILED:\n%lx ^ %lx = %lx\n", test1, test2, test3);
			sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
			fprintf(stdout, "%s ^ %s = %s\n", sop1.c_str(), sop2.c_str(), sop3.c_str());
		}

		op1 = tmp1;
		op2 = tmp2;
		test1 = test1_tmp;
		test2 = test2_tmp;

		zero_clear(test1, op1.getWidth());
		zero_clear(test2, op2.getWidth());

		test3 = ~test1;
		op3 = ~op1;

		if (op3 != test3) {
      total_log++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
			fprintf(stdout, "OP1: %lx\n", test1);
			fprintf(stdout, "NOT FAILED:\n~%lx = %lx\n", test1,test3);
			sop1 = op1.sPrint(); sop3 = op3.sPrint();
			fprintf(stdout, "~%s = %s\n", sop1.c_str(), sop3.c_str());
		}

		/*
			COMPARISON TEST
		*/

		op1 = tmp1;
		op2 = tmp2;
		test1 = test1_tmp;
		test2 = test2_tmp;

		test3 = test1 == test2;
		cmp_op = op1 == op2;

		if (cmp_op != test3) {
      total_cmp++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "EQ FAILED:\n%lx == %lx = %lx\n", test1, test2, test3);
			sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
			fprintf(stdout, "%s == %s = %i\n", sop1.c_str(), sop2.c_str(), cmp_op);
		}

		op1 = tmp1;
		op2 = tmp2;
		test1 = test1_tmp;
		test2 = test2_tmp;

		test3 = test1 != test2;
		cmp_op = op1 != op2;

		if (cmp_op != test3) {
      total_cmp++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "NEQ FAILED:\n%lx != %lx = %lx\n", test1, test2, test3);
			sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
			fprintf(stdout, "%s != %s = %i\n", sop1.c_str(), sop2.c_str(), cmp_op);
		}

		op1 = tmp1;
		op2 = tmp2;
		test1 = test1_tmp;
		test2 = test2_tmp;

		if (s1 && s2) {
			full_sign_ext(test1, op1.getWidth());
			full_sign_ext(test2, op2.getWidth());
			test3 = (long)test1 > (long)test2;
		} else {
			test3 = test1 > test2;
		}

		cmp_op = op1 > op2;

		if (cmp_op != test3) {
      total_cmp++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "GREATER FAILED:\n%lx > %lx = %lx\n", test1, test2, test3);
			sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
			fprintf(stdout, "%s > %s = %i\n", sop1.c_str(), sop2.c_str(), cmp_op);
		}

		op1 = tmp1;
		op2 = tmp2;
		test1 = test1_tmp;
		test2 = test2_tmp;

		if (s1 && s2) {
			full_sign_ext(test1, op1.getWidth());
			full_sign_ext(test2, op2.getWidth());
			test3 = (long)test1 >= (long)test2;
		} else {
			test3 = test1 >= test2;
		}

		cmp_op = op1 >= op2;

		if (cmp_op != test3) {
      total_cmp++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "GREATER OR EQ FAILED:\n%lx >= %lx = %lx\n", test1, test2, test3);
			sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
			fprintf(stdout, "%s >= %s = %i\n", sop1.c_str(), sop2.c_str(), cmp_op);
		}
		op1 = tmp1;
		op2 = tmp2;
		test1 = test1_tmp;
		test2 = test2_tmp;

		if (s1 && s2) {
			full_sign_ext(test1, op1.getWidth());
			full_sign_ext(test2, op2.getWidth());
			test3 = (long)test1 < (long)test2;
		} else {
			test3 = test1 < test2;
		}

		cmp_op = op1 < op2;

		if (cmp_op != test3) {
      total_cmp++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "LESS FAILED:\n%lx < %lx = %lx\n", test1, test2, test3);
			sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
			fprintf(stdout, "%s < %s = %i\n", sop1.c_str(), sop2.c_str(), cmp_op);
		}

		op1 = tmp1;
		op2 = tmp2;
		test1 = test1_tmp;
		test2 = test2_tmp;

		if (s1 && s2) {
			full_sign_ext(test1, op1.getWidth());
			full_sign_ext(test2, op2.getWidth());
			test3 = (long)test1 <= (long)test2;
		} else {
			test3 = test1 <= test2;
		}

		cmp_op = op1 <= op2;

		if (cmp_op != test3) {
      total_cmp++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "LESS OR EQ FAILED:\n%lx <= %lx = %lx\n", test1, test2, test3);
			sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
			fprintf(stdout, "%s <= %s = %i\n", sop1.c_str(), sop2.c_str(), cmp_op);
		}

		/*
				SHIFT TEST
		*/
		op1 = tmp1;
		op2 = tmp2;
		test1 = test1_tmp;
		test2 = test2_tmp & 0x3F;
		shift_op = test2;

		if (s1) {
      op1.toSigned();
			full_sign_ext(test1, op1.getWidth());
		} else {
			zero_clear(test1, op1.getWidth());
		}

		test3 = (long)test1 >> test2;
		op3 = op1 >> shift_op;

		if (d1) { zero_clear(test3, op3.getWidth()); }
		if (s1) { test3 = sign_ext(test3, op3.getWidth()); }

		if (op3 != test3) {
      total_shi++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x %x %x\n", t, s1, s2, d1, d2, rwn1, rwn2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "RIGHT SHIFT FAILED:\n%lx >> %lx = %lx\n", test1, test2, test3);
			sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
			fprintf(stdout, "%s >> %x = %s\n", sop1.c_str(), shift_op, sop3.c_str());
		}

		op1 = tmp1;
		op1.setWidth(BIGINT_BITS_ONE);

		test1 = test1_tmp & 0xFF;
		test2 = test2_tmp & 0x1F;
		shift_op = test2;

		if (s1) {
      op1.toSigned();
			full_sign_ext(test1, op1.getWidth());
		} else {
			zero_clear(test1, op1.getWidth());
		}

		test3 = (long)test1 << test2;
		op3 = op1 << shift_op;

		if (op3 != test3) {
      total_shi++;
			fprintf(stdout, "===========================\n");
			fprintf(stdout, "TEST %d : %x %x %x %x 1 1\n", t, s1, s2, d1, d2);
			fprintf(stdout, "OP1: %lx OP2: %lx\n", test1, test2);
			fprintf(stdout, "LEFT SHIFT FAILED:\n%lx << %lx = %lx\n", test1, test2, test3);
			sop1 = op1.sPrint();sop2 = op2.sPrint();sop3 = op3.sPrint();
			fprintf(stdout, "%s << %x = %s\n", sop1.c_str(), shift_op, sop3.c_str());
		}
	

	}
	fprintf(stdout, "\n");
#endif
  fprintf(stdout, "Total number of tests: %u; \nThe number of failed tests: \n", total);
  fprintf(stdout, "TOTAL ADD ERROR: %i\n", total_add);
  fprintf(stdout, "TOTAL SUB ERROR: %i\n", total_sub);
  fprintf(stdout, "TOTAL MUL ERROR: %i\n", total_mul);
  fprintf(stdout, "TOTAL DIV ERROR: %i\n", total_div);
  fprintf(stdout, "TOTAL MOD ERROR: %i\n", total_mod);
  fprintf(stdout, "TOTAL CMP ERROR: %i\n", total_cmp);
  fprintf(stdout, "TOTAL LOG ERROR: %i\n", total_log);
  fprintf(stdout, "TOTAL SHI ERROR: %i\n", total_shi);
	return 0;
}
