// testovaci skript pro mereni rychlosti
// Mereni probiha ve dvou smyckach kde jedna je zanorena do druhe.
// Pritom dojde k vytvoreni 2 promennych typu int
// Promenne se inkrementuji a odstrani se
// Obe smycky maji stejny pocet opakovani, 1000.
// Pak se vypise cas v milisekundach.

int limit = 1000;
long last = millis();

int loop_1 = 0;
int loop_2 = 0;
long iterations = 0;

println("START");
print('\n');

while(loop_1 < limit)
{
	while(loop_2 < limit)
	{
		loop_2++;
		iterations++;

		int number4 = 0;
		int number5 = 0;

		number4++;
		number5++;
	}

	loop_1 += 1;
	loop_2 = 0;
	iterations++;

	int number4 = 0;
	int number5 = 0;

	number4++;
	number5++;
}

print('\n');
print(cast("string", iterations));
println(" iterations...");
println("---------------");

print('\n');
println(strcat("millis: ", cast("string", millis()), ", last: ", cast("string", last), "."));
iterations = millis() - last;

println(strcat("time: ", cast("string", iterations), " ms."));
println("---------------");
print('\n');

println("SCRIPT END");
