# nameless-database
Комменты на английском, здесь на русском. Для себя чисто.

## План
Мечты обширны и глобальны, масштабы - наполеоновские.
- [x] Реализовать систему динамических типов в контексте столбца таблицы. Лишь я пойму, думаю
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
  
  - [x] Приравнивание
  - [x] Сложение
  - [x] Вычитание
  - [x] Умножение
  - [x] Деление
  - [x] Сравнения
    - [x] Равно
    - [x] Не равно
    - [x] Меньше
    - [x] Меньше или равно
    - [x] Больше
    - [x] Больше или равно
  - [x] Взятие данных
  - [x] Взятие данных (с шаблонным каст-аргументом, который возвращает данные нужного типа)
  - [x] push_back
  - [x] set_type
  - [x] set_data (данными с типом, по индексу)
  - [x] resize
  - [x] get_size
  - [x] get_type
  - [x] Деструктор должен очищать за собой выделенную извне память. Строки, к примеру.
  - [x] reserve, по факту просто косвенный вызов std::vector.reserve(n)
  - [x] Каст данных от одного типа к другому
  - [x] Флаги определяющие действия над типами и между типами
	
- [ ] Таблица базы данных
  - [x] Создание нового ключа
  - [x] Обращение к столбцу по индексу
  - [ ] Сохранение базы данных на диске
  - [ ] Загрузка базы данных с диска в память
- [ ] Собственный скриптовый язык программирования для базы данных
  - [ ] И что я здесь должен писать? Я ж забью к этому моменту, или мне уже настанет 18 лет и я сопьюсь
- [ ] Графический интерфейс
