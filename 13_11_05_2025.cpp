/*
*  Lambda ifadelerinden devam edildi. Standartlar lambda ifadeleri ile global alandan capture yapılmasını engelliyor. 
*  Fakat msvc derleyicisi buna izin veriyor. Telegramdan Necati hoca bununla ilgili bir mesaj ile birlikte kod attı. 
*  Bu kısımı aşağıya koydum.
* 
Hocam merhaba,
12. dersin sonunda verdiğiniz örnekle ilgili bir sorum var. Verdiğiniz örnek şöyle:

#include <iostream>

int g = 99;

auto fx = [=] { return g + 1; };

auto fy = [g = g] { return g + 1; };

int main()
{
  g = 500;

  std::cout << fx() << '\n';
  std::cout << fy() << '\n';
}

ancak ben bunu derlediğimde şu hatayı alıyorum:
error: non-local lambda expression cannot have a capture-default
    5 | auto fx = [=] { return g + 1; };


Doğru davranış GCC'deki.  Global isim alanında oluşturulan lambda ifadesi "default capture" kullanamaz.
Örnek şöyle olmalı:
#include <iostream>

int g = 99;


int main()
{
  auto fx = [=] { return g + 1; };
  auto fy = [g = g] { return g + 1; };

  g = 500;

  std::cout << fx() << '\n';
  std::cout << fy() << '\n';
}
*/

/*
*  Lambda ifadesinde lambda introducerden sonra yazılabilecek keywordler constexpr, noexcept, mutable ve -> type di. Fakat bunlardan
*  birden fazlasını kullanırsak sıralamaları önemli oluyor. Sıralama şu şekilde olmalıdır.
constexpr mutable noexcept -> double

*/

/*
*  Dersin başlangıcında lambdalar ile ilgili bazı mülakat soruları soruldu. Bu sorulardan bazılarını not aldım.
*/

/*
Soru 1: Çıktı ne olacak.

#include <iostream>

int main()
{
	auto x = [] {static int x{}; return ++x; };
	auto y = [] {static int x{}; return ++x; };

	std::cout << x() << x() << x() << '\n';
	std::cout << y() << y() << y() << '\n';
}

Cevap: Derleyici özdeş lambdalar için farklı closure typelar üretiyor. Dolayısıyla y ve x için iki farklı closure type oluştu.123 123

*/

/*
Soru 2: Hangileri syntax hatası


int main()
{
	const int x = 10;

	auto f = [x]()mutable {++x; };
	auto g = [x = x]() {++x; };
	auto h = [x = x]()mutable {++x; };
}

Cevap: Capture edilen nesne const oldugu zaman capture da da sınıfın veri elemanı const.  mutable olması demek üye 
fonksiyonun kendisi consttur.Lambda init capture da constluk düşüyor g de.Mutable olmadığı için syntax hatası. 

*/

/*

Soru 3:

int main()
{
	int a[10]{};

	auto f1 = [a]()mutable {a = nullptr; };
	auto f2 = [a = a]() {a = nullptr; };
	auto f3 = [a = a]()mutable {a = nullptr; };
	auto f4 = [a]() {++* a; };
	auto f5 = [a]()mutable {++* a; };
	auto f6 = [a = a]() {++* a; };
}

Cevap: Capture edilen dizi de dizi olarak capture edilir, array decay olmaz.Lambda init capture de array decay olur.F4 mutable
olsa syntax hatası değildi.Mesela f5 syntax hatası değil.F6 syntax hatası değil, pointerı set etmiyoruz, 
pointerın gösterdiğini set ediyoruz.

*/

/*
Soru 4

#include <iostream>
#include <type_traits>

double i{};

int main()
{
	auto f = [i = 0]() -> decltype(i){ return 1; }();
	std::cout << std::is_same_v<decltype(f), int>;
}

Cevap: Aşağıda ekrana 0 yani false yazar.Decltype(i) global i.Çünkü lambda capture içinde ki i
trailing return type için visible değil.İsim saklama yok.

*/

/*
*  Aşağıdaki kodda sınıfın non static member fonksiyonları arguman olarak görünmeyen this pointerınada sahip olduğu için
*  fp1 function pointer tanımlamasında syntax hatası alacağız. Fakat static fonksiyon bar için bir syntax hatası yoktur.

struct Myclass {
	int foo(int, int);
	static int bar(int, int);
};

int main()
{
	int (*fp1)(int, int) = &Myclass::foo;
	int (*fp2)(int, int) = &Myclass::bar;
}


*  Fakat yukarıdaki foo fonksiyonu için olan syntax hatasını aşağıdaki syntax ile çözebiliriz.


struct Myclass {
	int foo(int, int);
	static int bar(int, int);
};

int main()
{
	int (Myclass:: * fp1)(int, int) = &Myclass::foo;
}

*/

/*
*  Devamını not alamadım...
*  Positive lambda idiomu anlatıldı. https://www.youtube.com/watch?v=0mDUJo3BN_w&t=285s
* 
*/