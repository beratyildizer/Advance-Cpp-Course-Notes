/*
*	9. Derste optional sınıf sablonunda bulunanmonadic fonksiyonlardan bahsederek dersi kapatmıştık. 10. derste
*	en baştan monadic fonksiyonlar anlatıldı. 3 tane monadic fonksiyon var; transform, and_than ve or_else. Transform
*   fonksiyonundan başlayalım. transform diyor ki, programcı bana bir tane calleable'ı parametre olarak ver, bende optional 
*   da tutulan nesneyi bu calleable'a parametre olarak göndererek bu calleble'i çağırayım. Calleable'in döndürdüğünüde
*   bir optional nesnesinde sarmalayarak geri döndüreyim, yani bir optional döndüreceğim. Ama hangi optional açılımını
*   döndüreceğini bilemiyoruz. Ayrıca transformu çağırdığımız optional objesi boş ise bu transform bizim gönderdiğimiz
*   calleable'i hiç çağırmıyor, direk nullopt dönüyor.
*   
*   Not: Bu 3 monadic fonksiyon reference qualifierlar ile overloadlara sahiptir.
* 

#include <optional>
#include <iostream>

using namespace std;

int main()
{
	optional op = 15; // CTAD

	op.transform([](double x) { // transform op de sarmalanan 15 i fonksiyona arguman gönderecek ve daha sonra 2.3 ü sarmalayarak
		return 2.3;					 // bir optional nesnesi döndürecek.
		});
}

#include <optional>
#include <iostream>

using namespace std;

int main()
{
	optional op = 15; // CTAD

	auto opt = op.transform([](double x) {
		return optional{ 2 }; // calleable nesne optional döndürdüğü için transform optional<optional<int>> döndürecek.
		});
}

*/

/*
*	Peki and_then fonksiyonu nasıl çalışıyor? And_then de transform gibi bir calleable alıyor ve transform gibi bir optional döndürüyor.
*   Hatta optional da sarmalanan nesneyi calleable'a parametre göndererek ilgili calleable'ı çağırıyor. Fakat transform ile arasındaki
*   ince nuans farkı şuradadır: and_then'e geçilen calleable direk olarak bir optional nesnesi return etmelidir çünkü and_then direk olarak
*   bu optionalı return edecektir. Yani daha net ifade ile aldığı calleable'in geri dönüş değeri optional olmalıdır. Eğer bir optional
*   dönmez ise program cpprefrence'de ki ifade ile ill-formed olacaktır.
*   The return type (see below) must be a specialization of std::optional (unlike transform()). Otherwise, the program is ill-formed.
* 
*   and_then fonksiyonu da memberı olduğu optional nesnesi boş ise nullopt sarmalayan bir optional dönüyor, yani boş bir optional
*   dönüyor.


#include <optional>
#include <iostream>

using namespace std;

int main()
{
	optional op = 15; // CTAD

	auto opt = op.and_then([](double x) {
		return optional{ x * 1.2 }; // calleable nesne optional döndürdüğü için and_then optional<double> döndürecek.
		});
}

#include <optional>
#include <iostream>

using namespace std;

int main()
{
	optional op = 15; // CTAD

	auto opt = op.and_then([](double x) -> optional<double> {
		return  x * 1.2 ; // calleable dan direk olarak optional döndörmek yerine trailing return type dan yararlanmak yaygın olan.
		});
}

*/


/*
*	Diğer fonksiyon ise or_else. or_else de arguman olarak bir calleable alır ve optional nesnesi boş ise calleable in geri döndürdüğü
*   değeri direk return eder. Fakat dolu ise çağrılan optional nesnesini döndürecektir.
* 
*   Derste sorulan soru: hocam transform ve and_then için *this boş oldugunda bu iki fonksiyon nullopt_t mi döndürecek 
*	yoksa optional<nullopt_t> türü mü döndürecek?
*	Cevap optional<nullopt_t>.

// Asağıdaki kodda bir initializer list tutan optional nesnesini kullanarak initializer list memberlarını bir string olarak tersten
// ekrana yazdırdık.

#include <optional>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
	optional<initializer_list<int>> op{{1,3,5,7,9,17,23} };
	
	auto x = op.transform([](initializer_list<int> list) {
		string result{};
		for (auto i : list)
			result += to_string(i);
		cout << result << '\n';
		return result;
		}).and_then([](string s) -> optional<string> {
			reverse(s.begin(), s.end());
			return s;
			});

		cout << *x << '\n';

}

*/

/*
*  Dersin kalan bölümünde variant'lar işlendi. İlk olarak Necati hoca Tagged unionların ne olduklarından bahsetti. Tagged union aslında
*  heterojen bir değere sahip yani farklı türlerden değer tutabilen fakat sorguladığımızda hangi türden değer tuttuğunu söyleyen
*  yapılardır. C ve C++ dilinde yer alan union'lar için tam olarak Tagged Union terimini kullanamayız çünkü bir sorgulama mekanizmaları
*  yoktur. Fakat variant bizim için Tagged Union yapısına hizmet eden bir standart kütüphane ögesidir ve bir variadic sınıf sablonudur.
*  
*  C++ dilinde class type denen bir tür kategorisi vardır. Bu tür kategorisinin içerisinde structlar, classlar ve union lar girmektedir. 
*  C++ dilinde C dilinden farklı olarak unionlarında ctoru vs olabilir, member fonksiyonları olabilir. Fakat C++ dilinde unionların
*  bir sınıf gibi kullanımı neredeyse yok denecek kadar azdır. Tag Union için de variant kullanılıyor ve C++ dilinde unionların
*  kullanım sıklığı giderek azalmaktadır. 
*  
*  Not: Unionlardan kalıtım yapamıyoruz.


#include <string>
#include <variant>

int main()
{
	using namespace std;

	variant<int, char, double, string> vx;
}

*
*	Yukarıda bir variant nesnesi vx oluşturulmuştur. 4 tane template argumanı vardır ve bu argumanlar hangi türden değerler tutabileceğini
*   bizim için temsil eder. Variantın tutabileceği değerlere variantın alternative type'ları denmektedir. Yukarıdaki vx nesnesi
*   4 tane alternative'den birini tutmak zorundadır. Yukarıda variantın default ctor'u çağrıldı ve bu senaryoda variant ilk alternate'yi
*   tutmaktadır, yani int.
*  
*   Necati soru; variantın bir alternative si diyelim 400 byte burada noluyor? 
*   Cevap: Bir align storage ile static olarak tutuluyor, run time da tutulmuyor. Daha sonra new placement operatoru çağrılıyor. 
*  
*   Necati hoca bu sorudan sonra bir süre new placement operatorunden bahsetti. Amacı istenilen bellek bloğunda istenilen nesneyi
*   hayata getirmek. Aşağıda küçük bir örneği verilmiştir.

class myclass {
	int buf[1024];
};
char buf[sizeof(myclass)];

myclass* p = new (buf) myclass;


*
*	STL de bir çok yerde new placement operatorunden faydalanılıyor. Örneğin vectorde ki push back fonksiyonu. vector de allocate edilmiş
*   bir alan var. size capacity'den küçük oldukça containera eklenen ögeler contaenarın end'ine ekleniyor. Daha önceden allocate edilen
*   yerde containera eklenen nesneyi construct etmek için push_back fonksiyonunda new placement operatoru kullanılıyor.
*/

/*
*  Aşağıdaki kodda olduğu gibi variant nesnesinin ctoruna arguman verip alternative yi değiştirebiliriz.

#include <string>
#include <variant>

int main()
{
	using namespace std;

	variant<string, int, double> vx(3.4);
}

*/

/*
*  Variant sınıf sablonunun index isimli bir fonksiyonu vardır. Bu fonksiyon variantta tutulan alternative'nin zero based olarak
*  index numarasını döndürmektedir. Örneğin aşağıdaki kodda ekrana 1 yazacaktır.

#include <string>
#include <variant>

int main()
{
	using namespace std;

	variant<string, int, double> vx(3);

	cout << vx.index();
}

Not: Utility tuple de mesela template argumanı olarak tür alıyordu. Fakat tuple aldığı türlerden birer tane nesne olarak hepsini tutuyor.
Tuple pair'in çoklusu gibiydi. Ama hatırlarsak tuple de ilgili nesnelerden birine get fonksiyon şablonu ile erişebiliyorduk. Variant için de
benzer kullanımda bir get fonksiyonu var. Get fonksiyon sablonu ile type veya non-type parametre kullanarak tutulan alternate erişebiliriz.

Return value
Reference to the value stored in the variant.
Throws std::bad_variant_access on errors if type or non type template argumens is not stored in variant.

#include <string>
#include <variant>

int main()
{
	using namespace std;

	variant<string, int, double, string> vx(3.); // böyle bir variant kullanımı syntax hatası değil ama çok karşımıza çıkmaz.

	get<string>(vx); // compile time hatası.
}

#include <string>
#include <variant>

int main()
{
	using namespace std;

	variant<string, int, double> vx(3.); 

	get<3>(vx); // compile tine hatası, en büyük index 2.
}

#include <string>
#include <variant>

int main()
{
	using namespace std;

	variant<string, int, double> vx(3.);

	get<1>(vx); // Throws std::bad_variant_access.
}


Not: Şu ana kadar öğrendiğimiz araçlar ile variant'ta tutulan alternate'e erişmenin yolu get veya index fonksiyonlarıyla bir
if else bloğu oluşturmaktır.
*/

/*
*  Variant'da tutulan nesneyi öğrenmenin bir diğer yolu da holds_alternative fonksiyonunu kullanmaktır. Fakat bu fonksiyon
*  sadece type template argumanı alır. Fonksiyon eğer alternate bizim verdiğimiz type ise true dönerken değilse false döner.
*  Aşağıda örnekleri verilmiştir.

#include <string>
#include <variant>

int main()
{
	using namespace std;

	variant<string, int, double> vx;

	holds_alternative<int>(vs); // False
}

#include <string>
#include <variant>

int main()
{
	using namespace std;

	variant<string, int, double> vx;

	if (holds_alternative<string>(vs)) {

	}
	else if (holds_alternative<int>(vs)) {

	}
	else {

	}
}

*/

/*
*  Variant'te tutulan nesneyi anlamanın bir diğer yolu da get_if fonksion sablonunu kullanmaktır. Bu fonksiyon sablonuda
*  type ve non type parametre alır. Aldığı parametre variante'da tutulan nesne ise onun adresini geri dönerken değil ise
*  null pointer dönmektedir.


#include <string>
#include <variant>

int main()
{
	using namespace std;

	variant<string, int, double> vx("ayse");

	vx = 23.34;
	vx = 12;

	if (int* ptr = get_if<int>(&vx)) {
		cout << *ptr << '\n';
	}
	else if (double* ptr = get_if<double>(&vx)) {
		cout << *ptr << '\n';
	}
	else if(string* ptr = get_if<string>(&vx)) {
		cout << *ptr << '\n';
	}
}

Eğer ben variant'da tutulan alternate'nin ne oldugunu bilmiyorsam önce index sonra get fonksiyonuyla bunu öğrenebilirim. 
Yada hold_alternative fonksiyonunu kullanabilirim. Bunu da istemezsem yukarıdaki gibi get_if'i kullanabilirim. 

Peki ya variant nesnesini sizeof'u ne olacak. Aşağıda örneği verilmiştir.

#include <string>
#include <iostream>
#include <variant>

struct X { char buf[16]{}; };
struct Y { char buf[32]{}; };
struct Z { char buf[64]{}; };
struct T { char buf[128]{}; };

int main()
{
	using namespace std;

	variant<X,Y,Z,T> vx;  

	cout << sizeof(vx); //ekrana 129 yazdıracaktır, çünkü en büyük size'a sahip elemanla neyi tuttuğunu gçsteren tek byte lik
						// bir index var.
}

*/

/*
*  variant_size isimli bir meta sınıf sablonu vardır. Bu sınıf sablonunun açılımını kullanarak variant da kaç farklı alternate
*  tutuluyor bunu ögrenebiliyoruz. Aşağıda örneği gösterilmiştir.

#include <string>
#include <iostream>
#include <variant>

int main()
{
	using namespace std;

	using var = variant<int, char, double, string>;

	cout << variant_size<var>::value; // ekrana 4 yazdıracaktır.

}


*  Yukarıdaki value^ye ulaşmak için _v uzantılı bir tane variable template'de vardır. Aşağıda possible impl'si gösterilmiştir.


template<typename T>
constexpr size_t VariantSize_v = variant_size<T>::value;

#include <string>
#include <iostream>
#include <variant>

int main()
{
	using namespace std;

	using var = variant<int, char, double, string>;

	cout << variant_size_v<var>; // ekrana 4 yazdıracaktır.

}

*/

/*
*  Variant ta tutulacak türleri index'e göre veren de bir meta function vardır. Aşağıda örneği gösterilmiştir.


#include <string>
#include <iostream>
#include <variant>

int main()
{
	using namespace std;

	using var = variant<int, char, double, string>;

	variant_alternative<0, var>::type x; // 0. indeks de ki türü döndürür. X in türü int olacak.

}

*  _t uzantılı bir alias template'e sahiptir.


#include <string>
#include <iostream>
#include <variant>

int main()
{
	using namespace std;

	using var = variant<int, char, double, string>;

	variant_alternative<0, var>::type x; // 0. indeks de ki türü döndürür. X in türü int olacak.
	variant_alternative_t<0, var> x; // 0. indeks de ki türü döndürür. X in türü int olacak.

}

Not: Yukarıda variantta tutulabilecek alternate sayısını ve türü vere templatelerde olduğu gibi genel bir kuralı hatırlatmak
iyi olur. Variable hesaplayan templateler için genelde _v ile biten variable templatedeler de genel var iken tür hesaplayan template'ler
içinde _t ile biten alias templateler genelde vardır. 

Not: Atama operator fonksiyonları ile alternateler türünden bir nesneyi direk olarak bir variant'e atayabiliriz.

*/

/*
*   Biz variant nesnesinin bir nesne tutup tutmadığını öğrenmek isteyebiliriz. Bu durumda yayın konvensiyon monostate türü
*	kullanmak. Bu sadece bir tür, empty bir class, sadece global operatoru karşılaştırma fonksiyonlarına sahip. Aşağıdaki gibi
*	ilk argumanı monotstate yaparım ve monostate tuttuğu her durum benim için boş anlamına gelir. Daha sonra bu boşluğu hold_alternate,
*	get, get if ve operato= fonksiyonlarıyla check edebilriim, boş mu dolu mu diye.

#include <string>
#include <iostream>
#include <variant>

int main()
{
	using namespace std;

	using var = variant<monostate, int, char, double, string>;
	var vx;
	
	vx = 4.5;
	//

	vx = monostate();

}

*/


/*
*  Variant nesnesini tanımlarken variant'ta tutulabilecek alternatelerden birinin ctorune direk olara variant nesnesi hayata gelirken
*  arguman geçmek istersem in_place_index isimli sınıf sablonunu kullanabilirim.Bu sınıf sablonunun tek amacı budur ve bir tag class dır
*  benim için. Aşağıda örneği verilmiştir.


#include <string>
#include <iostream>
#include <variant>

class myclass {
public:
	myclass(int a, int b);
};

int main()
{
	using namespace std;

	using var = variant<int, myclass, string>;
	var vx(in_place_index<1>, 3,6);  // non type parameter index amaçlı kullanılıyor, myclass'ı belirtiyor

}

*/

/*
*  Variant'ın interfacesinde birde emplace fonksiyon sablonu vardır. Overloadlarından birisi type parametre alırken bir diğeri
*  non type parametre alır. Emplace çağrısı ile variant'a tuttuğu alternateyi destroy ettirip (dtor çağrılacak) benim istediğim
*  alternate'yi ctor edip tutacaktır. Zaten bütün vocabulary typelar için (any, optional variant) emplace fonksiyonunun kullanımı benzerdir.
* 

#include <string>
#include <iostream>
#include <variant>

class myclass {
public:
	myclass(int a, int b);
};

int main()
{
	using namespace std;

	using var = variant<int, myclass, string>;
	var vx("ali");  

	vx.emplace<myclass>(12, 24);

}

#include <string>
#include <iostream>
#include <variant>

class myclass {
public:
	myclass(int a, int b);
};

int main()
{
	using namespace std;

	using var = variant<int, myclass, string>;
	var vx;

	vx.emplace<2>(100, 'a'); // fill ctor

}

Yukarıdaki iki örnekle alternate'nin direk olarak ctoruna arguman göndereiyoruz. Eğer biz atama operator fonksiyonlarını kullansaydık
direk olarak alternate'lerin ctoruna arguman geçemezdik.

*/

/*
* 
Visitor Pattern : GoF patternlerden biri.Standard kütüphanede bir tane visit fonksiyonu var, bu visit fonksiyonu ilk 
argumanına bir calleable alıyor, daha sonraki argumanları da birer variant olmak zorunda. Variadic bir fonksiyondur.
Buradaki variant da tutulan alternate’yi bu calleable’a gönderiyor.Bu alternate yi bulma yöntemine takılmayalım ama
bizim ki gibi bir if else merdiveni vs olabilir.Burada önemli olan nokta bütün olabilecek alternate türlerinin ilgili 
fonksiyon için syntax hatası olmaması lazım.

*

#include <string>
#include <iostream>
#include <variant>


int main()
{
	using namespace std;

	using var = variant<int, double, string>;
	var vx("Nurhayat");

	visit([](auto x) {cout << x; }, vx);  // vx de tutulan variant ilgil calleabla nesneye arguman olarak gönderiliyor.

}

#include <string>
#include <iostream>
#include <variant>

class ThreeFold {
public:
	template <typename T>
	void operator()(T& x) {
		x = x + x + x;
	}
};

int main()
{
	using namespace std;

	variant<int, double, string> vx = 56;

	visit(ThreeFold{}, vx);
	visit([](const auto& a) {cout << a << '\n'; }, vx); 
}



#include <string>
#include <iostream>
#include <variant>


int main()
{
	using namespace std;

	variant<int, double, string> vx = 56;

	auto f = [](auto x) {
		if constexpr (is_same_v<decltype(x), int>) {
			// some code
		}
		else if contexpr(is_same_v<decltype(x), double>) {
			// some code
		}
		else if constexpr (is_same_v<decltype(x), string>) {
			// some code
		};
	}

	visit(f, vx);
}

*/