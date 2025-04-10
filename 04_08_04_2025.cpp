/*
*  4. Ders günü için anlatılanlar ve yazılan bazı kodlar şöyledir
*/

/*
*   Son derste exception handling'den bahsetmiştik. Ayrıca exception handlingde strong exception garanti
*   basic exception garanti, no throw garanti, no exception garanti konularına da demiştik. Vector sınıf sablonunun 
*   push_back fonksiyonu da strong exception garanti vermiştir. Yani push_back çağrısından dolayı bir exception throw meydana
*   gelirse bunun çalışan process'e bir yan etkisi olmayacaktır, herhangi bir kaynak ve kapsam sızıntısı olmayacaktır,
*   ilgili nesneler push_back fonksiyonundan önceki state'ni ve değerini korumaya devam edecektir. Peki bu durum move ctor'un noexcept
*   olup olması ile ne derece ilgilidir. 
* 
*   Şimdi şöyle düşünelim. Myclass sınıfın move ctor'unun user defined olarak implemente edildi ve noexcept specifierı ile tanımlanmadı. 
*   Yani biz bu fonksiyonun exception throw etmeyeceği garantisini vermiyoruz. Daha sonra vector nesnesinin Myclass
*   açılımı türünden bir nesne oluşturduk. Daha sonra Myclass türünden bir nesneyi push_back fonksiyonu ile
*   ilgili vector containerı'na insert edecek kodu yazıyoruz. Fakat vector sınıfında size capacity değerini geçtiği için bir
*   reallocation olacaktır. İşte bu durumda ilgili Myclass türünden nesneler taşınacak mı yoksa
*   kopyalanacak mı? Kopyalanacak
* 
*   Peki neden burada taşıma yerine bir kopyalama yapıldı. Sebebi direk olarak push_back fonksiyonunun strong exception garanti
*   vermesidir. Biz move ctor'u noexcept specifierı ile tanımlamadığımız için move ctor exception throw edebilir derleyici
*   bakış açısı ile. Dolayısı ile böyle bir taşıma durumunda exception throw meydana gelirse vectorde yeni allocate edilen yere
    taşınacak olan nesnelerin push_back çağrısından dolayı taşınması durumunda bir problem meydana gelirse vector nesnesi
	valid state durumunu koruyamayacak, kaynakları çalındı veya biraz çalındı:). Dolayısı ile push_back fonksiyonuna 
	bir Myclass nesnesini göndersek ve bir reallocation meydana gelirse burada copy ctor çağrılacaktır. Dolayısı ile kendimiz özel üye
*   fonksiyonlarını yazarsak burada move ctor'u noexcept yapmak daha özel bir sebep yoksa daha doğru olacaktır.
*   
*   
*/

/*
#include <iostream>
#include <vector>

using namespace std;

class Myclass {
public:
	Myclass() = default;
	Myclass(const Myclass&)
		{
		cout << "copy ctor ";
		}
	Myclass& operator=(const Myclass&) = default;
	Myclass(Myclass&&)
	{
		cout << "move ctor ";
	}

	Myclass& operator=(Myclass&&)
	{
		cout << "move assigment";
	}
};

*/

/*

int main()
{
	vector<Myclass> vec(3); // vector de 3 tane default ctor edilmiş Myclass nesnesi tutuluyor. size ve capacity eşit.
	Myclass temp;

	vec.push_back(move(temp)); // Yeni bir reallocation gerekecek ve vector de ki bütün nesneler move noexcept
						      // olmadığı için kopyalanacak.

}

*/

/* Bazı notlar:
*  1. Destructor default olarak noexcept bir special member fonksiyondur.
*  2. Chrono std içerisinde ayrı bir nested namespace içerisinde bulunmaktadır.
*  3. Swap fonksiyonları genelde noexcept olur. Aşağıda genel bir swap fonksiyon template'i gösterilmiştir.
*  4. SINFILARIN ÖZEL ÜYE FONKSİYONLARI EĞER DERLEYİCİ TARAFINDAN DEFAULT EDİİYORSA BUNLARIN NOEXCEPT OLUP OLMADIĞI DERLEYİCİ TARAFINDAN BELİRLENİYOR
*	DESTRUCTOR DEFAULT OLARAK NOEXCEPT
*/

/*
#include <iostream>

template<typename T>
void my_swap(T& s1, T& s2)noexcept
{
	T temp = std::move(s2);
	s2 = std::move(s1);
	s1 = std::move(T);
}

*/

/*
*  Aşağıda ki örnek derste yazıldı. Yukarıda move ctorun noexcept olmadığı durumda vectorun
*  push_back fonksiyonu çağrıldığında size ve capacity birbirine esit oldugu için 
*  vector nesnesi için bir reallocation gerekir ve copy ctor lar çağrılır.
*/

/*

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class berat : public exception {
public:
	const char* what()const noexcept override{
		return "Berat";
	}
};

class Student {
public:
	Student(const char* p) : s{p} {}
	string get_str()const
	{
		return s;
	}

	Student(const Student& other) : s{ other.s } { cout << "COPY " << s << '\n'; }
	Student(Student&& other)  : s{ move(other.s) } { throw(berat{}); cout << "MOVE " << s << '\n'; }

	
private:
	string s;
};

int main()
{
	vector<Student> vec{ "Hasan Kaynar", "Utku Altın", "Volkan Sarı" };

	cout << "capacity: " << vec.capacity() << '\n';

	try {
		vec.push_back(Student{ "Kutay Kök" });
	}
	catch (exception& x) {
		cout << x.what() << endl;
		for (auto& c : vec) {
			cout << c.get_str() << '\n';
		}
	}

	

}

*/

/*
*  Dersin kalan kısmında copy ellision konusu ele alında. C++ 17 standartı ile copy ellision ile ilgii bazı garantiler
*  verildi. C++ 17 öncesinde yapılmak istense bile eğer sınıfın copy ctor ve move ctor ları çağrılacak gibi değil ise 
*  syntax hatasına neden oluyordu. Fakat C++ 17 ile birlikte bazı durumlarda copy ellision standartlar tarafından garanti
*  altına alınmıştır. 
*  
*  Aslında copy elision temporary materialization ile ilgilidir. Bir geçici nesnenin constructorunun çağrılması için 
*  bir nesneye yada referansa bağlanması gerekir. Şimdi copy ellision olabilecek durumlara bakalim. 
*/

/*  1.	Bir fonksiyonun parametresi bir sınıf türünden ve sınıf bir geçici nesne ile çağrılıyorsa.
*/

/*
#include <iostream>

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

void func(Myclass x)
{
	cout << "func cagrildi" << '\n';
}

int main()
{
	func(Myclass{});
}

*/

/*
*   2.   Bir fonksiyonun geri dönüş değeri bir sınıf ve return ifadesi geçici nesne ise
*/

/*
Myclass foo()
{
	return Myclass{};
}

int main()
{
	Myclass m = foo();
}

*/


/*
*  3. Diğer durum ise NRVO olarak geçmektedir; açılımı named return value optimizationdur. Bu durum şöyledir. Sınıf 
*     döndüren bir fonksiyonun tanımında yani local alanda bir sınıf nesnesi oluşturulmuş ve return ifadesi ile bu sınıf nesnesi 
*     dönüyorsa derleyici copy elision yapar.
*/

/*
Myclass foo()
{
	Myclass data;
	return data;
}

int main()
{
	Myclass m = foo();
}

*/

/*
*   C++ 17 ile copy elision optimizasyonu 1. ve 2. senaryolar için garanti altına alınmıştır. Artık 1. ve 2. senaryolar
*   için mandatory copy elision ifadesi kullanılmaya başlanmıştır. Fakat aslında bu yanlış bir ifadedir. Çünkü ortada bir
*   kopyalama yoktur. Bu senaryolarda copy elisionun standartlar tarafından garantiye alınmasıyla ilgili sınıfların
*   copy ve move ctor ları olmasa bils syntax hatası olmayacaktır. Ayrıca bu durum ile fabrika fonksiyonları maliyetsiz olmuştur. 
*   Örneğin make_unique fonksiyonunun geri dönüş değeri unique_ptr nin herhangi bir tür açılımı ve biz bu geri dönüşü bir static
*   veya global unique_ptr nesnesine atasak bile move ctor çağrılmayacak. Copy ellision mekanizması ile maliyetsiz şekilde sadece
*   ctor çağrılacak.
*  
*   Yukarıdaki ikinci örnek Return Value Opt(RVO), 3. Örnek Named Return Value Opt(NRVO) dur. RVO standartlar tarafından 
*   garanti altına alınmasına rağmen NRVO standart değildir, compiler optmizasyonudur.
*   
*/

/*
struct Myclass {
	Myclass(int, int){}
	void set(int) {}
	void dosomething(){}

};

Myclass foo()
{
	Myclass m{ 212,46 };
	m.set(2);
	m.dosomething();
	//...
	return m;
}

Myclass x = foo(); // burada ilgili x nesnesi için copy ellision olacak mı? olacaksa member nesneler hangi değerde olacak. Anlamadim?
*/

/*
*   Bir fonksiyon içinde iki farklı return ifadesi var ve bu iki ifade de aynı sınıf türünden farklı nesneler return ediliyorsa
    burada NRVO olmayacaktır. Ayrıca NRVO olabilecek bir senaryoda bile ilgili sınıvın move ve copy ctor ları delete yada undefined ise
	syntax hatası olacaktır. Fakat C++ 17 de RVO nun standart olmasıyla bu sorun RVO için ortadan kalkmıstır.
*/

/*  Pessimistic move denen de bir olay vardır, kötümser move. Öyle bir senaryo var ki NVRO yu engelleyen hiçbir şey yok
*   fakat acemi programcı sanki faydası olacakmış gibi return ifadesinde move fonksiyonunu çağırıyor. Dolayısı ile burada NRVO da olacak
*   copy ellision sayesinde hiçbir taşıma kopyalama olmayacakken bir taşıma maliyeti ortaya cıkacaktır. Örneği aşağıda verilmiştir.
* 
* A foo(){
*	A ax;
*   //....
*   return std::move(ax);
* }
* 
* int main(){
*	A ax = foo();
  }
*/

/*
*   Necati hoca daha sonra move from state durumundan bahsetti ve burada faydalı örnekler verdi.Hayatta olan nesnenin kaynakları
*   çalınırsa ve nesne hala hayatta ise bu nesneler move from state'de olur. Move state'de olan nesneler hala hayatta olduklarından
*   dolayı bu nesnelere tekrardan atama yapıp kullanabiliriz. Örneğin hayattaki bir nesneyi move fonksiyonuna arguman olarak gönderip 
*   daha sonra geri dönüş değerini bir nesneyi hayata getirirken kullanırsak ilgili hayattaki nesnenin kaynağını çalabiliriz.
*   Fakat bu nesne hala valid durumdadır, yani tekrardan bir nesne ataması yapılıp kullanılabilir, tabi bu ilgili sınıfın
    implementasyonuna bağlıdır. Örneğin STL dışında third party bir kütüphanede bulunan bir sınıf türünden nesneler oluşturcaksak
	bu tür için move from state de olacak nesneler valid durumda mı değil mi bunu ilgili kütüphane dökümante etmeli.
	Peki neden böyle birşey yapalım? Bununla ilgili Necati hoca aşağıdaki faydalı örneği yazdı.


*/

/*

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

int main()
{
	ifstream ifs{ "falanca.txt" };
	if (!ifs) {
		cerr << "dosya acilamadi\n";
		return 0;
	}

	vector<string> lines;
	lines.reserve(1000);
	string sline;
	while (getline(ifs, sline)) {
		lines.push_back(move(sline)); //işte burada sline hala hayatta olmasına rağmen kaynağı çalınıyor. Fakat move from state
												// durumunda ki nesneler valid durumda olduğundan tekrardan bir nesne ataması yapılıp 
												// loop içinde kullanılıyor.
	}

	return 0;

}

*/

/*  Dersin bir bölümünde small string optimizasyonundan bahsedildi. Bende aldığım ve bildiklerimden aşağıdaki notu yazdım.
*   String implementasyonlarda small string optimization denen bir optimizasyon tekniği kullanıyorlar. String nesnesi içerisinde
*	yazıyı gösteren pointerları tutmakla beraber birde dizi tutuyor. Nedeni şu; yazıların çoğunluğu küçük yazılar oluyor. 
*	Bu sebeple bu yazılar için tekrardan dinamik memory allocation yapmamak için bu küçük yazıları bu string nesnesi içerisindeki
*	dizide tutuyor. İşte dinamik bellek yönetimi olmadan small string optimizationu ile yazı işlemlerini bu sınıf yapabilir. 
*	Yani yazı yeterince küçük olduğunda stackde yada data kısmında(string nesnesi otomatik ömürlü ise stack, static ömürlü ise data) 
*	bir dizi aracılığıyla tutulurken, büyük olması durumunda heap de tutulur.
*/

/*
*  Dersin sonunda aşağıdaki kod yazıldı ve remove_if fonksiyonunda arka planda bir taşıma işlemi yapıldığı söylendi.
*  Ayrıca remove_if fonksiyonunun lojik silme işlemi yaptığını ve containerdan gerçekten birşey silmek istersek
*  erase remove idiomunu kullanmamız gerektiğini hatırlatalım.
*/

/*
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class StrWrapper {
public:
	StrWrapper() = default;
	StrWrapper(const string& s) : m_str{s} {}
	StrWrapper(const StrWrapper&) = default;
	StrWrapper& operator=(const StrWrapper&) = default;
	
	StrWrapper(StrWrapper&& other) noexcept: m_str{ move(other.m_str) } {
		cout << "move ctor\n";
	}

	StrWrapper& operator=(StrWrapper&& other) noexcept {
		cout << "move assigment\n";

		if(this != &other)
			m_str = move(other.m_str);

		return *this;

	}

	size_t length()const {
		return m_str.length();
	}

private:
	string m_str{};
};


int main()
{
	vector<StrWrapper> vec;
	vec.reserve(20);
	vec.emplace_back("kemal");
	vec.emplace_back("deniz");
	vec.emplace_back("murat");
	vec.emplace_back("hasan");
	vec.emplace_back("can");
	vec.emplace_back("mert");

	auto iter = remove_if(vec.begin(), vec.end(), [](const StrWrapper& s)
		{ return s.length() == 5; });

	for (auto& r : vec) {
		cout << r.length() << '\n';
	}
}

*/