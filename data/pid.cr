// testovaci skript
// datovy typ 'LONG'

float epsilon = 0.01;
float dt = 0.01;             //100ms loop time
float MAX = 4;                  //For Current Saturation
float MIN = -4;
float Kp = 0.1;
float Kd = 0.01;
float Ki = 0.005;

float PIDcal(float setpoint, float actual_position) {
	float pre_error = 0;
	float integral = 0;
	float error;
	float derivative;
	float output;

	//Caculate P,I,D
	error = setpoint - actual_position;

	//In case of error too small then stop intergration
	if (abs(error) > epsilon) {
		integral = integral + error * dt;
	}
	derivative = (error - pre_error) / dt;
	output = Kp * error + Ki * integral + Kd * derivative;

	//Saturation Filter
	if (output > MAX) {
		output = MAX;
	} else if (output < MIN) {
		output = MIN;
	}
	//Update error
	pre_error = error;

	return output;
}

println("SCRIPT END");
