/*
* 53 Ders günü olacak. C++ Kursunu alanlar daha avantajlýdýr.
* 
* Bizim için bir ifadenin yani expressionun türü ve value kategorisi çok önemlidir. Peki expression nedir.
* An expression in C and C++ is any valid combination of operators, constants, functions and variables.
* 
* Örneðin: char x; // Burada x nesnesinin türü char'dýr fakat burada x için value category den bahsedemeyiz
* çünkü bu bir expression deðildir. Bir nesnenin tanýmýdýr. Value kategoriden bahsedebilmek için elimizde bir ifade yani 
* expression olmalýdýr.
* 
* Örneðin ++x bir ifadedir ve türü L valuedir. Sebebi ise prefix ++ operatorü C++ dilinde L value deðer üretir.
* 
* C++ dilinde 3 farklý value category vardýr. Bunlar sýrasýyla L value, X value ve PR value'dir.
* Bunlarýn farkýný sözel olarak anlatmak çok zor fakat yinede deneyeceðim. L value expressionlar bir identitye sahiptir
* fakat taþýnamaz. X value ifadeler hem identitye sahip hem taþýnabilir. PR value ifadeler ise taþýnabilir fakat bir identitye sahip
* deðildir.
* 
* Örneðin;
* 
* string&& func(void);
* func() // yanlis anlamadiysam burada func bir X value expressiondur.
* 
* 
* Referanslardan bahsedildi. 3 farklý referans vardýr. Bunlar sýrasýyla L value, R value ve universal referanstýr.
* 
* Bazý ifadeler ve onlarýn value categorisinden bahsedildi. Burada çok fazla örnek verildi. Bunlardan bazýlarý aþaðýdadýr.
* 1. Prefix ++/-- operatörüleri ile oluþturulan expressionlar C++ dilinde L value kategorisine sahiptir.
* 2. Lambda ifadeleri birer geçici nesnedir. Dolayýsý ile pr value expressiondur. 
* 3. Atama operatörü L value deðer üretmektedir.
* 
* Burada verilen örnekler çok fazlaydý ve bunlarý gerçekten akýlda tutmak zordu. Bu sebeple Necati hoca
* bir ifadenin value kategorisini öðrenmek için 3 farklý idiom gösterdi. Ben birini aþaðýda yazýp açýklamaya çalýþtým.
* 
* 
*/  

/*
*  Aþaðýda ValCat isimli bir sýnýf þablonu yazýlmýþtýr. Daha sonra bu sýnýf þablonunun iki tane partial specializationu yazýlmýþtýr.
*  Bunlardan birisi T& yani L value expressionlar için, bir diðeri ise T&& yani X value expressionlar için. Daha sonra pvc macrosu yazýlmýþtýr
*  ve bu macro da stringtification token'ý kullanýlmýþtýr. Bu tokenýn kullanýmý C dilinden geldiði için açýklama yapmayacaðým.
*  Ayrýca macro içerisinde decltype kullanýlmýþtýr. Decltype kullanýlma nedeni ise açýktýr. Decltype içerisinde kullanýlan expression yani operand
*  bir ifade ise tür ifadenin value kategorisine göre belirlenmektedir. Aþaðýda bu konu ile ilgili küçük bir açýklama yazýyorum.
* 
* The type denoted by decltype(e) is defined as follows:
	if e is an unparenthesized id-expression or an unparenthesized class member access ([expr.ref]), decltype(e) is the type of the entity named by e. 
	If there is no such entity, or if e names a set of overloaded functions, the program is ill-formed;
	otherwise, if e is an xvalue, decltype(e) is T&&, where T is the type of e;
	otherwise, if e is an lvalue, decltype(e) is T&, where T is the type of e;
	otherwise, decltype(e) is the type of e.
*
*   Daha sonra pvc macrosunu kullanarak L value, PR Value ve X Value ifadeleri macronun argumanýna vererek ekranda ilgili 
*   ifadenin value kategorisini görmüþ olduk. 
* 
*/

/*
#include <iostream>

template<typename T>
struct ValCat {
	static constexpr const char* p = "PR VALUE";
};

template<typename T>
struct ValCat<T&> {
	static constexpr const char* p = "L VALUE";
};

template<typename T>
struct ValCat<T&&> {
	static constexpr const char* p = "X VALUE";
};

#define pvc(expr)    std::cout << "value category of expr '" #expr "' is :" << ValCat<decltype((expr))>::p << '\n'

struct Nec {

};

Nec&& func(void)
{
	return Nec{};
}

int main(void)
{
	Nec x;
	pvc(x); // x ifadesi burada bir L value expressiondur.
	pvc(Nec{}); // Burada bir geçisi sýnýf nesnesi oluþturuluyor ve bu ifade PR value'dir.
	pvc(func()); // func sýnýfýnýn geri dönüþ türü Nec&& tür ve bu fonksiyon bir ifade olarak kullanýldýðýnda value kategorisi X value'dir.

	return 0;
}
*/


/*
* Deðiþken türü referans türü olabilir pointer türü olabilir ama ifadelerin türü referans türü OLAMAZ.
* ifadelerin türü referans türü olamaz. 
*	int x{};
*	int& r{x}; bu ifadenin türü int dir. int& deðildir.
*   int &&r = 23;
*   r-> lvalue expression ama Rvalue reference
*   
*   PR value epxression ise diðer operatörler ile oluþturulan operatörler.
----------------------------------------------------------------------
	Karakter sabitleri 
	tüm sabit ifadeleri
	pvcat(-x);
	pvcat(+x);
	pvcat(x);

	Xvalue expression
-----------------
	Bunlar birkaç tane.Hatta visual studio bir tanesinde de hatalý gösteriyor.

	- Return deðeri sað taraf referansý olan functionlara yapýlan çaðrý ifadeleridir.
	- Move un return deðeride X value expr.
	- PRvalue expression olan bir nesnenin nonstatic veri elemanýna eriþmek xvalue expressiondur.
	struct Nec{
	int x;
	};

	Nec{}.x; // Xvalue Expression. Sýnýf nesnesi geçici nesne yani prvalue expression.Elemanýna eriþme ifadesi xvalue expression

*/

/*
*  Not: Closure type ile ilgili derste bir konuþma geçti. Closure type'nin ne olduðunu buraya da yazmak istedim. 
*  lambdas are implemented is by creating a small class; this class overloads the operator(), 
   so that it acts just like a function. A lambda function is an instance of this class; when the class is constructed, any variables 
   in the surrounding enviroment are passed into the constructor of the lambda function class and saved as member variables. Bu ifadede belirtilen ve 
   derleyici tarafýndan oluþturulan türlere closure type denmektedir.
* 
*/

/*
*  Dersin bir bölümünde nodiscard attribute'den bahsedildi. Bu konuyu maalesef çok iyi bilmiyorum, kursun devamýnda ele alýnacak.
*  Fakat burada nodiscard bize fonksiyonun geri dönüþ deðerinin kullanýlmadýðý senaryolarda derleyiciye bir warning vermeye zorluyor, 
*  ki bu standartlarda "encourage compiler to give warning" tarzýnda bir ifadeyle anlatýlýyor. STL de nodiscard ile bildirilen birçok
*  fonksiyon ve fonksiyon þablonu vardýr.
*/

/* Þimdi biz value category'nin ne olduðunu anladýk. Bu value category'lerden ve function overloading kurallarýndan yararlanarak yavaþ yavaþ
*  move semantic konusuna geçeceðiz. Ýlk olarak aþaðýdaki kod parçasý üzerine konuþalým. Derste de bu kod parçasý üzerine konuþmalar yaptýk.
*/

/*
*  Aþaðýda ki kod parçacýðýnda foo fonksiyonu için 4 farklý overload yer almaktadýr. Main kýsmýnda ise foo fonksiyona
*  4 farklý çaðrý yapýlmýþtýr. Bu çaðrýlarda hangi fonksiyonlarýn overload sonucunda seçilecebileceði yorum satýrý olarak
*  fonksiyon çaðrýlarýnýn yanýna yazýlmýþtýr. Burada Necati hoca const std::string& arguman alan overloadýn bütün çaðrýlar
*  tarafýndan görülür olduðundan ve syntax hatasý almadýðýmýzdan dikkat etmemizi söyledi.
* 
*  Not: Zaten bir sýnýfýn özel üye fonksiyonlarý da böyle deðilmidir. Ha sýnýfýn özel üye fonkisyonlarý, ha aþaðýda foo ve
*  overload larý. Benzer örnekler olduðunu düþünüyorum.
*/

/*
#include <string>
#include <iostream>

void foo(std::string&) { std::cout << "foo(string &)\n"; }  //1
void foo(const std::string&) { std::cout << "foo(const string &)\n"; }  //2
void foo(std::string&&) { std::cout << "foo(string &&)\n"; } //3
void foo(const std::string&&) { std::cout << "foo(const string &&)\n"; } //4

int main()
{
	std::string s{ "l Value" };
	const std::string cs{ "const l Value" };

	foo(s); // 1 2 (3 4 error)
	foo(cs); // 2 (1 3 4 error)
	foo(std::move(s)); //3 4 2 (1 error)
	foo(std::move(cs)); // 4 2 (1 3 error)

}
*/

/* Þimdi yukarýdaki kodlarda hep move dan bahsedildi. Move ile ilgili birþeyler yazmak istiyorum. Aslýnda burada bir ikilem var.
*  Hangi move? Ýki tane move vardýr. Bunlardan birincisi algorithm baþlýk dosyasýnda bulunan move fonksiyonu yada algoritmasýdýr. Bunun
*  örnek impl'si ni aþaðýya býrakýyorum. 
*  
template<class InputIt, class OutputIt>
OutputIt move(InputIt first, InputIt last, OutputIt d_first)
{
    for (; first != last; ++d_first, ++first)
        *d_first = std::move(*first);
 
    return d_first;
}
*
*  Tabi biz burada algoritma move deðil utility baþlýk dosyasýnda bulunan move'dan bahsedeceðiz. Aslýnda move ismi burada
*  çok doðru seçilmiþ bir isim deðil. Çünkü move does not move. Bu ifade Bjarne tarafýndan da söylenmiþtir. Move bir nevi bize R value referans
*  türüne typecast yapmaktadýr. Sanki þunun gibidir;
*  move(s) -> static_cast<string&&>(s);
*  Peki bize ne faydasý vardýr. Bize faydasý bu bir nevi cast iþlemini yaparak x value categorisine sahip expression saðlamasý ve
*  function overloadda T&& parametreli fonksiyonun çaðrýlmasý. Ve biz bu fonksiyon içerisinde kaynak çalabiliriz. Bu yüzden move
*  does not move dedik; move kaynak çalmadý. Biz T&& argumanlý fonksiyond kaynaðý çaldýk. Aþaðýda bununla ilgili bir kod yazdým ve açýklamalar yapmaya çalýþtým.
*/

/*
*  Aþaðýdaki kod hakkýnda genelde yorum yapýldýðýnda acemi programcýlar hemen burada bir taþýmanýn olduðunu iddia ederler.
*  Fakat böyle bir taþýma aþaðýdaki kodda mevcut deðildir. Aþaðýda ki kodda sadece foo fonksiyonuna arguman olarak bir sað taraf referansý
*  gönderilmiþtir std::move sayesinde. Bunun makina düzeyinde pointer argumanlý bir fonksiyona adres göndermekten çok da farký yoktur.
*  name hala kendi kaynaklarýna sahiptir, hala denizi tutmaktadýr. Eðer biz taþýmanýn yani name nesnesinin kaynaklarýný çalmak istiyorsak
*  foo fonksiyonu içerisine þöyle bir kod ekleyebiliriz. std::string s{r}; iþte bu kodda bir taþýma olur ve s nesnesi r nesnesinin kaynaklarýný 
*  çalar.
*/

/*
#include <string>
#include <iostream>

void foo(std::string&& r) {
	
}
	

int main(void)
{
	std::string name{ "deniz" };
	foo(std::move(name));
	return 0;
}

*/

/*
*  Not: Primitive türler için taþýma ve kopyalama arasýnda bir fark yoktur.
*/