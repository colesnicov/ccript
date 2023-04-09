// testovaci skript
// datovy typ 'INT'

int var1;
int var2 = 15;

var1 = var2;

var1 = 10 + 10;
var2 = var1 + (20 / 10);

var1 *= 2;
var2 += 10;

var1 *= 2 + 2;
var2 += var1 - 1;

var2++;
--var2;

var1 = 9 + 0 + 9 + 0 + (9 * 0) + 9;

{
	int var3 = var1;
}

dump (var1);

delete var1;

int var1 = 9;

dump (var2);
dump (var1);

println("SCRIPT END");
