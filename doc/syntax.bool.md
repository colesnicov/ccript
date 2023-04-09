# CCRipt - Typ BOOL

[zpet](../readme.md)

CCRipt zna pravdivostni typ `bool`.

Tento typ muze nabyvat dvou hodnot:

 - `true` - pravda
 - `false` - lez

## Definice

```
bool pravdivost1;
```

## Definice a prirazeni

```
bool pravdivost1 = true;
```

nebo cislo. 

```
bool pravdivost1 = 1;
```

*Jakekoliv cislo vetsi nez 0 bude povazovano za pravdu*:

```
bool pravdivost1 = 10; // pravda
```

je stejne jako 

```
bool pravdivost1 = true; // pravda
```

a

```
bool pravdivost1 = 0;
```

je stejne jako 

```
bool pravdivost1 = false;
```


### Prirazeni

```
pravdivost1 = true;
pravdivost2 = pravdivost1;
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
pravdivost1 = true;
pravdivost12= pravdivost1 && (20 > 10);
```

#### Zkraceny:
  
```
pravdivost1 != 1;
pravdivost2 &= true;
```

nebo

```
pravdivost1 &= pravdivost1;
pravdivost2 &= pravdivost1 && true;
```

**Note, ted si tu nejsem jisty. Overit si definice a prirazeni a doplnit to sem!**


## Pravdivostni logika

Pro praci s typem `bool` je pouzita [pravdivostni logika](syntax.logic.md).



