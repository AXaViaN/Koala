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

		Instance().m_IconMap[Icon::NewFunction] = LoadTexture("Data/Icon/new_function.png");
		Instance().m_IconMap[Icon::NewVariable] = LoadTexture("Data/Icon/new_variable.png");
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

	m_TextMap[Text::Arithmetic] = u8"Aritmetik";
	m_TextMap[Text::Add] = u8"Topla";
	m_TextMap[Text::Subtract] = u8"Çikart";
	m_TextMap[Text::Multiply] = u8"Çarp";
	m_TextMap[Text::Divide] = u8"Böl";
	m_TextMap[Text::SquareRoot] = u8"Kare Kök";
	m_TextMap[Text::Power] = u8"Üs";
	m_TextMap[Text::Modulo] = u8"Mod";

	m_TextMap[Text::System] = u8"Sistem";
	m_TextMap[Text::Print] = u8"Yazdir";
	m_TextMap[Text::ReadNumber] = u8"Sayi Oku";
	m_TextMap[Text::ReadString] = u8"Yazi Oku";

	m_TextMap[Text::Flow] = u8"Akis";
	m_TextMap[Text::If] = u8"Eger";
	m_TextMap[Text::ForLoop] = u8"Sirali Döngü";
	m_TextMap[Text::WhileLoop] = u8"Kosullu Döngü";
	m_TextMap[Text::Continue] = u8"Döngüye Devam";
	m_TextMap[Text::Break] = u8"Döngüden Cikis";
	m_TextMap[Text::Loop] = u8"Döngü";
	m_TextMap[Text::Condition] = u8"Sart";
	m_TextMap[Text::True] = u8"Dogru";
	m_TextMap[Text::False] = u8"Yanlis";
	m_TextMap[Text::IfTrue] = u8"Dogru Ise";
	m_TextMap[Text::IfFalse] = u8"Yanlis Ise";

	m_TextMap[Text::General] = u8"Genel";
	m_TextMap[Text::Greater] = u8"Büyüktür";
	m_TextMap[Text::Smaller] = u8"Küçüktür";
	m_TextMap[Text::And] = u8"Ve";
	m_TextMap[Text::Or] = u8"Veya";
	m_TextMap[Text::NumberEquals] = u8"Sayi Esittir";
	m_TextMap[Text::StringEquals] = u8"Yazi Esittir";
	m_TextMap[Text::NumberToString] = u8"Sayiyi Yaziya Dönüstür";
	m_TextMap[Text::StringToNumber] = u8"Yaziyi Sayiya Dönüstür";

	m_TextMap[Text::Search] = u8"Ara";
	m_TextMap[Text::Result] = u8"Sonuc";
	m_TextMap[Text::Value] = u8"Deger";
	m_TextMap[Text::Start] = u8"Basla";
	m_TextMap[Text::End] = u8"Bitir";
	m_TextMap[Text::Index] = u8"Indeks";

	m_TextMap[Text::KoalaEditor] = u8"Koala Editor";
	m_TextMap[Text::NewProject] = u8"Yeni Proje";
	m_TextMap[Text::Save] = u8"Kaydet";
	m_TextMap[Text::Open] = u8"Ac";
	m_TextMap[Text::Cancel] = u8"Vazgec";
	m_TextMap[Text::FileName] = u8"Dosya Adi";

	m_TextMap[Text::Spawn] = u8"Sahnede Olustur";
	m_TextMap[Text::Apply] = u8"Uygula";
	m_TextMap[Text::Remove] = u8"Sil";

	m_TextMap[Text::FunctionName] = u8"Fonksiyon Adi";
	m_TextMap[Text::Number] = u8"Sayi";
	m_TextMap[Text::String] = u8"Yazi";
	m_TextMap[Text::Boolean] = u8"Kosul";
	m_TextMap[Text::Inputs] = u8"Girisler";
	m_TextMap[Text::InputCount] = u8"Giris Sayisi";
	m_TextMap[Text::Outputs] = u8"Cikislar";
	m_TextMap[Text::OutputCount] = u8"Cikis Sayisi";
	
	m_TextMap[Text::DataSizeMismatchError] = u8"koalaVM bu bilgisayarda calistirilamaz: veri boyutu uyusmazligi";
	m_TextMap[Text::KoaFile] = u8"koa dosyasi";
	m_TextMap[Text::KprojFile] = u8"kproj dosyasi";
	m_TextMap[Text::FileCouldNotBeFound] = u8"Dosya bulunamadi!";
	m_TextMap[Text::PressEnterToExit] = u8"Cikis icin enter'a bas";
	m_TextMap[Text::InvalidCodeFileError] = u8"Kod dosyasi hatali!";
	m_TextMap[Text::InvalidProjectFileError] = u8"Proje dosyasi hatali!";
	
	m_TextMap[Text::DivideByZeroError] = u8"Hatali giris: Sifira bölünme!";
	m_TextMap[Text::NumberRequiredError] = u8"Hatali giris: Numara bekleniyordu!";
	m_TextMap[Text::SetLocalModeError] = u8"Hatali mod: \'setlocal\' %zu konumunda tip hatasi! \"%d\" modu gecerli degil!";
	m_TextMap[Text::GetLocalModeError] = u8"Hatali mod: \'getlocal\' %zu konumunda tip hatasi! \"%d\" modu gecerli degil!";
	m_TextMap[Text::DerefModeError] = u8"Hatali mod: \'deref\' %zu konumunda tip hatasi! \"%d\" modu gecerli degil!";
	m_TextMap[Text::AssignrefModeError] = u8"Hatali mod: \'assignref\' %zu konumunda tip hatasi! \"%d\" modu gecerli degil!";
	m_TextMap[Text::GetConstModeError] = u8"Hatali mod: \'getconst\' %zu konumunda tip hatasi! \"%d\" modu gecerli degil!";
	m_TextMap[Text::AllocError] = u8"Bellek ayirma hatasi: \'alloc\' %zu konumunda new operatoru basarisiz oldu! \"%zu\" boyutu fazla buyuk olabilir.";
	m_TextMap[Text::S2IConversionError] = u8"Dönüsüm hatasi: %zu konumunda yazi-indeks dönüsümü basarisiz oldu!";
	m_TextMap[Text::S2DConversionError] = u8"Dönüsüm hatasi: %zu konumunda yazi-sayi dönüsümü basarisiz oldu!";
	m_TextMap[Text::InvalidBytecodeError] = u8"Hatali bytecode \"%x\", konum = %zu!";
	m_TextMap[Text::StackError] = u8"Stack hatasi: %s! Konum = %zu";
	m_TextMap[Text::StackFrameError] = u8"StackFrame hatasi: %s! Konum = %zu";
	
	m_TextMap[Text::Push8StackOverflow] = u8"Push8() yigin tasmasina neden oldu";
	m_TextMap[Text::Push64iStackOverflow] = u8"Push64i() yigin tasmasina neden oldu";
	m_TextMap[Text::Push64fStackOverflow] = u8"Push64f() yigin tasmasina neden oldu";
	m_TextMap[Text::Pop8OnEmptyStack] = u8"Pop8() cagirisi bos yiginda";
	m_TextMap[Text::Pop64iOnEmptyStack] = u8"Pop64i() cagirisi bos yiginda";
	m_TextMap[Text::Pop64fOnEmptyStack] = u8"Pop64f() cagirisi bos yiginda";
	m_TextMap[Text::Top8OnEmptyStack] = u8"Top8() cagirisi bos yiginda";
	m_TextMap[Text::Top64iOnEmptyStack] = u8"Top64i() cagirisi bos yiginda";
	m_TextMap[Text::Top64fOnEmptyStack] = u8"Top64f() cagirisi bos yiginda";

	m_TextMap[Text::PushFrameStackOverflow] = u8"PushFrame() yigin tasmasina neden oldu";
	m_TextMap[Text::ReserveStackOverflow] = u8"Reserve() yigin tasmasina neden oldu";
	m_TextMap[Text::PopFrameOnFirstFrame] = u8"PopFrame() cagirisi ilk cercevede";
	m_TextMap[Text::GetVariableRef8OutOfBound] = u8"GetVariableRef8() yigin disina ulasmaya calisiyor";
	m_TextMap[Text::GetVariableRef64iOutOfBound] = u8"GetVariableRef64i() yigin disina ulasmaya calisiyor";
	m_TextMap[Text::GetVariableRef64fOutOfBound] = u8"GetVariableRef64f() yigin disina ulasmaya calisiyor";
	m_TextMap[Text::GetConstant8OutOfBound] = u8"GetConstant8() yigin disina ulasmaya calisiyor";
	m_TextMap[Text::GetConstant64iOutOfBound] = u8"GetConstant64i() yigin disina ulasmaya calisiyor";
	m_TextMap[Text::GetConstant64fOutOfBound] = u8"GetConstant64f() yigin disina ulasmaya calisiyor";
}
Resource::~Resource() noexcept
{
	for( auto& icon : m_IconMap )
	{
		auto textureID = reinterpret_cast<unsigned int>(icon.second);
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
