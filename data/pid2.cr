// VE VYVOJI!!

float P = 10;
float I = 6.3;
float D = 450;

float current_temp = 0;
float last_regulator_error = 0;
float target_temp = 0;
float Proportional = 0;
float Integral = 0;
float Differential = 0;
float Power = 0;
while(1)
{
	current_temp = (1 - 0.15) * current_temp + 0.15 * GetTemp();
	last_regulator_error = regulator_error;
	regulator_error = target_temp - current_temp;

	Proportional = regulator_error * P;
	Integral = Integral + (regulator_error + last_regulator_error) / 2;
	if(Integral > 1000)
	{
		Integral = 999; // max ?
	}
	Differential = D * (regulator_error - last_regulator_error);

	Power = Proportional + (Integral * ( 1 / I)) + Differential;
	if(Power > 1000)
	{
		Power = 999; // max power
	}

	setPwm(Power);
}

