#include <chrono>
#include <cstdint>
#include "realtime_thread.h"
#include "DataLogger.h"
#include "GPA.h"

extern DataLogger myDataLogger;
extern GPA myGPA;
using namespace Eigen;
using namespace std::chrono;
// contructor for realtime_thread loop
realtime_thread::realtime_thread(IO_handler *io,float Ts) : thread(osPriorityHigh1, 1024)
{
  this->Ts = Ts;        // the sampling time
  this->m_io = io;      // a pointer to the inputs/outputs
  ti.reset();
  ti.start();
  obs = observer(Ts);
}

// decontructor for controller loop
realtime_thread::~realtime_thread() {}

// ----------------------------------------------------------------------------
// this is the main loop called every Ts with high priority
void realtime_thread::loop(void)
{
  float tim,w,V,u,x1,x2,y;
  Matrix<float,1,2> K2(3.3092, 2.0939);
  Vector2f x;
  V = 6.4031;

  while (1)
    {
    ThisThread::flags_wait_any(threadFlag);
    tim = 1e-6*(duration_cast<microseconds>(ti.elapsed_time()).count());
// --------------------- THE LOOP -----------------------------------------
    w = myDataLogger.get_set_value(tim);    // get set values from datalogger
    
    // AUFGABE 1.4
    x << m_io->read_ain1(), m_io->read_ain2();
    // u_out = V * w - K2*x;

    // Aufgabe 2.6
    obs.do_step(u_out, x(1));
    u_out = saturate(V * w - K2*obs.get_x_obsv());
    
    m_io->write_aout(u_out);
    
    myDataLogger.write_to_log(tim, u_out, obs.get_x_obsv()(0), x(0));
    // myDataLogger.write_to_log(tim, u_out, obs.get_x_obsv()(1), x(1));
    /* GPA
    u_out = myGPA.update(u_out, m_io->read_ain2()); */

    } // endof the main loop
}

// ----------------------------------------------------------------------------
void realtime_thread::sendSignal() { thread.flags_set(threadFlag); }

void realtime_thread::start_loop(void)
{
  thread.start(callback(this, &realtime_thread::loop));
  ticker.attach(callback(this, &realtime_thread::sendSignal), Ts);
}

float realtime_thread::saturate(float u){
    return u > 1 ? 1 : (u < -1 ? -1 : u);
}