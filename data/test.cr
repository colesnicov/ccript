float float1;
float float2 = 22.2;
float1 = float2;
float2 = float1-10;
float1 += 5;

int int1;
int int2 = 22;
int1 = int2;
int2 = int1 - 10;
int1 += 5;
--int1;
int1++;

println(12.34);
println(12.007);
println(true);
println('D');
println("hello");
println(4);

{

	int ii = 0;

	while(true) {
		println("while 1");
		int ii1 = 0;
		{
			int ii2 = 0;
			{
				int ii3 = 0;
				{
					int ii4 = 0;
					int ii5 = 0;

					println(ccDebugInfo("parser.depth"));
					println(ccDebugInfo("var.depth", ii1));

					//				break;
					if(ii < 5) {
						{
							int ii15 = 5;
						}
						//					break;
						ii++;
						//					println("CONTINUE I");
					} else {
						println(ii);
						println("BREAK I");
						dump(ii);
						break;
						println("BREAK II");

						//					println("CONTINUE II");
					}
					//				dump(ii15);
				}
			}
		}
		println("CONTINUE");
	}

	// testovaci skript

	{
		//	int ii = 0; 
		// FIXME doladit 'scope' promenne muzou mit stejne nazvy v ramci ruznych 'scope'?

		int ii2 = 0;

		while(true) {
			println("while 2");
			{
				ii++;
				ii2++;
				dump(ii);
			}
			if(ii2 == 2) {
				int ii22 = 0;
				println("CONTINUE VI");
				continue;
				println("CONTINUE II");
			}
			if(ii>12) {
				println("BREAK I");
				break;
				println("BREAK II");
			} else {
				println("CONTINUE I");
				//			continue;
				//			println("CONTINUE II");
			}
			//		ii++;
			dump(ii);
			println("CONTINUE");
		}

	}
	// komentar

	bool bl = true;

	if(strlen("asa") >= 6) {
		println("prosel -3");
	} else {
		println("neprosel -3");
	}

	if(5 < 6 && bl) {
		println("prosel -2");
	} else {
		println("neprosel -2");
	}

	if(bl && 5 == 5) {
		println("prosel -1");
	} else {
		println("neprosel -1");
	}

	if(5 >= 6) {
		println("prosel 0");
	} else {
		println("neprosel 0");
	}

	if(5 < 6) {
		println("prosel 1");
	} else {
		println("neprosel 1");
	}

	if(bl != bl) {
		println("prosel 2");
	} else {
		println("neprosel 2");
	}

	if(bl == false) {
		println("prosel 3");
	}

	if(bl) {
		println("prosel 4");
	}

	println(castTo("int", 'W'));
	println(castTo("char", 87));

	bool bl0;
	bl0=false;

	bool bl1 = true;
	bool bl3 = bl1 & (false | true);

	if(bl1 && bl0==true) {
		println("if OK");
		println(castTo("int", "20"));
	}elif(bl0==false ) {
		println("elif OK 1");
		println(strcat("ahoj"," \\ ","svete2"));
	}elif(bl1==true) {
		println("elif2 OK 2");
		println(strcat("ahoj"," ","svete3"));
	} else {
		println("else OK");
		println(strcat("ahoj"," \\ ","svete4"));
	}

	println(strcat("ahoj"," ","svete\"", "5"));

	char ch0 = 'B';
	char ch1 = ch0;

	ch0 = 'L';

	string str1;
	str1 = "f BASA";
	string str2 = ch1;

	int number1 = 40;

	println(strlen("ahojky"));
	println(true);

	int number2 = number1 + 10;
	dump (number2);
	number2 = millis();
	number2++;

	--number2;

	float fvar = 10.4 - (2.1 * 2);
	fvar = fvar - 30;
	fvar += 10;
	fvar--;
	++fvar;

	fvar = castTo("float", 343);

	++fvar;
	fvar += 0.10000;

	dump (fvar);

	println(strcat("vars size: ", ccDebugInfo("var.sizeAll"), " bytes."));
	
}

println("SCRIPT END");

