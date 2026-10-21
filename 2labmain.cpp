#include <algorithm>
#include <clocale>
#include <cstddef>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

int ReadInt(const std::string& prompt) {
  while (true) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
      throw std::runtime_error("Ввод завершен.");
    }

    std::istringstream input(line);
    int value;
    std::string extra;
    if (input >> value && !(input >> extra)) {
      return value;
    }
    std::cout << "Ошибка: введите одно целое число.\n";
  }
}

int ReadIntMin(const std::string& prompt, int min_value) {
  int value = ReadInt(prompt);
  while (value < min_value) {
    std::cout << "Ошибка: значение должно быть не меньше "
              << min_value
              << ".\n";
    value = ReadInt(prompt);
  }
  return value;
}

int ReadEvenCount(const std::string& prompt, int min_value) {
  int count = ReadIntMin(prompt, min_value);
  while (count % 2 != 0) {
    std::cout << "Ошибка: количество элементов должно быть "
                 "четным.\n";
    count = ReadIntMin(prompt, min_value);
  }
  return count;
}

std::string ReadToken(const std::string& prompt) {
  while (true) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
      throw std::runtime_error("Ввод завершен.");
    }

    std::istringstream input(line);
    std::string token;
    std::string extra;
    if (input >> token && !(input >> extra)) {
      return token;
    }
    std::cout << "Ошибка: введите одно слово или имя файла "
                 "без пробелов.\n";
  }
}

std::vector<int> ReadIntValues(int count, const std::string& name) {
  std::vector<int> values;
  values.reserve(static_cast<std::size_t>(count));
  for (int i = 0; i < count; ++i) {
    values.push_back(
        ReadInt(name + ", элемент " + std::to_string(i + 1) + ": "));
  }
  return values;
}

template <typename Iterator>
void PrintRange(Iterator first, Iterator last) {
  bool first_element = true;
  for (; first != last; ++first) {
    if (!first_element) {
      std::cout << ' ';
    }
    std::cout << *first;
    first_element = false;
  }
  if (first_element) {
    std::cout << "(пусто)";
  }
  std::cout << '\n';
}

// Задание 1
std::vector<int> ReorderVectorHalves(const std::vector<int>& values) {
  const std::vector<int>::const_iterator middle =
      values.begin() + static_cast<std::vector<int>::difference_type>(
                           values.size() / 2);
  std::vector<int> result;
  std::copy(middle, values.end(), std::back_inserter(result));
  std::copy(values.begin(), middle, std::back_inserter(result));
  return result;
}

// Задание 2
std::list<int> InsertFirstFiveReversed(const std::vector<int>& values,
                                       std::list<int> result) {
  std::list<int>::iterator position = result.begin();
  std::advance(position, 5);
  result.insert(position, values.rbegin(), values.rbegin() + 5);
  return result;
}

// Задание 3
void SwapListHalves(std::list<int>& list1, std::list<int>& list2) {
  std::list<int>::iterator first1 = list1.begin();
  std::list<int>::iterator middle1 = list1.begin();
  std::advance(middle1, static_cast<std::ptrdiff_t>(list1.size() / 2));
  std::list<int>::iterator middle2 = list2.begin();
  std::advance(middle2, static_cast<std::ptrdiff_t>(list2.size() / 2));
  const std::list<int>::iterator end2 = list2.end();

  list1.splice(list1.begin(), list2, middle2, end2);
  list2.splice(list2.end(), list1, first1, middle1);
}

// Задание 4
std::filesystem::path ResolveFilePath(
    const std::string& file_name,
    const std::filesystem::path& program_directory) {
  const std::filesystem::path path(file_name);
  if (path.is_absolute()) {
    return path;
  }
  return program_directory / path;
}

bool AreSameFile(const std::filesystem::path& first,
                 const std::filesystem::path& second) {
  if (first.lexically_normal() == second.lexically_normal()) {
    return true;
  }

  std::error_code error;
  if (!std::filesystem::exists(second, error)) {
    if (error) {
      throw std::runtime_error("Не удалось проверить результирующий файл.");
    }
    return false;
  }
  const bool same_file = std::filesystem::equivalent(first, second, error);
  if (error) {
    throw std::runtime_error("Не удалось сравнить исходный и результирующий "
                             "файлы.");
  }
  return same_file;
}

void FilterWordsToFile(int max_length,
                       const std::filesystem::path& source_path,
                       const std::filesystem::path& destination_path) {
  std::ifstream source(source_path);
  if (!source) {
    throw std::runtime_error("Не удалось открыть исходный файл: " +
                             source_path.string());
  }
  std::ofstream destination(destination_path);
  if (!destination) {
    throw std::runtime_error("Не удалось открыть результирующий файл: " +
                             destination_path.string());
  }

  std::remove_copy_if(
      std::istream_iterator<std::string>(source),
      std::istream_iterator<std::string>(),
      std::ostream_iterator<std::string>(destination, "\n"),
      [max_length](const std::string& word) {
        return word.size() > static_cast<std::size_t>(max_length);
      });
  if (!source.eof() && source.fail()) {
    throw std::runtime_error("Ошибка чтения исходного файла.");
  }
  destination.close();
  if (!destination) {
    throw std::runtime_error("Ошибка записи результирующего файла.");
  }
}

// Задание 5
bool RemoveLastZero(std::deque<int>& values) {
  const std::deque<int>::reverse_iterator zero =
      std::find(values.rbegin(), values.rend(), 0);
  if (zero != values.rend()) {
    values.erase(--zero.base());
    return true;
  }
  return false;
}

// Задание 6
void AddValuesToVectors(int a, int b, std::vector<int>& vector1,
                        std::vector<int>& vector2) {
  std::fill_n(std::inserter(vector1, vector1.begin()), 5, a);
  std::fill_n(std::back_inserter(vector1), 5, b);
  vector2.insert(vector2.begin(), 5, a);
  vector2.insert(vector2.end(), 5, b);
}

// Задание 7
void SortSecondHalf(std::vector<int>& values) {
  const std::vector<int>::iterator middle =
      values.begin() + static_cast<std::vector<int>::difference_type>(
                           values.size() / 2);
  std::sort(middle, values.end());
}

void MergeSortedHalves(std::vector<int>& values) {
  const std::vector<int>::iterator middle =
      values.begin() + static_cast<std::vector<int>::difference_type>(
                           values.size() / 2);
  std::inplace_merge(values.begin(), middle, values.end());
}

// Задание 8
std::vector<double> CalculateAdjacentAverages(const std::list<int>& values) {
  std::vector<double> averages;
  std::adjacent_difference(
      values.begin(), values.end(),
      std::inserter(averages, averages.begin()),
      [](int current, int previous) {
        return (static_cast<double>(current) +
                static_cast<double>(previous)) /
               2.0;
      });
  if (!averages.empty()) {
    averages.erase(averages.begin());
  }
  return averages;
}

// Задание 9
int CountVectorsContainingSet(
    const std::set<int>& base, const std::vector<std::set<int>>& candidates) {
  int matching_count = 0;
  for (std::vector<std::set<int>>::const_iterator candidate =
           candidates.begin();
       candidate != candidates.end(); ++candidate) {
    if (std::includes(candidate->begin(), candidate->end(), base.begin(),
                      base.end())) {
      ++matching_count;
    }
  }
  return matching_count;
}

bool IsUppercaseEnglishWord(const std::string& word) {
  return !word.empty() &&
         std::all_of(word.begin(), word.end(), [](char letter) {
           return letter >= 'A' && letter <= 'Z';
         });
}

std::string ReadUppercaseWord(const std::string& prompt) {
  while (true) {
    const std::string word = ReadToken(prompt);
    if (IsUppercaseEnglishWord(word)) {
      return word;
    }
    std::cout << "Ошибка: используйте только заглавные английские "
                 "буквы.\n";
  }
}

// Задание 10
std::map<char, std::size_t> SumWordLengthsByFirstLetter(
    const std::vector<std::string>& words) {
  std::map<char, std::size_t> lengths;
  for (std::vector<std::string>::const_iterator word = words.begin();
       word != words.end(); ++word) {
    lengths[(*word)[0]] += (*word).size();
  }
  return lengths;
}

// Интерфейс задания 1.
void RunTask1() {
  std::cout << "\nЗадание 1. Перестановка половин вектора\n";
  const int count =
      ReadEvenCount("Введите четное количество чисел (>= 2): ", 2);
  const std::vector<int> input = ReadIntValues(count, "Число");
  const std::vector<int> values(input.begin(), input.end());
  const std::vector<int> result = ReorderVectorHalves(values);
  std::cout << "Результат: ";
  PrintRange(result.begin(), result.end());
}

// Интерфейс задания 2.
void RunTask2() {
  std::cout << "\nЗадание 2. Вставка элементов в список\n";
  const int vector_count =
      ReadIntMin("Количество элементов вектора (>= 5): ", 5);
  const std::vector<int> input_vector =
      ReadIntValues(vector_count, "Элемент вектора");
  const std::vector<int> values(input_vector.begin(), input_vector.end());
  const int list_count =
      ReadIntMin("Количество элементов списка (>= 5): ", 5);
  const std::vector<int> input_list =
      ReadIntValues(list_count, "Элемент списка");
  const std::list<int> initial_list(input_list.begin(), input_list.end());
  const std::list<int> result = InsertFirstFiveReversed(values, initial_list);
  std::cout << "Список после вставки: ";
  PrintRange(result.begin(), result.end());
}

// Интерфейс задания 3.
void RunTask3() {
  std::cout << "\nЗадание 3. Обмен половин списков\n";
  const int count1 =
      ReadEvenCount("Количество элементов L1 (четное, >= 2): ", 2);
  const std::vector<int> values1 = ReadIntValues(count1, "L1");
  std::list<int> list1(values1.begin(), values1.end());
  const int count2 =
      ReadEvenCount("Количество элементов L2 (четное, >= 2): ", 2);
  const std::vector<int> values2 = ReadIntValues(count2, "L2");
  std::list<int> list2(values2.begin(), values2.end());
  SwapListHalves(list1, list2);
  std::cout << "L1: ";
  PrintRange(list1.begin(), list1.end());
  std::cout << "L2: ";
  PrintRange(list2.begin(), list2.end());
}

// Интерфейс задания 4.
void RunTask4(const std::filesystem::path& program_directory) {
  std::cout << "\nЗадание 4. Фильтрация слов в файле\n";
  const int max_length = ReadIntMin("Введите K (> 0): ", 1);
  const std::string source_name =
      ReadToken("Имя исходного файла: ");
  std::string destination_name =
      ReadToken("Имя результирующего файла: ");
  const std::filesystem::path source_path =
      ResolveFilePath(source_name, program_directory);
  std::ifstream source_check(source_path);
  if (!source_check) {
    throw std::runtime_error(
        "Исходный файл не найден или не открывается: " +
        source_path.string());
  }

  std::filesystem::path destination_path =
      ResolveFilePath(destination_name, program_directory);
  while (AreSameFile(source_path, destination_path)) {
    std::cout << "Ошибка: имена исходного и результирующего файлов "
                 "должны различаться.\n";
    destination_name =
        ReadToken("Введите другое имя результирующего файла: ");
    destination_path = ResolveFilePath(destination_name, program_directory);
  }
  FilterWordsToFile(max_length, source_path, destination_path);
  std::cout << "Подходящие слова записаны в файл " << destination_name
            << ".\n";
}

// Интерфейс задания 5.
void RunTask5() {
  std::cout << "\nЗадание 5. Удаление последнего нуля дека\n";
  const int count = ReadIntMin("Количество элементов дека (>= 0): ", 0);
  const std::vector<int> input =
      ReadIntValues(count, "Элемент дека");
  std::deque<int> values(input.begin(), input.end());
  if (RemoveLastZero(values)) {
    std::cout << "Последний нулевой элемент удален.\n";
  } else {
    std::cout << "Нулевых элементов нет, дек не изменен.\n";
  }
  std::cout << "Дек: ";
  PrintRange(values.begin(), values.end());
}

// Интерфейс задания 6.
void RunTask6() {
  std::cout << "\nЗадание 6. Добавление значений в векторы\n";
  const int a = ReadInt("Введите A: ");
  const int b = ReadInt("Введите B: ");
  const int count1 =
      ReadIntMin("Количество элементов V1 (>= 0): ", 0);
  const std::vector<int> input1 = ReadIntValues(count1, "V1");
  const int count2 =
      ReadIntMin("Количество элементов V2 (>= 0): ", 0);
  const std::vector<int> input2 = ReadIntValues(count2, "V2");
  std::vector<int> vector1(input1.begin(), input1.end());
  std::vector<int> vector2(input2.begin(), input2.end());
  AddValuesToVectors(a, b, vector1, vector2);
  std::cout << "V1: ";
  PrintRange(vector1.begin(), vector1.end());
  std::cout << "V2: ";
  PrintRange(vector2.begin(), vector2.end());
}

// Интерфейс задания 7.
void RunTask7() {
  std::cout << "\nЗадание 7. Сортировка и слияние половин вектора\n";
  std::vector<int> values;
  while (true) {
    const int count = ReadEvenCount(
        "Количество элементов вектора (четное, >= 2): ", 2);
    const std::vector<int> input =
        ReadIntValues(count, "Элемент вектора");
    values = std::vector<int>(input.begin(), input.end());
    if (std::is_sorted(values.begin(), values.begin() + count / 2)) {
      break;
    }
    std::cout << "Первая половина не отсортирована по возрастанию. "
                 "Введите вектор заново.\n";
  }
  SortSecondHalf(values);
  std::cout << "После sort: ";
  PrintRange(values.begin(), values.end());
  MergeSortedHalves(values);
  std::cout << "После inplace_merge: ";
  PrintRange(values.begin(), values.end());
}

// Интерфейс задания 8.
void RunTask8() {
  std::cout << "\nЗадание 8. Средние арифметические соседних элементов\n";
  const int count =
      ReadIntMin("Количество элементов списка L (>= 1): ", 1);
  const std::vector<int> input = ReadIntValues(count, "L");
  const std::list<int> values(input.begin(), input.end());
  const std::vector<double> averages = CalculateAdjacentAverages(values);
  std::cout << "Вектор средних: ";
  const std::ios::fmtflags old_flags = std::cout.flags();
  const std::streamsize old_precision = std::cout.precision();
  std::cout << std::fixed << std::setprecision(1);
  PrintRange(averages.begin(), averages.end());
  std::cout.flags(old_flags);
  std::cout.precision(old_precision);
}

// Интерфейс задания 9.
void RunTask9() {
  std::cout << "\nЗадание 9. Поиск векторов, содержащих элементы V0\n";
  const int base_count =
      ReadIntMin("Количество элементов V0 (>= 0): ", 0);
  const std::vector<int> input_base = ReadIntValues(base_count, "V0");
  const std::set<int> base(input_base.begin(), input_base.end());
  const int vector_count =
      ReadIntMin("Количество векторов N (> 0): ", 1);
  std::vector<std::set<int>> candidates;
  candidates.reserve(static_cast<std::size_t>(vector_count));
  for (int i = 0; i < vector_count; ++i) {
    const int count = ReadIntMin(
        "Количество элементов V" + std::to_string(i + 1) +
            " (не меньше " + std::to_string(base_count) + "): ",
        base_count);
    const std::vector<int> input =
        ReadIntValues(count, "V" + std::to_string(i + 1));
    candidates.push_back(std::set<int>(input.begin(), input.end()));
  }
  std::cout << "Количество подходящих векторов: "
            << CountVectorsContainingSet(base, candidates) << '\n';
}

// Интерфейс задания 10.
void RunTask10() {
  std::cout << "\nЗадание 10. Суммарная длина слов по первой букве\n";
  const int count = ReadIntMin("Количество слов (>= 1): ", 1);
  std::vector<std::string> input_words;
  input_words.reserve(static_cast<std::size_t>(count));
  for (int i = 0; i < count; ++i) {
    input_words.push_back(
        ReadUppercaseWord("Слово " + std::to_string(i + 1) + ": "));
  }
  const std::vector<std::string> words(input_words.begin(), input_words.end());
  const std::map<char, std::size_t> lengths =
      SumWordLengthsByFirstLetter(words);
  std::cout << "Буква и суммарная длина:\n";
  for (std::map<char, std::size_t>::const_iterator entry = lengths.begin();
       entry != lengths.end(); ++entry) {
    std::cout << entry->first << entry->second << '\n';
  }
}

void RunSet(int set, const std::filesystem::path& program_directory) {
  try {
    switch (set) {
      case 1:
        RunTask1();
        break;
      case 2:
        RunTask2();
        break;
      case 3:
        RunTask3();
        break;
      case 4:
        RunTask4(program_directory);
        break;
      case 5:
        RunTask5();
        break;
      case 6:
        RunTask6();
        break;
      case 7:
        RunTask7();
        break;
      case 8:
        RunTask8();
        break;
      case 9:
        RunTask9();
        break;
      case 10:
        RunTask10();
        break;
    }
  } catch (const std::exception& error) {
    std::cerr << "Ошибка: " << error.what() << '\n';
  }
}

int main(int argc, char* argv[]) {
  std::setlocale(LC_ALL, "Russian");
  try {
    if (argc < 1 || argv[0] == nullptr) {
      throw std::runtime_error("Не удалось определить путь к программе.");
    }
    std::error_code path_error;
    const std::filesystem::path program_path =
        std::filesystem::absolute(argv[0], path_error);
    if (path_error) {
      throw std::runtime_error("Не удалось определить папку программы.");
    }
    const std::filesystem::path program_directory = program_path.parent_path();

    while (true) {
      std::cout << "\nЛабораторная работа № 2, вариант 2\n"
                   "1. Задание 1\n"
                   "2. Задание 2\n"
                   "3. Задание 3\n"
                   "4. Задание 4\n"
                   "5. Задание 5\n"
                   "6. Задание 6\n"
                   "7. Задание 7\n"
                   "8. Задание 8\n"
                   "9. Задание 9\n"
                   "10. Задание 10\n"
                   "0. Выход\n";
      const int set = ReadInt("Выберите задание (0-10): ");
      if (set == 0) {
        std::cout << "До свидания!\n";
        return 0;
      }
      if (set < 1 || set > 10) {
        std::cout << "Такого задания нет. Выберите от 0 до 10.\n";
        continue;
      }
      RunSet(set, program_directory);
    }
  } catch (const std::exception& error) {
    std::cerr << "Работа программы завершена: " << error.what() << '\n';
    return 1;
  }
}
