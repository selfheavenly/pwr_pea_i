# TSP Solver v2.0 - Problem Komiwojażera

> Kompleksowy program w C++23 do rozwiązywania Traveling Salesman Problem (TSP) różnymi metodami heurystycznymi i dokładnymi.

---

## 📋 Spis treści

1. [Opis projektu](#opis-projektu)
2. [Wymagania funkcjonalne](#wymagania-funkcjonalne)
3. [Implementowane algorytmy](#implementowane-algorytmy)
4. [Struktura projektu](#struktura-projektu)
5. [Kompilacja](#kompilacja)
6. [Użytkowanie](#użytkowanie)
7. [Format plików](#format-plików)
8. [Wyniki działania](#wyniki-działania)
9. [Rozszerzanie programu](#rozszerzanie-programu)

---

## 🎯 Opis projektu

Program rozwiązuje **Problem Komiwojażera** (TSP - Travelling Salesman Problem) dla instancji symetrycznych i asymetrycznych. Implementacja obejmuje trzy metody: Brute Force (dokładną), Nearest Neighbour (zachłanną) oraz Random (losową próbkowanie).

### Główne cechy:
- ✅ **Konfigurowalny** - wszystkie parametry w pliku `config.txt`
- ✅ **Modularny** - łatwe dodawanie nowych algorytmów
- ✅ **Konsolowy** - brak interakcji z użytkownikiem
- ✅ **Format TSPLIB** - obsługa standardowych instancji
- ✅ **Zapis wyników** - eksport do CSV
- ✅ **Analiza błędów** - bezwzględny i względny
- ✅ **Wsparcie dla ATSP** - asymetryczne instancje

---

## 📝 Wymagania funkcjonalne

### 1. **Plik konfiguracyjny**
Program po uruchomieniu czyta parametry z pliku konfiguracyjnego (`config.txt`), który zawiera:
- Ścieżkę do pliku z danymi (instancja TSP w formacie TSPLIB)
- Wartość optymalną (jeśli nieznana w pliku danych)
- Parametry algorytmu (liczba iteracji, czas maksymalny, liczba powtórzeń)
- Warunki zatrzymania (czas, iteracje)
- Przełącznik `show_progress` do wyświetlania postępu

### 2. **Dane wejściowe**
Pliki danych w formacie **TSPLIB** (`.txt`, `.atsp`, `.tsp`) zawierają:
- Macierz odległości (FULL_MATRIX, UPPER_ROW, LOWER_DIAG_ROW)
- Rozmiar instancji (DIMENSION)
- Opcjonalnie: optymalne rozwiązanie i jego długość (TOUR_SECTION)

### 3. **Wyniki na ekranie**
Program wyświetla dla każdej instancji:
- Nazwę pliku z danymi
- Wynik optymalny (z TSPLIB lub configu)
- Ścieżkę optymalną (jeśli znana, dla instancji <16 miast)
- Parametry metody
- Czas realizacji w **milisekundach** (ms)
- Wynik znaleziony
- Ścieżkę znalezioną (dla instancji <16 miast)
- Błąd bezwzględny i względny (w jednostkach i %)

### 4. **Plik wyjściowy (CSV)**
Wyniki zapisywane są do pliku CSV zawierającego:
- Nazwę badanej instancji
- Wynik optymalny i ścieżkę optymalną
- Wynik znaleziony i ścieżkę znalezioną
- Czasy dla każdej próby (zgodnie z liczbą powtórzeń)
- Średni czas wyznaczenia rozwiązania w **ms**
- Błąd bezwzględny i względny (x2: w jednostkach i %)
- Parametry algorytmu

### 5. **Bez interakcji użytkownika**
Program działa całkowicie autonomicznie - wszystkie dane są wczytywane z plików konfiguracyjnych. Brak menu i wprowadzania danych z klawiatury.

---

## 🧮 Implementowane algorytmy

### 1. **Brute Force** (`brute-force`)
**Opis:** Algorytm dokładny przeszukujący wszystkie permutacje tras.

**Zalety:**
- Gwarantuje znalezienie optymalnego rozwiązania

**Wady:**
- Złożoność O(n!) - niepraktyczny dla n > 12-14 miast

**Parametry:**
- Brak (algorytm deterministyczny)

**Zastosowanie:** Małe instancje do weryfikacji wyników innych algorytmów.

---

### 2. **Nearest Neighbour** (`nearest-neighbour`)
**Opis:** Algorytm zachłanny - w każdym kroku wybiera najbliższe nieodwiedzone miasto.

**Zalety:**
- Bardzo szybki (O(n²))
- Deterministyczny

**Wady:**
- Nie gwarantuje optymalności
- Typowy błąd: 20-50%

**Parametry:**
- `start_city` - miasto początkowe (domyślnie: 0)

**Zastosowanie:** Szybkie przybliżone rozwiązania dla dużych instancji.

---

### 3. **Random** (`random`)
**Opis:** Algorytm losowy - generuje losowe permutacje i wybiera najlepszą.

**Zalety:**
- Prosty w implementacji
- Może znaleźć dobre rozwiązania przy wystarczającej liczbie prób

**Wady:**
- Stochastyczny - wyniki się różnią
- Wymaga wielu iteracji

**Parametry:**
- `iterations` - maksymalna liczba iteracji
- `max_time` - maksymalny czas wykonania (sekundy)

**Early stopping:** Algorytm zatrzymuje się wcześniej, jeśli znajdzie rozwiązanie optymalne (z tolerancją 0.01).

**Zastosowanie:** Benchmark dla innych metod, weryfikacja przestrzeni rozwiązań.

---

## 📁 Struktura projektu

```
pea_tsp_1/
│
├── CMakeLists.txt              # Konfiguracja CMake
├── main.cpp                    # Główny punkt wejścia programu
├── README.md                   # Dokumentacja (ten plik)
│
├── include/                    # Pliki nagłówkowe (.h)
│   ├── config.h
│   ├── result.h
│   ├── result_writer.h
│   ├── solvers.h
│   ├── test_suite.h
│   └── tsp_instance.h
│
├── src/                        # Implementacje (.cpp)
│   ├── config.cpp
│   ├── result_writer.cpp
│   ├── solvers.cpp
│   └── tsp_instance.cpp
│
└── data/                       # Pliki danych i konfiguracji
    ├── config.txt              # Plik konfiguracyjny
    ├── test_instances.txt      # Lista instancji testowych
    ├── br17.txt                # Przykładowa instancja TSPLIB
    └── results.csv             # Wynikowy plik CSV (generowany)
```

### Moduły programu:

- **config** - Wczytywanie i parsowanie pliku konfiguracyjnego
- **tsp_instance** - Ładowanie i reprezentacja instancji TSP w formacie TSPLIB
- **solvers** - Implementacje algorytmów (Brute Force, Nearest Neighbour, Random)
- **result** - Struktura przechowująca wyniki pojedynczego uruchomienia
- **result_writer** - Zapis wyników do pliku CSV
- **test_suite** - Moduł do automatycznych testów wielu instancji

---

## 🔧 Kompilacja

### Wymagania systemowe:
- **Kompilator C++23** (GCC 11+, Clang 14+, MSVC 2022+)
- **CMake 3.20+**

### Kompilacja za pomocą CMake:

#### Linux / macOS:
```bash
# Utwórz folder build
mkdir build
cd build

# Skonfiguruj CMake
cmake ..

# Skompiluj
cmake --build .

# Uruchom program
./pea_tsp_1
```

#### Windows (Visual Studio):
```bash
# Utwórz folder build
mkdir build
cd build

# Skonfiguruj CMake dla Visual Studio
cmake .. -G "Visual Studio 17 2022"

# Skompiluj
cmake --build . --config Release

# Uruchom program
Release\pea_tsp_1.exe
```

#### Windows (MinGW):
```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
pea_tsp_1.exe
```

### Kompilacja manualna (bez CMake):
```bash
g++ -std=c++23 -O3 -Wall -Wextra \
    main.cpp \
    src/config.cpp \
    src/result_writer.cpp \
    src/solvers.cpp \
    src/tsp_instance.cpp \
    -o pea_tsp_1
```

---

## 🚀 Użytkowanie

### ⚠️ WAŻNE: Lokalizacja plików

**Pliki konfiguracyjne i danych muszą znajdować się w tym samym katalogu co plik wykonywalny (`pea_tsp_1.exe` lub `pea_tsp_1`).**

Struktura katalogów po kompilacji:
```
build/                          # Lub Release/ na Windows
├── pea_tsp_1.exe              # Program
├── config.txt                 # TUTAJ! (skopiuj)
├── br17.txt                   # TUTAJ! (skopiuj)
└── results.csv                # Zostanie wygenerowany
```

### 1. **Tryb standardowy** (pojedyncza instancja)

Utwórz plik `config.txt` w katalogu z programem:

```ini
# TSP Solver Configuration File

# Ścieżka do pliku danych (TSPLIB format)
data_file = br17.txt

# Wartość optymalna (jeśli nieznana w pliku)
optimal_value = 39

# Metoda: brute-force, nearest-neighbour, random
method = nearest-neighbour

# Liczba powtórzeń dla każdego testu
repetitions = 5

# Parametry dla metody Random
iterations = 100000
max_time = 60.0

# Wyświetlanie postępu podczas rozwiązywania
show_progress = true

# Plik wyjściowy dla wyników
output_file = results.csv
```

**Uruchomienie:**
```bash
# Linux/macOS
./pea_tsp_1

# Windows
pea_tsp_1.exe
```

**Uruchomienie z innym plikiem konfiguracyjnym:**
```bash
./pea_tsp_1 my_config.txt
```

---

### 2. **Tryb testowy** (wiele instancji)

Utwórz plik `test_instances.txt`:
```txt
# Lista instancji testowych
# Format: ścieżka_do_pliku
br17.txt
ftv33.atsp
ftv44.atsp
```

**Uruchomienie:**
```bash
./pea_tsp_1 --test
```

Program automatycznie:
- Wczyta wszystkie instancje z listy
- Uruchomi wszystkie trzy algorytmy
- Zapisze wyniki do `test_results.csv`
- Zweryfikuje wyniki Brute Force z wartością optymalną

---

## 📄 Format plików

### 1. **Plik konfiguracyjny (`config.txt`)**

```ini
# Komentarze zaczynają się od #

# Ścieżka do pliku z danymi
data_file = nazwa_pliku.txt

# Wartość optymalna (opcjonalna, jeśli nie ma w pliku TSPLIB)
optimal_value = 123.45

# Wybór metody rozwiązania
method = nearest-neighbour   # brute-force | nearest-neighbour | random

# Liczba powtórzeń eksperymentu
repetitions = 10

# Parametry dla algorytmu Random
iterations = 100000          # Maksymalna liczba iteracji
max_time = 60.0             # Maksymalny czas w sekundach

# Maksymalna liczba iteracji (dla przyszłych algorytmów)
max_iterations = 10000

# Wyświetlanie postępu
show_progress = true        # true | false

# Plik wyjściowy
output_file = results.csv
```

---

### 2. **Plik danych TSPLIB (`.txt`, `.atsp`, `.tsp`)**

Program obsługuje format **TSPLIB**. Przykład `br17.txt`:

```
NAME: br17
TYPE: ATSP
COMMENT: 17 city problem (Repetto)
DIMENSION: 17
EDGE_WEIGHT_TYPE: EXPLICIT
EDGE_WEIGHT_FORMAT: FULL_MATRIX
EDGE_WEIGHT_SECTION
9999 3 5 48 48 8 8 5 5 3 3 0 3 5 8 8 5
3 9999 3 48 48 8 8 5 5 0 0 3 0 3 8 8 5
...
EOF
```

**Obsługiwane formaty macierzy:**
- `FULL_MATRIX` - pełna macierz n×n
- `UPPER_ROW` - górny trójkąt bez przekątnej
- `LOWER_DIAG_ROW` - dolny trójkąt z przekątną

**Obsługiwane typy:**
- `TSP` - symetryczny problem komiwojażera
- `ATSP` - asymetryczny problem komiwojażera

**Opcjonalne sekcje:**
```
TOUR_SECTION
1
5
3
2
4
-1
EOF
```

---

### 3. **Plik wynikowy (`results.csv`)**

Przykładowa struktura CSV:

```csv
Instance,Method,Optimal_value,Optimal_tour,Found_value,Found_tour,Execution_time_ms,Absolute_error,Relative_error,Relative_error_percent,algorithm,start_city,Average_time_ms
br17,Nearest Neighbour,39.00,"0-1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16",55.00,"0-2-9-1-13-11-12-15-5-4-3-14-6-8-7-16-10",0.125,16.00,0.410256,41.03,nearest-neighbour,0,0.134
br17,Nearest Neighbour,39.00,"0-1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16",52.00,"0-3-14-6-15-5-4-8-7-16-10-9-2-1-13-12-11",0.118,13.00,0.333333,33.33,nearest-neighbour,0,0.134
```

**Kolumny:**
- `Instance` - nazwa instancji
- `Method` - nazwa algorytmu
- `Optimal_value` - wartość optymalna
- `Optimal_tour` - trasa optymalna (jeśli znana)
- `Found_value` - wartość znaleziona
- `Found_tour` - trasa znaleziona
- `Execution_time_ms` - czas wykonania w milisekundach
- `Absolute_error` - błąd bezwzględny
- `Relative_error` - błąd względny (jako ułamek)
- `Relative_error_percent` - błąd względny w procentach
- Dodatkowe kolumny: parametry algorytmu
- `Average_time_ms` - średni czas dla serii powtórzeń

---

## 📊 Wyniki działania

### Przykładowy output na ekranie:

```
==================================================
   Travelling Salesman Problem Solver v2.0
   TSPLIB Format Support | C++23
==================================================

Loading configuration from: config.txt
Loading data from: br17.txt
Instance: br17
Type: ATSP
Comment: 17 city problem (Repetto)
Size: 17 cities
Optimal length: 39
Optimal tour: 0-1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16
Method: nearest-neighbour
Repetitions: 5

--------------------------------------------------
Starting solving...
--------------------------------------------------

Repetition 1/5
Found value: 55
Found tour: 0-2-9-1-13-11-12-15-5-4-3-14-6-8-7-16-10
Execution time: 0.125 ms
Relative error: 41.03%

Repetition 2/5
Found value: 52
Execution time: 0.118 ms
Relative error: 33.33%

...

==================================================
Summary:
Average execution time: 0.134 ms
==================================================

Results saved to file: results.csv

Program finished.
```

---

## 🔨 Rozszerzanie programu

### Dodawanie nowego algorytmu

Program został zaprojektowany modularnie. Aby dodać nowy algorytm:

#### 1. **Dodaj deklarację w `solvers.h`:**

```cpp
class MyNewSolver : public TSPSolver {
private:
    // Twoje parametry
    double my_parameter;

public:
    MyNewSolver(const TSPInstance& inst, double param, bool progress = false);
    
    std::string getName() const override;
    Result solve() override;
};
```

#### 2. **Zaimplementuj w `solvers.cpp`:**

```cpp
MyNewSolver::MyNewSolver(const TSPInstance& inst, double param, bool progress)
    : TSPSolver(inst, progress), my_parameter(param) {}

string MyNewSolver::getName() const { 
    return "My New Algorithm"; 
}

Result MyNewSolver::solve() {
    Result result;
    result.instance_name = instance.name;
    result.optimal_value = instance.optimal_length.value_or(0);
    result.method_name = getName();
    
    // Przepisz optymalną trasę jeśli istnieje
    if (instance.optimal_tour.has_value()) {
        result.optimal_tour = instance.optimal_tour.value();
    }
    
    // Dodaj parametry
    result.addParameter("algorithm", "my-new-algorithm");
    result.addParameter("my_parameter", my_parameter);
    
    auto start = chrono::high_resolution_clock::now();
    
    // =============================
    // TUTAJ TWÓJ ALGORYTM
    // =============================
    vector<int> tour = yourAlgorithmFunction();
    double length = instance.calculateTourLength(tour);
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    
    result.found_tour = tour;
    result.found_value = length;
    result.execution_time = elapsed.count();
    
    calculateErrors(result, instance.optimal_length.value_or(length));
    
    return result;
}
```

#### 3. **Dodaj obsługę w `main.cpp`:**

```cpp
} else if (config.method == "my-new-algorithm") {
    solver = new MyNewSolver(instance, config.my_parameter, config.show_progress);
```

#### 4. **Dodaj parametr do `config.h` i `config.cpp`:**

```cpp
// config.h
class Config {
public:
    // ... inne pola
    double my_parameter;
    
    Config();
};

// config.cpp - w readConfig():
} else if (key == "my_parameter") {
    config.my_parameter = stod(value);
```

#### 5. **Zaktualizuj `config.txt`:**

```ini
method = my-new-algorithm
my_parameter = 42.0
```

---

## 📈 Analiza wyników

### Interpretacja błędów:

**Błąd bezwzględny:**
```
Absolute Error = |Found Value - Optimal Value|
```

**Błąd względny:**
```
Relative Error = Absolute Error / Optimal Value
Relative Error % = Relative Error × 100%
```

### Typowe wyniki dla algorytmów:

| Algorytm | Złożoność | Błąd średni | Czas (17 miast) |
|----------|-----------|-------------|-----------------|
| Brute Force | O(n!) | 0% | ~3-5 sekund |
| Nearest Neighbour | O(n²) | 20-50% | <1 ms |
| Random (100k iter) | O(k×n) | 15-40% | ~150 ms |

---

## 🐛 Rozwiązywanie problem