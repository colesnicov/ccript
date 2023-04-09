# CCRipt - Typ INT, LONG, FLOAT

[zpet](../readme.md)

CCRipt rozlisuje tri cislne typy. **cele cislo** `int`, **dlouhe cele cislo** `long` a **dlouhe cislo s teckou** (dale uz jen *'s teckou'*) `float`.

Krome prezentace se lisi take mnozstvim pameti kterou vyzaduji pro praci s nimi. Obecne se da rict ze `long` i `float` je narocnejsi jak na pamet tak na procesor. V pripade `float` to druhe plati v pripade ze nase 'zelezo' postrada 'FPU'.

Prace s nimi se od sebe nelisi. Na techto typech je mozne provadet stejne aritmeticke operace.

## Cele cislo

[Celociselny typ](https://en.wikipedia.org/wiki/Integer_%28computer_science%29) se ve *skriptach* definuje klicovym slovem **int** nebo **long** a ma rozsah:

 - **int** (*-2147483647*) az (*2147483647*).
 - **long** (*-9223372036854775807*) az (*9223372036854775807*).

## Cislo s teckou

[Cislo s teckou](https://en.cppreference.com/w/cpp/keyword/float) se ve *skriptach* definuje klicovym slovem **float** a ma rozsah:
 - **float** (*-3.4028235E+38*) az (*3.4028235E+38*).



## Definice

*Cele cislo*:

```
int cislo1;
```

nebo 

```
long dlouhe1;
```

*Cislo s teckou*:

```
float steckou1;
```



## Definice a prirazeni

*Cele cislo*:

```
int number1 = 10;
```

nebo 

```
long number1 = 10;
```

*Cislo s teckou*:

```
float number1 = 0.123;
```

u `float` se nula muze vynechat:

```
float number1 = .123;
```


## aritmetika

V prubehu prirazeni promenne je mozne provadet jiste matematicke ukony:
 - `+` - scitani
 - `-` - odecitani
 - `*` - nasobeni
 - `/` - deleni
 

 a pouziti nekolika zpusobu vypoctu.

#### Bezny:

```
number1 = 10 + 10;
number2 = number1 + (20 / 10);
```



#### Zkraceny:

Kratky zapis:

```
number1 *= 2;
number2 += 10;
```

nebo

```
number1 *= 2 + 2;
number2 += number1 - 10;
```


#### A jeste:

Jeste kratsi:

```
number2++;
--number2;
```

## Pravdivostni logika

S ciselnymi typy je mozne provadet nektere 'logicke' ukony:
 - `==` - rovna se
 - `!=` - lisi se
 - `>`  - vetsi
 - `<`  - mensi
 - `>=` - vetsi nebo rona se
 - `<=` - mensi nebo rona se
 
Napriklad:

```
number1 == number2;
number1 < number2;
number1 <= number2;
```
  
## Priorita matematickych operaci

Protoze zpracovani skript probiha postupnym ctenim *zleva doprava*, je dulezite si dobre naplanovat prioritu vypoctu.

Na rozdil od domaci kalkulacky, kde ve vetsine pripadu pri napsani <b>_2 + 2 * 2_  = 6</b> a nemusite resit poradi operatoru tak **tady tomu tak NENI**. Ve skriptach *ccript* **se poradi vypoctu urcuje s pomoci zavorek '('** a **')'**!

**spatne:**

```
number1 = 2 + 2 * 2;
```

**spravne:**

```
number1 = 2 + (2 * 2);
```

nebo

```
number1 = 2 * 2 + 2;
```

**Je tedy** <u>nutno pouzit zavorky</u>: 

```
number1 = 2 + (2 * 2);
              ^     ^
```

nebo <u>usporadat poradi operaci</u> tak aby vysledek byl spravny:

```
number1 = 2 + 2 * 2; // vysledek je 8

number1 = 2 + (2 * 2); // vysledek je 6

number1 = 2 * 2 + 2; // vysledek je 6
```


 