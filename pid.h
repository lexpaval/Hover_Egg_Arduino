// pid.h
#ifdef __cplusplus
extern "C"{
#endif

	#define OUTMAX 255
	#define OUTMIN 0

	typedef struct _PID
	{
		double windup_guard;
		double proportional_gain;
		double integral_gain;
		double derivative_gain;
		double prev_error;
		double int_error;
		double control;
	}PID;
	// struct that contains all the variables needed for PID implementation

	void pid_zeroize(PID* pid);
	// set prev and integrated error to zero - use when dynamically changing setpoints

	void pid_update(PID* pid, double curr_error, double dt);
	// actual implementation of the PID with windup guard

#ifdef __cplusplus
} // extern "C"
#endif