#include <algorithm>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>

using namespace std;

vector<long double> numbers;
vector<char> operations;

/**
 * Priority of operators is given by this map
 */
unordered_map<char, size_t> priority = {
	{ '+', 0 },
	{ '-', 0 },
	{ '*', 1 },
	{ '/', 1 },
};

/**
 * Perform a single operation on two numbers, depending on operator
 */
template<typename T>
T perform(T a, T b, char o) {
	switch(o) {
		case '+':
//		cout << a + b << ' ';
			return a + b;
		case '-':
//		cout << a - b << ' ';
			return a - b;
		case '*':
//		cout << a * b << ' ';
			return a * b;
		case '/':
//		cout << a / b << ' ';
			return a / b;
		default: /* never reaches */
			return 0;
	}
}

/**
 * Performs the operation on two numbers, based on info in stack
 */
template<typename T>
void perform_single_operation(stack<T>& nums, stack<size_t>& operators) {
	T a, b;
	a = nums.top(); nums.pop();
	b = nums.top(); nums.pop();
	nums.push(perform<T>(b, a, operations[operators.top()]));
	operators.pop();
}

/**
 * Perform all operations on a given possible configuration of parenthesis
 */
template<typename T>
T perform_all_operations(vector<size_t>& possible_operations, vector<size_t>& parenthesis_index) {
	stack<T> nums;
	stack<size_t> operators;
	size_t operators_size_at_opening_parenthesis = 0;

	nums.push(numbers[0]);
	for (size_t i = 0; i < possible_operations.size(); ++i) {
		if (i == parenthesis_index[0]) { operators_size_at_opening_parenthesis = operators.size(); }
		else if ((operators.size() > operators_size_at_opening_parenthesis) && !(priority[operations[operators.top()]] < priority[operations[possible_operations[i]]])) {
			perform_single_operation<T>(nums, operators);
		}

		nums.push(numbers[i + 1]);
		operators.push(possible_operations[i]);

		if (i == parenthesis_index[1]) {
			while (operators.size() > operators_size_at_opening_parenthesis) {
				perform_single_operation<T>(nums, operators);
			}
			operators_size_at_opening_parenthesis = 0;
		}
	}
	while (operators.size() > operators_size_at_opening_parenthesis) {
		perform_single_operation<T>(nums, operators);
	}

	return nums.top();
}

/**
 * Returns true if the obtained final value of operation is equal to given 'k'
 */
template<typename T>
bool evaluate(vector<size_t>& possible_operations, vector<size_t>& parenthesis_index, T& k) {
	T value = perform_all_operations<T>(possible_operations, parenthesis_index);

	if (value == k) {
		return true;
	}
	return false;
}

/**
 * Update the possible operations that can be performed to get 'k'
 */
bool update_possible_operations(vector<size_t>& possible_operations) {
	bool is_updatable = false;
	size_t last_updatable_index;
	for (size_t i = 0; i < numbers.size() - 1; ++i) {
		if (possible_operations[i] != operations.size() - 1) {
			last_updatable_index = i;
			is_updatable = true;
		}
	}
	if (is_updatable) {
		++possible_operations[last_updatable_index];
		for (size_t i = last_updatable_index + 1; i < numbers.size() - 1; ++i) {
			possible_operations[i] = 0;
		}
	}
	return is_updatable;
}

/**
 * Updates the position of parenthesis to be evaluated
 */
bool update_parenthesis_index(vector<size_t>& parenthesis_index) {
	size_t diff = parenthesis_index[1] - parenthesis_index[0];
	if (diff == numbers.size() - 2)
		return false;
	else if (parenthesis_index[1] == numbers.size() - 2) {
		parenthesis_index[0] = 0, parenthesis_index[1] = diff + 1;
	} else {
		++parenthesis_index[0],
		++parenthesis_index[1];
	}
	return true;
}

/**
 * Computes all the possible values of a possible permutation of given input
 * returns true if found, else returns false
 */
template<typename T>
bool compute(vector<size_t>& possible_operations, vector<size_t>& parenthesis_index, T& k) {
	do {
		parenthesis_index[0] = 0, parenthesis_index[1] = 0;
		do {
			bool possible = evaluate<T>(possible_operations, parenthesis_index, k);
			if (possible) {
				return true;
			}
		} while (update_parenthesis_index(parenthesis_index));
	} while (update_possible_operations(possible_operations));
	return false;
}

/**
 * Evaluates if it is possible to get the required 'k' using any possible
 * combination of input numbers and operations
 */
template<typename T>
void solve_for_k(T& k) {
	bool found;
	vector<size_t> possible_operations(numbers.size() - 1), parenthesis_index(2);
	sort(numbers.begin(), numbers.end());

	do {
		for (size_t i = 0; i < possible_operations.size(); ++i) {
			possible_operations[i] = 0;
		}
		cout << "Computing ";
		for (size_t i = 0; i < numbers.size(); ++i) {
			cout << numbers[i] << ' ';
		}
		cout << endl;

		found = compute<T>(possible_operations, parenthesis_index, k);
		if (found) {
			cout << (parenthesis_index[0] == 0 ? " ( " : " ") << numbers[0] << ' ';
			for (size_t i = 0; i < possible_operations.size(); ++i) {
				cout << operations[possible_operations[i]]
					<< (parenthesis_index[0] == i + 1 ? " ( " : " ")
					<< numbers[i + 1]
					<< (parenthesis_index[1] == i ? " ) " : " ");
			}
			cout << "= " << k << endl;
			break;
		}
	} while (next_permutation(numbers.begin(), numbers.end()));
	if (!found) {
		cout << "No solution found!" << endl;
	}
	possible_operations.clear();
	parenthesis_index.clear();
}

int main() {
	size_t n, m;
	// n: total numbers to input
	// m: total operations to use
	char operation;
	long double number, k;
	cout << "Enter total numbers to equate to k: ";
	cin >> n;
	cout << "Enter total numbers of operations available: ";
	cin >> m;
	cout << "Enter the value of k: ";
	cin >> k;
	numbers.reserve(n);
	operations.reserve(m);

	for (int i = 1; i <= n; ++i) {
		cout << "Enter number " << i << ": ";
		cin >> number;
		numbers.push_back(number);
	}
	for (int i = 1; i <= m; ++i) {
		cout << "Enter operation " << i << ": ";
		cin >> operation;
		operations.push_back(operation);
	}
	cout << "solving ..." << endl;

	solve_for_k<long double>(k);
	numbers.clear();
	operations.clear();
	return 0;
}
