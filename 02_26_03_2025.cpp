/* Ýlk derste STL kütüphanesinde iki farklý move fonksiyon olduðundan bahsetmiþtik. Bunlardan birisi
*  algorithm baþlýk dosyasýnda ki move iken bir diðeri utility baþlýk dosyasýnda olan move fonksiyonudur.
*  Bu iki fonksiyonun implementasyonlarýný hep beraber yapalým.
* 
*/

/*
template<typename InIter, typename OutIter>
OutIter Copy(InIter beg, InIter end, OutIter destbeg)
{
	while (beg != end) {
		*destbeg++ = *beg++;
	}
	return destbeg;
}
*/

/*
*  Yukarýda copy fonksiyonunun possible implementasyonunu yazdýk. Þimdi algorithm dosyasýnda ki move fonksiyonuna bakalým.
*  Aslýnda algorithm içerisinde ki move'da utility move fonksiyonunu kullanmaktadýr.
*/

/*
* template<typename InIter, typename OutIter>
OutIter Move(InIter beg, InIter end, OutIter destbeg)
{
	while (beg != end) {
		*destbeg++ = std::move(*beg++);
	}
	return destbeg;
}
*/

/* 
*  Necati hocamýz derste þu cümleyi kullandý ve bende buraya yazmak istiyorum. Run time da her nesne ölümü tadacaktýr:)
*  Not: Yazma algoritmalarý copy, transform vs gibi end iteratörünü döndürüyor. Ayrýca hiçbir STL algoritmasý
*  exception throw etmiyor.
*  
*  Not: Utility baþlýk dosyasý içerisinde bildirimi bulunan ve yukarýda possible implementasyonunu yazdýðýmýz move algoritmasýný
*  kullanmak istediðimizde mecburen dolaylý veya direk olarak utility baþlýk dosyasýnýda include etmeliyiz. Buda utility'den
*  gelecek bütün bildirimler sebebiyle compile time süresini uzatabilir.
*  
*  Zamanýnda Cpp Standartlarý için >> operatörünün tek operand alan halinin std::move yerine kullanýlabilmesini içeren bir proposol
*  teklif edilmiþ fakat bu proposol kabul edilmemiþ. Zaten kabul edilseydi böyle bir deðiþikliði compilerlarýn implemente etmesi 
*  çok kolay olmayabilirdi.
*   
*  Universel reference adeta diyor ki gel ne olursan ol yine gel.
*  
*  Not: Meta functionlar genelde generic olarak implemente edilmiþ sýnýf yada member fonksiyonlar olabilmektedir ve bunlarýn amacý
*  bir deðeri run time da hesaplamak deðil bu deðeri veya türü compile time da hesaplamaktýr. Örneðin type trait kütüphanesi meta programming 
*  tekniðinden yararlanýlarak üretilmiþtir. Yada ratio kütüphanesindeki temel türler, deðerler bu meta programming programlama tekniði ile
*  geliþtirilmiþtir.
*/

/* Algorithm dosyasýndaki move algoritmasýnýn possible implementasyonunu yazdýðýmýza göre þimdi de utility 
*  baþlýk dosyasýnda bulunan move'u yazalim.
*/

/*
#include <type_traits>

template <typename T>
constexpr remove_reference_t<T>&& Move(T&& t)noexcept
{
	return static_cast<remove_reference_t<T>&&>(t);
}
*/

/*
*  Yukarýdaki possible move implementasyonunda aslýnda C++ dilinin birçok özelliði kullanýlmýþtýr. Burada ki implementasyonda
*  görüldüðü gibi universal reference'den yararlanýlmýþtýr (Universal reference olmasý için bir type deduction olmalý). remove_reference 
*  implementasyonu da aþaðýda verilmiþtir. remove_reference aslýnda template parametresi olarak aldýðý türün referanssýz halini 
*  member olarak sahip olan sýnýf þablonudur. Herhangi bir türden referansý remove etmek için kullanýlýyor ve member type alias ile
*  oluþturulmuþ tür referanssýz türdür. Bunu yaparken partial specializationlar dan faydalanmaktadýr. Ayrýca yazým kolaylýðý olmasý için
*  bir type alias oluþturulmuþtur. Bu type alias remove_reference içerisindeki member type'yi kullanmak için oluþturulmuþtur. 
*/

/*
template<typename T>
class RemoveReference {
	using type = T;
};

template<typename T>
class RemoveReference<T&> {
public:
	using type = T;
};

template<typename T>
class RemoveReference<T&&> {
public:
	using type = T;
};

template<typename T>
using RemoveReference_t = typename RemoveReference<T>::type;
*/

/*
*  Note:  The typename keyword is necessary when referring to a nested type in a template class, which 
*  is the case for RemoveReference<T>::type. Without typename, the compiler will not recognize it as a type.
*/

/*
*  Bir member fonksiyonun universal reference olmasýný istiyorsak member fonksiyonlarý member template olarak
*  yazmalýyýz. Aþaðýda örneði verilmiþtir.
*/

/*
template <typename T>
class Myclass {
public:
	template<typename U>
	void foo(U&&);
};

int main()
{
	Myclass<int> m;

	m.foo(12);
	int x;
	m.foo(x);
}
*/

/*
*  Dersin bir bölümünde reference collapsing anlatıldı. Reference collapsing aslında reference to reference durumunun oluştuğu senaryodur.
*  Pointerlar da olduğu gibi bir referansı gösteren referans yoktur. Fakat tür çıkarımından dolayı universal reference olan yerde yada bir using type bildirimi vs de
*  reference to reference tür çıkarımı elde edilebilir ve bu durumda ilgili türün ne olacağı bazı kurallara bağlıdır. 
   Bu kurallara reference collapsing denmektedir. Bu kurallar aşağıda gösterilmiştir.
   T& &&x  : T &x
   T&& &x  : T &x
   T& &x   : T &x
   T&& &&x : T &&x
   
   Aşağıya dersten sonra okuduğum bu konu ile ilgili iki yazının linkini bırakıyorum.
   https://leimao.github.io/blog/CPP-Reference-Collapsing/
   https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers

*/

/*
* int main()
{
	using intref = int&;
	int x{};

	intref& r1 = x;  //int& r1;
	intref&& r2 = x;  //int& r2;
}
*/

/*  
*  Aşağıda yer alan kod bloğunu derste örnek olarak yazdık. Bu şekilde bir fonksiyonun geri dönüş değerini 
*  const yapmak tabi ki bir syntax hatasına sebep olmaz. Fakat bu şekilde bir kod yazmamalıyız. Hem client kod için 
*  reference veya pointer olmayan bir geri dönüş türünü const yapmak anlamsız olabilir. Hemde böyle bir durumda move constructor yerine
*  copy constructor çağrılmasına sebep olacaktır. Sebebi ise açıktır. Copy constructorun parametrik yapısı const myclass& iken
*  move constructorun parametrik yapısı myclass&& şeklindedir. Ve burada function overloading de const parametreli copy constructor seçilecektir.
*  Bu ve buna benzer davranışlar, move constructorlarına çağrılmasına engel olup copy constructorların çağrılamasına neden olan copy senaryoları
*  copy fallback olarak isimlendirilmektedir.
*/

/*
* class myclass {};

const myclass foo()
{
	return myclass{};
}

int main(void)
{
	myclass x = foo();
}
*/

/*
*  Aşağıda bu durumla ilgili bir örnek daha gösterilmiştir.
*/
/*
*  #include <iostream>
#include <utility>

class Myclass {
public:
	Myclass() = default;

	Myclass(const Myclass&)
	{
		std::cout << "copy ctor\n";
	}

	Myclass(Myclass&&)
	{
		std::cout << "move ctor\n";
	}
};

int main()
{
	const Myclass x;

	Myclass y = std::move(x);
	//y is copy constructed
}
*/

/*
*  Dersin devamında bir sınıfın özel üye fonksiyonlarından bahsedildi. 6 farklı special member function vardır. Bunlar sırasıyla
*  default ctor, destructor, copy ctor, move ctor, copy assigment, move assigment. Bu ilgili fonksiyonlar 6 farklı durumda olabilir. 
*  Bunlar implicitly declared, implicitly declared but deleted, user declared, user declared but defaulted, user declared but deleted, not exist.
*  Implicityl olarak aslında compilerın bu özel üye fonksiyonları kendi implemente ettiği senaryolardır. 
*  Peki hangi durumda biz bu sınıf fonksiyonları kendimiz yazmalıyız. Öncelikle bunlar için big five da denmektedir. Eğer birini yazdıysan hepsini
*  yazman öneriliyor. Fakat her zaman geçerli olmasa da, duruma göre değişebilsede genel olarak şöyle diyebiliriz. Bir sınıfın constructoru içerinde
*  run time da bir allocation yapılıyorsa ve elde edilen alan yine sınıf içerisinde bir member pointer tarafından gösteriliyorsa burada tercih edilen pratik
*  move semantiğinden de yararlanmak için buradaki special member fonksiyonları yazmaktır. Tabi en başta da belirttiğim gibi bu bir genel ifadedir.
*  
*  Bazı amatör programcılar move constructor da arguman sağ taraf referansı olduğu için taşıma işlemi otomatik olacak sanıyorlar. Fakat parametrik yapıda
*  R value referansda olsa	kullanıldığı yerde L value expression'dur. Bu yüzden bu argumanı utility'de bulunan std::move ile kullanmamız gerekmektedir. 
* 
*  Not: Bir sınıfın destructorunun olmadığı senaryok yok, ya olacak, ya olacak.
*  
*  Bir sınıf ya copyable and moveable, ya copyable but not moveable, ya non copyable but moveable, yada not movable not copyable durumda olabilir.
*  Örneğin STL de unique_ptr not copyable but moveable bir fonksiyondur. Vector container'ı hem copyable hem de moveable dir. Bir sınıfı sadece moveable yapmak
*  istiyorsak sadece move constructor ve assigment fonksiyonlarını yazmalıyız.
*/

/*
Taşımadan da iyisi var.Zaten taşıma semantiği herzaman daha verimli olacak diye birşey yok.

class Myclass {
public:


private:
	std::array<int, 1000>ax; // Burada taşıma hiçbir avantaj getirmeyecek. İnt in taşınması ile kopyalanması arasında fark yok.
							// burada taşıma maliyeti, kopyalama maliyetine eşit.
};
*/

/*
*  Ne kadar zahmet o kadar hata yapma riski:)
*/

/* Dersin bir bölümünde reference qualifierLar dan bahsedildi. Bu notları kendim yazmak yerine aşağıda ki linkten okuyarak aldım.
*  https://github.com/umutkotankiran/Cpp_Advanced/blob/main/03_15_09_2021/main.cpp
*/

/*
* 
* REFERENCE QUALIFIERS
--------------------
Modern C++ 11 ile geldi. 
Overloading i value kategorisi ile belirleyebiliyoruz bu sayede.
Mesela bir member func ı sadece rvalue için çağrılmasını sağlayabiliriz. Reference Qualifier ile sağlıyoruz bunu.

class Myclass{
public:
	void func()const &	// Sadece const Lvalue expr olan sınıf nesneleri için çağrılabilir.
	{
		std::cout << "const func()&\n";
	}

	void func()& // Sadece Lvalue expr olan sınıf nesneleri için çağrılabilir.
	{
		std::cout << "func()&\n";
	}

	void func()&& // Sadece Rvalue expr olan sınıf nesneleri için çağrılabilir.
	{
		std::cout << "func()&&\n";
	}

	void func()const && // Sadece Rvalue expr olan sınıf nesneleri için çağrılabilir.
	{
		std::cout << "const func()&&\n";
	}
};

int main()
{
	Myclass m;
	const Myclass cm;

	m.func();
	Myclass{}.func();
	std::move(m).func();

	cm.func();
	std::move(cm).func();
}


Burada Lvalue, Rvalue, const Lvalue, const Rvalue hertürlü ihtimali yazabiliriz.
Dangling referans sorunuda böyle çözülebilir. Birçok yerde kullanılıyor.

Copy assignment ı sağ taraf için engelleyebiliriz. Herşeyi engelleyebiliriz veya izin verebiliriz.

class Myclass
{
public:
	Myclass& operator=(const Myclass&); // derleyici bunu yazar implicitly declared durumda

	Myclass& operator=(const Myclass&)&; // böyle yazabiliriz artık.Sadece lvalue expr için çağrılır.

	Myclass& operator=(const Myclass&)& = default; // Böyle user declared defaulted ettirebiliriz.

};


Sınıfın üye funclarını niteliyorlar. Çağrılmasını sınıf nesnesinin r veya lvalue olmasına bağlıyordu.
Bu yolla function overloading yapabiliyorduk. Neden yapalım? mesela bazı funcların sağ taraf değeri olan
sınıf nesneleri için çağrılmalarını engellemek. Sol taraf ve sağ taraf değeri olan sınıf
nesneleri aynı funcın farklı kodlar oluşturmak yani overloading yapmak.
Sınıfın member funcları için kullanılabilir. Global funclar için kullanılmıyor.

ÖNEMLİ KURALLAR
---------------
1 - Reference quailifer kullanırsak, reference qualifier olmayan bir functionu overload olarak yazamayız.

ÖR:
class Myclass{
public:
	void func(int);  // Sentaks hatası oldu. Sebebi Referans olmayan overload kabul etmiyor.
	void func(int)&;
	void func(int)&&;
};

Sebebi Referans olmayan overload kabul etmiyor !!

2 - Sağ taraf ve sol taraf için farklı kodları kullanılması örneğin veri elemanını get functionlar için
önemli olabiliyor.

*/


