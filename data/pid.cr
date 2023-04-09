// testovaci skript
// datovy typ 'LONG'
// VE VYVOJI!!

float epsilon = 0.01;
//100ms loop time
float dt = 0.01;
//For Current Saturation
float MAX = 4;
float MIN = -4;
float Kp = 0.1;
float Kd = 0.01;
float Ki = 0.005;
float pre_error = 0;

float PIDcal(float setpos, float pos)
{
	float integral = 0;
	float error;
	float derivative;
	float output;

// Caculate P,I,D
	error = setpos - pos;

// In case of error too small then stop intergration
	if (abs(error) > epsilon)
	{
		integral = integral + error * dt;
	}
	derivative = (error - pre_error) / dt;
	output = Kp * error + Ki * integral + Kd * derivative;

// Saturation Filter
	if (output > MAX)
	{
		output = MAX;
	}
	elif(output < MIN)
	{
		output = MIN;
	}

// Update error
	pre_error = error;

	return output;
}

float deg = env("ang");
float set = env("set");

float pid = 0;

while(1)
{
	pid = PIDcal(set, deg);

	print("pid: ");
	println(cast("string", pid));
	print("pre_error: ");
	println(cast("string", pre_error));
	deg--;
	if(!deg)
	{
		break;
	}
	sleep(100);
}

println("SCRIPT END");
