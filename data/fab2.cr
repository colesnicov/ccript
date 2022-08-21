// fibonacci series
// Two versions

int Fibonacci(int n) {
	if (n == 0) {
		return 0;
	} elif (n == 1) {
		return 1;
	} else {
		int n1 = n - 1;
		int n2 = n - 2;
		int n3 = Fibonacci(n1) + Fibonacci(n2);
		return n3;
	}
}

int Fibonacci2(int n) {
	if (n == 0) {
		return 0;
	} elif (n == 1) {
		return 1;
	} else {
		n -= 1;
		int n2 = n - 1;
		n = Fibonacci2(n) + Fibonacci2(n2);
		return n;
	}
}

int i = 0;
int c = 1;
int n = 5;

println("Fibonacci series");
while(1)
{
	if(c>n) {
		break;
	}

	println(cast("string", Fibonacci(i)));

	c++;
	i++;
}

i = 0;
c = 1;
n=5;

println ("Fibonacci series 2");
while(1)
{
	if(c>n) {
		break;
	}

	println(cast("string", Fibonacci2(i)));

	c++;
	i++;
}
