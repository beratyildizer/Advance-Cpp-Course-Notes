/*
* 53 Ders g�n� olacak. C++ Kursunu alanlar daha avantajl�d�r.
* 
* Bizim i�in bir ifadenin yani expressionun t�r� ve value kategorisi �ok �nemlidir. Peki expression nedir.
* An expression in C and C++ is any valid combination of operators, constants, functions and variables.
* 
* �rne�in: char x; // Burada x nesnesinin t�r� char'd�r fakat burada x i�in value category den bahsedemeyiz
* ��nk� bu bir expression de�ildir. Bir nesnenin tan�m�d�r. Value kategoriden bahsedebilmek i�in elimizde bir ifade yani 
* expression olmal�d�r.
* 
* �rne�in ++x bir ifadedir ve t�r� L valuedir. Sebebi ise prefix ++ operator� C++ dilinde L value de�er �retir.
* 
* C++ dilinde 3 farkl� value category vard�r. Bunlar s�ras�yla L value, X value ve PR value'dir.
* Bunlar�n fark�n� s�zel olarak anlatmak �ok zor fakat yinede deneyece�im. L value expressionlar bir identitye sahiptir
* fakat ta��namaz. X value ifadeler hem identitye sahip hem ta��nabilir. PR value ifadeler ise ta��nabilir fakat bir identitye sahip
* de�ildir.
* 
* �rne�in;
* 
* string&& func(void);
* func() // yanlis anlamadiysam burada func bir X value expressiondur.
* 
* 
* Referanslardan bahsedildi. 3 farkl� referans vard�r. Bunlar s�ras�yla L value, R value ve universal referanst�r.
* 
* Baz� ifadeler ve onlar�n value categorisinden bahsedildi. Burada �ok fazla �rnek verildi. Bunlardan baz�lar� a�a��dad�r.
* 1. Prefix ++/-- operat�r�leri ile olu�turulan expressionlar C++ dilinde L value kategorisine sahiptir.
* 2. Lambda ifadeleri birer ge�ici nesnedir. Dolay�s� ile pr value expressiondur. 
* 3. Atama operat�r� L value de�er �retmektedir.
* 
* Burada verilen �rnekler �ok fazlayd� ve bunlar� ger�ekten ak�lda tutmak zordu. Bu sebeple Necati hoca
* bir ifadenin value kategorisini ��renmek i�in 3 farkl� idiom g�sterdi. Ben birini a�a��da yaz�p a��klamaya �al��t�m.
* 
* 
*/  

/*
*  A�a��da ValCat isimli bir s�n�f �ablonu yaz�lm��t�r. Daha sonra bu s�n�f �ablonunun iki tane partial specializationu yaz�lm��t�r.
*  Bunlardan birisi T& yani L value expressionlar i�in, bir di�eri ise T&& yani X value expressionlar i�in. Daha sonra pvc macrosu yaz�lm��t�r
*  ve bu macro da stringtification token'� kullan�lm��t�r. Bu token�n kullan�m� C dilinden geldi�i i�in a��klama yapmayaca��m.
*  Ayr�ca macro i�erisinde decltype kullan�lm��t�r. Decltype kullan�lma nedeni ise a��kt�r. Decltype i�erisinde kullan�lan expression yani operand
*  bir ifade ise t�r ifadenin value kategorisine g�re belirlenmektedir. A�a��da bu konu ile ilgili k���k bir a��klama yaz�yorum.
* 
* The type denoted by decltype(e) is defined as follows:
	if e is an unparenthesized id-expression or an unparenthesized class member access ([expr.ref]), decltype(e) is the type of the entity named by e. 
	If there is no such entity, or if e names a set of overloaded functions, the program is ill-formed;
	otherwise, if e is an xvalue, decltype(e) is T&&, where T is the type of e;
	otherwise, if e is an lvalue, decltype(e) is T&, where T is the type of e;
	otherwise, decltype(e) is the type of e.
*
*   Daha sonra pvc macrosunu kullanarak L value, PR Value ve X Value ifadeleri macronun arguman�na vererek ekranda ilgili 
*   ifadenin value kategorisini g�rm�� olduk. 
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
	pvc(Nec{}); // Burada bir ge�isi s�n�f nesnesi olu�turuluyor ve bu ifade PR value'dir.
	pvc(func()); // func s�n�f�n�n geri d�n�� t�r� Nec&& t�r ve bu fonksiyon bir ifade olarak kullan�ld���nda value kategorisi X value'dir.

	return 0;
}
*/


/*
* De�i�ken t�r� referans t�r� olabilir pointer t�r� olabilir ama ifadelerin t�r� referans t�r� OLAMAZ.
* ifadelerin t�r� referans t�r� olamaz. 
*	int x{};
*	int& r{x}; bu ifadenin t�r� int dir. int& de�ildir.
*   int &&r = 23;
*   r-> lvalue expression ama Rvalue reference
*   
*   PR value epxression ise di�er operat�rler ile olu�turulan operat�rler.
----------------------------------------------------------------------
	Karakter sabitleri 
	t�m sabit ifadeleri
	pvcat(-x);
	pvcat(+x);
	pvcat(x);

	Xvalue expression
-----------------
	Bunlar birka� tane.Hatta visual studio bir tanesinde de hatal� g�steriyor.

	- Return de�eri sa� taraf referans� olan functionlara yap�lan �a�r� ifadeleridir.
	- Move un return de�eride X value expr.
	- PRvalue expression olan bir nesnenin nonstatic veri eleman�na eri�mek xvalue expressiondur.
	struct Nec{
	int x;
	};

	Nec{}.x; // Xvalue Expression. S�n�f nesnesi ge�ici nesne yani prvalue expression.Eleman�na eri�me ifadesi xvalue expression

*/

/*
*  Not: Closure type ile ilgili derste bir konu�ma ge�ti. Closure type'nin ne oldu�unu buraya da yazmak istedim. 
*  lambdas are implemented is by creating a small class; this class overloads the operator(), 
   so that it acts just like a function. A lambda function is an instance of this class; when the class is constructed, any variables 
   in the surrounding enviroment are passed into the constructor of the lambda function class and saved as member variables. Bu ifadede belirtilen ve 
   derleyici taraf�ndan olu�turulan t�rlere closure type denmektedir.
* 
*/

/*
*  Dersin bir b�l�m�nde nodiscard attribute'den bahsedildi. Bu konuyu maalesef �ok iyi bilmiyorum, kursun devam�nda ele al�nacak.
*  Fakat burada nodiscard bize fonksiyonun geri d�n�� de�erinin kullan�lmad��� senaryolarda derleyiciye bir warning vermeye zorluyor, 
*  ki bu standartlarda "encourage compiler to give warning" tarz�nda bir ifadeyle anlat�l�yor. STL de nodiscard ile bildirilen bir�ok
*  fonksiyon ve fonksiyon �ablonu vard�r.
*/

/* �imdi biz value category'nin ne oldu�unu anlad�k. Bu value category'lerden ve function overloading kurallar�ndan yararlanarak yava� yava�
*  move semantic konusuna ge�ece�iz. �lk olarak a�a��daki kod par�as� �zerine konu�al�m. Derste de bu kod par�as� �zerine konu�malar yapt�k.
*/

/*
*  A�a��da ki kod par�ac���nda foo fonksiyonu i�in 4 farkl� overload yer almaktad�r. Main k�sm�nda ise foo fonksiyona
*  4 farkl� �a�r� yap�lm��t�r. Bu �a�r�larda hangi fonksiyonlar�n overload sonucunda se�ilecebilece�i yorum sat�r� olarak
*  fonksiyon �a�r�lar�n�n yan�na yaz�lm��t�r. Burada Necati hoca const std::string& arguman alan overload�n b�t�n �a�r�lar
*  taraf�ndan g�r�l�r oldu�undan ve syntax hatas� almad���m�zdan dikkat etmemizi s�yledi.
* 
*  Not: Zaten bir s�n�f�n �zel �ye fonksiyonlar� da b�yle de�ilmidir. Ha s�n�f�n �zel �ye fonkisyonlar�, ha a�a��da foo ve
*  overload lar�. Benzer �rnekler oldu�unu d���n�yorum.
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

/* �imdi yukar�daki kodlarda hep move dan bahsedildi. Move ile ilgili bir�eyler yazmak istiyorum. Asl�nda burada bir ikilem var.
*  Hangi move? �ki tane move vard�r. Bunlardan birincisi algorithm ba�l�k dosyas�nda bulunan move fonksiyonu yada algoritmas�d�r. Bunun
*  �rnek impl'si ni a�a��ya b�rak�yorum. 
*  
template<class InputIt, class OutputIt>
OutputIt move(InputIt first, InputIt last, OutputIt d_first)
{
    for (; first != last; ++d_first, ++first)
        *d_first = std::move(*first);
 
    return d_first;
}
*
*  Tabi biz burada algoritma move de�il utility ba�l�k dosyas�nda bulunan move'dan bahsedece�iz. Asl�nda move ismi burada
*  �ok do�ru se�ilmi� bir isim de�il. ��nk� move does not move. Bu ifade Bjarne taraf�ndan da s�ylenmi�tir. Move bir nevi bize R value referans
*  t�r�ne typecast yapmaktad�r. Sanki �unun gibidir;
*  move(s) -> static_cast<string&&>(s);
*  Peki bize ne faydas� vard�r. Bize faydas� bu bir nevi cast i�lemini yaparak x value categorisine sahip expression sa�lamas� ve
*  function overloadda T&& parametreli fonksiyonun �a�r�lmas�. Ve biz bu fonksiyon i�erisinde kaynak �alabiliriz. Bu y�zden move
*  does not move dedik; move kaynak �almad�. Biz T&& argumanl� fonksiyond kayna�� �ald�k. A�a��da bununla ilgili bir kod yazd�m ve a��klamalar yapmaya �al��t�m.
*/

/*
*  A�a��daki kod hakk�nda genelde yorum yap�ld���nda acemi programc�lar hemen burada bir ta��man�n oldu�unu iddia ederler.
*  Fakat b�yle bir ta��ma a�a��daki kodda mevcut de�ildir. A�a��da ki kodda sadece foo fonksiyonuna arguman olarak bir sa� taraf referans�
*  g�nderilmi�tir std::move sayesinde. Bunun makina d�zeyinde pointer argumanl� bir fonksiyona adres g�ndermekten �ok da fark� yoktur.
*  name hala kendi kaynaklar�na sahiptir, hala denizi tutmaktad�r. E�er biz ta��man�n yani name nesnesinin kaynaklar�n� �almak istiyorsak
*  foo fonksiyonu i�erisine ��yle bir kod ekleyebiliriz. std::string s{r}; i�te bu kodda bir ta��ma olur ve s nesnesi r nesnesinin kaynaklar�n� 
*  �alar.
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
*  Not: Primitive t�rler i�in ta��ma ve kopyalama aras�nda bir fark yoktur.
*/