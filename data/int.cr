// testovaci skript
// datovy typ 'INT'

int var1;

int var2 = 15;

//int var1 = 2; // error, duplicate variable 'var1'
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

//var1 == var2;
//var1 < var2;
//var2 >= 10.5;


{
	int var3 = var1;
}
//var3 += 12.3; // error, undefined variable 'var2'

delete var1;

int var1 = 9; // ok

println("SCRIPT END");
