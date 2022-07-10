// Program to Display Fibonacci Sequence
// Source https://www.programiz.com/c-programming/examples/fibonacci-series

int t1 = 0;
int t2 = 1;
int nextTerm = 0;
int n = 100;

nextTerm = t1 + t2;

print("Fibonacci Series:");

while (nextTerm <= n) {
	print(strcat(" ", nextTerm));
	t1 = t2;
	t2 = nextTerm;
	nextTerm = t1 + t2;
}

println('.');

