/*
*  Format kütüphanesi ile ilgili son derste kaldığımız yerden devam edildi.
* 
*  Necati hoca tekrardan neden format kütüphanesine ihtiyaç olduğunu ne fayda getirdiğinden standart
*  iostream kütüphanesinden ve C'de ki printf, fprintf, sprintf vs gibi fonksiyonların dezavantajlarından
*  bahsetti. 
* 
*  Printf type safe değil fakat format kütüphanesi type safe. Bazı kontrolleri compile time da yapabiliyor. Iostream de ki 
*  eski formatlama çok hantal, yavaş. Tek line da bir sürü sey yapamıyorsunn, birçok fonksiyon cagırman gerekebilir. Printf ile 
*  operator printf yazamıyorsun da mesela, bu da dezavantaj. Bu yüzden user defined türlerle kullanabiliyoruz. 
*  Eski iostream de streamin state’I vardı, bu state’I set ve unset etmek için setf ve unsetf fonksiyonları ile ios::flaglarını 
*  kullanıyorduk. 
* 
*  Aşağıdaki gibi tek line yazmak için şöyle hileler var. Hex manipulatoru possible impl. si bu. Bunun gibi
*  bir çok input stream manipulator vardır. Bu manipulatorleri kullanarak tek line'da ilgili stream'in 
*  state durumunu değiştirebiliyoruz. Bu manipulatorler olmasaydı tek line yerine setf fonksiyonuna
*  ios_base de bulunan fmt_flaglerden arguman göndererek ilgili streamin state'ini değiştirebiliyoruz.
* 

std::ostream& Hex(std::ostream& os)
{
    os.setf(std::ios::hex, std::ios::basefield);
    return os;
}

int main()
{
    using namespace std;
    cout << Hex;
}

*
*  Aşağıdaki gibi de olabilir manipulatorler.

#include <iostream>
#include <iomnip>

class sp {
public:
    sp(int val) : mcnt(val) {}
    friend std::ostream& operator<<(std::ostream& os, const sp& x)
    {
        auto n;
    }

private:
    int mcnt{};
};

int main()
{
    using namespace std;
    cout << sp(12);
}


*
*  Gelelim format kütüphanesine. Necatinin hoca yeni bir konu öğrendiğinde  ilk olarak macro düzeyde öğrenmeye çalışıyor, nedir?
*  Neden var? olmasa ne olur? Sonra micro düzeye gidiyor.
*  
*  İşin kalbinde ismi format olan bir fonksiyon sablonu var.  Bu fonksiyon sablonu template parametreye sahip. Bu format 
*  bize string veriyor. Bu özelliğinden dolayı Necati hoca sprintf’e benzetti ve sprintf ile ilgili derste aşağıdaki
*  örneği yazdı.

#include <iostream>
#include <iomnip>
#include <format>

int main()
{
    int x = 5;
    double dval = 12.56;
    const char* p = "Berat Yildizer";

    char buf[100];

    sprintf(buf, "%d %.3f %s", x, dval, p);
}

*
*  Format fonksiyonu ilk argumanında bizden bir yazı alır. Bu yazıda printf'in ilk argumanı gibi ekrana yazdırılacak yazı
*  veya formatlanacak nesneleri için % yerine bu sefer {} kullanılır. Daha sonra format fonksiyonu bize bir string döndürür.
*  Aşağıda bununla ilgili bir örnek gösterilmiştir. 3 tane {} olmasına rağmen sadece x nesnesi gönderilmiştir. format fonksiyonu
*  sadece x'i yazdıran bir string döndürecektir.
* 
int main()
{
    using namespace std;
    int x = 23453;
    double dval{ 356.961 };
    string name{ "necati" };

    cout << format("{} {} {}", x) << '\n';
}


*  Buradaki {} işareti zero based olarak gitmektedir ve biz içerisine sayılar yerleştirerek bu sırayı custom yapabiliriz.
*  Aşağıdaki örnekte bu durum gösterişmiştir ve format fonksiyonu şöyle bir string dönecektir: ahmet 5. 
* 
int main()
{
    using namespace std;
    string name{ "ahmet" };
    int cnt = 5;

                             // 0     1
    cout << format("{1} {0}\n", cnt, name) << '\n';
}

*  Aşağıdaki format fonksiyonu 3 tane x değerini yan yana 3 tane x'in yan yana olduğu bir string geri dönecektir.
* 
int main()
{
    using namespace std;
    int x = 4;

    cout << format("{0} {0} {0}\n", x);
}

*  Derste yazılan bir başka örnek aşağıdadır. 

#include <iostream>
#include <format>
#include <string>

int main()
{
    std::string s1{ "Necati" };
    std::string s2{ "Ergin" };

    std::cout << std::format("{0} {1}\n", s1, s2);
    std::cout << std::format("{1} {0}\n", s1, s2);
    std::cout << std::format("{0} {0}\n", s1);
    std::cout << std::format("{0} {0}\n", s2);
    std::cout << std::format("{0} {1}\n", s1, s2);
}

*  Peki format fonksiyonuna arguman olarak vereceğimiz const char* parametreli parametrede {} içerisine
*  neler yazabiliriz. Aşağıda gösterilmektedir. İlk olarak fill karakteri ve < ve > ile alignmenti belirtebiliriz. 
*  Daha sonra sign karakter için bir girdi verebiliriz. # karakteri decimal, binary gibi formatting output ve
*  buraya yazabileceklerimiz alttaki table da yer almaktadır. Diğer bütün format karakterleri, anlamları vs aşağıdaki sitede yer 
*  almaktadır.
   https://en.cppreference.com/w/cpp/utility/format/spec.html
    
    [[fill]align] [sign][#][0][width][.precision][type]

    | Spec. | 42       | '@'       | true | Meaning                          |
    | ----- | -------- | --------- | ---- | -------------------------------- |
    | none  | 42       | @         | 64   | Default format                   |
    | d     | 42       | 64        | 1    | Decimal notation                 |
    | b/B   | 101010   | 1000000   | 1    | Binary notation                  |
    | #b    | 0b101010 | 0b1000000 | 0b1  | Binary notation with prefix      |
    | #B    | 0B101010 | 0B1000000 | 0B1  | Binary notation with prefix      |
    | o     | 52       | 100       | 1    | Octal notation                   |
    | x     | 2a       | 40        | 1    | Hexadecimal notation             |
    | X     | 2A       | 40        | 1    | Hexadecimal notation             |
    | #x    | 0x2a     | 0x40      | 0x1  | Hexadecimal notation with prefix |
    | #X    | 0X2A     | 0X40      | 0X1  | Hexadecimal notation with prefix |
    | c     | \*       | @         | '\1' | As character with the value      |
    | s     | invalid  | invalid   | true | bool as string                   |


*  Yukarıdaki table ile kullanım örnekleri aşağıda verilmiştir.
*  Not: Format karakterlerini yada optionlarını girmeden önce : koymalıyız ve eğer istersek iki noktadan önce
*  zero based yerine başka bir sıralama istersek ona göre bir index yazabiliriz.
int main()
{
    using namespace std;

    int x = 2435;
    char c = '!';
    bool b = true;

    std::cout << format("{} {} {}", x, c, b) << '\n';
    std::cout << format("{:d} {:d} {:d}", x, c, b) << '\n';
    std::cout << format("{:b} {:b} {:b}", x, c, b) << '\n';
    std::cout << format("{:#b} {:#b} {:#b}", x, c, b) << '\n';
    std::cout << format("{:#B} {:#B} {:#B}", x, c, b) << '\n';
    std::cout << format("{:o} {:o} {:o}", x, c, b) << '\n';
    std::cout << format("{:#o} {:#o} {:#o}", x, c, b) << '\n';
    std::cout << format("{:x} {:x} {:x}", x, c, b) << '\n';
    std::cout << format("{:#x} {:#x} {:#x}", x, c, b) << '\n';
    std::cout << format("{:X} {:X} {:X}", x, c, b) << '\n';
}


#include <iostream>
#include <format>

int main()
{
    using std::cout, std::format;

    cout << format("{:x}", 47802) << '\n'; // baba
    cout << format("{:X}", 47802) << '\n'; // BABA
    cout << format("{:#x}", 47802) << '\n'; // 0xbaba
    cout << format("{:#X}", 47802) << '\n'; // 0XBABA
    cout << format("{:b}", 47802) << '\n'; // 10111010111010
    cout << format("{:#b}", 47802) << '\n'; // 0b10111010111010
    cout << format("{:o}", 47802) << '\n'; // 135272
    cout << format("{:#o}", 47802) << '\n'; // 0135272
    cout << format("{:f}", 5.) << '\n'; // 5.000000
    cout << format("{:#f}", 5.) << '\n'; // 5.000000
}

*
*  Format karakterlerinde output with ve precision dinamik olarak run time'da da belirtilebiliyor.
* 
int main()
{
    using std::cout, std::format;

    double dval = 3. / 7;
    int width = 12;
    int precision = 5;

    cout << format("|{2:<{0}.{1}f}|", width, precision, dval); // |0.42857     |
    // 2 == position
    // 0 == width
    // 1 == precision
}

*  Derste yazılan bir başka örnek ve çıktıları.
* 
int main()
{
    using std::cout, std::format;

    int x{ 171 };

    cout << format("{:*^20d}|", x) << '\n';  // |       171       |
    cout << format("{:*^20b}|", x) << '\n';  // | 10101011       |
    cout << format("{:*^20b}|", x) << '\n';  // | 0b10101011     |
    cout << format("{:*^20X}|", x) << '\n';  // | ab             |
    cout << format("{:*^20X}|", x) << '\n';  // | AB             |
    cout << format("{:*^20X}|", x) << '\n';  // | 0XAB           |
}

*  
*  Printf de yüzde için iki tane % de kullanıyorduk. Formatta da küme parantezi için de yine içi içe küme parantezi yazıyoruz.
*  Aşağıda bununla ilgili yazılan örnek kod.

int main()
{
    using namespace std;

    int ival{ 546 };

    cout << format("{{}}\n");  // {546}
}

*
*  Aşağıdaki örnekte width formatlama parametresini kullanıcında aldık. Yukarıda da dediğimiz gibi output witdh
*  run time da belirlenebiliyor ve width içerideki {} ile eşleşiyor.
* 
int main()
{
    using std::cout, std::format, std::cin;

    int x = 41396;

    cout << "output width: ";
    int width;
    cin >> width;

    cout << format("|{:{}{|", x, width) << '\n';
    cout << format("|{: <{}}|", x, width) << '\n';
    cout << format("|{:>{}}|", x, width) << '\n';
    cout << format("|{: ^{}}|", x, width) << '\n';
}

*
*  Derste sorduğum soru: :>>…} saga dayalı > ile fill mi edecek hocam? ... sonrası herhangi birşey anlamında
*  Cevap: Evet, aşağıda bir örneği.
* 
int main()
{
    using namespace std;

    cout << format("{:>>10}\n", 34);
}

*   
*   Format fonksiyonu arka tarafta bir string nesnesi oluşturuyor ve bize o stringi veriyor. Fakat biz bu yükden 
*   kurtulmak istiyorsak, yani run time da bir allocation yapılmasını istemiyorsak format_to fonksiyonunu
*   kullanabiliriz. Bu fonksiyona varolan bir yerin başlangıç konumunu gösteren bir iterator vererek string yerine
*   elde edilecek formatlı yazıyı direk olarak o konuma yazdırabiliriz. Verilen range formatlı cıkıştan küçük
*   olursa bu tanımsız davranış olacaktır. Format_to ile derste yazılan iki örnek aşağıda verilmiştir. Bu fonksiyon
*   ile range yazmak için ostream_iterator kullanmak çok yaygındır ve ilk örnekte ostream_iterator<cout> açılımı ile
*   standart çıkış akımına direk formatlanmış yazıyı yazdırabildik.
* 
int main()
{
    using namespace std;

    int ival = 82745;

    std::ofstream ofs{ "out.txt" };
    if (!ofs) {
        std::cerr << "out.txt dosyasi olusturulamadi\n";
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 100; ++i) {
        format_to(ostream_iterator<char>(ofs), "{:>8} {:>12.6f}\n", i, sqrt(i));
    }
}


int main()
{
    std::string name{ "Ali Aksoy" };
    int id{ 78945 };
    double dval{ 54.213455 };
    std::string str;

    format_to(back_inserter(str), "[{} {} {:.2f]()


*
*   format_to ile dinamik bellek ayırmadan bir range'e yazı yazabiliyoruz. Ancak dezavantajı, range'i kontrol edemememiz
*   ve eğer range çok küçükse tanımsız davranışa yol açabilmesidir. Bunun için ise format_to_n fonksiyonu mevcuttur.
*
*   Format_to_n bir sınıf türünden geri dönüşe sahip. Bu geri dönüş sınıfının iki elemanı var, birisi .out. 
*   Bu en son yazılan konumdan bir sonraki ni döndürüyor. Bu bir iteratordur. En son ki yere null terminated string 
*   yaomak için null karakteri kendimiz yazmalıyız. Diğer elemanı da size dır. Bu size da normal de yazması gereken 
*   size sayısını veriyor. Böylece size a bakarak karakterlerin hepsini yazdımı yazmadı mı onu test edebiliriz. 
* 
int main()
{
    using namespace std;
    char buf[10];
    string str;

    cout << "bir yazi girin: ";
    cin >> str;

    format_to_n(buf, sizeof(buf) - 1, "{}", str);
}

*
*   Birde formatted_size fonksiyonu var. Bu fonksiyon ile size_t türünden formatlanmış yazının boyutunu elde edebiliriz. 
*   Bu fonksiyonu da formatted_to ile kullanarak size ile ilgili tanımsız davranışların önüne
*   geçebiliriz. Aşağıda derste yazılan örnekler verilmiştir.
*
int main()
{
    int x{ 65234 };
    double dval = 123.654;

    const char* p = "{:#X} {:2f}";
    const auto n = std::formatted_size(p, x, dval); // formatted string size

    std::vector<char> buf(n);

    format_to(buf.begin(), p, x, dval);

    for (auto c : buf)
        std::cout << c << ' ';  // 0 X F E D 2   1 2 3 . 6 5
}


int main()
{
    using namespace std;

    int age = 41;
    string name = "berat yildizer";

    auto n = formatted_size("{:>20} {:#b}", name, age);

    cout << "formatted length = " << n << '\n';
}

* 
*   format_to_n ile ilgili derste yazılan diğer örnek aşağıda.

int main()
{
    using namespace std;
    char buf[10];
    string str;

    cout << "bir yazi girin: ";
    cin >> str;

    auto result = format_to_n(buf, sizeof(buf) - 1, "{:.<20}", str);

    *result.out = '\0';

    cout << "|" << buf << "| \n";
    cout << "formatted size : " << result.size << "\n";
}

Şimdi gelelim user defined türler için format kütüphanesine. Formatter bir sınıfı var.  Formatlayacağımız user defined 
türün bir specialization u nu formatter için yazıyoruz. Bu formatter specialization için iki tane fonksiyon yazmam gerekiyor.
Dersin son bölümlerinde anlatıldı bu kısım. 20. Derste bu konudan devam edilecek. Orada detaylı not yazacağım.
*/