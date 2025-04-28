/*
*  User defined literaller template'de olabilir fakat charlardan oluşan bir template parameter pack olarak bunu yapmalıyız.
*  Aşağıda bir user defined literal template örneği verilmiştir.
*  
*  Not: Variadic templateler kursun ilerleyen bölümünde detaylı olarak ele alınacaktır.


template<char ...CHARS>
constexpr unsigned operator"" _b()
{
	//
}

int main()
{
	101_b; // Derleyici bu user defined literali aşağıdaki gibi bir fonksiyon çağrısı yapar.
	//operator""_b<'1', '0', '1'>();
}


*  Peki derleyici yukarıdaki user defined literal ifadesini operator""_b<'1', '0', '1'>(); olarak fonksiyon çağrısı haline
*  getirdikten sonra literal operatorlerde bunlara nasil ulaşacağız. Bunun nasil olacağı aşağıda gösterilmiştir. Pack expansion
*  yaparak ilgili char elemanları bir dizide toplamak daha sonra bu dizi üzerinden kodu yazmak en yaygın konvesyoneldir.


template<char ...CHARS>
constexpr unsigned operator"" _b()
{
	constexpr char str[] = { CHARS..., '\0' };
	unsigned result{};

	for (int i = 0; str[i] != '\0'; ++i) {
		result = result * 2 + str[i] - '\0';
	}

	return result;
}

*/

/*
*  Strong type dediğimiz türler tipik olarak tam sayı gerçek sayı türlerinden değişkenleri sarmalayıp domain'e uygun başka
*  bir interface verirler. Bunun en büyük faydası sudur. Problem domainindeki varlıkları çoğu zaman bir sınıf yerine gerçek 
*  veya tam sayı olarak temsil edebiliriz. Buda tür dönüşümlerine izin verip niyeti anlatmadığı için bunları ilave maliyet almadan
*  yani zero maliyet le domaine uygun farklı bir tür haline getirebilirim. Bu tarz türlere strong types denmektedir. Aşağıda da bunun
*  küçük bir örneği var. Meter bir strong type dir ve aslında bir gerçek sayıyı metre yi temsil etmesi için oluşturulmuştur, buna 
*  da uygun bir interface sunmuştur. Böyle bir sınıf olmasaydı double değişkeni direk kullancaktık ki o zaman hem niyet belli etmeyecek
*  hem implicit tür dönüşümlerine müsait. Böyle strong typelar ile ilgili domaine uygun nesneyi oluşturmak için de user defined literal
*  operator yazılıp kullanılabilir. Aşağıdaki örnekte bu literal operatorun de kodu gösterilmiştir.
*  
*  Ayrıca bu Meter sınıfından bir nesneyi kullanıcı kendide oluşturmasın diye bir nested type oluşturulup ctor için bu nested type
*  da bir arguman olarak eklenmiştir.

#include <iostream>

class Meter {
	double mts;
	
public:
	class PreventUsage {};
	explicit constexpr Meter(PreventUsage, double meter) : mts{ meter } {}
	explicit constexpr operator double()const { return mts; }
};

constexpr Meter operator"" _m(long double m)
{
	return Meter(Meter::PreventUsage(), static_cast<double>(m));
}

std::ostream& operator<<(std::ostream& os, const Meter& m)
{
	return os << static_cast<double>(m) << "meters";
}

int main()
{
	Meters m = 3.9_m;
}

*/


/*
*		STRING_VIEW_STRING_VIEW_STRING_VIEW_STRING_VIEW_STRING_VIEW
* 
*  Necati hoca string_view STL öğesini anlattı. C++ 17 ile dile eklenmiştir. 
* 
*  string_view aslında bir tür eş ismi, direk bir template değil. Amacı ise gözlemci olmak, izlemek. Yani bir yazı için bellek
*  alanı ayırmadan ilgili yazıyı gösteren, o yazının izlenmesine sağlayan basic_string_view in sınıf sablonunun bir specializationudur.
*  string_view başlık dosyasında yer almaktadır.
* 
*  string_view sınıfı için özel not almadın, Umut Kotankıran bu konuda çok iyi not almış, direk olarak onun notlarını kullandım.
*/

/*
* 
* 
STRINGVIEW SINIFI
-----------------
Temel varlık sebebini anlamaya çalışalım. Bİr çok durumda salt okuma amaçlı bir yazıyı alan function yazıyoruz. Mesela C olsaydı 
parametre const char * olurdu.Bu bizden salt access amaçlı bir yazı istediğini söylüyor. YAzıyı değiştirmeyeceğini söylüyor.

Dil C++ olunca, C++17 ile dile eklenen string view sayılmazsa, her seçeneğin belirli dezavantajları var.
Mesela func parametresini string türünden yaparsak bunun dezavantajları,

1 - Bu functiona herhangibir yazıyı geçemeyiz. Ya string nesnesi olacak yada bir cstring kullanılacak.

2 - String literali zaten statik ömürlü programın sonuna kalacak bir dizi. BUrada func parametresi referans olduğundan burada bir temporary materialization sözkonusu olacak ve 
	bir string nesnesi oluşturulacak. BU ilavebir maliyet. Neden bir string nesnesi oluşturulsun zaten biz cstring gönderiyoruz.

3 - Const char * yaparsak parametreyi bu seferde string nesnesi gönderemeyiz.Const char * yapınca func içindekioperasyonların bir interface e sahip olmasını istiyoruz.
	char da bu interface yok !!!!!!!!!!!! BURASI ÇOK ÖNEMLİ !!!!!

4 - Overloading yapılabilirdi ama buda okuma amaçlı bir yazıyı kullanma fikri için biraz overkill oluyor.Sırf bunun için 2-3 tane overload mu yazalım ? hayır

5 - Yazı string / cstring olmak zorunda da değil.Mesela std::array<char,20> böyle böyle bir parametre verirsek bu yazıyı tutabilir. Ya da vector<char> olabilir.


void func(const std::string& x);

int main()
{
	func("sercan satici dogukan polat"); 
										 
}

1500. HATIRLATMA
C de string literalleri char, C++ ta const char olarak saklanıyor.Değiştirmek tanımsız davranış.

char *p = "Alican"; //C de  geçerlidir.
const char *p = "Alican"; //C++ de  geçerlidir.

=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================

İDEALİ OKUMA AMAÇLI ALACAĞIM STRİNGİ BİRDE STRİNG MANİPLASYON İŞLEMLERİ İÇİN İSİMLENDİRİPMİŞ APİLER VERİLMİŞ OLMASI
HEM ÇOK DÜŞÜK MALİYETLE HERTÜRLÜ YAZIYI GEÇEYİM AMA BİR SINIFIN İNTERFACE İNEDE SAHİP OLSUN.

Yazı hangi formda olursa olsun karakterler bellekte ardışık.İster char dizisinde olsun ister string nesnesinde tutulsun(string nesnesinde tutulması SSO yapılarak tutuluyor veya
heapten alınan bir bellek alanında tutuluyor, cstring zaten static ömürlü bellekte kalıyor sürekli) Ben bir pointer alsam ve bu pointer yazı karakterlerinden birini gösterse ve bir
pointer daha alsam o da yazının daha sonraki bir karakterini gösterse bu iki pointer arasındaki tüm karakterlere erişebilirim. Pointer olmasıda zorunlu değil.
Pointer aritmetiğine göre 1. pointer 2. tamsayı değişken olsa yine bu adresi elde ederim.

Bunu bir sınıf haline getirsek, bu 2 pointer veya 1 ptr 1 tamsayı değişkeni olarak alıp bunları paketleyip member functionları olsa. Salt okuma amaçlı kullanılacak yazıyla ilgili
bir sınıf olduğu için member funcları yazı üzerinde okuma amaçlı erişim yapacak üye funclar olsun. Yazma amaçlı kullanıldığında sentaks hatası olsun.
Bu yazılabilir. İçerisinde herşeyi yazabiliriz. Bu sınıf STL deki C++17 ile gelen STRING VIEW sınıfı.Adı üstünde stringi görüntülemeye ilişkin bir sınıf.

Bu dizilerede uygulanabilir. Vectore de uygulanabilir.array ede uygulanabilir.Bellekte ardışık olarak tutulan öğelere uygulanabilen bir yapı.

=====================================================================================================================================================================================

HEADER FILE STRING_VIEW
#include <string_view>

Bu bir template. string_view bir typedef ismi.

int main()
{
	----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	std::cout << "sizeof(char *) = " << sizeof(char *) << "\n"; // 4 byte
	std::cout << "sizeof(string_view) = " << sizeof(string_view) << "\n"; // 8 byte

	Kopyalama maliyeti temel türden verileri kopyalama maaliyetinden fazla değil. 

	----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	MÜLAKAT/SINAV SORUSU GENELDE DANGLING REFERANS SORUSU OLACAK

	mesela sercansatici yazısındaki ercan yazısını kullanmak istesem, bu yazı stringview tarafından tutulsa, string view nesnesi bu yazının sahibi değil,
	dolayısıyla stringview oluşturulduğunda biz adresi tutmuş oluyoruz. string view adresteki yazı halen bellekte duruyor mu durmuyor mu bunu bilemez.
	Bunu kullanırken çok dikkatli olmak lazım.
	
	Baya küçük bir sınıf string_view
}

=====================================================================================================================================================================================
=====================================================================================================================================================================================
=====================================================================================================================================================================================

CONSTRUCTORS

 1. Default constructor. Constructs an empty std::basic_string_view. 
 After construction, data() is equal to nullptr, and size() is equal to ​0​.

int main()
{
	string_view sv; // default construct edilebilir.String view herhangibir yazının gözlemcisi durumunda olmayabilir.

	string_view interfacesinde stringte olan neredeyse tüm okuma araçlı funclar var.
}


string_view default construct edildiğinde bir yazıyı göstermiyor.


int main()
{
	string_view sv;

	cout << boolalpha;

	std::cout << sv.empty() << "\n"; // true
	std::cout << sv.size() << "\n"; // 0
	std::cout << sv.length() << "\n"; // 0
	std::cout << (sv.data() = nullptr) << "\n"; // true

	--------------------------------------------------------------------------------------------------------------------------------------------
	
	Gözlemci olsaydı

	sv = "Ali";
	std::cout << sv.empty() << "\n"; // false
	std::cout << sv.size() << "\n"; // 3
	std::cout << sv.length() << "\n"; // 3
	std::cout << (sv.data() = nullptr) << "\n"; // false
}


-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------

CTORLAR DEVAM
 2. Constructs a view of the null-terminated character string pointed to by s, not including the terminating null character.

int main()
{
	using namespace std;

	char str[] = "necati ergin c++ anlatiyor";

	string_biew sv(str);  // sv, str yazısının gözlemcisi oldu

	std::cout << sv.size() << "\n"; // 26
	std::cout << sv<< "\n"; //necati ergin c++ anlatiyor. Bu yazıyı yazdırması demek sonunda null character var demek değil.

	------------------------------------------------------------------------------------------------
	
	string_view sv(str.data(),6);

	std::cout << sv.size() << "\n"; // 6
	std::cout << sv<< "\n"; //necati. 

	std::cout << sv.data(); // Buradaki çağrılan func cstring parametreli func oldupunda bu yazı nullchar görene
							// kadar yazacak.YANİ necati ergin c++ anlatiyor yazacak yine

	------------------------------------------------------------------------------------------------

	array<char,5> ar{'a','h','m','e','t'};

	string_view sv{ar.data(),ar.size()};

	std::cout << sv << "\n"; // ahmet yazar
	std::cout << sv.data() << "\n"; // TANIMSIZ DAVRANIŞ.Burada çağrılan func yazı sonunda '\0' arıyor.

	std::cout << sv.size() << "\n"; // 5 yazar.

	------------------------------------------------------------------------------------------------

	string str{"gokhan zilanli"};

	string_view sv{str};

	std::cout << "sv = " << sv << "\n";
	std::cout << "sv.size() = " << sv.size() << "\n";

}

-----------------------------------------------------------------------------------------------------------------------------------------

DATA CTOR
---------
3. constexpr basic_string_view( const CharT* s, size_type count );
Constructs a view of the first count characters of the character array starting with the element pointed by s. s can 
contain null characters. The behavior is undefined if [s, s + count) is not a valid range


int main()
{
	using namespace std;

	string str[] = "necati ergin c++ anlatiyor";
	string_view sv{str.data(), 6};

	std::cout << "sv = " << sv << "\n";
	std::cout << "sv.size() = " << sv.size() << "\n";

}

-----------------------------------------------------------------------------------------------------------------------------------------

RANGE CTOR
----------

4. 
template< class It, class End >
constexpr basic_string_view( It first, End last );

int main()
{
	using namespace std;

	string str[] = "necati ergin c++ anlatiyor";

	//char str[] = "Enesalp"; // char da olabilirdi tabiki.

	string_view sv{str.data() + 3, str.data() + 8};

	std::cout << "sv = " << sv << "\n";
	std::cout << "sv.size() = " << sv.size() << "\n";
}



void func(std::string_view sv) // Okuma amaçlı yazı istiyor.
{
	
}

int main()
{
	char str[]{"necati ergin"};
	std::string sx = {"mustafa"};
	std::string_view sv;

	func("bitcoin rises");
	func(str);
	func(sx);
	func(sv);

}

-----------------------------------------------------------------------------------------------------------------------------------------

string_view kullandığımızda bu yazının sahibi değiliz, sahibi başkasıdır.

int main()
{
	char str[]{"Necati ergin"};

	string_view sv = str;

	std::cout << "sv = " << sv << "\n";

	str[1] = 'u';
	str[3] = 'x';

	std::cout << "sv = " << sv << "\n"; // Yazıyı yazdırır.Adres değişmedi. Değişmiş yazıyı yazdık sadece.
}

-----------------------------------------------------------------------------------------------------------------------------------------

Burada auto ile yapılan type deductiona dikkat


int mani()
{
	auto x = "mustafa";
	auto y = "mustafa"s; // Bu literaling return değeri string
	auto z = "mustafa"sv; // string view için literal operator func var. z nin türü string view oldu.
}



template<typename T>
void foo(T x)
{
	
}

int main()
{
	using namespace std::literals;

	foo("mustafa"); // Parametre const char *
	foo("mustafa"s); // Parametre string
	foo("mustafa"sv); // Parametre string view olur
}

Stringin büyük olduğu yerlerde performans farkıda o kadar büyük olur. 
Burada literal operator func konusunu anlatacağını söyledi hoca. Temel C++ sonlarına doğru gördük.

=====================================================================================================================================================================================
=====================================================================================================================================================================================

TİPİK YANLIŞ KULLANIMLAR

- BIR FUNCIN RETURN DEĞERI STING_VIEW YAPILMAMALI

class Person{
public:
	std::string getname()const;
private:
	std::string mname;
};

int main()
{
	Person p;
	string_view sv = p.getname(); // BU FUNCIN RETURN DEĞERINI BIR CONST LVALUE REF VEYA RVALUE REF INITIALIZE ETMEK IÇIN KULLANIRSAK
}				      // LIFE EXTENSION OLUR AMA BUNU BIR STRINGVIEW NESNESINE ILK DEĞER OLARAK VERIRSEK, BU NESNE ASLINDA SADECE
				      // BU YAZIYA ILIŞKIN POINTER TUTUYOR. BU DURUMDA FUNC RETURN DEĞERI NESNENIN HAYATI BITTIĞINDEN
				      // SV DANGLING REFERANS OLACAK.


-----------------------------------------------------------------------------------------------------------------------------------------

ÖR:

string func()
{
	string s{"akif"};

	return s;
}

int main()
{
	string_view sv = func();
	
	std::cout << "sv = " << sv << "\n"; // TANIMSIZ DAVRANIŞ.
}

-----------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	string sx{"mustafa bakircioglu"};

	string_view sv{sx};

	std::cout << "sv = " << sv << "\n";

	sx.assign(100,'A'); // Yazı büyüdü. Reallocation oldu.Yazı taşındı.string_view içindeki adres dangling hale geldi

	std::cout << "sv = " << sv << "\n"; //DANGLING POINTER OLUŞTU. TANIMSIZ DAVRANIŞ
}

-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------

OKUMA AMAÇLI HER İŞLEMDE STRİNG VİEW KULLANABİLİRİZ.

void process(string_view sv)
{
	sv.substr(); // Böyle çağırmak sv.substr(0, npos); yani tamamının kopyası

	------------------------------------------------------------------------------------------------

	sv.substr(15); // bu indexten başyarak geriye kalan hepsi, sv.substr(15, npos); aynısı

	------------------------------------------------------------------------------------------------

	auto sx = sv.substr(15,20); // 15. indexten başla 20 karakter substring al.
								// sx türüde string view.

	------------------------------------------------------------------------------------------------

	if(auto idx = sv.find('.'); idx != string::npos) // Arama işlemide aynı şekilde yapılır.
	{
		//...
	}

}

int main()
{
	string sx{"mustafa bakircioglu"};
}

-----------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	string sx {"mustafa bakircioglu"};

	cout << boolalpha << sx.starts_with("mustafa") << "\n"; // True
	cout << boolalpha << sx.starts_with("musti") << "\n"; // False
	
	cout << boolalpha << sx.ends_with("oglu") << "\n"; // True
	cout << boolalpha << sx.ends_with("Aglu") << "\n"; // False
}

*/


/*
*  TEMEL VARLIK NEDENI YAZI İLE İLGİLİ İŞLEMLERDE YAZININ KOPYALANMASI İLE OLUŞAN MAALİYETTEN KAÇMAK.

1 pointer 1 tamsayı olabilir içerisinde, 2 pointer olabilir. Standartlar bunu söylemiyor.
Maliyeti az, 2 pointer olsa 8byte zaten. String kopyalanmasına göre çok çok daha maliyetsiz.

1 - String view nesnesi yazının sahibi değil, başkasının yazısının adresini tutuyor içinde.
	Stringview hayatta iken ilgili yazının hayatı biterse dangling referans olacak.
	
2 - Bir functionun stringview döndürmesi en sık hatalardan biri. Otomotik ömürlü nesneyi stringview
	ile dönmek mesela.

3 - Stringview ın bir interface i var.String sınıfının tüm get interface i stringviewda var.
	2 noktaya dikkat etmek lazım. Datayı çağırınca string vector array de olduğu için bir adres veriyor.
	Bu adres stringviewda tutulan adres. Dolayısı ile bu yazı sonunda null character olmak zorunda değil.
	olabilir, olmayadabilir. Yani data funcının return değerini null termnated byte stream olarak kullanma girişimi
	TANIMSIZ DAVRANIŞ.

4 - Başka bir dangling ptr senaryosu. String nesnesinin tuttuğu bir yazıya pointer içeriyorsa, string
	sınıfı türünden nesne belirli işlemlerle yazı büyütülünce reallocation yapıyor, reallocation yaparsa
	bizim pointer yine dangling hale gelecek

5 - Sadece bir pointer tuttuğumuz için bizim pointer hayatta iken yazı değişebilir.
	Yani pointer dangling değil ama adresteki yazı değişti. Bu durumda biz o pointer yada stringview
	nesnesini kullanınca değişmiş yazıyı elde ederiz.

=================================================================================================================================
=================================================================================================================================
=================================================================================================================================
=================================================================================================================================

#include<string_view>
#include<string>

template <typename T>
T concatanate(const T &x, const T&y)
{
	return x + y;
}

std::string operator+(std::string_view x, std::string_view y)
{
	return std::string{ x } + std::string{ y };
}

int main()
{
	std::string_view sv{"harun"};
	auto val = concatanate(sv,sv);
	std::cout << "val = " << val << "\n"; //TANIMSIZ DAVRANIŞ
}

Concatanate te T türü stringview olacak, bu durumda toplama operatörünün oprandları stringview türünden.
Çağrılan function bizim yazdığımız operator+ olacak.

operator+ içinde stringler toplanmış ve return değeri string. Concatanate içinde ise return değeri T yani stringview
x + y türü string idi, bunu return ettiğimizde T türüne yani string_view türüne dönüşecek.
Buradaki yanıltıcı nokta funcın return değerinin string değil stringview olması.

Ömrü bitmiş bir sağ taraf nesnesini stringview ile döndürdük


---------------------------------------------------------------------------------------------------------------------------------

ÖR:

std::string get_str(int ival)
{
	//..
	return std::to_string(ival);
}

int main()
{
	int x;

	std::cout << "bir tamsayi girin\n";
	cin>>x;

	auto &r1 = get_str(x); // SENTAKS HATASI
	const auto &r2 = get_str(x); // KOD LEGAL. LIFE EXTENSION
	auto &&r3 = get_str(x); //GEÇERLİ

	std::string_view sv = get_str(x); //TANIMSIZ DAVRANIŞ VAR.SAĞ TARAFDEĞERİ VE ÖMRÜ BİTTİ. ÖMRÜ BİTMİŞ NESNEYİ TUTUYOR STRINGVIEW
}

---------------------------------------------------------------------------------------------------------------------------------

int main()
{
	using namespace std::literals;

	std::string_view x = "abc\0\0def";
	std::string_view y = "abc\0\0def"sv;

	std::cout << "x.size() = " << x.size() << "\n"; // 3. Constructor Cstring parametreli, \0 a kadar alır bu sebeple karakterleri
	std::cout << "y.size() = " << y.size() << "\n"; // 8. BU stringview nesnesi, tüm karakterler dahil burada.Artık cstring ctor değil.
}

ÇOK ÖNEMLİ BURASI !!!!!!!!!!!!!!

int main()
{
	using namespace std;

	char str[] = {'a','l','p','e','r'};

	string_view sv{str}; //Burada çalışan cstring parametreli ctor

	std::cout << sv << "\n"; // UNDEFINED BEHAVIOR VAR. NULL TERMINATED BYTE STREAM DEĞİL.

	-------------------------------------------------------------------------------------------------------------

	char str[] = {'a','l','p','e','r'};

	string_view sv{str,5};

	std::cout << sv << "\n"; // Şuanda kod geçerli.Çalışan inserter string view inserterı.Nullchar beklemiyor.

	-------------------------------------------------------------------------------------------------------------

	char str[] = {'a','l','p','e','r'};

	string_view sv{str,5};

	std::cout << sv.data() << "\n"; // UNDEFINED BEHAVIOR.Çünkü şuanda çalışan cstring inserter.
					// null char istiyor ama yazı sonunda nullchar yok :D:D
}

---------------------------------------------------------------------------------------------------------------------------------

int main()
{
	string_view sv{"Enes Alp"};

	string str = sv; //Sentaks hatası. Ctor explicit
}

---------------------------------------------------------------------------------------------------------------------------------

void func(std::string str);

int main()
{
	string_view sv{"Enes alp"};

	func(sv); //SENTAKS HATASI
	func(string{sv}); // GEÇERLİ
}


---------------------------------------------------------------------------------------------------------------------------------

RETURN DEĞERİ STRING OLAN NESNENIN RETURN İFADESİ STRING_VIEW OLAMAZ

string func()
{
	string_view sv{"Enes alp"};

	return sv; // BURASI SENTAKS HATASI
}

---------------------------------------------------------------------------------------------------------------------------------

int main()
{
	string str{"melih çiçek"};

	string_view sv = str; // Burada sentaks hatası yok.Çünkü string_view ın string parametreli Ctoru var.

	sv = str; // Burada atama var. HATA DA YOK. NASIL OLUYORDA GEÇERLİ OLDU ??
		  // TÜR DÖNÜŞTÜRME OPERATOR FUNCTIONU
		  // str.operator std::basic_string_view<char, std::char_traits<char>>();
		  // Bu bildiğimiz tür dönüştürme operator func.Stringview gereken yerde string kullnırsak 
		   // Bu stringi stringviewa dönüştürecek. 


}

REMOVE_PREFIX / REMOVE_SUFFIX
-----------------------------
Bunlar gözlem aralığını daraltıyor. Normalde biz string view nesnesi olarak gözlemciyiz sahibi değiliz.
Bu alanı istersek daraltabiliriz. Bu yazıyı değiştirmek değil string view içindeki pointerları değiştirmek oluyor.

remove_prefix : Gözlem aralığını önden daraltıyor
remove_suffix : Gözlem aralığını sondan daraltıyor


int main()
{
	string_view str{"melih cicek"};

	std::cout << "(" << str << ")\n"; // melih cicek
	str.remove_prefix(6);
	std::cout << "(" << str << ")\n"; // cicek

	---------------------------------------------------------------

	std::cout << "(" << str << ")\n"; // melih cicek
	str.remove_suffix(6);
	std::cout << "(" << str << ")\n"; // melih

}

Bu function sadece pointerın değerini değiştirdi.


---------------------------------------------------------------------------------------------------------------------------------

KOLAY GIBI GÖRÜNEN ZOR SORU !!!!

int main()
{
	string_view str{"melih cicek"};
	std::cout << "(" << str << ")\n";
	str.remove_suffix(6);

	cout << str.data() << "\n"; // melih çiçek yazar. Data değişmedi. O halen melih çiçekin adresi.

	---------------------------------------------------------------------------------------

	DİKKAT!!
	BU FARKLI !!!

	std::cout << "(" << str << ")\n";
	str.remove_prefix(6); // dikkat!!! ön tarafdan daralttık

	cout << str.data() << "\n"; // ŞIMDI ÇİÇEK YAZAR
}


*/

/*
*  JOSUTTIS KITABINDAN BASIC NOTLAR:
* 
With C++17, a special string class was adopted by the C++ standard library, that allows us to deal
with character sequences like strings, without allocating memory for them: std::string_view.
That is, std::string_view objects refer to external character sequences without owning them.
That is, the object can be considered as a reference to a character sequence.

Using such a string view is cheap and fast (passing a string_view by value is always cheap).
However, it is also potentially dangerous, because similar to raw pointers it is up to the programmer
to ensure that the referred character sequences is still valid, when using a string_view).

Due to the possible nullptr value and possible missing null terminator, you should always use
size() before accessing characters via operator[] or data() (unless you know better).

There are two major applications of string views:
1. You might have allocated or mapped data with character sequences or strings and want to use this
data without allocating more memory. Typical examples are the use of memory mapped files or
dealing with substrings in large texts.
2. You want to improve the performance for functions/operations that receive strings just to directly
process them read-only not needing a trailing null terminator.

A special form of this might be to deal with string literals as objects having an API similar to
strings:
static constexpr std::string_view hello{"hello world"};

Usually “smart objects” such as smart pointers are considered to be safer (or at least not more dangerous)
than corresponding language features. So, the impression might be that a string view, which
is a kind of string reference, is safer or at least as safe as using string references. But unfortunately
this is not the case. String views are in fact more dangerous than string references or smart pointers.
They behave more like raw character pointers.

Don’t Assign Strings to String Views
	Consider we declare a function returning a new string:
		std::string retString();

Using the return value is usually pretty safe:
	Assigning it to a string or an object declared with auto is safe (but moves, which is usually OK,
	but doesn’t have the best performance):
		auto std::string s1 = retString(); // safe

Assigning the return value to a string reference is, if possible, pretty safe as long we use the object
locally because references extend the lifetime of return values to the end of their lifetime:

		std::string& s2 = retString(); // Compile-Time ERROR (const missing)

		const std::string& s3 = retString(); // s3 extends lifetime of returned string
		std::cout << s3 << '\n'; // OK

		auto&& s4 = retString(); // s4 extends lifetime of returned string
		std::cout << 4 << '\n'; // OK

For a string view this safety is not given. It does neither copy nor extend the lifetime of a return value:

	std::string_view sv = retString(); // sv does NOT extend lifetime of returned string
	std::cout << sv << '\n'; // RUN-TIME ERROR: returned string destructed

The problem is the same as when calling:

	const char* p = retString().c_str();
	or:
	auto p = retString().c_str();

*/

/*
*  Dersin kalan kısmında C++ 17 ile hayatımıza giren optional sınıf şablonu anlatıldı.
* 
*  Optional sınıf şablonu bir nesneyi tutmak veya tutmamak için vardır.Implementasyonun da optional sizeof değeri tutulacak 
*  nesnenin türü ve bir nesneyi tuttuğunu gösteren bool flagın sizeof'u kadardır. Aşağıdaki gibi bir sınıf şablonudur,
*  bir nesneyi tutar veya tutmaz. optional header file'da bulunmaktadır, bu header'ı include etmek gerekmetedir.
*  template< class T >
*  class optional;

Aşağıdaki kodda bir string nesnesi tutuyor ve operator bool fonksiyonu sayesinde if condition kısmında
kullanılabiliyor.Bir nesne tutup tutmadığını anlamanın yolu operator bool veya has_value member fonksiyonudur.

#include <optional>
#include <string>

using namespace std;

optional<string> x{ "alican" };

if (x) {
	std::cout << "x nesnesi dolu";
}
else {
	std::cout << "x nesnesi bos";
}

*/

/*
*  optional headerın'da nullopt_t türünden bir empty class vardır. Ayrıca bu türden nullopt isimli bir nesnede
*  vardır. Optional nesnelerini boş oluşturmak için nullopt sabiti de kullanılabilir çünkü optional sınıf sablonunun
*  nullopt_t türünden parametreli bir ctor'u vardır. Aşağıda küçük bir örneği gösterilmiştir.

optional<string> s{ nullopt };
if (x != nullopt) {

}

Optional sınıfı bir nesneyi tutuyorsa tuttuğu nesneye * operatörü veya value member fonksiyonu ile ulaşabiliriz. Fakat
operator* fonksiyonu ile optional sınıfının tuttuğu nesneye ulaşmaya çalışırsak ve optional bir nesne tutmuyorsa
bu undefined behaviourdur ve exception throw edilmez. Fakat value fonksiyonu exception throw etmektedir.

Nesne tutan bir optional nesnesini boşaltmak için ise reset fonksiyonunu çağırabiliriz yada nullopt değerini 
atayabiliriz. Bu durumda optional nesnesi boş hale gelecek ve tuttuğu nesne destroy edilecek.

Optional sınıf nesnesi bos ise ona bir nesne atamak istediğimizde emplace fonksiyonunu kullanabiliriz.
Emplace fonksiyonunda ilgili nesne direk kendi yerinde construct edilir. Yani biz emplace fonksiyonuna optional da 
tutulacak sınıf türünden nesnenin constructuruna gönderilecek argumanı direk geçeriz, tıpkı vectorde ki emplace
fonksiyonları gibi, perfect forwardingden yararlanan. Eğer daha önceden bir nesne tutuyorsa oda destroy edilir.
Aşağpıda cpp den açıklaması var.

Constructs the contained value in-place. If *this already contains a value before the call, the contained value
is destroyed by calling its destructor.

9. Derste optional sınıfına devam edilecektir.

*/