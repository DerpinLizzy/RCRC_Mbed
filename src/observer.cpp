#include "observer.h"

observer::observer(float Ts)   // constructor
{
    this->Ts = Ts;
    init_matrices();
}

observer::~observer() {}

// init the A, B, C, H Matrices
void observer::init_matrices()
{
    A << -905.387, 452.694,452.694, -452.694;
    B << 452.694, 0.000;
    H << 6203.968, 1641.919;
    C << 0,1;
}

// calculate the observed states -> calc sum, integrate, store in x_hat
void observer::do_step(float u,float y_meas)
{
    dxdt = A*x_hat + B*u + H * (y_meas - C * x_hat);
    integrate_states();
}

// to the integration dxdt -> x, use trapezoidal form
void observer::integrate_states()
{
    x_hat += Ts * dxdt;
}

// get the observed states
Matrix<float,N,1> observer::get_x_obsv()
{
    return x_hat;
}