// testovaci skript
// datovy typ 'LONG'

long var1;

long var2 = 15;

var1 = var2;

var1 = 10 + 10;
var2 = var1 + (20 / 10);

var1 *= 2;
var2 += 10;

var1 *= 2 + 2;
var2 += var1 - 1;

var2++;
var2 = 9223372036854775807;

var1 = 9 + 0 + 9 + 0 + (9 * 0) + 9;

{
	long var3 = var1;
}

dump (var1);
dump (var2);

delete var1;

long var1 = -9223372036854775807;

dump (var2);
dump (var1);

println("SCRIPT END");

