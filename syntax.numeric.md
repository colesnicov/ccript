#CCRipt <sub>*An Embeddable Interpreter for a 'C' Like Scripting Language*</sub>

###### *version* v.1b0

[NAHORU](readme.md)

#### Ciselny typ

CCRipt rozlisuje dva cislne typy. **cele cislo** `int` a **s desetinou plovouci carkou** (dale uz jen *'s plovouci carkou'*) `float`.

Krome prezentace se lisi take mnozstvim pameti kterou vyzaduji pro praci s nimi. Obecne se da rict ze `float` je narocnejsi jak na pamet tak na procesor. To druhe plati v pripade ze nase 'zelezo' postrada 'FPU'.

Prace s nimi se od sebe nelisi. Na obou typech je mozne provadet stejne aritmeticke operace. Lisi se tedy pouze v definici..

##### Cele cislo

[Celociselny typ](https://en.wikipedia.org/wiki/Integer_%28computer_science%29) se ve *skriptach* definuje stejne jako v 'C' klicovym slovem **int** a ma rozsah **-2,147,483,648** az **2,147,483,647** vcetne **0**.

##### Cislo s plovouci carkou

[Cislo s plovouci carkou](https://en.cppreference.com/w/cpp/keyword/float) se ve *skriptach* definuje stejne jako v 'C' klicovym slovem **float** a ma rozsah **-3.4028235E+38** az **3.4028235E+38** vcetne **0**.

-----------------------

##### Definice

*Cele cislo*:

```
int number1;
```

*Cislo s plovouci carkou*:

```
float number1;
```

------------------------

##### Definice a prirazeni

*Cele cislo*:

```
int number1 = 10;
```

*Cislo s plovouci carkou*:

```
float number1 = 0.123;
```

u `float` se pocatecni nula muze vynechat:

```
float number1 = .123;
```

--------------------------

##### Prirazeni

```
number1 = 10;
number1 = number2;
```

### aritmetika

V prubehu prirazeni promenne je mozne provadet jiste matematicke ukony:
 - `+` - scitani
 - `-` - odecitani
 - `*` - nasobeni
 - `/` - deleni
 

 a pouziti nekolika zpusobu vypoctu.

##### Bezny:

```
number1 = 10 + 10;
number2 = number1 + (20 / 10);
```

-----------------------------

##### Zkraceny:
  
```
number1 *= 2;
number2 += 10;
```

  nebo

```
number1 *= 2 + 2;
number2 += number1 - 10;
```

------------------------

##### A jeste:

```
number2++;
--number2;
```

#### Priorita operatoru

Protoze se provadeni skript probiha postupnym ctenim *zleva doprava*, je dulezite si dobre naplanovat prioritu vypoctu.

Na rozdil od domaci kalkulacky, kde ve vetsine pripadu pri napsani `2 + 2 * 2` dostanete `6`, ve skriptach *ccript* **se poradi vypoctu urcuje s pomoci zavorek '(' a ')'**!

To znamena ze musite napsat: `2 + (2 * 2)`.

##### spatne:

```
number1 = 9 + 0 + 9 + 0 + 9 * 0 + 9;
```

##### spravne:

```
number1 = 9 + 0 + 9 + 0 + (9 * 0) + 9;
```

Zmena je tedy <i>0 + **(9 * 0)** + 9;</i>.


### Pravdivostni logika

S ciselnymi typy je mozne provadet nektere 'logicke' ukony:
 - `==` - rovna se
 - `!=` - lisi se
 - `>`  - vetsi
 - `<`  - mensi
 - `>=` - vetsi nebo rona se
 - `<=` - mensi nebo rona se
 
```
number1 == number2;
number1 < number2;
number2 >= 10;
```
-----------------------

todo: 
 - pridat retezeni? *'a < b && a != 0'* toto je dostupne v bloku `while`
  

### Mazani 

I zde plati pravidlo zivotnosti a viditelnosti promenne v ramci bloku.
 
```
int number1 = 0;
{
  int number2 = number1;
}
number2 += 1; // error, undefined variable 'number2'
```

Odstranit muzu kdykoliv: 

```
delete number1;
delete number2;
 ```
 
*Nekdy take musim*: 
 
```
int number1 = 0;
// int number1 = 2; // error, duplicate variable 'number1'

delete number1;

float number1 = 2.3; // ok
 ```

 