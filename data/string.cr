// testovaci skript
// datovy typ 'STRING'

string var1;

string var2 = "Hello";

//string var1 = "Hello 2"; // error, duplicate variable 'var1'
var1 = var2;

var2 = "world";

dump(var2);
dump(var1);

println("SCRIPT END");
