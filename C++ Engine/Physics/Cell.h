#pragma once
#include <memory>
#include <vector>

class Collider;

class Cell {
public:
	std::vector<std::weak_ptr<Collider>> solidColliders;
	std::vector<std::weak_ptr<Collider>> triggerColliders;
};