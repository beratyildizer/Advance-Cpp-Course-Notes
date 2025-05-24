/*
*  C++ 14 ile generalized lambda expr. yazabiliyor hale geldik. Peki ben iki arguman alan fakat argumanlarının ikisinin de türü T 
*  olan template lambda'yı nasil yazabilirim. Bunu templated lambdalar ile yazabiliriz. 
* 
*  Örneğin aşağıdaki foo fonksiyon şablonu gibi bir şablonu nasıl lambdalar ile yapabilirim? Yani T için tür çıkarımını yapmak 
*  istiyorum, vector için değil.

template <typename T>
void foo(const std::vector<T>& x, const std::vector<T>& y);

int main()
{
    using namespace std;

    vector<int> x;
    vector<int> y;

    foo(x, y)
}

*/

/*
*  İşte bunu templated lambda syntaxı ile yapabiliriz. Bu templated lambdalar ile içerisine type parameter, non type parameter, 
*  template parameter pack vs hepsini yazabiliyoruz. Aşağıda bununla ilgili örnek verilmiştir.

*  Lambda introducerdan sonra bir açısal parantez açarız. Bu açısal parantezin içerisine yukarıda belirttiğimiz gibi type parameter,
*  non type parameter, template parameter pack vs herşey yazılabiliyor. Daha sonra bu template parametresini kullanabiliyoruz.
*  Derste bununla ilgili yazılan kodlar aşağıda verilmiştir.

int main()
{
    auto f = []<typename T>(T x, T y) {
        ///
    };
}

using namespace std;

auto fsum = []<typename T>(vector<T> &x, vector<T> &y) {
    ///
};

int main()
{
    auto f = []<typename T>(const std::vector<T> &x, const std::vector<T> &y) {
        //...
    };

    std::vector<int> vx;
    std::vector<int> vy;

    f(vx, vy);
}

int main()
{
    auto fpush = []<typename T>(std::vector<T> &x, const T &val)
    {
        x.push_back(val);
    };
}

auto fx = []<typename T, std::size_t SIZE>(T(&r)[SIZE]) {

};

/*
*   Necati hoca derste şu soruyu sordu. Öyle bir template yaz ki, birinci template argumanı için çıkarım yapılsın fakat ikinci
*   için yapılmasın ve aynı zamanda iki type aynı olsun. Bunun için type identity'i kullanabiliriz. Örneği 
*   aşağıda gösterilmiştir.

template <typename T>
struct TypeIdentity {
    using type = T;
};

int main()
{
    using namespace std;

    type_identity_t<int>
}

*/

/* 
*  Dersin kalan bölümünde STL algoritmaları işlendi. Bütün algoritmaların üzerinden Necati hoca bir liste üzerinden geçti.
*  Bende derste tek tek not almak yerine o listeyi Necati hocadan istedim. Listeyi aldıkdan sonra bazı eklemeler yapıp 14. ve 15.
*  dersler için o listeyi kullanacağım.
*/