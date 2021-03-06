# C++11
### Nouveautés et fonctionnalités

- [nullptr](#nullptr)
- [auto](#auto)
- [decltype](#decltype)
- [Initialisation et construction](#init)
  - [Initialisation d'attributs](#attr_init)
  - [Appel d'un constructeur depuis un autre](#constructor_call)
  - [Listes d'initialisation](#initializer_list)
- [Lamda expressions](#lambda)
- [Nouvelles collections](#collections)
  - [Unordered associative](#unordered)
  - [Tuple](#tuple)
  - [Array](#array)
  - [Forward list](#forward_list)
- [Fonctions begin et end](#begin_end)
- [Héritage de constructeurs](#constructors)
- [Override](#override)
- [Final](#final)
- [Bug de chevrons](#chevrons_fix)
- [Boucle for sur un intervalle](#for)
- [Énumérations fortement typées](#enums)
- [Pointeurs intelligents](#smart_pointers)
  - [shared_ptr](#shared_ptr)
  - [weak_ptr](#weak_ptr)
  - [unique_ptr](#unique_ptr)
- [Templates variadiques](#variadic_templates)
- [constexpr](#constexpr)

---

#### nullptr <a id="nullptr"></a>

C++11 introduit une nouvelle valeur `nullptr`, spécialement prévue pour les pointeurs nuls. Cette valeur est de type `std::nullptr_t`. En effet, l'ancienne valeur `NULL` n'étant qu'un alias sur la valeur 0, celle-ci pouvait entrainer des confusions pour le compilateur, comme le montre l'exemple suivant.

```cpp
bool is_even(int i)
{
    return (i % 2 == 0);
}

bool is_even(int *i)
{
    if (i)
        return (*i % 2 == 0);
    
    return false;
}

int main()
{
    int i = 2;
    int *pi = new int(3);
    
    std::cout << std::boolalpha;
    std::cout << is_even(i) << std::endl;           // is_even(int)   OK
    std::cout << is_even(pi) << std::endl;          // is_even(int*)  OK
    std::cout << is_even(NULL) << std::endl;        // is_even(int)   Non souhaité
    std::cout << is_even(nullptr) << std::endl;     // is_even(int*)  OK
}
```

Ici, les deux premiers appels à la fonction `is_even` appellent chacun leur version respective. Lorsque l'on envoie `NULL` à la fonction, c'est la version prenant en paramètre un entier qui est appelée puisque celle-ci vaut 0. Or ici, elle peut représenter un pointeur, créant ainsi une ambiguïté. L'utilisation de la valeur `nullptr` entraine ici le bon appel de fonction.

Ce nouveau type doit donc être utilisé pour toute affectation de pointeur.

---

#### auto <a id="auto"></a>

Le mot-clé `auto`, lorsqu'il est utilisé à la place d'un type de variable, permet au compilateur de déduire ce type à la compilation. Celui-ci sera donc principalement utilisé lors de la création de variables. Pour cela il faut initialiser la variable lors de sa création. C'est à l'aide de la valeur qu'elle reçoit que le compilateur va pouvoir en déduire son type.

```cpp
auto i = 5;             // int
const auto *j = &i;     // const int*
const auto k = 2;       // const int
auto l = k;             // int

static auto y = 0.0;    // double

auto n = 1, s = "str";  // Erreur !!
```

Ici l'ensemble des types sont déduits des valeurs reçues. Par exemple, `i` sera de type entier puisque 5 lui est affecté, et `j` pointant sur ce dernier, il sera un pointeur sur entier.
La dernière ligne ne compile pas, puisque des deux variables `n` et `s` sont déduits deux types 
A noter que comme le montre la variable `l`, le mot-clé `auto` ne conserve pas les qualificateurs de type comme `const`, `&` ou `&&`, contrairement à [`decltype(auto)`](https://github.com/Manouel/Cxx-14#decltype_auto) (C++ 14).

Il est également possible d'utiliser le mot-clé `auto` pour indiquer le type de retour d'une fonction. Pour cela, il faut préciser le type de retour avec la syntaxe `-> type` en indiquant directement le type, ou en utilisant la notation `decltype`, afin d'indiquer au compilateur le type qu'il doit déduire de `auto`.

```cpp
auto f() -> int;
auto g() -> decltype(i) { return 0; }
auto h() -> decltype(f());
```

Ici, les retours des trois fonctions seront de type `int`.

---

#### decltype <a id="decltype"></a>

Tout comme `auto`, le mot-clé `decltype` permet de déterminer un type, à la différence qu'au lieu d'utiliser l'expression affectée, il va utiliser l'expression qui lui est fournie.

```
decltype(EXPRESSION) IDENTIFIANT = VALEUR;
```

De plus, contrairement à `auto`, `decltype` conserve les modificateurs de type comme `const`. 

```cpp
const int i = 1;        // const int
auto j = 2;             // int

decltype(i) x = 3;      // const int
decltype(j) y = 4;      // int

i = 11;                 // erreur
j = 12;
x = 13;                 // erreur
y = 14;
```

Dans cet exemple, la variable `j` sera de type `int`, car `auto` ne conserve pas le `const`. En revanche, `x` sera bien de type `const int`, car déclaré avec `decltype`.

Mis à part cela, le principal intérêt de `decltype` par rapport à `auto` est lorsque l'on souhaite utiliser le type d'une variable déclarée `auto` (car seul le compilateur connait son type), ou bien avec des templates.

Comme vu précédemment, `decltype` va également pouvoir permettre de renseigner le type de retour d'une fonction déclaré comme `auto`.

---

#### Initialisation et construction <a id="init"></a>

##### Initialisation d'attributs <a id="attr_init"></a>

Parmi les nouveautés apportées, nous avons tout d'abord l'initialisation des attributs d'instance lors de leur déclaration dans la classe. Ceci permet donc d'initialiser les attributs avec une valeur par défaut. Dans le cas où les attributs ne sont pas initialisés dans le constructeur, ceux-ci disposeront quand même d'une valeur cohérente.

```cpp
class A
{
private:
    int m_intVal = 5;
    std::string m_stringVal {"value"};

public:
    A() {}
    A(int val) : m_intVal(val) {}
};
```

##### Appel d'un constructeur depuis un autre <a id="constructor_call"></a>

Un constructeur d'une classe peut désormais appeler un autre constructeur de la même classe. Ceci se révèle donc pratique lorsque plusieurs constructeurs effectuent des actions communes. Cela va permettre d'éviter la répétition d'instructions ou bien un appel à une fonction regroupant ces instructions par chaque constructeur.
Ou bien comme dans l'exemple présenté, cela peut etre utile afin que le constructeur vide appelle un autre constructeur avec des valeurs par défaut.

```cpp
class A
{
private:
    int nb;

public:
    A(int n) : nb(n) {}
    A() : A(42) {}
};
```

##### Listes d'initialisation <a id="initializer_list"></a>

Il est également possible d'initialiser un objet par une liste d'initialisation, comme avec les tableaux. Ceci va donc permettre d'initialiser les objets de la classe vector par exemple, en le remplissant directement, ou alors également des instances de classes en appelant le constructeur correspondant.

```cpp
std::vector<std::string> vect {"alpha", "beta", "gamma"};
std::vector<int> vect2 = {1, 2, 3};
std::map<int, std::string> m {{1, "1"}, {2, "2"}};
double *doubles = new double[3] {0.5, 1.2, 12.99};

Paire p1 {};         // Constructeur vide (= Paire p1;)
Paire p2 {12, 20};   // Appel du constructeur Paire(int, int)
```

Il existe un nouveau type représentant ces listes : `std::initializer_list` définie dans le header `<initializer_list>`. Cela permet par exemple de prendre une liste d'initialisation en paramètre de fonction. Il est possible d'itérer sur ces listes de la même manière que les conteneurs.

```cpp
#include <initializer_list>

void processList(std::initializer_list<int> list)
{
    for (std::initializer_list<int>::iterator it(list.begin()) ; it != list.end() ; ++it)
    {
        // ...
    }
}

int main()
{
    processList({1, 2, 3});
    
    return 0;
}
```

---

#### Lamda expressions <a id="lambda"></a>

Une lambda expression permet de définir une fonction anonyme. Cela permet d'éviter d'avoir à déclarer une fonction dans une classe ou un fichier.
Pour les fonctions comme for_each qui prennent en paramètre un pointeur sur une fonction, il est donc possible de définir celle-ci directement au moment de l'appel à for_each.
On y indique les  variables capturées, c'est à dire les variables définies en dehors de la fonction lambda et que l'on veut utiliser, les paramètres de la fonction, éventuellement le type de retour qui n'est pas obligatoire car peut être déduit, et enfin le corps de la fonction lambda.

```cpp
[ capture ] ( params ) -> ret { body }
```

---

#### Nouvelles collections <a id="collections"></a>

##### Unordered associative <a id="unordered"></a>

Il existe maintenant les collections unordered pour les ensembles et conteneurs associatifs fonctionnant sur un principe clé/valeur. Cela correspond donc aux classes  `map`, `multimap`, `set` et `multiset`. Ces nouvelles collections permettent d'éviter un tri inutile des éléments ou de définir notre propose fonction de hachage utilisée pour stocker les éléments. Cela permet donc de définir notre propre stockage et notre propre complexité lors de l'utilisation du conteneur.

```cpp
std::unordered_set<std::string> uset;
std::unordered_multiset<int> umset;

std::unordered_map<int, std::string> umap;
std::unordered_multimap<int, std::string> ummap;
```

##### Tuple <a id="tuple"></a>

Ce nouveau conteneur permet de créer une collection de taille fixe avec des types différents. On peut l'utiliser comme le type `pair` à la différence que celui-ci ne se limite pas à 2 éléments. On peut donc créer un ensemble comportant une liste de types prédéfinis. Il existe également une fonction `make_tuple` prenant en paramètre les valeurs du tuple.

```cpp
std::tuple<std::string, int, bool> loutre("loulou", 10, true);
auto t = std::make_tuple(1, 1.0, 'a', "hello"s);
```

Ici il n'existe pas de membres `first` ou `second` comme sur les paires, mais une fonction template `get` prenant en paramètre l'indice de la valeur. Il y a aussi une fonction `tuple_size` prenant en paramètre (template) le type du tuple et permettant d'accéder à la taille par le membre `value`. 

```cpp
std::cout << "Nom : " << std::get<0>(loutre) << std::endl;
std::cout << "Nombre de propriétés : " << std::tuple_size<decltype(loutre)>::value << std::endl;
```

A noter qu'il existe maintenant une fonction `tie` permettant de récupérer le résultat d'un tuple ou d'une paire dans des variables existantes, avec la possibilité d'utiliser `std::ignore` pour ignorer des valeurs.

```cpp
std::string name;
int age;
std::tie(name, age, std::ignore) = loutre;
std::tie(name, age, std::ignore) = std::make_tuple("loulou", 10, true);
```

##### Array <a id="array"></a>

Le type `array` définit dans la STL un conteneur représentant les tableaux statiques du langage. On peut alors définir ces tableaux de manière à ce qu'ils comportent toutes les propriétés des conteneurs de la STL.

```cpp
std::array<int, 4> tab = {1, 5, 8, 9};
std::array<int, 4>::iterator it;

for (it = tab.begin(); it != tab.end(); ++it)
{
    std::cout << *it << " ";
}
std::cout << std::endl;
```

##### Forward list <a id="forward_list"></a>

Il est maintenant possible avec `forward_list` de créer des listes simplements chaînées (contrairement aux `list` qui sont doublement chaînées). Chaque élément ne possédant ici qu'un pointeur vers l'élément suivant, il n'est pas possible d'effectuer un parcours en sens inverse ou de revenir en arrière avec les itérateurs.

```cpp
std::forward_list<int> l { 1, 2, 3 };
```

---

#### Fonctions begin et end <a id="begin_end"></a>

Il existe maintenant, en plus des méthodes `begin` et `end` des conteneurs retournant respectivement un itérateur sur le début et la fin du conteneur, des fonctions `begin` et `end`.

Il est donc possible de surcharger les fonctions afin de permettre de les utiliser avec n'importe quel type de conteneur, même ceux qui ne contiennent pas les méthodes,  ou même avec les tableaux.

```cpp
std::vector<int>::iterator it;
for (it = begin(v); it != end(v); it++)
{
    // ...
}
```

---

#### Héritage de constructeurs <a id="constructors"></a>

On peut maintenant en C++11, hériter des constructeurs d'une super classe à l'aide du mot-clé `using`.

Dans l'exemple présenté, les deux objets feront appel au constructeur de `B`. Dans la classe `D`, le mot `using` indique que la classe peut utiliser le constructeur de `B` pour les instances de `D` qui sont créées. Cela fonctionne avec les objets alloués automatiquement ou dynamiquement.

En revanche, l'héritage de constructeurs ne peut être utilisé que si l'héritage entre les classes n'est pas virtuel.

```cpp
class D : public B
{
    public:
        using B::B;
};
```

---

#### Override <a id="override"></a>

Le mot-clé `override` permet d'imposer au compilateur qu'une fonction doit hériter d'une fonction virtuelle d'une classe mère.
Cela permet donc, en cas d'erreur dans la signature, de générer une erreur de compilation indiquant que la méthode signalée `override` n'hérite d'aucune autre méthode.

C'est le cas dans l'exemple suivant, avec les méthodes `foo(int) const` et `foo(string)` de la classe `B` déclarées `override` et qui doivent donc hériter d'une méthode et la redéfinir. Or dans la classe `B`, la deuxième méthode `foo` est déclarée constante contrairement à celle de la classe mère, et la troisième prend en paramètre un `string` au lieu d'un `int`. Le programme ne compile donc pas.

En revanche, la méthode `foo(int)` de la classe `B` redéfinit bien celle de la classe `A`.
A noter que la méthode de la classe mère doit bien être déclarée `virtual`.

```cpp
class A 
{
	public:
	
		virtual void foo(int i)
		{
			std::cout << "A::foo(int) " << i << std::endl;
		}
};

class B : public A
{
	public:

		virtual void foo(int i) override
		{
			std::cout << "B::foo(int) " << i << std::endl;
		}
		
		virtual void foo(int) const override;	    // Ne compile pas
		virtual void foo(std::string s) override;   // Ne compile pas
};
```

---

#### Final <a id="final"></a>

Tout comme le mot-clé `override` utilisé pour indiquer qu'une méthode doit hériter d'une méthode de la classe parente, le mot-clé `final` permet d'indiquer qu'une classe ou une méthode virtuelle n'est pas dérivable.
Dans l'exemple, la classe `B` ne peut pas redéfinir la méthode `foo(int)` déclarée `final` dans la classe `A`. En revanche, la méthode `foo(string)` ne la redéfinissant pas, il n'y a pas d'erreur.

De la même manière que pour les méthodes, il est possible d'annoter une classe comme `final`, afin qu'elle ne puisse pas être spécialisée. C'est le cas de la classe `B`, et c'est pourquoi la création de la classe `C` produit une erreur de compilation.

```cpp
class A 
{
	public:
	
		virtual void foo(int i) final;  
        void bar() final;                   // Fonction non virtuelle, ne peut pas être final
	
};

class B final : public A
{
	public:

		virtual void foo(int i);            // Pas possible
		virtual void foo(std::string s);
};

class C : public B                          // Ne peut pas spécialiser B déclaré final
{

};
```

---

#### Bug de chevrons <a id="chevrons_fix"></a>

Lors de la création de conteneurs de conteneurs, comme dans l'exemple qui suit, il fallait espacer les chevrons fermants afin que le compilateur ne les interprète pas comme un opérateur de flux `>>`. Il n'est maintenant plus nécessaire de mettre l'espace, et le compilateur fait la différence avec l'opérateur.

```cpp
std::map<int, std::vector<int> >  // Avant
std::map<int, std::vector<int>>	  // C++11
```
 
---

#### Boucle for sur un intervalle <a id="for"></a>

Il est désormais possible d'utiliser la boucle for sur un intervalle à la manière de `for_each` qui existait sur les conteneurs.

```cpp
std::vector<int> elements = {0, 1, 2, 3, 4, 5};

// Par valeur
for (int e : elements)
    std::cout << e << ' ';
std::cout << '\n';

for (auto e : elements)
    std::cout << e << ' ';
std::cout << '\n';

// Par référence
for (int& e : elements)
    ++e;

for (const int& e : elements)
    std::cout << e << ' ';
std::cout << '\n';

// Sur une liste d'initialisation
for (int e : {0, 1, 2, 3, 4, 5})
    std::cout << e << ' ';
std::cout << '\n';

// Sur les tableaux
int elements[] = {0, 1, 2, 3, 4, 5};
for (int e : elements)
    std::cout << e << ' ';
std::cout << '\n';
```

---

#### Énumérations fortement typées <a id="enums"></a>

Il existe maintenant des énumérations fortement typées, identifiées par `enum class`. Contrairement aux énumérations classiques où les valeurs sont définies dans la portée globale, dans les `enum class` les valeurs sont dans la portée de l'énumération. C'est pourquoi contrairement aux énumérations classiques, l'accès aux valeurs se fait par le nom de l'énumération suivi par l'opérateur de portée `::` (`Enum::Value`).
Cela permet maintenant d'avoir deux valeurs avec le même identifiant dans la même portée.

```cpp
/** enum **/

enum E1 { V1 };
enum E2 { V1 };		// Erreur : redéfinition de V1
int V1;				// Erreur : redéfinition de V1

/** enum class **/

enum class E1 { V1 };
enum class E2 { V1 };
int V1;

E2 e2 = E2::V1;
```

De plus, avec les énumérations classiques les valeurs étaient implicitement converties en entier (lors d'un affichage par exemple), alors que les énumérations fortement typées nécessitent un cast explicite, et évitent donc les opérations qui n'ont pas de sens.

```cpp
enum class Direction { Haut, Droite, Bas, Gauche };

Direction dir = Direction::Haut;
std::cout << dir << std::endl;  // Ne compile plus car pas de cast explcite ! (alors que c'est le cas avec les enum)
std::cout << static_cast<int>(dir) << std::endl;

Direction dir2 = dir + 1;       // N'a pas de sens, ne compile plus.
```

Il est aussi possible de spécifier le type des valeurs de l'énumération, et qui par défaut est `int`. Celui-ci peut être renseigné après le nom de l'énumération, comme présenté dans l'exemple suivant. Ici par exemple, on peut choisir de stocker des `char`, car plus légers que des entiers.

```cpp
enum class A : char { V1, V2 = 'b', V3 };

std::cout << static_cast<int>(A::V1) << std::endl;	// 0
std::cout << static_cast<char>(A::V2) << std::endl;	// b
std::cout << static_cast<char>(A::V3) << std::endl;	// c
```

Concernant l'attribution de valeurs, celle-ci ne change pas, et les valeurs sont bien incrémentées pour les éléments suivants.

Enfin, tout comme les classes, les énumérations fortement typées permettent les déclarations anticipées.

```cpp
enum class E;
void foo(E e);
enum class E { V1, V2, V3 };
```

---

#### Pointeurs intelligents <a id="smart_pointers"></a>

Les pointeurs intelligents ont pour but de pallier les problèmes des pointeurs classiques (pointeurs nus), qui doivent être systématiquement désalloués manuellement d'une part, et qui dans certains cas peuvent même entrainer les problèmes suivants.

```cpp
// Cas 1 (fuite mémoire)

int foo()
{
    int *i = new int(42);
    bar();			// Si bar lève une exception, delete non appelé.
    delete i;
}

// Cas 2 (fuite mémoire)

class A
{
private:
    int *i;
    int *j;

public:
    /* Si le new int(42) échoue par manque de mémoire, une exception est levée. 
       L'objet A n'est alors pas considéré comme construit, et son destructeur ne sera pas appelé.
       La mémoire allouée pour i ne sera donc pas libérée. */
    A() : i(new int(314)), j(new int(42)) {}
    ~A()
    {
        delete j;
        delete i;
    }
};

// Cas 3 (déréférencement de pointeur invalide)

int *i = new int(42);
int *j = i;
delete i;
*j = 2;  	// Erreur

```

Les problèmes sont généralement des fuites mémoires comme présentés ci-dessus, des désallocations sur des zones mémoires déjà libérées/non allouées, ou alors des accès mémoire sur des zones libérées par d'autres pointeurs (cas 3).
De plus, les fonctions retourant des pointeurs sur des objets ne sont pas explicites, et il est difficile de savoir si c'est à l'utilisateur de libérer l'objet ou non.

```cpp
MyStruct* getStruct();
```

Les pointeurs intelligents présentent une solution à ces problèmes. Ils encapsulent des pointeurs, et gèrent la durée de vie des objets pointés.

##### shared_ptr <a id="shared_ptr"></a>

Le premier type de pointeur intelligent est `shared_ptr`, qui permet à plusieurs pointeurs d'accéder à la donnée partagée. Il y a un compteur de référence sur l'objet pointé, incrémenté lorsqu'un nouveau pointeur pointe sur l'objet, et décrémenté lorsqu'un pointeur arrête de pointer sur l'objet (pointeur détruit ou pointant vers un autre objet). Lorsque ce compteur atteint 0, la mémoire de l'objet pointé est libérée.

###### Utilisation

```cpp
std::shared_ptr<int> ptr1(new int (42));
std::shared_ptr<int> ptr2 = std::make_shared<int>(42);	// A utiliser
```

Pour créer un `shared_ptr`, il suffit donc de lui passer un pointeur nu en paramètre. Cependant, il existe également une fonction `make_shared<>`, dont il faut privilégier l'utilisation car plus sûre et plus performante, et qui permet d'éviter toute présence de pointeur nu dans le code.

```cpp
std::shared_ptr<int> a = std::make_shared<int>(3);
std::cout << a.use_count() << std::endl;				            // 1
std::shared_ptr<int> b = a;
std::cout << a.use_count() << " & " << b.use_count() << std::endl;	// 2 & 2

{
    std::shared_ptr<int> c;
    std::shared_ptr<int> d = std::make_shared<int>(314);
	std::cout << a.use_count() << std::endl;                        // 2
	c = a;
	std::cout << a.use_count() << std::endl;                        // 3
	d = c;                                                          // Objet 314 libéré (compteur 0)
	std::cout << a.use_count() << std::endl;                        // 4
}

// Les pointeurs c et d ont été détruits (portée locale au bloc)

std::cout << a.use_count() << std::endl;                            // 2
b.reset();
std::cout << a.use_count() << std::endl;                            // 1

// Destruction de a, libération de l'objet pointé
```

Il existe une méthode `use_count` qui permet d'afficher le nombre de référence sur l'objet pointé par le `shared_ptr`, principalement utilisé pour du débogage. La méthode `reset` permet au pointeur de ne plus pointer sur rien (construction par défaut).

Ici, c'est donc les destructeurs des pointeurs appelés automatiquement qui vont permettre la suppression des références, et donc la libération automatique de l'objet.

Il existe également des conversions entre `shared_ptr`, avec les fonctions `static_pointer_cast`, `dynamic_pointer_cast` et `const_pointer_cast`, dont les effets sont les mêmes que leurs équivalents `static_cast`, `dynamic_cast` et `const_cast` lorsqu'ils sont appliqués sur des pointeurs nus.

```cpp
class A {/*...*/};
class B : public A {/*...*/};

std::shared_ptr<A> sa(new A);
std::shared_ptr<A> sab(new B);

std::shared_ptr<B> static_sb = std::static_pointer_cast<B>(sa);		// Ok mais comportement indéfini
std::shared_ptr<B> dynamic_sb = std::dynamic_pointer_cast<B>(sa);	// nullptr

std::shared_ptr<B> static_sab = std::static_pointer_cast<B>(sab);	// Ok
std::shared_ptr<B> dynamic_sab = std::dynamic_pointer_cast<B>(sab);	// Ok
```

###### Risques

Les `shared_ptr` peuvent cependant présenter des problèmes, notamment lors de la création de cycles.

```cpp
struct A
{
    std::shared_ptr<B> b; 
};

struct B
{
    std::shared_ptr<A> a;
};

std::shared_ptr<A> a = std::make_shared<A>();
std::shared_ptr<B> b = std::make_shared<B>();

a->b = b;
b->a = a;

a.reset();
b.reset();
```

Dans cet exmple, les zones mémoires allouées pour les objets `A` et `B` sont toutes les deux pointées par le pointeur créé localement, ainsi que par l'attribut de l'autre classe. Une fois les `reset` appelés, les objets sont devenus inaccessibles depuis le main, mais chaque pointeur dans la classe fait survivre l'autre objet en maintenant son compteur de référence à 1. On obtient donc une fuite mémoire. Les [`weak_ptr`](#weak_ptr) présentent une solution à ce problème.

###### Fonction de désallocation

Dans certains cas, l'objet alloué doit être libéré via un appel à une fonction et non en appelant `delete` dessus, par exemple si l'on utilise une bibliothèque qui manipule des pointeurs sur nos objets. Dans ce cas, il est possible de passer en deuxième argument du constructeur du `shared_ptr` la fonction de désallocation à utiliser.

```cpp
// Fonctions à utiliser
A* createA();
void freeA(A *t);

std::shared_ptr<A> ptr(createA(), &freeA);
```

##### weak_ptr <a id="weak_ptr"></a>

Comme expliqué au dessus, les pointeurs `weak_ptr` s'utilisent avec des `shared_ptr` et ont pour but de casser les cycles. Contrairement à un `shared_ptr`, un `weak_ptr` ne modifie pas le comptage de référence de l'objet sur lequel il pointe, et ne possède qu'un simple accès. Cependant contrairement à un pointeur nu, l'utilisateur est averti si l'objet pointé a été désalloué par un `shared_ptr`.

Pour l'utilisation dans les cycles, un membre possèdera un `shared_ptr` et l'autre un `weak_ptr`. Si, comme souvent, il y a une relation d'appartenance entre les deux objets (ex : région/département), alors le contenant aura un `shared_ptr` sur le contenu, car il est chargé de sa libération, et l'autre objet n'aura qu'un `weak_ptr` qui n'influera pas sur la durée de vie de l'objet auquel il appartient.

###### Utilisation

Les deux possibilités de création d'un `weak_ptr` sont à partir d'un `shared_ptr` ou bien d'un autre `weak_ptr`. Ici il n'est pas possible d'en créer à partir d'un pointeur nu.

De plus, un `weak_ptr` ne possède pas d'opérateur `->`. Il faut donc créer un `shared_ptr` à partir du `weak_ptr` pour accéder à l'objet pointé, en utilisant le constructeur `shared_ptr(const weak_ptr<T>&)` ou la méthode `lock` (plus explicite).

Dans l'exemple suivant, on peut voir un cas d'utilisation des `weak_ptr` avec des cycles.

```cpp
class Employee;
class Team
{
    public:
        std::list<std::shared_ptr<Employee>> employees;
        
        std::string getName() const { return "name"; }
};

class Employee
{
    public:
        std::weak_ptr<Team> team;
        
        std::string getTeamName() const
        {
            std::shared_ptr<Team> teamPtr = team.lock(); // ou : shared_ptr<Team> teamPtr(team); 
            
            if (teamPtr)
                return teamPtr->getName();
            else
                return "unknwon";   // Objet pointé détruit
        }
};

int main()
{
    std::shared_ptr<Employee> manu = std::make_shared<Employee>();
    
    {
        std::shared_ptr<Team> pepishado = std::make_shared<Team>();
        
        pepishado->employees.push_back(manu);
        manu->team = pepishado;                 // Affectation du weak_ptr à partir d'un shared_ptr
        
        std::cout << manu->getTeamName() << std::endl;	// name
    }
    
    // Le pointeur pepishado étant détruit, le compteur est décrémenté.
    // Ici il valait 1 car le weak_ptr de manu ne l'a pas incrémenté.
    // L'objet Team pointé est détruit malgré le cycle (au passage il a décrémenté le compteur de manu).
    
    std::cout << manu->getTeamName() << std::endl;		// unknwon
    
    return 0;
}
```

A noter qu'il existe aussi une méthode `bool expired()` permettant de savoir si l'objet pointé par le `weak_ptr` a été détruit ou non.

```cpp
std::weak_ptr team;
if (!team.expired())
    std::cout << "Valid team" << std::endl;
else
    std::cout << "No more team" << std::endl;
```

##### unique_ptr <a id="unique_ptr"></a>

Ce dernier type de pointeur intelligent représente un pointeur simple qui est le seul à pointer sur l'objet alloué. Lorsque le pointeur est détruit, l'objet est détruit. Avec ce type de pointeur la copie est interdite.

Il permet donc d'utiliser le RAII pour libérer automatiquement des objets, en étant plus léger qu'un `shared_ptr` puisqu'il n'a pas de compteur de référence. De plus, il facilite la lecture du code en indiquant qu'il s'agit uniquement d'un pointeur libéré automatiquement.

Ici on utilise un `unqiue_ptr`, pratique aussi pour définir une fonction de désallocation.

```cpp
void *createProcess();
void deleteProcess(void *p);

void foo()
{
    std::unique_ptr<void> process(createProcess(), &deleteProcess);
    
    // process sera automatiquement détruit à la fin de la fonction
}
```

---

#### Templates variadiques <a id="variadic_templates"></a>

Il existe maintenant une nouvelle notation permettant de créer des fonctions ou classes à nombre variables de paramètres. Pour cela, il suffit d'utiliser la syntaxe `...` dans les templates, comme le montre l'exemple qui suit.

```cpp
template<typename... Args>
void foo(Args... args)
{
    // ...
}
```

Ici il est possible d'appeler la fonction avec autant de paramètres que l'on veut, avec les types que l'on veut.
Les templates variadiques sont le plus souvent utilisés de façon récursive, comme le montre l'exemple qui suit.

```cpp
template<typename T>
T adder(T v)
{
	return v;
}

template<typename T, typename... Args>
T adder(T first, Args... args)
{
	return first + adder(args...);
}


std::cout << adder(5, 3, 7) << std::endl;                   // 15

std::string s1 = "h", s2 = "el", s3 = "lo";
std::cout << adder(s1, s2, s3) << std::endl;                // "hello"
```

La condition d'arrêt est isolée dans une fonction dédiée, qui sera appelée lorsqu'il n'y aura qu'un argument à la fonction `adder`. Sinon, le premier élément est additionné au reste, noté `args...`, qui est passé récursivement à la fonction `adder`.

Dans certains cas, il est également possible d'utiliser chaque paramètre un par un, sans récursion.
Ce deuxième exemple présente une fonction convertissant en chaine de caractères chaque paramètre. Tout d'abord une version récursive de cette fonction.

```cpp
template<typename T>
std::string to_string_impl(const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

// Appelé quand args est vide
std::vector<std::string> to_string()
{
	return {};
}

template<typename T, typename... Args>
std::vector<std::string> to_string(const T& t, const Args&... args)
{
	std::vector<std::string> s;
	s.push_back(to_string_impl(t));

	const auto remainder = to_string(args...);				// Récursion
	s.insert(s.end(), remainder.begin(), remainder.end());
	return s;
}
```

Ici, il est donc possible d'écrire cette fonction sans récursion, en utilisant l'expansion `...` sur `args` dans la liste d'initialisation, après l'appel de la fonction.

```cpp
template<typename T>
std::string to_string(const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

template<typename... Args>
std::vector<std::string> to_string(const Args&... args)
{
	return {to_string(args)...};
}
```

Dans cette fonction, chaque élément de `args` est envoyé à la première fonction `to_string` et le résultat est ajouté à la liste pour construire le vecteur résultant.

---

#### constexpr <a id="constexpr"></a>

Un nouveau mot-clé `constexpr` permet en C++11 de déclarer des expressions constantes de compilation. Il peut être utilisé sur des variables ou des fonctions, mais seulement avec des types littéraux. A la différence du mot-clé `const` qui permet simplement d'assurer qu'une variable ne sera pas modifiée, ici la valeur sera calculée à la compilation.

Cette syntaxe permet d'une part de remplacer la syntaxe `define` qui est dépréciée, car ne permet pas de vérification de type, et ne respecte pas les règles de portée. Une variable `constexpr` doit obligatoirement être initialisée.

```cpp
#define buffer_size 	512

// C++11
constexpr int buffer_size = 512;
```

`constexpr` peut aussi être utilisé sur des fonctions qui seront exécutées à la compilation. Une fonction `constexpr` ne peut pas être virtuelle, et son type de retour et le type de ses paramètres doivent être des types littéraux. De plus, celle-ci ne peut pas contenir de déclarations de variable, de conditions ou de boucles.

```cpp
constexpr int facto(int n)
{
	return (n <= 1 ? 1 : n * facto(n - 1));
}

int sum(int a, int b)
{
    return a + b;
}

int i = 3;
constexpr int j = 4;
constexpr int k;                // Erreur : non initialisé
constexpr int l = i + 1;        // Erreur : i non constant

constexpr int m = facto(4);     // 24
constexpr int n = facto(i);     // Erreur : i non constant
constexpr int o = facto(j);     // 24
constexpr int p = sum(2, 3);    // Erreur : sum non constexpr
```

Ici la fonction `facto` sera évaluée et utilisée comme une constante de compilation lors de ses différents appels.

Ce type de fonction permet également leur utilisation à des endroits où le compilateur requiert des valeurs constantes, comme les `switch`, les paramètres templates ou les assertions.

```cpp
constexpr int n = 2;

constexpr int sum(int a, int b)
{
    return a + b;
}

int main()
{
	static_assert(n == sum(1, 1), "");    // OK car n et sum constexpr

	int i = 3;
	switch (i)
	{
		case 0: break;
		case sum(1, 2): break;     // OK car sum constexpr
 	}
}
```

Les constructeurs peuvent également être définis `constexpr` si chacun de leurs paramètres est de type littéral, si la classe associée n'a pas de super-classe virtuelle et si le constructeur ne contient pas de bloc try/catch. 
