# CCRipt - Embeddable Interpreter for a Custom 'C' Like Scripting Language


**ccript** vznikl jako volnocasovy maraton za snahou uspokojit svou potrebu po prostredku jak s pomoci, textovych a tudyz i snadno upravitelnych, skript upravovat chovani programu a intereagovat s HW vrstvou zarizeni. Byl vyvijen s vedomim ze na poli s *'embedded'* zarizenimi, v dobe vyvoje tohoto nastroje, neni k dispozici nic co by plne vyhovelo mym potrebam.

**ccript** je napsan na cistem 'C' s moznosti nekterych nastaveni, sadou predpripravenych funkci a praci se sesti datovymi typy.

## version 1r1

**Release Candidate. Nutno testovat. Pripominky jsou vitany!**


## Kratce

**ccript** umoznuje
 - pracovat se sesti datovymi typy:
    - [int](doc/syntax.numeric.md) - zaporna a kladna cisla. Napriklad *-5000* nebo *12095*.
    - [long](doc/syntax.numeric.md) - zaporna a klodna dlouha cisla. Napriklad *-93455000* nebo *84512095*.
    - [float](doc/syntax.numeric.md) - zaporna a klodna dlouha cisla s carkou. Napriklad *-93455000* nebo *84512095*.
    - [bool](doc/syntax.bool.md) - pravdivostni. Napriklad *true* nebo *false* take *1* nebo *0*.
    - [char](doc/syntax.strings.md) - slova a vety. Napriklad *Hello* nebo *Hello world*.
    - [string](doc/syntax.strings.md) - pismena. Napriklad *A* nebo *z* nebo jiny znak *?*.
 
 - Smycky:
    - [while](doc/syntax.while.md) - dokola opakujici se blok pri splneni podminky.
 
 - Podminky:
    - [if](doc/syntax.if.md) - Blok se zpracuje jen pri splneni podminky.
 
 - Promenne:
    - [obecne](doc/syntax.vars.md) - Zasady prace s promennymi.
    - [prostredi](doc/syntax.env.md) - Promenne prostredi.
 
 - Funkce:
    - [preddefinovane](doc/syntax.functions.md) - Sada preddefinovanych funkci.
    - [vlastni](doc/syntax.builtin.md) - Volani vlastnich *C* funkci uvnitr skripty. 
    - [funkcni bloky](doc/syntax.blocks.md) - Definice a volani vlastnich funkci uvnitr skripty.
 
 
 - [pravdivostni logika](doc/syntax.logic.md).
 - [a omezeni](doc/syntax.limitations.md) - Doporuceni a praktiky kterym je treba se vyhnout.


## Zavislosti

Ke sve praci vyzaduje
 - [colesnicov/cvector](https://github.com/colesnicov/cvector) - implementace dynamickeho kontejenu v C.
 - [colesnicov/filebuffer](https://github.com/colesnicov/filebuffer) - dynamicky pristup k obsahu souboru.
 - [colesnicov/emblib](https://github.com/colesnicov/emblib) - pomocne funkce ktere nemusi byt dostupne na vsech embedded platformach.


## Licence

MIT

## TODO
 - Restrukturovat strukturu projektu. Nazvy souboru a funkci musi mit rozumnejsi logiku a vhodnejsi umisteni.
 - Prace s poli. Napr. `int arr[5] = 3;` inicializuje pole s peti prvky typu `int` a priradi jim hodnotu '3'. A `arr[4] = 5;` priradi novou hodnotu poslednimu prvku (indexace zacina od nuly).
 
 

