/*
*  İlk olarak daha önceden derste sorulan bir soru cevaplandı. Soru şu şekildeydi. back_inserter ile
*  move_iterator beraber kullanılabilir mi? Evet yapılabilir. push_back işlemini back_inserter
*  move_iterator kullanarak yapacak ve örnek kod aşağıdadır. Names'de ki stringler gün sonunda moved
*  from state'e kalacaktır. 
*  
int main()
{
    using namespace std;

    vector<string> names{ "necati", "aysegul", "cem", "cinar", "berat" };
    vector<string> dest;

    copy(make_move_iterator(names.begin()), make_move_iterator(names.end()), back_inserter(dest));
}

This code moves the strings from the names vector to the dest vector using std::make_move_iterator and 
std::copy, which efficiently transfers ownership rather than copying the strings. After the operation, 
the names vector will be left with empty strings.
* 
*/

/* Necati hoca bugün algoritmaları tamamlayacağını söyledi. Daha sonra da spaceship operatorunu anlatacak.
*/

/*
*  İlk olarak sorted range algoritmalardan başladı ders. Bu sorted range algoritmalar sıralanmış rangeler
*  üzerinde işlem yapıyor. Yani arguman olduğu range'in sıralanmış olması gerekmektedir. Aşağıda bu 
*  algoritmalar listelenmiştir.
* 
Sorted Range Algorithms Search and Range Queries:

binary_search

lower_bound

upper_bound

equal_range

Merge and Set Operations:

includes

merge

inplace_merge

set_intersection

set_union

set_difference

set_symmetric_difference
*/

/*
*  Include algoritması bir sıralı range başka bir sıralı range'i içeriyor mu, bunun cevabını alır.
*  Bool geri döndürmektedir. Search ve find_end iterator döndürürken bu fonksiyon bool döndürüyor. Ayrıca
*  aranan range'in sıralı olması gerekmektedir. does include? sorusunun cevabını verir. Aşağıda derste 
*  yazılan örnek kod.
* 
* 
int main()
{
    using namespace std;

    vector<int> ivec;
    rfill(ivec, 20, Irand{ 0, 99 });
    sort(ivec.begin(), ivec.end());
    cprint(ivec);

    int x, y, z, t;
    std::cout << "kucukten buyuge 4 tam sayi girin: ";
    cin >> x >> y >> z >> t;

    auto list = { x, y, z, t };

    cprint(list.begin(), list.end());

    std::cout << boolalpha << includes(ivec.begin(), ivec.end(), list.begin(), list.end()) << '\n';
}

*/

/*
*  Diğer algoritma merge algoritması, birleştirme anlamında. Burada ki kastedilen sıralı birleştirme.
*  Necati hocamız C kurslarında yazdığı O(n) karmaşıklığında ki merge kodu aşağıdadır.
* 
#define SIZE 10

int main()
{
    int a[SIZE] = { 2, 6, 8, 11, 34, 77, 92, 133, 312, 765 };
    int b[SIZE] = { 12, 16, 18, 21, 45, 72, 75, 89, 103, 105 };
    int c[2 * SIZE];

    int idx_a = 0, idx_b = 0;

    for (int i = 0; i < 2 * SIZE; ++i) {
        if (idx_a == SIZE)
            c[i] = b[idx_b++];
        else if (idx_b == SIZE)
            c[i] = a[idx_a++];
        else if (a[idx_a] < b[idx_b])
            c[i] = a[idx_a++];
        else
            c[i] = b[idx_b++];
    }
}

This program merges two sorted arrays a and b, each of size SIZE (10), into a single sorted
array c of size 2 * SIZE (20) using a two-pointer merge algorithm. It's effectively implementing
the merge step of merge sort.

*
* Merge algoritması ile ilgilide aşağıdaki kod derste yazıldı. ostream_iterator sayesinde
* sıralanacak iki range yeni bir range yerine standart çıkış akımına yazdırıldı.
* 
* 
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include "nutility.h"
#include <iterator>

int main()
{
    using namespace std;

    vector<int> x;
    vector<int> y;

    rfill(x, 10, Irand(0, 99));
    rfill(y, 8, Irand(0, 99));
    sort(x.begin(), x.end());
    sort(y.begin(), y.end());

    merge(x.begin(), x.end(), y.begin(), y.end(), ostream_iterator<int>{cout, " "});
}

This C++ program generates two random sorted vectors and merges them directly to std::cout using
std::merge and ostream_iterator, showing the combined sorted sequence.

*/

/*
*  Not: Hiçbir STL algoritması exception throw etmez. Bunun nedeni run time da performance overhead eklememektir.
*/

/*
*  Merge'in bir diğer hali inplace_merge'dir. Bu algoritma yerinde merge ediyor. Yani bir range aynı
*  range de merge ediyor. Karmşıklığı O(n log n) dir.
* 
* 
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> vec = {2, 5, 8, 11, 1, 3, 9, 10};

    // First half:   {2, 5, 8, 11}
    // Second half:  {1, 3, 9, 10}
    // Both are sorted, now we merge them in place

    std::inplace_merge(vec.begin(), vec.begin() + 4, vec.end());

    for (int n : vec)
        std::cout << n << ' ';
}

*/

/*
*   Birde sorted range set fonksiyonlarına bakalım. Bu algoritmalar ile kümelerin kesişimlerine, birleşimlerine,
*   farkına, simetrik farkına vs bakıyoruz.

kesişimi       A ∩ B
birleşimi      A ∪ B
farkı          A - B    A'da olan B'de olmayanlar
simetrik farkı (A - B) ∪ (B - A)

set_intersection
set_union
set_difference
set_symmetric_difference

*
*  Bu algoritmaların hepsinin karmaşılığı O(n) ve bu algoritmalar da sorted range üzerinde çalışıyor.
*  Ayrıca hepsinin parametrik yapılarıda aynı. Aşağıda bu fonksiyonlar ile ilgili örnekler yer almaktadır.
* 
int main()
{
    using namespace std;

    vector<int> x;
    vector<int> y;

    rfill(x, 10, Irand{ 0, 20 });
    rfill(y, 10, Irand{ 0, 20 });
    sort(x.begin(), x.end());
    sort(y.begin(), y.end());

    cprint(x);
    cprint(y);

    vector<int> dest;

    //set_intersection(x.begin(), x.end(), y.begin(), y.end(), back_inserter(dest));
    //set_union(x.begin(), x.end(), y.begin(), y.end(), back_inserter(dest));
    //set_difference(x.begin(), x.end(), y.begin(), y.end(), back_inserter(dest));
    set_symmetric_difference(x.begin(), x.end(), y.begin(), y.end(), back_inserter(dest));

    cprint(dest);
}

*/

/*
*  Diğer bir önemli grupta minimum maximim algoritmalarıdır. Bu algoritmaların sayısı az ama
*  overload sayısı fazladır.
*  
*  Aşağıdaki örnekte referans dönmektedir min ve max algoritmalarında. Ayrıca iki arguman da eşitse birinci argumana
*  fonksiyon referans dönmektedir. Bu dil tarafından verilen bir garantidir. 
* 
int main()
{
    using namespace std;

    int x = 5, y = 9;

    cout << min(x, y) << '\n';
    cout << max(x, y) << '\n';
}

*
*  Ayrıca bu fonksiyonların calleable alan overloadları da vardır. Aşağıdaki örnek bunu göstermektedir.
* 
int main()
{
    using namespace std;

    string s1{ "muhittin" }, s2{ "tan" };

    std::cout << min(s1, s2) << '\n';

    const auto f = [](const string& s1, const string& s2) {
        return s1.size() < s2.size();
    };

    std::cout << min(s1, s2, f) << '\n';
}
*
*   Ayrıca bu min ve max algoritmalarının initializer list parametreli overloadları da dile eklendi. Bu sayede biz
*   2 den fazla nesneyide karşılaştırabiliriz bu algoritmalar ile. Aşağıda bunun bir örneği verilmiştir.
* 
template <typename ...Args>
auto getmin(Args && ...args)
{
    return std::min({ args... });
}

int main()
{
    using namespace std;

    string s1, s2, s3, s4;

    cout << getmin(3, 6, 8, 2, 9, 5, 5) << '\n';
}

This code defines a variadic template function getmin that takes any number of arguments and returns the minimum value 
using std::min. In main(), it prints the smallest value from the given list of integers: 2.
*/

/*
*   Ayrıca min_element ve max_element algoritmaları da vardır. Bu iki algoritma bir range alır ve o rangede ki
*   en küçük ve en büyük nesnenin konumunu iterator ile dönerler. Aşağıda ki örnek bunu göstermektedir.
* 

int main()
{
    using namespace std;

    vector<int> ivec;
    rfill(ivec, 100, Irand{ 0, 999 });
    cprint(ivec);

    auto min_iter = min_element(begin(ivec), end(ivec));
    auto max_iter = max_element(begin(ivec), end(ivec));

    cout << "min = " << *min_iter << " " << min_iter - begin(ivec) << " indeksli eleman\n";
    cout << "max = " << *max_iter << " " << max_iter - begin(ivec) << " indeksli eleman\n";
}

This program fills a vector with 100 random integers between 0 and 999, finds the minimum and maximum 
values using std::min_element and std::max_element, and prints both the values and their indices in the vector.

*/

/*
*  Hem min hem de max'i bulmak için de istersek minmax algoritmasını kullanabiliriz. minmax geri dönüş
*  olarak pair'in iterator açılımını döndürmektedir. First'i min elementin konumunu gösterirken, second'ı
*  ise max elementin konumunu göstermektedir. Aşağıdaki örnek bunun için yazılmıştır ve structural bindingden
*  faydalanılmıştır.

int main()
{
    using namespace std;

    vector<int> ivec;
    rfill(ivec, 10, Irand{ 0, 999 });
    cprint(ivec);

    auto [min_iter, max_iter] = minmax_element(ivec.begin(), ivec.end());
}

This program fills a vector with 10 random integers between 0 and 999, prints them, and uses std::minmax_element to 
efficiently find both the minimum and maximum elements in a single pass. The result is unpacked into
min_iter and max_iter using structured bindings.

*/

/*
*  Not: iter_swap algoritması/fonksiyonu iteratorlerin konumundaki nesneleri swap ediyordu, iteratorleri değil. 
*/

/*
*  Derste son olarak permutasyon algoritmalarına bakıldı. Bunlardan en yaygını is_permutation algoritmasıdır. Bu
*  algoritma iki range alır ve ikinci range birinci range^in permutasyonu mu diye bakar. Eğer öyle ise true değer dönmektedir.
*  En popüler mülakat sorularından birisi anagramdır. Bir text yazı tersten de düzden de aynı ise buna anagram denmektedir.
*  C++ programcıları bu soruya aşağıdaki kodu yazar:)
* 
* 
int main()
{
    using namespace std;

    string s1, s2;
    std::cout << "iki kelime girin: ";
    cin >> s1 >> s2;

    cout << boolalpha << is_permutation(s1.begin(), s1.end(), s2.begin()) << '\n';
}

This program takes two words as input and checks 
if one is a permutation of the other using std::is_permutation, then prints true or false accordingly.

*/

/*
*  next_permutation algoritması da rangede ki bütün permutasyonları geri döndürür. En son döndüreceği
*  permutasyon kalmaz ise false döndürür. Aşağıdaki kodda örnek olarak gösterilmiştir.
* 

int main()
{
    using namespace std;

    vector vec{ 1, 7, 9 };
    int cnt{};
    do {
        cprint(vec);
        ++cnt;
    } while (next_permutation(vec.begin(), vec.end()));
}
This code uses std::next_permutation to generate and print all 
lexicographically ordered permutations of the vector {1, 7, 9} and counts how many permutations exist.
*/

/*
*  Containerların default karşılaştırma kriteri lexicographical compare dir. Peki bu nedir? Bu iki tane veri yapısının 
*  hemn eleman sayısı hem karşılıklı elemanları eşit olacak. Strcmp de mesela C dilinde böyle çalışıyor. Bu sayede set’in
*  vector açılımı olabilir ve vectorler böyle sıralanabilir. Peki containerlar farklı ise? Bu zaman bu karşılaştırmayı yapamam.
*  Fakat bunu yapmak için lexicographical_compare algoritması vardır. Yaptığı şey de birinci range ikinci range den küçük mü? 
*  Bunun cevabını alıyor. 
* 
using namespace std;

string s1{ "can" }, s2{ "caner" };

cout << boolalpha << (s1 < s2) << '\n';
cout << boolalpha << lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end()) << '\n';

*/

/*
*  Accumulate algoritmasından bahsedildi. Bir range ve init değeri alır. Rangede ki ögeleri sırasıyla init değeri ile beraber
*  toplayarak bir işlem yapar ve sonucu döner. Ayrıca 4. argumana calleable alan bir overloadı vardır. Bu calleable'a
*  sırasıyla rangede ki ögeleri ve initi gönderir.
* 
int main()
{
    using namespace std;

    vector<string> svec;

    rfill(svec, 10, random_name);
    cprint(svec);

    auto result = accumulate(svec.begin(), svec.end(), string{});
    reverse(result.begin(), result.end());

    cout << result;
}
This program fills a vector<string> with 10 randomly generated names, concatenates them using std::accumulate,
then reverses the resulting string and prints it. It's a creative way to generate and display a reversed concatenation 
of multiple strings.

int main()
{
    using namespace std;

    vector<int> x{ 2, 3, 7, 1, 8, 2, 2 };

    accumulate(x.begin(), x.end(), 1, multiplies{});
}

This code calculates the product of all elements in the vector x using std::accumulate with the multiplies 
binary function and an initial value of 1. However, the result is not stored or printed—so to make it useful, 
it should be assigned to a variable or output with cout.

int main()
{
    using namespace std;

    vector<int> x{ 1, 1, 2, 2, 2 };

    cout << accumulate(x.begin(), x.end(), 1, [](int a, int b) {
        return a * a + b * b;
    }) << '\n';
}

This code uses std::accumulate with a custom lambda function that accumulates the sum of squares in a non-standard way:
a * a + b * b at each step, where a is the running total. It demonstrates how custom operations in accumulate can lead 
to non-intuitive results.

*/

/*
*  Necati hoca yeni konu olan spaceship, resmi adıyla three way comparison operatorune geçiş yaptı. Bu operator
*  gösterim olarak uzay gemisine benzediği için spaceship olarak da isimlendirili, gösterimi şöyledir: <=>
*  
*  Bu operator C++ 20 ile hayatımıza girmiştir ve önceliği <, >, >=, <= operatorlerinde daha yüksektir. Bu operator
*  bize karşılaştırmanın kesin sonucunu vermektedir. Bunu verirken ürettiği değer bool değil bir türdür. 3 farklı
*  türden bir değer dönebilir ve bu türler compare başlık dosyasında bildirilmiştir. Bu türler sırasıyla
*  strong_ordering, weak_ordering ve partial_ordering'dir. Spaceship operatorunu kullanacak isek bu başlık dosyasını da include
*  etmemiz gerekmektedir. Bu operatorun en yaygın kullanım senaryosu sınıf için operator fonksiyon olarak
*  yazılmasıdır. Yani biz bu operatoru global alanda vs de kullanabilsek de bu yaygın bir konvesyon değildir. Yani
*  cliend code bu operatoru cok kullanmıyor.
* 
*  C++20 ile beraber karşılaştırma operatorleri iki gruba ayrılmaya başlandı; primary ve secondary. Primary operatorler
*  spaceship ve == operatorleridir. Kalan karşılaştırma operatorleri ise secondary gruptadır. Primary operator grubunda 
*  olan spaceship ve == operatorlerinin operator fonksiyonları artık sınıf içerisinde default bildirilebiliyor ve
*  derleyici tıpkı special member functionlar gibi bu default bildirimlerin kodunu yazıyor. 
* 
*  Spaceship operatorunun üreteceği değer comparison başlık dosyasında bulunan 3 türden biridir ve hangi
*  türü üreteceği operandların türüne bağlıdır. Mesela operandlar int ise bu tür strong ordering iken operandlar
*  double iken bu tür partial orderingdir. Bu türlerin sabitleri vardır, bu sabitler equal, equivalent, greater ve less.  
*  Eğer karşılaştırma da 3 tane olası değer var ise, küçük, büyük eşit ve karşılaştırmada bunlardan biri elde edileceği
*  garanti ise bu operator sttong ordering türden döndürüyor, mesela int lerin, stringlerin karşılaştırılması.
* 
*  Bazı türlerin karşılaştırılmasında iki deper eşdeğer olmamasına ragmen eşdeğerce Kabul edilebiliyor. Burda  equivalent
*  kavramı var işte equal yerine, eşit değil ama eşit sonucu üretiliyor. Burada weak_ordering üretecek tür olarak. 
*  Mesela ahmet AHMET karşılaştırılması. Key sensitive olmayan bir sınıf olsaydı işte weak_ordering üretilecekti.
* 
*  NAN: Not a number. C de var. Nedir bu. Partial orderingi anlatırken bu örnek verildi. Yani bir karşılaştırma 
*  tam olarak yapıalamaz. Karşılaştırma büyük, küçük, eşittir gibi yapılamıyorsa burada partial_ordering türü üretilir 
*  spaceship operatöründe. Mesela double bir sayı ile NAN karşılaştırılması. 

Aşağıda partial_orderinig unordered değeri olacak üretilen değer.

int main()
{
    using namespace std;

    float f = 3.4f;
    boolalpha(cout);

    auto result = f <=> NAN;

    cout << boolalpha << (result == partial_ordering::unordered);
}

This code uses the three-way comparison operator (<=>) to compare a floating-point number (f) with NAN (Not A Number). 
The result is checked for being unordered (since NAN cannot be ordered), and it prints true or false accordingly. The output 
will be true because comparing NAN with any value, including NAN itself, results in an unordered comparison.
*/

/*  
*   Strong_ordering sabitleri 0 ile karşılaştırma işlemlerine sokulabiliyor. Aşağıda örneği verilmiştir.
* 
int main()
{
    using namespace std;

    cout << (10 <=> 5) > 0 << '\n';
}
In this example, the three-way comparison operator (<=>) is used to compare 10 and 5. Since 10 is greater than 5, 
the result is strong_ordering::greater (not partial_ordering). The expression (10 <=> 5) > 0 checks if the comparison 
result is greater than zero, which is true because strong_ordering::greater is considered as greater than zero. Therefore,
the output will be 1 (true).

Strong_ordering::greate > 0 doğru
Strong_ordering::less < 0 doğru
Strong_ordering::equal == 0 doğru

*/

/*
*  Secondary operatorlerle ilgili en önemli nokta şudur. Bu operatorler rewritable'dır. Yani örneğin derleyici 
*  iki sınıf nesnesini < operatoru ile karşılaştırıldığı bir ifade görürse ve bu sınıflar için operator< fonksiyonu
*  mevcut değilse derleyici bu çağrıyı x <=> y < 0 ifadesine dönüştürüyor. Dolayısı ile secondary operatorler writable
*  oluyor.

int main()
{
    using namespace std;

    // rewritable
    // x < y
    // x <=> y < 0
}

*   Eşit değil operatorunun de == primary operarator çağrısına dönüştürüyor compiler artık C++ 20 ile aşağıdaki gibi.
// x != y
// !(x == y)

*   Mesela aşağıdaki kod C++20 den önce syntax hatasıydı. 

class Nec {
public:
    bool operator==(const Nec&) const;
};

int main() {
    using namespace std;
    Nec x, y;
    x != y;
}
*/