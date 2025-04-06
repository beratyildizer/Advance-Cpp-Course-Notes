/*
*  Reference Qualifier lar daha detaylı ele alındı. Derste reference qualifier ile ilgili
*  bahsedilenler şöyledir.
*
*  Reference qualifierlar sınıfların non static member fonksiyonlarının hangi türden nesneler tarafından
*  çağrılabileceğini belirlemek için kullanılabiliyor. Örneğin öyle bir member fonksiyon yazalım ki sadece L value
*  expression olan sınıf nesneleri için çağrılsın. Peki bu bize ne kazandırır. Şüphesiz biz reference qualifierları kullanarak
*  bir member fonksiyon için sol taraf expression olan sınıf nesneleri aracılığı ile çağrıldığında farklı bir impl. yazarız,
*  sağ taraf expression olan sınıf nesneleri ile çağrıldığında farklı bir impl. yazarız.
*/

/*
*  Örneğin aşağıdaki kodda reference qualifier ile foo fonksiyonu için iki farklı overlaod yazdık. Overloadlar dan birisi sadece
*  L value expression olan sınıf nesneleri için çağrılabilirken, diğeri ise sadece R value expression olanlar için çağrılacak. Ekrana aşağıdaki
*  kod çalıştırıldığında şu yazacaktır.
*
*	L Value Expression
*	R Value Expression
*/

/*

#include <iostream>

class Nec {
public:
	void foo()& { std::cout << "L Value Expression\n"; };
	void foo()&& { std::cout << "R Value Expression\n"; }
};

int main(void)
{
	Nec x;
	x.foo();
	Nec{}.foo();

	return 0;
}

*/

/*
*  Aşağıdaki foo fonksiyonlarının hepsi overloaddır, redecleration değildir.
*/

/*
class Nec {
public:
	void foo()const& {};
	void foo()& {};
	void foo()&& {};
	void foo()const&& {};
};
*/

/*
*  Reference qualifier olan overloadlara reference qualifier olmayan overloadlar eşlik edemez. Syntax hatası olur.
*  Bu durum şu şekilde ifade edilir; cannot overload a member function with ref-qualifier.
*/

/* Şimdi aşağıda bulunan kod hakkında konuşalım. En son söyleyeceğimizi ilk baştan söyleyelim. Aşağıdaki gibi bir kod yazmamalıyız.
*  Çünkü aşağıdaki gibi yazılan bir kod dangling referans sorununu ortaya çıkarıyor. Peki nasil ve neden?
*
*  Aşağıdaki kodda aslında dangling referansa neden olan durum for range based loop için derleyicinin nasıl bir kod ürettiği ile ilgili.
*  Derleyici aşağıdaki kodu aslında üretiyor.
*
auto&& range = get_sentence().str();

for (auto iter = range.begin(); iter != range.end(); ++iter) {
	char c = *iter;
	cout << c;
}

*  Burada range bir universal referencedir. Yani ne olursan ol yine gel. Burada tür çıkarım kuralları reference collapsing kurallarına göre belirlenmektedir.
*  universal reference'e atama yapılan işlem de bir sağ taraf değeri olduğu için range'in tür sağ taraf referansı oldu. Fakat daha sonra for içerisinde
*  range kullanılan her yerde range bir dangling referanstır. Sebebi ise açıktır. get_sentence fonksiyonu ile get edilen Sentence sınıf türünden nesnenin
*  ömrü fonksyion çağrısı ile bitti ve nesne içerisinde ki bir membera referans dönen str fonksiyonunu kullandık. Hayatta olmayan nesnein memberını göstermeye çalış
*  tığı için range artık bir dangling referanstır.
*
*  Özet olarak derleyicinin for range based loop için ürettiği kodu incelediğimiz hayatı bitmiş bir nesnenin memberına referans ile döndüğümüzde dangling referans
*  durumu meydana gelebilir.
*/

/*

#include <iostream>
#include <string>

using namespace std;

class Sentence {
public:
	Sentence(const char* p) : ms{ p } {};
	const string& str()const
	{
		return ms;
	}

private:
	string ms{};
};

Sentence get_sentence()
{
	Sentence str{ "Ilk Ders" };

	return str;
}

int main()
{
	for (char c : get_sentence().str()) {
		cout << c;
	}

	return 0;
}

*/

/*
*  Şüphesiz buradaki str fonksiyonu referans olmasaydı dangling referans durumu oluşmayacaktı.
*  Çünkü fonksiyonun geri dönüş değeri direk olarak string olduğunda direk olarak fonksiyonun implemetasyonuna göre ya member ms nesnesi kopyalanacak
*  yada taşınacaktı.
*
*  İşte buradaki sorunu reference qualifier lar ile çözebiliriz. Sağ taraf expression nesne olarak str fonksiyonu çağrıldığı senaryoda geri
*  dönüş türünü direk string nesnesi yapıp, sol taraf nesneler için referans yaparsak bu sorun çözülür. Aşağıda örneği gösterilmiştir.
*
*/

/*

#include <string>

class Sentence {
public:
	Sentence(const char* p) : ms{ p } {};
	std::string str()&&
	{
		return std::move(ms); // move fonksiyonunu kullanmasak da olurdu, fakat bu sefer bir deep copy olacaktı ms member string nesnesi için.
	}

	const std::string& str()const&
	{
		return ms;
	}
private:
	std::string ms;
};
*/

/* 
*  Move only typeların genel olarak ne anlama geldiğinden bahsedildi. Bir sınıf türünü move only yapabilmek için
*  copy ctor ve copy assigment fonksiyonlarını delete edip move ctor ve move assigment special member functionları tanım
*  lamamız yeterlidir.Fakat biz move ctor ve move assigment fonksiyonlarını yazarsak zaten derleyici copy ctor ve copy assigment fonksiyonunu delete
*  ettiği için yani implicitly declared but deleted durumda olduğu için bu fonksiyonlar bizim bunları tekrardan user olarak delete etmemiz
*  zorunlu değildir. Standart kütüphane de unique_ptr, thread, jthread, ostream, unique_lock, shared_lock, future, promise move only nesne türleridir.
*/

/*
#include <vector>

using namespace std;

class MoveOnly {
	MoveOnly() = default;
	MoveOnly(MoveOnly&&);
	MoveOnly& operator=(MoveOnly&&);
	//....
};

int main()
{
	vector<MoveOnly> mvec(10);

	for (auto m : mvec) { // Burada syntax hatası olmasının sebebi açıktır. mvec sadece moveable olan MoveOnly sınıf türündeen nesneler tuta 
					     //  bir vector nesnedir ve burada for range based loop da vector de tutulan move only nesneler kopyalanmaya çalışıyor.
		                 // derleyici şu mesajı verecektir; attempting to reference a deleted function. auto& m : mvec olsaydı veya universal ref. olsaydı bir problem olmazdı.

	}
}
*/

/*
*  Aşağıda derste yazılan bir örnek
* 
#include <memory>
#include <vector>
#include <string>

using namespace std;

using uptr = unique_ptr<string>;

int main()
{
	vector<uptr> vec;

	uptr ptr{ new string{"Necati Ergin"} };

	//vec.push_back(ptr); // syntax hatası. unique_ptr moveable bir type. Burada ptr sol taraf expression olduğu için push back in kopyalayan overloadı çağrılıyor.
	vec.push_back(move(ptr)); //legal
	vec.push_back(make_unique<string>("Necati")); // legal, fabrika fonksiyonlar sağ taraf değeri döndürürler. (L value to X Value conversion)
	vec.push_back(uptr{ new string("ayse") }); // legal
	vec.emplace_back(new string{ "ali" }); // direk olarak nesneyi ilgili bellek alanında construct ediyor, bu yüzden uptr nesnesinin constructoruna arguman veriyoruz.
	vec.emplace_back(); // uptr nesnesini direk ilgili yerde default ctor etti. Yani uptr nesnesi vectorde tutulan bu insertte boş.
}
*/

/*
*  Şimdi yeni bir iterator adopter öğreneceğiz. Bu iteratoru öğrenmeden önce iterator adopter nedir onu konuşalım.
*  Aslında iterator adopterler iteratorleri sarmalayan ve onları kendi interfacesi ve interfacesinde ki impl'ler ile yeni 
*  özellikler kazandıran sınıflardır. Örneğin reverse_iterator, const_iterator. Bunlar gibi bir move_iterator vardır. Move iterator
*  sarmaladığı iteratoru aslında kendi operator member fonksiyonlarıyla sarmaladığı iterotoru  utility move'a gönderip geri dönüşünü döndürür.
*  Sözel olarak anlatması biraz zor, aşağıdaki kodlar ile açıklamaya çalıştım.
*/

/*
#include <vector>

using namespace std;

class Myclass{};

vector<Myclass> vec(10);
move_iterator<vector<Myclass>::iterator> iter{vec.begin()};
auto x = *iter // move ctor cağrılacak, sebebi aşağıda.

// burada artık *iter dediğimiz de bu expression'nun value categorysi X Value yani R value dir. Çünkü move iterator interfacesinde
// operator* member fonksiyonu ilgili iteratorun gösterdiği nesneyi şöyle döndürür; std::move(*iter) -> *iter;

Not: Yukarıdaki iter nesnesini CTAD dan faydalanıp şu şekilde de define edebiliriz.
move_iterator iter(vec.begin())

*/

/*
*  STL de bulunan algoritmaları kullanırken range olarak move iterator kullanırsak containerları taşırız. Örneği aşağıda verilmiştir.
*/

/*
#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <string>

using namespace std;

using uptr = unique_ptr<string>;

int main()
{
	vector<uptr> vec;
	const char* pa[] = { "Berat", "Utku", "Hasan", "Volkan", "Kutay" };

	for (auto p : pa) {
		vec.push_back(make_unique<string>(p));
	}

	//list<uptr> ulist{ vec.begin(), vec.end() }; // gecersiz, cunku unique_ptr bir moveable nesne, kopyalanamaz.
	list<uptr> ulist( move_iterator<vector<uptr>::iterator>(vec.begin()), move_iterator<vector<uptr>::iterator>(vec.end())); 
	//süphesiz yukarıdaki ifadeyi algorithm move fonksiyonu ile de yazabilirdik. O zaman da taşıma olurdu.
	// yukarıdaki ifade de move_iterator için template tür argumanını açık açık yazmak yerine CTAD'dan da faydalanabilirdim.
	

	for (const auto& up : vec) {
		cout << (up ? *up : "empty") << ' ';
	}
	std::cout << endl;
	for (const auto& up : ulist) {
		cout << (up ? *up : "empty") << ' ';
	}
}
*/

/*
*  Ayrıca move_iterator için de bir fabrika fonksiyon bulundurulmuştur ve klasik olarak bu fonksiyonda make ile başlamaktadır.
*  make_move_iterator.
* list<uptr> ulist( make_move_iterator(vec.begin()), make_move_iterator(vec.end())); 
*/

/*
*  Not: Hatırlatma amaçlı derste tekrardan söz edildiği için yazıyorum. Bir fonksiyon düşünün. Geri dönüş türü referans değil ise 
*  o fonksiyon expression olarak kullanıldığı yerde R value'dir. Eğer sağ taraf referansı ise geri dönüş türü X value yani yine
*  R Value dir. Fakat geri dönüş türü L value referans ise expression olarak kullanıldığı yerde L value oluyor value catogory. 
*/

/*
*  Dersin kalan bölümünde C++ da bulunan exception mechanism anlatıldı.
* Şimdi exception safety denen bir kavram ve bunun 4 farklı derecesi vardır. Bu exception safety exception throw durumunu ve throw edilen bir exceptiondan 
  sonra kodun durumunu bize açıklar. Levellar ve açıklaması sırayla söyledir.
  No-throw guarantee: Burada ilgili fonksiyonun veya kodun exception throw etmeyeceği garanti altındadır. Anladığım kadarıyla noexcept olarak bildirilen
  fonksiyonlar bu kısımdadır.

  Strong exception safety: Herhangi bir exception throw edildiği zaman bu exception yakalanır ve exceptioınun bir yan etkisi programa olmayacaktır. 
  Program orjinal yerine geri dönecektir. Nesneler ayrıca eski değerini koruyacak. Orjinal yerine dönmekten kasıt budur aslında.

  Basic exception safety: Bir execution throw edildikten sonra bu execution yakalanacaktır. Herhangi bir memory leakage olmayacağı garanti altındadır. 
  Fakat nesnenin değerinin koruyacağı garantisini vermiyor.

  No exception quarentee: Bir exception throw edildiğinde programın ne yapacağı belli değildir. Resource leakage,memory corruption, aklınıza ne gelirse olabilir.

*/

/*
Bizim run time oluşabilecek hatalar ile başa cıkmamızı yarayan system exception handling dir. C de olmayan C++ olan araçlar ile exception
handling mekanizmasını kullanabiliyoruz. C dilinde de exception handling mekanizmasını kullanabiliriz fakat dilde bunun için araç yok, kendimiz bu implementasyonu 
yapabiliriz. Söyle düsünmeliyiz. Bir fonksiyon var ve bu fonkisiyonun kodu doğru. Run time da bilinmeyen bir nedenden dolayı işlevini yerine 
getiremezse bu hata ile ilgili bu fonksiyonu çağıran fonksiyonun bilgilendirilmesi gerekmektedir. İşte bu mekanizma exception handling’dir.
Belki işlevini yapamayan fonksiyonu çağıran fonksiyonuda başka bir fonksiyon çapırıyor ve hiyerarşi olarak en yukarıda bir fonksiyon oluyor.
İşte hata durumundan en yukarıdaki fonksiyonu bilgilendirmemiz gerekmektedir. Bu hatanın en üst fonksiyona iletilmesine exception throw denmektedir.
Bu exception handling modern derleyicilerle run time etkisini nerdeyse yok oldu. Yüzdesel olarak yüzde 3 gibi bir etki söyleniyor. 
*/

/*
* Modern C++ ile noexcept specifierı ve noexcept anahtar sözcüğü dile eklenmiştir. Buradaki dezavantaj ise bu aynı iki
* kelimenin hem specifier hemde anahtar sözcük olarak kullanılmasıdır. Bu nokta bazen zorluk cıkarabilmektedir.
*/

/*
* Aşağıdaki sekilde bir syntaxa sahiptir noexcept specifierı. Burada noexcept specifierını kendimi tek basına yazarsak func fonksiyonu içerisinde herhangi
* bir exception throw edilmeyeceği garantisini derleyiciye veriyoruz ve derleyici buna göre bir optimizasyon yapma imkanı buluyor. Eğer noexceptten
* sonra parantez açarsak parantez içerisine bir sabit ifadesi yazmalıyız. Bu sabit ifadesinin değeri true ise derleyiciye func fonksiyonu içerisinde yine 
* exception throw edilmeyeceğinin garantisini veriyoruz. Eğer bu sabit ifadesinin değeri false ise bu garantiyi maalesef vermiyoruz. 
*  void func()noexcept;
*  void func()noexcept(true);
*/

/*
*  Aşağıda no except operatoru gösterilmiştir. operator için yazılan expression compile time’da evaluate edilir. Eğer expr herhangi bir exception throw etmeme garantisine 
*  sahipse true değer dönerken potential olarak exception throw edebilecek bir ifade ise false döner. Buradaki expression da sizeof 
*  gibi decltype gibi unevaluated context içerisindedir.
*  noexcept(expression)
*/

/*
*  Aşağıda birinci noexcept specifier iken ikinci noexcept operatördür. 
*  void f();
*  vod foo()noexcept(noexcept(f())); 
*/

/*
*  Not : noexcept specifierı fonksiyonun imzasının bir parçası. Yani bildirimde varsa tanımda da olmalıdır. 
* Şimdi derleyici eğer özel üye fonksiyonları kendisi yazarsa bu özel üye fonksiyonların hepsi noexcept olarak yazılır. Eğer
* özel üye fonksiyonları biz yazarsak bu özel üye fonksiyonların noexcept olup olmayacağının sorumluluğu bize aittir.
*/

/*
#include <iostream>
#include <type_traits>

using namespace std;

template <typename T>
void func(T)noexcept(is_nothrow_default_constructible_v<T>)
{

}

class Nec {
public:
	Nec()noexcept;
};

int main()
{
	constexpr auto b = noexcept(func(Nec{}));
	cout << b; //ekrana 1 yazar, güzel bir örnek.
}
*/

/*
*  Not: Noexcept specifierı ile tanımlanmış bir fonksiyon pointerına sadece noexcept tanımlanmış bir fonksiyon adresi atayabiliriz.
*/

/*
*  Promise no less, require no more. Buradaki ifade Liskov	Substitotion Principle ile ilgilidir. Bu prensibin sağlanması için
*  türemis sınıf base sınıfın verdiği garantilerden daha az vermemesi gerekiyor. Yani taban sınıfın sanal fonksiyonu 
*  noexcept ise override edilen türemiş sınıftaki sanal fonksiyonda noexcept olmalı, aksi syntax hatasıdır. Fakat bunun tersi senaryo
*  için bir problem yok. Türemiş sınıf taban sınıfın verdiği garantinden daha fazla garanti verebilir.
*/

/*
*  Vector sınıfının push_back fonksiyonu strong exception garanti vermektedir. Yani eğer yapılan insert işlemi bir nedenden dolayı
*  başarısız olursa ilgili container exception throw işleminden sonra önceki state sini koruyacak ve herhangi bir kapsam sızıntısı olmayacaktır.
*/