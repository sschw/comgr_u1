#include "random_var.h"

mt19937& e_rand() {
	thread_local mt19937 e_rand(chrono::system_clock::now().time_since_epoch().count());
	return e_rand;
}

float u_01dist() {
	thread_local uniform_real_distribution<float>* u_01dist;
	if (u_01dist == nullptr) {
		u_01dist = new uniform_real_distribution<float>(0, 1.0f);
	}
	return (*u_01dist)(e_rand());
}

float u_11dist() {
	thread_local uniform_real_distribution<float>* u_11dist;
	if (u_11dist == nullptr) {
		u_11dist = new uniform_real_distribution<float>(-1.0f, 1.0f);
	}
	return (*u_11dist)(e_rand());
}
float n_dist() {
	thread_local normal_distribution<float>* n_dist;
	if (n_dist == nullptr) {
		n_dist = new normal_distribution<float>(0, 1.0f);
	}
	return (*n_dist)(e_rand());
}
