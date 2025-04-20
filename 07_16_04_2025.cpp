/* 
*  İlk olarak perfect returning implementasyonunu lambda ifadeleri ile nasil yapabiliriz bundan bahsedildi. Burada 
*  trailing return type syntax'ı kullanılmıştır. Derste yazılan kod aşağıda verilmiştir.

int main()
{
	auto f = [](auto func, auto&&... args) -> decltype(auto) { // -> decltype(auto) trailing return type.
		return func(std::forward<decltype(args)>(args)...);
	}
}

*  Not: 6. dersde öğrenmiştik ama hatırlatalım. Yukarıda fonksiyon argumanlarını auto ile yazarak derleyici tarafından yazılacak
*  closure type'in üye fonksiyon operator()'u fonksiyon sablonu olarak yazılmasına sebep olduk. 
*/

/*
*  7. derste perfect returning ile birlikte deferred perfect returning'i görmüştük. Peki deferred perfect returning'i lambdalar ile
*  nasil yapabiliriz? Aşağıda bununla ilgili yazılan kod verilmiştir.

template<typename ...Args>
void f(Args &&...);

int main() {

	auto fn = [](auto f, auto&&... args)->decltype(auto) {
		decltype(auto) ret = f(std::forward<decltype(args)>(args)...);

		if constexpr (std::is_rvalue_reference_v<decltype(ret)>) {
			return std::move(ret);
		}
		else {
			return ret;
		}
	}
}

Not: Lambdalar çok geniş bir başlık altında kurs içerisinde ele alınacaktır. Ele alınacak konular lambda ifadeleri ile ilgili
ileri konular ve idiomlar olacaktır.

Not: Yukarıdaki iki örnekte kullanılan decltype(auto)'nun ne olduğunu 6. derste görmüştük. Tür çıkarımı tamamen decltype tür 
çıkarımı kuralları ile aynıdır. 

*/

/* 
*  Dersin devamında Necati hoca aşağıdaki soruları sordu ve bizlerden cevap istedi. Sorulan sorular ve cevapları
*  şu şekildedir.


NOT:
1. Pr value ise tür T türüdür.
2. L value ise tür T & türüdür.
3. X value ise tür T && türüdür.

* 

decltype(auto) foo(int x) {
	return x;  // no problem, deduce edilen tür int. 
}

decltype(auto) foo(int x) {
	return (x);  // problem, (x) bir expression, deduce edilen tür int&, local ve ömrü bitecek bir değişkeni int&'e bağlayamayız.
				// burada life extension vs de olmaz.
}

decltype(auto) foo(int x) {
	return ++x;  // problem, prefix ++ operatoru L value değer üretir.deduce edilen tür bu sebeple int& olacak.problem sebebi yukarıdaki ile aynı
				// x++; olsaydı bir problem olmayacaktı. postfix ++ operatoru PR value değer üretir çünkü.

}

decltype(auto) foo(int x) {
	return (x >= 0 ? x : 0);  // no problem, ternary operatorun velue kategorisi ikinci ve 3. operandın value kategorisine bağlı
							 // Burada 3. operand R value olduğu için R value değer üretecek,
}

struct Nec {
	int x{ 0 };
};

decltype(auto) foo(int x) {
	return (Nec{}) // KAYIT.
}

struct Nec {
	int x{ 0 };
};

decltype(auto) foo(int x) {
	return (Nec{}.x) // problem. Bir membera nokta veya ok operatoru ile erişiliyorsa bu expr. değil isimdir. Fakat burada
						// isim parantaz içinde, bu sebeple expr. oluyor. Dolayısı ile problem.
}


*/


/*

#include <iostream>

using namespace std;

class myclass {

};

void foo(const myclass&)
{
	cout << "const myclass&\n";
}

void foo(myclass&)
{
	cout << "myclass&\n";
}

void foo(myclass&&)
{
	cout << "myclass&&\n";
}

void foo(const myclass&&)
{
	cout << "const myclass&&\n";
}

*  Yukarıdaki kodu foo'nun template hali haric daha önceden de bir iki kere yazıp hangisinin hangi argumanlar çağrıldığında
*  viable olduğuna, function overload'da kaçıncı olduğuna vs bakmıstık. Örneğin const myclass& parametreli fonksiyonun bütün çağrılar
*  için viable oldugundan vs bahsetmistik. 
* 
*  Fakat simdi bunlara birde foo fonksiyonu için template sablonunu ekledik. Bu durumda çağrılacak ilk fonksiyon geçen derslerde yaptığımız
*  gibi olacaktır ki buda exact match'dir. Fakat artık ikinci çağrılacak sıralamayı template specializationu alacaktır. Daha yalın
*  bir ifade ile fonksiyon argumanında exact match yok ise viable olan fonksiyon olmasına rağmen ilgili türden fonksiyonun template
*  specializationu çağrılacaktır.
*/

/*
*  Aşağıda iki tane count_if çağrısı var ve birbirleri arasındaki fark soruldu. 
struct BigPred {
	bool operator()(int);
	unsigned char buffer[100000]{};
};

int main() {
	std::vector<int> ivec(2000);
	// some code here

	BigPred pred;

	auto nx = count_if(begin(ivec), end(ivec), pred);
	auto ny = count_if (begin(ivec), end(ivec, std::ref(pred)));

}

*  Aslında fark çok açıktır. İlk count_if çağırısında fonksiyona direk olarak pred nesnesi calleable olarak geçildi ve burada pred nesnesi
*  kopyalandı. Burada pred nesnesi için kopyalama oldukça maliyetlidir çünkü içeirisnde 100000 elemanlı bir array tutmaktadır. Ayrıca
*  ilk çağrıda count_if fonksiyonu bir fonksiyon sablonu olduğu için ikinci template argumanına tür çıkarımı BigPred olarak yapılmıştır.
*  Fakat ikinci çağrıda ref fabrika fonksiyonu kullanılarak bir reference_wrapper nesnesi arguman olarak fonksiyona geçilmiştir ve 
*  predicate sınıfını tutan/gösteren reference_wrapper nesnesi kopyalanmıştır. Reference_wrapper sizeof'u çok küçük oldugu için
*  burada maliyet çok çok çok azdır. 
* 

Peki reference_wrapper kullanmadan da burada maliyeti benzer biçimde nasil azaltabilirdik.Eğer count_if için template parametrelerini
aşağıdaki gibi explicit yazarsak bunu başarabilirdik.

std::count_if<std::vector<int>::iterator, BigPred&>(...);

Derste sorulan soru: Hocam bigpred’i taşıyabilir miydik? Std move ile.
Cevap: Taşıyabilirdik tabi ama gereksiz. İçerde static elemanlar var, pointer kopyalayamıyoruz. Dolayısıya kopyalama ve taşıma 
arasında maliyet farkı yok.

*/

/*	
*  Tag class = empty class, boş bir sınıf. Bu tarz sınıfların varlık nedeni aşağıdaki gibi.
*  Aynı türden nesne alacak ctor yada fonksiyon fakat her nesne için farklı bir iş yaptıracak. Overloadı böyle yapabiliriz.
*  Bu teknik optional, variant, any gibi STL ögelerinde de kullanılıyor.

#include <array>
#include <string>
#include <mutex>

constexpr struct DeferLock_t{}deferlock;
constexpr struct AdoptLock_t {}adoptlock;
constexpr struct TryToLock_t {}trytolock;

class UniqueLock {
public:
	UniqueLock();
	UniqueLock(std::mutex& r, DeferLock_t); 
	UniqueLock(std::mutex& r, AdoptLock_t);
	UniqueLock(std::mutex& r, TryToLock_t);
};

*/

/* 
*  Dersin kalan bölümünde user defined literal konusundan bahsedildi. İlk olarak olarak user defined literal nedir, tanımını yapmaya
*  çalışalım. 
*   
*  Integer, floating point, string gibi türleri direk olarak suffix yani son eklerle kullanıp bir fonksiyon çağrısına dönüştürmeye
*  ve genellikle bu fonksiyon çağırısnın geri dönüşü ile bir nesne elde etmeye yarayan bir araçtır. Aşağıda standart kütüphanede
*  bulunan bir literal ile ilgili örnek verilmiştir.

int main(void)
{
	using namespace std::literals; //standart kütüphanede bulunan literalsler isim çakışmasını önlemek için
								  //literals namespace alanı içerisinde bildirilmiştir.

	"ahmet"s; // bu ifade aslında standar olan bir user defined literaldir. Aşağıdaki fonksiyonu çağırmak gibidir ve bir stirng nesnesi geri döndürür.
	operator""s("ahmet", 5);
}

*/

/*
*  Standart kütüphane de verildiği gibi bizde kendi türlerimiz için kendimize has user defined literal olarak kullanılabilecek
*  fonksiyonlar yazabiliyoruz. Bu fonksiyonları aşağıdaki formlarda yazabiliriz. (https://en.cppreference.com/w/cpp/language/user_literal)
* 
A user-defined literal is an expression of any of the following forms
decimal-literal ud-suffix	(1)	
octal-literal ud-suffix	(2)	
hex-literal ud-suffix	(3)	
binary-literal ud-suffix	(4)	
fractional-constant exponent-part ﻿(optional) ud-suffix	(5)	
digit-sequence exponent-part ud-suffix	(6)	
character-literal ud-suffix	(7)	
string-literal ud-suffix	(8)	

1-4) user-defined integer literals, such as 12_km
5-6) user-defined floating-point literals, such as 0.5_Pa
7) user-defined character literal, such as 'c'_X
8) user-defined string literal, such as "abd"_L or u"xyz"_M

*/

/*
*  Standartlarda user defined literal oluşturulan sabite verilen isim iken literal operator ise çağrılan fonksiyona verilen isimdir.
* 
*  Literal operator fonksiyonları yazarken parametrik yapıda özgür değiliz. Aşağıdaki parametrik yapılara sadece izin verilmiştir.
* 
Only the following parameter lists are allowed on literal operators:
( const char* )	(1)
( unsigned long long int )	(2)
( long double )	(3)
( char )	(4)
( wchar_t )	(5)
( char8_t )	(6)	(since C++20)
( char16_t )	(7)
( char32_t )	(8)
( const char*, std::size_t )	(9)
( const wchar_t*, std::size_t )	(10)
( const char8_t*, std::size_t )	(11)	(since C++20)
( const char16_t*, std::size_t )	(12)
( const char32_t*, std::size_t )	(13)
*/

/* Peki user defined literal operatorleri nasil yazabiliriz. Burada operator""_ kısmı zorunludur. Bundan sonra ise user defined suffix'i
*  ekleyebiliriz. Örneğin operator""_b(unsigned long long in); Eğer daha sonra 8334_b gibi bir çağrı yaparsak burada yazacağımız fonksiyon
*  8334 argumanı ile çağrılacaktır. Fonksiyonun geri dönüş türü ile ilgili bir kısıt bulunmamaktadır.
* 
*  Not: Literal operatorleri yazarken '_' bizim yazdıklarımız için zorunludur. Fakat '_' standart literal operatorler için kullanılmıyor.
* 
*  ??? operator""_x(const char* ) -> 83548_x -> operator""_x("83548");
   Literal operatorler direk olarak isimiylede çağırılabilir.
*/

/*
*  İki parametreli literal operatorlerin çağrılabilmesi için sabitin bir string literali olması gerekmektedir.	
*  ??? operator""_s(const char*, size_t) -> "mehmet"_s -> operator""_s("mehmet", 6) -> 
*  
*  Literal operator fonksiyonları constexpr yapmak iyi bir pratiktir. Böylece fonksiyonun geri dönüş değeri compile time sabiti olarak
*  kullanılabilecek gibi olursa run time da tasarruf yapmıs oluruz.
*  
*  Aşağıda user defined literaller ile ilgili derste yazılan birkaç örnek verilmiştir.


constexpr double operator""_km(long double val)
{
	return static_cast<double>(val / 1000 );
}

constexpr double operator""_m(long double val)
{
	return static_cast<double>(val);
}

constexpr double operator""_cm(long double val)
{
	return static_cast<double>(val * 100);
}

constexpr double operator""_mm(long double val)
{
	return static_cast<double>(val * 1000);
}

int main() {
	1.456_km + 345.81_m + 130007.001_cm + 651712.98_mm;
	// operator""_km(1.456) + operator""_m(345.81) + operator""_m(130007.001) + operator""_mm(651712.98);
}

Not: Literallerin isim çakışmasına uğramaması için namespace içerisinde bildirmek daha doğru olacaktır.

constexpr std::size_t operator""_KB(unsigned long long val)
{
	return static_cast<std::size_t>(val * 1024)
}

constexpr std::size_t operator""_MB(unsigned long long val)
{
	return static_cast<std::size_t>(val * 1024 * 1024)
}

int main()
{
	std::array<char, 24_KB> arr;
}

*/

/*
*  İki parametreli literal operatorler için örnekler. Sabit string!

std::string operator""_rs(const char* p, std::size_t s)
{
	std::string result{ p };

	std::reverse(result.begin(), result.end());
	return result;
}

int main() {
	auto str = "Ortalik karisacak"_rs; 
}


void operator""_print(const char* p, std::size_t s) {
	std::cout << p;
}

int main() {
	"necati bulasma"_print;
}

*/

/*

constexpr unsigned operator""_bin(const char* p)
{
	int result = 0;

	for (int i = 0; p[i] != '\0'; ++i) {
		if (p[i] != '0' && p[i] != '1') {
			throw std::invalid_argument{ "invalid_argument" };
		}
		result = result * 2 + p[i] - '0';
	}

	return result;
}

int main()
{
	10011_bin; // operator""_bin("10011);
}

*/

/*
*  Aşağıda literal operatorlerin kullanıldığı sık senaryolarda biri örneklenmiş. Bir sınıf nesnesi oluşturmak için
*  literal operator kullanılıyor. Yani tıpkı bir fabrika fonksiyon gibi. Ayrıca hatırlatma amaçlı sunuda belirtmek istiyorum,
   mydate atamasında bir copy ellison olacaktır, mandatory copy ellision.


#include <iostream>
using namespace std;
class Date {
public:
	Date(const char*) {
		cout << "1";
	}
	Date(const Date&&) = delete;
};

Date operator""_dt(const char* p, std::size_t) {
	return Date{ p };
}

int main()
{
	auto mydate = "04-09-1987"_dt;
}

*/

/*
*  Not: Literal operatorler içinde function overloading yaoabiliriz. Fakat derste literal operatorler için function overloading
*  resoulution kurallarından bahsedilmedi.
*/

