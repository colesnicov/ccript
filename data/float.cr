// testovaci skript
// datovy typ 'FLOAT'

float var1;

float var2 = 10.5;

var1 = var2;

var1 = 5.10 + 10;
var2 = var1 + (20 / 0.10);

var1 *= 2;
var2 += 10;

var1 *= 2 + 2;
var2 += var1 - .1;

var2++;
--var2;

var1 = 9.0 + 0.0 + 9.0 + 0.0 + (9.0 * 0.0) + 9.0;

{
	float var3 = var1;
}

dump (var1);

delete var1;

float var1 = 2.9;

dump (var2);
dump (var1);

println("SCRIPT END");
