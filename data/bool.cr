// testovaci skript
// datovy typ 'BOOL'

bool pravdivost1;

bool pravdivost2 = 1;

dump (pravdivost2);

pravdivost1 = pravdivost2;

dump (pravdivost1);

pravdivost1 = true && false;
pravdivost2 = pravdivost1 || true;

pravdivost1 = true && (true || 1);
pravdivost2 = !pravdivost1;

dump (pravdivost2);
dump (pravdivost1);

println("SCRIPT END");
