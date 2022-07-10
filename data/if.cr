// testovaci skript
// testovani podminky if

int zero = 0;

if(zero == 0) {
	println("(zero == 0) = 1");
}

println("");

if(zero > 12) {
	println("(zero > 12) = 1");
} else {
	println("(zero > 12) = 0");
}

println("");

if(5 >= 6) {
	println("(5 >= 6) = 1");
} else {
	println("(5 >= 6) = 0");
}

println("");

if(5 < 6 && true) {
	println("(5 < 6 && true) = 1");
} else {
	println("(5 < 6 && true) = 0");
}

println("");

if(false && 5 == 5) {
	println("(false && 5 == 5) = 1");
} else {
	println("(false && 5 == 5) = 0");
}

println("");

if(5 < 6) {
	println("(5 < 6) = 1");
} else {
	println("(5 < 6) = 0");
}

println("");

if(true == 1) {
	println("(true == 1) = 1");
} else {
	println("(true == 1) = 0");
}

println("");

if(1.0 == 1) {
	println("(1.0 == 1) = 1");
} else {
	println("(1.0 == 1) = 0");
}

println("");

if(false) {
	println("(false) = 1");
} else {
	println("(false) = 0");
}

println("");

if(true && true < 0) {
	println("(true && true < 0) = 1");
}elif(false == zero ) {
	println("(false == zero) = 1");
}elif(true == true) {
	println("(true == true) = 1");
} else {
	println("(else) = 1");
}

println("");

println("SCRIPT END");
