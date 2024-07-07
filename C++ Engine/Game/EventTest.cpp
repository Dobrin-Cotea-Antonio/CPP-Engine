#include "EventTest.h"
#include <iostream>
#include "../Base/InputManager.h"

EventTest::EventTest() {}

EventTest::~EventTest() {}

void EventTest::Update(){

	if (!wasSetUp) {

		auto lambda2 = [*this](float pTest) mutable { TestMethod2(pTest); };
		auto lambda3 = [*this](float pTest1, int pTest2) mutable { TestMethod3(pTest1, pTest2); };
		auto lambda4 = [*this](float pTest1, int pTest2) mutable { TestMethod4(pTest1, pTest2); };

		Method<float> method2(lambda2);
		Method<float, int> method3(lambda3);
		Method<float, int> method4(lambda4);

		event3.Subscribe(method3,selfPointer);
		event3.Subscribe(method4, selfPointer);
		

		event3.Invoke(2.2,5);

		event3.Unsubscribe(method3, selfPointer);

		event3.Invoke(3.5,6);

		wasSetUp = true;	
	}

	//if (InputManager::GetInstance()->IsKeyPressed(Key::W))
	//	event3.Invoke(5.5, 2);
}

void EventTest::TestMethod1() {
	std::cout << "Test method 1: \n";
}

void EventTest::TestMethod2(float pArg1) {
	std::cout << "Test method 2: " << pArg1 << "\n";
}

void EventTest::TestMethod3(float pArg1, int pArg2) {
	std::cout << "Test method 3: " << pArg1 << " " << pArg2 << "\n";
}

void EventTest::TestMethod4(float pArg1, int pArg2){
	std::cout << "Test method 4: " << pArg1 << " " << pArg2 << "\n";
}


