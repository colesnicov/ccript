// testovaci skript
// testovani podminky if

int zero = 0;

float zerof = 100.0;
float in_1 = 1.0;
float data = 60.0;
float data_min = 90.0;

if((0 == in_1))
{
	println("vypnuto 2");
}
else
{
	println("zapnuto 2");
}

if(data_min < data)
{
	println("jasno 2");
}
else
{
	println("tma 2");
}

if((0 == in_1) && data_min > data)
{
	println("vstup 'OUT0' ZAPNUTO");
}
else
{
	println("vstup 'OUT0' zustava");
}

if((0 < in_1) || data_min < data)
{
	println("vstup 'OUT0' zustava");
}
else
{
	println("vstup 'OUT0' Zapnuto");
}

if(zero == 0)
{
	println("(zero == 0) = 1");
}

if(zerof)
{
	println("(zerof == 1.0) = 1");
}

if(1==zerof)
{
	println("(1==zerof) = 1");
}

if(90<zerof)
{
	println("90<(zerof) = 1");
}
else
{
	println("90<(zerof) = 0");
}

if(zero > 12)
{
	println("(zero > 12) = 1");
}
else
{
	println("(zero > 12) = 0");
}

if(5 >= 6)
{
	println("(5 >= 6) = 1");
}
else
{
	println("(5 >= 6) = 0");
}

if((5 < 6) && true)
{
	println("(5 < 6 && true) = 1");
}
else
{
	println("(5 < 6 && true) = 0");
}

if(false && (5 == 5))
{
	println("(false && 5 == 5) = 1");
}
else
{
	println("(false && 5 == 5) = 0");
}

if(5 < 6)
{
	println("(5 < 6) = 1");
}
else
{
	println("(5 < 6) = 0");
}

println("");

if(true == 1)
{
	println("(true == 1) = 1");
}
else
{
	println("(true == 1) = 0");
}

if(1.0 == 1)
{
	println("(1.0 == 1) = 1");
}
else
{
	println("(1.0 == 1) = 0");
}

println("");

if(false)
{
	println("(false) = 1");
}
else
{
	println("(false) = 0");
}

if(true && (true < 0))
{
	println("(true && true < 0) = 1");
}elif(false == zero )
{
	println("(false == zero) = 1");
}elif(true == true)
{
	println("(true == true) = 1");
}
else
{
	println("(else) = 1");
}

println("SCRIPT END");
