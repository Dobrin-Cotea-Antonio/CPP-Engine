#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>
#include "SFML/Graphics/Texture.hpp"

class TextureCache {

private:
	static TextureCache* instance;

	std::map<std::string, std::shared_ptr<sf::Texture>> textures;
	std::map<std::string, int> useCount;

#pragma region Constructor/Destructor
protected:
	TextureCache();
public:
	virtual ~TextureCache();
#pragma endregion

public:
#pragma region Deleted Methods
	TextureCache(TextureCache& pOther) = delete;
	void operator=(const TextureCache&) = delete;
#pragma endregion

#pragma region Instance
	static TextureCache* GetInstance();
#pragma endregion

#pragma region Methods
	sf::Texture& GetTexture(const std::string& pTextureAdress);
	void DecreaseTextureCount(const std::string& pTextureAdress);
#pragma endregion

};