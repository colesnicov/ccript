# CCRipt - Omezeni

[zpet](../readme.md)

CCRipt ma jista omezeni.


## Podminky v prikazech WHILE a IF:

Pri retezeni si je treba dat pozor na prioritu operatoru a zanorovani!

**spatne**:

```
if(5 < 6 && true) ...
```

Pro spravne vyhodnoceni je treba jednotlive (jednodussi casti jedne slozite podminky) zanorit do zavorek **(** a **)**.

**spravne:**

```
if((5 < 6) && true) ...
```

Kazde zanorovani do zavorek si vyzaduje dodatecnou pamet RAM a proto je, v pripade embedded zarizeni, lepsi se temto technikam vyhnout a pouzit predcasny vypocet a vysledek ulozit do promenne ktera pak bude testovana. Napriklad takto:

```
bool predprip = 5 < 6;
if((5 < 6) && true) ...
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

## Priorita logickych operaci

Protoze zpracovani skript probiha postupnym ctenim *zleva doprava*, je dulezite si dobre naplanovat prioritu vypoctu.

Na rozdil od plnehodnotneho C, kde je priorita operatoru urcovana na zaklade priority operatoru, ve skriptach *ccript* **se poradi vypoctu urcuje s pomoci zavorek '('** a **')'**!

To znamena ze pro vyhodnoceni podminke jestli je **A je `true` a zaroven je take `true` B nebo C** tak musime pouzit zavorek nebo spravne poradi operaci.

#### spatne

```
pravdivost1 = A && B || C;
```

#### spravne

```
pravdivost1 = A && (B || C);
```

nebo

```
pravdivost1 = B || C && A;
```

**Je tedy** <u>nutno pouzit zavorky</u>: 

```
pravdivost1 = A && (B || C);
                   ^      ^
```

nebo <u>usporadat poradi operaci</u> tak aby vysledek byl spravny:

```
pravdivost1 = A && B || C; // vysledek je spatny

pravdivost1 = A && (B || C); // vysledek je spravny

pravdivost1 = B || C && A; // vysledek je spravny
```


 


  

 
  

 

