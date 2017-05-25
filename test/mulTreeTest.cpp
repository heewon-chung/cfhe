#include <iostream>
#include <vector>
#include <random>
#include <time.h>

using namespace std;

template<typename T>
T mulTree(vector<T> &inputs)
{
	int original_size = inputs.size();

	if (original_size == 1)
	{
		return inputs[0];
	}

	// multiply two elements and add to the back of the vector
	#pragma omp parallel for
	for (int i = 0; i < inputs.size() - 1; i += 2)
	{
		inputs.emplace_back(inputs[i]*inputs[i + 1]);
	}
	
	T ret = inputs[inputs.size() - 1]; 

	// remove extra elements
	inputs.resize(original_size);

	return ret;
}

template<typename T>
void print_std_vector( vector<T> &in)
{
	for (auto i = in.begin(); i != in.end(); ++i)
	{
		cout << *i << ' ';
	}
	cout << endl;
}

int main()
{
	srand(time(NULL));
	const int n = (rand() % 16) + 1;
	vector<int> in(n);
	
	for (int i = 0; i < n; i++)
		in[i] = (rand() % 32) + 1;
		
	cout << "input vector before multiplication: ";
	print_std_vector<int>( in );
	int prod = mulTree<int>( in );
	cout << "input vector after multiplication: ";
	print_std_vector<int>( in );
	// check 
	int p = 1;
	for (int i = 0; i<n;i++)
		p = p * in[i];
	
	cout << "n: " << n << endl;
	if (prod != p)
		cout << "ERROR:: Multiplication FAILED\n";
	cout << "multiply_many: " << prod << endl;
	cout << "multiply_chk : " << p << endl;
	
	return 0;
}