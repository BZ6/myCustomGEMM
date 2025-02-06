import subprocess
import os

def run_command(command, output_file):
    """Выполняет команду и записывает вывод в файл."""
    result = subprocess.run(command, shell=True, check=True, text=True, capture_output=True)

    # Записываем стандартный вывод и стандартный поток ошибок в файл
    with open(output_file, 'a') as file:
        file.write(f'Command: {command}\n')
        file.write(f'Standard Output:\n{result.stdout}\n')
        if result.stderr:
            file.write(f'Standard Error:\n{result.stderr}\n')

    return result

def run_scenarios(scenarios, output_file):
    """Выполняет сценарии по очереди и записывает вывод в файл."""
    for scenario in scenarios:
        print(f'Выполнение сценария: {scenario}')
        run_command(scenario, output_file)

def cartesian_product_concat(str, set1):
    result = []
    for s1 in set1:
        result.append(str + s1)
    return result

def all(kernel_replaces, work_groups_replaces, scenarios):
    result = []
    for s1 in kernel_replaces:
        for s2 in work_groups_replaces:
            scenarios_to_run.append(s1)
            scenarios_to_run.append(s2)
            for s3 in scenarios:
                scenarios_to_run.append(s3)
    return result

if __name__ == "__main__":
    output_file = 'AMD_research_8.csv'  # Файл для записи вывода

    # Рабочие kernels: 1, 2, 4, 10
    # kernels = [
    #     '1"',
    #     '2"',
    #     '4"',
    #     '10"'
    # ]
    # work_groups = [
    #     '2"',
    #     '4"',
    #     '8"',
    #     '16"',
    #     '32"'  
    # ]

    kernels = [
        '2"'
    ]
    work_groups = [
        '2"',
        '4"',
        '8"',
        '16"',
        '32"'  
    ]

    replaces = [
        'python replace_line.py .\\src\\settings.h 16 "#define KERNEL ',
        'python replace_line.py .\\src\\settings.h 20 "#define TS '
    ]
    scenarios = [
        'cmake --build .\\build',
        '.\\myGEMM.exe'
    ]

    # Сценарии для выполнения
    kernel_replaces = cartesian_product_concat(replaces[0], kernels)
    work_groups_replaces = cartesian_product_concat(replaces[1], work_groups)

    scenarios_to_run = []

    scenarios_to_run.append(all(kernel_replaces, work_groups_replaces, scenarios))

    # scenarios_to_run.append(all(kernel_replaces[0:2], work_groups_replaces[0:4], scenarios))
    # scenarios_to_run.append(all(kernel_replaces[2:], work_groups_replaces[1:], scenarios))

    # Выполнение сценариев
    run_scenarios(scenarios_to_run, output_file)
