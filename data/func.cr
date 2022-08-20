// testovaci skript
// datovy typ 'LONG'

int sum(int a, int b) {
	int res = a + b;

	return res;
}

// neprijima long argument
// kontrolovat jaky typ prijima f-ce a podle toho parsovat...
int delay2(int ms) {
	int m = ms;
	while (true) {
		println(ms);
		if (ms == 0) {
			m += 32;
			return m;
		}
		ms--;
	}

	return 10;
}

int r2 = delay2(2);

int r = sum(1, 2);

int var1 = 20;

long var2 = 15;

r = sum(var1, 5);

//int var1 = 2; // error, duplicate variable 'var1'
//var1 = var2;

var1 = 10 + 10;
//var2 = var1 + (20 / 10);

var1 *= 2;
var2 += 10;

var1 *= 2 + 2;
//var2 += var1 - 1;

var2++;
--var2;

var1 = 9 + 0 + 9 + 0 + (9 * 0) + 9;

//var1 == var2;
//var1 < var2;
//var2 >= 10.5;

{
	long var3 = var2;
}
//var3 += 12.3; // error, undefined variable 'var2'

dump (var1);

delete var1;

long var1 = -9223372036854775807; // ok

dump (var2);
dump (var1);
dump (r);
dump (r2);

println("SCRIPT END");
