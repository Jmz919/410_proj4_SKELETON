#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/externs.h"

using namespace std;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){
	return myIO.getNext(anOrder);
}

//contains a loop that will get orders from filename one at a time 
//then puts them in order_in_Q then signals baker(s) using cv_order_inQ 
//so they can be consumed by baker(s) 
//when finished exits loop and signals baker(s) using cv_order_inQ that 
//it is done using b_WaiterIsFinished
void Waiter::beWaiter() {
	ORDER myOrder;
	// Keep getting orders from a file until there are no more orders to get
	while (Waiter::getNext(myOrder) == SUCCESS) {
		lock_guard<mutex> lock(mutex_order_inQ);

		// Add the order to the order_in_Q for the baker to bake and box
		order_in_Q.push(myOrder);

		// Notify bakers to start bake and boxing donuts
		cv_order_inQ.notify_all();
	}

	// Signal that the waiter is finished
	b_WaiterIsFinished = true;
	cv_order_inQ.notify_all();
}

