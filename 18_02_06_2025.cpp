/*
*  Spaceship operatorunu incelemeye devam ettik 18. derste.
*  
*  Bir sınıfın karşılaştırma operator fonksiyonlarının yazılması hep aynı kodların yazılmasına sebep oluyor. 
*  Genelde operator< ve operator== fonksiuonları implemente ediliyor ve diğer kalan operator karşılaştırma 
*  fonksiyonları bu iki fonksiyonu çağıracak şekilde implemente ediliyor. Bazı durumlarda da bu karşılaştırma
*  operatorleri için bir çok şeyide fonksiyonu tanımlarken belirtmeliyiz; mesela exception throw durumu, 
*  geri dönüş değeri ile ilgili nodiscard kullanımı vs... 
* 
*  Fakat biz bu detaylarla uğraşmasak ve bunları tek tek overload etmek yerine derleyici bizim için yazsa nasil 
*  olur? Tıpkı sınıfların special member functionları gibi. Spaceship yada resmi adıyla three way comparison operatoru
*  ile derleyiciye bunu yaptırabiliyoruz.
* 
*  Spaceship operatoru dile C++ 20 ile eklenmişti. Bu operator ile birlikte bazı fonksiyonların default edilebilmesi, 
*  bazı türler de dile eklendi. Compare header'ı mesela hayatımıza giriş yaptı. Mesela STL'in containerlarına bakarsak
*  C++ 20 ile beraber bütün operator karşılaştırma fonksiyonları dilden kaldırıldı ve bunlar yerine sadece
*  operator<=> fonksiyonu declare edildi. Bu spaceship operatoru ile birlikte karşılaştırma operatorleri için de 
*  iki grup oluştu. Bunlar primary ve secondary. 17. Derste de belirttiğimiz gibi == ve <=> operatorleri primary kategoride.
*  Bunların en büyük özelliği artık default olarak bildirilebilmesi ve reversible olması. Reversible ne demek birazdan 
*  anlayacağız.
* 
*  Not: Eğer biz sadece spaceship operatorunu default edersek derleyici == operator fonksiyonunu da default 
*  edecektir. İster global olsun ister member fonksiyon olsun bu fonksiyonları default edebiliriz.
* 
class Myclass {
public:
    // auto operator<=>(const Myclass&) const = default;
    friend auto operator<=>(const Myclass&, const Myclass&) = default;

private:
    int mx{}, my{};
};

*
*  Peki derleyicinin yazacağı member default spaceship operatoru nasıl yazılır. Derleyici sanki şöyle bir kod yazar.
*  *this'in memberlarını bildirim sırasına göre compare eder.
* 
*  Aşağıda bir örneği gösterilmiştir. Burada operator<=> sınıfın veri elemanı olan mval int olduğu için ve int
*  iki nesneyi karşılaştırdığı için strong_ordering döndürür.Aşağıdaki fonksiyonu işte biz default yapsaydıkda
*  derleyici böyle bir kod yazacaktı. Mesela mval double türden bir nesne olsaydı spaceship operatoru partial_ordering
*  türden birşey döndürecekti. Peki Mint sınıfında iki tane member olsaydı bu operator bunları karşılaştırırken ne 
*  döndürecekti. İlk memberı compare edecekti. Eğer eşit değilse ikinci ögeye bakmayacaktı. Eşit ise ikinci ögeye bakacaktı.
* 
class Mint {
public:
    Mint() = default;
    Mint(int val) : mval{val} {}

    auto operator<=>(const Mint& other) const {
        return mval <=> other.mval;
    }

private:
    int mval{};
};

*
*  Aşağıda iki örnek verilmiştir ve bu iki örnek aslında aynı şeyi yapmaktadır. Peki neden. Çünkü küçüktür operator
*  secondary grupta olduğu için rewritable. Derleyici bu çağrıyı gördüğü zaman sınıfın yada global alanda operator< fonksiyonu 
*  olmadığı için bu ifadeyi ((p1 <=> p2) < 0) ifadesine dönüştürür. Unutma, secondary operatorlerin en büyük özelliği 
*  burada olduğu gibi rewritable olmasıdır. 
* 
struct Point {
    Point() = default;
    Point(int x, int y) : mx(x), my(y) { }

    auto operator<=>(const Point&) const = default;

    int mx{}, my{};
};

int main()
{
    using namespace std;

    Point p1{2, 5}, p2{2, 7};

    cout << ((p1 <=> p2) < 0) << '\n';
}

struct Point {
    Point() = default;
    Point(int x, int y) : mx(x), my(y) { }

    auto operator<=>(const Point&) const = default;

    int mx{}, my{};
};

int main()
{
    using namespace std;

    Point p1{2, 5}, p2{2, 7};

    cout << (p1 < p2) << '\n';
}


* 
*  Not: Biz operator<=> fonksiyonunu sınıf içinde hiç bildirmez isek derleyici bu fonksiyonu implicitly default
*  yapmamaktadır. Ben istiyorsam bunu explicit default yapmak zorundayım.
* 
*  Not: Biz operator hazır kendimiz yazarsak == otomatik default edilmiyor. Fakat kendimiz default etseydik o zaman gelirdi.

*
*  İki tane member olduğu zaman derleyicinin default bildirim için yazacağı operator<=> fonksiyonu aşağıdaki gibidir.
* 
struct Point {
    Point() = default;
    Point(int x, int y) : mx(x), my(y) {}

    std::strong_ordering operator<=>(const Point& other) const {
        if (auto result = mx <=> other.mx; result != 0)
            return result;
        return my <=> other.my;
    }

    int mx{}, my{};
};

*
*  Aşağıda derste yazılan operator== fonksiyonu için bir örnek verilmiştir. Derleyici operator== fonksiyonunu 
*  default yazdığımız zaman aşağıdaki gibi bir kod yazar. Ayrıca derleyici operator!= çağırısını gördüğü zaman
*  ve operator== bildirilmişse operator!= secondary grupta ve rewritable oldupu için onu !(a==b) şeklinde
*  yazmaktadır. Aşağıdaki örnek de aslında bunu göstermektedir.
* 
// if == operator function is declared compiler rewrites != expressions
// for a != b
// !(a == b) if it is not valid
// !(b == a)

#include <iostream>

struct Nec {
public:
    Nec(int val) : mx(val) {} //constructor is not explicit!
    [[nodiscard]] bool operator==(const Nec& other) const
    {
        return mx == other.mx;
    }
private:
    int mx;
};

int main()
{
    std::cout << std::boolalpha;
    Nec n1{ 43 }, n2{ 76 };

    std::cout << (n1 == n2) << '\n';
    std::cout << (n1 != n2) << '\n';
    std::cout << (10 != n1) << '\n';
    std::cout << (n2 != 76) << '\n';
}

* 
*  Primary operatorler için reversible ifadesini kullanmıştık. Bunun anlamı şudur. Örneğin derleyici
*  bir operator<=> veya operator== çağrısı gördüğü zaman eğer operandların sırasını gerekli durumlarda
*  değiştirebilir. Fakat secondary operatorler için bu geçerli değildir, secondary operatorler rewritable değildir.

struct Nec {
    int m_val{};
    bool operator!=(int val) const
    {
        return val != m_val;
    }
};

int main()
{
    Nec n;

    auto b1 = n != 5;
    auto b2 = 5 != n; //invalid
    //secondary operators cannot be reversed
}

*
*  Aşağıda derste yazılan birkaç örnek ve çıktıları yorum satırında yer almaktadır.

#include <compare>
#include <iostream>

int main()
{
    boolalpha(std::cout);

    std::cout << (10 <=> 5 > 0) << '\n';  // true
    std::cout << (10 <=> 5 < 0) << '\n';  // false
    std::cout << (10 <=> 5 <= 0) << '\n'; // false
    std::cout << (10 <=> 5 >= 0) << '\n'; // true
    std::cout << (10 <=> 5 == 0) << '\n'; // false
    std::cout << (10 <=> 5 != 0) << '\n'; // true
}


class Point {
public:
    Point(int x, int y) : mx(x), my(y) {}
    auto operator<=>(const Point&) const = default;
private:
    int mx;
    int my;
};

int main()
{
    Point p1{ 2, 2 }, p2{ 2, 5 };

    boolalpha(std::cout);

    std::cout << (p1 == p2) << '\n'; // false
    std::cout << (p1 != p2) << '\n'; // true

    std::cout << (p1 < p2) << '\n';  // true
    std::cout << (p1 <= p2) << '\n'; // true
    std::cout << (p1 > p2) << '\n';  // false
    std::cout << (p1 >= p2) << '\n'; // false
}

*
*  Derleyici default olarak bildirilen sınıfın operator<=> fonksiyonunu yapabilirse constexpr yapmaktadır. 
*  Aşağıdaki örnek bununla ilgilidir.
* 
struct Nec {
    int a;
    char b;
    auto operator<=>(const Nec&) const = default;
};

constexpr bool test()
{
    Nec x{1, 'a'}, y{2, 'b'};
    return (x <=> y) < 0;
}

static_assert(test());

*
*  Ayrıca derleyici yazacağı default operator<=> için exception throw etmeme garantisi verir, yani fonksiyonu
*  noexcept specifierı ile birlikte yazmaktadır.
* 
struct Nec {
    int x;
    auto operator<=>(const Nec&) const = default;
};

static_assert(noexcept(Nec{} <=> Nec{}));

* 
*  Sınıfın operator<=> ve operator== fonksiyonları aşağıdaki gibi de yazılabilir.

class Nec {
public:
    constexpr Nec(int i) noexcept : mval{ i } {}

    // for == and !=
    [[nodiscard]] bool operator==(const Nec& other) const noexcept {
        return mval == other.mval;
    }

    // for < <= > >=
    [[nodiscard]] std::strong_ordering operator<=>(const Nec& other) const noexcept {
        if (mval == other.mval)
            return std::strong_ordering::equal;
        if (mval < other.mval)
            return std::strong_ordering::less;
        return std::strong_ordering::greater;
    }

private:
    int mval;
};

*
*  STL de bilindiği üzere lexicopraphical_compare fonksiyonu vardır. Bu fonksiyon iki farklı range alır ve
*  bu rangeleri lexicopraphical olarak karşılaştırır. Bu rangeleri de iterator olarak almaktadır. Aşağıda bu fonksiyonun
*  bir kullanım örneği gösterilmiştir.

#include <iostream>
#include <compare>
#include <algorithm>
#include <vector>

int main()
{
    using namespace std;

    vector x{ 1, 2, 5, 7, 9 };
    vector y{ 1, 2, 6 };

    cout << boolalpha << (x < y) << '\n';

    cout << lexicographical_compare(
        x.begin(), x.end(), y.begin(), y.end()) << '\n';
}

*
*  C++ 20 ile birde lexicographical_compare_three_way fonksiyonu eklendi ve bu fonksiyon spaceship operatorunun
*  geri döndürdüğü değer. Bu fonksiyon spaceship operatorunu kullanarak bu comparisonu yapmaktadır. 

int main()
{
    using namespace std;

    vector x{ 1, 2, 5, 7, 9 };
    vector y{ 1, 2, 6 };

    auto result = lexicographical_compare_three_way(
        x.begin(), x.end(), y.begin(), y.end());
}

*
*  Şimdilik three way comparison operator ile işimiz bu kadar. Necati hoca yeni konu başlığına geçiş yaptı.
*  Format kütüphanesi. C++ 20 ile yapılan bir devrimdir kendisi. 
*  
*  Formatted output, unformatted output: Formatted output for human, another is for machine. Unformatted, bellekte 
*  tutulduğu gibi byte byte iletmek. Putchar mesela formatsız çıkış işlemi. Printf mesela formatlı. Formatlı cıkış demek
*  insanın anlayacağı sekilde göstermek. Formatlı cıkısın maliyeti formatsıza göre fazla.
*
*  C++ 20 den önce formatlı giriş ve çıkış işlemleri çok teferrutatlıydı. Her stream nesnesinin bir state'e durumu 
*  vardır ve biz bu state durumunu çeşitli flaglar ile set ediyoruz. Daha sonra bu format state'e göre output'u görüyoruz. 
*  ios_base sınıfında boolalpha, hex vs gibi flagları setf fonksiyonuna arguman vererek bu state'i değiştiriyorduk. Hem
*  format state'i değiştirmek hemde birşeyler okuyup yazmak için tek satırda da manipulatorleri kullanıyorduk, mesela
*  endl manipulatoru. Bunların kullanımı hem maliyetli olabiliyor, hem karmaşık olabiliyor vs vs.. Dolayısı ile 
*  C++ 20 de format kütüphanesi dile eklenerek bu dezavantajlar yok edilmeye çalışılmıştır. 
* 
*  Mesela C dilinde printf ile formatlı çıkış işlemlerini standart çıkış akımına birşeyler yazmak için yapabiliyorduk.
*  printf formatlı çıkışı bize ilk argumanı olan const char* parametreli null terminated string ile sağlıyor ve biz bu
*  stringi dışarıdan format karakterler veriyoruz. Printf'in en büyük dezavantajı type safe değil mesela. 
* 
*  İşte format kütüphanesi ile bu dezavantajları yenip, tek line da, çoğu kontrolu compile time'da, type safe
*  bir şekilde vs formatlı giriş çıkış işlemlerini yapabiliyoruz. 
* 
*  Aşağıdaki cppcon videosu format kütüphanesi için çok faydalı.
*  
*  https://www.youtube.com/watch?v=IdM0Z2a4fjU&t=1441s&ab_channel=CppCon
* 
* /