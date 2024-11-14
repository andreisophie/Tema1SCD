# Tema 1 SCD - Server OAuth

by Maruntis Andrei

## Aspecte generale

Tema necesita implementarea unei aplicatii client-server care se ocupa de autentificarea si autorizarea unor operatii specificate in protocolul dat.

Implementarea este realizata in limbajul C, respectiv fisierul interfata din care au fost generate fisierele corespunzatoare pentru aplicatie. In general, codul aplicatiei se gaseste in fisierele `tema1_client.c` si `tema1_server.c`, plus mici modificari la fisierul `tema1_svc.c` pe care le voi detalia mai jos. Restul codului din celelalte fisiere este cel generat automat de utilitarul rpcgen.

Pentru coding style am folosit script-urile de la temele din anul 1, materia PCLP1.

## Rularea aplicatiei

Toate fisierele sursa necesare rularii aplicaitiei sunt deja generate, nu este nevoie sa rulam utilitarul rpcgen.

In Makefile, pe ultimele linii, sunt cateva reguli foarte utile pentru a rula programul:

- Variabilele `TESTNO` si `TOKEN_LIFETIME` pot fi modificate pentru a rula un anumit test
- In 2 terminale separate este suficient sa rulam comenzile `make run_server` si apoi `make run_client` (Aceste comenzi vor apela automat regulile de build necesare compilarii aplicatiei)
- Dupa rulare, se poate rula regula `make clean` pentru a sterge fisierele obiect si executabilele

## Procedurile utilizate. Fisierul interfata

Primul pas in dezvoltarea temei este definirea procedurile utilizate de aplicatie si scrierea fisierului interfata `.x`. Am implementat 5 proceduri: 4 dintre ele sunt cele mentionate in protocol (Request Authorization, Request Access Token, Approve Request Token si Validate Delegated Action) si o procedura suplimentara, **Refresh Access Token**.

Necesitatea implementarii acestei ultime proceduri tine de aspectul codului. Teoretic, este posibil sa actualizez jetonul de acces folosind procedura Request Access Token, avand in vedere faptul ca **jetonul de refresh devine jetonul de autentificare** pentru procesul de actualizare a jetonului de acces. Cu toate acestea, am preferat sa am o separatie clara intre functia care se ocupa de generarea jetonului de acces si cea care se ocupa de actualizarea jetonului, avand in vedere **mesajele diferite** care trebuie afisate (atat de client, cat si de server), respectiv **verificarile suplimentare** care ar diferentia intre cele doua functionalitati, daca as folosi aceeasi procedura.

In ceea ce priveste implementarea procedurilor, fiecare dintre cele 5 functii are urmatoarele similaritati:

- Fiecare procedura are asociata o structura `<procedure_name>_arg` pe care o primeste ca argument si care contine datele necesare pentru a executa functionalitatea, conform protocolului
- Fiecare procedura are asociata o structura `<procedure_name>_ret` pe care o trimite clientului dupa executia procedurii si care contine datele cerute, conform protocolului
- In plus, fiecare structura `_ret` are asociata o enumeratie `<procedure_name>_status` care semnifica statutul executiei procedurii la final: spre exemplu, SUCCESS sau ERROR, care pentru unele proceduri poate fi separat intre USER_NOT_FOUND, NOT_PERMITTED etc.

Pentru detalii despre datele exacte din aceste structuri, vezi fisierul interfata `tema1.x`.

## Implementarea server-ului

La pornire, server-ul trebuie să ruleze o funcție care să încarce datele utilizatorilor si resurselor din fișierele primite ca parametru din linia de comanda. Pentru asta, a fost nevoie de un mic artificiu de sintaxa:

- Functia `main` care se executa la pornirea server-lui se gaseste in `tema1_svc.c`, iar in cadrul acestei functii trebuie rulata o functie de initializare `initialize_server` a carei definitii se va gasi in mod obligatoriu in fisierul `tema1_server.c`, deoarece structurile de date necesarii rularii server-ului trebuie sa fie in acelasi fisier cu functiile corespunzatoare procedurilor.
- Pentru a rezolva aceasta problema, am folosit keyword-ul `extern` urmat de semnatura functiei `initialize_server` in fisierul `tema1_svc.c`, pentru a informa compilatorul ca definitia functiei va fi in alt fisier.

La executia functiei `initialize_server` descrisa mai sus, server-ul citeste din fisierele de intrare datele despre utilizatori si resurse si le pune in doi vectori de structuri pentru utilizatori, respectiv pentru resurse.

Structura pentru utilizatori contine date precum:

- Identificatorul utilizatorului
- Token-urile sale de autentificare, respectiv de acces
- Termenul de valabilitate al token-ului de acces
- Daca utilizatorul doreste reinnoirea automata a token-ului
- Permisiunile asociate token-ului

Vectorul de resurse contine, in mod mai simplu, doar denumirea resurselor.

Atunci cand este necesara cautarea unor date in baza de date a server-ului, asta se face prin parcurgerea vectorului corespunzator element cu element.

Ultima parte relevanta din implementarea server-ului sunt functiile care se ruleaza la apelarea procedurilor de catre clienti. In general, aceste functii fac urmatorii pasi:

1. Cauta utilizatorul care a lansat cererea in baza de date, dupa datele de identificare pe care le-a primit (spre ex. identificatorul pentru Request Authorization, jetonul de autentificare pentru Approve Request Token)
2. Daca a gasit utilizatorul, executa operatiunea corespunzatoare, descrisa in protocol
3. Trimite catre client un raspuns ce va contine un cod de statut, respectiv datele cerute, prevazute de protocol

Pentru mai multe detalii despre fiecare procedura, vezi fisierul `tema1_server.c`.

## Implementarea clientului

In mod asemanator cu server-ul, clientul trebuie sa ruleze o functie de initializare care deschide fisierul din care va citi comenzile utilizatorilor. In cazul clientului, am acces la functia `main` direct in fisierul `tema1_client.c` unde am implementat toata logica clientului, deci acest pas este elementar.

Datele utilizatorilor sunt stocate in structuri in mod asemanator cu cele din server, chiar daca acestea din client detin mai putine date. Spre deosebire de server, vectorul de structuri nu va contine date la inceputul executiei, ci va fi completat cu datele corespunzatoare pe parcursul executiei.

Dupa initializare, clientul va citi din fisier comenzile, linie si cu linie, si va executa pentru fiecare apelurile de proceduri corespunzatoare. Mai precis:

- Pentru o actiune de tipul REQUEST, clientul va executa:
    - Un apel al procedurii **Request Authorization**, pentru a obitne **jetonul de autorizare**
    - Daca s-a executat cu succes apelul precedent, va apela procedura **Approve Request Token** pentru a **semna jetonul** si pentru ca serverul sa ii **asocieze permisiunile**
    - La final, va apela procedura **Request Access Token**, pentru a obtine **jetonul de acces** si **jetonul de refresh** (daca a fost cerut).
- Pentru o actiune asupra resurselor, clientul va executa:
    - Daca utilizatorul are un jeton expirat si detine un jeton de refresh, mai intai va apela procedura **Refresh Access Token** pentru a obtine un **jeton de acces nou si valid**
    - Apeleaza procedura **Validate Delegated Action** pentru a executa operatia in sine

## Alte mentiuni legate de implementare

Un aspect important pe parcursul implementarii este **folosirea corecta a pointerilor catre sirurile de caractere**, anume duplicarea acestora folosind `strdup`, dupa primirea string-urilor de la apelurile de proceduri. Acest lucru este important, deoarece structurile argument si de return ale apelurilor procedurilor reutilizeaza aceleasi zone de memorie pentru stocarea acelor date.

Modul in care se prelucreaza drepturile utilizatorilor este urmatorul:

- Drepturile se citesc din fisierul de approvals si sunt stocate **ca string**, "as-is"
- In server, legatura dintre utilizatori si drepturile de acces la resurse se face astfel:
    - Structura asociata fiecarui utilizator are un camp de permisiuni cu tipul de data `char **` sau **array de string-uri**
    - Semnificatia acestui vector este urmatoarea: `permissions[j]` reprezinta drepturile utilizatorului curent asupra resurse `resources[j]`
    - Spre exemplu:
        - Fiind date, pentru un anumit utilizator: `permissions = {"RI", "RXM"}` si `resources={"Files", "UserSettings"}`, semnificatia este urmatoarea:
        - Utilizatorul are drepturi de Read si Insert asupra resurse Files si drepturi de Read, Execute si Modify asupra resurse UserSettings
- Tipul operatiei pe care vrea sa o execute clientul se trimite ca string (ex. `"READ"`, `"EXECUTE"`), in cadrul structurii argument a procedurii
- Server-ul va verifica daca utilizatorul are dreptul de a executa operatia ceruta cautand litera corespunzatoare in string-ul de permisiuni asociat resursei (ex. pt READ cauta litera `R`, pe EXECUTA cauta litera `X`)
- Cautarea mentionata la punctul precedent se realizeaza folosind functia `strchr`

