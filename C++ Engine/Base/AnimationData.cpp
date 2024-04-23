#include "AnimationData.h"

#pragma region Constructor/Destructor
AnimationData::AnimationData()
{
}
AnimationData::AnimationData(const int pCols, const int pRows, const int pFrames, const float pAnimationSpeed, const std::string pImageAdress) :cols(pCols), rows(pRows), frames(pFrames), animationSpeed(pAnimationSpeed),imageAdress(pImageAdress), animate(true) {
}

AnimationData::~AnimationData() {
}
#pragma endregion