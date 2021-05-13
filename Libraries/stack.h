#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

class Stack {

	private:

		long long chop1_;
		int capacity_;
		int size_;
		int* elem_;
		long long hash_summ_;
		long long chop2_;

	public:
	
	////The function create stack////
		Stack();
		Stack(int size);
	////The function destruct stack////
		~Stack();
	////The function checks the stack data////
		int verification();
	////The function prints stack data and errors////
		void dump();
	////The function change stack capacity////
		void cap_change();
	////The function puts an element on the stack////
		void push(int elem);
	////The function returns an element from the stack////
		int pop();
	////The function prohibits to copy stack////
		Stack(const Stack& stk) = delete;
	////The function prohibits to use operator = to constant stack////
		const Stack& operator= (const Stack* stk) = delete;
	////The function allows to use [] to stack////
		const int& operator[](int number) const;
			  int& operator[](int number);
	////The function returns stack capacity////
		int get_cap();
		int get_size();
};


///The function prints time in dump file////
void time(const char* mode);

enum error {
	stack_ok,
	w_value_cap,
	w_value_size,
	size_lrg_cap,
	w_value_chop1,
	w_value_chop2,
	null_ptr,
	w_value_canary1,
	w_value_canary2,
	w_val_hsh_sm
};
