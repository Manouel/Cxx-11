# C++11
### Nouveautés et fonctionnalités

- [nullptr](#nullptr)
- [auto](#auto)
- [decltype](#decltype)
- [Initialisation et construction](#init)
- [Lamda expressions](#lambda)
- [Nouvelles collections](#collections)
  - [Unordered associative](#unordered)
  - [Tuple](#tuple)
  - [Array](#array)
- [Fonctions begin et end](#begin_end)
- [Héritage de constructeurs](#constructors)
- [Override](#override)
- [Final](#final)
- [Ordre d'appel des constructeurs et destructeurs](#linearization)
- [Boucle for sur un intervalle](#for)
- Pointeurs intelligents
- Énumérations fortement typées

---

#### nullptr <a id="nullptr"></a>


```cpp
bool pair_int(int i)
{
  cout << "test pair/impair ";
  return (i%2 == 0);
}

bool pair_int(int *i)
{
  if (!i)
  {
    cout << "pas d'entier à traiter ";
    return true;
  }
  else
    return *i % 2 == 0;
}

int main()
{
  int i = 2;
  cout << pair_int(i) << endl;
  
  int *pi = new int;
  *pi = 3;
  
  cout << pair_int(*pi) << endl;
  cout << pair_int(NULL) << endl;
  cout << pair_int(nullptr) << endl;

  return 0;
}
```

Dans l'exemple, un fichier contient une fonction `pair_int` qui retourne vrai si l'entier qu'elle reçoit est pair. Il y a 2 versions, la première prenant en paramètre l'entier lui-même, la seconde un pointeur sur cet entier.

L.25, c'est la fonction l.5 qui est appelée car on lui envoie la valeur d'un entier `i`. De même lors du 2e appel l.30 car on envoie le contenu pointé par `pi`.

Lors de l'appel l.31, on envoie `NULL`, généralement utilisé pour affecter un pointeur. Seulement ici, c'est un entier qui est reconnu, puisque `NULL` correspond à la valeur 0.

Dans le cas où l'on souhaitait indiquer la valeur d'un pointeur, ça n'est pas la fonction souhaitée qui est appelée, mais la fonction l.5. En revanche, lorsque l'on utilise la valeur nullptr du C++11, de type `std::nullptr_t`, spécialement prévue pour les pointeurs, le programme comprendra bien pointeur nul et la fonction prenant un pointeur en entier sera appelée.

Ce nouveau type et cette nouvelle valeur permettent donc d'éviter les ambiguïtés qu'il pouvait y avoir entre type entier et pointeur, comme dans cet exemple.

---

#### auto <a id="auto"></a>

Le mot-clé `auto`, lorsqu'il est utilisé à la place d'un type de variable, permet au compilateur de déduire ce type à la compilation. Celui-ci sera donc principalement utilisé lors de la création de variables. Pour cela il faut initialiser la variable lors de sa création. C'est à l'aide de la valeur qu'elle reçoit que le compilateur va pouvoir en déduire son type.

```cpp
  /* int */
  auto i = 5;
  
  /* int si x de type int, sinon erreur */
  const auto *v = &x, u = 6;

  /* float */
  static auto y = 0.0;
  
  /* int */
  auto ii = 3;
  
  /* int */
  auto g() -> decltype(ii) { return 0; }
  auto h() -> decltype(ii);
```

Dans l'exemple, la variable `i` sera de type entier, puisqu'elle reçoit un littéral entier valant 5.

Concernant la 2e ligne, celle-ci compilera uniquement si la variable `x` est de type `int`. En effet, de même que lorsque l'on indique un type normal, l'ensemble des variables déclarées sur une même ligne doivent être de même type.
Etant donné que la variable `u` reçoit, un littéral entier 6, le compilateur ne pourra déduire le type représenté par `auto` uniquement si le pointeur `v` est de même type, c'est à dire si `x` est un entier.
A noter que le mot-clé `auto` peut être utilisé avec d'autres mots-clé comme `const`.

La variable `y` sera de type double, car elle est initialisée avec un littéral flottant.

Il est également possible d'utiliser le mot-clé `auto` pour indiquer le type de retour d'une fonction. Pour cela, il faut utiliser la notation `decltype`, en plus du mot-clé `auto`, afin d'indiquer au compilateur quel type il doit déduire de `auto`.

Dans l'exemple précédent, le type de retour de la fonction `g` sera le type de la variable `ii`, comme le `decltype` l'indique. Ici par exemple, `g` retournera un entier.
Cette notation fonctionne aussi pour les déclarations de fonction. De même `h` retournera un entier. Il faut bien sur que cette signature corresponde lors de la définition de la fonction `h`.

```cpp
  auto h() -> decltype(1)   		int h() { return 1; }
  auto h() -> decltype(1) 			int h() -> decltype(1) { return 1; }
  
  
  string str = "str";
  auto f(int i) -> decltype(str) {if (i%2==0) return "pair"; else return "";}
  
  auto k = 2; k = 1.2;
  auto l = 4.2; l = 3;
  auto x = 5, *y = &x;
  auto a = 5, b = "cinq";
```

Pour la déclaration de la variable `j`, il faut retirer le type `auto` ou bien `int`. En effet, le compilateur va remplacer le mot `auto` par déduction du type. Il ne faut donc pas indiquer un type supplémentaire.

Concernant la première ligne avec la fonction `h`, il faut rajouter un `decltype` sur la signature qui comprend le mot `auto`. Sinon le compilateur ne peut savoir de quel type il s'agit. Comme la fonction retourne 1, on met une expression entière dans le `decltype`.
Pour la 2e ligne, il faut ajouter le `decltype` à la déclaration et à la définition pusique chacune d'elle comprend le mot-clé `auto`.
De manière générale, la notation `decltype` doit être utilisée à chaque fois que `auto` est indiqué (signature ou définition de fonction).

Pour la fonction `f`, auto devra correspondre à un string puisque `f` retourne une chaine de caractères dans le `if`. Il faut donc déclarer une varialble de type `string` et ajouter un `decltype` pour indiquer au compilateur que le type de retour sera `string`. De plus il faut ajouter une valeur de retour au `else`.

Pour les variables `k` et `l`, il n'y aura pasde soucis de compilation. `k` sera de type `int`, car affecté avec la valeur 2. Lorsqu'on lui affecte un nombre flotant, la valeur sera castée en `int`, et donc tronquée, `k` vaudra 1.
`l` sera un `double`, car initialisé avec 4.2. Lors de l'affectation, 3 sera casté en double, `l` vaudra 3.0.

La ligne suivante compile également, `auto` sera changé en `int`. La variable `y` sera un pointeur sur `int`, et reçoit l'adresse de `x`, également de type `int`.

La dernière ligne ne compile pas, `a` reçoit un littéral entier, et `b` une chaine de caractères.
Il faut déclarer ces variables sur 2 lignes distinctes.

---

#### decltype <a id="decltype"></a>

Le mot-clé decltype permet de déterminer le type d'une expression.
Il peut donc être utilisé pour créer une variable. Mais il eut également désigner le type de variable déclarée avec auto, comme dans l'exemple suivant.

```cpp
  int i;
  auto j = 2;
  
  decltype(i) v;
  decltype(j) v2;
```

Ici, on considère 2 variables i et j, de type int (j étant déclaré avec auto).
Le decltype va donc permettre de créer des variables du même type qu'une varialble déclarée avec un type comme i, et avec auto comme j. v et v2 seront donc également de type int.

Comme vu précédemment, decltype va également pouvoir permettre de renseigner le type de retour d'une fonction déclaré comme auto.

---

#### Initialisation et construction <a id="init"></a>

1/- Parmi les nouveautés apportées, nous avons tout d'abord l'initialisation des attributs d'instance lors de leur déclaration dans la classe. Ceci permet donc d'initialiser les attributs avec une valeur par défaut. Dans le cas où les attributs ne sont pas initialisés dans le constructeur, ceux-ci disposeront quand même d'une valeur cohérente.

2/- Un constructeur d'une classe peut désormais appeler un autre constructeur de la même classe. Ceci se réveille donc pratique lorsque plusieurs constructeurs effectuent des actions communes. Cela va permettre d'éviter la répétition d'instructions ou bien un appel à une fonction regroupant ces instructions par chaque constructeur.
Ou bien comme dans l'exemple présenté, cela peut etre utile afin que le constructeur vide appelle un autre constructeur avec des valeurs par défaut.

3/- Il est également possible d'initialiser un objet par une liste d'initialisation, comme avec les tableaux. Ceci va donc permettre d'initialiser les objets de la classe vector par exemple, en le remplissant directement.

```cpp
std::vector<std::string> vect {"alpha", "beta", "gamma"};
std::map<int, std::string> map {{1, "1"}, {2, "2"}};
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

Il existe maintenant les collections unordered pour les conteneurs associatifs fonctionnant sur un principe clé/valeur. Cela correspond donc aux classes  map, multimap, set et multiset. Ces nouvelles collections permettent de définir notre propose fonction de hachage utilisée pour stocker les éléments. Cela permet donc de définir notre propre stockage et notre propre complexité lors de l'utilisation du conteneur.

##### Tuple <a id="tuple"></a>

Ce nouveau conteneur permet de créer une collection de taille fixe avec des types différents. On peut l'utiliser comme le type pair à la différence que celui-ci ne se limite pas à 2 éléments. On peut donc créer un ensemble comportant une liste de types prédéfinis.

```cpp
std::tuple<std::string, int, bool> loutre("loulou", 10, true);
```

##### Array <a id="array"></a>

Le type array définit dans la stl un conteneur représentant les tableaux statiques du langage. On peut alors définir ces tableaux de manière à ce qu'ils comportent toutes les propriétés des conteneurs de la stl.

```cpp
std::array<int, 4> tab = {1, 5, 8, 9};
std::array<int, 4>::iterator it;

for (it = tab.begin(); it != tab.end(); ++it)
{
    std::cout << *it << " ";
}
std::cout << std::endl;
```

---

#### Fonctions begin et end <a id="begin_end"></a>

Il existe maintenant, en plus des méthodes begin et end des conteneurs retournant respectivement un itérateur sur le début et la fin du conteneur, des fonctions begin et end.
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

On peut maintenant en C++11, hériter des constructeurs d'une super classe à l'aide du mot-clé using.

Dans l'exemple présenté, les deux objets feront appel au constructeur de B. Dans la classe D, le mot using indique que la classe peut utiliser le constructeur de B pour les instances de D qui sont créées. Cela fonctionne avec les objets alloués automatiquement ou dynamiquement.

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

Le mot-clé override permet d'imposer au compilateur qu'une fonction doit hériter d'une fonction virtuelle d'une classe mère.
Cela permet donc, en cas d'erreur dans la signature, de générer une erreur de compilation indiquant que la méthode signalée override n'hérite d'aucune autre méthode.
C'est le cas du programme 2, la méthode f de la classe B déclarée override et qui doit donc hériter d'une méthode et la redéfinir. Or dans la classe B, f prend en paramètre un int au lieu d'un double. Le programme ne compile pas.

Le programme 1 fonctionne en C++98, mais la méthode f de B n'hérite pas de celle de A pour les même raisons puisque la signature n'est pas la même.

Le programme 3 fonctionne en C++11, la méthode de B hérite bien de celle de la classe A car les signatures correspondent.

---

#### Final <a id="final"></a>

Tout comme le mot-clé override utilisé pour indiquer qu'une méthode doit hériter d'une méthode de la classe parente, final permet d'indiquer qu'une classe ou une méthode n'est pas dérivable.

Les 3 programmes présentés ne pourront pas fonctionner en C++98 car utilisent tous le terme final.
Le premier ne fonctionnera pas en C++11 non plus car la classe A étant déclarée final, il n'est pas possible de créer de classe dérivant de celle-ci.

Le deuxième programme comporte une erreur similaire puisque la classe A déclare une méthode f déclarée final et que la classe B hérite de cette méthode f comportant la même signature.


Dans le troisième programme, la méthode f de A est déclarée final et B définit une autre méthode f puisqu'elle ne comporte pas la même signature. Celle de la classe B n'est donc  pas considérée comme une méthode dérivant de celle de A. Et donc le final ne pose pas de problèmes car on fait la distinction entre les méthodes.

---

#### Ordre d'appel des constructeurs et destructeurs <a id="linearization"></a>

L'ordre d'appel des constructeurs et destructeurs ne change pas en C++11. L'affichage suivant présente cet ordre d'appel avec les classes CompteBancaire.


Résultat :
Constructeur CB
Constructeur CompteRemunere
Constructeur CompteDepot
Constructeur CompteDepotRemunere
Destructeur CompteDepotRemunere
Destructeur CompteDepot
Destructeur CompteRemunere
Destructeur CB

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
