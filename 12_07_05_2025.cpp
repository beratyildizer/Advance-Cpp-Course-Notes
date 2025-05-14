/*
*  Any sınıfından devam edildi. Aşağıdaki örnekte heterojen bir vector nesnesi oluşturmuş olduk. Örnekte vectorde tutulan any nesnelerinin
*  hangi türden bir nesne tuttukları any_cast fonksiyonu ile anlaşılmıştır. Fakat istersek type fonksiyonunu da kullanabilirdik. 
*  Type fonksiyonuda type_info türü geri döndürüyordu ve bu sınıfında interfacesini kullanabilirdik any de tutulan türleri anlamak
*  için.

#include <any>
#include <vector>
#include <iostream>

using namespace std;

int main()
{
	vector<any> vec{ 2, 3.4, "ali", 'X' };

	for (const auto& a : vec) {
		if (auto ip = any_cast<int>(&a)) {
			cout << *ip;
		}
		else if (auto dp = any_cast<double>(&a)) {
			cout << *dp;
		}
		else if (auto cp = any_cast<const char*>(&a)) {
			cout << *cp;
		}
		else if (auto p = any_cast<char>(&a)) {
			cout << *p;
		}
	}

}

*/

/*
*   Aşağıdaki kodda derste yazıldı ve bizde böyle kodlar yazıp parser işlemleri yapabiliriz. Örneğin aşağıda iki 3 farklı int tutan 
*   any nesnesi olacak fakat biz hangi int'in ne olduğunu pair'in first'i ile anlayabiliyoruz. Ayrıca for döngüsü içerisinde de
*   structural binding kullandık.Structural binding C++ 17 ile hayatımıza giren ve birden fazla değişkene tek bir line ile değer
*   verebilme anlamına geliyor. Bunu yaparken değişkenlere atanacak değer bir struct, class veya dizi olabilir. Aşağıda structural
*   binding işleminde property pair'in first'üne (string) referans göstericekken, value pair'in second'ına (any) referans gösterecek.

#include <vector>
#include <any>
#include <string>
#include <iostream>
#include <iomanip>

using tv_pair = std::pair<std::string, std::any>;

int main()
{
    using namespace std::literals;

    std::vector<tv_pair> vec;

    vec.emplace_back("name", "ahmet aksoy"s);
    vec.emplace_back("year", 1998);
    vec.emplace_back("month", 11);
    vec.emplace_back("month day", 22);
    vec.emplace_back("wage", 87.67);
    vec.emplace_back("town", "eskisehir"s);
    vec.emplace_back("gender", "male"s);
    vec.emplace_back("country", "Turkey"s);
    //

    std::cout << std::left;
    for (const auto& [property, value] : vec) { // structural binding
        if (value.type() == typeid(int))
            std::cout << std::setw(16) << property << any_cast<int>(value) << '\n';
        else if (value.type() == typeid(double))
            std::cout << std::setw(16) << property << any_cast<double>(value) << '\n';
        else if (value.type() == typeid(std::string))
            std::cout << std::setw(16) << property << any_cast<std::string>(value) << '\n';
    }
}

*/

/*
*  Bir any nesnesi içerisinde doğrudan bir dizi tutma şansım yok, daima array decay oluyor ve ben dizinin başlangıç adresini gösteren
*  bir pointer tutuyorum. make_any fabriak fonksiyonunu da kullansam bu durum değişmiyor. Aşağıdaki örnek bu durumu göstermek için
*  derste yazılmıştır.

#include <any>
#include <iostream>

int main()
{
	int ar[] = { 1, 2, 3, 4, 5 };

	std::any a = ar;

	std::cout << "sizeof(a) = " << sizeof(a) << '\n';


	if (a.type() == typeid(int[])) {
		std::cout << "stores an array\n";
	}
	else if (a.type() == typeid(int*)) {
		std::cout << "stores a pointer\n";
	}

	std::cout << a.type().name() << '\n';
}

*/

/*
*  İlk iki vocabulary type olan optional ve variant türlerinin implementasyonlarında standartlar dynamic allocation'u yasaklamışlardır.
*  Yani standartlar hangi STL ögesinin ne şekilde implemente edileceğini söylemesede optional ve variant impl'sinde bir restrict 
*  getirmiştir. Bu sebeple variant ve optional da herşey static. Fakat any için böyle bir restrict yoktur. Any arkada static bir buffer
*  tutar, bu bufferın boyutu derleyiciden derleyiciye değişir. Eğer any de tutulacak nesnenin boyutu bufferın size nı aşarsa işte
*  burada bir reallocation yapar. Yani şöyle de diyebiliriz. Any impl'sinde arka tarafta bir small string optimazyonu vardır, tıpkı
*  stringlerde olduğu gibi. Aşağıdaki örnek bunun için derste yazılmıştır. Any nesnesi Data türünden bir nesne tutana kadar bir
*  reallocaiton yapmıyor, çünkü içerde tuttuğu buffer asaşğıdaki string ve int nesneler için yeterli boyutta benim derleyicimde. 
*  Fakat data nesnesi any'de kopyalama ile tutulmaya basladığında bir reallocaton olacaktır.
*  

#include <iostream>
#include <cstdlib>
#include <any>
#include <string>

void* operator new(size_t n)
{
	std::cout << "operator new called n: " << n << '\n';
	auto vp = std::malloc(n);
	if (!vp)
		throw std::bad_alloc{};
	return vp;
}

void operator delete(void* vp)
{
	if (!vp)
		free(vp);
}

struct Data {
	char buffer[2000];
};

int main()
{
	std::cout << "sizeof(any)        = " << sizeof(std::any) << '\n';

	std::any ax = 10;
	ax = std::string{ "alican" };
	Data data;
	ax = data;
}

*/

/*
*  Any_cast'in geri dönüş türünü kalıtımın olduğu bir sınıf hiyerarşisinde is a ilişkisi varmış gibi kullanamam. Yani örneği
*  türemiş sınıf türünde bir nesne tutan any nesnesini any_cast fonksiyonuna arguman vererek çağırıp template argumanına base 
*  class'ın türünü yazarsam burada any_cast nullptr döndürecektir. Bunun ilgili derste yazılan örnek aşağıdadır.


#include <iostream>
#include <any>

struct Base { };
struct Der : Base { };

int main()
{
	std::any a = Der();
	if (Base* base_ptr = std::any_cast<Base>(&a)) {
		std::cout << "correct";
	}
	else {
		std::cout << "incorrect";
	}
}

*/

/*
*  Dersin kalan bölümünde Lamba Expression'lara geçildi. İlk olarak Necati hoca Lambdalar ile ilgili giriş yapacağını daha sonra 
*  ileri Lambda konularını ve idiomlarını anlatacağını söyledi. Lambda expressionların value kategorisi PR value'dir. Adında da
*  anlaşılacağı gibi bir expressiondur. Lambda ifadesinin türü de derleyicinin oluşturduğu sınıf türü yani closure type'dir. 
*  Closure type standartlar da geçen ifadedir. O türden bir nesne oluşturursak da buna closure object deniyor. Derleyici oluşturduğu
*  closure type'a operator() fonksiyonu yazıyor, fonksiyonun geri dönüş türü auto deduction ile belirleniyor.
*  
*  Aşağıda main içerisinde bulunan lambda expression için derleyinin yazacağı closure type cgt_7234 ile örneklenmiştir. Derleyici
*  buna benzer birşey yazmaktadır, yazdığı fonsiyon const'tur.
*  
*  Not: Derste IIFE'den kısaca bahsedildi, burada ki yazı burayı anlatıyor. https://www.cppstories.com/2016/11/iife-for-complex-initialization/
* 

class cgt_7234 {
public:
	auto operator()(int x)const {
		return x * x;
	}
};

int main()
{
	[](int x) {return x * x; };
}

*/

/*
*  Lambda ifadeleri STL algoritmaları ile çok fazla kullanılıyor. Necati hocanın derste bunun ile ilgili yazdığı kodlar ve 
*  açıklamaları aşağıdadır. 
*/

/*
*  İlk olarak bir vector nesnesi sort algortiması ile sıralanmıştır. Sort algoritması bir calleable alır ve bu calleable'a
*  vector range'in de bulunan nesneleri arguman olarak gönderir. Bu calleable'ın geri dönüşüne göre sıralama yapar. Burada 
*  lambda expression'u calleable vermek yerine sort fonksiyonuna fonksiyon adresi, function object vs de verebilirdik.
*  
*  Daha sonra for_each algoritmasının 3. argumanına calleable olarak bir lambda ifadesi geçilmiştir. For_each fonksiyonu da
*  rangede ki her ögeyi üçüncü argumandaki fonksiyona arguman olarak gönderir.

#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

int main()
{
	std::vector vec{ 7, 1, -9, 3, 6, -2, 8, -4, 5 };
	sort(vec.begin(), vec.end(), [](int a, int b) {return std::abs(a) < std::abs(b); });

	for_each(vec.begin(), vec.end(), [](int i) {std::cout << i << ' '; });
}

*/

/*
*  Aşağıda ki kodda vector de string nesneleri tutulmaktadır. copy_if algoritmasını çağırarak standart input dan karakter sayısı(n) ve
*  karakter'in kendisini (c) alıyoruz. Daha sonra c karakterinden n tane içeren string nesnelerin standar çıkış akımına 
*  yazdırılıyor. Standart çıkış akımına yazdırmak için ostream_iterator kullanılmıştır. Yazma algoritmalarında yani bir range
*  alıp ilgili range aralığına data yazan algoritmalarda(copy gibi) range yerine bir dosyaya veya ekrana yasmak istersek bu
*  ostream_iterator'u kullanabiliriz.

#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

int main()
{
	vector<string> svec{"ali", "veli", "ahmet", "hasan", "utku", "volkan", "ceyhun"};

	int n;
	char c;

	cin >> c >> n;

	copy_if(svec.begin(), svec.end(), ostream_iterator<string>(cout, "\n"), [c, n](const string& s) {
		return count(s.begin(), s.end(), c) == n;
		});

}

*/

/*
*  Lambda ifadeler type template argumanı bekleyen yerlerde de çok kullanılıyor. Aşağıda bununla ilgili bir örnek verilmiştir.
*  Set containerına sıralama callebla olarak lambda ifadesinin türü geçilmiştir type parametre olarak.

auto comp = [](int lhs, int rhs) {
	return abs(lhs) < abs(rhs);
};

set<int, decltype(comp)> myset;

*/

/*
*  Derste sorulan soru: Hocam lambda ifadesini using bildirimiyle tanımlayabiliyor muyduk?
*  Cevap: Evet, aşağıdaki 2 örnek bu soruya karşılık yazıldı.

int main()
{
	auto f = [](int x) {return x * 3; };

	using call_t = decltype(f);
}



*  Ayrıca C++ 20 ile lambda ifadelerini unevaluated context'de kullanabiliriz ve yukarıdaki kodu aşağıdaki gibi de yazabiliriz.


int main()
{
	using callable = decltype([](int x) {return x * 5; });
}

*/

/*
*  Bir lambda expression'un genel tanımı aşağıdaki gibidir. Köşeli paranteze lambda capture deniyor ve bu lambda capture içerisine
*  =, & vs gibi şeyler yazılabiliyor, detaylarına bakacağız. Normal parantezin içerisine operator() fonksiyonunun argumanlarını yazıyoruz.
*  Süslü parantez içerisine de operator() fonksiyonunun kodunu yazıyoruz. Normal parantezle süslü parantez arasına yazılan
*  anahtar kelimelerin anlamlarını ise ilerleye sürelerde görücez.
* 
*			 constexpr
*			 noexcept
*  			 mutable
*  [](int x) -> double { //code}
*  lambda
*  introducer.
*/

/*
*  C++ ı yeterince bilmeyen kişiler ilk görüşte sasırabiliyor ama aşağıdaki kodların hepsi syntax dahilindedir.
int main(void)
{
	([](){{}})();
	[](){{}}();
	[](){}();
	[]{}();
}

*/

/*
*  Her özdeş lambda için derleyici farklı bir closure type yazmaktadır. Dolayısıyla aşağıdaki kodda ekrana false yazacaktır.

#include <type_traits>
#include <iostream>

int main()
{
	auto f1 = [](int i) {return i * 5; };
	auto f2 = [](int i) {return i * 5; };

	auto b1 = (f1 == f2); //false - function pointers not equal
	boolalpha(std::cout);
	std::cout << "b1 = " << b1 << '\n';
	constexpr auto b2 = std::is_same_v<decltype(f1), decltype(f2)>; //false - different types
	std::cout << "b2 = " << b2 << '\n';

}

*/

/*
*  Lambda fonksiyonları default template arguman alabilirler. C++ 14 ile bu özellik dile eklendi. Lambdalar ile ilgili
*  her C++ standartında bunun gibi farklı özellikler eklenmeye devam etmiştir. Aşağdaıki kod bu konuya örnek olarak yazılmıştır.

#include <iostream>

int main()
{
	auto f = [](int x = 0) {std::cout << '(' << x << ")\n"; };

	f(10);
	f();
}
*
*/

/*
*  Bir lambda ifadesinde süslü parantez içerisine yazılan kodun aslında operator() fonksiyonunun içerisine yazılan kod
*  olduğunu söylemiştik. Bu kod içerisinde static ömürlü global nesneler visible durumdadır ve bu nesnelerin hepsi
*  kullanılabilir. Fakat lambda ifadesi ile aynı scope de bildirilen otomatik ömürlü nesneler görünür durumda değildir. Bu 
*  sebepler bu nesneleri direk olarak bir lambda ifadesi içerisinde kullanırsam name lookup hatası alacağım. Eğer bu otomatik
*  ömürlü nesneleri lambda operator() fonksiyonunun içerisinde kullanmak istersem bunun yolu bu otomatik ömürlü
*  nesneleri ya copy capture yapacağım, yada copy referans. 
* 
*  Aşağıdaki kodda ilk olarak copy capture örneği verilmiştir. Aşağıdaki kodda bir otomatik ömürlü nesnenin ismini
*  lambda capture ismine direk bu şekilde yazarsak derleyici yazacağı closure type içerisinde bu nesne türünden bir member
*  nesne yaratacak ve bu nesneyi x ile initialize edecek, yani kopyalayacak. Bu yüzden bu yönteme copy capture denmiştir. Daha
*  sonra bu x ismini biz fonksiyon bloğunda kullanabiliriz.

#include <iostream>

int main()
{
	int x = 10;
	auto f = [x](int i) {return x * i; };

}


*/

/*
*  Lambda ifadesi karşılığı derleyicinin yazdığı closure type'in operator() fonksiyonu const bir fonksiyondur. Eğer bu fonksiyonu
*  non const yapmak istiyorsak normal parantezle süslü parantez arasında mutable anahtar sözcüğünü kullanmalıyız. Mutable
*  sınıfının bir diğer kullanımıda sınıfların member üyeleri için vardı. Orada amaç const fonksiyonların o nesneyi değiştirebilmesiydi.
*  Aşağıdaki kod bunu göstermek için yazılmıştır.


#include <iostream>

int main()
{
	int x = 5;
	auto f = [x](int i) mutable{return x++; };
	//Yukarıda ++x ifadesi syntax hatası olmadı, cünkü lambda fonksiyonu artık const değil. 

}

*/

/*
*  Otomatik ömürlü bir nesneyi lambda kodu içerisinde kullanmanın diğer bir yoluda referans capture oldugunu söylemiştik. 
*  Reference capture'i kullanmak için & operatorunu capture edilecek nesnenin ismiyle beraber lambda introducer köşeli
*  parantezi içerisinde kullanmalıyız.
* 
*  Aşağıdaki kod capture referansa bir örnektir. Sınıfın string referans türünden bir member nesnesi olacak ve bu nesne
*  otomatik ömürlü name nesnesini gösterecek. Dikkat edersek lambda fonksiyon const olmasına rağmen name += p ifadesi syntax
*  hatası olmadı. Çünkü biz sınıfın string türünden member nesnesi name'yi değiştirmiyoruz, main içerisinde tanımlanan name'i
*  değiştiriyoruz. Yani aslında assembly düzeyinde member pointer nesnesinin değerini değiştirmiyoruz, pointerın gösterdiği
*  nesneyi değiştiriyoruz. Bu sebeple bir syntax hatası olmadı ve murat + *p ekrana yazılacak fonksiyon çağrılırsa. Fakat aşağıda
*  fonksiyon çağrılmamış:)

#include <iostream>
#include <string>

int main()
{
	string name{ "murat" };

	auto f = [&name](const char* p) {
		name += p;
	};

	cout << name;

}

*/

/* 
*  Eğer biz lambda introducer içerisine sadece eşittir operatorunu (=) yazarsak lambda operator() fonksiyon bloğunda kullanılan
*  bütün otomatik ömürlü nesneler capture edilecektir. Bununla ilgili örnek kod aşağıdadır ve aşağıdaki kodda a,b,c nesneleri
*  tek tek capture copy ile kopyalanacaktır. 

int main()
{
	int a = 3, b = 5, c = 7;
	string name{ "murat" };

	auto f = [=]() {
		return a + b + c + 12;
	};

	cout << name;

}

*/

/*
*  Köşeli parantez içerisine & yazarsak da operator() fonksiyon bloğunda kullanılan bütün otomatik ömürlü nesneler referans
*  yolu ile capture edilecektir. 
*  
*  Ayrıca lambda introducer içerisine [=,&x] gibi birşeyde yazılabilir ve bunun anlamı şudur. Fonksiyon bloğunda kullanılan
*  bütün otomatik ömürlü nesneleri kopyala fakat x'i referans yolu ile capture et.
* 
*/

/*
*  Soru: referans yolu ile capture edilen nesnelerin ömrünü bitirip lambda ifadesinden 
*  bir nesnenin hala hayatta olduğu bir durum oluşturabilir miyiz hocam?
*  Cevap: Kesinlikle evet. Bu sebeple referans ile capture ederken çok dikkat etmeliyiz. Aşağıda bir örneği gösterilmiştir.

auto foo(int x)
{
	return [&x](int i) {return i * x}; //gecmis olsun, dangling referans olacak closure type içerisinde
}

int main()
{
	auto mylambda = foo(12); // gecmis olsun, dangling referans olacak closure type içerisinde
}

*/

/*
*  Soru: Bir nesne move yolu ile de capture edilebilir mi? 
*  Cevap: Evet. C++ 14 ile lambdalar için init capture özelliği eklendi. Amacı direk olarak bu olmasada
*  bunu yapmamıza da izin verdi.
* 
*  Aşağıdaki kod bu durumun bir örneği. uptr değerini eşitliğin sağındaki ifadeden aldı. Sağındaki ifadenin
*  türü x valu oldupu için string sınıfının move ctor'u çağrıldı.


#include <memory>
#include <iostream>
#include <string>

using namespace std;

int main()
{
	auto uptr = make_unique<string>("necati ergin");

	auto f = [uptr = move(uptr)](){cout << *uptr; };
}

*/

/*
*  Aşağıdaki soru mülakatlarda soruluyor. İlk fonksiyonda global g kullanılmış ve fonksiyon çağrıldığında 500 + 1 değerini dödürecek.
*  İkinci lambda da yani fy de g kopyalanmış ve kopyalandığı anda g nin değeri 99. dolayısıyla ekrana 501 ve 100 değerleri
*  yazdırılacak.

#include <iostream>

int g = 99;



int main()
{
	auto fx = [] {return g + 1; };
	auto fy = [g = g] {return g + 1; };

	g = 500;

	std::cout << fx() << '\n';
	std::cout << fy() << '\n';
}

*/