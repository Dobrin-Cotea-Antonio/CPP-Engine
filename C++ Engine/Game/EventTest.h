#pragma once
#include "../Base/Event.h"
#include "../Base/Component.h"

class EventTest :public Component {
private:
	Event<> event1;
	Event<float> event2;
	Event<float, int> event3;

	bool wasSetUp = false;

public:
	EventTest();
	virtual ~EventTest();

	virtual void Update()override;
private:
	void TestMethod1();
	void TestMethod2(float pArg1);
	void TestMethod3(float pArg1, int pArg2);
	void TestMethod4(float pArg1, int pArg2);
};