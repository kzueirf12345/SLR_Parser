# SLRParser

Это задание по курсу "Введение в тензорные компиляторы" 4 семестр. Разбор грамматики простого арифтметического языка методом shift/reduce.

Авторы: 
-   Попов Владимир, Б01-411

## Зависимости

| Зависимость           | Минимальная версия    | Назначение                                    |
|-----------------------|-----------------------|-----------------------------------------------|
| **CMake**             | 3.26                  | Сборка проекта и зависимостей                 |
| **GCC / Clang**       | GCC 10 / Clang 11     | Компиляция C++20 кода                         |
| **Flex**              | 2.6.4                 | Лексический анализ                            |

### Установка зависимостей (Ubuntu/Debian)

#### 1. Системные зависимости
```bash
sudo apt update
sudo apt install -y \
    build-essential \
    git \
    wget \
    snap
```

#### 2. CMake

```bash
sudo snap install cmake --classic
# Если уже есть cmake из apt, а он скорее всего слишком старый, то для использования нового из snap выполните
echo 'export PATH="/snap/bin:$PATH"' >> ~/.bashrc # или ~/.zshrc
source ~/.bashrc # или ~/.zshrc
```


#### 3. Flex

```bash
sudo apt install flex libfl-dev
```

## Сборка проекта

```bash
git clone https://github.com/kzueirf12345/SLR_Parser
cd SLR_Parser

cmake -B build -DCMAKE_BUILD_TYPE=Release # -DSANITIZE=ON включение санитайзеров
cmake --build build -j$(nproc)

# Запуск
./build/SLRParser
```

## Грамматика
Основаная грамматика

```bnf
<start>     ::= <sum>

<sum>       ::= <sum> "+" <mul> | <sum> "-" <mul> | <mul>

<mul>       ::= <mul> "*" <brakets> | <mul> "/" <brakets> | <brakets>

<brakets>   ::= "(" <sum> ")" | NUM | ID
```

Развёрнутая форма

```bnf
<start>     ::=* <sum>

<sum>       ::=* <sum> "+" <mul> 
<sum>       ::=* <sum> "-" <mul>
<sum>       ::=* <mul>

<mul>       ::=* <mul> "*" <brakets>
<mul>       ::=* <mul> "/" <brakets>
<mul>       ::=* <brakets>

<brakets>   ::=* "(" <sum> ")"
<brakets>   ::=* NUM
<brakets>   ::=* ID
```