#include "../Libraries/stack.h"
#line 3

const int poison = -666;
const int max_value = 2147483637;
const long long canary = 0x2237EA177;
const int canary_size = 4; 
const int decrement = 5;

#define ASSERT_OK(stack)					\
do {										\
	error = stack->verification();			\
											\
	if(error != 0){							\
											\
		printf("Verification failed"		\
			" on %d string\n", __LINE__);	\
											\
		stack->dump();						\
	}										\
} while(0);									\

#define case_of_switch(enum_const) 			\
do {										\
											\
	case enum_const:						\
	return #enum_const;						\
											\
} while(0);									\

const char* test_error(int error){

	switch(error){

		case 0: return "OK";

		case_of_switch(w_value_cap)
		case_of_switch(w_value_size)
		case_of_switch(size_lrg_cap)
		case_of_switch(w_value_chop1)
		case_of_switch(w_value_chop2)
		case_of_switch(null_ptr)
		case_of_switch(w_value_canary1)
		case_of_switch(w_value_canary2)
		case_of_switch(w_val_hsh_sm)

		default: return "Unexpected case";
	}
}

Stack::Stack():
	chop1_(canary),
	capacity_(10),
	size_(0),
	elem_ (new int[10 + canary_size]),
	hash_summ_(0),
	chop2_(canary)
{
	time("w");

	memcpy(elem_,                             &canary, sizeof(long long));
	memcpy(elem_ + capacity_ + canary_size/2, &canary, sizeof(long long));

 	for(int i = 2; i < capacity_ + canary_size/2; i++){
		elem_[i] = poison;
	}

	int error = 0;
	ASSERT_OK(this)
}

Stack::Stack(int size):
	chop1_(canary),
	capacity_(size),
	size_(0),
	elem_ (new int[size + canary_size]),
	hash_summ_(0),
	chop2_(canary)
{
	time("w");

	memcpy(elem_,                             &canary, sizeof(long long));
	memcpy(elem_ + capacity_ + canary_size/2, &canary, sizeof(long long));

 	for(int i = 2; i < capacity_ + canary_size/2; i++){
		elem_[i] = poison;
	}
 
	int error = 0;
	ASSERT_OK(this)
}

void Stack::push(int elem){
	
	int error = 0;
	ASSERT_OK(this)

	if(size_ > capacity_ - 5){
		cap_change();
	}

	elem_[size_ + canary_size/2] = elem;
	size_++;
	hash_summ_ += elem*((canary + size_)%7)*size_;

	ASSERT_OK(this)  
}

int Stack::pop(){
	
	int error = 0;
	ASSERT_OK(this)
	
	int x = elem_[size_ + canary_size/4];

	hash_summ_ -= x*((canary + size_)%7)*(size_);
	elem_[size_ + canary_size/2 - 1] = poison;
	size_--;

	ASSERT_OK(this)

	return x;
}

int Stack::verification(){
	
	long long summ = 0;

    for (int i = 1; i <= size_; i++){
    	summ += elem_[i + canary_size/4]*((canary + i)%7)*i;
    }

	if(capacity_ <= 0 || capacity_ >= max_value){

		printf("%s\n", test_error(w_value_cap));
		return w_value_cap;

	} else if(size_ < 0 || size_ >= max_value){
		
		printf("%s\n", test_error(w_value_size));
		return w_value_size;

	} else if(size_ >= capacity_){

		printf("%s\n", test_error(size_lrg_cap));
		return size_lrg_cap;

	} else if(chop1_ != canary){

		printf("%s\n", test_error(w_value_chop1));
		return w_value_chop1;

	} else if(chop2_ != canary){

		printf("%s\n", test_error(w_value_chop2));
		return w_value_chop2;

	} else if(elem_ == NULL){

		printf("%s\n", test_error(null_ptr));
		return null_ptr;

	} else if(((long long*) elem_)[0] != canary){
		
		printf("%s\n", test_error(w_value_canary1));
		return w_value_canary1;

	} else if(*((long long*)(elem_ + capacity_ + canary_size/2)) != canary){

		printf("%s\n", test_error(w_value_canary2));
		return w_value_canary2;

	} else if(summ != hash_summ_){

		printf("%s\n", test_error(w_val_hsh_sm));
		return w_val_hsh_sm;
	}	

	return stack_ok;
}

void Stack::dump(){

	FILE* file = fopen("STACK_DUMP.log", "a");
	assert(file);

	fprintf(file, "\t\tSTACK DUMP\n");	
	
	fprintf(file, "Size: %d\nCapacity: %d\n", size_, capacity_);
	
	fprintf(file, "Hash summ: %lld\n", hash_summ_);
	
	fprintf(file, "Poison: %d\n", poison);

	fprintf(file, "Canary: %lld\n", canary);

	fprintf(file, "Stack Pointer: %p\n", this);

	fprintf(file, "Stack elements:\n\n");

	if(elem_ != NULL){ 

		fprintf(file, "\tstack.canary = %lld\n", *((long long*)elem_));

		for (int i = canary_size/2; i < capacity_ + canary_size/2; i++){

			fprintf(file, "\tstack.elem[%d] = %d\n", i - 2, elem_[i]);
		}

		fprintf(file, "\tstack.canary = %lld\n", *((long long*)(elem_ + capacity_ + canary_size/2)));
	}	

	fclose(file);
}

void Stack::cap_change(){
	
	int error;
	ASSERT_OK(this)

	int* tmp = NULL;

	if(size_ > capacity_ - 5){
		capacity_ += decrement;
		tmp = new int[capacity_ + canary_size];
	} else {
		return;
	}

	if(tmp != NULL){
		
		for (int i = 0; i < capacity_ - 5; i++){
			tmp[i] = elem_[i];
		}

		delete[] (elem_);

		elem_ = tmp;

		memcpy(elem_ + capacity_ + canary_size/2, &canary, sizeof(long long));
		
		for(int i = size_ + canary_size/2; i < capacity_ + canary_size/2; i++){
			elem_[i] = poison;
		}

	} else {
		printf("Failed to reallocate\n");
	}

}

Stack::~Stack(){

	int error = 0;
	ASSERT_OK(this)
	
	for(int i = 0; i < capacity_ + canary_size; i++){
		elem_[i] = poison;
	}

	chop1_    = poison;
	size_     = poison;
	capacity_ = poison;
	chop2_    = poison;
	delete[]    (elem_);

	time("a");
}

int& Stack::operator[](int number){
	assert( 0 <= number && number < size_);
	return elem_[number + canary_size/2];
}

const int& Stack::operator[](int number) const{
	assert( 0 <= number && number < size_);
	return elem_[number + canary_size/2];
}

void time(const char* mode){

	FILE* file = fopen("STACK_DUMP.log", mode);
	assert(file);

	time_t rawtime;
	
	struct tm * timeinfo;
	
	time (&rawtime);
	
	timeinfo = localtime (&rawtime);
	
	fprintf(file, "\n%s\n", asctime(timeinfo));

	if(!strcmp(mode, "w")){

		fprintf(file, "Stack was created.\n\n");
	
	} else if(!strcmp(mode, "a")) {

		fprintf(file, "Stack was deleted.\n");	
	}

	fclose(file);
}

int Stack::get_size(){
	return size_;
}

int Stack::get_cap(){
	return capacity_;
}
