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
		print("delay: ");
		print(cast("string", ms));
		println(".");
		if (ms == 0) {
			m += 32;
			return m;
		}
		ms--;
	}
}

int r2 = delay2(2);

int r = sum(1, 2);

dump (r);

r = sum(20, 5);

dump (r);
dump (r2);

println("SCRIPT END");
