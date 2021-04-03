#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
using namespace std;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

//bake, box and append to anOrder.boxes vector
//if order has 13 donuts there should be 2 boxes
//1 with 12 donuts, 1 with 1 donut
void Baker::bake_and_box(ORDER &anOrder) {
	Box myBox;
	for (int i = 0; i < anOrder.number_donuts; i++) {
		DONUT myDonut;
		// Box is full add to list of boxes for order
		if (!myBox.addDonut(myDonut)) {
			// Add box to boxes vector
			anOrder.boxes.push_back(myBox);

			// Clear full box and add donut into the box
			myBox.clear();
			myBox.addDonut(myDonut);
		}
	}
	// Add last box that is not full to boxes for order
	anOrder.boxes.push_back(myBox);
}

//as long as there are orders in order_in_Q then
//for each order:
//	create box(es) filled with number of donuts in the order
//  then place finished order on order_outvector
//  if waiter is finished (b_WaiterIsFinished) then
//  finish up remaining orders in order_in_Q and exit
//
//You will use cv_order_inQ to be notified by waiter
//when either order_in_Q.size() > 0 or b_WaiterIsFinished == true
//hint: wait for something to be in order_in_Q or b_WaiterIsFinished == true
void Baker::beBaker() {
	std::unique_lock<std::mutex> lock(mutex_order_inQ);
	while (!order_in_Q.size() > 0 && !b_WaiterIsFinished) {
		cv_order_inQ.wait(lock);
	}

	while (order_in_Q.size() > 0) {
		lock_guard<mutex> lock(mutex_order_inQ);
		ORDER myOrder = order_in_Q.front();
		order_in_Q.pop();
		Baker::bake_and_box(myOrder);
	}
}
