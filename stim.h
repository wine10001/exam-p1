#ifndef _STIM_H_
#define _STIM_H_

#include <iomanip>
using std::setw;
#include "adder.h"
#include "ready_valid_port.h"

// WIDTH is defined in adder.h
const int DEPTH = (1 << WIDTH);

SC_MODULE(stim) {

  sc_in_clk i_clk;
  sc_out<bool> o_rst;
  //o_a output ports: message, ready, valid signals
  rdyvld_port_out<sc_uint<WIDTH> > o_a;
  //o_b output ports: message, ready, valid signals
  rdyvld_port_out<sc_uint<WIDTH> > o_b;
  //i_sum input ports: message, ready, valid signals
  rdyvld_port_in<sc_uint<WIDTH+1> > i_sum;
  //Store the previous inputs to FIFOs
  sc_uint<WIDTH> t_a;
  sc_uint<1> t_a_rdy;
  sc_uint<1> t_a_vld;
  sc_uint<WIDTH> t_b;
  sc_uint<1> t_b_rdy;
  sc_uint<1> t_b_vld;
  sc_uint<WIDTH+1> t_sum;
  sc_uint<1> t_sum_rdy;
  sc_uint<1> t_sum_vld;

  void stim_gen() {
    //initilize handshaking signals
    //o_a.vld.write(false); 
    //o_b.vld.write(false); 
    //i_sum.rdy.write(false);
    for (int a = 0; a < DEPTH; a++) {
      for (int b = 0; b < DEPTH; b++) {
        wait();
        o_a.write(a);
        o_b.write(b);
      }
    }
  }

  void sink() {
    while(true) {
      t_sum=i_sum.read();
      wait();
    }
  }

  void monitor() {
    cout << setw(8) << "time" << setw(8) << "a" << setw(8) << "a rdy" << setw(8) << "a vld" << setw(8) << "b" 
         << setw(8) << "b rdy" << setw(8) << "b vld" 
         << setw(8) << "sum" << setw(8) << "sum rdy" 
         << setw(8) << "sum vld" << endl;
    while (true) {
      wait();
      t_a=o_a.msg.read();
      t_a_rdy=o_a.rdy.read();
      t_a_vld=o_a.vld.read();
      t_b=o_b.msg.read();
      t_b_rdy=o_b.rdy.read();
      t_b_vld=o_b.vld.read();
      t_sum=i_sum.msg.read();
      t_sum_rdy=i_sum.rdy.read();
      t_sum_vld=i_sum.vld.read();
      cout << setw(8) << sc_time_stamp();
      cout << setw(8) << t_a.to_string(SC_BIN);
      cout << setw(8) << t_a_rdy.to_string(SC_BIN);
      cout << setw(8) << t_a_vld.to_string(SC_BIN);
      cout << setw(8) << t_b.to_string(SC_BIN);
      cout << setw(8) << t_b_rdy.to_string(SC_BIN);
      cout << setw(8) << t_b_vld.to_string(SC_BIN);
      cout << setw(8) << t_sum.to_string(SC_BIN);
      cout << setw(8) << t_sum_rdy.to_string(SC_BIN);
      cout << setw(8) << t_sum_vld.to_string(SC_BIN) << endl;
      cout << "===============================================================================================" << endl;
    }
  }

  SC_CTOR(stim) {

    SC_THREAD(stim_gen);
    sensitive << i_clk.pos();
    dont_initialize();

    SC_THREAD(sink);
    sensitive << i_clk.pos();

    // the first row of output table
    SC_THREAD(monitor);
    sensitive << i_clk.pos();
 
  }
};

#endif
