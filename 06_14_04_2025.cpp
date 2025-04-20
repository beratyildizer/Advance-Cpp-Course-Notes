/*
*  Dersin ilk bölümlerinde forward fonksiyon şablonunun sağ taraf referansı olan overloadı için neden static_cast ifadesi
*  kullanılmış bunun üzerinde duruldu. İlk olarak Necati hoca static_assert ile ilgili çok kısa sözel bilgiler verdi.
*  static_assert Modern C++ ile hayatımız girmiştir ve bir anahtar kelimedir. İlk argumanı constant bir expression olması
*  gerekmektedir. Expression compile time da non-zero olarak ele alınırsa derleme işleminde bir hata olmayacaktır. Fakat 
*  compile time da değerlendirilen ifade true bir değer olarak değerlendirilirse derleme işlemi yapılamayacaktır. İkinci arguman
*  zorunlu değil fakat eğer istersek ikinci argumana bir yazı geçebiliriz ve compiler hata mesajında ikinci argumana geçtiğimiz
*  yazıyı gösterir.
* 
*  Şimdi aşağıya forwardın coğu derleyici tarafından yapılan common implementasyonunu tekrar yazalım.



#include <type_traits>
#include <utility>

template<typename T>
constexpr T&& Forward(std::remove_reference_t<T>& t)noexcept {
	return static_cast<T&&>(t);
}

template<typename T>
constexpr T&& Forward(std::remove_reference_t<T>&& t)noexcept {
	static_assert(!std::is_lvalue_reference<T>::value, "bad forward call");
	return static_cast<T&&>(t);
}

*/

/* 
*  Peki ikinci Forward implementasyonunda neden static_assert(!std::is_lvalue_reference<T>::value, "bad forward call") ifadesi
*  kullanılmaktadır. Aşağıdaki gibi yapılan çağrıları syntax hatası yapmak için static_assert eklenmiştir. Peki neden?
*  
*  main içerisindeki forward çağrısında sağ taraf referans parametreli forward templati çağrılacaktır, çünkü forwarding argumanı
*  bir sağ taraf değeri. Bu durumda T için explicit olarak tür myclass& olarak çıkarılacaktır ve sonuç olarak sağ taraf çağrılacaktır.
*  Dolayısı ile static_cast'te R value'yi L value'ye cast etmiş olacağım. R value hiçbir ihtimalde L value'ye cast edilmesin diye
*  böyle bir önlem alınmaktadır.

 
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
	foo(Forward<T>(t));
}

int main(){
	call_foo(Forward<myclass&>(myclass{}));
}

*/

/*
*  C++ 20 ile hayatımıza Abbreviated Template aracı girmiştir. Necati hoca bu aracı yada yöntemi Türkçe'ye kısaltılmış
*  template olarak çevirmiştir. Aşağıda örnekleri verilmiştir.
*/
 /*
 
template <typename T>
void foo(T);
void foo(auto x); // Abbreviated template, same as template<typename T> void foo(T) 

template <typename T>
void foo(T&);
void foo(auto& x); // Abbreviated template, same as template<typename T> void foo(T&) 

template <typename T>
void foo(T&&);
void foo(auto&& x); // Abbreviated template, same as template<typename T> void foo(T&&) 

*/

/*
*  Burada Necati Hoca derste şu soruyu sordu. Peki biz abbreaviated template aracını universal reference için kullanırsak
*  perfect forwarding mekanizmasını nasil implemente edebiliriz? Yani forward fonksiyonuna nasıl çağrı yapabiliriz?
*  Aşağıda bunu nasıl yapacağımız gösterilmiştir. Peki bu nasil çalıştı?
* 
*  Burada call_foo fonksiyonu universal reference. Ortada bir template T olmasada biz fonksiyon argumanı t nin ya sol taraf
*  referansı yada sağ taraf referansı olacağını biliyoruz. Dolayısı ile biz t yi decltype'a göndererek template T olmasada
   decltype(t) ile bunu yapabiliriz.
   
   Not: decltype'a sol taraf referansı olan x'i verirsek x bir isim olduğu için tür çıkarımı sol taraf referans türü olarak çıkarken
   sağ taraf referansı verirsek tür çıkarımı sağ taraf referansı olarak çıkacaktır.
*/

/*

void call_foo(auto&&) { // Abbreviated template, same as template<typename T> void call_foo(T&&) 
	foo(std::forward<decltype(t)>(t));
} 

*/

/*
*  Bir sınıfın member fonksiyonu da template olabilir. Bunu zaten biliyorduk. Buradan yola çıkarak o zaman biz
*  lambdalar ve Abbreviated template aracını kullanarak derleyicinin yazdığı closure type için bir member template
*  fonksiyonu yazabiliriz. Örneği aşağıda verilmistir
*/

/*

class neco_berat { //isime takılma, closure type için compilerın sınıfa verdiği ismi salladım.
public:
	template <typename T>
	int operator()(T x)const
	{
		return x * x;
	}
};

int main() {
	auto f = [](auto x) {return x * x}; // bu sekilde bir closure type içerisinde ki operator() fonksiyonunu template yapabiliriz.
										// Bu lambda ifadesi sonrası derleyicinin yazacağı closure type sınıfı yukarıda taklit edilmistir.

}

*/

/* 

Lambdalar ile universal reference ve perfect forwardingi aşağıda ne güzel implemente ettik:)

auto call_foo = [](auto&& t) {
	foo(forward<decltype(t)>(t));
};

*/

/*
*  Necati hocanın dersteki sorusu: Forward kullanmadan perfect forwarding'i nasil yapabilirsin.
*  Cevap: Tabi ki if constexpr ile, örnek kod aşağıda.
*/

/*
#include <iostream>
#include  <type_traits>

using namespace std;

struct myclass {};

template<typename T>
void cal_foo(T&& param)
{
	if constexpr (is_lvalue_reference_v<T>) {
		foo(t);
	}
	else {
		foo(move(t));
	}
}

*/

/* 
*  Aşağıda Person sınıfı gösterilmiştir. Bu person sınıfının iki farklı getName fonksiyonu overloadı vardır.Bu iki overload  
*  reference qualifier'ler ile yapılmıştır. Bu reference qualifierlar getter fonksiyonlarda çok sık tercih edilmektedir. Sebebide
*  gereksiz kopyalamanın önüne geçmek.
*/

/*

#include <string>
#include <iostream>

class Person {
private:
	std::string name;
public:
	void printf()const {
		std::cout << "printf\n";
	}

	std::string getName()const&& {
		return std::move(name);
	}

	const std::string& getName()const& {
		return name;
	}
};

*/

/* 
*  Peki biz universal reference parametreli bir template fonksiyonunun L value nesneler için çağrılmasını yasaklamak 
*  istesek bunu nasıl yapabiliriz? Bunun için conceptler veya SFINAE'yi kullanabiliriz.
*  Conceptler ile nasil yapilacabileceği aşağıda gösterilmiştir.
*/

/*
 Conceptleri çok iyi bilmediğim için buralarda fazla not almadım. Kursun ilerleyen bölümlerinde anlatılacaktır.


template<typename T>
requires (!std::is_lvalue_reference_v<T>)
void foo(T&& t) {
	bar(std::forward<T>(t));
}

*/

/*
*  SFINAE (Substition Failure Is Not An Error) daha önceden çok daha fazla popülerdi fakat conceptlerin standart
*  hale gelmesiyle adeta pabucu dama atıldı. SFINAE şu anlama geliyor. Derleyici bir fonksiyon sablonu gördüğünde ilk olarak
*  substition işlemini yapmaktadır yani type parametreler için bir tür çıkarımı yapar. Eğer bu tür işlemi sırasında veya
*  bunu yaptıktan sonra fonksiyonun imzasında bir syntax hatası alırsa bu fonksiyon yerine syntax hatası vermek yerine bunu
*  overload setten cıkarır. Zaten SFINAE açılımıda bununla ilgili fikir vermektedir
*  
*  SFINAE ile ilgili sorunun çözümü enable_if kullanarak şu şekilde yapılabilir.
*/

/*
 Genel olarak enable_if ve alias template olan enable_if_t araçlarını kullanarak bir fonksiyonun
 çağrılmasını belirli türler için engelleyebiliriz. Tamamen compie time'a yönelik bir impl. Yani metaprogramming.

#include <type_traits>

template<typename T, typename = typename std::enable_if_t<!std::is_lvalue_reference_v<T>>>
void bar(T&& arg) {
	//...
}

*/

/*
*  Peki şunu yapabilir miyiz, bir universal reference parametreli fonksiyon sablonunun sadece string türler için
*  çağrılmasını istiyoruz. Yine burada da conceptler veya SFINAE kullabilirim. Conceptler ile nasıl yapilabileceği
*  aşağıda gösterilmiştir.
* 
*  is_convertible_v kullanılmıştır. Bu trait bir türden diğerine dönüşüm var mi bunu kontrol eder.
*/

/*

template<typename T>
requires std::is_convertible_v<T, std::string>
void foo(T&&) {
	// ...
}

/*
*  Yukarıdaki foo fonksiyonuna aşağidaki Nec türden bir argumanda gönderebilirim. Çünkü sınıfın type castin operator
*  fonksiyonu vardır. is_convertible_v Nec türü için true değer olacaktır.
*/

/*
 
struct Nec {
	operator std::string()const;
};

*/

/*
*  Daha önceden hatırlarsak auto&& r1 = x; gibi bir auto kullanımınında universal reference olduğundan bahsetmiştik. 
*  Peki gerçekten auto&& olarak universal reference kullanabileceğimiz bir senaryo olabilir mi? 
*  Aşağıdaki gibi bir kod olduğunu düşünelim.
*/
 /*
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template<typename Con, typename T>
void Fill(Con& c, const T& val)
{
	for (auto& x : c)
		x = val;
}

int main() {
	vector<int> ivec(10);
	Fill(ivec, 42);

	for (auto i : ivec) {
		cout << i << " ";
	}
}*/

/*
*  Yukarıdaki kod gayet legal bir kod ve bir syntax hatası yoktur. Peki biz kodu aşağıdaki gibi yazsaydık ne olurdu?
*/

/*

#include <iostream>
#include <string>
#include <vector>

using namespace std;

template<typename Con, typename T>
void Fill(Con& c, const T& val)
{
	for (auto& x : c)
		x = val;
}

int main() {
	vector<bool> ivec(10);
	Fill(ivec, true);

	for (auto i : ivec) {
		cout << i << " ";
	}
}

*/

/*
*  Kesinlikle syntax hatası.Peki neden vector<bool> açılımı yapınca herhangi bir syntax hatası aldık. Aslında bu Necati
*  hocanın değimiyle C++'ın gençlik günahı. Derleyicinin for range based for loop için nasil bir kod yazdığını anlayabilirsek
*  olay çözülüyor.
*/

/*
* 
for (auto& elem : con) {  // bu for range based loop için derleyici aşağıdaki kodu yazıyor. Açıklamada aşağıda.
	elem
}

*  Derleyici yukarıdaki con'u bir universal reference'ye bağlıyor. Bu sayede L value'de R value'de olsa con bir problem olmayacak. 
*  Aşağıda gösterilmiştir bu bağlama. Daha sonra for döngüsünü de yukarıda ki range based for loop için aşağıdaki gibi yazıyor.
* 
*  Aşağıdaki kodda auto& elem = *ibeg; atamasında *ibeg'in bir L value expression olması gerekmektedir. vector<bool> açılımı vectorun
*  bir partial specialiazationudur. vector<bool> açılımı yani bir bool tutmuyor ve bit seviyesinde bu işlemi yapıyor ve bir proxi sınıf 
*  kullanıyor ilgili bite erişmek için. Bu proxy sınıf da geri döndüğü yerde sağ taraf değeri olarak döner. Fakat şu soru akıllara 
*  geleblir. Nasil olurda ben bu proxu sınıfı bool gereken yerde kullanabilirim yada bu vector<bool> açılımını nasil olurda
*  herhangi bir bitin değerini bool türden bir değişkende saklayabilirim. Çünkü bu proxi sınıfın bool'a typecast yapan bir
*  operator bool fonksiyonu vardır. 

auto&& range = con;

for (auto ibeg = begin(range), iend = end(range); iend != ibeg; ++ibeg) {
	auto& elem = *ibeg; 
}

*/

/* 
*  Özet: Vector<bool> açılımı içinde bool tutan bir vector container değildir, tam tersine vector<bool> olarak implemente edilmiş
*  ayrı bir vector specializationudur. Ve içeride her bool bitsel olarak tutulmuştur. Peki bu specialization nasil olurda herhangi
*  bitsel olarak tutulan bir bool'a erişmek istediğimizde bunu biza sağlar. Mesela aşağıda nasil olurda bool b ataması syntax hatası
*  olmadı.
*  vector<bool> bvec(20);
*  bool b = bvec[10];
*  Dediğimiz gibi bvec[10] fonksiyonu bir proxy sınıf döner fakat bu proxy sınıf bir operator bool typecast fonksiyonuna
*  sahip olduğu için bool b ye atama legal olmuş oldu. Ama biz yukarıda *ibeg ifadesini direk olarak auto& elem'e atadık
*  ve burada *ibeg sağ taraf referencesi proxi sınıf dönmüştür, bu sebeple syntax hatası olmustur. Bunu engellemek için
*  for range based loop içerisinde (Fill fonksiyonunda) auto&& yapabiliriz.
*/

/*
*  Auto&& kullanabileceğimiz bir diğer senaryoda şöyle olabilir. Bir fonksiyonun geri dönüşünü bir nesnede tutacağız
*  ve daha sonra o türü koruyarak başka fonksiyona arguman olarak geçeceğiz. 
*  
*  Bu şekilde anlatması biraz karışık olmuş olabilir ama aşağıda bunun için örnek bir kod ve yorum satırları bulunmaktadır.


template<typename T>
void foo(T&& t) {
	auto&& ret = bar(std::forward<T>(t)); // universal reference

	// .. some code here
	
	baz(std::forward<decltype(ret)>(ret)); // eğer ret değişkenini direk olarak baz fonksiyonuna gönderseydik burada 
										  //  ret sağ taraf referansı olsada kullanıldığı yerde L value bir expr olur. Bu sebeple	
										//   tekrardan forward kullandık. Compile time da tür çıkarımı için de ret için decltype'ı kullandık.
}

*/

/*
Aşağıda auto return type gösterilmiştir. Eğer bir bir fonksiyonun geri dönüş türünü koruyarak çağıran fonksiyondan return etmek
istersek aşağıdaki gibi bir impl. bu işe yaramayacaktır. Çünkü fonksiyonun geri dönüş değeri L value'de R value'de olsa
auto referansdan arındırılmış bir tür olacaktır.

template <typename T>
auto call_foo(T&& t)
{
	return foo(std::forward<T>(t));
}

*/

/*
*  Peki bunun yolu nedir? Yani wrapper fonksiyonun çağırdığı fonksiyonun geri dönüş türünü de koruyarak nasil return edebiliriz.
*  Bunun yolu decltype(auto) dan geçmektedir. decltype(auto) C++ 14 ile dile eklendi. 
*  
*  Necati hoca decltype(auto) ile ilgili açıklama yapmadan tekrarda decltype için uygulanan tür çıkarım kurallarından bahsedildi.
*  Ben burada genel olarak bir açıklamaya yapacağım.
*  
*  Eğer decltype operatatörünün operandı bir isim yani identifier ise operandın türü bir nevi geri döndürülür. Yani operand bir isim
*  yani expression değilse operandın türü ne ise deduce edilen tür de o dur.
* 
*  Fakat operand bir expression ise işletilen kurallar biraz farklıdır. Bu kurallar özet olarak şöyledir.
*  
*  Eğer decltype operatörünün operandı bir isim değil yani bir ifade ise; örneğin
*  decltype(19), decltype (y + 1), decltype((y))
* 
*  böyle bir durumda deduce edilecek tür operand olan expressionun değer kategorisine bağlıdır.
*  1. Pr value ise tür T türüdür. 
*  2. L value ise tür T& türüdür.
*  3. X value ise tür T&& türüdür.
* 
*  Not: Sağ taraf referansı türünden dönüş değeri dönen fonksiyonlara yapılan cağrı X value değer kategorisine sahiptir, 
*  tıpkı move gibi.
* 
*  Not: Decltype da sizeof, noexcept, typeid gibi işlem kodu üretmiyor operandı için. Yani operand'ı unevaluated context'tir.
* 
*  Peki decltype(auto) için tür çıkarımı nasil oluyor? Burada da tür çıkarımı decltype ile bire bir aynıdır.
*  


int&& foo();

int main() {
	int x = 10;
	decltype(auto) y = foo(); // foo çağırısı X value dir, dolayısı ile y nin türü int&&.
}

*/

/*
*  Şimdi tekrardan buraya yani decltype(auto) ya nereden geldik oraya dönelim. Amacımız neydi; bir fonksiyon var, başka fonksiyonu
*  çağırıyor. Çağırdığı fonksiyonun geri dönüş value categorisinide koruyarak geri dönmek istiyor. İşte burada decltype(auto) yu 
*  kullanabiliriz. Aşağıda örneği verilmiştir. Buna perfect returning denmektedir.

template<typename T>
decltype(auto) foo(T&& t)
{
	return bar(std::forward<T>(t));
}

// Standar kütüphanede invoke, apply gibi araçlarda bu şekilde implemente edilmiştir.

*/

/* 
*  Necati hoca daha sonra deferred perfect returning mekanizmasından bahsetti. Bu mekanizma da bir fonksiyonun geri dönüş değerini 
*  bir local nesnede tutacağız ve bazı kodlardan sonra o local değikeni return edeceğiz. Fakat burada anahtar nokta ilk çağrılıp
*  geri dönüş değeri tutulan fonksiyonu return ediyoruz ya, iste onun value categorisini de tutup return edeceğiz.

   Bir önceki örnekten farkı direk olarak bar'ı return etmek yerine bar'ın geri dönüş değerini saklayıp başka kodlardan sonra
   return etmek. Bunu da aşağıdaki gibi yapabiliriz. Burada kodun açıklamasını Necati hoca detaylı olarak yaptı ama 
   direk olarak da ezberleyedebilirsiniz dedi.

template <typename F, typename ...Args>
decltype(auto) call(F&& f, Args&& ...args)
{
	decltype(auto) ret = f(std::forward<Args>(args)...);
	// some code here

	if constexpr (std::is_rvalue_reference_v<decltype(ret)>) { // ret referansdan bağımsız kullanıldığı yerde sol taraf değeri
		return std::move(ret);								  // bu sebepten direk return ret yapmadan if constexpr kullandık.
	}
	else {
		return ret;
	}
}

*/

/*
*  Deferred perfect returningi böyle de yapabilir miyiz? Necati hocaya telegramdan sordum.
*  
template <typename F, typename ...Args>
decltype(auto) call(F&& f, Args&& ...args)
{
	decltype(auto) ret = f(std::forward<Args>(args)...);
	// some code here

	return return std::forward<decltype(ret)>(ret);

Cevap:
Hayır.
Bu durumda çağrılan f fonksiyonunun geri dönüş türü referans olmayan bir tür ise, call fonksiyonunun geri dönüş türü sağ 
taraf referans türü olur.
Yanlış olmakla birlikte tanımsız davranış olur. Çünkü geri dönüş türü referans türü olan bir fonksiyon otomatik ömürlü bir 
nesne döndürmüş oluyor.
}

*/
