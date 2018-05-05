#include <Koala/Utility/Resource.h>

#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Koala::Utility {

static void* LoadTexture(const std::string& path);

void Resource::Initialize(bool loadIcons)
{
	Instance();

	if(loadIcons)
	{
		Instance().m_IconMap[Icon::Empty] = LoadTexture("");

		Instance().m_IconMap[Icon::New] = LoadTexture("Data/Icon/new.png");
		Instance().m_IconMap[Icon::Open] = LoadTexture("Data/Icon/open.png");
		Instance().m_IconMap[Icon::Save] = LoadTexture("Data/Icon/save.png");
		Instance().m_IconMap[Icon::Settings] = LoadTexture("Data/Icon/settings.png");
		Instance().m_IconMap[Icon::Compile] = LoadTexture("Data/Icon/compile.png");
		Instance().m_IconMap[Icon::Run] = LoadTexture("Data/Icon/run.png");
	}
}

const std::string& Resource::GetText(Text text)
{
	auto str = Instance().m_TextMap.find(text);
	if(str == Instance().m_TextMap.end())
		return Instance().m_TextMap.at(Text::Empty);
	else
		return str->second;
}
void* Resource::GetIcon(Icon icon)
{
	auto ico = Instance().m_IconMap.find(icon);
	if(ico == Instance().m_IconMap.end())
		return nullptr;
	else
		return ico->second;
}

Resource::Resource()
{
	m_TextMap[Text::Empty] = "";

	m_TextMap[Text::ControlPanel] = u8"Kontrol Paneli";
	m_TextMap[Text::Details] = u8"Detaylar";
	m_TextMap[Text::Palette] = u8"Palet";
	m_TextMap[Text::Program] = u8"Program";

	m_TextMap[Text::Functions] = u8"Fonksiyonlar";
	m_TextMap[Text::Variables] = u8"Degiskenler";

	m_TextMap[Text::Search] = u8"Ara";
	m_TextMap[Text::Arithmetic] = u8"Aritmetik";
	m_TextMap[Text::System] = u8"Sistem";

	m_TextMap[Text::Add] = u8"Topla";
	m_TextMap[Text::Subtract] = u8"Çikart";
	m_TextMap[Text::Multiply] = u8"Çarp";
	m_TextMap[Text::Divide] = u8"Böl";
	m_TextMap[Text::SquareRoot] = u8"Kare Kök";
	m_TextMap[Text::Power] = u8"Üs";
	m_TextMap[Text::Modulo] = u8"Mod";
	m_TextMap[Text::Print] = u8"Yazdir";
	m_TextMap[Text::ReadNumber] = u8"Sayi Oku";
	m_TextMap[Text::ReadString] = u8"Yazi Oku";

	m_TextMap[Text::Result] = u8"Sonuc";
}
Resource::~Resource() noexcept
{
	for( auto& icon : m_IconMap )
	{
		unsigned int textureID = reinterpret_cast<unsigned int>(icon.second);
		if(textureID)
		{
			glDeleteTextures(1, &textureID);
		}
	}

	m_IconMap.clear();
}

static void* LoadTexture(const std::string& path)
{
	int x;
	int y;
	int channel;
	unsigned char* image = stbi_load(path.c_str(), &x, &y, &channel, 4);
	bool isLoaded = true;
	if(image == nullptr)
	{
		isLoaded = false;
		
		x = 1;
		y = 1;
		image = new unsigned char[4];
		image[0] = 255;
		image[1] = 0;
		image[2] = 255;
		image[3] = 255;
	}

	// Create texture
	unsigned int textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Free from CPU
	if(isLoaded)
	{
		stbi_image_free(image);
	}
	else
	{
		delete[] image;
	}

	return reinterpret_cast<void*>(textureID);
}

} // namespace Koala::Utility
