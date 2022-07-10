// testovaci skript pro mereni rychlosti
// Mereni probiha ve smycce
// Pritom dojde k vytvoreni 3 promennych typu int
// Promenne se inkrementuji a odstranise
// Navrat na zacatek smycky i tak na 100 opakovani
// Pak se vypise cas v milisekundach.

// Pozor, protoze typ int nemuze pojmout typ long, funkce millis() vraci overflowed hodnotu.

int limit = 100;
int last = millis();

int number1 = 0;
int number2 = 0;

println("WHILE 1");
print('\n');

while(number1 < limit) {
	int number3 = 0;
	int number4 = 0;
	int number5 = 30;
	number1++;
	number2++;
	number3++;
	number4++;
}

print('\n');
print (number1);
println(" iterations..."); 
println("---------------"); 

print('\n');
println(strcat("millis: ",millis(), ", last: ", last, "."));
number1 = millis() - last;

println(strcat("time: ", number1, " ms."));
println("---------------");
print('\n');

println("SCRIPT END");
