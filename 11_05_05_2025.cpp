/*
*  Vocabulary type türlerinden biri olan variant sınıf sablonundan devam ediyoruz. Dersin başında Necati hoca
*  variant ile ilgili bazı küçük hatırlatmaları tekrardan yaptı. Variant n tane türden seçilmiş bir değeri tutuyordu.
*  Tuttuğumuz değeri get, hold_alternates vb. fonksiyonlar ile sorgulayabiliyorduk. Klasik union'ların biraz daha modernleşmiş
*  halidir. En son visit fonksiyonunda kalmıştık. Visit nasıl bir fonksiyondu? İlk argumanı bir calleable olmak zorundaydı. Daha
*  sonra fonksiyona istediğimiz sayıda variant'ı arguman olarak verebiliyorduk. Visit fonksiyonuda arguman olarak verdiğimiz
*  variant'ta tutulan nesneleri calleable'a arguman olarak geçirip çağırır.
*
*  Visit fonksiyonundan bahsetmeden önce Necati hoca bir variant nesnesini default ctor edilmiş hale getirmenin
*  bir örneğini yaptı. Bunu yaparkende visite geçeceği calleable nesneyi yani aşağıda bu bir closure type, bunun
*  operator() fonksiyonunu auto ile template olarak sol taraf referansı ile yazdı. Daha sonra remove_reference_t'yi
*  kullanarak referanslığı kaldırıp o türden bir temporary object oluşturup tekrardan variantta tutulan nesneyi atandı.
*  Yazarak anlatmak zor, aşağıda ki kod okunarak anlaşılabilir.

#include <iostream>
#include <variant>
#include <string>

using namespace std;

int main() {
	using std::cout;

	variant<int, string, double> vx("Mustafa");

	cout << get<1>(vx) << '\n';
	auto f = [](auto& val) {
		val = std::remove_reference_t<decltype(val)>{};  // variantte tutulan nesne türünden bir temp object default ctor edilerek
	};												     // tekrardan variant da tutulan nesneye atılıyor.

	visit(f, vx);
	cout << get<1>(vx).size() << '\n';

}

*/

/*
*  Visit fonksiyonu ile yapılabilen bir overload idiomu vardır. Bu idiomu anlatmadan önce public kalıtımla ilgili küçük
*  hatırlatmalar yaptı.
*
*  Not: Multiple inheritance'de yani çoklu kalıtımda farklı base sınıflarda aynı isimde fonksiyonlar olursa ve o fonksiyon
*  çağrılırsa bir function overloading olmaz, syntax hatası olur. Sebebi de ambiguity hatasıdır.


struct A { void foo(int); };
struct B { void foo(double); };
struct C { void foo(long); };

struct Der : A, B, C {

};

int main()
{
	Der myder;
	myder.foo(12); // ambiguity.
	myder.B::foo(14.5); // legal.
}

Yukarıdaki örnekler dısında aşağıdaki gibi using anahtar kelimesini kullanarak fonksiyonları inject ederek de
bu sorunu çözebiliriz.

struct A { void foo(int); };
struct B { void foo(double); };
struct C { void foo(long); };

struct Der : A, B, C {
	using A::foo;
	using B::foo;
	using C::foo;
};

int main()
{
	Der myder;
	myder.foo(24); // legal..
}

*/

/*
*  Aşağıdaki örnek ile A, B ve C sınıflarında türetilmiş bir sınıfı parameter pack ile compiler'a instantiate
*  ettirebildik. Parameter pack, variadic template vs konularını Necati hoca kursun ilerleyen saatlerinde çok detaylı
*  ele alacaktır.

struct A { void foo(int); };
struct B { void foo(double); };
struct C { void foo(long); };

template<typename ...Args>
struct Der : Args ...{
	using Args::foo...; // foo() olmadığına dikkat et.
};

int main()
{
	Der<A, B, C> myder;
}

*/

/*
*  Not: C++ 20 den önce closure typelar için derleyici sınıfın default constructorunu ve operator= fonksiyonlarını
*  delete ediyordu. Fakat C++ 20 ile birlikte stateless lambdalar için (capture yapmayan) bu fonksiyonlar artık delete edilmiyor.
*  Buda bize bu closure typelardan bir nesne oluşturma ve bu closure typeları base class olarak kalıtım da kullanabilme
*  imkanını sunmuştur. Aşağıda bunun bir örneği verilmiştir.

auto f = [](int x) {return x * x; };

struct Der : decltype(f) {

};

*/

/*
*  Şimdi overloader idiomuna bakalım. Bu idiomu variant için uygularken C++ 20 ile gelen stateless lambdalardan
*  bir nesne oluşturma ve bu closure typelarda base class olarak kalıtım da kullanabilmeden faydalanıyoruz. Aşağıda
*  Bununla ilgili bir örnek verilmiştir. Overload isimli bir variadic template function tanımlanmıştır. Bu variadic template'nin
*  lambda ifadelerden kalıtımlanmış bir açılımını elde edebildik main içerisinde. Bunu yaparkende aggragate type init
*  syntaxından faydalandık. Bu sayede biz types nesnesini (Overload fonksiyon sablonunun bir template açılımı) bir calleable
*  gibi kullanabildik ve farklı nesneler için farklı fonksiyonlar çağrılmış oldu. Virtual Dispatch mekanizmasını anımsattı:)
*
*  Not: Tekrardan hatırlatıyorum. Closure typeları base class olarak kullanabilmenin yolu stateless lambdalar ile C++ 20
*  de dile eklenen kuraldır, bunu yukarılarda açıkladım.

#include <iostream>
#include <variant>
#include <string>

using namespace std;

template<typename ...Args>
struct Overload : Args...{
	using Args::operator()...; // burada base sınıfların operator() fonksiyonları injecte edilmiştir.
};

struct X {};
struct Y {};
struct Z {};

int main()
{
	auto types = Overload{ // burada aggregate type init syntaxından faydalanıyoruz.
	 [](int) { return "int"; },
	 [](char) { return "char"; },
	 [](unsigned) { return "unsigned"; },
	 [](float) { return "float"; },
	 [](X) { return "X"; },
	 [](Y) { return "Y"; },
	 [](Z) { return "T"; },
	 [](auto) { return "unknown type"; },
	};

	cout << types(12);
}

*/

/*
*  Peki yukarıdaki Overload idiomu ile variant'ların yada visit'in alakası nedir?
*
*  Necati hoca aşağıdaki kodu derste yazdı. Aşağıdaki kod gerçekten çok öğretici bir kod. Şöyle ki öyle bir
*  vector nesnesi oluşturduk ki sanki farklı türlerden nesneler tutabilen bir kod oldu. Tabi aslında hikaye öyle değil.
*  Arka tarafta vector'un variant açılımını biz instantiate ettik. Daha sonra bu vectorde tutulan variant nesnelerini
*  for range based loop ile sırasıyla visit fonksiyonuna arguman olarak gönderdik. Visit fonksiyonuna gönderdiğimiz calleable
*  için de yukarıdaki Overload idiomun dan faydalandık. Bu sayede vectorde tutulan variant nesnelerinde farklı türden nesnelerde
*  tutuluyor olsa aynı calleable hepsi için kullanılmış oldu. Run time'a maliyet getirmeyen adeta compile time da çalışan
*  polimorfizim yapmışız izlenimi verdi:)

#include <variant>
#include <vector>
#include <iostream>

template<typename ... Args>
struct Overload : Args ...
{
	using Args::operator() ...;
};

template<class... Args>
Overload(Args...)->Overload<Args...>;

struct Nec {};
struct Erg {};

int main()
{
	using var_type = std::variant<int, char, unsigned, float, double, long long, Nec, Erg>;

	auto stypes = Overload{
		[](int) { return "int"; },
		[](char) { return "char"; },
		[](unsigned) { return "unsigned"; },
		[](float) { return "float"; },
		[](double) { return "double"; },
		[](long long) { return "long long"; },
		[](Nec) { return "Nec"; },
		[](auto) { return "unknown type"; },
	};

	std::vector<var_type> varvec{ 23, 'A', 45U, 5.f, 6.4, 34LL, Nec{}, Erg{} };

	for (auto v : varvec) {
		std::cout << std::visit(stypes, v) << '\n';
	}
}

Derste burayla ilgili sorulan bir soru:

O zaman virtual dispatchsiz compile time inheritance gerceklestirme yontemlerinden biri de overloader idiom'dur denebilir degil mi?

Not: Open Closed Principle: Değiştirmeye kapalı, eklemeye açık. Necati hoca yukarıdaki soruya Open Closed principle'yi ihlal
ettiği için hayır cevabını verdi. Yani şöyle ki ben başka bir tür için de bir overload eklemek istesem mevcut yazılmış kodu yukarıda
edit etmem gerekiyor. Fakat inheritance de öyle değildi. Ben bir türemiş sınıfta taban sınıfın sanal fonksiyonunu direk olarak override
edebilirim ve bunu yaparken taban sınıfı değiştirmem. Yani kalıtımda eski kodları kullanabilirim ve open closed principle'e uymus olurum
fakat yukarıdaki kodda bunu yapamam.

*/

/*
*  Aşağıda derste yazılan kod bloğu bulunmaktadır. 3 farklı user defined sınıf türü vardır ve bu sınıf türlerinin her birinin
*  bir tane member fonksiyonu vardır. Bir tane PetCry sınıfı tanımlanmıştır ve bu sınıf içerisinde de 3 farklı operator() fonksiyonu
*  overload edilmiştir. Biz vectorun variant<Cat, Dog, Lambda> açılımı türünden bir nesne oluşturuyoruz ve daha sonra
*  bu vectorde bu türlerden birini tutan variant nesneleri tutuyoruz. Visit fonksiyonuna PetCry sınıfından bir geçisi nesneyi
*  calleable olarak veriyoruz, ikinci argumana da vector de tutulan variant nesneleri sırayısla gönderiyoruz. Aşağıdaki yapının
*  avantajı bizim için bağımlılığın olmaması ve herşeyin compile time da hesaplanmasıdır.

#include <variant>
#include <vector>
#include <iostream>
#include <string>

class Cat {
public:
	Cat(const std::string name) : m_name{ std::move(name) } {}
	void meow()const {
		std::cout << m_name << " miyavliyor\n";
	}
private:
	std::string m_name;
};

class Dog {
public:
	Dog(const std::string name) : m_name{ std::move(name) } {}
	void woof()const {
		std::cout << m_name << " havliyor\n";
	}
private:
	std::string m_name;
};

class Lamb {
public:
	Lamb(const std::string name) : m_name{ std::move(name) } {}
	void bleat()const {
		std::cout << m_name << " meeliyor\n";
	}
private:
	std::string m_name;
};

using Pet = std::variant<Cat, Dog, Lamb>;

struct PetCry {
	void operator()(const Cat& cat) { cat.meow(); }
	void operator()(const Dog& dog) { dog.woof(); }
	void operator()(const Lamb& lamb) { lamb.bleat(); }
};

int main()
{
	using namespace std;

	vector<Pet> vec{ Cat{"minnos"}, Dog{"soko"}, Lamb{"kuzucuk"}, Lamb{"pamuk"} };

	for (const Pet& p : vec) {
		visit(PetCry{}, p);
	}
}

*/

/*
*  Bir variant nesnesi valueless durumunda olabiliyor. Peki bu nasil olabilir? Variant nesnesi ile ilgili bazı islemler, mesela
*  emplace gibi işlemler buna neden olabilir. Örneğin variant da tutulan nesneyi dtor ettik ve yen değer ctor edilirken exception
*  throw edildi. İşta burada variant nesnesi valueless durumunda olacaktır. Variant nesnesinin böyle bir durumda olup olmadığını
*  valueless_by_exception fonksiyonu vardır ve bunu kullanabiliriz. Bu fonksiyon true değer döner ise variant bu durumda,
*  false değer dönerse variant bu durumda değil.
*
*  Aşağıdaki kod bu duruma güzel bir örnektir.
*/

/*
*
#include <variant>
#include <vector>
#include <iostream>
#include <string>

struct S {
	operator int()const {
		if (true)
			throw std::runtime_error("error from S::operator int\n");
		return 1;
	}
};

int main()
{
	using namespace std;

	variant<double, int> var(3.4);

	try{
		var.emplace<1>(S{});
	}
	catch (const std::exception& ex) {
		cout << ex.what() << '\n';
	}

}

*/

/*
*  Derste sorulan bir soru üzerine perfect forwarding mekanizması ve forward fonksiyonunun nasil çalıştığı derste tekrardan
*  anlatıldı, daha önceden not aldığım için not almadım.
*/

/*
*  3. vocabulary type any sınıfıydı. Any direk olarak bir sınıftır, bir sınıf sablonu değil. Bir nevi type erasure implementasyonudur.
*  C dilinde type erasure idiomuna en yakın araç void pointerlardı. C++ da CRTP, variant, any ve type erasure kalıtıma da birer
*  alternatif olabilir.
*
*  Bir any türünden nesne oluşturduğumuz da herhangi türden bir nesneyi bu any ile tutabiliyoruz. Peki bu nasil olabiliyor. Aşağıda
*  örnek kod olarak gösterilmiş universal reference parametreli bir ctor'u var. Ayrıca universal reference türünden parametresi olan
*  operator= fonksiyonları var(copy ctor, copy assigment). Bu sayede her türden bir nesneyi any de sarmalayıp tutabiliyorum.
*
*  Peki any'de her türden bir nesne tutabiliyoruz da ilgili anda any sınıfında hangi türden nesnenin tutulacağını nasıl anlayacağız?
*  Any_cast isimli bir fonksiyon sablonu vardır ve bu fonksiyon sablonu bir tane type parametre ile oluşturulmuştur. Aldığı parametreden
*  bir nesne tutuyorsa onu geri döndürürken tutmuyorsa bad_any_cast türünden bir exception throw eder. Aşağıda küçük bir örneği
*  gösterilmiştir.

#include <any>
#include <vector>
#include <iostream>

class myclass{};

int main()
{
	using namespace std;

	any x = 12;

	try{
		std::cout << any_cast<double>(x) << '\n';
	}
	catch(const std::exception& ex) {
		std::cout << ex.what() << '\n';
	}
}

*/

/*
*  Dersin çok küçük bir bölümünde dynamic_cast operatoruna atıf da bulunuldu.Run time type identification için C++ içerisinde kullanılan
*  bir operatordur. Arguman olarak bir sanal sınıf almak zorundadır, yoksa compile time da kod derlenmyecektir. Aldığı arguman
*  açısal parantez de yazılan tür ise dönüşüm gerçekleşecektir, değil ise nullptr üretecektir.
*
void Add(B* b)
	{
		A* obj = dynamic_cast<A*>(b);
		if(obj != nullptr)
			// code...
	}
*/

/*
*  Any sınıfının içerisinde type isimli bir member fonksiyon vardır. Bu fonksiyon typeid operatorü gibi type_info döndürür. Bu geri
*  dönüş değeri kullanılarak da any de tutulan türü anlayabiliriz. aşağıda küçük bir örneği gösterilmiştir.


#include <any>
#include <vector>
#include <iostream>

class myclass {};

int main()
{
	using namespace std;

	any x = 12;

	if (x.type() == typeid(int)) {
		cout << "int tutuyor\n";
	}
	else if (x.type() == typeid(double)) {
		cout << "double tutuyor\n";
	}
	else {
		cout << "ne bileyim ne tutuyor\n";
	}
}

*/

/*
*  Any sınıfının interfacesinde çok fazla fonksiyon yoktur. reset, emplace, swap modifier fonksiyonlardır. Birde type ve has_value
*  fonksiyonları vardır. Derste bu fonksiyonlar tanıtıldı. (type fonksiyonunu yukarda öğrendik zaten). Bu member fonksiyonların
*  yaptıkları diğer vocabulary typelardaki member fonksiyonlar ile aynıdır. Dolayısıyla tek tek not almadım.
*
*/

/*
*  Aşağıdaki örnek derste yazılmısti. emplace kullanımına güzel bir örnek. Ayrıca containerlar la kullanımına da güzel bir örnek.
*  En son ki emplace fonksiyonu çağrısında set containar ın int, callebla açılımından bir nesne tutmuştur. Burada calleable
*  sorter olarak kullanılıyordu set containerında. Ayrıca verdiğimiz calleable'i set containerının ctoruna göndermek için
*  emplace fonksiyonuna arrguman olarak da geçmemiz gerekiyor.


#include <any>
#include <string>
#include <vector>
#include <set>

class Nec
{
	int mx, my;
public:
	Nec(int x, int y) : mx{ x }, my{ y } { }
	//...
};

int main()
{
	auto fcmp = [](int x, int y) {return abs(x) < abs(y); };
	std::any a;

	a = 12; //int
	a = Nec{ 10, 20 }; //Nec
	a.emplace<Nec>(2, 5); //Nec
	a.emplace<std::string>(10, 'A'); //string
	a.emplace<std::vector<int>>(100); //vector<int>
	a.emplace<std::set<int, decltype(fcmp)>>({ 1, 5, -4, -6, 3 }, fcmp);

}

*/

/*
*  any sınıfında tutulacak nesnenin direk olarak ctoruna arguman göndermek için diğer vocabulary typelar da olduğu gibi in_place_type
*  sınıf sablonunu kullanabiliriz. Aşağıda bununla ilgili bir örnek verilmiştir. Yukarıdaki gibi set container kullanılmıştır.

#include <any>
#include <set>
#include <iostream>

class myclass {};

int main()
{
	using namespace std;

	auto fcmp = [](int x, int y) {return abs(x) < abs(y); };
	
	std::any a{ in_place_type<set<int, decltype(fcmp)>>, {3,7,9,12}, fcmp };


}

*/
/*
*	ASAGIDA KALAN KISIM NECATI HOCANIN ANY İLE İLGILI HAZIRLADIGI NOTLARDAN ALINMISTIR. HERSEY COK DETAYLI ACIKLANMIS
*/
/*
* 
Öyle bir nesne olsun ki istediğimiz herhangi türden bir değeri tutabilsin.İstediğimiz zaman nesnemizin tuttuğu değeri herhangi 
türden bir değer olarak değiştirebilelim.C++17 standartları ile dile eklenen std::any sınıfı işte bu işe yarıyor.

C++ dilinin sağladığı en önemli avantajlardan biri tür güvenliği(type safety).Yazdığımız kodlarda değer taşıyacak nesnelerimizi
bildirirken onların türlerini de belirtiyoruz.Derleyici program bu bildirimlerden edindiği bilgi ile nesne üzerinde hangi işlemlerin yapılabileceğini derleme zamanında biliyor ve kodu buna göre kontrol ediyor.C++ dilinde değer taşıyan nesnelerin türleri programın çalışma zamanında hiçbir şekilde değişmiyor.

std::any sınıfı herhangi bir türden değer tutabilirken bir değer türü(value type) olarak tür güvenliği de sağlıyor.any bir sınıf 
şablonu değil.Bir any nesnesi oluşturduğumuzda onun hangi türden bir değer tutacağını belirtmemiz gerekmiyor.any türünden bir nesne
herhangi bir türden değeri tutabilirken sahip olduğu değerin türünü de biliyor.Peki bu nasıl mümkün oluyor ? Yani nasıl oluyor da bir
nesne herhangi türden bir değeri saklayabiliyor ? Bunun sırrı any nesnesinin tuttuğu değerin yanı sıra bu değere ilişkin typeid değerini
de(bir std::type_info nesnesini) tutuyor olması.

any sınıfının tanımı any isimli başlık dosyasında :

namespace std {
	class any {
		//...
	};
}
any nesnelerini oluşturmak
Bir any nesnesi belirli türden bir değeri tutacak durumda ya da boş olarak yani bir değer tutmayan durumda hayata getirilebilir :

#include <string>
#include <any>
#include <bitset>
#include <vector>

int main()
{
	using namespace std::literals;

	std::any a1{ 12 }; //int
	std::any a2 = 4.5; //double
	std::any a3{ "necati" }; //const char *
	std::any a4{ "necati"s }; //std::string
	std::any a5{ std::bitset<16>{} }; //std::bitset<16>
	std::any a6{ std::vector<int>{1, 3, 5} }; //std::vector<int>
	std::any b1;  //boş
	std::any b2{}; //boş
	std::any b3 = {}; //boş
}
any sınıf nesnesinin kurucu işlevine gönderilen argümandan farklı türden bir değeri tutabilmesi için kurucu işlevin ilk parametresine 
standart <utility> başlık dosyasında tanımlanan in_place_type<> argümanının gönderilmesi gerekiyor.any tarafından tutulacak nesnenin
kurucu işlevine birden fazla değer gönderilmesi durumunda da yine in_place_type<> çağrıdaki ilk argüman olmalı :

#include <any>
#include <string>
#include <complex>
#include <set>

int main()
{
	using namespace std;

	any a1{ "alican" };  //const char *
	any a2{ in_place_type<string>, "necati" }; //string
	any a3{ in_place_type<complex<double>>, 4.5, 1.2 //complex<double>
	auto fc = [](int a, int b) {return std::abs(a) < std::abs(b); };
	any a4{ in_place_type<set<int, decltype(fc)>>, {1, 4, 5}, fc }; //set<int>
	//...
	}
		std::make_any<> yardımcı işlevi
		any türünden bir nesne oluşturmanın bir başka yolu da make_any<> yardımcı fabrika işlevini kullanmak.Burada any 
		nesnesinin tutacağı değerin türü şablon tür argümanı olarak seçildiğinden in_place_type<> yardımcısının kullanılması gerekmiyor :


#include <any>
#include <string>
#include <complex>

	int main()
	{
		using namespace std;

		auto a1{ make_any<string>(10, 'X') };
		auto a2{ make_any<complex<double>>(1.2, 4.5) };
		//...
	}
	any nesneleri için bellek ihtiyacı
	Bir any sınıf nesnesi tarafından tutulacak değerin bellek gereksinimi(storage) 1 byte da olabilir 5000 byte da.any nesnesi 
		sahip olacağı değeri tutmak için heap alanında bir bellek bloğu edinebilir.Bu konuda derleyiciler istedikleri gibi kod 
		üretebiliyorlar.Derleyiciler tipik olarak doğrudan any nesnesi içinde bir bellek alanını görece olarak küçük nesnelerin 
		tutulması amaçlı kullanıyorlar. (C++17 standartları da böyle bir gerçekleştirimi öneriyor.) Eğer any tarafından saklanacak 
		değer bu bellek alanına sığıyor ise değer bu alanda tutuluyor.Bu tekniğe "küçük tampon optimizasyonu" (small buffer optimization
			SBO) deniyor.Saklanacak nesne bu bellek alanına sığmıyor ise heap alanından bir bellek bloğu elde ediliyor.Aşağıda programı
		kendi derleyiciniz ile derleyerek çalıştırın ve any nesneleri için sizeof değerinin ne olduğunu görün :

#include <any>
#include <iostream>

	int main()
	{
		std::cout << "sizeof(any) : " << sizeof(std::any) << '\n';
	}
	Benim farklı derleyiciler ile yaptığım testlerin sonucu şöyle oldu :

	GCC 8.1                        16
		Clang 7.0.0                    32
		MSVC 2017 15.7.0 32 - bit        40
		MSVC 2017 15.7.0 64 - bit        64
		any nesnesinin değerini değiştirmek
		Sınıfın atama operatör işlevi ya da emplace<> işlev şablonu ile bir any nesnesinin değeri 
		değiştirilebilir.Aşağıdaki kodu inceleyin :

#include <any>
#include <string>
#include <vector>
#include <set>

	class Nec
	{
		int mx, my;
	public:
		Nec(int x, int y) : mx{ x }, my{ y } { }
		//...
	};

	int main()
	{
		using namespace std;

		auto fcmp = [](int x, int y) {return abs(x) < abs(y); };
		any a;
		a = 12; //int
		a = Nec{ 10, 20 }; //Nec
		a.emplace<Nec>(2, 5); //Nec
		a.emplace<string>(10, 'A'); //string
		a.emplace<vector<int>>(100); //vector<int>
		a.emplace<set<int, decltype(fcmp)>>({ 1, 5, -4, -6, 3 }, fcmp);
	}
	any nesnesini boşaltmak
		Bir değer tutan any nesnesini boşaltmak için sınıfın reset isimli işlevi çağrılabilir :

	a.reset();
	Bu çağrı ile any türünden a değişkeni eğer boş değil ise a değişkeninin tuttuğu nesnenin hayatı sonlandırılıyor.Bu işlemden
		sonra a değişkeni boş durumda.any nesnesini boşaltmanın bir başka yolu da ona varsayılan kurucu işlev(default constructor)
		ile oluşturulmuş bir geçici nesneyi atamak :

	a = std::any{};
	Atama aşağıdaki gibi de yapılabilir :

	a = {};
	any nesnesinin boş olup olmadığını sınamak
		Bir any nesnesinin boş olup olmadığı yani bir değer tutup tutmadığı sınıfın has_value isimli üye işleviyle sınanabilir
		. (boost::any sınıfında olan empty üye işlevi yerine has_value ìsminin tercih edilmiş olması ilginç.)

		bool has_value()const noexcept;
	Aşağıdaki koda bakalım :

#include <any>
#include <iostream>

	int main()
	{
		using namespace std;

		cout.setf(ios::boolalpha);
		any a;
		cout << a.has_value() << '\n'; //false
		a = 45;
		cout << a.has_value() << '\n'; //true
		a.reset();
		cout << a.has_value() << '\n'; //false
		a = false;
		cout << a.has_value() << '\n'; //true
		a = {};
		cout << a.has_value() << '\n'; //false
	}
	std::any_cast<> işlev şablonu
		any nesnesinin tuttuğu değere erişmenin tek yolu onu global any_cast<> işleviyle tuttuğu değerin türüne 
		dönüştürmek._any_cast<> _işleviyle, any sınıf nesnesi bir değer türüne bir referans türüne ya da bir adres türüne
		dönüştürülebilir :

#include <any>
#include <string>
#include <iostream>

	int main()
	{
		using namespace std;

		string name{ "kaan aslan" };
		any a{ name };
		string s{ any_cast<string>(a) };
		s = "oguz karan";
		cout << any_cast<string>(a) << "\n"; //kaan aslan
		string& rs{ any_cast<string&>(a) };
		rs = "necati ergin";
		cout << any_cast<string>(a) << "\n"; //necati ergin
		const string& crs{ any_cast<string&>(a) }
		crs = "ali serce"; //gecersiz
	}
	std::bad_any_cast
		any_cast<> ile yapılan dönüşüm başarısız olursa yani dönüşümdeki hedef tür any nesnesinin tuttuğu tür ile aynı değilse 
		std::bad_any_cast sınıfı türünden bir hata nesnesi gönderilir :

#include <any>
#include <iostream>

	int main()
	{
		using namespace std;

		any a{ 12 };
		try {
			int ival = any_cast<int>(a);
			cout << "ival = " << ival << '\n';
			a = 3.4;
			ival = any_cast<int>(a);
			cout << "ival = " << ival << '\n';
		}
		catch (const std::bad_any_cast& ex) {
			cout << "hata yakalandi: " << ex.what() << '\n';
		}
	}
	Burada gönderilen bad_any_cast sınıfı için türetme hiyerarşisi şöyle :

	std::exception
		^
		std::bad_cast
		^
		std::bad_any_cast
		any_cast / <> dönüştürme işlevini kullanarak any tarafından tutulan nesneye gösterici(pointer) semantiği ile de 
		erişilebilir.Ancak bu durumda şablon argümanı olarak kullanılan tür tutulan nesnenin türü değil ise bir hata nesnesi
		gönderilmez(exception throw edilmez), nullptr değeri elde edilir :

#include <any>
#include <iostream>

	int main()
	{
		using namespace std;

		any a{ 12 };
		if (int* p = any_cast<int>(&a))
			cout << *p << "\n";
		else
			cout << "tutulan nesne int turden degil\n";
		//...
	}
	tutulan nesnenin türünü öğrenmek
		Sınıfın type isimli üye işlevi ile any nesnesi tarafından tutulmakta olan nesnenin türü öğrenilebilir :

	const std::type_info& type() const noexcept;
	İşlevin geri dönüş değeri any nesnesinin tuttuğu değerin tür bilgisini taşıyan type_info nesnesi.Eğer any nesnesi boş
		ise type işlevinin geri dönüş değeri typeid(void)olur.Bu işlevle erişilen type_info nesnesi type_info sınıfının operator==
		işleviyle bir karşılaştırma işlemine sokulabilir.Aşağıdaki kodu inceleyelim :

#include <any>
#include <iostream>

	int main()
	{
		using namespace std;

		cout.setf(ios::boolalpha);
		any x;
		cout << (x.type() == typeid(void)) << '\n';
		x = 12;
		cout << (x.type() == typeid(int)) << '\n'; //true
		cout << (x.type() == typeid(double)) << '\n'; //false
	}
	any sınıfı ve taşıma semantiği
		any sınıfı taşıma(move) semantiğini de destekliyor.Ancak taşıma semantiğinin desteklenmesi için tutulan değere 
		ilişkin türün kopyalama semantiğini de desteklemesi gerekiyor.unique_ptr / gibi kopyalamaya kapalı ancak taşımaya 
		açık türlerden değerler(movable but not copyable) any nesneleri tarafından tutulamazlar.Aşağıdaki kodda string nesnesinden
		any nesnesine ve any nesnesinden string nesnesine yapılan taşıma işlemlerini görebilirsiniz :

#include <any>
#include <string>
#include <iostream>

	int main()
	{
		using namespace std;

		string name("Dennis Ritchie");
		std::any a{ move(name) }; //name stringi a'ya taşınıyor.
		cout << "a = " << any_cast<string>(a) << '\n';

		cout << "name length = " << name.length() << '\n';
		name = move(any_cast<string>(a)); // a'daki string name'e taşınıyor
		cout << "name : " << name << '\n';
		cout << "name length = " << name.length() << '\n';
	}
	any sınıfının kullanıldığı yerler
		C++17 standartları öncesinde C++ dilinde yazılan kodlarda daha önce void* türünün kullanıldığı birçok yerde any sınıfı 
		kullanılabilir. void* türünden bir gösterici(pointer) değişken, herhangi türünden bir nesnenin adresini tutabilir.Ancak 
		void* türünden bir değişken adresini tuttuğu nesnenin türünü bilmez ve onun hayatını kontrol edemez.Ayrıca void* türü bir 
		gösterici türü olduğu için "deger türü" (value type) semantiğine sahip değildir.any istenilen herhangi türden bir değeri
		saklayabilir.Tutulan nesnenin değeri ve türü değiştirilebilir.any tuttuğu nesnenin hayatını da kontrol eder ve her zaman 
		tuttuğu nesnenin türünü bilir.Eğer tutulacak değerin hangi türlerden olabileceği kesinlikle biliniyorsa any yerine std::variant 
		türünün kullanılması çok daha uygun olacaktır.Aşağıdaki kullanım örneği resmi öneri metninden alındı :

#include <string>
#include <any>
#include <list>

	struct property {
		property();
		property(const std::string&, const std::any&);
		std::string name;
		std::any value;
	};

	typedef std::list<property> properties;
	Yukarıdaki kodda tanımlanan property türünden bir nesne hem istenilen türden bir değer saklayabilir hem de bu değere
		ilişkin tanımlayıcı bir yazıyı tutabilir.Böyle bir tür GUI uygulamalarından oyun programlarına kadar birçok yerde 
		kullanılabilir.Bir kütüphanenin ele alacağı türleri bilmeden o türlerden değerleri tutabilmesi ve başka API'lere bunları
		gönderebilmesi gereken durumlarda any sınıfı iyi bir seçenek oluşturabilir. Betik (script) dilleriyle arayüz oluşturma,
		betik dilleri için yazılan yorumlayıcı programlarda böyle türlere ihtiyaç artabiliyor.

		any sınıfının tasarımında büyük ölçüde Kelvin Henney tarafından yazılan ve 2001 yılında boost kütüphanesine eklenen
		boost::any sınıfı esas alındı.Kevlin Henney ve Beman Dawes 2006 yılında WG21 / N1939 = J16 / 06–0009 belge numarasıyla
		any sınıfının standartlara eklenmesi önerisini sundular.Nihayet Beman Dawes ve Alisdair Meredith'in önerileriyle diğer
		kütüphane bileşenleriyle birlikte any sınıfı da C++17 standardı ile dile eklendi. boost::any kütüphanesinde olmayan, emplace
		işlevi, std::in_place_type_t<> parametreli kurucu işlev, küçük tampon optimizasyonu (small buffer optimization) yapılabilmesi
		gibi bazı özellikler std::any kütüphanesine yer alıyor.

		*/