// testovaci skript
// datovy typ 'INT'

long var1;

long var2 = 15;

//int var1 = 2; // error, duplicate variable 'var1'
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

//var1 == var2;
//var1 < var2;
//var2 >= 105;

{
	long var3 = var1;
}
//var3 += 123; // error, undefined variable 'var3'

dump (var1);

delete var1;

//var1 = 0; // error, undefined variable 'var1'

long var1 = -9223372036854775807; // ok

dump (var2);
dump (var1);

println("SCRIPT END");
