// testovaci skript
// datovy typ 'BOOL'

bool var1;

bool var2 = 1;

//bool var1 = 1; // error, duplicate variable 'var1'
var1 = var2;

var1 = true & false;
var2 = var1 | true;

var1 = true & (true | 1);
var2 = !var1;

println("SCRIPT END");
