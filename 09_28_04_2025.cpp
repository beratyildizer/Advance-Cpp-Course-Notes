/*
*  Optional sınıf sablonunu görmeye devam ediyoruz. Optional dan sonra yine C++ 17 ile hayatımıza giren
*  variant ve any'i öğreneceğiz. Bu 3'lü için vocabulary types'da denmektedir. Bunlardan optional ve variant
*  bir sınıf şablonuyken any direk bir sınıftır. 
*  
*  Optional taşıma semantiğinden faydalanabiliyor, container'da tutulabiliyor, C++ 20 ile artık monadic
*  operasyonları da desteklemeye başladı. Monadic operasyonlarla ilgili detaya dersin devamında geleceğiz.
*  
*  Bu sınıfın muhakkak constructorlarını iyi bilmeliyiz, member fonksiyonlarını iyi bilmeliyiz tam olarak etkin 
*  bir biçimde kullanabilmek için.
* 
*  Optional bir nesneye sahipken, sahip olduğu nesneyi bıratırmanın, artık boş olmasının, sahip oldugu nesnenin hayatını
*  bitirmeyi aşağıdaki 3 yol ile yapailiriz.

using namespace std;

optional op(3566);

op = nullopt;
op.reset();
op = {};

* Yukarıda görüldüğü gibi op nesnesini hayata getirirken Class Template Argument Deduction(CTAD)'dan faydalandık. 
* Necati hoca optional ile ilgili kodları yazarken neredeyse template argumanını explicit yazmak yerine sürekli
* CTAD'dan faydalandı.
*  
* Optional nesnesi ya dolu olacak ya boş olacak. Dolu olma durumunu engage olarak tanımlayanlarda var. Bir optional
* nesnesi dolu mu boş mu bunu anlamanın yolu operator bool veya has_value fonksiyonunu kullanmaktır. Ayrıca tuttuğu nesneye erişmek için 
* de * veya -> operatorlerini kullanabiliriz ki bunun da anlamı aslında optional interfacesinde bu operator fonksiyonlar da implemente
* edilmiştir. Tuttuğu nesneye erişmek için birde value member fonksiyonu var. Bu fonksiyonun operator* ve operator-> dan 
* en büyük farkı şudur; eğer optional nesnesi birşey tutmuyorsa ve biz value fonksiyonuna çağrı yaparsak bu fonksiyon exception
* throw ediyor. 
* 
* Derste Necati hocaya sorduğum soru:
* Tutacağımız nesne başka nesnenin kaynağını çalarak hayata gelebilir mi hocam?
* 
* Cevap: Kesinlikle evet. Aşağıda bunu gösteren güzel bir örnek vardır.
* 

#include <iostream>
#include <string>
#include <optional>

int main()
{
	using namespace std;

	string str(100, 'A');

	optional<string> op(move(str));

	std::cout << str.length();
}

*  Aşağıdaki kodda tekrardan op2 nesneyi hayata gelirken bir taşıma oldu. Fakat op nesnesi hala bir string tutmaya devam ediyor
*  sadece tuttuğu stringin kaynağı çalınmış. Ekrana engage yazacak.


#include <iostream>
#include <string>
#include <optional>

int main()
{
	using namespace std;

	string str(100, 'A');

	optional<string> op(move(str));

	auto op2 = move(op);

	cout << (op ? "engaged" : "empty");

}

*/

/*
*  Derste bir öğrenci optional sınıfının iteratoru var mi diye bir soru sordu. Necati hoca olmadığını söyleyip eğer
*  optional nesnesinde bir container tutuluyor ve containerın iteratorune ihtiyacımız var ise aşağıdaki kodu örnek gösterdi.

#include <iostream>
#include <vector>
#include <optional>

int main()
{
	using namespace std;

	optional op = vector<int>{ 3,7,9,2,8 }; //CTAD

	for (auto iter = op->begin(); iter != op->end(); ++iter) {
		cout << *iter;
	}

}

*/

/*
#include <iostream>
#include <vector>
#include <optional>

int main()
{
	using namespace std;
	int x = 10;
	int* p = &x;
	int** ptr = &p;
	
	optional op = ptr;
	cout << ***op; //ilk yıldız op nin tuttuğu nesneyi get ediyor, diğer iki yıldız pointer to pointer.

	//cout << **op.operator*(); // buda acik hali

}

*/

/*
*  Optional sınıfının birde emplace isimli bir member fonksiyonu vardır. Bu fonksiyon aldığı argumanla nesneyi direk
*  olarak new placement operatorü ile ilgili yerde ctor ediyor, yani verdiğim argumanları tutulacak nesnenin ctoruna geçerek
*  nesneyi in-place de hayata getiriyor. Fakat optional nesnesi dolu iken bir emplace çağrısı yaparsak nesneyi hayata getirmeden
*  önce tutulan nesneyi desstroy ediyor, yani tutulan nesnenin dtor u çağrılıyor.
*  
*  Constructs the contained value in-place. If *this already contains a value before the call,
   the contained value is destroyed by calling its destructor.


Örneğin aşağıdaki kodda getchar çağrısına standart çıkış akımına şu yazdırılacaktır;
Myclass()
dtor
Myclass(int, int, int)


#include <iostream>
#include <optional>

class Myclass {
public:
	Myclass() {
		std::cout << "Myclass()\n";
	}
	Myclass(int, int, int) {
		std::cout << "Myclass(int,int,int)\n";
	}

	~Myclass() {
		std::cout << "dtor\n";
	}
};

int main() {
	using namespace std;

	optional<Myclass> op;

	op.emplace();
	op.emplace(1,2,3);
	(void)getchar();
}

*/

/*
*  optional sınıfının birde make_optional isimli fabrika fonksiyonu vardır. Bu fonksiyon tıpkı diğer fabrika fonksiyonları
*  gibi bizim verdiğimiz template türünden optional açılımını return etmektedir. Örneğin aşağıdaki kodda op nesnesinin türü
*  optional<Myclass> açılımı olacaktır. 

#include <optional>
class Myclass {
public:
	Myclass() {
		std::cout << "Myclass()\n";
	}
	Myclass(int, int, int) {
		std::cout << "Myclass(int,int,int)\n";
	}

	~Myclass() {
		std::cout << "dtor\n";
	}
};
using namespace std;

auto op = make_optional<Myclass>(2, 5, 9);

*/

/*
*  Not: Standartlar optional implementasyonun da dinamik memory allocation yapılmamasını dikte etmislerdir, yani optional içerisinde
*  çalışma zamanında herhangi bir allocation yapılmayacaktır, herşey compile time da allocate edilecektir.
*/

/*
*  Aşağıdaki kodda CTAD dan faydalanılmıştır. op için tür çıkarımı optional<string> olacaktır, çünkü standart
   operator""s user defined literal operatoru aslında çağrılıyor ve bu operatorde bir string geri dönüyor.

#include <iostream>
#include <optional>

int main()
{
	using namespace std;
	optional op("hakan"s);
	cout << typeid(op).name() << '\n';
}
*/

/*

#include <iostream>
#include <optional>

using namespace std;

class Myclass {
public:
	Myclass()
	{
		cout << "ctor" << '\n';
	}

	Myclass(const Myclass&)
	{
		cout << "copy ctor " << '\n';;
	}

	Myclass& operator=(const Myclass&)
	{
		cout << "copy assigment" << '\n';
	}

	Myclass(Myclass&&)
	{
		cout << "move ctor " << '\n';
	}

	Myclass& operator=(Myclass&&)
	{
		cout << "move assigment" << '\n';;
	}
};

int main()
{
	// Aşağıdaki kodda ilk olarak geçici nesnenin ctor'u çağrıldı, daha sonra geçici nesnenin kaynağı optional nesnesinin
	// sarmaladığı Myclass nesnesini hayata getirirken çalındı ve move ctor çağrldı. 
	
	std::optional<Myclass> op{ Myclass{} }; 
}

*/

/*
*  Atama operator fonksiyonlarından bahsedildi. Atama operator fonksiyonları birer template olduğu için farklı türlerden atama
*  yapabiliyoruz. Ayrıca nullopt_t türünden de bir atama operator fonksiyon overloadı var.

#include <optional>
#include <string>

int main() {
	std::optional<std::string> s1{ "necati" };
	std::optional<std::string> s2{ "sade" };

	s1 = s2;

	if (s1.has_value() && s2.has_value()) {
		*s1 = *s2; // burada T türünün operator= fonksiyonu cagrilir.
	}
	else if (!s1.has_value() && s2.has_value()) {
		s1.emplace(*s2); // s1 boşsa ve s2 doluysa yeni bir T nesnesi oluşturulur.
	}
	else if (s1.has_value() && !s2.has_value()) {
		s1.reset(); // s2 boşsa, s1 i boş hale getirir (destructor çağrılır)
	}

	// İkiside boşsa hiçbirşey yapmaz.
	
}

*/

/*
*  Biz optionalın sarmaladığı nesneye operator*, operator-> veya value fonksiyonları ile erişebiliyorduk. Eğer boş bir optional nesnesinin
*  operator* veya operator-> fonksiyonlarına çağrı yaparsak bu tanımsız davranış olur fakat value fonksiyonuna yaparsak value fonksiyonu
*  exception throw eder(std::bad_optional_access if *this does not contain a value.). Aşağıda bununla ilgili bir örnek verilmiştir.

#include <optional>
#include <string>
#include <iostream>

int main()
{
	using namespace std::literals;

	std::optional op{ "Hasan Kaynar"s };

	std::cout << op.value() << '\n';
	op.value().assign(5, 'A');
	std::cout << op.value() << '\n';
	op = std::nullopt; // nullopt_t parametreli operator= çagrilacak.

	try {
		std::cout << "value is " << op.value() << '\n';
	}
	catch (const std::bad_optional_access& ex) {
		std::cout << "exception caught: " << ex.what();
	}
}

*/

/*
*  Birde value_or fonksiyonu var. Returns the contained value if *this contains a value, otherwise returns default_value.
*  default_value value_or'a gönderdiğimiz arguman. Aşağıda bununla ilgili derste yazılan güzel bir örnek.

#include <optional>
#include <string>
#include <iostream>

void display_e_mail(const std::optional<std::string>& op) {
	std::cout << "e_mail: " << op.value_or("not_declared") << '\n';
}

int main()
{
	std::optional<std::string> e_mail_address{ "necati@gmail.com" };
	display_e_mail(e_mail_address);
	e_mail_address = std::nullopt;
	display_e_mail(e_mail_address);
}
 Not: Value fonksiyonu referans dönerken value_or referans dönmüyor.
*/

/*
*  operator* fonksiyonu reference qualifierlerdan faydalanarak implementasyonda & ve && overloadlara sahip. Bu sebeple aşağıda
*  taken'a yapılan atamada taken opt2 nesnesinin tuttugu stringin kaynağını çaldı. Özet olarak şöyle anlatabiliriz. operator* fonksiyonu 
   reference qualifierlar ile hem & hemde && overloadlarına sahip. move fonksiyonu X value yani R value döndürdüğü için ve bu döndürdüğü
   deper operator* fonksiyonuna arguman olarak gönderildiği için && overload çağrıldı ve bu overload da yine aynı sekilde R value döndürür.
   Bu yüzde taken string sınıfı türünden bir nesne oldu ve move ctor ile hayata kaynakları çalarak geldi.
*
#include <optional>
#include <string>
#include <iostream>

using namespace std;

int main()
{
	optional<int> opt1 = 1;
	*opt1 = 2;
	optional<string> opt2 = "abc"s;

	auto taken = *move(opt2);
}

*/

/*
*  Optional nesnesi semantic olarak C++ 17 ile hayatımıza giren if with initializer ile de aşağıdaki gibi çok sık 
*  kullanılıyor. 
*  
*  Not: Necati hoca if with initializer ile ilgili bilgilendirmeler yaptı. Tıpkı for deyiminde olduğu gibi artık 
*  if deyiminde de bir variable tanımlayabiliyoruz ve bu variablenin scopu if parantezinin içi oluyor. Bu syntax modern C++ ile
*  hayatımıza girmiştir ve bunun amacı scope leakagedir. 


#include <optional>
#include <string>
#include <iostream>

using namespace std;

optional<string> get_middle_name(const string& rname)
{
	return "alican";
}

int main()
{
	string str;
	//...

	if (auto ostr = get_middle_name(str); ostr) {
		cout << *ostr;
	}
	else {
		cout << "no middle name";
	}
}

*/

/* 
*  Not: new placement operatoru bizim istediğimiz bellek alanında ilgili nesneyi hayata getirir. New operatorune bir adres geçilir, 
*  daha sonra geçilen adresde ilgili türden nesne hayata gelir ve ctoru çağrılır. STL de çok fazla new placement operatoru kullanılmaktadır.
*  Örneğin vectorun push_back fonksiyonun düşün. Size ve capacity i tutar ve eklenecek nesnenin sona ekleneceğini bilir. O yerde
*  new placement operatoru ile ilgili nesneyi ctor eder.
*/

/*
#include <optional>

template<typename Con, typename Pred>
auto find_if(Con&& c, Pred&& pred)
{
	using std::begin, std::end;
	auto beg_iter = begin(c), end_iter = end(c);
	auto result = std::find_if(beg_iter, end_iter, pred);
	using iterator = decltype(result);

	if (result == end_iter)
		return std::optional<iterator>();

	return std::optional<iterator>(result);
}

template<typename Con, typename T>
auto find(Con&& c, const T& tval)
{
	return find_if(std::forward<Con>(c),
		[&tval](auto&& x) {return x == tval; });
}

*/

/*
*  Optional sınıfında referans tutmak için reference_wrapper sınıfından faydalanabiliriz.Necati hoca reference_wrapper 
*  sınıf sablonu hakkında da bilgiler verdi.Bu sablonun amacı referansları sarmalamaktır ve referans ile yapamadığımız şeyleri yapabilme
*  yi sağlamaktır ki bunlar örneğin container da referans tutumak, optional da referans tutmak, Reference_wrapper aslında
*  implementasyonda bir pointer tutmaktadır ve en kilit nokta içerisinde ki typecast operator member fonksiyondur.Yani içerisinde
*  operator T& fonksiyonu var ve bu fonksiyon aslında reference_wrapper da tutulan pointerı derefrence ederek return ediyor. Hayal
*  edebilirsin ki gayet aslında mantıklı ve complex olmayan bir impl. Possible impl asağida

template <class T>
class reference_wrapper {
	// types
	typedef T type;
public:

	// construct/copy/destroy
	reference_wrapper(T& ref) noexcept : _ptr(std::addressof(ref)) {}
	reference_wrapper(T&&) = delete;
	reference_wrapper(const reference_wrapper&) noexcept = default;

	// assignment
	reference_wrapper& operator=(const reference_wrapper& x) noexcept = default;

	// access
	operator T& () const noexcept { return *_ptr; }
	T& get() const noexcept { return *_ptr; }
}

#include <optional>
#include <iostream>

int main()
{
	std::string name{ "hasan" };
	std::optional op = ref(name); // CTAD dan faydalanıldı.
	op->get() += "can";     // reference_wrapperin get fonksiyonu var, string de operator+= fonksiyonu var. Bunlardan faydalaniliyor.
	std::cout << "name = " << name << '\n';
}

*/

/*
*  Kalan bölümde optional nesnesinin karşılaştırma operator fonksiyonlarından bahsedildi. Bunlar birer sınıf sablonudur, dolayısıyla
*  aynı türden nesneleri tutmayan optional nesnelerinide aslında karşılaştırabilirim, ayrıca nullopt ile de karşılaştırabilirim ki
*  bu zaten optional dolu mu boş mu diye anlamanın en common yollarından biridir. Burada en önemli soru şu olabilir. Boş bir optional
*  nesnesi ile dolu bir optional nesnesi karşılaştırılabilir mi? Tabi ki evet ve burda genel kural şudur: Boş optional nesnesi
*  her zaman dolu optional nesnesinden küçüktür!
*  
*  Bu konu ile alakalı yazılan kodlar aşağıdadır.


#include <optional>
#include <iostream>

int main()
{
	using std::optional, std::cout, std::nullopt;

	optional<int> oe;
	optional<int> ox{ 10 };
	optional<int> oy{ 20 };

	cout.setf(std::ios::boolalpha);

	cout << (oe == ox) << '\n'; //false
	cout << (oe == nullopt) << '\n'; //true
	cout << (oe < ox) << '\n'; //true
	cout << (ox > oy) << '\n'; //false
	cout << (ox == 10) << '\n'; //true

	optional<unsigned> oz;
	optional<unsigned> omin{ 0 };
	cout << (oz < omin) << '\n'; //true
}

#include <optional>
#include <iostream>

int main()
{
	using std::optional, std::nullopt, std::cout;
	optional<bool> oe{ nullopt };

	optional ox{ false };
	optional oy{ true };

	boolalpha(cout);

	cout << (oe == ox) << '\n';  //false
	cout << (oe == oy) << '\n';  //false
	cout << (oe < ox) << '\n';  //true
	cout << (oe < oy) << '\n';  //true
	cout << (oe == true) << '\n';  //false
	cout << (oe == false) << '\n';  //false
	cout << (ox == oy) << '\n';  //false
	cout << (ox < oy) << '\n';  //true
}

#include <optional>
#include <iostream>

int main()
{
	std::optional<int> op1;  //empty 
	std::optional op2{ 2 };
	std::optional op3{ 3 };

	boolalpha(std::cout);
	std::cout << (op1 > op2) << '\n';
	std::cout << (op2 < op3) << '\n';
	std::cout << (op1 < op2) << '\n';
	std::cout << (op1 == std::nullopt) << '\n';
	std::cout << (op3 == 3) << '\n';
}

*/

/*
*  C++ 20 ile eklenen ve monadic işlemlerde kullanılan transform, and_then ve or_else fonksiyonları anladıltı dersin son bölümünde. 
*  Ortalık biraz karısti ve cok anlayamadığım için not almadım. Necati hoca 10. derste en baştan anlattı ve orada not aldım.
*/

/*
*  Not: Necati hoca fonksiyonal programlama ile ilgili bir dili orta düzeyde öğrenmek mesela python gibi iyi olabilir gibi bir
*  öneride bulundu.
*/