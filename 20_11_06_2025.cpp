/*
*  Format kütüphanesinden dersimizde devam ediyoruz.
*  
*  En son format fonksiyonlarının custom yani user defined türler için kullanımını inceliyorduk. Burada yapılması gereken
*  std::formatter sınıfının kendi türümüz için explicit specializationu'nu oluşturmaktır. Necati hoca bu noktada
*  template specialiationu'na göndermelerde bulundu.
*  
*  Neydi explicit template specialition. Derleyici bir template parametresini aldıktan sonra o türe göre ilgili template'i
*  instantiate etmektedir. Örneğin Neco bir sınıf sablonu ise Neco<int> bizim için Neco sınıfının int specializationu dur. 
*  Neco'nun int specialization türünü elde etmek içinde derleyici ilgili template'i int template argumanı ile instantiete eder, 
*  yani kodunu yazar. Biz bir template'in explicit specializationu'nu yazdıktan sonra derleyiciye o tür için bir instantiete 
*  yapmamasını ve direk olarak bizim verdiğimiz explicit specializationu kullanmasını söylüyoruz. Aşağıda da syntax olarak
*  nasil yapıldığı gösterilmiştir.
* 
template <typename T>
class MyClass {
public:
    void foo();
};

template <>
class MyClass<int> {
public:
    void bar();
};

*
*  Not: Bir template specializationun interface'si ilgili sınıftan farklı olabilir. 
* 
*  Explicit template speciazation'un tipik kullanım örneği unordered_set, unordered_map gibi containerlardır. Bu template
*  sınıf sablonlarının ikinci tür parametresi hasher function içindir ve biz user defined türler için bu hasher'in explicit
*  specializationu nu yazarız. Aşağıda örneği gösterilmiştir.

template <>
struct std::hash<Date> {
    // Hash function implementation
};

int main()
{
    using namespace std;
    unordered_set<Date> myset;
}

*
*  Custom typeları format fonksiyonu ile kullanmamız için ilk yapmamız gereken formatter sınıf sablonunun kendi custom
*  türümüz için specializationu'nu yazmaktır. Daha sonra bu specialization içerisinde isimleri parse ve format
*  olan iki fonksiyon yazmamız gerekmektedir ve format fonksiyonu formatlamayı yapacak fonksiyon, parse fonksiyon
*  ise format stringi parse edecek fonksiyondur. Bu fonksiyonları yazarken köşeli parantezler {} içerisine yazacağımız format
*  karakterlerini kendi isteğimize göre belirleyebiliriz. Bu kısım gerçekten karışık, yani custom typelar için format
*  kütüphanesini kullanabilmek biraz karmaşık ve yazarak anlatması da epey zor. Dolayısıyla derste yazılan örneği 
*  direk olarak aşağıya koyuyorum

#include <iostream>
#include <format>
#include <string>
#include <vector>

struct Point {
	int mx, my;
};

template<>
class std::formatter<Point>
{
public:
	auto parse(std::format_parse_context& pc)
	{
		auto iter = pc.begin();
		for (; iter != pc.end() && *iter != '}'; ++iter)
		{
			if (*iter == '#')
			{
				cb_ = true;
			}
			else
			{
				throw std::format_error{ "invalid formatting character!" };
			}
		}
		return iter;
	}

	typename std::format_context::iterator format(const Point& p, std::format_context fc)
	{
		return std::format_to(fc.out(), "{}{}, {}{}", (cb_ ? '{' : '('), p.mx, p.my, (cb_ ? '}' : ')'));
	}
private:
	bool cb_{};
};

int main()
{
	Point p1{ 2,6 }, p2{ 3,5 };
	std::cout << std::format("{:#} {}", p1, p2) << '\n';
}

*
* 
*  Dersin kalan kısmında STL'de ki span sınıf sablonu anlatıldı. Span aslında bellekte continuous olarak yer alan aynı türler
*  için bir gözlemci amacıyla kullanılıyor, tıpkı string_view gibi. Mesela std::array, std::vector, std::string, normal
*  bir array, gibi bellekte	continuous olarak tutulan ögeler için span'i kullanabiliriz. lightweight : hafif siklet.
*  
*  Aşağıda span'in nasıl bir sınıf sablonu olduğu görülüyor. İki farklı template parametresine sahip. İkinci template
*  parametresi default olarak standart kütüphanede tanımlı olan dynamic_extent değerini alır ve bu değer
*  size_t türünün ilgili sistemde alabileceği max. değerdir. 

inline constexpr std::size_t dynamic_extent = std::numeric_limits<std::size_t>::max();

namespace std {
	template<typename ElementType, size_t Extent = dynamic_extent>
	class span {
		//...
	};
}

C++20 standardı ile C++ standart kütüphanesine eklenmiş çok önemli bir sınıf şablonudur. Bu sınıf, bellek üzerindeki
ardışık nesnelere tür güvenli, boyut bilgili, düşük maliyetli bir erişim sağlar. İşlevselliği bakımından 
pointer'ların (T*) ve C tarzı dizilerin modern ve güvenli bir alternatifi olarak düşünülebilir.

std::span avantajları
Niyeti daha iyi anlatıyor.

void foo(const int *p, std::size_t n);
void foo(std::span<int>)

Kopyalaması kolay. Değerle bir fonksiyona aktarılması maliyeti düşük. Örneğin
sizeof(std::span) == sizeof(pointer) + sizeof(size_t) olabilir.

container'lara benzer bir arayüz (interface) sunuyor.
üye fonksiyonları kullanım kolaylığı sağlıyor.
range base for loop ile kullanılabiliyor.
elemanların nerede tutulduğu std::span için önemli değil. Elemanların contigous memory'de
olması yeterli. (c arrays, std::array, std::vector, std::initilizer_list etc.)
std::ranges kütüphanesi belirli yerlerde std::span kullanıyor. Bir span nesnesi görüntülediği range'in sahibi
değil. Dangling pointer oluşma riski var.

default ctor ile oluşturulan bir span nesnesi dynamic extent'e sahip. Böyle bir nesnenin size üye fonksiyonu
çağrıldığında 0 değeri empty fonksiyonu çağrıldığında true değeri döndürür. static extent'e sahip std::span türünün 
default ctor'u yoktur.

boş bir std::span nesnesinin

data() fonksiyonu nullptr değerini döndürür.
size() fonksiyonu 0 değerini döndürür.
empty() fonksiyonu true değerini döndürür.

*
*
*  Eğer biz ikinci tür argumanında dynamic_extent'i kullanmayıp ikinci tür argumanını burada ilgili span nesnesinin sizeof'u sadece
*  pointer sizeof'u çıkarken, ikinci tür argumanını biz vermeyis dynamic_extent kullanılırsa burada 
*  sizeof pointer + size_t olacaktır. Aşağıda bununla ilgili örnek gösterilmiştir.

#include <span>
#include <iostream>
#include <vector>

int main()
{
	int a[10]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::vector<int> ivec{ 2, 5, 8, 9 };

	std::span<int, 10> sp1{ a };	//fixed extent
	std::span<int> sp2{ a };	//dynamic extent
	std::span<int> sp3{ivec};	//dynamic extent

	std::cout << "sizeof(sp1) = " << sizeof(sp1) << '\n';
	std::cout << "sizeof(sp2) = " << sizeof(sp2) << '\n';
	std::cout << "sizeof(sp3) = " << sizeof(sp3) << '\n';
}

*  Not: Fixed extent olan bir span nesnesi ikinci template argumanı 0 değilse default constructible değildir.
* 
*  Bir span ile yapabileceğimiz bazı şeyleri gösteren örnek aşağıdadır.

int main()
{
	using namespace std;

	int ar[5]{1, 2, 3, 4, 5};
	span<int, 5> sp(ar);

	sp[3] *= 10;
	sp.front()++;
	sp.back()--;
	sort(sp.begin(), sp.end());

	for (auto i : sp) {
		cout << i << " ";
	}
}

*
*  Aşağıdaki kodda spy dynamic extent olduğu için default construct edilebildi. Bununla beraber member size fonksiyonu
*  span'in dynamic extent olduğu durumda içeride tutulan ve size'i gösteren değeri değil gözlem aralığının boyutunu 
*  döndürüyor. spy ile şuan da gözlemlenen bir contiguous bir aralık olmadığı için 0 döndürecektir.

#include <span>
#include <iostream>

int main()
{
	std::span<int> spy; //valid

	boolalpha(std::cout);

	std::cout << "spy.size() = " << spy.size() << '\n';
	std::cout << "spy.empty() = " << spy.empty() << '\n';
	std::cout << "spy.data() == nullptr = " << (spy.data() == nullptr) << '\n';
}

*  Not: Span nesnesi gözlediği alandaki nesnelerin hayatlarını kontrol edemiyor, onların hala aynı memory'de olup
*  olmadığını anlayamıyor. Dolayısıyla örneğin bir reallocation, ilgili nesnelerin destruct edilmesi vs hepsi
*  tanımsız davranıştır. Mesela aşağıdaki kod buna bir örnektir.

std::span<int> foo()
{
	int ar[5]{1, 2, 3, 4, 5};
	return std::span(ar);
}

int main()
{
	using namespace std;
	auto sp = foo();

	for (auto i : sp)
		cout << i;
}


*
*  Aşağıdaki kodda mesela ub'dir çünkü vector de bir reallocation olacaktır push_back çağrısından sonra.
* 

int main()
{
	using namespace std;

	vector<int> ivec{ 1, 2, 3, 4, 5 };
	cout << ivec.capacity() << '\n';

	span<int> sp(ivec);
	copy(sp.begin(), sp.end(), ostream_iterator<int>(cout, " "));
	std::cout << "\n";

	ivec.push_back(6);
	cout << ivec.capacity() << '\n';

	// ub
	copy(sp.begin(), sp.end(), ostream_iterator<int>(cout, " "));
}

*
*  Bir span nesnesi ile span template parametresinin const olması farklı şeyler. Span nesnesinin const olması 
*  span'in gözlem yaptığı aralıkdaki bir nesnenin değerini değiştirmeye engel değildir. Fakat template
*  tür parametresi const ise span gözlem yaptığı aralıktaki bir nesneyi değiştirmeye çalışırsa bu syntax hatası olacaktır.
*  Aşağıda bu konu ile ilgili derste yazılan örnek yer almaktadır.

#include <array>
#include <span>

int main()
{
	std::array<int> a1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	std::array<int> a2{0, 8, 15};

	const std::span<int> cspn{a1}; // span itself is const
	std::span<const int> sp{a1}; // viewed elements are const

	cspn[0] = 42; // valid
	++cspn.back(); // valid
	// sp[0] = 42; // error
	cspn = a2; // error cspn const
	sp = a2; // valid
}

*
*  Örneğin aşağıdaki kodda modify fonksiyonuna biz direk bir array, vector vs geçersek syntax hatası olacaktır,
*  çünkü ilgili nesne const bir nesne olacaktır ve onu değiştiremeyeceğiz. Fakat biz bir span nesnesi gönderirsek
*  bu fonksiyona span nesnesinin kendisi const olacağı için span nesnesinin gözlemlediği yerde değişiklik
*  yapmak syntax hatasına neden olmayacaktır.
* 
#include <span>
#include <array>

template<typename T>
void modifyElemsOfConstColl(const T& coll)
{
	coll[0] = {}; // OK for spans, ERROR for regular containers
	auto ptr = coll.data();
	*ptr = {}; // OK for spans, ERROR for regular containers
	for (auto pos = std::cbegin(coll); pos != std::cend(coll); ++pos) {
		*pos = {}; // OK for spans, ERROR for regular containers
	}
}

int main()
{
	std::array<int> arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	// modifyElemsOfConstColl(arr); // ERROR: elements are const
	modifyElemsOfConstColl(std::span{ arr }); // OOPS: compiles and modifies elements of arr
}

*
*  Reallocation'un tanımwsız davranışa yol açtığını gösteren ve derste yazılan bir başka örnek.

#include <vector>
#include <span>
#include <iostream>

void print(std::span<int> sp)
{
	for (auto i : sp)
		std::cout << i << ' ';
	std::cout << '\n';
}

int main()
{
	std::vector<int> ivec{ 1, 3, 5, 7 };
	std::span x{ ivec };

	for (int i = 0; i < 10; ++i) {
		print(x);
		ivec.push_back(i); // reallocation makes the pointer dangling
		std::cout << "cap = " << ivec.capacity() << '\n';
	}
}

*
*  Not: Eğer CTAD dan faydalanarak bir span nesnesini initilialize edersek ve bu initialize ettiğimiz expr. bir
*  dizi ise burada span nesnesi fixed extent olacaktır. Çünkü dizinin boyutu bir compile time sabitidir, yada şöyle
*  diyelim derleyici dizinin boyutunu compile time'da anlayabiliyor. Fakat biz bu span'i bir vector nesnesi ile
*  initialize edersek bu sefer dynamic extent olacaktır çünkü vectorun size'i run time' da anlaşılabilir.
* 
*  Not: span'in size member fonksiyonu gözlem aralığının boyutunu vermektedir.

*  Constluk ile ilgili derste yazılan bir başka örnek.

#include <span>
#include <iostream>

int main()
{
	int ar[] = { 3, 6, 7 };
	const std::span sp = ar;

	sp[1] = 4; // valid
	*sp.begin() = 4; // valid
	sp.back() = 4; // valid

	for (auto i : ar)
		std::cout << i;
}

*/
