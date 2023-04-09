// testovaci skript pro smycku while
// Jsou zde predstaveny celkem tri smycky.
// Z toho je ta treti zanorena do te druhe

int number1 = 0;
int number2 = 0;

println("WHILE 1");
print('\n');

int a = 5;
int b = 0;

while(a - b>0)
{
	b++;
	print('\n');
	println("  - break");

}

while(1)
{

	if(number1 == 5)
	{
		print('\n');
		println("  - break");

		break;

	}elif(number1 == 2)
	{
		print('\n');
		println("  - continue");
		print('\n');

		number1++;

		continue;

	}

	print("  iteration = ");
	println(number1);

	number1++;
}

print('\n');
println(strcat(number1, " iterations..."));
println("---------------");
println("");

// Dve smycky, jedna zanorena do druhe.
// Prvni odecita od 2, druha scita od 0 do 2

number1 = 2;

println("WHILE 2");
print('\n');

while(number1)
{

	print("  iteration = ");
	println(number1);

	number2 = 0;

	while(number2 <= 2)
	{
		print("   - inner loop ");
		println(number2);
		number2++;
	}

	print('\n');

	number1--;
}

print('\n');
println(strcat(number1, " remaining iterations..."));
println("---------------");

println("SCRIPT END");
