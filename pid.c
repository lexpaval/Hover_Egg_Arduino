#include "pid.h"

void pid_zeroize(PID* pid)
{
	// set prev and integrated error to zero - use when dynamically changing setpoints
	pid->prev_error = 0;
	pid->int_error = 0;
}

void pid_update(PID* pid, double curr_error, double dt)
{
	double diff;
	double p_term;
	double i_term;
	double d_term;

	// integration with windup guarding
	pid->int_error += (curr_error * dt);
	if (pid->int_error < -(pid->windup_guard))
	{
		pid->int_error = -(pid->windup_guard);
	}
	else
	{
		if (pid->int_error > pid->windup_guard)
		{
			pid->int_error = pid->windup_guard;
		}
	}

	// differentiation
	diff = ((curr_error - pid->prev_error) / dt);

	// scaling
	p_term = (pid->proportional_gain * curr_error);
	i_term = (pid->integral_gain     * pid->int_error);
	// set maximum and minimun value for integrator
	if (i_term > OUTMAX) i_term = OUTMAX;
	else if (i_term < OUTMIN) i_term = OUTMIN;

	d_term = (pid->derivative_gain   * diff);

	// summation of terms
	pid->control = p_term + i_term + d_term;
	//set maximum and minimum value for output (control)
	if (pid->control > OUTMAX) pid->control = OUTMAX;
	else if (pid->control < OUTMIN) pid->control = OUTMIN;

	// save current error as previous error for next iteration
	pid->prev_error = curr_error;
}
