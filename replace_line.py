import sys

def replace_line_in_file(file_name, line_number, new_line):
    try:
        # Открываем файл для чтения
        with open(file_name, 'rb') as file:
            lines = file.readlines()

        # Проверяем, существует ли указанная строка
        if line_number <= 0 or line_number > len(lines):
            print(f"Ошибка: Строка {line_number} не существует в файле.")
            return

        # Определяем формат конца строки
        if lines[line_number - 1].endswith(b'\r\n'):
            newline_char = b'\r\n'
        elif lines[line_number - 1].endswith(b'\n'):
            newline_char = b'\n'
        else:
            newline_char = b''

        # Заменяем указанную строку
        lines[line_number - 1] = new_line.encode() + newline_char

        # Открываем файл для записи и сохраняем изменения
        with open(file_name, 'wb') as file:
            file.writelines(lines)

        print(f"Строка {line_number} успешно заменена на '{new_line}' в файле '{file_name}'.")

    except FileNotFoundError:
        print(f"Ошибка: Файл '{file_name}' не найден.")
    except Exception as e:
        print(f"Ошибка: {e}")

if __name__ == "__main__":
    if len(sys.argv) < 4:
        print("Использование: python replace_line.py <имя_файла> <номер_строки> <новая_строка>")
    else:
        file_name = sys.argv[1]
        try:
            line_number = int(sys.argv[2])
        except ValueError:
            print("Ошибка: Номер строки должен быть целым числом.")
            sys.exit(1)
        new_line = ''
        for i in range(3, len(sys.argv)):
            new_line += sys.argv[i] + ' '
        replace_line_in_file(file_name, line_number, new_line)
