/*
*   Perfect forwarding: Tür bilgisinin korunarak wrapper fonksiyondan çağrılan fonksiyona geçilmesidir. Mesela standart
*   kütüphanede ki containerların emplace_back fonksiyonları bu şekilde impl. edilmiştir. Aldıkları argumanları tür bilgisini
*   koruyarak ilgili sınıfların constructorlarına direk geçerler. 
* 
*   Örneğin böyle bir perfect forwarding mekanizması olmasaydı aşağıdaki kodda s nesnesi taşınmak yerine kopyalanacaktı.
*   Çünkü emplace_back'in && türünden aldığı referans kullanıldığı yerde value category'si yine L value olacaktı.
*/

/*

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Myclass {
public:
	Myclass();
	Myclass(const string&);
	Myclass(string&&);
};

int main()
{
	vector<Myclass> vec;
	string s{ "Nuri" };

	vec.emplace_back(move(s));
}

*/

/*
*  Fabrika fonksiyonlarının bir çoğuda perfect forwarding mekanizmasından yararlanmaktadır. Örneğin make_unique fabrika fonksiyonu.
*  make_unique fonksiyonu T türden adresini tutacağı nesneyi hayata getirirken aldığı argumanları constructora geçer ve 
*  burada perfect forwarding mekanizmasından yararlanarak tür bilgisini koruyarak bunu yapar.
* 
*  Örneğin aşağıdaki kodda s nesnesi make_unique<Myclass> açılımının argumanına geçilmiştir. Burada make_unique bir Myclass
*  nesnesini dinamik olarak hayata getirecektir. Hayata getirirken aldığı argumanın tür value categorysini koruyarak constructora
*  gönderecektir. 
*  
*/

/*

#include <iostream>
#include <string>
#include <memory>

using namespace std;

class Myclass {
public:
	Myclass();
	Myclass(const string&);
	Myclass(string&&);
};

int main()
{
	string s{ "Nuri" };

	auto p = make_unique<Myclass>(s);
}

*/

/* 
#include <iostream>

using namespace std;

class Myclass
void foo(const Myclass)
*/

/*
*  Şimdi aşağıda bir myclass türü için farklı overloadların impl'leri ve bu foo fonksiyona farkılı argumanlarla çağrı yapılmıştır.
*  Bu fonksiyon çağrılarında hangi foo fonksiyonunun çağrılacağı function overload resolution kuralları ile ilgilidir. Peki bu 
*  örneği biz neden yazdık? Şu senaryoyu düşünelim. Biz bu foo fonksiyonuna direk çağrı yapmak yerine bu fonksiyonu çağıran
*  ve adeta wrapper görevi gören bir fonksiyona çağrı yapmak isteseydik ve eğer perfect forwarding mekanizması olmasaydı
*  her overload için bir tane de wrapper fonksiyon yazmak zorunda olacaktık. Fakat perfect forwarding sayesinde tek bir wrapper ile
*  bütün overloadlara tür bilgisini koruyarak çağrı yapabiliriz.
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

int main() {
	myclass m;
	const myclass cm;

	foo(m);				//myclass&
	foo(cm);				// cont myclass&
	foo(myclass{});			//myclass&&
	foo(move(m));		//myclass&&

}

*/

/*
*  İlk olarak perfect forwarding mekanizmasının nasil implemente edildiğine bakmadan önce universal reference 
*  ve reference collapsing kurallarını tekrar hatırlayalim. Çünkü perfect forwarding mekanizmasın da universal reference 
*  den direk olarak faydalanıyor.
* 
*  Universal referans template fonksiyonlarda template arguman türünün && ile yazılması durumuydu, bunu zaten öğrenmiştik.
*  İki tane & ifadesi kullanıldığı için sağ taraf referansı ile karıştırılabilir ama kesinlikle sağ taraf referansı değildir.
*  Aşağıda bir universal referans örneği gösterilmiştir. Foo fonksiyon şablonu argumanı bir universal referanstır. Ayrıca
*  auto&& t = expr; da bir universal referanstır. Aşağıda T ve param için tür çıkarımının nasıl olacağı önceki derslerde
*  öğrendiğimiz gibi reference collapsing kuralları ile belirlenmektedir. Buradaki reference collapsing kurallarını şöyle genelleyebiliriz.
*  Örneğin eğer universal reference'ye myclass türünde bir sol taraf ifadesi atanırsa T için tür myclass&, param için tür 
*  myclass& olacaktır. Fakat eğer atanan ifade bir sağ taraf değeri ise o zaman T için tür myclass param için myclass&& olarak
*  deduce edilecektir. Aşağıda bununla ilgili örnek verilmiştir.
* 
*  Not: En başta universal referenceyi sadece perfect forwarding mekanizması için düşünmüşler.Fakat universal referencenin
*  başka amaçlarıda olabilir, bu sonradan farkedilmiş. Bu sebeple forwarding referans dan universal referansa doğru
*  standart olmayan bir söylem değişikliği olmuştur.
*/

/*

#include <iostream>

using namespace std;

struct myclass{};

template<typename T>
void foo(T&& param)
{
	T x;
}

int main()
{
	myclass m;
	foo(m);			// T -> myclass&, param myclass&
	foo(myclass{});	// T -> myclass, param myclass&&

}

*/

/* 
*  Universal referanslar sayesinde compile time da gelen argumanın L value mu yoksa R value mu olduğunu saptayabiliriz.
*  Bu sayede fonksiyon içerisinde L value'ler için farklı bir impl, R value ler için farklı bir impl yazabiliriz. Tabiki
*  perfect forwarding implementasyonu standart kütüphane de bu şekilde yapılmamıştır. Fakat bu şekilde yapılsaydı aşağıdaki
*  gibi bir kod yazabilirdik.(if constexpr).
*/

/*
#include <iostream>
#include  <type_traits>

using namespace std;

struct myclass {};

template<typename T>
void foo(T&& param)
{
	if constexpr (is_lvalue_reference_v<T>) {
		cout << "code for L value arguments\n";
	}
	else {
		cout << "code for R value arguments\n";
	}
}

*/

/*
*  Aşağıdaki kodlardan hiçbiri universal reference değildir.
*/

/*

template <typename T>
class myclass {};

template<typename T>
void foo(myclass<T>&&) {}

template<typename T>
void foo(typename T::value_type&& x){}


template<typename T>
void push_back(std::vector<T>& vec, typename std::vector<T>::value_type&& val)
{
	vec.push_back(val);
}

*/

/*	
*  Derste universal reference bir fonksiyon şablonu için de explicit specialization yazılabileceği söylendi ve aşağıdaki
*  örnek yazıldı. 
*/

/*
* 
#include <vector>
#include <string>
#include <iostream>

using namespace std;

template <typename T>
void func(T&&)
{
	cout << "primary template\n";
}

template <>
void func(string&&)
{
	cout << "explicit spec. for string &&\n";
}

int main()
{
	string name{ "kerim" };

	func(string{ "neco" });
}

*/

/*
*  Derste sorulan bir soru üzerine Necati hoca fonksiyon şablonları için full specialization'dan bahsetti ve aşağıdaki örneği verdi.
*  Aşağıda func fonksiyon şablonunun int* bir full specializationu verilmiştir. Full specialization'un diaomond(<>) ile yapıldığı
*  hatırlatıldı. Main fonksiyondaki func çağrısında T* template parametreli fonksiyon sablonu instantied edilip çağrılacaktır.
*  Çünkü function template full specialization function overload resolutiona girmez. T ve T* parametreli templateler girer ve
*  buradan en spesifik olanı yani T* seçilir. Fakat full specialization T* parametreli sablonun üstünde olduğu için isim
*  aramadan dolayı T* fonksiyon şablonunun full specializationu olmaz. Dolayısı ile T* parametreli instantiate edilip çağrılır. 
*  Fakat full specialization olan func fonksiyonu T* parametreli funcdan da aşağıya yazılsaydı işte o zaman full specialization
*  çağrılırdı.
*
*/

/*
* 
#include <iostream>

using namespace std;

template<typename T>
void func(T)
{

}

template <>
void func(int* p)
{

}

template<typename T>
void func(T*)
{

}

int main()
{
	int x{};

	func(&x);
}

*/

/*
*  Perfect forwarding mekanizmasından hala bahsediyoruz, hala bitmedi. Şimdi tam olarak nasıl yapıldıuğını görmeden std::move 
*  implementasyonunu tekrardan yazalım.
*/

/*

#include <type_traits>

using namespace std;

template<typename T>
constexpr remove_reference_t<T>&& Move(T&& t)noexcept 
{
	return static_cast<remove_reference_t<T>&&>(t);
}

*/

/*
*  Aşağıda da remove_reference için örnek impl'ler verilmiştir. Buralar için detaylı bilgi vermiyorum. 2. derste
*  çok detaylı işlendi bu konular.
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
*  Şimdi aşağıda perfect forwarding işleminin nasil yapıldığını anlayalim. 4 farklı foo fonksiyonu var. Bu foo fonkisyonlarına
*  wrapper fonksiyon olan call_foo fonksiyonu ile çağrı yapıyoruz. Fakat wrapper fonksiyon arguman olarak aldığı nesnenin tür bilgisini
*  koruyarak foo fonksiyonuna çağrı yapmalıdır çünkü foo fonksiyonunun farklı overloadları vardır. Eğer direk olarak
*  aldığı argumanı foo fonksiyonunu çağırmak için arguman olarak geçseydi her ne kadar call_foo fonkisyonu bir universal reference
*  kullanmasına rağmen t türüne bakılmaksızın kullanıldığı yerde sol taraf expression olacaktır. Dolayısı ile forward fonksiyon sablonu
*  kullanılmıştır. Forward fonksiyon sablonu aldığı argumanın tür bilgisini koruyarak geri dönmektedir.
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

template<typename T>
void call_foo(T&& t)
{
	foo(forward<T>t);
}

*/

/* 
*  Peki forward bu işlemi nasıl yapmaktadır? Forward fonksiyon sablonu iki farklı overload'a sahiptir ve bu overloadlerin
*  impl'si aşağıda verilmiştir. 
* 
*  Not: Aşağıdaki impl'de Forward fonksiyonunu detaylı açıklmayacağım, bunları sözel anlatmak çok zor ama şuana kadar
*  derste anlatılanlar ile kendim de kodu okuduğum zaman nasıl tür bilgisini geri döndürüyor gayet anlaşılır. Fakat anahtar nokta 
*  şurada. Forward fonksiyonu bir universal referans olan yerde çağrılıyor. Universal referans olan fonksiyona çağrı yapılan 
*  arguman sağ taraf değeri ise T yalın T olur, referans olmaz sadece T olur. Fakat Sol taraf değeri ile çağrılırsa fonksiyon
*  template argumanı T T& olur. Bu T nin durumuna göre aşağıdaki overloadlardan biri çağrılır.
* 
*/

/*

template <typename T>
T&& Forward(std::remove_reference_t<T>& t)
{
	return static_cast<T&&>(t);
}

template <typename T>
T&& Forward(std::remove_reference_t<T>&& t)
{
	return static_cast<T&&>(t);
}

*/

/* 
*  Dersin kalan ksımında type traitler işlendi ve implementasyonları yapıldı. Type traitler neydi? Compile time da
*  bize type ve değer döndürebilen meta fonksiyon/sınıf sablonlarıdır. Necati hoca is_pointer ve
*  ve is_pointer_t impl'sini aşağıdaki gibi yazdı. Çok mantıklı, okuması kolay compile time da anlaşılan bir meta
*  programming yapmıs olduk.
*/

/*

template <typename T, T val>
struct IntegralConstant {
	static constexpr T value = val;
};

using TrueType = IntegralConstant<bool, true>;
using FalseType = IntegralConstant<bool, false>;

template <typename T>
struct is_pointer : FalseType {

};

template <typename T>
struct is_pointer<T*> : TrueType{

};

template <typename T>
constexpr bool is_pointer_t = is_pointer<T>::value;

int main()
{
	is_pointer_t<int*>;
}

*/

/* 
*  Aşağıda da is_same için örnek impl gösterilmiştir. 
*/

/*

template <typename T, typename U>
struct isSame : FalseType{};

template <typename T>
struct isSame<T,T> : TrueType{};

*/

/*
*  Perfect forwarding mekanizması genelde variadic templateler ile yapılmaktadır. Çünkü çağrılan fonksiyona gönderilecek
*  arguman sayısı sabit olmayabilir. Mesela emplace_back böyle bir fonksiyondur, make_unique böyle bir fonksiyodur. Bunların
*  örnekleri aşağıda verilmiştir. 
* 
*/

/*

template<typename T, typename ...Args>
std::unique_ptr<T> MakeUnique(Args&& ...args)
{
	return std::unique_ptr<T> {new T(std::forward<Args>(args)...)};
}

template<typename ...Args>
void emplace(Args&&... args)
{
	//Note: c is a vector object.
	c.emplace_back(std::forward<Args>(args)...);
}

*/

/*
*  Not: Templateler ile ilgili(type parameter pack, no-type parameter pack, template template parameter gibi) bazı bilgiler ve
*  kodlar yazıldı perfect forwarding ile ilgili. Bu kodlar ve verilen bilgiler temel olarak variadic templateler ile ilgiliydi. 
*  Necati hoca templateleri çok geniş olarak ilerleyen haftalarda ele alacağı için bu bölümde ekstra not almadım.
*/
