# nameless-database
Комменты на английском, здесь на русском. Для себя чисто.

## План
Мечты обширны и глобальны, масштабы - наполеоновские.
- [ ] Реализовать систему динамических типов в контексте столбца таблицы. Лишь я пойму, думаю
  Поддерживаемые типы:
  - bool
  - uint8_t
  - int8_t
  - uint16_t
  - int16_t
  - uint32_t
  - int32_t
  - uint64_t
  - int64_t
  - float
  - double
  - string
  - Возможно, будут еще какие-то. BigInt, например
  
  Типы должны взаимодействовать между собой - к примеру, к числу 5 типа uint16_t можно добавить -5 типа int8_t и должно получиться 0. Можно добавить "1488" типа string и получить 1493 и вроде того
  
  - [x] Приравнивание
  - [x] Сложение
  - [x] Вычитание
  - [x] Умножение
  - [x] Деление
  - [ ] Сравнения
    - [ ] Равно
    - [ ] Не равно
    - [ ] Меньше
    - [ ] Меньше или равно
    - [ ] Больше
    - [ ] Больше или равно
  - [x] Взятие данных
  - [x] Взятие данных (с шаблонным каст-аргументом, который возвращает данные нужного типа)
  - [x] push_back
  - [x] set_type
  - [ ] set_data (сырыми данными, по индексу)
  - [ ] set_size
  - [x] Деструктор должен очищать за собой выделенную извне память. Строки, к примеру.
  - [x] reserve, по факту просто косвенный вызов std::vector.reserve(n)
  - [x] Каст данных от одного типа к другому
  - [ ] Флаги определяющие действия над типами и между типами
  - [ ] Флаги определяющие действия над столбцом
  - [x] Вывод данных (stdout)
  	- [x] Отдельный элемент
	- [x] Тип
	- [x] Все элементы
	- [x] Тип + все элементы
	
- [ ] Таблица базы данных. Пока-что приглядывается красно-черное дерево хранящее вектора. Подумаю об этом.
  - [ ] Создание нового ключа
  - [ ] Обращение по индексу и с помощью ключа
  - [ ] Сохранение базы данных на диске
  - [ ] Загрузка базы данных с диска в память
  - [ ] Флаги определяющие действия над таблицей
- [ ] Собственный скриптовый язык программирования для базы данных
  - [ ] И что я здесь должен писать? Я ж забью к этому моменту, или мне уже настанет 18 лет и я сопьюсь
- [ ] Графический интерфейс
