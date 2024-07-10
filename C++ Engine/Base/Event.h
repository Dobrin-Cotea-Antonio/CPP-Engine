#pragma once
#include <functional>
#include <vector>
#include <utility>
#include <type_traits>
#include <map>
#include <memory>
#include "../Base/Object.h"

template<typename... A>
class Method {
protected:
	size_t hash;

	std::function<void(A...)> boundMethod;

public:
	Method(std::function<void(A...)> pMethod) : hash(pMethod.target_type().hash_code()), boundMethod(std::move(pMethod)) {}

	bool operator == (const Method<A...>& pMethod) {
		return hash == pMethod.hash;
	}

	bool operator != (const Method<A...>& pMethod) {
		return hash != pMethod.hash;
	}

	constexpr size_t ReturnHashCode() const throw() {
		return hash;
	}

	Method<A...>& Invoke(A... pArgs) {
		boundMethod(static_cast<A&&>(pArgs)...); 

		return (*this);
	}

	void operator()(A... pArgs) {
		boundMethod(static_cast<A&&>(pArgs)...);
	}
};

template<typename... A>
class Event {
protected:
	//should be used when the event is not attached to a Component
	std::vector<Method<A...>> normalMethodList;
	//events that are attached to a component are placed in here
	std::vector<Method<A...>> objectMethodList;
	std::vector<std::weak_ptr<Object>> objectList;

public:
	Event<A...>& Subscribe(const Method<A...> pMethod) {
		bool wasFound = false;

		for (int i = 0; i < normalMethodList.size(); i++)
			if (normalMethodList[i].ReturnHashCode() == pMethod.ReturnHashCode()) {
				wasFound = true;
				break;
			}

		if (!wasFound)
			normalMethodList.push_back(pMethod);

		return (*this);
	}

	Event<A...>& Subscribe(const Method<A...> pMethod, std::weak_ptr<Object> pRef) {
		bool wasFound = false;

		for (int i = 0; i < objectMethodList.size(); i++)
			if (objectMethodList[i].ReturnHashCode() == pMethod.ReturnHashCode()) {
				wasFound = true;
				break;
			}

		if (!wasFound) {
			objectMethodList.push_back(pMethod);
			objectList.push_back(pRef);
		}

		return (*this);
	}

	Event<A...>& Unsubscribe(const Method<A...> pMethod) {
		for (int i = 0; i < normalMethodList.size(); i++)
			if (normalMethodList[i].ReturnHashCode() == pMethod.ReturnHashCode()) {
				normalMethodList.erase(normalMethodList.begin() + i);
				break;
			}

		return (*this);
	}

	Event<A...>& Unsubscribe(const Method<A...> pMethod, std::weak_ptr<Object> pRef) {
		for (int i = 0; i < objectMethodList.size(); i++)
			if (objectMethodList[i].ReturnHashCode() == pMethod.ReturnHashCode()) {
				objectMethodList.erase(objectMethodList.begin() + i);
				objectList.erase(objectList.begin() + i);
				break;
			}

		return (*this);
	}

	Event<A...>& Clear() {
		normalMethodList.clear();
		objectMethodList.clear();
		objectList.clear();
		return (*this);
	}

	Event<A...>& Invoke(A... pArgs) {
		std::vector<Method<A...>> clonedMethods(normalMethodList);

		for (int i = 0; i < clonedMethods.size(); i++)
			clonedMethods[i].Invoke(static_cast<A&&>(pArgs)...);


		for (int i = 0; i < objectMethodList.size(); i++) {
			if (objectList[i].lock().get() == nullptr) {
				objectMethodList.erase(objectMethodList.begin() + i);
				objectList.erase(objectList.begin() + i);
				i--;
				continue;
			}

			objectMethodList[i].Invoke(static_cast<A&&>(pArgs)...);
		}

		return (*this);
	}
};
