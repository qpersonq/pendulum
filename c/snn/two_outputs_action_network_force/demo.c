
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define randomdef		((double) rand() / (double)(RAND_MAX))

#define BETA		0.2      // Learning rate for action weights, a. 
#define BETAH   	0.05     // Learning rate for action weights, b, c.
#define RHO     	1.0      // Learning rate for critic weights, d. 
#define RHOH   		0.2      // Learning rate for critic weights, e, f.
#define GAMMA   	0.9      // ratio of current prediction, v
#define TIMESTEP	0.01
#define TAU     	0.02 // 141 steps, fmax = 1
//TAU     = 0.02; // 1091 steps, fmax = 600

// Parameters for cartpole simulation
#define g		9.8 //Gravity
#define Mass_Cart	1.0 //Mass of the cart is assumed to be 1Kg
#define Mass_Pole 	0.1 //Mass of the pole is assumed to be 0.1Kg
#define Total_Mass 	Mass_Cart+Mass_Pole
#define Length		0.5 //Half of the length of the pole
#define PoleMass_Length	Mass_Pole*Length
#define FORCE_MAX	600 // max force 
#define Fourthirds	1.3333333

int MAX_FAILURES  =  10000;      // Termination criterion for unquantized version. 
// MAX_STEPS   =     100000;
int MAX_STEPS   =     800;
int PAST_STEPS    = 1000;

//logfile = disp(['fmax600_tau' mat2str(TAU) '_st' mat2str(TIMESTEP) '_max' int2str(MAX_STEPS) '.log'])

double MAX_POS = 2.4;
double MAX_VEL = 1.5;
double MAX_ANGLE = 0.2094;
double MAX_ANGVEL = 2.01;

int FinalMaxSteps = 0;
int total = 10;
int bal = 0;
int balanced = 0;
int failure = -1;

double a[5][5], b[5], c[5], d[5][5], e[5][2], f[5][2];
double x[5], xold[5], y[5], yold[5], v, vold, z[5], p[2];
double r_hat, push, unusualness, sum_error = 0.0;
double h, hdot, theta, thetadot;
int bp_flag = 0, count_error = 0, total_count = 0;

/*** Prototypes ***/
void cartpole_snn();
int cartpole(double x, double xdot, double theta, double thetadot, double force);
void initState();
void initWeights();
void updateWeights();
void setInputValues(double h, double hdot, double theta, double thetadot);
void evalForward();
void actionForward();
double getForce(int push, double t);

int main() {
	#pragma omp parallel
	{
		printf("demo\n");
	}

	//tic

	// save statistics in log files
	// record videos
	int i;
	//#pragma omp parallel for
	for (i = 0; i < total; i ++) {
		// write to file
		printf("Run %d: ", i + 1);
		cartpole_snn();

		if (balanced) {
	        	bal = bal + 1;
	        	printf("Balanced = %d", bal);
		}
		printf("\n");
	}

	//toc

	// report.m
	printf("Final Max Steps: %d\n", FinalMaxSteps);
	printf("Success rate: %f percent (%d/%d)\n", 100.0*bal/total, bal, total);

	return EXIT_SUCCESS;
}

void cartpole_snn() {
	//function Cart_Pole_NN
	// Two-layer neural network: action network and evaluation network
	// network architecture: 5 x 5 x 2, 5 x 5 x 1
	int plot = 1;   //// boolean for plotting. 1: plot, 0: no plot

	int steps = 0, actualMaxSteps = 0, totalSteps = 0;
	int failures=0, lspikes = 0, rspikes = 0, spikes = 0;

	//global grafica
	//grafica = false; // indicates if display the graphical interface
	//xpoints = []; ypoints = [];

	// Initialize action and heuristic critic weights and traces
	initWeights();

	// Starting state is (0 0 0 0)
	initState();

	// Find box in state space containing start state
	setInputValues(h, hdot, theta, thetadot);

	/*
	// Turning on the double buffering to plot the cart and pole
	if plot 
	    handle = figure(1);
	    set(handle,'doublebuffer','on')
	end

	tStart = tic;
	*/
	// state evaluation
	evalForward();

	int right, left, push, i, k;
	double q, pp, fsum, rhat;
	// Iterate through the action-learn loop. 
	while (steps < MAX_STEPS && failures < MAX_FAILURES) {
	//     if steps == 100
	//         grafica = false;
	//     end
    
	/*
    	// Plot the cart and pole with the x and theta
    	if grafica
    	    plot_Cart_Pole(h,theta)
    	end
  	*/  
    	//Choose action randomly, biased by current weight. 
    		actionForward();
    
    		if (randomdef <= p[0]) {
    	   		 right = 1; rspikes = rspikes + 1;
    		} else
   	    	 	right = 0;
    
   		if (randomdef <= p[1]) {
        		left = 0; lspikes = lspikes + 1;
		} else
      			left = 1;
   
    		// q = 1.0/0.5/0 best: 586 steps
		if (right == 1 && left == 0) {
		        push = 1;   
		        q = 1.0; pp = p[0];
		} else if (right == 0 && left == 1) {
        		push = -1;  
		        q = 0.5; pp = p[1];
    		} else {
		        push = 0;   
        		q = 0; pp = 0;
		}
		unusualness = q - pp; 
           
//     F(steps) = 0;
   		fsum = 0;
    		for (k = 1; k < steps; k++) 
		        fsum = fsum + getForce(push, (steps - k)*TIMESTEP);
//     push = F(steps);
		push = fsum;
	//     fprintf('//d: //f\n', steps, push);
	     	printf("%d: push %d\n", steps, push);

		//Preserve current activities in evaluation network
		// Remember prediction of failure for current state
    		vold = v;
    		// remember inputs and hidden unit values
		for (i = 1; i < 5; i++) {
			xold[i] = x[i]; // state variables
		        yold[i] = y[i]; // hidden units
    		}
    
    		//Apply action to the simulated cart-pole: failure = r
        	cartpole(h,hdot,theta,thetadot, push);
       
    		setInputValues(h, hdot, theta, thetadot);

    		// state evaluation
		evalForward(x, a, b, c);
    
    		if (failure < 0) { // r = -1, Failure occurred
	    		failures=failures+1;
		        printf("Episode %d: steps %d\n", failures, steps);
                       
        //[xpoints, ypoints] = plot_xy(xpoints, ypoints, failures, steps);
        
	        	steps = 0;
        
        		//Reset state to (0 0 0 0).  Find the box. ---
		   	initState();

        		//Reinforcement upon failure is -1. Prediction of failure is 0.
        		rhat = failure - vold;
        		failure = 0;      
    		} else {  // r = 0, Not a failure.
        		//Reinforcement is 0. Prediction of failure given by v weight.
        		steps=steps+1;

        		//Heuristic reinforcement is:   current reinforcement
		        //     + gamma * new failure prediction - previous failure prediction
		        rhat = failure + GAMMA * v - vold;
		}
        
    		updateWeights (unusualness, xold, yold, a, b, c, d, e, f, z); 
    
		//if (steps > 0.95*MAX_STEPS)
		//        grafica = true;

		if (actualMaxSteps < steps)
        		actualMaxSteps = steps;
    		
    		totalSteps = totalSteps + 1;
	}
	
	if (failures == MAX_FAILURES) {
	    printf("Pole not balanced. Stopping after %d failures \n", failures);
	    balanced = 0;
	} else {
	    printf("Pole balanced successfully for at least %d steps at %d trials\n", steps, failures);
	    balanced = 1;
	}

	printf("Max steps: %d, Total Steps: %d \n", actualMaxSteps, totalSteps);

	if (FinalMaxSteps < actualMaxSteps)
	    FinalMaxSteps = actualMaxSteps;

	//toc(tStart)

	// stats.m
	// firing rates: L, R, all
	double rl = lspikes / totalSteps; // left rate
	double rr = rspikes / totalSteps; // right rate
	double ra = (lspikes + rspikes) / totalSteps; // all rate
	//disp(['Firing rate = ' num2str(ra) ' (L: ' num2str(rl) ', R: ' num2str(rr) ')']);
}

// Cart_Pole: Takes an action (0 or 1) and the current values of the
// four state variables and updates their values by estimating the state
// TAU seconds later.
int cartpole(double x, double xdot, double theta, double thetadot, double force) {
	// Parameters for simulation
	//double Total_Mass=Mass_Cart+Mass_Pole;
//PoleMass_Length=Mass_Pole*Length;

	double temp = (force + PoleMass_Length *thetadot * thetadot * sin(theta))/ Total_Mass;
	double thetaacc = (g * sin(theta) - cos(theta)* temp)/ (Length * (Fourthirds - Mass_Pole * cos(theta) * cos(theta) / Total_Mass));
	double xacc = temp - PoleMass_Length * thetaacc* cos(theta) / Total_Mass;

	// Update the four state variables, using Euler's method.
	x=x+Tau*x_dot;
	x_dot=x_dot+Tau*xacc;
	theta=theta+Tau*thetadot;
	thetadot=thetadot+Tau*thetaacc;

	int failure = 0;
	if (abs(x) > max_pos || abs(theta) > max_angle)
	    failure = -1; //to signal failure 

	return failure;
}

void updateWeights() {
/*
function [a,b,c,d,e,f] = updateWeights(BETA, RHO, BETAH, RHOH, rhat, ...
    unusualness, xold, yold, a, b, c, d, e, f, z)

for i = 1: 5,  
     // for each hidden unit
    factor1 = BETAH * rhat * yold(i) * (1 - yold(i)) * sign(c(i));
    factor2 = RHOH * rhat * z(i) * (1 - z(i)) * unusualness;// * sign(f(i)); // unusualness required, sign(f(i)) optional
    for j = 1:5,    
        // for each weight I-H
        a(i,j) = a(i, j) + factor1 * xold(j); // evaluation network I-H
        d(i,j) = d(i, j) + factor2 * xold(j); // action network I-H
    end
    // for each weight H-O
    b(i) = b(i) + BETA * rhat * xold(i);   // evaluation network I-O
    c(i) = c(i) + BETA * rhat * yold(i);   // evaluation network H-O

    for j = 1:2
        e(i,j) = e(i,j) + RHO * rhat * xold(i) * unusualness;  // action network I-O
        f(i,j) = f(i,j) + RHO * rhat * z(i) * unusualness;  // action network H-O
    end
//     e(i) = e(i) + RHO * rhat * xold(i) * unusualness;  // action network I-O
//     f(i) = f(i) + RHO * rhat * z(i) * unusualness;  // action network H-O
end
*/
}

// Initialize action and heuristic critic weights and traces 
// to random values between -0.1 and 0.1
void initWeights() {
	time_t t;
   
  	/* Intializes random number generator */
	srand((unsigned) time(&t));

	int i, j;
	for (i = 0; i< 5; i++) {
		for (j = 0; j< 5; j++) {
	    	    a[i][j] = randomdef * 0.2 - 0.1; // evaluation network I-H
	    	    d[i][j] = randomdef * 0.2 - 0.1; // action network I-H
	    	}
	//     b(i) = randomdef;   // evaluation network I-O
	    	b[i] = randomdef * 0.2 - 0.1;   // evaluation network I-O
	    	c[i] = randomdef * 0.2 - 0.1;   // evaluation network H-O
		for (j = 0; j< 2; j++) {
	        	e[i][j] = randomdef * 0.2 - 0.1;   // action network I-O
	        	f[i][j] = randomdef * 0.2 - 0.1;   // action network H-O
		}
	}
}

void initState() {
	h         = randomdef * 2 * MAX_POS - MAX_POS;       //cart position, meters
	hdot     = randomdef * 2 * MAX_VEL - MAX_VEL;       //cart velocity
	theta     = randomdef * 2 * MAX_ANGLE - MAX_ANGLE;      // pole angle, radians
	thetadot = randomdef * 2 * MAX_ANGVEL - MAX_ANGVEL;    // pole angular velocity
}

/*
% evaluation network forward function: sigmoid-linear
% x: input layer. 
% y: hidden layer. sigmoid
% v: output layer. linear
% a: I-H synapses
% b: I-O synapses
% c: H-O synapses
*/
void evalForward() {
/*
% output: state evaluation
for i = 1: 5,
    s = 0.0;
    for j = 1:5,
        s = s + a(i,j) * x(j); % I-H * input
    end
    y(i) = sigmoid(s);  % hidden layer
end

s = 0.0;
for i = 1:5,
    s = s + b(i) * x(i) + c(i) * y(i); % I-O * input + H-O * hidden
end

v = s; % output layer
*/
}

/*
% action network forward function: sigmoid-sigmoid
% x: input layer. 
% z: hidden layer. sigmoid
% p: output layer. sigmoid
% d: I-H synapses
% e: I-O synapses
% f: H-O synapses
*/
void actionForward() {
/*
% output: action
for i = 1: 5,
    s = 0.0;
    for j = 1:5,
        s = s + d(i,j) * x(j); % I-H * input
    end
    z(i) = sigmoid(s);  % hidden layer
end

for j = 1:2,
    s = 0.0;
    for i = 1:5,
        s = s + e(i, j) * x(i) + f(i, j) * z(i); % I-O * input + H-O * hidden
    end
    p(j) = sigmoid(s); % output layer
end
*/
}

double sigmoid(double x) {
	return 1.0 / (1.0 + exp(-x));
}

void setInputValues(double h, double hdot, double theta, double thetadot) {
	x[0] = ( h + MAX_POS ) / (2 * MAX_POS);
	x[1] = ( hdot + MAX_VEL ) / (2 * MAX_VEL);
	x[2] = ( theta + MAX_ANGLE ) / (2 * MAX_ANGLE);
	x[3] = ( thetadot + MAX_ANGVEL ) / (2 * MAX_ANGVEL);
	x[4] = 0.5;
}

double getForce(int push, double t) {
	return push * FORCE_MAX * t * exp(-t/TAU);
}
